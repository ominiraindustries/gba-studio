"""Block 3 art (G-50, ahead of schedule): fx_root, fx_thorn, fx_seed, chest cursed frames, treant (from PixelLab).

    python -I art/tools/build_block3.py seed [names]   # new .aseprite files that are missing (never overwrites)
    python -I art/tools/build_block3.py extend         # chest +cursed_closed 2, +cursed_open 3 (backup in old_style/pre_g50/)
Then: python -I art/tools/build_art.py export
"""
import math
import os
import random
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_block2 import (ART, blank, put, get, rim, shift, line, disc, save, read_aseprite, pal, hexc,  # noqa: E402
                          backup)

# woods palette (roots, thorns, seeds, treant): bark teals + magenta hex glow
WOOD_HEX = ["#07060c", "#0c0d1c", "#1c242c", "#2a343d", "#1e434e", "#2a5d69", "#346675", "#4b8787", "#8ca4b4",
            "#c8dfeb", "#4b1530", "#c81c28", "#ff3838", "#ffa08c", "#34466e"]
WOOD = pal(WOOD_HEX)
# 1 black 2 deep 3 bark dark 4 bark 5 teal dark 6 teal 7 teal light 8 moss light 9 pale 10 white 11 wine
# 12 magenta 13 neon magenta 14 pink 15 cold rim


def fx_root():
    rnd = random.Random(5)
    frames = []
    # crack 0-1: ground crack glowing magenta (warning, 40 f); 1 = brighter and wider
    for k in range(2):
        m = blank(16, 16)
        pts = [(1, 11), (4, 10), (6, 12), (8, 10), (10, 11), (12, 9), (14, 10)]
        for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
            line(m, x0, y0, x1, y1, 1)
        for (x0, y0), (x1, y1) in (((6, 12), (5, 14)), ((10, 11), (11, 13)), ((8, 10), (8, 8))):
            line(m, x0, y0, x1, y1, 1)
        glow = [r[:] for r in m]
        for y in range(16):
            for x in range(16):
                if m[y][x] == 0 and any(get(m, x + dx, y + dy) == 1 for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    glow[y][x] = 12 if k == 0 else 13
                elif m[y][x] == 1 and k == 1 and rnd.random() < 0.35:
                    glow[y][x] = 14
        frames.append(glow)
    # spike 2-4: a root spike bursting up, full, then sinking
    def spike(h, crumbs):
        m = blank(16, 16)
        for i in range(h):
            y = 14 - i
            half = max(0, round(2.6 * (1 - i / h)))
            for dx in range(-half, half + 1):
                c = 4 if dx < 0 else (6 if dx == half else 5)
                put(m, 8 + dx + (1 if i > h * 0.6 else 0), y, c)
        tip = 14 - h
        put(m, 9, tip, 9)
        for x, y in ((4, 9), (12, 11)):     # side thorns
            if h > 8:
                put(m, x, y + (12 - h) // 2, 8); put(m, x + (1 if x < 8 else -1), y + (12 - h) // 2, 4)
        for x in range(4, 13):               # broken ground at the base
            put(m, x, 15, 3 if x % 3 else 1)
        for x, y in crumbs:
            put(m, x, y, 3)
        return rim(m, 15)
    frames += [spike(9, [(3, 13), (13, 12)]), spike(13, [(2, 12), (14, 11), (4, 10)]), spike(6, [(3, 14), (12, 14)])]
    save("fx_root", 16, 16, WOOD, frames, [200, 200, 50, 300, 90], [("crack", 0, 1), ("spike", 2, 4)])


def fx_thorn():
    frames = []
    for k in range(2):
        m = blank(16, 16)
        # two intertwined bramble vines across the tile (seamless left-right at rows 7-9)
        for x in range(16):
            y1 = 8 + round(2.5 * math.sin((x + k) * 0.785))
            y2 = 8 + round(2.5 * math.sin((x + k) * 0.785 + 3.14))
            put(m, x, y1, 4); put(m, x, y1 - 1, 6)
            put(m, x, y2, 3); put(m, x, y2 + 1, 5)
        # thorns: pale tips with a magenta glint
        for i, (x, y, dx, dy) in enumerate(((2, 4, 0, -1), (6, 12, 0, 1), (10, 4, 0, -1), (14, 12, 0, 1),
                                             (4, 9, -1, 1), (12, 7, 1, -1))):
            yy = y + (k if dy > 0 else -k) * 0
            put(m, x, yy, 8); put(m, x + dx, yy + dy, 9)
            if (i + k) % 3 == 0:
                put(m, x + dx, yy + dy, 13)
        frames.append(rim(m, 1))
    save("fx_thorn", 16, 16, WOOD, frames, [250, 250], [("idle", 0, 1)])


def fx_seed():
    frames = []
    for k in range(2):
        m = blank(8, 8)
        disc(m, 3.5, 3.5, 2.6, 5); disc(m, 3.2, 3.2, 1.6, 7); put(m, 3, 2, 9)
        put(m, 4, 4, 12); put(m, 4, 5, 13) if k else put(m, 5, 4, 13)
        # leaf sprout rotates
        if k == 0:
            put(m, 6, 1, 8); put(m, 7, 0, 7)
        else:
            put(m, 1, 6, 8); put(m, 0, 7, 7)
        frames.append(rim(m, 1))
    save("fx_seed", 8, 8, WOOD, frames, [120, 120], [("fly", 0, 1)])


def treant():
    """From art/raw/treant/: idle_*.png, walk_*.png, slam_*.png, death_*.png (PixelLab, 64x64), quantized to WOOD."""
    raw = os.path.join(ART, "raw", "treant")
    seq = []
    for anim, n in (("idle", 4), ("walk", 4), ("slam", 3)):
        files = sorted(f for f in os.listdir(raw) if f.startswith(anim + "_") and f.endswith(".png"))
        pick = [files[round(i * (len(files) - 1) / max(1, n - 1))] for i in range(n)] if len(files) > n else files
        assert len(pick) == n, (anim, files)
        seq += [Image.open(os.path.join(raw, f)).convert("RGBA") for f in pick]
    frames = [quant(im) for im in seq]
    # common bottom anchor: feet on the last row (one offset for all frames)
    bottom = max(max((y for y in range(64) if any(f[y])), default=0) for f in frames)
    frames = [shift(f, 0, 63 - bottom) for f in frames]
    frames = [rim(f, 15) for f in frames]   # style C cold rim
    # hurt 11: idle 0 flashed (pale bark, white eyes)
    flash = {3: 6, 4: 7, 5: 8, 6: 9, 2: 5, 12: 10, 13: 10, 11: 13}
    frames.append([[flash.get(c, c) for c in r] for r in frames[0]])
    # death 12-15: sinks and crumbles into wood chips (dithered), magenta glow fades out
    rnd = random.Random(2)
    base = frames[0]
    for k in range(4):
        f = shift(base, 0, 4 * (k + 1))
        keep = 1 - 0.22 * (k + 1)
        f = [[(c if (c and rnd.random() < keep) else 0) for c in r] for r in f]
        if k >= 1:
            f = [[(4 if c in (11, 12, 13) else c) for c in r] for r in f]
        frames.append(f)
    durs = [180] * 4 + [160] * 4 + [150, 120, 300] + [120] + [140] * 4
    save("treant", 64, 64, WOOD, frames, durs,
         [("idle", 0, 3), ("walk", 4, 7), ("slam", 8, 10), ("hurt", 11, 11), ("death", 12, 15)])


def quant(im):
    """RGBA 64x64 -> WOOD indices: nearest colour by luminance-weighted RGB, alpha < 128 = transparent;
    saturated magenta-ish pixels map to the magenta ramp, the rest to the bark/teal ramp."""
    im = im.resize((64, 64), Image.NEAREST) if im.size != (64, 64) else im
    out = blank(64, 64)
    for y in range(64):
        for x in range(64):
            r, g, b, a = im.getpixel((x, y))
            if a < 128:
                continue
            magenta = r > g + 40 and b > g + 20
            cands = range(11, 15) if magenta else list(range(1, 11)) + [15]
            best = min(cands, key=lambda i: 2 * (WOOD[i][0] - r) ** 2 + 4 * (WOOD[i][1] - g) ** 2 + 3 * (WOOD[i][2] - b) ** 2)
            out[y][x] = best
    return out


def extend_chest():
    w, h, p, frames, durs, tags = read_aseprite("chest")
    if any(t[0] == "cursed_closed" for t in tags):
        print("chest already extended"); return
    backup("chest")
    # recogibles: 1 #07060c 8 #46155a 9 #e63c3c 10 #ff3838 11 #4b1530 12 #ffc838 13 #fff4b0 15 #2a343d
    recol = {15: 8, 11: 1, 12: 15, 13: 9}   # dark purple body, black planks, slate iron, red studs
    closed = [[recol.get(c, c) for c in r] for r in frames[0]]
    # red rune on the front (rows 9-12) and a magenta lock
    for x, y in ((5, 10), (6, 9), (6, 11), (7, 10), (8, 10), (9, 9), (9, 11), (10, 10)):
        put(closed, x, y, 9)
    put(closed, 7, 8, 10); put(closed, 8, 8, 10)
    opened = [[recol.get(c, c) for c in r] for r in frames[1]]
    for x, y in ((6, 3), (7, 2), (9, 1), (10, 3), (5, 1), (8, 4)):   # curse wisps rising out of the lid
        put(opened, x, y, 10 if (x + y) % 2 else 8)
    for x, y in ((4, 11), (5, 10), (6, 11), (7, 10)):
        put(opened, x, y, 9)
    frames += [closed, opened]
    durs += [100, 100]
    tags += [("cursed_closed", 2, 2), ("cursed_open", 3, 3)]
    save("chest", 16, 16, p, frames, durs, tags)


NEW = {"fx_root": fx_root, "fx_thorn": fx_thorn, "fx_seed": fx_seed, "treant": treant}

if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        for name, fn in NEW.items():
            if len(sys.argv) > 2 and name not in sys.argv[2:]:
                continue
            if os.path.exists(os.path.join(ART, name + ".aseprite")):
                print("exists, skipped:", name)
                continue
            fn()
    elif cmd == "extend":
        extend_chest()
