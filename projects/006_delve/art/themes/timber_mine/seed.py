"""DELVE theme 'timber_mine' (G-36): an old mine shored with timber - horizontal planks between squared posts, a
cap beam, soot from lanterns, rusty nail heads, broken planks over raw rock, board floor, joisted ceiling.
Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((8, 6, 4), (150, 110, 68), 10, (60, 40, 22))),        # shoring timber
    ("stain", P.ramp((5, 5, 5), (48, 42, 36), 5)),                          # soot, gaps
    ("wood", P.ramp((10, 10, 9), (86, 80, 68), 6)),                         # door: grey weathered boards
    ("iron", P.ramp((10, 9, 8), (98, 92, 84), 5)),
    ("rust", P.ramp((44, 20, 8), (136, 70, 26), 3)),
    ("flame", SA.FLAME),
    ("bone", P.ramp((24, 22, 16), (170, 160, 126), 5, (82, 78, 60))),
    ("cloth", P.ramp((30, 24, 14), (102, 86, 54), 3)),                     # sacking
    ("acc", P.ramp((9, 9, 11), (74, 72, 70), 4)),                          # raw rock behind the boards
]
POST = 9            # post width (texels), centred on the face edge so a post stands at every cell corner


def grain(r, h=64, w=64, horiz=True):
    cells = ((2, 24, .5), (1, 8, .3), (4, 4, .2)) if horiz else ((24, 2, .5), (8, 1, .3), (4, 4, .2))
    return P.fbm(r, h, w, cells)


def planks(r, pal, rot=False):
    """Values of horizontal planks (heights 6-9) with seams, butt joints, knots and nail heads."""
    V = np.zeros((64, 64))
    seam = np.zeros((64, 64), bool)
    nails = []
    G = grain(r)
    y = 0
    while y < 64:
        h = int(r.integers(6, 10)) if y + 9 < 64 else 64 - y
        base = r.uniform(0.32, 0.55)
        cut = int(r.integers(12, 52))
        for yy in range(y, y + h):
            k = yy - y
            V[yy] = base + 0.35 * (G[yy] - 0.5) + (0.10 if k == 1 else -0.12 if k == h - 1 else 0)
        seam[y] = True
        seam[y:y + h, cut] = True
        V[y:y + h, cut + 1] -= 0.08
        nails += [(y + 2, cut - 2), (y + 2, cut + 3), (y + h // 2, POST // 2 + 2), (y + h // 2, 64 - POST // 2 - 3)]
        if r.random() < 0.6:                                     # knot
            ky, kx = y + h // 2, int(r.integers(8, 56))
            V[ky, kx - 1:kx + 2] -= 0.2
            V[ky - 1, kx] += 0.08
        y += h
    if r.random() < 0.5 and rot:
        V = V[:, ::-1]
    return V, seam, nails


def post(r, V, x0=-(POST // 2)):
    G = grain(r, horiz=False)
    for k in range(POST):
        x = (x0 + k) % 64
        s = k / (POST - 1)
        V[:, x] = 0.62 - 0.45 * s + 0.3 * (G[:, x] - 0.5) + (0.12 if k == 1 else 0) - (0.25 if k == POST - 1 else 0)
    for y in range(0, 64, 21):                                  # adze marks
        V[(y + int(r.integers(0, 8))) % 64, (x0 + int(r.integers(1, POST - 1))) % 64] -= 0.25


def wall(seed, pal, feature=None):
    r = np.random.default_rng(seed)
    V, seam, nails = planks(r, pal)
    V = V * 0.85
    post(r, V)
    cap = np.zeros((64, 64), bool)
    cap[0:6] = True                                             # cap beam
    G = grain(r)
    V[0:6] = 0.5 + 0.3 * (G[0:6] - 0.5) + np.array([0.12, 0.08, 0.02, -0.04, -0.1, -0.2])[:, None]
    V[6] = 0.04
    seam[:, (POST // 2) % 64] = False
    if feature:
        feature(r, pal, V, seam)
    V = K.torch_fall(V, 0.5)
    S = P.fbm(r, 64, 64)
    soot = np.clip((0.5 - K.YY / 64.0) * 1.6 + (S - 0.5), 0, 1) > 0.55
    out = K.idx(pal, "stone", V, 0.8)
    out[soot & ~seam] = K.idx(pal, "stain", V * 1.4, 0.8)[soot & ~seam]
    post_cols = ((K.XX + POST // 2) % 64) < POST
    out[seam & ~post_cols & ~cap] = pal.ramps["stain"][0]
    for y, x in nails:
        y, x = y % 64, x % 64
        if 7 < y < 63:
            out[y, x] = pal.ramps["rust"][2]
            out[y + 1, x] = pal.ramps["rust"][0]
            if r.random() < 0.6:
                for k in range(2, int(r.integers(3, 6))):       # rust run
                    out[min(63, y + k), x] = pal.ramps["rust"][1 if k < 3 else 0]
    for x in range(-(POST // 2), POST - POST // 2, 3):          # spikes holding the cap
        out[2, x % 64] = pal.ramps["iron"][3]
        out[3, x % 64] = pal.ramps["iron"][0]
    if feature and hasattr(feature, "after"):
        feature.after(r, pal, out)
    return out


def brace(r, pal, V, seam):
    """Diagonal brace from the bottom of the left post to the cap at the right."""
    G = grain(r)
    for y in range(7, 64):
        xc = POST - 1 + (63 - y) * 0.78
        for d in range(-3, 4):
            x = int(xc + d)
            if 0 <= x < 64 - POST // 2:
                V[y, x] = 0.55 - 0.08 * d + 0.25 * (G[y, x] - 0.5) - (0.3 if d == 3 else 0)
                seam[y, x] = d == 3
            if 0 <= x + 4 < 64 - POST // 2:
                V[y, int(xc + 4)] = min(V[y, int(xc + 4)], 0.08)          # cast shadow


def broken(r, pal, V, seam):
    hole = np.zeros((64, 64), bool)
    edges = {}
    y = 14
    while y < 54:                                             # whole planks snapped: ragged per plank row
        h = int(r.integers(6, 10))
        w = 17 - abs(y + h / 2 - 34) * 0.45
        a, b = int(30 - w + r.uniform(-5, 4)), int(34 + w + r.uniform(-4, 5))
        for yy in range(y, min(54, y + h)):
            hole[yy, max(POST, a + int(r.integers(-1, 2))):min(64 - POST // 2 - 1, b + int(r.integers(-1, 2)))] = True
        y += h
    broken.hole = hole


def broken_after(r, pal, out):
    hole = broken.hole
    Vr = P.stone_shade(r, *P.courses(r, (10, 12, 14, 12, 16), 8, 18, joint2=0.5, jitter=2), base=(0.25, 0.5))
    out[hole] = K.idx(pal, "acc", Vr, 0.62)[hole]
    ring = hole & ~np.roll(hole, -1, 0)
    out[ring] = pal.ramps["stone"][1]
    for y, x in zip(*np.nonzero(K.edge(hole, 0, 1) | K.edge(hole, 0, -1))):
        if r.random() < 0.7:                                    # splintered ends
            out[y, x] = pal.ramps["stone"][int(r.integers(4, 8))]
    top = K.edge(hole, 1, 0)
    out[np.roll(top, 1, 0) & hole] = pal.ramps["stain"][0]


broken.after = broken_after


def xbrace(r, pal, V, seam):
    brace(r, pal, V, seam)
    G = grain(r)
    for y in range(7, 64):
        xc = 64 - POST + (POST - 1 - (63 - y) * 0.78) - POST + 1
        xc = (64 - POST // 2 - 1) - (63 - y) * 0.78
        for d in range(-3, 4):
            x = int(xc + d)
            if POST - POST // 2 <= x < 64 - POST // 2 and not (abs(d) < 2 and r.random() < 0):
                if V[y, x] > 0.45 and d < 0:
                    continue
                V[y, x] = 0.48 - 0.07 * d + 0.25 * (G[y, x] - 0.5) - (0.3 if d == 3 else 0)


def paint(rng, pal):
    t = {"wall": wall(41, pal), "v1": wall(42, pal, brace), "v2": wall(43, pal, broken), "v3": wall(44, pal, xbrace)}
    # floor: boards running into the distance, nail pairs at the cross joints, dirt
    r = np.random.default_rng(51)
    G = grain(r, horiz=False)
    V = np.zeros((64, 64))
    seam = np.zeros((64, 64), bool)
    x = 0
    while x < 64:
        w = int(r.integers(8, 12)) if x + 12 < 64 else 64 - x
        b = r.uniform(0.3, 0.5)
        V[:, x:x + w] = b + 0.3 * (G[:, x:x + w] - 0.5)
        seam[:, x] = True
        cut = int(r.integers(0, 64))
        seam[cut, x:x + w] = True
        V[(cut + 1) % 64, x:x + w] += 0.08
        x += w
    out = K.idx(pal, "stone", V, 0.85)
    dirt = P.fbm(r, 64, 64) > 0.58
    out[dirt] = K.idx(pal, "stain", V * 1.5, 0.8)[dirt]
    out[seam] = pal.ramps["stain"][0]
    for _ in range(14):
        y, x = int(r.integers(0, 64)), int(r.integers(0, 64))
        out[y, x] = pal.ramps["rust"][1]
    t["floor"] = out
    # ceiling: heavy joists across with boards between
    r = np.random.default_rng(61)
    G = grain(r)
    V = 0.22 + 0.2 * (grain(r, horiz=False) - 0.5)
    for y0 in (0, 32):
        for k in range(12):
            V[y0 + k] = 0.5 - 0.03 * k + 0.25 * (G[y0 + k] - 0.5) + (0.1 if k == 1 else 0)
        V[y0 + 12] = 0.03
        V[y0 + 13] = 0.08
    V[:, ::9] *= 0.4
    t["ceil"] = K.idx(pal, "stone", V, 0.9)
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(28, 22, 12), (52, 42, 24), (80, 66, 40), (104, 88, 56)]))
