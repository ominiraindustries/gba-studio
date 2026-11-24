"""Block 7 art (G-51, ahead of schedule): F3 Ashen Keep and ending.

    python -I art/tools/build_block7.py fetch            # PixelLab sources -> art/raw/keep/
    python -I art/tools/build_block7.py seed [names]     # F3 palettes, fx_fireball, fx_breath, fx_slash, ember_dragon,
                                                         # ending_bg (creates missing files, never overwrites)
    python -I art/tools/build_block7.py props            # adds brazier, banner, lava_crack, broken_throne to props
                                                         # (copy in old_style/pre_g51/; no-op once done)
Then: python -I art/tools/build_art.py export
"""
import json
import math
import os
import random
import sys
import urllib.request

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_block2 import ART, blank, put, get, rim, shift, disc, save, pal, hexc, read_aseprite, FIRE_PAL  # noqa: E402
from build_block4 import line, rect, recolour, backup, nearest_master, MASTER  # noqa: E402
from build_block5 import dissolve, to_indices  # noqa: E402
from build_hordes import tri, quant  # noqa: E402

RAW = os.path.join(ART, "raw", "keep")
GFX = os.path.join(os.path.dirname(ART), "graphics")

# ---------------------------------------------------------------- F3 palettes (same tiles, other palette)
# brasa = dark grey body with hot red/amber cracks (#ff7a2a of the design is not in the master palette: closest
# master colours #e63c3c / #a06818 / #ffc838 are used); the cold rim and index 0 stay.
F3_PALETTES = {
    "cinder_slime": ("slime", {1: "#0c0d1c", 2: "#1c242c", 3: "#2a343d", 4: "#36435b", 5: "#4b1530", 6: "#e63c3c",
                               7: "#a06818"}),
    "ash_bat": ("bat", {6: "#1c242c", 7: "#4b1530", 8: "#2a343d", 12: "#ffc838", 9: "#bc161b", 10: "#e63c3c",
                        13: "#36435b", 14: "#8ca4b4", 15: "#abc4d2"}),
    "cultist": ("goblin", {4: "#1c242c", 5: "#2a343d", 7: "#2a343d", 8: "#36435b", 9: "#8ca4b4", 12: "#8ca4b4",
                           14: "#ffc838"}),
    "ash_knight": ("orc", {7: "#34466e", 8: "#2f3551", 9: "#36435b", 12: "#8ca4b4", 11: "#34466e", 13: "#8ca4b4",
                           14: "#40e8f8", 6: "#21263b"}),
    "ember_warden": ("skeleton_big", {8: "#4b1530", 10: "#bc161b", 11: "#36435b", 12: "#8ca4b4", 9: "#ffc838",
                                      2: "#190922", 6: "#2a343d"}),
    "hellhound": ("wolf", {3: "#1c242c", 5: "#2a343d", 6: "#4b1530", 8: "#a06818", 13: "#e63c3c", 12: "#bc161b"}),
    "iron_champion": ("orc_warchief", {8: "#34466e", 2: "#0c1626", 5: "#21263b", 10: "#40e8f8", 15: "#1a7f9c",
                                       3: "#21263b", 4: "#2f3551"}),
}


def f3_palettes():
    for name, (base, mapping) in F3_PALETTES.items():
        if os.path.exists(os.path.join(ART, name + "_palette.aseprite")):
            print("exists, skipped:", name); continue
        w, h, p, fr, durs, tags = read_aseprite(base)
        p = (list(p) + [(0, 0, 0)] * 16)[:16]
        save(name + "_palette", w, h, recolour(p, mapping), fr[:4], [150] * 4, [("preview", 0, 3)])


# ---------------------------------------------------------------- FX (fire palette of build_block2)
# FIRE_PAL: 1 #07060c 2 #190922 3 #4b1530 4 #bc161b 5 #e63c3c 6 #a06818 7 #ffc838 8 #fff4b0 9 #c8dfeb 10-12 smoke
# 13-15 magentas
def fx_fireball():
    frames = []
    for k in range(2):
        m = blank(8, 8)
        for i, (x, y) in enumerate(((0, 4 + k % 2), (1, 4 - k % 2), (2, 4))):
            put(m, x, y, 4 if i < 2 else 5)
        disc(m, 4.5, 3.8, 2.4, 5); disc(m, 4.8, 3.6, 1.5, 7); put(m, 5, 3, 8)
        if k:
            put(m, 3, 1, 6); put(m, 6, 6, 6)
        frames.append(rim(m, 3))
    save("fx_fireball", 8, 8, FIRE_PAL, frames, [70, 70], [("fly", 0, 1)])


def fx_breath():
    """16x16 cone piece (tiled along the breath): flame tongues licking right."""
    rnd = random.Random(70)
    frames = []
    for k in range(3):
        m = blank(16, 16)
        for x in range(16):
            half = 3 + x * 0.3 + math.sin((x + k * 3) * 0.9) * 1.2
            for y in range(16):
                d = abs(y - 7.5)
                if d <= half:
                    c = 8 if d < half * 0.25 else (7 if d < half * 0.5 else (5 if d < half * 0.8 else 4))
                    if d > half - 1 and rnd.random() < 0.4:
                        continue
                    put(m, x, y, c)
        for _ in range(4):
            put(m, rnd.randrange(16), rnd.choice((1, 2, 13, 14)), 6)
        frames.append(m)
    save("fx_breath", 16, 16, FIRE_PAL, frames, [70, 70, 70], [("burn", 0, 2)])


def fx_slash():
    """Champion's slash, facing right: 0 = warning (thin red arc), 1 = full white-hot arc, 2 = fading."""
    frames = []
    for k in range(3):
        m = blank(32, 32)
        r_out, r_in = (14.5, 13.5) if k == 0 else ((15.5, 9) if k == 1 else (15.5, 12))
        for y in range(32):
            for x in range(32):
                dx, dy = x - 8 + 0.5, y - 16 + 0.5
                r = math.hypot(dx, dy)
                ang = math.degrees(math.atan2(dy, dx))
                if -60 <= ang <= 60 and r_in <= r <= r_out:
                    if k == 0:
                        c = 5 if (x + y) % 2 == 0 else 4
                    elif k == 1:
                        c = 8 if r > r_out - 2 else (9 if r > r_out - 3.5 else (5 if r > r_in + 1.5 else 4))
                    else:
                        if (x + y) % 2:
                            continue
                        c = 5 if r > r_out - 2 else 3
                    put(m, x, y, c)
        frames.append(m)
    save("fx_slash", 32, 32, FIRE_PAL, frames, [500, 60, 80], [("arc", 0, 2)])


# ---------------------------------------------------------------- PixelLab sources
KEEP_OBJ = "5a69c2af-b84b-42e3-b29a-fa9d11c7ecac"


def fetch(urls):
    os.makedirs(RAW, exist_ok=True)
    for name, url in urls.items():
        req = urllib.request.Request(url, headers={"User-Agent": "gba-art-fetch/1.0"})
        with open(os.path.join(RAW, name), "wb") as f:
            f.write(urllib.request.urlopen(req, timeout=120).read())
        print("fetched", name)


DRAGON = pal(["#07060c", "#0c0d1c", "#190922", "#2a012f", "#4b1530", "#1c242c", "#2a343d", "#36435b", "#bc161b",
              "#e63c3c", "#a06818", "#ffc838", "#fff4b0", "#8ca4b4", "#34466e"])
# 1 black 2-5 dark wine 6-8 ash greys 9-10 reds 11-13 ambers/gold 14 bone 15 cold rim


def ember_dragon():
    d = os.path.join(RAW, "dragon")
    idle = [Image.open(os.path.join(d, f"idle_{i}.png")).convert("RGBA") for i in range(4)]
    breath = [Image.open(os.path.join(d, f"breath_{i}.png")).convert("RGBA") for i in range(3)]
    fly = [Image.open(os.path.join(d, f"fly_{i}.png")).convert("RGBA") for i in range(2)]
    frames, box = to_indices(idle + breath + fly, DRAGON, 64)
    idl, br, fl = frames[:4], frames[4:7], frames[7:9]
    # breath: head glow grows (DISENO: cabeza brilla 40 f antes del cono)
    for k, f in enumerate(br):
        ys = [y for y in range(64) if any(f[y])]
        top = ys[0] if ys else 8
        cx = 32
        hot = {9: 11, 10: 12} if k == 0 else {9: 12, 10: 13, 11: 13}
        for y in range(top, min(64, top + 22)):      # head glows hotter frame by frame (warning)
            f[y] = [hot.get(c, c) for c in f[y]]
    fl = [shift(f, 0, -6) for f in fl]                      # airborne
    land = shift(idl[0], 0, 0)
    for x in range(10, 54):                                 # dust ring on landing
        if (x // 3) % 2:
            put(land, x, 63, 7); put(land, x, 62, 6)
    rnd = random.Random(71)
    base = idl[0]
    death = [dissolve(base, k, 5, rnd, up=0, recol=({9: 7, 10: 6, 11: 7, 12: 13} if k >= 1 else None))
             for k in range(5)]
    for k in range(1, 5):
        death[k] = shift(death[k], 0, 2 * k)
    frames = [rim(f, 15) for f in idl + br + fl + [land]] + death
    save("ember_dragon", 64, 64, DRAGON, frames, [150] * 4 + [300, 200, 120] + [100, 100, 200] + [90, 100, 110, 130, 160],
         [("idle", 0, 3), ("breath", 4, 6), ("fly", 7, 8), ("land", 9, 9), ("death", 10, 14)])


def ending_bg():
    """8 bpp 256x256 (image at 0,0-239,159), snapped to the master palette (like title_bg)."""
    src = Image.open(os.path.join(RAW, "ending.png")).convert("RGB").resize((240, 160), Image.NEAREST)
    src = src.point(lambda v: round(255 * (v / 255) ** 1.8))    # darker, like title_bg (style C night)
    cols = {}
    for c in src.getdata():
        cols.setdefault(c, nearest_master(c))
    used = sorted(set(cols.values()), key=lambda h: sum(hexc(h)))
    palette = [(0, 0, 0)] + [hexc(h) for h in used]
    idx = {h: i + 1 for i, h in enumerate(used)}
    m = blank(256, 256)
    for y in range(160):
        for x in range(240):
            m[y][x] = idx[cols[src.getpixel((x, y))]]
    from aseprite_io import write_aseprite
    write_aseprite(os.path.join(ART, "ending_bg.aseprite"), 256, 256, palette + [(0, 0, 0)] * (256 - len(palette)),
                   [m], [100], [])
    print("wrote ending_bg", len(used), "colours")


# ---------------------------------------------------------------- props + 4 F3 pieces
NEW_PROPS = [("brazier", 0, 136, 32, 32), ("banner", 32, 136, 16, 32), ("lava_crack", 48, 136, 32, 32),
             ("broken_throne", 80, 136, 64, 64)]


def props():
    layout_path = os.path.join(ART, "props_layout.json")
    lay = json.load(open(layout_path))
    if any(e["name"] == "brazier" for e in lay):
        print("props already extended"); return
    w, h, p, fr, durs, tags = read_aseprite("props")
    backup("props")
    shutil_copy = os.path.join(ART, "old_style", "pre_g51", "props_layout.json")
    if not os.path.exists(shutil_copy):
        json.dump(lay, open(shutil_copy, "w"), indent=1)
    keep = [hexc(c) for c in json.load(open(os.path.join(ART, "..", "art", "palette_c.json")))["master"][:0]]  # noqa
    # quantise the new pieces against the F3 (keep) props palette so they read right where they are used
    from build_block4 import PROPS_PALETTES
    kp = [hexc(c) for c in PROPS_PALETTES["props_keep"]]
    big = [r[:] + [0] * (256 - len(r)) for r in fr[0]] + [[0] * 256 for _ in range(200 - h)]
    for name, x, y, pw, ph in NEW_PROPS:
        im = Image.open(os.path.join(RAW, name + ".png")).convert("RGBA")
        if name == "lava_crack":       # the source is a square floor tile: keep the glowing fissure + 1 px of rock
            px = im.load()
            hot = {(x, y) for y in range(im.height) for x in range(im.width)
                   if px[x, y][3] > 128 and px[x, y][0] > 110}
            near = {(x + dx, y + dy) for (x, y) in hot for dx in (-1, 0, 1) for dy in (-1, 0, 1)}
            for y in range(im.height):
                for x in range(im.width):
                    if (x, y) not in near:
                        px[x, y] = (0, 0, 0, 0)
        bb = im.getbbox()
        im = im.crop(bb)
        s = min(pw / im.width, ph / im.height, 1.0)
        if s < 1.0:
            im = im.resize((max(1, round(im.width * s)), max(1, round(im.height * s))), Image.NEAREST)
        q = quant(im, kp, max(pw, ph))
        # quant centres into a square, bottom-anchored: take the pw x ph bottom-centre window
        off = (max(pw, ph) - pw) // 2
        for yy in range(ph):
            for xx in range(pw):
                v = q[max(pw, ph) - ph + yy][off + xx]
                if v == 1:                  # generator outline -> props darkest
                    v = 2
                big[y + yy][x + xx] = v
        lay.append({"name": name, "x": x, "y": y, "w": pw, "h": ph})
    save("props", 256, 200, p, [big], [100], tags)
    json.dump(lay, open(layout_path, "w"), indent=1)


NEW = {"fx_fireball": fx_fireball, "fx_breath": fx_breath, "fx_slash": fx_slash, "ember_dragon": ember_dragon,
       "ending_bg": ending_bg}

if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        only = sys.argv[2:]
        if not only or "palettes" in only:
            f3_palettes()
        for name, fn in NEW.items():
            if only and name not in only:
                continue
            if os.path.exists(os.path.join(ART, name + ".aseprite")):
                print("exists, skipped:", name); continue
            fn()
    elif cmd == "props":
        props()
