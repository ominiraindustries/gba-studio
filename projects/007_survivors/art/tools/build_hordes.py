"""Phase 2-3 horde art (G-50, ahead of schedule): ghost, imp (drawn in code) and gargoyle (PixelLab, art/raw/gargoyle/).

    python -I art/tools/build_hordes.py seed [names]   # creates the missing .aseprite files (never overwrites)
Then: python -I art/tools/build_art.py export
"""
import os
import random
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_block2 import ART, blank, put, get, rim, shift, line, disc, save, pal  # noqa: E402

GHOST = pal(["#07060c", "#191935", "#34466e", "#36435b", "#8ca4b4", "#abc4d2", "#c8dfeb", "#40e8f8", "#1a7f9c",
             "#c0fcff", "#3b2171", "#2d1153", "#fd3ffd", "#e020c0"])
# 1 black 2 deep 3 rim 4 shade dark 5 shade 6 body 7 light 8 cyan eye 9 cyan dark 10 cyan white
# 11-14 violet/magenta (wraith recolour slots)


def tri(px, a, b, c, col):
    xs = [p[0] for p in (a, b, c)]; ys = [p[1] for p in (a, b, c)]
    def side(p, q, r):
        return (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0])
    for y in range(min(ys), max(ys) + 1):
        for x in range(min(xs), max(xs) + 1):
            p = (x, y)
            d1, d2, d3 = side(a, b, p), side(b, c, p), side(c, a, p)
            if (d1 >= 0 and d2 >= 0 and d3 >= 0) or (d1 <= 0 and d2 <= 0 and d3 <= 0):
                put(px, x, y, col)


def ghost_body(phase, dy):
    m = blank(16, 16)
    disc(m, 7.5, 6 + dy, 4.6, 6)
    wave = (0, 1, 2, 3, 2, 1)
    for x in range(3, 13):
        bottom = 11 + wave[(x + phase) % 6] + dy
        for y in range(6 + dy, min(16, bottom + 1)):
            put(m, x, y, 6)
    for x in (2, 13):                      # slight flare at the hem
        for y in range(10 + dy, min(16, 12 + dy)):
            put(m, x, y, 6)
    # shading: left/bottom darker, top highlight
    for y in range(16):
        for x in range(16):
            if m[y][x] == 6:
                if x <= 4 or get(m, x, y + 1) == 0:
                    m[y][x] = 5
                elif get(m, x, y - 1) == 0 and x < 10:
                    m[y][x] = 7
    # face: hollow eyes with cyan pupils, small open mouth
    ey = 6 + dy
    for ex in (5, 9):
        put(m, ex, ey, 1); put(m, ex + 1, ey, 1); put(m, ex, ey + 1, 1); put(m, ex + 1, ey + 1, 8)
    put(m, 7, ey + 3, 2); put(m, 8, ey + 3, 2); put(m, 7, ey + 4, 1)
    return m


def ghost():
    frames = []
    for k, dy in enumerate((0, -1, -1, 0)):
        frames.append(rim(ghost_body(k, dy + 1), 3))
    base = frames[0]
    for keep in (0, 1):            # fade 4-5: dithered phase-out (4 = half, 5 = sparse cyan outline)
        f = [r[:] for r in base]
        for y in range(16):
            for x in range(16):
                if f[y][x] and ((x + y) % 2 if keep == 0 else (x % 2 or y % 2)):
                    f[y][x] = 0
                elif f[y][x] and keep == 1:
                    f[y][x] = 9
        frames.append(f)
    rnd = random.Random(4)
    for k in range(3):             # death 6-8: rises and frays into cyan wisps
        f = shift(base, 0, -2 * (k + 1))
        f = [[(c if c and rnd.random() < 0.75 - 0.25 * k else 0) for c in r] for r in f]
        f = [[(10 if c in (6, 7) and k >= 1 else (8 if c == 5 and k >= 1 else c)) for c in r] for r in f]
        frames.append(f)
    save("ghost", 16, 16, GHOST, frames, [140] * 4 + [100, 100] + [90, 90, 120],
         [("float", 0, 3), ("fade", 4, 5), ("death", 6, 8)])


IMP = pal(["#07060c", "#2a012f", "#4b1530", "#bc161b", "#e63c3c", "#46155a", "#290c36", "#8ca4b4", "#c8dfeb",
           "#ffc838", "#fff4b0", "#a06818", "#34466e"])
# 1 black 2 wing dark 3 body dark 4 red 5 red light 6 wing 7 wing deep 8 horn 9 horn light 10 eye gold 11 fire
# 12 amber 13 rim


def imp_frame(wing, dy, cast=False):
    m = blank(16, 16)
    tips = {"up": ((1, 1), (14, 1)), "mid": ((0, 6), (15, 6)), "down": ((1, 11), (14, 11))}[wing]
    for (sx, tip, low) in ((6, tips[0], 4), (9, tips[1], 11)):
        tri(m, (sx, 8 + dy), tip, (low, 11 + dy), 6)
        line(m, sx, 8 + dy, tip[0], tip[1], 2)
    # body + head
    disc(m, 7.5, 10.5 + dy, 2.2, 3); disc(m, 7.5, 6 + dy, 2.6, 3)
    for x in range(6, 10):
        put(m, x, 5 + dy, 4)
    put(m, 7, 4 + dy, 5); put(m, 8, 4 + dy, 5)
    # horns
    for (x, y) in ((5, 3), (4, 2), (10, 3), (11, 2)):
        put(m, x, y + dy, 8 if y == 3 else 9)
    # eyes
    put(m, 6, 6 + dy, 10); put(m, 9, 6 + dy, 10); put(m, 7, 8 + dy, 1); put(m, 8, 8 + dy, 1)
    # tail
    line(m, 8, 12 + dy, 10, 14 + dy, 3); put(m, 11, 14 + dy, 4); put(m, 11, 15 + dy, 4)
    if cast:   # fireball held over the head
        disc(m, 7.5, 1.5 + dy, 1.6, 12); put(m, 7, 1 + dy, 11); put(m, 8, 1 + dy, 11)
        put(m, 6, 4 + dy, 4); put(m, 9, 4 + dy, 4)
    return rim(m, 13)


def imp():
    frames = [imp_frame("up", 0), imp_frame("mid", 1), imp_frame("down", 1), imp_frame("mid", 0),
              imp_frame("up", 1, cast=True)]
    base = imp_frame("mid", 1)
    rnd = random.Random(6)
    for k in range(3):   # death 5-7: tumbles down, fades to embers
        f = shift(base, 0, 2 * (k + 1))
        f = [[(c if c and rnd.random() < 0.8 - 0.25 * k else 0) for c in r] for r in f]
        if k:
            f = [[(12 if c in (4, 5) else (2 if c in (3, 6) else c)) for c in r] for r in f]
        frames.append(f)
    save("imp", 16, 16, IMP, frames, [90] * 4 + [250] + [90, 100, 120],
         [("fly", 0, 3), ("cast", 4, 4), ("death", 5, 7)])


GARG = pal(["#07060c", "#0c0d1c", "#1c242c", "#2a343d", "#36435b", "#2f3551", "#34466e", "#8ca4b4", "#abc4d2",
            "#c8dfeb", "#e63c3c", "#bc161b", "#191935", "#21263b", "#4b1530"])


def quant(im, palette, size):
    """RGBA -> palette indices (nearest weighted RGB; alpha < 128 transparent), cropped/centred into size x size
    with the content's bottom on the last row."""
    bb = im.getbbox()
    im = im.crop(bb)
    out = blank(size, size)
    ox = (size - im.width) // 2; oy = size - im.height
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = im.getpixel((x, y))
            if a < 128:
                continue
            best = min(range(1, len(palette)), key=lambda i: 2 * (palette[i][0] - r) ** 2 + 4 * (palette[i][1] - g) ** 2
                       + 3 * (palette[i][2] - b) ** 2)
            put(out, x + ox, y + oy, best)
    return out


def gargoyle():
    raw = os.path.join(ART, "raw", "gargoyle")
    perch = quant(Image.open(os.path.join(raw, "east.png")).convert("RGBA"), GARG, 32)
    fly_im = [Image.open(os.path.join(raw, f"fly_{i}.png")).convert("RGBA") for i in range(4)]
    # one common crop box for the flight so the body does not jitter
    boxes = [im.getbbox() for im in fly_im]
    box = (min(b[0] for b in boxes), min(b[1] for b in boxes), max(b[2] for b in boxes), max(b[3] for b in boxes))
    w, h = box[2] - box[0], box[3] - box[1]
    fly = []
    for im in fly_im:
        c = Image.new("RGBA", (max(w, 1), max(h, 1)))
        c.alpha_composite(im.crop(box))
        if c.width > 32 or c.height > 32:     # centre-crop (wing tips may lose a pixel)
            l = max(0, (c.width - 32) // 2); t = max(0, (c.height - 32) // 2)
            c = c.crop((l, t, l + min(32, c.width), t + min(32, c.height)))
        f = blank(32, 32)
        q = quant_full(c, GARG)
        ox = (32 - len(q[0])) // 2; oy = (32 - len(q)) // 2
        for y, row in enumerate(q):
            for x, v in enumerate(row):
                if v:
                    put(f, x + ox, y + oy, v)
        fly.append(f)
    # dive: flight frame 0 tilted 35 degrees nose-down
    src = fly_im[0].crop(box).rotate(-35, resample=Image.NEAREST, expand=True)
    dive = quant(src, GARG, 32) if src.getbbox() else fly[0]
    perch, dive = rim(perch, 7), rim(dive, 7)
    fly = [rim(f, 7) for f in fly]          # style C cold rim
    rnd = random.Random(9)
    deaths = []
    for k in range(3):            # turns to stone and crumbles
        f = shift(fly[2], 0, 3 * (k + 1))
        f = [[(c if c and rnd.random() < 0.85 - 0.25 * k else 0) for c in r] for r in f]
        f = [[({11: 4, 12: 3}.get(c, c) if k == 0 else ({10: 8, 9: 8, 11: 4, 12: 3}.get(c, c))) for c in r] for r in f]
        deaths.append(f)
    save("gargoyle", 32, 32, GARG, [perch] + fly + [dive] + deaths, [300] + [90] * 4 + [200] + [100, 110, 130],
         [("perch", 0, 0), ("fly", 1, 4), ("dive", 5, 5), ("death", 6, 8)])


def quant_full(im, palette):
    out = blank(im.width, im.height)
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = im.getpixel((x, y))
            if a >= 128:
                out[y][x] = min(range(1, len(palette)), key=lambda i: 2 * (palette[i][0] - r) ** 2
                                + 4 * (palette[i][1] - g) ** 2 + 3 * (palette[i][2] - b) ** 2)
    return out


def up2_thin(f, rimc):
    """2x nearest; the doubled outer rim is thinned back to 1 px (inner rim pixels take the inner colour)."""
    n = len(f) * 2
    big = [[f[y // 2][x // 2] for x in range(n)] for y in range(n)]
    out = [r[:] for r in big]
    for y in range(n):
        for x in range(n):
            if big[y][x] == rimc and not any(get(big, x + dx, y + dy) == 0 for dx in (-1, 0, 1) for dy in (-1, 0, 1)
                                             if 0 <= x + dx < n and 0 <= y + dy < n):
                nb = [get(big, x + dx, y + dy) for dx in (-2, -1, 0, 1, 2) for dy in (-2, -1, 0, 1, 2)]
                nb = [c for c in nb if c not in (0, rimc)]
                if nb:
                    out[y][x] = max(set(nb), key=nb.count)
    return out


def gargoyle_lord():
    """64x64 seeded by enlarging gargoyle 2x: fly 0-3, dive 4, stone 5, death 6-9."""
    from build_block2 import read_aseprite
    w, h, p, fr, durs, tags = read_aseprite("gargoyle")
    stone = {10: 8, 11: 4, 12: 3, 9: 8, 1: 3}
    frames = [up2_thin(fr[k], 7) for k in (1, 2, 3, 4, 5)]
    frames.append([[stone.get(c, c) for c in r] for r in frames[0]])
    rnd = random.Random(12)
    for k in range(4):
        f = shift(up2_thin(fr[6 + min(k, 2)], 7), 0, 2 * k)
        if k == 3:
            f = [[(c if c and rnd.random() < 0.3 else 0) for c in r] for r in f]
        frames.append(f)
    save("gargoyle_lord", 64, 64, p, frames, [90] * 4 + [200, 300] + [100, 110, 130, 150],
         [("fly", 0, 3), ("dive", 4, 4), ("stone", 5, 5), ("death", 6, 9)])


NEW = {"ghost": ghost, "imp": imp, "gargoyle": gargoyle, "gargoyle_lord": gargoyle_lord}


if __name__ == "__main__":
    if sys.argv[1] == "seed":
        for name, fn in NEW.items():
            if len(sys.argv) > 2 and name not in sys.argv[2:]:
                continue
            if os.path.exists(os.path.join(ART, name + ".aseprite")):
                print("exists, skipped:", name)
                continue
            fn()
