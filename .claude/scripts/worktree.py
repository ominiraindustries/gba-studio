"""Agent worktrees for the GBA workspace: sparse by default, removed safely when the branch is merged.

Lessons from the SNES workspace (2026-11-21/23): ~107 full worktrees (~650 MB each) filled disk C because nobody
removed them after merging; and a junction to a TRACKED dir (tools/vendor) let git delete the real files of the main
checkout through the link. So:
  * add = sparse cone checkout (BASE_PATHS + the project + --paths) and junctions ONLY to dirs that git does not
    track (checked with `git ls-files` at creation time; config in .claude/worktree.json);
  * remove = refuse if dirty/untracked/unmerged, archive built *.gba, unlink every junction first (link only, never
    its target), verify none remains, then `git worktree remove`;
  * never `git worktree add` by hand, never `rm -rf` a worktree.

Usage (always python -I, from the main checkout or any worktree):
  worktree.py add NAME --base BRANCH [--project projects/x] [--paths a,b] [--full] [--branch B]
  worktree.py remove NAME [--into BRANCH] [--force-untracked] [--delete-branch]
  worktree.py list
"""
import argparse, json, os, pathlib, re, shutil, stat, subprocess, sys


def git_raw(args, cwd):
    return subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True, encoding="utf-8", errors="replace")


def main_root():
    p = git_raw(["rev-parse", "--path-format=absolute", "--git-common-dir"], pathlib.Path(__file__).resolve().parent)
    if p.returncode:
        sys.exit("worktree.py: not inside a git repository")
    return pathlib.Path(p.stdout.strip()).resolve().parent


MAIN = main_root()
WT_ROOT = MAIN / ".claude" / "worktrees"
ROM_ARCHIVE = MAIN / ".claude" / "rom_archive"
CFG = MAIN / ".claude" / "worktree.json"
CFG_BASE = "gbaWorktreeBase"
NAME_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]{0,63}$")
DEFAULTS = {
    # tracked dirs every agent needs (cone mode also brings the files at the top of each parent: CLAUDE.md ...);
    # a dir that is not in git is skipped here and, if listed in junctions, linked instead
    "base_paths": [".claude/agents", ".claude/skills", ".claude/scripts", "docs", "tools", "templates", "engine",
                   "toolchain"],
    # big untracked tool dirs shared through junctions; skipped (with a message) if git tracks anything inside
    "junctions": ["toolchain", "toolchain/devkitPro", "engine", "engine/butano", "tools/mesen", "tools/mgba"],
}


def config():
    c = dict(DEFAULTS)
    if CFG.exists():
        c.update(json.loads(CFG.read_text(encoding="utf-8")))
    return c


def git(args, cwd=MAIN, check=True):
    p = git_raw(args, cwd)
    if check and p.returncode:
        sys.exit(f"git {' '.join(args)} failed: {(p.stderr or p.stdout).strip()}")
    return p


def is_reparse(path):
    st = os.lstat(path)
    return bool(getattr(st, "st_file_attributes", 0) & 0x400) or stat.S_ISLNK(st.st_mode)


def walk(root):
    """Yield (path, is_reparse); reparse points are reported, never entered."""
    stack = [str(root)]
    while stack:
        d = stack.pop()
        try:
            entries = list(os.scandir(d))
        except OSError:
            continue
        for e in entries:
            rp = is_reparse(e.path)
            yield e.path, rp
            if not rp and e.is_dir(follow_symlinks=False):
                stack.append(e.path)


def reparse_points(root):
    return [p for p, rp in walk(root) if rp]


def size_mb(root):
    return sum(os.lstat(p).st_size for p, rp in walk(root) if not rp and os.path.isfile(p)) / (1 << 20)


def target_of(link):
    t = os.readlink(link)
    return os.path.normcase(os.path.abspath(t[4:] if t.startswith("\\\\?\\") else t))


def make_junction(link, target):
    """Junction link -> target without a shell string (_winapi, else cmd with an argv list), then verify it."""
    try:
        import _winapi
        _winapi.CreateJunction(str(target), str(link))
    except (ImportError, AttributeError, OSError):
        p = subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(target)], capture_output=True, text=True)
        if p.returncode:
            raise OSError(f"mklink /J failed: {(p.stderr or p.stdout).strip()}")
    if not is_reparse(link) or target_of(link) != os.path.normcase(str(target)):
        raise OSError(f"junction {link} does not point to {target}")


def unlink(path):
    if not is_reparse(path):
        raise OSError(f"refusing to delete {path}: not a link")
    os.rmdir(path) if os.path.isdir(path) else os.unlink(path)


def worktrees():
    out, cur = {}, None
    for l in git(["worktree", "list", "--porcelain"]).stdout.splitlines():
        if l.startswith("worktree "):
            cur = out.setdefault(pathlib.Path(l[9:]).resolve(), {"branch": None})
        elif cur is not None and l.startswith("branch "):
            cur["branch"] = l[7:].removeprefix("refs/heads/")
    return out


def merged(branch, into):
    return bool(branch) and git(["merge-base", "--is-ancestor", branch, into], check=False).returncode == 0


def cmd_add(a):
    if not NAME_RE.match(a.name):
        sys.exit("bad name: letters, digits, '.', '_' and '-' only")
    branch, path, cfg = a.branch or a.name, WT_ROOT / a.name, config()
    if os.path.lexists(path):
        sys.exit(f"{path} already exists")
    if git(["rev-parse", "--verify", "--quiet", a.base], check=False).returncode:
        sys.exit(f"unknown base {a.base}")
    paths = []
    if not a.full:
        paths = list(cfg["base_paths"]) + ([a.project.strip("/")] if a.project else [])
        paths += [p.strip().strip("/") for p in (a.paths or "").split(",") if p.strip()]
        paths = [p for p in dict.fromkeys(paths)
                 if git(["cat-file", "-t", f"{a.base}:{p}"], check=False).stdout.strip() == "tree"
                 or print(f"skip path {p}: not a directory in {a.base}")]
    WT_ROOT.mkdir(parents=True, exist_ok=True)
    git(["worktree", "add", "--no-checkout", "-b", branch, str(path), a.base])
    made = []
    try:
        if not a.full:
            git(["-C", str(path), "sparse-checkout", "set", "--cone", *paths])
        git(["-C", str(path), "checkout", "--force", branch])
        git(["config", f"branch.{branch}.{CFG_BASE}", a.base])
        for rel in cfg["junctions"]:
            src, dst = MAIN / rel, path / rel
            if not src.is_dir() or is_reparse(src):
                continue
            if git(["ls-files", "--", rel]).stdout.strip():
                print(f"skip junction {rel}: git tracks files inside (use sparse paths instead)")
                continue
            if os.path.lexists(dst):
                print(f"skip junction {rel}: already present")
                continue
            dst.parent.mkdir(parents=True, exist_ok=True)
            make_junction(dst, src)
            made.append(rel)
    except (OSError, SystemExit) as e:
        print(f"add failed ({e}); undoing", file=sys.stderr)
        for rp in reparse_points(path) if path.exists() else []:
            unlink(rp)
        git(["worktree", "remove", "--force", str(path)], check=False)
        git(["branch", "-D", branch], check=False)
        sys.exit(1)
    print(f"junctions: {', '.join(made) or 'none'}")
    print(f"created {path} on {branch} from {a.base} ({'full' if a.full else 'sparse'}), {size_mb(path):.1f} MB")


def cmd_remove(a):
    path = (WT_ROOT / a.name).resolve()
    info = worktrees().get(path)
    if info is None:
        sys.exit(f"refused: {path} is not a registered worktree")
    cwd = pathlib.Path.cwd().resolve()
    if cwd == path or path in cwd.parents:
        sys.exit("refused: the current directory is inside the worktree")
    st = git(["-C", str(path), "status", "--porcelain"], check=False).stdout.splitlines()
    tracked = [l for l in st if l.strip() and not l.startswith("??")]
    untracked = [l[3:] for l in st if l.startswith("??")]
    if tracked:
        sys.exit(f"refused: {len(tracked)} uncommitted change(s), e.g. {tracked[0]}")
    if untracked and not a.force_untracked:
        sys.exit(f"refused: {len(untracked)} untracked file(s), e.g. {untracked[0]} (--force-untracked)")
    branch = info["branch"]
    into = a.into or git(["config", "--get", f"branch.{branch}.{CFG_BASE}"], check=False).stdout.strip() or "main"
    if not merged(branch, into):
        sys.exit(f"refused: {branch} is not merged into {into}")
    before = size_mb(path)
    roms = [p for p, rp in walk(path) if not rp and p.lower().endswith(".gba")]
    for r in roms:
        dst = ROM_ARCHIVE / a.name / os.path.relpath(r, path)
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(r, dst)
    links = reparse_points(path)
    for lp in links:
        unlink(lp)
    if reparse_points(path):
        sys.exit("refused: links still present; nothing deleted")
    p = git(["worktree", "remove", *(["--force"] if untracked else []), str(path)], check=False)
    if p.returncode:
        sys.exit(f"git worktree remove failed (links already removed): {p.stderr.strip()}")
    if a.delete_branch:
        git(["branch", "-d", branch], check=False)
    print(f"removed {a.name} ({branch} merged into {into}): {len(links)} link(s), {len(roms)} ROM(s) archived, "
          f"{before:.1f} MB freed")


def cmd_list(a):
    for path, info in worktrees().items():
        if path == MAIN.resolve():
            continue
        b = info["branch"]
        base = git(["config", "--get", f"branch.{b}.{CFG_BASE}"], check=False).stdout.strip() or "main"
        dirty = git(["-C", str(path), "status", "--porcelain"], check=False).stdout.strip()
        print(f"{path.name:<24} {b or '(detached)':<24} base={base:<12} "
              f"{'merged' if merged(b, base) else 'unmerged'} {'dirty' if dirty else 'clean'} {size_mb(path):.0f} MB")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("add"); p.add_argument("name"); p.add_argument("--base", required=True)
    p.add_argument("--project"); p.add_argument("--paths"); p.add_argument("--full", action="store_true")
    p.add_argument("--branch"); p.set_defaults(f=cmd_add)
    p = sub.add_parser("remove"); p.add_argument("name"); p.add_argument("--into")
    p.add_argument("--force-untracked", action="store_true"); p.add_argument("--delete-branch", action="store_true")
    p.set_defaults(f=cmd_remove)
    p = sub.add_parser("list"); p.set_defaults(f=cmd_list)
    a = ap.parse_args()
    a.f(a)


if __name__ == "__main__":
    main()
