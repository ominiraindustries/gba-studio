"""Seeds DELVE art and exports it.

  python -I seed_all.py              generate PNG seeds, create art/<name>.aseprite ONLY if missing, export all
  python -I seed_all.py export-only  just export every art/*.aseprite (after the user edits them)

Export targets: sprites and ui_frame -> graphics/<name>.bmp (Butano); textures -> art/export/<name>.bmp
(input of the engine's tools/build_view.py; Butano does not compile them directly).
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
GFX = os.path.join(os.path.dirname(ART), "graphics")
ASE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"

MON = "idle:1-2:400;attack:3-4:150;hit:5-5:200"
ASSETS = [  # name, frame height, tags, background layer, palette size
    ("mon_rat", 64, MON, 0, 16), ("mon_skeleton", 64, MON, 0, 16), ("mon_slime", 64, MON, 0, 16),
    ("prop_barrel", 32, "", 0, 16), ("prop_bones", 32, "", 0, 16),
    ("prop_spikes", 32, "down:1-1:500;up:2-2:500", 0, 16),
    ("prop_crate", 32, "", 0, 16), ("prop_statue", 32, "", 0, 16), ("prop_table", 32, "", 0, 16),
    ("prop_chest", 32, "closed:1-1:500;open:2-2:500", 0, 16),
    ("prop_plate", 32, "up:1-1:500;pressed:2-2:500", 0, 16), ("prop_pit", 32, "", 0, 16),
    ("ui_frame", 160, "", 0, 256),
    ("tex_wall_torch", 64, "flame:1-2:150", 1, 64), ("tex_bars", 64, "", 0, 64), ("tex_arch", 64, "", 0, 64),
] + [(t, 64, "", 1, 64) for t in (
    "tex_wall_stone", "tex_door", "tex_door_open", "tex_floor", "tex_ceiling", "tex_stairs_down",
    "tex_wall_lever_up", "tex_wall_lever_down", "tex_wall_chains", "tex_wall_crack", "tex_wall_dart",
    "tex_wall_banner", "tex_wall_skulls")]


def run(args):
    subprocess.run([ASE, "-b"] + args, check=True, stdout=subprocess.DEVNULL)


def main():
    if "export-only" not in sys.argv:
        for script in ("delve_art.py", "delve_sprites.py", "delve_ui.py"):
            subprocess.run([sys.executable, "-I", os.path.join(HERE, script)], check=True)
        for name, fh, tags, bg, ncol in ASSETS:
            dst = os.path.join(ART, name + ".aseprite")
            if os.path.exists(dst):
                continue
            run(["--script-param", "in=" + os.path.join(HERE, "out", name + ".png"), "--script-param", "out=" + dst,
                 "--script-param", "fh=%d" % fh, "--script-param", "tags=" + tags,
                 "--script-param", "bg=%d" % bg, "--script-param", "ncol=%d" % ncol,
                 "--script", os.path.join(HERE, "build_aseprite.lua")])
            print("created", os.path.relpath(dst, ART))
    os.makedirs(os.path.join(ART, "export"), exist_ok=True)
    for f in sorted(os.listdir(ART)):
        if not f.endswith(".aseprite"):
            continue
        name = f[:-9]
        dst = os.path.join(ART, "export" if name.startswith("tex_") else GFX, name + ".bmp")
        run([os.path.join(ART, f), "--sheet-type", "vertical", "--sheet", dst])
    print("exported")


if __name__ == "__main__":
    main()
