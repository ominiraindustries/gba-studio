"""G-53: lift goblin / orc / wolf off the dark floor (hue option A: goblin green, orc olive, wolf light brown-grey).

Only palette colours change, plus one pixel rule: the rim index (1 px border outside the silhouette) becomes a light
tinted colour, and pixels of that index that are NOT on the silhouette edge are moved to the body index given in
INNER so the inside does not get light speckles. Frames, sizes, tags and durations are untouched (Aseprite does the
edit through a Lua script). The new colours are appended to palette_c.json "master" (key "hordes") so snap_master.py
keeps them. Originals: art/old_style/pre_g53/.
    python -I art/tools/hordes_g53.py          # goblin orc wolf
    python -I art/tools/build_art.py export
"""
import json
import os
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
ASEPRITE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"

# index -> colour. Indices keep their meaning (rotten_palette reuses goblin/orc tiles by index).
RECIPES = {
    "goblin": {"rim": 10, "inner": 9, "pal": {
        1: "#0d1914", 2: "#142a20", 3: "#1a3626", 4: "#24482c", 5: "#2e5a34", 7: "#3a6a3a", 8: "#477a40",
        9: "#568a48", 10: "#98c88a", 12: "#74a65c", 14: "#ffc838"}},
    "orc": {"rim": 10, "inner": 9, "pal": {
        1: "#13150c", 2: "#1f2412", 3: "#2a3116", 4: "#363f1a", 5: "#454f20", 6: "#6a2a2c", 7: "#525e26",
        8: "#5f6c2a", 9: "#737f34", 10: "#cac68c", 12: "#98a44c", 14: "#ffc838", 15: "#abc4d2"}},
    "wolf": {"rim": 7, "inner": 6, "pal": {
        2: "#2a221e", 3: "#4e4236", 4: "#5e5042", 5: "#6e5e4c", 6: "#8a7860", 7: "#d2c2a2", 8: "#f0e8d8",
        12: "#5a3a30"}},
}

LUA = r"""
local spr = app.open(app.params.file)
local pal = spr.palettes[1]
for i, hex in pairs(PAL) do
  pal:setColor(i, Color{ r = tonumber(hex:sub(2,3),16), g = tonumber(hex:sub(4,5),16), b = tonumber(hex:sub(6,7),16), a = 255 })
end
local rim, inner = tonumber(app.params.rim), tonumber(app.params.inner)
local moved = 0
for _, cel in ipairs(spr.cels) do
  local src = cel.image
  local img = src:clone()
  local w, h = src.width, src.height
  local function clear(x, y)
    if x < 0 or y < 0 or x >= w or y >= h then return true end
    return src:getPixel(x, y) == 0
  end
  for y = 0, h - 1 do
    for x = 0, w - 1 do
      if src:getPixel(x, y) == rim and not (clear(x+1,y) or clear(x-1,y) or clear(x,y+1) or clear(x,y-1)) then
        img:drawPixel(x, y, inner)
        moved = moved + 1
      end
    end
  end
  cel.image = img
end
spr:saveAs(app.params.file)
print("moved " .. moved)
"""


def run(name):
    r = RECIPES[name]
    pal_lua = "local PAL = {" + ", ".join(f'[{i}] = "{h}"' for i, h in r["pal"].items()) + "}\n"
    with tempfile.NamedTemporaryFile("w", suffix=".lua", delete=False) as f:
        f.write(pal_lua + LUA)
        script = f.name
    path = os.path.join(ART, name + ".aseprite")
    out = subprocess.run([ASEPRITE, "-b", "--script-param", f"file={path}", "--script-param", f"rim={r['rim']}",
                          "--script-param", f"inner={r['inner']}", "--script", script],
                         capture_output=True, text=True)
    os.unlink(script)
    print(f"{name:8s} {out.stdout.strip()} {out.stderr.strip()}")


def add_master():
    p = os.path.join(ART, "palette_c.json")
    d = json.load(open(p))
    new = sorted({h for r in RECIPES.values() for h in r["pal"].values()} - set(d["master"]))
    d["hordes"] = sorted(set(d.get("hordes", [])) | set(new) | {h for r in RECIPES.values() for h in r["pal"].values()})
    d["master"] += new
    json.dump(d, open(p, "w"), indent=1)
    print(f"palette_c.json +{len(new)} hordes colours")


if __name__ == "__main__":
    for n in sys.argv[1:] or list(RECIPES):
        run(n)
    add_master()
