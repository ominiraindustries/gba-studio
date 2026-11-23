#!/usr/bin/env python3
"""kb.py: fast lookup over the GBA workspace knowledge (hardware, Butano API, guides, lessons, tasks).

Usage (always python -I, from any folder):
  kb.py query "OAM affine matrix" [--kind hw|butano|guide|lessons|tasks[,..]] [--limit N] [--full]
  kb.py symbol bn::sprite_ptr [--full]        exact Butano API entry (overloads, members, header:line)
  kb.py brief --role game-dev --task "..."    the few files/sections/APIs to read for a task
  kb.py stats | rebuild

Sources (kind):
  hw       docs/ref/gbatek-gba.txt (GBATEK, by chapter/register), docs/ref/gba-hw-resumen.md
  butano   docs/api/butano/index.jsonl (one chunk per symbol, overloads merged), docs/api/butano/manual/*.md
  guide    CLAUDE.md, docs/INDEX.md, docs/guias, docs/harness, docs/estudios, docs/ref/README.md, .claude/skills,
           .claude/agents,
           projects/*/README.md and DISENO.md
  lessons  docs/guias/lecciones*.md, docs/lecciones*.md
  tasks    docs/datos/tareas.jsonl
The index is cached in .claude/cache/kb_index.pickle and refreshed per file when a source changes (mtime/size).
Scoring: BM25 over folded, lightly stemmed tokens (identifiers also split on _ and ::), Spanish->English synonyms,
title/symbol boost. No external dependencies.
"""
import argparse
import json
import math
import pickle
import re
import sys
import time
import unicodedata
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CACHE = ROOT / ".claude" / "cache"
CACHE_FILE = CACHE / "kb_index.pickle"
INDEX_VERSION = 4
BUTANO_INC = "engine/butano/butano/include"

SOURCES = [  # (glob relative to ROOT, kind, chunker)
    ("docs/ref/gbatek-gba.txt", "hw", "gbatek"),
    ("docs/ref/gba-hw-resumen.md", "hw", "md"),
    ("docs/api/butano/index.jsonl", "butano", "butano_jsonl"),
    ("docs/api/butano/manual/*.md", "butano", "md"),
    ("docs/guias/lecciones*.md", "lessons", "md"),
    ("docs/lecciones*.md", "lessons", "md"),
    ("CLAUDE.md", "guide", "md"),
    ("docs/INDEX.md", "guide", "md"),
    ("docs/guias/*.md", "guide", "md"),
    ("docs/harness/*.md", "guide", "md"),
    ("docs/estudios/**/*.md", "guide", "md"),
    ("docs/ref/README.md", "guide", "md"),
    (".claude/skills/*/SKILL.md", "guide", "md"),
    (".claude/agents/*.md", "guide", "md"),
    ("projects/*/README.md", "guide", "md"),
    ("projects/*/DISENO.md", "guide", "md"),
    ("docs/datos/tareas.jsonl", "tasks", "tasks"),
]
KINDS = ("hw", "butano", "guide", "lessons", "tasks")
KIND_WEIGHT = {"hw": 1.0, "butano": 1.0, "guide": 1.15, "lessons": 1.0, "tasks": 0.9}

ROLES = {  # role -> always-read list (path, why)
    "game-dev": [("CLAUDE.md", "secciones 'Butano primero', 'Arte y audio', 'Verificación'"),
                 ("docs/guias/que-usar-butano.md", "necesidad -> API de Butano"),
                 (".claude/skills/gba-build/SKILL.md", "compilar"), (".claude/skills/gba-run/SKILL.md", "probar")],
    "engine-dev": [("docs/guias/que-usar-butano.md", "qué trae Butano y qué va a lib/ (gx::)"),
                   ("docs/harness/worktrees.md", "worktree propio"),
                   (".claude/skills/gba-performance/SKILL.md", "medir antes de optimizar")],
    "designer": [("docs/guias/diseno-juegos.md", "principios y checklist (cítalos)"),
                 (".claude/skills/gba-design/SKILL.md", "cómo entregar el diseño")],
    "artist": [(".claude/skills/gba-gfx/SKILL.md", "formatos, paletas, aseprite"),
               ("docs/ref/gba-hw-resumen.md", "sección 'Vídeo' y 'Sprites (OBJ)': límites")],
    "reviewer": [("docs/harness/flujo.md", "estados y evidencia"),
                 ("docs/guias/que-usar-butano.md", "¿duplica algo de Butano?")],
    "docs": [("docs/INDEX.md", "dónde va cada página")],
}

STOP = set("""a an and are as at be by for from has have how in into is it its of on or that the this to was what when
where which with el la los las un una unos unas de del al y o u en por para con sin se su sus que es son lo como más
mas pero si no ya hay muy le les me mi""".split())

SYN = {  # folded Spanish/alias -> English tokens (added to the query with lower weight)
    "guardar": "save sram", "guardado": "save sram backup", "partida": "save", "memoria": "memory",
    "fondo": "background bg", "fondos": "background bg", "sprite": "obj", "sprites": "obj", "obj": "sprite",
    "paleta": "palette", "paletas": "palette", "color": "palette", "colores": "color palette",
    "sonido": "sound audio", "musica": "music", "efecto": "effect", "efectos": "effect", "audio": "sound",
    "pantalla": "screen display", "linea": "line scanline", "lineas": "line scanline", "scanline": "line hblank",
    "temporizador": "timer", "temporizadores": "timer", "interrupcion": "interrupt irq", "irq": "interrupt",
    "interrupt": "irq", "teclado": "keypad", "botones": "keypad button", "boton": "keypad button", "mando": "keypad",
    "tecla": "key keypad", "mosaico": "mosaic", "ventana": "window", "ventanas": "window",
    "transparencia": "blending alpha", "mezcla": "blending", "brillo": "brightness fade", "fundido": "fade",
    "rotacion": "rotation affine", "escala": "scale affine", "escalado": "scaling affine", "afin": "affine",
    "affine": "rotation scaling", "ciclos": "cycles", "tiempo": "timing", "limite": "limit maximum",
    "limites": "limit maximum", "texto": "text", "fuente": "font", "camara": "camera", "azar": "random",
    "aleatorio": "random", "cartucho": "cartridge gamepak", "rom": "gamepak cartridge", "espera": "wait",
    "esperas": "waitstate wait", "waitstate": "wait", "save": "sram backup", "sram": "save backup",
    "flash": "backup", "eeprom": "backup", "vblank": "blank", "hblank": "blank", "dma": "transfer",
    "mapa": "map", "tiles": "tile", "baldosas": "tile", "rendimiento": "performance cpu", "cpu": "cycles",
    "por": "", "frame": "vblank", "cuadro": "frame", "enemigo": "enemy", "nivel": "level", "jefe": "boss",
    "dificultad": "difficulty", "diseno": "design", "prueba": "test", "probar": "test run", "compilar": "build make",
    "matrix": "rotation scaling parameter pa", "matriz": "matrix affine rotation scaling", "salto": "jump", "saltar": "jump", "salta": "jump", "record": "score", "records": "score",
    "tarea": "task", "tareas": "task", "oam": "obj sprite attribute", "bg": "background",
}


# ---------------------------------------------------------------- tokens
def fold(s):
    s = unicodedata.normalize("NFKD", s.lower())
    return "".join(c for c in s if not unicodedata.combining(c))


def stem(w):
    if len(w) <= 3 or w.isdigit() or "_" in w:
        return w
    for suf in ("ings", "ing", "ed", "es", "s", "e"):
        if w.endswith(suf) and len(w) - len(suf) >= 3 and not (suf == "s" and w.endswith("ss")):
            return w[:-len(suf)]
    return w


TOKEN_RE = re.compile(r"[a-z0-9_]+")


def tokens(text):
    """Folded tokens; identifiers keep the whole name plus their _ parts; hex like 4000204h kept."""
    out = []
    for t in TOKEN_RE.findall(fold(text)):
        if len(t) < 2 and not t.isdigit():
            continue
        if "_" in t:
            t = t.strip("_")
            if not t:
                continue
            out.append(t)
            out += [stem(p) for p in t.split("_") if len(p) >= 2 and p not in STOP]
        elif t not in STOP:
            out.append(stem(t))
    return out


# ---------------------------------------------------------------- chunkers
def chunk_window(lines, start, title, size=70):
    """Split a long section at blank lines into pieces of ~size lines."""
    out = []
    i = 0
    n = len(lines)
    part = 0
    while i < n:
        j = min(n, i + size)
        if j < n:
            k = j
            while k > i + size // 2 and lines[k - 1].strip():
                k -= 1
            if k > i + size // 2:
                j = k
        part += 1
        t = title if part == 1 else f"{title} ({part})"
        out.append((start + i, t, "\n".join(lines[i:j])))
        i = j
    return out


def chunk_md(text, rel):
    lines = text.split("\n")
    heads = []
    in_code = False
    for i, ln in enumerate(lines):
        if ln.startswith("```"):
            in_code = not in_code
        if not in_code:
            m = re.match(r"(#{1,3})\s+(.*)", ln)
            if m:
                heads.append((i, len(m.group(1)), m.group(2).strip()))
    if not heads or heads[0][0] > 0:
        heads.insert(0, (0, 1, Path(rel).name))
    out = []
    stack = {}
    for k, (i, lvl, title) in enumerate(heads):
        end = heads[k + 1][0] if k + 1 < len(heads) else len(lines)
        stack[lvl] = title
        for deeper in [l for l in stack if l > lvl]:
            del stack[deeper]
        full = " > ".join(stack[l] for l in sorted(stack))
        body = lines[i:end]
        if not "".join(body).strip():
            continue
        out += chunk_window(body, i + 1, full)
    return out


REG_HEAD = re.compile(r"^[0-9A-F]{7}h(?:[,.][0-9A-F.h]+)?\s+-\s+\S")


def chunk_gbatek(text, rel):
    lines = text.split("\n")
    heads = [i for i in range(len(lines) - 1)
             if lines[i] and not lines[i].startswith(" ") and re.fullmatch(r"-{3,}", lines[i + 1])
             and abs(len(lines[i + 1]) - len(lines[i].rstrip())) <= 1]
    if not heads or heads[0] > 0:
        heads.insert(0, 0)
    out = []
    for k, h in enumerate(heads):
        end = heads[k + 1] if k + 1 < len(heads) else len(lines)
        chap = lines[h].strip() or "GBATEK"
        # sub-split at register headings ("4000000h - DISPCNT - ...")
        cuts = [h] + [i for i in range(h + 2, end) if REG_HEAD.match(lines[i])] + [end]
        for a, b in zip(cuts, cuts[1:]):
            if a >= b:
                continue
            sub = chap if a == h else f"{chap} > {lines[a].strip()[:80]}"
            out += chunk_window(lines[a:b], a + 1, sub, size=60)
    return out


def chunk_butano_jsonl(text, rel):
    """One chunk per symbol (overloads merged); also returns the symbol table."""
    by_sym = {}
    order = []
    for ln in text.split("\n"):
        if not ln.strip():
            continue
        e = json.loads(ln)
        s = e["symbol"]
        if s not in by_sym:
            by_sym[s] = []
            order.append(s)
        by_sym[s].append(e)
    out = []
    for s in order:
        es = by_sym[s]
        e0 = es[0]
        parts = [s]
        for e in es:
            parts.append(e["signature"])
            if e.get("brief"):
                parts.append(e["brief"])
            for p, t in e.get("params", []):
                parts.append(f"{p}: {t}")
            if e.get("returns"):
                parts.append("returns " + e["returns"])
            if e.get("notes"):
                parts.append(e["notes"][:400])
        body = "\n".join(dict.fromkeys(parts))
        title = f"{s} ({e0['kind']}{', %d overloads' % len(es) if len(es) > 1 else ''})"
        loc = f"{BUTANO_INC}/{e0['header']}"
        out.append((e0["line"], title, body, {"path": loc, "symbol": s, "group": e0["group"], "kind": e0["kind"]}))
    return out, by_sym


def chunk_tasks(text, rel):
    out = []
    for i, ln in enumerate(text.split("\n"), 1):
        if not ln.strip():
            continue
        try:
            t = json.loads(ln)
        except ValueError:
            continue
        title = f"{t.get('id')} [{t.get('status')}] {t.get('title', '')}"
        body = "\n".join(str(x) for x in (title, " ".join(t.get("labels", [])), t.get("acceptance", ""),
                                          t.get("notes", "")) if x)
        out.append((i, title, body, {"task": t.get("id"), "status": t.get("status")}))
    return out


# ---------------------------------------------------------------- index
def source_files():
    seen = set()
    out = []
    for g, kind, chunker in SOURCES:
        for p in sorted(ROOT.glob(g)):
            rel = p.relative_to(ROOT).as_posix()
            if rel in seen or not p.is_file():
                continue
            seen.add(rel)
            out.append((rel, kind, chunker))
    return out


def chunk_file(rel, kind, chunker):
    text = (ROOT / rel).read_text(encoding="utf-8", errors="replace")
    syms = None
    if chunker == "gbatek":
        raw = chunk_gbatek(text, rel)
    elif chunker == "butano_jsonl":
        raw, syms = chunk_butano_jsonl(text, rel)
    elif chunker == "tasks":
        raw = chunk_tasks(text, rel)
    else:
        raw = chunk_md(text, rel)
    chunks = []
    for item in raw:
        line, title, body = item[:3]
        meta = item[3] if len(item) > 3 else {}
        # lines that merely show kb.py example queries would match those queries: not indexed
        tt = tokens("\n".join(l for l in body.split("\n") if "kb.py " not in l))
        ht = tokens(title) if kind != "butano" or "symbol" not in meta else tokens(meta["symbol"].replace("::", " ")
                                                                                  + " " + meta["symbol"])
        tf = Counter(tt)
        for t in ht:
            tf[t] += 3
        chunks.append({"path": rel, "kind": kind, "line": line, "title": title, "text": body, "meta": meta,
                       "len": max(1, len(tt) + 3 * len(ht)), "tf": dict(tf)})
    return chunks, syms


def build(force=False, verbose=False):
    idx = None
    if CACHE_FILE.exists() and not force:
        try:
            with open(CACHE_FILE, "rb") as f:
                idx = pickle.load(f)
            if idx.get("version") != INDEX_VERSION:
                idx = None
        except Exception:
            idx = None
    files = source_files()
    sigs = {}
    for rel, _, _ in files:
        st = (ROOT / rel).stat()
        sigs[rel] = (st.st_mtime_ns, st.st_size)
    if idx and idx["sigs"] == sigs:
        return idx
    per_file = idx["per_file"] if idx else {}
    syms = idx["syms"] if idx else {}
    changed = 0
    for rel, kind, chunker in files:
        if idx and idx["sigs"].get(rel) == sigs[rel] and rel in per_file:
            continue
        chunks, s = chunk_file(rel, kind, chunker)
        per_file[rel] = chunks
        if s is not None:
            syms = s
        changed += 1
    for rel in list(per_file):
        if rel not in sigs:
            del per_file[rel]
    docs = []
    for rel, _, _ in files:
        docs += per_file.get(rel, [])
    post = defaultdict(list)
    for did, d in enumerate(docs):
        for t, c in d["tf"].items():
            post[t].append((did, c))
    avg = defaultdict(float)
    cnt = Counter()
    for d in docs:
        avg[d["kind"]] += d["len"]
        cnt[d["kind"]] += 1
    avg = {k: avg[k] / cnt[k] for k in cnt}
    idx = {"version": INDEX_VERSION, "sigs": sigs, "per_file": per_file, "docs": docs, "post": dict(post),
           "avg": avg, "syms": syms, "built": time.strftime("%Y-%m-%d %H:%M:%S")}
    CACHE.mkdir(parents=True, exist_ok=True)
    gi = CACHE / ".gitignore"
    if not gi.exists():
        gi.write_text("*\n", encoding="utf-8")
    tmp = CACHE_FILE.with_suffix(".tmp")
    with open(tmp, "wb") as f:
        pickle.dump(idx, f, protocol=pickle.HIGHEST_PROTOCOL)
    tmp.replace(CACHE_FILE)
    if verbose:
        print(f"index: {changed} file(s) re-chunked, {len(docs)} chunks, {len(post)} terms", file=sys.stderr)
    return idx


# ---------------------------------------------------------------- search
def query_terms(q):
    base = tokens(q)
    terms = Counter()
    for t in base:
        terms[t] += 1.0
    for w in TOKEN_RE.findall(fold(q)):
        for s in SYN.get(w, "").split():
            for t in tokens(s):
                if t not in terms:
                    terms[t] += 0.4
    return terms, base


def search(idx, q, kinds=None, limit=5, exclude_paths=()):
    terms, base = query_terms(q)
    docs, post, avg = idx["docs"], idx["post"], idx["avg"]
    N = len(docs)
    k1, b = 1.2, 0.75
    scores = defaultdict(float)
    matched = defaultdict(set)
    for t, w in terms.items():
        pl = post.get(t)
        if not pl:
            continue
        df = len(pl)
        idf = math.log(1 + (N - df + 0.5) / (df + 0.5))
        for did, tf in pl:
            d = docs[did]
            if kinds and d["kind"] not in kinds:
                continue
            dl = d["len"] / avg.get(d["kind"], 1.0)
            scores[did] += w * idf * tf * (k1 + 1) / (tf + k1 * (1 - b + b * dl))
            if w >= 1.0:
                matched[did].add(t)
    nbase = len(set(base)) or 1
    idents = [t for t in TOKEN_RE.findall(fold(q.replace("::", "__"))) if "_" in t]
    idents = [i.replace("__", "::") for i in idents]
    want_action = "action" in fold(q)
    res = []
    for did, s in scores.items():
        d = docs[did]
        if d["path"] in exclude_paths:
            continue
        cover = len(matched[did]) / nbase
        s *= 0.5 + cover  # prefer chunks matching more of the query
        if not (kinds and len(kinds) == 1):
            s *= KIND_WEIGHT.get(d["kind"], 1.0)
        sym = d["meta"].get("symbol")
        if sym:
            fs = fold(sym)
            for ident in idents:
                if fs == ident or fs.endswith("::" + ident) or fs.endswith(ident):
                    s *= 2.0
                elif ident in fs:
                    s *= 1.3
            if "_action" in fs and not want_action:
                s *= 0.6
            if "_hbe_" in fs and "hbe" not in fold(q) and "hblank" not in fold(q):
                s *= 0.8
        res.append((s, did))
    res.sort(reverse=True)
    return res[:limit], terms


def snippet(text, terms, nlines=3, width=150):
    lines = [l for l in text.split("\n") if l.strip()]
    scored = []
    for i, l in enumerate(lines):
        tl = set(tokens(l))
        sc = sum(terms.get(t, 0) for t in tl)
        scored.append((sc, -i, l))
    best = sorted(scored, reverse=True)[:nlines]
    best = sorted(best, key=lambda x: -x[1])
    out = []
    for sc, _, l in best:
        if sc <= 0 and out:
            continue
        l = l.rstrip()
        out.append(l if len(l) <= width else l[:width] + "...")
    return out


def location(d):
    p = d["meta"].get("path", d["path"])
    return f"{p}:{d['line']}"


def fmt_hit(d, terms, score=None, full=False, nlines=3):
    sc = f"  ({score:.1f})" if score is not None else ""
    head = f"[{d['kind']}] {location(d)}  {d['title']}{sc}"
    if d["kind"] == "butano" and d["meta"].get("group"):
        head += f"  -> docs/api/butano/{d['meta']['group']}.md"
    if full:
        body = d["text"].split("\n")
        if len(body) > 120:
            body = body[:120] + [f"... ({len(body) - 120} more lines; Read {location(d)})"]
        return head + "\n" + "\n".join("    " + l for l in body)
    return head + "\n" + "\n".join("    " + l for l in snippet(d["text"], terms, nlines))


def parse_kinds(s):
    if not s:
        return None
    ks = {k.strip() for k in s.split(",") if k.strip()}
    bad = ks - set(KINDS)
    if bad:
        sys.exit(f"unknown kind(s) {sorted(bad)}; kinds: {', '.join(KINDS)}")
    return ks


# ---------------------------------------------------------------- symbol
def norm_symbol(s):
    return re.sub(r"\s+", "", s).strip(":")


def find_symbol(syms, q):
    q = norm_symbol(q)
    for cand in (q, "bn::" + q):
        if cand in syms:
            return cand, []
    lq = fold(q)
    suffix = [s for s in syms if fold(s).endswith("::" + lq)]
    if len(suffix) == 1:
        return suffix[0], []
    if suffix:
        return None, sorted(suffix, key=len)[:15]
    sub = [s for s in syms if lq in fold(s)]
    sub.sort(key=lambda s: (len(s), s))
    return None, sub[:15]


def print_symbol(idx, q, full=False):
    syms = idx["syms"]
    sym, sugg = find_symbol(syms, q)
    if not sym:
        if sugg:
            print(f"'{q}' no es exacto; candidatos:")
            for s in sugg:
                e = syms[s][0]
                print(f"  {s}  ({e['kind']}) {e['brief'][:90]}")
        else:
            print(f"'{q}' no está en la API de Butano. Prueba: kb.py query \"{q}\" --kind butano")
        return 1
    es = syms[sym]
    e0 = es[0]
    print(f"{sym}  ({e0['kind']}, módulo {e0['group']} -> docs/api/butano/{e0['group']}.md)")
    for e in es:
        print(f"\n  {e['signature']}")
        print(f"  {BUTANO_INC}/{e['header']}:{e['line']}")
        if e.get("brief"):
            print(f"  {e['brief']}")
        for p, t in e.get("tparams", []):
            print(f"    tparam {p}: {t}")
        for p, t in e.get("params", []):
            print(f"    {p}: {t}")
        if e.get("returns"):
            print(f"    devuelve: {e['returns']}")
        if e.get("notes") and (full or len(es) <= 3):
            for ln in e["notes"].split("\n")[: (40 if full else 6)]:
                print(f"    | {ln}")
    if e0["kind"] in ("class", "struct", "namespace", "enum"):
        pre = sym + "::"
        kids = [s for s in syms if s.startswith(pre) and "::" not in s[len(pre):]]
        if kids:
            print(f"\n  Miembros ({len(kids)}){'' if full else '; --full para firmas'}:")
            for s in kids:
                k = syms[s]
                name = s[len(pre):]
                if full:
                    for e in k:
                        print(f"    {e['signature']}  (L{e['line']})")
                        print(f"        {e['brief'][:150]}")
                else:
                    ov = f" x{len(k)}" if len(k) > 1 else ""
                    print(f"    {name}{ov}: {k[0]['brief'][:110]}")
    return 0


# ---------------------------------------------------------------- brief
def que_usar_rows(task_terms, n=5):
    p = ROOT / "docs" / "guias" / "que-usar-butano.md"
    if not p.exists():
        return []
    rows = []
    for i, ln in enumerate(p.read_text(encoding="utf-8").split("\n"), 1):
        if ln.startswith("|") and not ln.startswith("|---") and "Necesidad" not in ln:
            tl = set(tokens(ln))
            sc = sum(task_terms.get(t, 0) for t in tl)
            if sc > 0:
                rows.append((sc, i, ln.strip()))
    rows.sort(key=lambda r: (-r[0], r[1]))
    return rows[:n]


def cmd_brief(idx, role, task, limit):
    r = (role or "game-dev").lower()
    r = r[4:] if r.startswith("gba-") else r
    if r not in ROLES:
        print(f"(rol '{role}' desconocido; uso game-dev. Roles: {', '.join(ROLES)})")
        r = "game-dev"
    print(f"# kb brief  rol={r}  tarea: {task}\n")
    print("## Leer siempre (sólo la sección indicada)")
    for p, why in ROLES[r]:
        print(f"- {p}: {why}")
    terms, _ = query_terms(task)
    rows = que_usar_rows(terms)
    if rows:
        print("\n## que-usar-butano.md (filas que encajan)")
        for _, i, ln in rows:
            print(f"- docs/guias/que-usar-butano.md:{i}  {ln[:200]}")
    sections = [("Guías y skills", {"guide"}, limit), ("Lecciones", {"lessons"}, 3), ("API de Butano", {"butano"}, 6),
                ("Hardware (GBATEK / resumen)", {"hw"}, 3), ("Tareas", {"tasks"}, 3)]
    always = {p for p, _ in ROLES[r]}
    for title, kinds, n in sections:
        hits, t = search(idx, task, kinds, n, exclude_paths=always if "guide" in kinds else ())
        top = hits[0][0] if hits else 0
        hits = [(s, d) for s, d in hits if s > 1.0 and s >= 0.4 * top]
        if not hits:
            continue
        print(f"\n## {title}")
        for s, did in hits:
            d = idx["docs"][did]
            line = snippet(d["text"], t, 1, 120)
            extra = f" — {line[0].strip()}" if line and not d["meta"].get("symbol") else ""
            if d["meta"].get("symbol"):
                b = idx["syms"].get(d["meta"]["symbol"], [{}])[0].get("brief", "")
                extra = f" — {b[:110]}"
            print(f"- {location(d)}  {d['title']}{extra}")
    print("\nDespués: `kb.py symbol <bn::x>` para la firma exacta; `kb.py query \"...\" --kind K` para afinar.")


# ---------------------------------------------------------------- main
def main(argv=None):
    ap = argparse.ArgumentParser(prog="kb.py", description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd")
    q = sub.add_parser("query", help="ranked search")
    q.add_argument("text", nargs="+")
    q.add_argument("--kind", help="hw,butano,guide,lessons,tasks (comma list)")
    q.add_argument("--limit", "-n", "--n", type=int, default=5)
    q.add_argument("--full", action="store_true", help="print whole chunks")
    s = sub.add_parser("symbol", help="exact Butano API entry")
    s.add_argument("name", nargs="+")
    s.add_argument("--full", action="store_true")
    b = sub.add_parser("brief", help="what to read for a task")
    b.add_argument("--role", default="game-dev")
    b.add_argument("--task", required=True)
    b.add_argument("--limit", type=int, default=4)
    sub.add_parser("stats")
    sub.add_parser("rebuild")
    a = ap.parse_args(argv)
    if not a.cmd:
        ap.print_help()
        return 0
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    idx = build(force=a.cmd == "rebuild", verbose=a.cmd in ("rebuild", "stats"))
    if a.cmd == "query":
        text = " ".join(a.text)
        hits, terms = search(idx, text, parse_kinds(a.kind), a.limit)
        if not hits:
            print("sin resultados")
            return 1
        for sc, did in hits:
            print(fmt_hit(idx["docs"][did], terms, sc, a.full))
        return 0
    if a.cmd == "symbol":
        return print_symbol(idx, " ".join(a.name), a.full)
    if a.cmd == "brief":
        cmd_brief(idx, a.role, a.task, a.limit)
        return 0
    if a.cmd in ("stats", "rebuild"):
        c = Counter(d["kind"] for d in idx["docs"])
        print(f"built {idx['built']}  files {len(idx['sigs'])}  chunks {len(idx['docs'])}  terms {len(idx['post'])}"
              f"  symbols {len(idx['syms'])}")
        print("  " + ", ".join(f"{k} {c[k]}" for k in KINDS))
        return 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
