"""Seeds and exports DELVE's wall decoration sprites (deco_*).

  python -I deco_seed.py              regenerate seeds (deco_art.py), create art/deco_*.aseprite ONLY if missing,
                                      export every art/deco_*.aseprite to graphics/ (.bmp + .json)
  python -I deco_seed.py export-only  only export (after the user edits the .aseprite files)

seed_all.py export-only also exports them (it exports every art/*.aseprite) but does not write the .json.
"""
import json
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
GFX = os.path.join(os.path.dirname(ART), "graphics")
OUT = os.path.join(HERE, "out")
ASE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"

FLAME = ("flame:1-4", "110,70,140,90")
DECOS = {}
for view in ("front", "left", "right"):
    DECOS["deco_torch_" + view] = (64, FLAME)
    DECOS["deco_banner_" + view] = (64, ("wave:1-2", "500,500"))
    DECOS["deco_chains_" + view] = (64, ("", ""))
    DECOS["deco_lever_" + view] = (64, ("up:1-1;down:2-2", "500,500"))
DECOS["deco_torch_glow"] = (64, ("", ""))


def run(args):
    subprocess.run([ASE, "-b"] + args, check=True, stdout=subprocess.DEVNULL)


def main():
    if "export-only" not in sys.argv:
        subprocess.run([sys.executable, "-I", os.path.join(HERE, "deco_art.py")], check=True)
        for name, (fh, (tags, durs)) in DECOS.items():
            dst = os.path.join(ART, name + ".aseprite")
            if os.path.exists(dst):
                continue
            shadow = os.path.join(OUT, name + "_shadow.png")
            run(["--script-param", "shadow=" + (shadow if os.path.exists(shadow) else ""),
                 "--script-param", "object=" + os.path.join(OUT, name + "_object.png"),
                 "--script-param", "out=" + dst, "--script-param", "fh=%d" % fh,
                 "--script-param", "tags=" + tags, "--script-param", "durs=" + durs,
                 "--script", os.path.join(HERE, "deco_aseprite.lua")])
            print("created", name + ".aseprite")
    for f in sorted(os.listdir(ART)):
        if not (f.startswith("deco_") and f.endswith(".aseprite")):
            continue
        name = f[:-9]
        run([os.path.join(ART, f), "--sheet-type", "vertical", "--sheet", os.path.join(GFX, name + ".bmp")])
        fh = DECOS.get(name, (64,))[0]
        with open(os.path.join(GFX, name + ".json"), "w") as fp:
            json.dump({"type": "sprite", "height": fh}, fp)
    print("exported deco_*")


if __name__ == "__main__":
    main()
