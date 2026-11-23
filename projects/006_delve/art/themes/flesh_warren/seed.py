"""DELVE theme 'flesh_warren' (G-36, optional 7th): a warren dug into something alive - dark bulging lobes of
flesh with deep folds, purple veins, wet glints, a rib cage pushing through, a tooth-lined slit, blistered
pustules, ichor puddles on a fleshy floor, a veined membrane overhead. Kept dark and desaturated so monsters read.
Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((8, 3, 4), (146, 94, 86), 10, (62, 26, 28))),         # flesh
    ("stain", P.ramp((18, 6, 16), (104, 48, 78), 5)),                       # veins
    ("wood", P.ramp((12, 8, 6), (82, 62, 44), 6)),
    ("iron", P.ramp((10, 8, 8), (100, 90, 86), 5)),
    ("rust", P.ramp((36, 28, 10), (116, 98, 40), 3)),                       # ichor
    ("flame", SA.FLAME),
    ("bone", P.ramp((30, 24, 20), (204, 188, 158), 5, (104, 92, 76))),
    ("cloth", P.ramp((26, 10, 10), (96, 40, 34), 3)),
    ("acc", P.ramp((70, 34, 38), (196, 150, 140), 4)),                     # wet sheen
]


def lobes(r, n=10, flat=1.0):
    """Wrapping voronoi lobes shaded as bulging domes lit from the upper left; returns V, fold mask."""
    pts = r.uniform(0, 64, (n, 2))
    d = np.full((64, 64), 1e9)
    d2 = np.full((64, 64), 1e9)
    ox = np.zeros((64, 64))
    oy = np.zeros((64, 64))
    for px, py in pts:
        dx = (K.XX - px + 32) % 64 - 32
        dy = (K.YY - py + 32) % 64 - 32
        dd = np.hypot(dx, dy * 1.2)
        upd = dd < d
        d2 = np.where(upd, d, np.minimum(d2, dd))
        ox, oy = np.where(upd, dx, ox), np.where(upd, dy, oy)
        d = np.minimum(d, dd)
    edge = d2 - d
    R = np.clip(edge / 12.0, 0, 1)                                   # 0 at the fold, 1 deep inside
    nx, ny = -ox / (d + 2), -oy / (d + 2)
    V = 0.06 + 0.5 * R ** 0.5 * flat + 0.3 * (nx * -0.6 + ny * -0.7) * R ** 0.5
    V += 0.12 * (P.fbm(r, 64, 64) - 0.5)
    return V, edge < 1.6, R, nx, ny


def veins(r, pal, out, n=5):
    for _ in range(n):
        x, y = r.uniform(0, 64), r.uniform(-4, 10)
        a = np.pi / 2 + r.uniform(-0.6, 0.6)
        for k in range(int(r.integers(30, 70))):
            out[int(y) % 64, int(x) % 64] = pal.ramps["stain"][3 if k % 4 else 4]
            out[int(y) % 64, int(x + 1) % 64] = pal.ramps["stain"][1]
            a += r.uniform(-0.35, 0.35)
            x += np.cos(a)
            y += np.sin(a) * 0.9
            if r.random() < 0.04:
                bx, by, b = x, y, a + r.choice([-1, 1]) * 0.9
                for _j in range(int(r.integers(5, 14))):
                    out[int(by) % 64, int(bx) % 64] = pal.ramps["stain"][2]
                    bx += np.cos(b)
                    by += np.sin(b)


def wall(seed, pal, feature=None, n=6):
    r = np.random.default_rng(seed)
    V, fold, R, nx, ny = lobes(r, n)
    V = K.torch_fall(V, 0.45)
    out = K.idx(pal, "stone", V, 0.8)
    out[fold] = pal.ramps["stone"][0]
    veins(r, pal, out)
    wet = (R > 0.5) & (nx * -0.6 + ny * -0.7 > 0.55) & (r.random((64, 64)) < 0.5)
    out[wet] = pal.ramps["acc"][int(r.integers(1, 3))]
    if feature:
        feature(r, pal, out, V)
    return out


def ribs(r, pal, out, V):
    b = pal.ramps["bone"]
    for k, (y0, amp) in enumerate(((11, 5), (24, 3), (38, 6), (52, 4))):
        gaps = [(int(r.integers(0, 64)), int(r.integers(5, 14))) for _ in range(2)]
        for x in range(64):
            if any((x - g0) % 64 < gl for g0, gl in gaps):
                continue                                             # flesh grown over the bone
            yc = y0 + amp * np.sin(np.pi * x / 64 + k) - 2
            t = np.cos(np.pi * x / 64 * 2) * 0.5 + 0.5
            for j, lv in ((-1, 4), (0, 3), (1, 1)):
                out[int(yc + j) % 64, x] = b[lv if x % 31 else 0]
            out[int(yc + 2) % 64, x] = pal.ramps["stone"][0]


def slit(r, pal, out, V):
    b = pal.ramps["bone"]
    for y in range(6, 60):
        w = 4.5 * np.sin(np.pi * (y - 6) / 54) ** 0.7
        for x in range(int(32 - w - 3), int(32 + w + 4)):
            d = abs(x + 0.5 - 32)
            if d <= w:
                out[y, x] = pal.ramps["stone"][0]
            elif d <= w + 2:
                out[y, x] = pal.ramps["stone"][6 if x < 32 else 2]   # swollen lips
        if y % 4 == 0 and w > 1.5:                                   # teeth along both edges
            for s in (-1, 1):
                tx = int(32 + s * w - (s > 0))
                out[y, tx] = b[3]
                out[y + 1, tx - s] = b[2]
                out[y + 1, tx] = b[1]


def pustules(r, pal, out, V):
    for _ in range(9):
        cx, cy, rr = r.uniform(12, 52), r.uniform(26, 56), r.uniform(2.2, 4.5)
        m = K.ellipse(cx, cy, rr, rr * 0.9)
        sh = (cx - K.XX) * 0.6 + (cy - K.YY) * 0.7
        out[m] = K.idx(pal, "acc", 0.35 + 0.4 * sh / rr, 0.9)[m]
        out[int(cy - rr * 0.4), int(cx - rr * 0.3)] = pal.ramps["acc"][3]
        shadow = np.roll(np.roll(m, 1, 0), 1, 1) & ~m
        out[shadow] = pal.ramps["stone"][0]
        if r.random() < 0.5:
            for k in range(int(r.integers(3, 8))):                   # drip
                out[min(63, int(cy + rr) + k), int(cx)] = pal.ramps["rust"][1 if k < 3 else 0]


def paint(rng, pal):
    t = {"wall": wall(191, pal), "v1": wall(192, pal, ribs), "v2": wall(193, pal, slit),
         "v3": wall(194, pal, pustules, 12)}
    r = np.random.default_rng(201)
    V, fold, R, nx, ny = lobes(r, 14, 0.6)
    out = K.idx(pal, "stone", V * 0.85, 0.8)
    out[fold] = pal.ramps["stone"][0]
    pud = P.fbm(r, 64, 64) > 0.64
    out[pud] = pal.ramps["stone"][1]                                # dark blood pooled in the hollows
    gl = pud & ~np.roll(pud, 1, 0) & (K.XX % 3 != 0)
    out[gl] = pal.ramps["acc"][1]
    t["floor"] = out
    r = np.random.default_rng(211)
    V, fold, R, nx, ny = lobes(r, 8, 0.5)
    out = K.idx(pal, "stone", V * 0.55, 0.8)
    out[fold] = pal.ramps["stone"][0]
    veins(r, pal, out, 7)
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(26, 10, 10), (48, 16, 16), (72, 26, 24), (96, 40, 34)],
                                          trim=[(70, 60, 50), (120, 106, 90), (180, 166, 140)]))
