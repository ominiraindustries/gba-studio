#!/usr/bin/env python3
"""Generate the Butano API reference from the pinned Butano headers.

Reads every engine/butano/butano/include/bn_*.h doc comment (Doxygen style) and writes:
  docs/api/butano/index.jsonl     one line per documented symbol
                                  {symbol, kind, header, line, group, brief, signature, params, returns, notes}
  docs/api/butano/<group>.md      per-module Markdown (group = @ingroup of the header's @file block)
  docs/api/butano/README.md       version, module list, how to regenerate
  docs/api/butano/manual/*.md     Butano's own manual pages (include/documentation/*.h) as Markdown

Rerun after updating Butano:  python -I .claude/scripts/gen_butano_api.py [--butano PATH]
No external dependencies. The parser is a light C++ scanner (comments, strings, braces, preprocessor), not a compiler:
it is good enough for Butano's consistent header style.
"""
import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_BUTANO = ROOT / "engine" / "butano"
OUT = ROOT / "docs" / "api" / "butano"


# ---------------------------------------------------------------- doc comments
def clean_comment(raw):
    """'/** ... */' or '///' block -> list of text lines without the comment decoration."""
    body = raw
    if body.startswith("/**"):
        body = body[3:]
        if body.endswith("*/"):
            body = body[:-2]
        lines = []
        for ln in body.split("\n"):
            s = ln.strip()
            if s.startswith("*"):
                s = s[1:]
                if s.startswith(" "):
                    s = s[1:]
            lines.append(s.rstrip())
    else:
        lines = [re.sub(r"^\s*///<?\s?", "", ln).rstrip() for ln in body.split("\n")]
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    return lines


def parse_doc(lines):
    """Split Doxygen tags: brief, params, tparams, returns, ingroup, notes (other text, @warning, @code...)."""
    d = {"brief": "", "params": [], "tparams": [], "returns": "", "ingroup": "", "notes": [], "def": "",
         "file": False}
    cur = None  # ("brief"|"param"|"tparam"|"returns"|"notes", ref)
    in_code = False
    for ln in lines:
        s = ln.strip()
        if in_code:
            d["notes"].append(ln)
            if s.startswith("@endcode"):
                d["notes"][-1] = "```"
                in_code = False
            continue
        if s.startswith("@code"):
            d["notes"].append("```cpp")
            in_code = True
            cur = None
            continue
        m = re.match(r"@(\w+)\s*(.*)", s)
        if m:
            tag, rest = m.group(1), m.group(2)
            if tag == "brief":
                d["brief"] = rest
                cur = ("brief", None)
            elif tag in ("param", "tparam"):
                pm = re.match(r"(?:\[[a-z,]+\]\s*)?(\S+)\s*(.*)", rest)
                ent = [pm.group(1), pm.group(2)] if pm else [rest, ""]
                d[tag + "s"].append(ent)
                cur = (tag, ent)
            elif tag in ("return", "returns"):
                d["returns"] = rest
                cur = ("returns", None)
            elif tag == "ingroup":
                d["ingroup"] = rest.split()[0] if rest else ""
                cur = None
            elif tag == "file":
                d["file"] = True
                cur = None
            elif tag == "def":
                d["def"] = rest
                cur = None
            elif tag in ("cond", "endcond"):
                cur = None
            else:  # warning, note, see, ...
                d["notes"].append(f"**{tag}**: {rest}")
                cur = ("notes", None)
            continue
        if not s:
            if cur and cur[0] == "brief":
                cur = ("notes", None)
            elif cur and cur[0] == "notes" and d["notes"] and d["notes"][-1] != "":
                d["notes"].append("")
            cur = ("notes", None) if cur else None
            continue
        if cur is None:
            if not d["brief"]:
                d["brief"] = s
                cur = ("brief", None)
            else:
                d["notes"].append(s)
                cur = ("notes", None)
        elif cur[0] == "brief":
            d["brief"] += " " + s
        elif cur[0] in ("param", "tparam"):
            cur[1][1] += " " + s
        elif cur[0] == "returns":
            d["returns"] += " " + s
        else:
            d["notes"].append(s)
    while d["notes"] and not d["notes"][-1]:
        d["notes"].pop()
    return d


# ---------------------------------------------------------------- C++ scanning
def strip_template(s):
    """Remove leading 'template<...>' clauses (balanced angle brackets)."""
    while True:
        m = re.match(r"\s*template\s*<", s)
        if not m:
            return s
        i, depth = m.end(), 1
        while i < len(s) and depth:
            if s[i] == "<":
                depth += 1
            elif s[i] == ">":
                depth -= 1
            i += 1
        s = s[i:]


def norm_ws(s):
    return re.sub(r"\s+", " ", s).strip()


ACCESS = re.compile(r"^\s*(?:(?:public|private|protected)\s*:\s*)+")


def classify(stmt, scope_kind):
    """Return (kind, name) for a declaration statement, or None."""
    s = ACCESS.sub("", stmt).strip()
    s = re.sub(r"\[\[[^\]]*\]\]\s*", "", s)
    if scope_kind == "enum":
        m = re.match(r"(\w+)", s)
        return ("enumerator", m.group(1)) if m else None
    m = re.match(r"namespace\s+([\w:]+)", s)
    if m:
        return ("namespace", m.group(1))
    body = strip_template(s).strip()
    body = re.sub(r"^(?:friend|static|inline|constexpr|consteval|explicit|virtual|extern|BN_CODE_IWRAM|BN_CODE_EWRAM"
                  r"|\s)+", "", body)
    m = re.match(r"enum\s+(?:class|struct)?\s*(\w+)", body)
    if m:
        return ("enum", m.group(1))
    m = re.match(r"(class|struct|union)\s+(?:\w+\s+)?(\w+)(?:\s+final)?\s*(?::|$)", body)
    if m and "(" not in body.split(":")[0]:
        return ("struct" if m.group(1) == "struct" else "class", m.group(2))
    m = re.match(r"using\s+(\w+)\s*=", body)
    if m:
        return ("alias", m.group(1))
    if "(" in body:
        head = body.split("(")[0]
        if re.search(r"operator\s*$", head):  # operator()(...)
            return ("function", "operator()")
        m = re.search(r"(operator\s*(?:\w+|[^\w\s(]+)|~?\w+)\s*$", head)
        if m and not re.match(r"(?:return|if|while|for|switch)$", m.group(1)):
            name = re.sub(r"\s+", "", m.group(1)) if m.group(1).startswith("operator") else m.group(1)
            if name.startswith("operator") and re.match(r"operator\w", name) and name not in ("operator", ):
                name = name.replace("operator", "operator ", 1)
            return ("function", name)
        return None
    m = re.search(r"(\w+)\s*(?:=|\{|\[|$)", body)
    if m:
        return ("variable", m.group(1))
    return None


def scan_header(path):
    """Yield (line, scope list, stmt text, doc dict, terminator) for each documented declaration."""
    src = path.read_text(encoding="utf-8", errors="replace")
    n = len(src)
    i = 0
    line = 1
    scope = []  # list of (kind, name) kind in namespace|class|enum|other
    stmt = []
    stmt_line = None
    pending = None  # (doc dict, line)
    hidden = 0  # inside @cond ... @endcond
    out = []
    file_doc = None

    def emit(term):
        nonlocal pending, stmt, stmt_line
        text = "".join(stmt)
        if pending and not hidden and text.strip():
            out.append((stmt_line or pending[1], list(scope), text, pending[0], term))
            pending = None
        elif pending and term in ";,}":
            pending = None
        stmt = []
        stmt_line = None
        return text

    at_line_start = True
    while i < n:
        c = src[i]
        if src.startswith("/**", i) and not src.startswith("/**/", i):
            j = src.find("*/", i + 3)
            j = n if j < 0 else j + 2
            raw = src[i:j]
            d = parse_doc(clean_comment(raw))
            if d["file"]:
                file_doc = d
            elif not hidden:
                pending = (d, line)
                stmt, stmt_line = [], None
            line += raw.count("\n")
            i = j
            continue
        if src.startswith("//", i):
            j = src.find("\n", i)
            j = n if j < 0 else j
            com = src[i:j]
            if "@cond" in com:
                hidden += 1
            elif "@endcond" in com:
                hidden = max(0, hidden - 1)
            elif com.startswith("///") and not hidden:
                pending = (parse_doc(clean_comment(com)), line)
            i = j
            continue
        if src.startswith("/*", i):
            j = src.find("*/", i + 2)
            j = n if j < 0 else j + 2
            line += src[i:j].count("\n")
            i = j
            continue
        if c == "#" and at_line_start:
            j = i
            while True:
                k = src.find("\n", j)
                if k < 0:
                    k = n
                    break
                if src[k - 1] == "\\":
                    j = k + 1
                    continue
                break
            directive = src[i:k]
            m = re.match(r"#\s*define\s+(\w+)(\([^)]*\))?(.*)", directive.replace("\\\n", " "), re.S)
            if m and pending and not hidden:
                out.append((line, [("macro", "")], norm_ws(directive.replace("\\\n", " ")), pending[0], "#"))
                pending = None
            line += directive.count("\n")
            i = k
            continue
        if c in "\"'":
            j = i + 1
            while j < n and src[j] != c:
                j += 2 if src[j] == "\\" else 1
            if c == '"' and i > 0 and src[i - 1] == "R":  # raw string: rare in headers, skip to )"
                j = src.find(')"', i)
                j = n if j < 0 else j + 1
            seg = src[i:j + 1]
            stmt.append(seg)
            line += seg.count("\n")
            i = j + 1
            continue
        if c == "\n":
            line += 1
            at_line_start = True
            stmt.append(" ")
            i += 1
            continue
        if not c.isspace():
            at_line_start = False
            if stmt_line is None:
                stmt_line = line
        top = scope[-1][0] if scope else "namespace"
        if c == "{":
            text = emit("{")
            cls = classify(text, top) if text.strip() else None
            if top == "other":
                scope.append(("other", ""))
            elif cls and cls[0] in ("namespace", "class", "struct", "enum") and not re.search(r"\)\s*(?:const)?\s*"
                                                                                                 r"(?:noexcept)?\s*$", text):
                scope.append((cls[0] if cls[0] != "struct" else "class", cls[1]))
            else:
                scope.append(("other", ""))
            i += 1
            continue
        if c == "}":
            if top == "enum":
                emit("}")
            else:
                stmt, stmt_line = [], None
            if scope:
                scope.pop()
            i += 1
            continue
        if c == ";":
            if top != "other":
                emit(";")
            else:
                stmt, stmt_line = [], None
            i += 1
            continue
        if c == "," and top == "enum":
            emit(",")
            i += 1
            continue
        if c == ":" and top == "class" and re.fullmatch(r"\s*(public|private|protected)\s*", "".join(stmt)):
            stmt, stmt_line = [], None
            i += 1
            continue
        if top != "other":
            stmt.append(c)
        i += 1
    return out, file_doc


def qualify(scope, name):
    parts = []
    for kind, nm in scope:
        if kind in ("namespace", "class", "enum") and nm:
            parts.append(nm)
    parts.append(name)
    return "::".join(p for p in parts if p)


def signature(stmt, kind):
    s = ACCESS.sub("", stmt)
    s = re.sub(r"\[\[nodiscard\]\]\s*", "", s)
    s = norm_ws(s)
    if kind == "function":
        # drop a trailing member-initialiser list / body remnants
        s = re.sub(r"\)\s*:\s*_\w+\(.*$", ")", s)
        s = re.sub(r"\s*=\s*(default|delete)$", r" = \1", s)
    if kind == "enumerator":
        s = s.rstrip(",")
    if len(s) > 400:
        s = s[:400] + " ..."
    return s


def enum_signature(path, line, sig):
    """Append the enumerators (with their //!< docs) to an enum signature."""
    lines = path.read_text(encoding="utf-8", errors="replace").split("\n")[line - 1:line + 120]
    body = "\n".join(lines)
    a = body.find("{")
    b = body.find("}", a)
    if a < 0 or b < 0:
        return sig
    items = []
    for ln in body[a + 1:b].split("\n"):
        m = re.match(r"\s*(\w+)(\s*=\s*[^,/]+)?,?\s*(?://!<\s*(.*))?", ln)
        if m and m.group(1):
            val = norm_ws(m.group(2) or "")
            doc = norm_ws((m.group(3) or "").replace("`", ""))
            items.append(f"{m.group(1)}{(' ' + val) if val else ''}{(' /* ' + doc + ' */') if doc else ''}")
    return f"{sig} {{ {', '.join(items)} }}" if items else sig


# ---------------------------------------------------------------- manual pages
def manual_to_md(path):
    src = path.read_text(encoding="utf-8", errors="replace")
    blocks = re.findall(r"/\*\*(.*?)\*/", src, re.S)
    out = []
    for b in blocks:
        lines = clean_comment("/**" + b + "*/")
        for ln in lines:
            s = ln.strip()
            m = re.match(r"@page\s+(\S+)\s+(.*)", s)
            if m:
                out += ["", f"# {m.group(2)}", ""]
                continue
            m = re.match(r"@(sub)*section\s+(\S+)\s+(.*)", s)
            if m:
                level = 2 + (len(re.match(r"@((?:sub)*)", s).group(1)) // 3)
                out += ["", "#" * level + " " + m.group(3), ""]
                continue
            m = re.match(r"@defgroup\s+(\S+)\s+(.*)", s)
            if m:
                out += ["", f"## {m.group(2)} (`{m.group(1)}`)", ""]
                continue
            if s.startswith("@code"):
                out.append("```cpp" if "cpp" in s or "{" in s or s == "@code" else "```")
                continue
            if s.startswith("@endcode"):
                out.append("```")
                continue
            if s.startswith("@ingroup"):
                out.append(f"(dentro de `{s.split()[1]}`)" if len(s.split()) > 1 else "")
                continue
            ln = re.sub(r'@ref\s+(\S+)\s+"([^"]+)"', r"\2", ln)
            ln = re.sub(r"@ref\s+(\S+)", r"\1", ln)
            ln = re.sub(r'<a href="([^"]+)">(.*?)</a>', r"[\2](\1)", ln)
            ln = re.sub(r"</?(?:b|i|em|strong|br|p|li|ul|ol)/?>", "", ln)
            out.append(ln)
    text = "\n".join(out).strip() + "\n"
    return re.sub(r"\n{3,}", "\n\n", text)


# ---------------------------------------------------------------- main
def butano_version(inc):
    v = inc / "bn_version.h"
    if v.exists():
        m = re.search(r'#define BN_VERSION_STRING "([^"]+)"', v.read_text(encoding="utf-8", errors="replace"))
        if m:
            return m.group(1)
    return "unknown"


def group_titles(inc):
    g = inc / "documentation" / "bn_documentation_groups.h"
    titles, parents = {}, {}
    if g.exists():
        for b in re.findall(r"/\*\*(.*?)\*/", g.read_text(encoding="utf-8", errors="replace"), re.S):
            m = re.search(r"@defgroup\s+(\S+)\s+(.*)", b)
            if m:
                titles[m.group(1)] = m.group(2).strip()
                p = re.search(r"@ingroup\s+(\S+)", b)
                if p:
                    parents[m.group(1)] = p.group(1)
    return titles, parents


def md_escape_cell(s):
    return s.replace("|", "\\|")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--butano", default=str(DEFAULT_BUTANO), help="Butano checkout (folder with butano/include)")
    ap.add_argument("--out", default=str(OUT))
    ap.add_argument("--changelog-lines", type=int, default=400, help="keep only the newest N changelog lines")
    a = ap.parse_args()
    base = Path(a.butano)
    inc = base / "butano" / "include"
    if not inc.is_dir():
        sys.exit(f"no Butano headers at {inc}")
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    version = butano_version(inc)
    titles, parents = group_titles(inc)

    entries = []
    headers = sorted(inc.glob("bn_*.h"))
    for h in headers:
        decls, fdoc = scan_header(h)
        group = (fdoc or {}).get("ingroup") or ""
        for line, scope, stmt, doc, term in decls:
            if any("_bn" == nm or nm.startswith("_") for _, nm in scope):
                continue
            if term == "#":
                m = re.match(r"#\s*define\s+(\w+)", stmt)
                kind, name = "macro", m.group(1) if m else (doc["def"] or "?")
                qual = name
            else:
                top = scope[-1][0] if scope else "namespace"
                cls = classify(stmt, top)
                if not cls:
                    continue
                kind, name = cls
                if kind in ("class", "struct", "enum", "namespace", "alias") and stmt.strip().endswith(";") is False \
                        and term == ";" and kind != "alias" and kind != "namespace":
                    pass
                qual = qualify(scope, name)
                if kind == "namespace":
                    qual = name
                if not qual.startswith("bn") and kind != "macro":
                    qual = qual  # global helpers (rare) keep their name
            if not doc["brief"] and not doc["params"]:
                continue
            e = {
                "symbol": qual,
                "kind": kind,
                "header": h.name,
                "line": line,
                "group": doc["ingroup"] or group or h.stem[3:],
                "brief": norm_ws(doc["brief"]),
                "signature": signature(stmt, kind),
            }
            if kind == "enum":
                e["signature"] = enum_signature(h, line, e["signature"])
            if doc["params"]:
                e["params"] = [[p, norm_ws(t)] for p, t in doc["params"]]
            if doc["tparams"]:
                e["tparams"] = [[p, norm_ws(t)] for p, t in doc["tparams"]]
            if doc["returns"]:
                e["returns"] = norm_ws(doc["returns"])
            notes = "\n".join(doc["notes"]).strip()
            if notes:
                e["notes"] = notes[:1500]
            entries.append(e)

    # file-level group per header (for Markdown placement)
    hdr_group = {}
    for e in entries:
        hdr_group.setdefault(e["header"], defaultdict(int))[e["group"]] += 1
    hdr_main = {h: max(c.items(), key=lambda kv: kv[1])[0] for h, c in hdr_group.items()}

    with open(out / "index.jsonl", "w", encoding="utf-8", newline="\n") as f:
        for e in entries:
            f.write(json.dumps(e, ensure_ascii=False) + "\n")

    # per-group Markdown
    by_group = defaultdict(lambda: defaultdict(list))
    for e in entries:
        by_group[hdr_main[e["header"]]][e["header"]].append(e)
    old = {p.name for p in out.glob("*.md") if p.name != "README.md"}
    written = set()
    for g, hdrs in sorted(by_group.items()):
        fn = f"{g}.md"
        written.add(fn)
        lines = [f"# Butano {version} — {titles.get(g, g)} (`{g}`)", "",
                 f"Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. "
                 f"No editar a mano.", ""]
        if g in parents:
            lines += [f"Módulo padre: [{parents[g]}]({parents[g]}.md)", ""]
        for hname, es in sorted(hdrs.items()):
            lines += [f"## {hname}", ""]
            for e in es:
                if e["kind"] in ("class", "struct", "namespace", "enum", "macro"):
                    lines += [f"### `{e['symbol']}` ({e['kind']}, {hname}:{e['line']})", "", e["brief"], ""]
                    if e["kind"] != "namespace":
                        lines += [f"`{e['signature']}`", ""]
                    if e.get("notes"):
                        lines += [e["notes"], ""]
                    continue
                short = e["symbol"].rsplit("::", 1)[-1]
                lines.append(f"- **{short}** `{e['signature']}` (L{e['line']}): {e['brief']}")
                extra = [f"`{p}` {t}" for p, t in e.get("tparams", []) + e.get("params", [])]
                if e.get("returns"):
                    extra.append(f"devuelve: {e['returns']}")
                if extra:
                    lines.append("  " + "; ".join(extra))
                if e.get("notes"):
                    lines.append("  " + e["notes"].replace("\n", "\n  "))
            lines.append("")
        (out / fn).write_text("\n".join(lines), encoding="utf-8", newline="\n")
    for stale in old - written:
        (out / stale).unlink()

    # manual pages
    man_out = out / "manual"
    man_out.mkdir(exist_ok=True)
    man_files = []
    for doc in sorted((inc / "documentation").glob("bn_documentation_*.h")):
        name = doc.stem.replace("bn_documentation_", "")
        name = re.sub(r"^[a-z]{1,2}_", "", name) if name not in ("groups", "main_page") else name
        md = manual_to_md(doc)
        if "changelog" in name:
            ls = md.split("\n")
            if len(ls) > a.changelog_lines:
                md = "\n".join(ls[:a.changelog_lines]) + \
                     f"\n\n(Recortado a las {a.changelog_lines} líneas más recientes; el completo está en " \
                     f"`engine/butano/butano/include/documentation/{doc.name}`.)\n"
        (man_out / f"{name}.md").write_text(md, encoding="utf-8", newline="\n")
        man_files.append((name, doc.name, len(md)))

    # README
    counts = defaultdict(int)
    for e in entries:
        counts[e["kind"]] += 1
    rd = [f"# Referencia de Butano {version}", "",
          "Generada desde los comentarios Doxygen de `engine/butano/butano/include/bn_*.h` por "
          "`python -I .claude/scripts/gen_butano_api.py`. Regenérala al actualizar Butano. No se edita a mano.", "",
          "Consulta rápida: `python -I .claude/scripts/kb.py symbol bn::sprite_ptr` o "
          "`kb.py query \"...\" --kind butano` (skill `gba-kb`).", "",
          f"- `index.jsonl`: {len(entries)} símbolos ("
          + ", ".join(f"{k} {v}" for k, v in sorted(counts.items(), key=lambda kv: -kv[1])) + ").",
          f"- {len(written)} módulos Markdown (agrupados por `@ingroup` del fichero).",
          "- `manual/`: páginas del manual de Butano (getting started, import de recursos, FAQ, ejemplos, "
          "changelog recortado).", "", "## Módulos", "", "| Módulo | Título | Símbolos |", "|---|---|---|"]
    gcount = {g: sum(len(v) for v in hdrs.values()) for g, hdrs in by_group.items()}
    for g in sorted(by_group):
        rd.append(f"| [{g}]({g}.md) | {md_escape_cell(titles.get(g, g))} | {gcount[g]} |")
    rd += ["", "## Manual", "", "| Página | Origen | Bytes |", "|---|---|---|"]
    for name, src, sz in man_files:
        rd.append(f"| [manual/{name}.md](manual/{name}.md) | `{src}` | {sz} |")
    (out / "README.md").write_text("\n".join(rd) + "\n", encoding="utf-8", newline="\n")
    print(f"Butano {version}: {len(entries)} symbols, {len(written)} modules, {len(man_files)} manual pages -> {out}")


if __name__ == "__main__":
    main()
