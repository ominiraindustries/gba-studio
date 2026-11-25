"""Wendel (and the Cinda / Morrow recolours) brighter than the hordes: the wizard must be the most readable thing on
screen. Only palette entries change (same tiles, frames, tags, durations; index meaning kept so the alt palettes
still line up with wizard tiles). Target: mean interior luminance (Rec.601, edge pixels excluded) >= 1.3x the
brightest horde enemy (wolf ~81.6 after G-53). New colours are appended to palette_c.json "master" (key
"wizard_bright") so snap_master.py keeps them. Originals: art/old_style/pre_wizbright/.
    python -I art/tools/wizard_bright.py
    python -I art/tools/build_art.py export
"""
import json
import os
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
ASEPRITE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"

# 1 inner outline, 2/3 deep shade, 4 robe shade, 5/7 staff-hat greys, 6 robe, 8 crystal, 9 light rim
RECIPES = {
    "wizard": {1: "#1c1838", 2: "#4a2060", 3: "#2e2c58", 4: "#6a48b8", 5: "#3c4466", 6: "#9878e8", 7: "#5a6c8c",
               8: "#ff5a48", 9: "#c8b8f8"},
    "wizard_cinda_palette": {1: "#2a1418", 2: "#6a2038", 3: "#3a1430", 4: "#d03a2c", 5: "#4a4c58", 6: "#f06048",
                             7: "#4a4c58", 8: "#ffc838", 9: "#eecf74"},
    "wizard_morrow_palette": {1: "#0e1c26", 2: "#1e4258", 3: "#1a2c44", 4: "#2e6e80", 5: "#3a4a58", 6: "#4a9cb0",
                              7: "#4e6a74", 8: "#40e8f8", 9: "#c0d8e4"},
}

LUA = r"""
local spr = app.open(app.params.file)
local pal = spr.palettes[1]
for i, hex in pairs(PAL) do
  pal:setColor(i, Color{ r = tonumber(hex:sub(2,3),16), g = tonumber(hex:sub(4,5),16), b = tonumber(hex:sub(6,7),16), a = 255 })
end
spr:saveAs(app.params.file)
print("ok " .. #pal)
"""


def run(name, pal):
    with tempfile.NamedTemporaryFile("w", suffix=".lua", delete=False) as f:
        f.write("local PAL = {" + ", ".join(f'[{i}] = "{h}"' for i, h in pal.items()) + "}\n" + LUA)
        script = f.name
    path = os.path.join(ART, name + ".aseprite")
    out = subprocess.run([ASEPRITE, "-b", "--script-param", f"file={path}", "--script", script],
                         capture_output=True, text=True)
    os.unlink(script)
    print(f"{name:22s} {out.stdout.strip()} {out.stderr.strip()}")


def add_master():
    p = os.path.join(ART, "palette_c.json")
    d = json.load(open(p))
    cols = {h for r in RECIPES.values() for h in r.values()}
    new = sorted(cols - set(d["master"]))
    d["wizard_bright"] = sorted(cols)
    d["master"] += new
    json.dump(d, open(p, "w"), indent=1)
    print(f"palette_c.json +{len(new)} wizard colours")


if __name__ == "__main__":
    for n, pal in RECIPES.items():
        run(n, pal)
    add_master()
