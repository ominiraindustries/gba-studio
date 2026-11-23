"""DELVE theme 'root_temple' (G-36): a sunken temple of ash-grey limestone taken by roots - big ashlar blocks, a
carved meander frieze, a worn mask relief, roots splitting the masonry, a dark tide line, a flooded floor where only
the tops of the flags break the black water, roots hanging from the vault. Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((6, 7, 7), (148, 144, 128), 10, (58, 58, 52))),       # ash limestone
    ("stain", P.ramp((12, 8, 6), (98, 72, 48), 5)),                         # roots
    ("wood", P.ramp((10, 8, 6), (80, 62, 44), 6)),
    ("iron", P.ramp((12, 12, 8), (112, 104, 70), 5)),                       # tarnished bronze
    ("rust", P.ramp((20, 34, 28), (70, 108, 88), 3)),                       # verdigris
    ("flame", SA.FLAME),
    ("bone", P.ramp((24, 24, 18), (172, 166, 136), 5, (84, 82, 66))),
    ("cloth", P.ramp((16, 22, 16), (66, 80, 58), 3)),
    ("acc", P.ramp((3, 8, 10), (62, 94, 96), 4)),                          # black-teal water
]
ASHLAR = dict(heights=(21, 22, 21), w=(22, 34), base=(0.32, 0.5), joint2=0.15)


def roots(r, pal, out, n=3, y0=0, thick=3.0, length=(30, 64)):
    rt = pal.ramps["stain"]
    for _ in range(n):
        x, y = r.uniform(0, 64), float(y0)
        w = r.uniform(thick * 0.7, thick)
        L = int(r.uniform(*length))
        dx = r.uniform(-0.4, 0.4)
        for k in range(L):
            for j in range(-int(w) - 1, int(w) + 2):
                xx = int(x + j) % 64
                d = (j + 0.5) / (w + 0.5)
                if abs(d) <= 1:
                    out[int(y) % 64, xx] = rt[int(np.clip(2.6 - 2.4 * d + r.uniform(-0.4, 0.4), 0, 4))]
                elif abs(d) <= 1.4 and j > 0:
                    out[int(y) % 64, xx] = pal.ramps["stone"][0]       # shadow on the stone, right side
            x += dx + r.uniform(-0.6, 0.6)
            y += 1
            w = max(0.6, w - r.uniform(0, 0.08))
            if r.random() < 0.05 and w > 1.2:                         # side rootlet
                roots_side(r, pal, out, x, y, w * 0.5)


def roots_side(r, pal, out, x, y, w):
    s = r.choice([-1, 1])
    for k in range(int(r.integers(5, 14))):
        out[int(y) % 64, int(x) % 64] = pal.ramps["stain"][2 if k % 2 else 1]
        x += s * r.uniform(0.6, 1.2)
        y += r.uniform(0.3, 1.0)


def carve(pal, out, mask, V):
    """Engraved line: dark groove, its lower/right lip catches the light."""
    out[mask] = pal.ramps["stone"][1]
    lip = np.roll(mask, 1, 0) & ~mask
    out[lip] = K.idx(pal, "stone", V + 0.2, 0.85)[lip]


def meander(y0, h):
    m = np.zeros((64, 64), bool)
    for x0 in range(0, 64, 16):                                       # square spiral key, 16 px period
        for (a, b, c, d) in ((0, 0, 0, 12), (0, 12, 12, 12), (12, 4, 12, 12), (4, 4, 12, 4), (4, 4, 4, 8),
                             (4, 8, 8, 8)):
            ys, ye = sorted((y0 + a * h // 12, y0 + c * h // 12))
            xs, xe = sorted((x0 + b, x0 + d))
            m[ys:ye + 1, xs:xe + 1] = True
        m[y0 + h, x0 + 12:x0 + 16] = True
    return m


def base(r, pal, tide=None):
    V, M, _ = K.blocks(r, pal, ASHLAR, cracks=5, chips=6, pits=20)
    out = K.idx(pal, "stone", V, 0.85)
    if tide is not None:                                              # dark wet band below the old waterline
        wet = (K.YY > tide + 2 * np.sin(K.XX * 0.4)) & ~M
        out[wet] = K.idx(pal, "stone", V * 0.6, 0.85)[wet]
        line = (np.abs(K.YY - (tide + 2 * np.sin(K.XX * 0.4))) < 0.8)
        out[line] = pal.ramps["acc"][2]
    return out, V, M


def mask_relief(pal, out, V):
    """A worn mask carved in a big block: brow, hollow eyes, nose ridge, a grim mouth (relief, not text)."""
    face = K.ellipse(32, 30, 15, 18)
    V2 = V + 0.18 * face * (0.6 - 0.5 * ((K.XX - 32) / 15.0) - 0.4 * ((K.YY - 30) / 18.0))
    out[face] = K.idx(pal, "stone", V2, 0.85)[face]
    rim = face & ~(np.roll(face, -1, 0) & np.roll(face, -1, 1))
    out[rim] = pal.ramps["stone"][1]
    for ex in (25, 39):
        eye = K.ellipse(ex, 27.5, 3.6, 1.8) | K.ellipse(ex + (1.5 if ex < 32 else -1.5), 26.5, 2.0, 1.4)
        out[eye] = pal.ramps["stone"][0]
        out[K.edge(eye, 1, 0) & ~np.roll(eye, -1, 0)] = pal.ramps["stone"][1]
        out[np.roll(eye, 1, 0) & ~eye] = pal.ramps["stone"][7]
    brow = (np.abs(K.YY - 22 + 0.04 * (K.XX - 32) ** 2) < 1.0) & face
    out[brow] = pal.ramps["stone"][7]
    nose = (np.abs(K.XX - 32) < 1.5) & (K.YY > 27) & (K.YY < 37) & face
    out[nose & (K.XX < 32)] = pal.ramps["stone"][7]
    out[nose & (K.XX >= 32)] = pal.ramps["stone"][2]
    mouth = (np.abs(K.YY - 41 - 0.05 * (K.XX - 32) ** 2) < 0.9) & (np.abs(K.XX - 32) < 8)
    carve(pal, out, mouth, V)


def paint(rng, pal):
    r = [np.random.default_rng(s) for s in (161, 162, 163, 164)]
    wall, V, M = base(r[0], pal)
    carve(pal, wall, meander(24, 12) & ~M, V)
    roots(r[0], pal, wall, 2, 0, 2.5, (20, 50))
    v1, V1, M1 = base(r[1], pal)
    mask_relief(pal, v1, V1)
    roots(r[1], pal, v1, 1, 0, 2.0, (14, 30))
    v2, V2, M2 = base(r[2], pal)
    roots(r[2], pal, v2, 5, 0, 3.5, (40, 64))
    v3, V3, M3 = base(r[3], pal, tide=40)
    carve(pal, v3, meander(24, 12) & ~M3, V3)
    roots(r[3], pal, v3, 1, 0, 2.0, (10, 26))
    t = {"wall": wall, "v1": v1, "v2": v2, "v3": v3}
    rf = np.random.default_rng(171)
    V, M, _ = K.blocks(rf, pal, dict(heights=(16, 16, 16, 16), w=(14, 24), base=(0.3, 0.45), joint2=0.2), light=0,
                       bevel=0.8)
    W = P.fbm(rf, 64, 64)
    water = (W > 0.56) | (M & (W > 0.4))
    out = K.idx(pal, "stone", V, 0.8)
    out[water] = K.idx(pal, "acc", 0.25 + 0.5 * (W - 0.42) * (~M), 0.8)[water]
    shore = K.edge(~water, -1, 0) | K.edge(~water, 1, 0)
    out[shore & ~water] = pal.ramps["stone"][2]
    glint = water & (np.roll(W, 1, 1) - W > 0.015) & (K.YY % 3 == 0) & (rf.random((64, 64)) < 0.35)
    out[glint] = pal.ramps["acc"][3]
    t["floor"] = out
    rc = np.random.default_rng(181)
    V, M, _ = K.blocks(rc, pal, dict(heights=(21, 22, 21), w=(18, 30), base=(0.13, 0.24)), light=0, bevel=0.7)
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(6):
        x, y = rc.uniform(0, 64), rc.uniform(0, 64)
        a = rc.uniform(0, 2 * np.pi)
        for k in range(int(rc.integers(10, 26))):
            out[int(y) % 64, int(x) % 64] = pal.ramps["stain"][2 if k % 3 else 1]
            out[int(y + 1) % 64, int(x) % 64] = pal.ramps["stone"][0]
            a += rc.uniform(-0.4, 0.4)
            x += np.cos(a)
            y += np.sin(a)
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(14, 22, 16), (26, 38, 28), (42, 58, 42), (60, 78, 58)],
                                          trim=[(60, 56, 36), (104, 96, 62), (150, 140, 92)]))
