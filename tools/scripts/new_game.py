#!/usr/bin/env python3
"""Create a Butano game project from templates/butano_game.

    python -I tools/scripts/new_game.py 001_my_game [--title "My Game"] [--romtitle MYGAME] [--romcode AMGE]

NAME becomes projects/NAME/ and the ROM projects/NAME/NAME.gba. Placeholders replaced in text files:
@NAME@, @TITLE@ (on-screen title), @ROMTITLE@ (header, <=12 chars A-Z 0-9 space), @ROMCODE@ (4 chars).
"""
import argparse, pathlib, re, shutil, sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
TEMPLATE = ROOT / "templates" / "butano_game"
TEXT_SUFFIXES = {".cpp", ".h", ".hpp", ".md", ".bat", ".json", ""}  # "" = Makefile
SKIP_DIRS = {"build"}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name", help="folder name, e.g. 001_my_game (lowercase letters, digits, _)")
    ap.add_argument("--title", help="text shown on screen (default: derived from NAME)")
    ap.add_argument("--romtitle", help="ROM header title, max 12 chars (default: derived from NAME)")
    ap.add_argument("--romcode", default="2HBE", help="ROM header game code, 4 chars (default 2HBE)")
    ap.add_argument("--dest", default=str(ROOT / "projects"), help="parent folder (default projects/)")
    a = ap.parse_args()

    if not re.fullmatch(r"[a-z0-9_]+", a.name):
        sys.exit("NAME: use only lowercase letters, digits and _ (it becomes the ROM file name)")
    words = [w for w in a.name.split("_") if not w.isdigit()] or [a.name]
    title = a.title or " ".join(w.capitalize() for w in words)
    romtitle = (a.romtitle or "".join(words)).upper()[:12]
    if not re.fullmatch(r"[A-Z0-9 ]{1,12}", romtitle):
        sys.exit("--romtitle: 1-12 characters A-Z, 0-9 or space")
    if not re.fullmatch(r"[A-Z0-9]{4}", a.romcode):
        sys.exit("--romcode: exactly 4 characters A-Z or 0-9")
    if not title.isascii():
        sys.exit("--title: ASCII only (the template font has no accented glyphs)")

    dest = pathlib.Path(a.dest).resolve() / a.name
    if dest.exists():
        sys.exit(f"{dest} already exists")
    shutil.copytree(TEMPLATE, dest, ignore=shutil.ignore_patterns(*SKIP_DIRS, "*.gba", "*.elf"))
    subs = {"@NAME@": a.name, "@TITLE@": title, "@ROMTITLE@": romtitle, "@ROMCODE@": a.romcode}
    replaced = 0
    for f in dest.rglob("*"):
        if f.is_file() and f.suffix.lower() in TEXT_SUFFIXES and f.name != ".gitkeep":
            data = f.read_bytes().decode("utf-8")
            new = data
            for k, v in subs.items():
                new = new.replace(k, v)
            if new != data:
                f.write_bytes(new.encode("utf-8"))
                replaced += 1
    left = [str(f.relative_to(dest)) for f in dest.rglob("*") if f.is_file() and f.suffix.lower() in TEXT_SUFFIXES
            and "@" in f.read_text("utf-8", errors="replace") and re.search(r"@[A-Z]+@", f.read_text("utf-8"))]
    if replaced == 0 or left:
        sys.exit(f"placeholder replacement failed (replaced in {replaced} files; still present in {left})")
    print(f"created {dest} (title {title!r}, ROM header {romtitle!r}/{a.romcode}); placeholders replaced in "
          f"{replaced} files")
    print(f"build: source tools/env.sh && make -C {dest.relative_to(ROOT).as_posix()} -j8   (or compilar.bat)")


if __name__ == "__main__":
    main()
