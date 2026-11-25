"""Block 2 art (G-50): big_slime, wolf, fx_ember/explosion/fire/chain/dust, hud_bossbar (new files) and the
additions to orc_warchief (stomp, horn, attack re-anchor) and icons (ember_toss, storm_chain).

    python -I art/tools/build_block2.py seed [names]   # creates the NEW .aseprite files that are missing (never overwrites)
    python -I art/tools/build_block2.py extend         # appends frames to orc_warchief / icons (backup in old_style/pre_g50/;
                                                       # skipped if the file already has the new tags)
Then: python -I art/tools/build_art.py export

Everything is drawn in code with colours from palette_c.json (style C). Index 0 = transparent.
"""
import json
import math
import os
import random
import shutil
import subprocess
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import aseprite_io  # noqa: E402

TOOLS = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(TOOLS)
ASEPRITE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
MASTER = set(json.load(open(os.path.join(ART, "palette_c.json")))["master"])


def hexc(h):
    assert h in MASTER, h
    return (int(h[1:3], 16), int(h[3:5], 16), int(h[5:7], 16))


def pal(colours):
    p = [(0, 0, 0)] + [hexc(c) for c in colours]
    assert len(p) <= 16
    return p


def blank(w, h):
    return [[0] * w for _ in range(h)]


def put(px, x, y, c):
    if 0 <= y < len(px) and 0 <= x < len(px[0]):
        px[y][x] = c


def get(px, x, y):
    if 0 <= y < len(px) and 0 <= x < len(px[0]):
        return px[y][x]
    return 0


def rim(px, c, over=(0,)):
    """1 px outline (4-neighbour) around the non-transparent shape."""
    h, w = len(px), len(px[0])
    out = [r[:] for r in px]
    for y in range(h):
        for x in range(w):
            if px[y][x] in over and px[y][x] == 0:
                if any(get(px, x + dx, y + dy) not in (0,) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    out[y][x] = c
    return out


def shift(px, dx, dy):
    h, w = len(px), len(px[0])
    out = blank(w, h)
    for y in range(h):
        for x in range(w):
            if px[y][x]:
                put(out, x + dx, y + dy, px[y][x])
    return out


def line(px, x0, y0, x1, y1, c, thick=1):
    n = max(abs(x1 - x0), abs(y1 - y0), 1)
    for i in range(n + 1):
        x = round(x0 + (x1 - x0) * i / n); y = round(y0 + (y1 - y0) * i / n)
        for t in range(thick):
            put(px, x + t, y, c)


def disc(px, cx, cy, r, c, only_empty=False):
    for y in range(int(cy - r - 1), int(cy + r + 2)):
        for x in range(int(cx - r - 1), int(cx + r + 2)):
            if (x - cx) ** 2 + (y - cy) ** 2 <= r * r:
                if not only_empty or get(px, x, y) == 0:
                    put(px, x, y, c)


def save(name, w, h, palette, frames, durations, tags):
    path = os.path.join(ART, name + ".aseprite")
    for f in frames:
        assert len(f) == h and all(len(r) == w for r in f), name
    aseprite_io.write_aseprite(path, w, h, palette, frames, durations, tags)
    print(f"wrote {name}: {w}x{h} x{len(frames)} tags {tags}")


def read_aseprite(name):
    """(w, h, palette, frames, durations, tags) via Aseprite batch export."""
    tmp = os.path.join(ART, "raw", "_export")
    os.makedirs(tmp, exist_ok=True)
    png = os.path.join(tmp, name + "_rd.png"); data = os.path.join(tmp, name + "_rd.json")
    subprocess.run([ASEPRITE, "-b", os.path.join(ART, name + ".aseprite"), "--sheet-type", "vertical", "--sheet", png,
                    "--data", data, "--format", "json-array", "--list-tags"], check=True, capture_output=True)
    im = Image.open(png); meta = json.load(open(data))
    assert im.mode == "P"
    fw = meta["frames"][0]["frame"]["w"]; fh = meta["frames"][0]["frame"]["h"]
    p = im.getpalette()
    ncol = min(16, len(p) // 3)
    palette = [tuple(p[i * 3:i * 3 + 3]) for i in range(ncol)]
    px = list(im.getdata())
    frames = []
    for k in range(len(meta["frames"])):
        frames.append([px[(k * fh + y) * fw:(k * fh + y) * fw + fw] for y in range(fh)])
    durations = [f["duration"] for f in meta["frames"]]
    tags = [(t["name"], t["from"], t["to"]) for t in meta["meta"].get("frameTags", [])]
    return fw, fh, palette, frames, durations, tags


# ---------------------------------------------------------------- big_slime (2x nearest of slime, cleaned)

def big_slime():
    w, h, p, frames, durs, tags = read_aseprite("slime")
    gold = 7
    out = []
    for f in frames:
        big = [[f[y // 2][x // 2] for x in range(32)] for y in range(32)]
        # source outline pixels: gold touching transparency (or the canvas edge)
        edge = [[f[y][x] == gold and any(get(f, x + dx, y + dy) == 0 or not (0 <= x + dx < 16 and 0 <= y + dy < 16)
                                          for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))
                 for x in range(16)] for y in range(16)]
        thin = [r[:] for r in big]
        for y in range(32):
            for x in range(32):
                if big[y][x] == gold and edge[y // 2][x // 2]:
                    if not any(get(big, x + dx, y + dy) == 0 or not (0 <= x + dx < 32 and 0 <= y + dy < 32)
                               for dx in (-1, 0, 1) for dy in (-1, 0, 1)):
                        nb = [get(big, x + dx, y + dy) for dx in (-2, -1, 0, 1, 2) for dy in (-2, -1, 0, 1, 2)]
                        nb = [c for c in nb if c not in (0, gold)]
                        thin[y][x] = max(set(nb), key=nb.count) if nb else 2
        # break the 2x2 blockiness of interior colour steps: dither single diagonal steps
        for y in range(1, 31):
            for x in range(1, 31):
                c = thin[y][x]
                if c not in (0, gold) and (x + y) % 2 == 0:
                    r, d = thin[y][x + 1], thin[y + 1][x]
                    if r == d and r not in (0, gold, c) and thin[y][x - 1] == c and thin[y - 1][x] == c:
                        thin[y][x] = r
        out.append(thin)
    # extra shine on the living frames (top-left highlight, 2 small bubbles)
    for k in range(4):
        f = out[k]
        ys = [y for y in range(32) if any(f[y])]
        top = ys[0]
        for (dx, dy, c) in ((9, 4, 6), (10, 4, 6), (9, 5, 6), (11, 3, 6)):
            if f[top + dy][dx] not in (0, gold):
                f[top + dy][dx] = c
        for (bx, by) in ((20, top + 13 + (k % 2)), (12, top + 17 - (k % 2))):
            if f[by][bx] not in (0, gold):
                f[by][bx] = 5
    save("big_slime", 32, 32, p, out, durs, tags)


# ---------------------------------------------------------------- wolf 32x16, faces right

WOLF_PAL = pal(["#07060c", "#0c0d1c", "#191935", "#21263b", "#2f3551", "#36435b", "#34466e", "#8ca4b4", "#c8dfeb",
                "#e63c3c", "#bc161b", "#4b1530", "#ffc838"])
# 1 black 2 deep 3 body 4 body2 5 back 6 back light 7 rim 8 pale fur 9 white 10 red 11 dark red 12 ear 13 eye gold
W_SPANS = {0: [(24, 24), (27, 27)], 1: [(23, 24), (26, 27)], 2: [(22, 28)], 3: [(1, 2), (21, 28)],
           4: [(2, 4), (8, 29)], 5: [(3, 30)], 6: [(5, 30)], 7: [(6, 29)], 8: [(6, 26)], 9: [(7, 22)],
           10: [(8, 12), (16, 21)]}


def wolf_body(head_dy=0, tail_dy=0):
    m = blank(32, 16)
    for y, spans in W_SPANS.items():
        for a, b in spans:
            for x in range(a, b + 1):
                yy = y
                if x >= 21:
                    yy += head_dy
                if x <= 6:
                    yy += tail_dy
                put(m, x, yy, 3)
    return m


def shade(m):
    h, w = len(m), len(m[0])
    for x in range(w):
        ys = [y for y in range(h) if m[y][x] == 3]
        if not ys:
            continue
        put(m, x, ys[0], 6)
        if len(ys) > 2:
            put(m, x, ys[1], 5)
        if len(ys) > 3:
            put(m, x, ys[-1], 2)
    return m


def leg(m, ax, ay, kx, ky, fx, fy, far):
    c = 2 if far else 5
    line(m, ax, ay, kx, ky, c, 2)
    line(m, kx, ky, fx, fy, c, 1 if far else 2)
    if not far:
        line(m, kx + 1, ky, fx + 1, fy, 6)        # lit front edge of the near legs
    put(m, fx, fy, 1 if far else 8)
    if not far:
        put(m, fx + 1, fy, 8)


def wolf_frame(body_dy, legs, head_dy=0, tail_dy=0, eye="pale", mouth=True):
    m = wolf_body(head_dy, tail_dy)
    m = shift(m, 0, 1 + body_dy)
    m = shade(m)
    # far legs first (behind), then near legs
    for far in (True, False):
        for (ax, kx, ky, fx, fy, is_far) in legs:
            if is_far == far:
                leg(m, ax, 11 + body_dy, kx, ky, fx, fy, far)
    hy = 1 + body_dy + head_dy
    put(m, 24, hy + 1, 12); put(m, 27, hy + 1, 12)            # ear insides
    put(m, 30, hy + 6, 1); put(m, 31, hy + 6, 0)              # nose
    if mouth:
        put(m, 27, hy + 7, 11); put(m, 28, hy + 7, 11); put(m, 29, hy + 7, 9); put(m, 26, hy + 7, 1)
    if eye == "pale":
        put(m, 26, hy + 4, 13)
    else:   # red glare (warning)
        put(m, 26, hy + 4, 10); put(m, 27, hy + 4, 10); put(m, 25, hy + 4, 11); put(m, 26, hy + 3, 11)
    return rim(m, 7)


def wolf():
    # (attach_x, knee_x, knee_y, foot_x, foot_y, far)
    run = [
        (0, [(10, 7, 13, 4, 15, True), (11, 8, 13, 6, 15, False), (19, 23, 13, 25, 15, True), (20, 24, 12, 27, 14, False)], 0),
        (-1, [(10, 10, 13, 12, 15, True), (11, 12, 13, 14, 15, False), (19, 19, 13, 17, 15, True), (20, 20, 13, 19, 15, False)], 0),
        (0, [(10, 13, 13, 15, 15, True), (11, 14, 12, 16, 14, False), (19, 17, 13, 14, 15, True), (20, 18, 13, 16, 15, False)], 1),
        (-1, [(10, 8, 13, 7, 15, True), (11, 9, 13, 9, 15, False), (19, 21, 13, 22, 15, True), (20, 22, 13, 24, 15, False)], 0),
    ]
    frames = [wolf_frame(dy, legs, tail_dy=t) for dy, legs, t in run]
    # crouch: body low, head lower, wide stance, red eyes
    frames.append(wolf_frame(2, [(10, 8, 14, 6, 15, True), (11, 9, 14, 8, 15, False), (19, 21, 14, 23, 15, True),
                                 (20, 22, 14, 25, 15, False)], head_dy=1, tail_dy=1, eye="red"))
    # dash: stretched, legs flung out, red eyes, cold speed streaks behind
    d = wolf_frame(0, [(10, 6, 12, 2, 13, True), (11, 7, 13, 3, 14, False), (19, 24, 12, 28, 12, True),
                       (20, 25, 13, 29, 13, False)], head_dy=1, tail_dy=-1, eye="red")
    for y, x0, x1 in ((6, 0, 2), (9, 0, 3), (12, 0, 1)):
        for x in range(x0, x1 + 1):
            if d[y][x] == 0:
                d[y][x] = 7
    frames.append(d)
    # death: hit (recoil), on its back, dissolving
    hit = wolf_frame(1, [(10, 8, 13, 5, 14, True), (11, 9, 13, 7, 15, False), (19, 22, 13, 25, 13, True),
                         (20, 23, 14, 26, 14, False)], head_dy=2, tail_dy=1, eye="red", mouth=True)
    frames.append(hit)
    base = wolf_frame(0, [(10, 10, 13, 10, 15, True), (11, 11, 13, 11, 15, False), (19, 19, 13, 19, 15, True),
                          (20, 20, 13, 20, 15, False)], mouth=False)
    upside = [list(reversed(r)) for r in reversed(base)]     # rotated 180: legs up, head on the left
    upside = [r[:] for r in upside]
    # squash into the bottom rows: keep rows 0..11 of the rotated shape (body+legs stubs), drop the ears
    lying = blank(32, 16)
    rows = [r for r in upside if any(r)]
    rows = rows[:12]
    for i, r in enumerate(rows):
        lying[16 - len(rows) + i] = r[:]
    # cross the eye: rotated eye position -> paint any pale/red eye pixels as 'x'
    for y in range(16):
        for x in range(32):
            if lying[y][x] in (13, 10, 11):
                lying[y][x] = 1
    frames.append(lying)
    rnd = random.Random(8)
    fade = [[c if c and rnd.random() < 0.45 else 0 for c in r] for r in lying]
    fade = [[(7 if c in (2, 3, 4, 5, 6) else c) for c in r] for r in fade]
    frames.append(fade)
    durs = [80] * 4 + [500, 120, 100, 160, 160]
    save("wolf", 32, 16, WOLF_PAL, frames, durs, [("run", 0, 3), ("crouch", 4, 4), ("dash", 5, 5), ("death", 6, 8)])


# ---------------------------------------------------------------- fire FX palette (ember, explosion, fire, dust)

FIRE_PAL = pal(["#07060c", "#190922", "#4b1530", "#bc161b", "#e63c3c", "#a06818", "#ffc838", "#fff4b0", "#c8dfeb",
                "#2a343d", "#36435b", "#8ca4b4", "#c81c28", "#ff3838", "#ffa08c"])
# 1 black 2 deep purple 3 wine 4 dark red 5 red 6 amber 7 gold 8 pale gold 9 white 10 smoke dark 11 smoke 12 dust
# 13 magenta 14 neon magenta 15 pink


def fx_ember():
    frames = []
    for k in range(2):
        m = blank(8, 8)
        disc(m, 3.5, 3.5, 3.2, 5)
        disc(m, 3.5, 3.5, 2.3, 7)
        disc(m, 3.2 + 0.3 * k, 3.2, 1.2, 8)
        put(m, 3, 3, 9)
        # flame licks rotate between frames
        tips = ((0, 2), (7, 5), (5, 0), (2, 7)) if k == 0 else ((2, 0), (7, 2), (0, 5), (5, 7))
        for x, y in tips:
            put(m, x, y, 4)
        for x, y in ((1, 1), (6, 6)) if k == 0 else ((6, 1), (1, 6)):
            if m[y][x] == 5:
                m[y][x] = 4
        frames.append(m)
    save("fx_ember", 8, 8, FIRE_PAL, frames, [70, 70], [("fly", 0, 1)])


def fx_explosion():
    rnd = random.Random(3)
    frames = []
    c = 15.5
    # 0: flash core
    m = blank(32, 32); disc(m, c, c, 5, 7); disc(m, c, c, 3.5, 8); disc(m, c, c, 2, 9)
    for a in range(8):
        ang = a * math.pi / 4
        for r in range(6, 9):
            put(m, round(c + r * math.cos(ang)), round(c + r * math.sin(ang)), 7 if r < 8 else 6)
    frames.append(m)
    # 1: fireball
    m = blank(32, 32); disc(m, c, c, 10, 4); disc(m, c, c, 9, 5); disc(m, c, c, 7, 6); disc(m, c, c, 5, 7); disc(m, c, c, 3, 8)
    frames.append(m)
    # 2: ring with smoke core
    m = blank(32, 32); disc(m, c, c, 13.5, 4); disc(m, c, c, 12.5, 5); disc(m, c, c, 11, 6); disc(m, c, c, 9.5, 5)
    disc(m, c, c, 8, 3); disc(m, c, c, 6, 2)
    for _ in range(10):
        a = rnd.random() * 6.283; r = 7 + rnd.random() * 4
        disc(m, c + r * math.cos(a), c + r * math.sin(a), 1.2, 7)
    frames.append(m)
    # 3: smoke clouds (overlapping puffs, lit top-left) with a few embers
    m = blank(32, 32)
    puffs = [(c + 10.5 * math.cos(i * 6.283 / 10 + 0.3), c + 10.5 * math.sin(i * 6.283 / 10 + 0.3), 3.4 - (i % 2) * 0.6)
             for i in range(10)] + [(c - 3, c - 2, 3.2), (c + 3, c + 2, 3)]
    for x, y, r in puffs:
        disc(m, x, y, r, 10)
    for x, y, r in puffs:
        disc(m, x - 0.8, y - 0.8, r * 0.55, 11)
    for x, y, r in puffs[::3]:
        put(m, round(x - 1), round(y - 1), 12)
    for x, y, r in puffs[1::4]:
        put(m, round(x + 1), round(y + 1), 5); put(m, round(x + 2), round(y + 1), 4)
    frames.append(rim(m, 1))
    # 4: dispersing smoke (dithered, wider)
    m = blank(32, 32)
    for i in range(12):
        a = i * 6.283 / 12 + 0.1
        r = 12.5 + (i % 2)
        disc(m, c + r * math.cos(a), c + r * math.sin(a), 2.4, 10)
        disc(m, c + r * math.cos(a) - 0.6, c + r * math.sin(a) - 0.6, 1.0, 11)
    for y in range(32):
        for x in range(32):
            if m[y][x] and (x + y) % 2:
                m[y][x] = 0
    frames.append(m)
    frames = [rim(f, 1) if i < 3 else f for i, f in enumerate(frames)]  # 3 is rimmed above
    save("fx_explosion", 32, 32, FIRE_PAL, frames, [50, 60, 70, 80, 90], [("burst", 0, 4)])


def flame(m, x, base, hgt, w=2):
    """A flame tongue: outer red, inner amber/gold, tip flicker."""
    for i in range(hgt):
        y = base - i
        half = max(0, round(w * (1 - i / hgt)))
        for dx in range(-half, half + 1):
            col = 4 if abs(dx) == half else (6 if i > hgt * 0.45 else 7)
            if get(m, x + dx, y) in (0, 3, 4):
                put(m, x + dx, y, col)
    put(m, x, base - hgt, 5)


def fx_fire():
    frames = []
    hs = [(8, 5, 7), (6, 8, 5), (7, 6, 8), (9, 7, 6)]
    for k in range(4):
        m = blank(16, 16)
        # glowing ember bed (ellipse)
        for y in range(11, 16):
            for x in range(16):
                if ((x - 7.5) / 7.5) ** 2 + ((y - 13) / 2.6) ** 2 <= 1:
                    m[y][x] = 3 if (x + y + k) % 3 else 4
        for x, y in ((4, 13), (10, 14), (7, 12), (12, 12)):
            put(m, x + (k % 2), y, 6)
        h1, h2, h3 = hs[k]
        flame(m, 4, 13, h1, 2); flame(m, 11, 13, h3, 2); flame(m, 7 + (k % 2), 13, h2 + 2, 2)
        # spark
        put(m, 3 + k * 3, 12 - hs[k][k % 3] - 2, 8)
        frames.append(rim(m, 1))
    save("fx_fire", 16, 16, FIRE_PAL, frames, [90] * 4, [("burn", 0, 3)])


def fx_dust():
    frames = []
    # 0 small puff at the bottom, 1 bigger cloud, 2 dispersing
    m = blank(8, 8); disc(m, 2.5, 5.5, 1.6, 12); disc(m, 5, 6, 1.4, 12); disc(m, 2.2, 5, 0.8, 9); put(m, 5, 5, 9)
    frames.append(rim(m, 11))
    m = blank(8, 8); disc(m, 2.2, 4.5, 2.1, 12); disc(m, 5.3, 4.2, 1.9, 12); disc(m, 3.8, 6, 1.6, 12)
    disc(m, 1.8, 3.8, 1, 9); disc(m, 5, 3.5, 0.9, 9); put(m, 2, 3, 9)
    frames.append(rim(m, 11))
    m = blank(8, 8)
    for x, y, c in ((1, 2, 12), (2, 2, 11), (5, 1, 12), (6, 2, 11), (0, 5, 11), (3, 4, 12), (4, 4, 11), (7, 5, 12),
                    (2, 7, 11), (5, 6, 11), (6, 6, 12)):
        put(m, x, y, c)
    frames.append(m)
    save("fx_dust", 8, 8, FIRE_PAL, frames, [70, 80, 90], [("puff", 0, 2)])


# ---------------------------------------------------------------- fx_chain (FX palette, cyan neon)

FX_PAL_HEX = ["#190922", "#c8dfeb", "#c8dfeb", "#ffa08c", "#c81c28", "#e63c3c", "#c0fcff", "#40e8f8", "#1a7f9c",
              "#346675", "#c8dfeb", "#ff3838", "#c81c28", "#3b2171", "#07060c"]
FX_PAL = pal(FX_PAL_HEX)   # = fx_spark / fx_impact palette (shared slot)


def fx_chain():
    paths = [[(0, 4), (2, 2), (3, 5), (5, 3), (7, 4)],
             [(0, 3), (1, 5), (4, 2), (6, 5), (7, 3)],
             [(0, 4), (2, 5), (3, 2), (5, 4), (7, 3)]]
    frames = []
    for k, pts in enumerate(paths):
        m = blank(8, 8)
        for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
            line(m, x0, y0, x1, y1, 7)
        glow = [r[:] for r in m]
        for y in range(8):
            for x in range(8):
                if m[y][x] == 0 and any(get(m, x + dx, y + dy) == 7 for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    glow[y][x] = 8 if (x + y + k) % 2 else 0
        for y in range(8):
            for x in range(8):
                if m[y][x] == 7 and (x + k) % 3 == 0:
                    glow[y][x] = 2        # white-hot core pixels
        frames.append(glow)
    save("fx_chain", 8, 8, FX_PAL, frames, [40, 40, 40], [("zap", 0, 2)])


# ---------------------------------------------------------------- hud_bossbar (UI palette = hud_xp)

def hud_bossbar():
    w, h, p, frames, durs, tags = read_aseprite("hud_xp")
    out = []
    for k in range(17):
        f = [r[:] for r in frames[0]]
        n = 2 * k
        for x in range(n):
            f[2][x] = 14; f[3][x] = 14; f[4][x] = 14; f[5][x] = 11
        if 0 < n < 32:
            for y in (2, 3, 4, 5):
                f[y][n - 1] = 15
        out.append(f)
    notch = blank(32, 8)
    for y in range(1, 7):
        notch[y][15] = 8; notch[y][16] = 1
    notch[0][15] = 1; notch[7][15] = 1
    out.append(notch)
    save("hud_bossbar", 32, 8, p, out, [100] * 18, [("fill", 0, 16), ("notch", 17, 17)])


NEW = {"big_slime": big_slime, "wolf": wolf, "fx_ember": fx_ember, "fx_explosion": fx_explosion, "fx_fire": fx_fire,
       "fx_chain": fx_chain, "fx_dust": fx_dust, "hud_bossbar": hud_bossbar}


def seed(only):
    for name, fn in NEW.items():
        if only and name not in only:
            continue
        if os.path.exists(os.path.join(ART, name + ".aseprite")):
            print("exists, skipped:", name)
            continue
        fn()


# ---------------------------------------------------------------- extensions of existing files

def backup(name):
    d = os.path.join(ART, "old_style", "pre_g50")
    os.makedirs(d, exist_ok=True)
    dst = os.path.join(d, name + ".aseprite")
    if not os.path.exists(dst):
        shutil.copy2(os.path.join(ART, name + ".aseprite"), dst)


def extend_warchief():
    w, h, p, frames, durs, tags = read_aseprite("orc_warchief")
    if any(t[0] == "stomp" for t in tags):
        print("orc_warchief already extended"); return
    backup("orc_warchief")
    p = list(p) + [(0, 0, 0)] * (16 - len(p))
    p[14] = hexc("#2a343d")   # dust shadow
    p[15] = hexc("#bc161b")   # dark red (horn glow)
    # attack 6-14: feet two rows above the walk's; move down 2 so feet sit on the last row like walk,
    # and close the cut blade at the bottom with the outline colour (8) instead of a flat interior edge.
    for k in range(6, 15):
        f = shift(frames[k], 0, 2)
        for x in range(64):
            if f[63][x] not in (0, 8, 7):
                f[63][x] = 8
        frames[k] = f
    walk0, walk2 = frames[0], frames[2]
    # stomp 15: lifted (up 3, chest high); 16: slam (squash 2 rows + dust)
    up = shift(walk2, 0, -3)
    slam = [r[:] for r in walk0]
    slam = [slam[0]] + [slam[0]] * 2 + slam[1:40] + slam[42:]     # remove rows 40-41 (squash)
    slam = [r[:] for r in slam[:64]]
    for cx, cy, r in ((10, 61.5, 3), (15, 60.5, 2.2), (54, 61.5, 3), (49, 60.5, 2.2), (5, 62.5, 1.6), (59, 62.5, 1.6)):
        disc(slam, cx, cy, r, 11, only_empty=True)
        disc(slam, cx, cy - 0.6, r * 0.5, 13, only_empty=False) if False else None
    for y in range(56, 64):
        for x in range(64):
            if slam[y][x] == 11 and get(slam, x, y - 1) == 0:
                slam[y][x] = 12
            elif slam[y][x] == 11 and y == 63:
                slam[y][x] = 14
    frames += [up, slam]
    durs += [160, 220]
    # horn 17-18: a bone war horn raised to the mouth (right side of the hood), 18 = blast with red neon arcs
    def horn(f, blast):
        f = [r[:] for r in f]
        steps = 40
        for i in range(steps + 1):          # tapered, curved bone horn: mouthpiece at the hood, bell up-right
            t = i / steps
            x = 38 + 12 * t; y = 31 - 9 * t - 3 * math.sin(math.pi * t); r = 0.5 + 2.3 * t
            disc(f, x, y, r, 12)
            disc(f, x - 0.4, y - 0.5, r * 0.45, 13)
        for t in (0.35, 0.65):
            x = 38 + 12 * t; y = 31 - 9 * t - 3 * math.sin(math.pi * t); r = 0.5 + 2.3 * t
            for d in range(-3, 4):
                if abs(d) <= r:
                    put(f, round(x + d * 0.6), round(y + d * 0.8), 7)
        disc(f, 50.6, 20.6, 1.6, 2)         # bell opening
        disc(f, 38, 33, 1.5, 4)             # gauntlet
        f = rim_only(f, 8)
        if blast:
            for r, col in ((5, 10), (8, 15), (11, 10)):
                for a in range(-35, 40, 9):
                    ang = math.radians(a - 40)
                    put(f, round(51 + r * math.cos(ang)), round(20 + r * math.sin(ang)), col)
        return f
    frames += [horn(walk0, False), horn(shift(walk0, 0, -1), True)]
    durs += [200, 120]
    tags += [("stomp", 15, 16), ("horn", 17, 18)]
    save("orc_warchief", 64, 64, p, frames, durs, tags)


def rim_only(f, c):
    """Outline newly drawn parts: any empty pixel touching a non-outline pixel gets colour c."""
    out = [r[:] for r in f]
    for y in range(len(f)):
        for x in range(len(f[0])):
            if f[y][x] == 0 and any(get(f, x + dx, y + dy) not in (0, c) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                out[y][x] = c
    return out


def extend_icons():
    w, h, p, frames, durs, tags = read_aseprite("icons")
    if any(t[0] == "ember_toss" for t in tags):
        print("icons already extended"); return
    backup("icons")
    # recogibles palette: 1 #07060c 2 #1a7f9c 3 #40e8f8 4 #a06818 7 #c8dfeb 9 #e63c3c 11 #4b1530 12 #ffc838 13 #fff4b0
    # ember toss: fireball flying up-right with a red/amber tail
    m = blank(16, 16)
    for i in range(7):
        disc(m, 3 + i * 1.0, 12 - i * 1.0, 1.2 + i * 0.25, 9 if i < 4 else 4)
    disc(m, 10, 6, 3.6, 4); disc(m, 10, 6, 2.8, 12); disc(m, 10.4, 5.6, 1.6, 13)
    for x, y in ((2, 14), (4, 13), (6, 11)):
        put(m, x, y, 11)
    put(m, 14, 3, 12); put(m, 13, 1, 9)
    ember = rim(m, 1)
    # storm chain: zigzag lightning with white core, cyan glow and two small links
    m = blank(16, 16)
    pts = [(10, 0), (5, 7), (9, 8), (4, 15)]
    for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
        line(m, x0, y0, x1, y1, 3, 3)
    for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
        line(m, x0 + 1, y0, x1 + 1, y1, 7, 1)
    for x, y in ((13, 4), (2, 10)):
        disc(m, x, y, 1.3, 2); put(m, x, y, 3)
    chain = rim(m, 1)
    frames += [ember, chain]
    durs += [100, 100]
    tags += [("ember_toss", 8, 8), ("storm_chain", 9, 9)]
    save("icons", 16, 16, p, frames, durs, tags)


if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        seed(sys.argv[2:])
    elif cmd == "extend":
        extend_warchief(); extend_icons()
