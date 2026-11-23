"""DELVE theme 'bone_crypt' (G-36): dusty sandstone catacomb - big ashlar blocks under dust, burial niches with a
skull and long bones, an ossuary wall of stacked skulls and femurs, a shrouded body in a long niche, flagstones with
bone splinters, cobwebbed vault. Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((9, 7, 5), (170, 146, 106), 10, (74, 58, 40))),       # sandstone
    ("stain", P.ramp((50, 46, 40), (166, 160, 146), 5)),                    # dust, cobweb
    ("wood", P.ramp((12, 9, 6), (84, 64, 42), 6)),
    ("iron", P.ramp((12, 10, 8), (100, 90, 74), 5)),
    ("rust", P.ramp((36, 26, 12), (104, 78, 34), 3)),
    ("flame", SA.FLAME),
    ("bone", P.ramp((30, 26, 18), (216, 202, 166), 5, (112, 100, 76))),
    ("cloth", P.ramp((30, 26, 20), (120, 108, 86), 3)),                     # shroud
    ("acc", P.ramp((40, 16, 10), (116, 52, 30), 3)),                       # old red ochre
]
ASHLAR = dict(heights=(16, 16, 16, 16), w=(20, 30), base=(0.32, 0.52), joint2=0.15)


def base_wall(r, pal, dust=0.55):
    V, M, _ = K.blocks(r, pal, ASHLAR, cracks=4, chips=5, pits=22)
    out = K.idx(pal, "stone", V, 0.85)
    D = P.fbm(r, 64, 64, ((4, 16, .6), (2, 4, .4))) + 0.5 * (K.YY / 64.0)   # dust settles low and on the lips
    lip = ~M & np.roll(M, 1, 0) & (r.random((64, 64)) < 0.8)
    dust_m = ((D > dust + 0.45) & ((K.XX + K.YY) % 2 == 0) | lip) & ~M
    out[dust_m] = K.idx(pal, "stain", V * 0.8 + 0.1, 0.8)[dust_m]
    return out, V, M


def skull(pal, out, cx, cy, s=1.0, tilt=0, lit=1.0):
    b = pal.ramps["bone"]
    m = K.ellipse(cx, cy, 4.2 * s, 3.8 * s) | K.ellipse(cx + tilt, cy + 3.2 * s, 2.8 * s, 2.0 * s)
    sh = (K.XX + 0.5 - cx) * -0.5 + (K.YY + 0.5 - cy) * -0.6
    V = (0.45 + 0.35 * sh / (4 * s)) * lit
    out[m] = K.idx(pal, "bone", V, 0.8, 0.5)[m]
    for ex in (-1.6, 1.6):
        e = K.ellipse(cx + ex * s + tilt * 0.3, cy + 0.6 * s, 1.2 * s, 1.1 * s)
        out[e] = pal.ramps["stone"][0]
    out[int(cy + 2.2 * s) % 64, int(cx + tilt * 0.5) % 64] = pal.ramps["stone"][0]
    for k in (-1, 1):
        out[int(cy + 3.8 * s) % 64, int(cx + tilt + k) % 64] = b[1]
    return m


def femur(pal, out, x0, x1, y, lit=1.0):
    b = pal.ramps["bone"]
    for x in range(x0, x1 + 1):
        knob = x - x0 < 2 or x1 - x < 2
        out[y, x] = b[min(4, int(3 * lit) + 1)]
        out[y + 1, x] = b[max(0, int(2 * lit))]
        if knob:
            out[y - 1, x] = b[min(4, int(3 * lit))]
            out[y + 2, x] = b[0]
    out[y + 2, x0 + 2:x1 - 1] = pal.ramps["stone"][0]


def niche(r, pal, out, x0, y0, x1, y1, content="bones"):
    st = pal.ramps["stone"]
    for y in range(y0, y1 + 1):                                       # deep, darker toward the back top
        for x in range(x0, x1 + 1):
            out[y, x] = st[0] if y < y0 + (y1 - y0) * 0.45 or r.random() < 0.3 else st[1]
    out[y0 - 1, x0 - 1:x1 + 2] = st[1]                                # lintel underside in shadow
    out[y1 + 1, x0 - 1:x1 + 2] = st[6]                                # sill lit by the torch
    out[y1 + 2, x0 - 1:x1 + 2] = st[3]
    out[y0:y1 + 1, x1 + 1] = st[5]
    if content == "bones":
        skull(pal, out, x0 + 6, y1 - 3, 0.9, 0, 0.9)
        femur(pal, out, x0 + 11, x1 - 2, y1 - 2, 0.8)
        femur(pal, out, x0 + 13, x1 - 4, y1 - 4, 0.55)
    elif content == "shroud":
        body = K.poly([(x0 + 3, y1 + 1), (x0 + 5, y1 - 6), (x0 + 14, y1 - 7), (x1 - 10, y1 - 6),
                       (x1 - 3, y1 - 4), (x1 - 2, y1 + 1)])
        V = 0.5 - 0.3 * ((K.YY - (y1 - 6)) / 7.0) + 0.15 * np.sin(K.XX * 0.9) * (K.YY < y1 - 3)
        out[body] = K.idx(pal, "cloth", V, 0.7)[body]
        skull(pal, out, x0 + 6, y1 - 4, 0.85, 1, 0.8)


def ossuary(r, pal):
    out = np.full((64, 64), pal.ramps["stone"][0])
    for row, y in enumerate(range(5, 64, 16)):
        lit = 0.55 + 0.45 * np.sin(np.pi * (y + 6) / 76.0)
        for x in range(0 if row % 2 else 5, 64, 10):
            skull(pal, out, x + 4.5, y, 1.0, int(r.integers(-1, 2)), lit * r.uniform(0.75, 1.0))
        for fy in (y + 7, y + 10):                                    # femurs packed between the rows
            x = -int(r.integers(0, 8))
            while x < 64:
                L = int(r.integers(10, 16))
                femur(pal, out, max(0, x), min(63, x + L), fy % 62, lit * r.uniform(0.5, 0.85))
                x += L + 2
    return out


def paint(rng, pal):
    r = [np.random.default_rng(s) for s in (101, 102, 103, 104)]
    wall, _, _ = base_wall(r[0], pal)
    ochre = np.abs((K.XX * 0.9 + K.YY) % 23 - 11) < 1.0                # faded painted band of red ochre
    band = ochre & (K.YY > 33) & (K.YY < 46) & (P.fbm(r[0], 64, 64) > 0.45)
    del band                                                          # (tried: ochre band read as noise)
    v1, _, _ = base_wall(r[1], pal)
    for x0, y0 in ((5, 9), (37, 9), (5, 40), (37, 40)):
        niche(r[1], pal, v1, x0, y0, x0 + 22, y0 + 11)
    v2 = ossuary(r[2], pal)
    v3, _, _ = base_wall(r[3], pal)
    niche(r[3], pal, v3, 4, 28, 59, 44, "shroud")
    for _ in range(3):                                                # cobweb strands in a top corner
        x, y = 0.0, float(r[3].integers(0, 6))
        for k in range(18):
            v3[int(y) % 64, int(x) % 64] = pal.ramps["stain"][3 if k % 3 else 2]
            x += 1
            y += r[3].uniform(0.2, 0.9)
    t = {"wall": wall, "v1": v1, "v2": v2, "v3": v3}
    rf = np.random.default_rng(111)
    V, M, _ = K.blocks(rf, pal, dict(heights=(21, 22, 21), w=(18, 30), base=(0.25, 0.4), joint2=0.2), light=0,
                       bevel=0.6, cracks=6, chips=3, pits=12)
    out = K.idx(pal, "stone", V, 0.8)
    D = P.fbm(rf, 64, 64) > 0.5
    out[D & ~M] = K.idx(pal, "stain", V * 0.9, 0.8)[D & ~M]
    for _ in range(7):                                                # bone splinters
        y, x = int(rf.integers(2, 60)), int(rf.integers(2, 58))
        L = int(rf.integers(3, 7))
        for k in range(L):
            out[y, x + k] = pal.ramps["bone"][3 if k % 2 else 2]
            out[y + 1, x + k] = pal.ramps["stone"][0]
    t["floor"] = out
    rc = np.random.default_rng(121)
    V, M, _ = K.blocks(rc, pal, dict(heights=(16, 16, 16, 16), w=(14, 22), base=(0.13, 0.25)), light=0, bevel=0.7)
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(5):
        x, y = rc.uniform(0, 64), rc.uniform(0, 64)
        a = rc.uniform(0, np.pi)
        for k in range(int(rc.integers(8, 20))):
            out[int(y) % 64, int(x) % 64] = pal.ramps["stain"][1]
            x += np.cos(a)
            y += np.sin(a) * 0.6 + 0.3
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(34, 12, 10), (58, 20, 16), (84, 30, 22), (108, 44, 30)],
                                          trim=[(70, 58, 34), (112, 94, 56), (156, 136, 90)]))
