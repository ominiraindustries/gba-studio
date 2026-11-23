"""DELVE theme 'ice_cavern' (G-36): frozen blue-black rock slabs, rime on every lip, icicles under the joints,
sheets of deep blue ice, frost drifts and cracked ice on the floor. Run: python -I seed.py [export-only]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((5, 7, 12), (128, 150, 174), 10, (38, 52, 74))),       # frozen rock
    ("stain", P.ramp((64, 86, 110), (214, 230, 238), 5)),                   # rime / frost
    ("wood", P.ramp((9, 10, 13), (72, 74, 78), 6)),                         # frost-greyed wood
    ("iron", P.ramp((8, 10, 14), (100, 114, 128), 5)),
    ("rust", P.ramp((30, 26, 28), (90, 80, 78), 3)),
    ("flame", SA.FLAME),
    ("bone", P.ramp((22, 26, 32), (176, 182, 184), 5, (84, 92, 100))),
    ("cloth", P.ramp((10, 14, 30), (44, 58, 94), 3)),
    ("acc", P.ramp((14, 30, 52), (104, 168, 206), 4)),                     # deep blue ice
]
LAY = dict(heights=(12, 20, 14, 18), w=(14, 32), base=(0.28, 0.52), joint2=0.35, jitter=3)
FLOOR = dict(heights=(21, 22, 21), w=(20, 34), base=(0.26, 0.40), joint2=0.1)
CEIL = dict(heights=(16, 16, 16, 16), w=(14, 26), base=(0.12, 0.24), joint2=0.3, jitter=2)


def icicles(rng, pal, out, M, n=7, lmin=2, lmax=8, top=True):
    st, fr = pal.ramps["stone"], pal.ramps["stain"]
    rows = list(np.nonzero(M.all(1))[0])
    if top:
        rows = [0] + rows
    rows = sorted(set(r for r in rows if not (r - 1 in rows)))
    for r0 in rows:
        for _ in range(n):
            x = int(rng.integers(0, 64))
            L = int(rng.integers(lmin, lmax + (6 if r0 == 0 else 0)))
            for k in range(L):
                y = (r0 + 1 + k) % 64
                lvl = int(np.clip(4 - 4 * k / L + rng.random() * 0.6, 0, 4))
                out[y, x] = fr[lvl]
                if k < L // 3:
                    out[y, (x + 1) % 64] = fr[max(0, lvl - 2)]
                    out[y, (x + 2) % 64] = st[0]
                else:
                    out[y, (x + 1) % 64] = st[0]


def wall(seed, pal, ice=0.62, rime=0.55, feature=None):
    r = np.random.default_rng(seed)
    V, M, _ = K.blocks(r, pal, LAY, cracks=5, chips=4, pits=18)
    out = K.idx(pal, "stone", V, 0.85)
    N = P.fbm(r, 64, 64)
    sheet = (N > ice) & ~M
    out[sheet] = K.idx(pal, "acc", V * 1.05 + 0.5 * (N - ice), 0.75)[sheet]
    lip = ~M & np.roll(M, 1, 0)
    lip2 = ~M & np.roll(M, 2, 0) & (r.random((64, 64)) < rime)
    out[lip | lip2] = K.idx(pal, "stain", V * 0.9 + 0.15 * lip, 0.75)[lip | lip2]
    if feature:
        feature(r, pal, out, V)
    icicles(r, pal, out, M)
    return out


def crevasse(r, pal, out, V):
    """A deep vertical fissure glowing faintly blue from inside the ice."""
    x = 30.0
    for y in range(64):
        x += r.uniform(-0.8, 0.8)
        w = 1.5 + 1.5 * np.sin(np.pi * y / 64)
        for xx in range(int(x - w - 2), int(x + w + 3)):
            d = abs(xx + 0.5 - x)
            if d <= w:
                out[y, xx % 64] = pal.ramps["acc"][0] if d > w - 1 else pal.ramps["stone"][0]
                if d < 0.6 and (y % 9) in (3, 4):
                    out[y, xx % 64] = pal.ramps["acc"][1]
            elif d <= w + 1.2:
                out[y, xx % 64] = pal.ramps["stain"][1] if xx < x else pal.ramps["stone"][1]


def icefall(r, pal, out, V):
    """A frozen waterfall poured down the middle of the wall: vertical streaks of deep ice, a frost foot."""
    ac, fr = pal.ramps["acc"], pal.ramps["stain"]
    N = P.fbm(r, 64, 64, ((32, 3, .6), (8, 2, .4)))
    for y in range(64):
        half = 9 + 4 * (y / 64) ** 2 + 1.5 * np.sin(y * 0.3)
        for x in range(int(32 - half), int(32 + half) + 1):
            s = (x + 0.5 - 32) / half
            v = 0.55 - 0.35 * s + 0.6 * (N[y, x] - 0.5) + 0.15 * np.cos(x * 1.7)
            if abs(s) > 0.9:
                out[y, x] = pal.ramps["stone"][0]
            else:
                out[y, x] = ac[int(np.clip(v * 4, 0, 3))] if v < 0.8 else fr[int(np.clip((v - 0.8) * 10, 1, 4))]
    for y in range(56, 64):
        for x in range(14, 50):
            if abs(x - 32) < 18 - (63 - y) * 1.6 and r.random() < 0.9:
                out[y, x] = fr[int(np.clip(1 + r.random() * 2 + (y < 59), 0, 4))]


def paint(rng, pal):
    t = {"wall": wall(11, pal), "v1": wall(12, pal, ice=0.52, rime=0.8), "v2": wall(13, pal, feature=crevasse),
         "v3": wall(14, pal, ice=0.7, feature=icefall)}
    # floor: old ice over flagstones, frost drifts, cracks with a lit edge, cold glints
    r = np.random.default_rng(21)
    V, M, _ = K.blocks(r, pal, FLOOR, light=0, bevel=0.5, cracks=8, chips=2, pits=8)
    out = K.idx(pal, "stone", V, 0.8)
    N = P.fbm(r, 64, 64)
    ice = N > 0.45
    out[ice] = K.idx(pal, "acc", V * 0.95 + 0.2 * (N - 0.45), 0.75)[ice]
    drift = P.fbm(r, 64, 64) > 0.68
    out[drift] = K.idx(pal, "stain", 0.15 + 0.5 * (P.fbm(r, 64, 64) - 0.3), 0.8)[drift]
    glint = ice & ~drift & (np.roll(N, 1, 1) - N > 0.02) & (K.XX % 4 != 0) & (r.random((64, 64)) < 0.25)
    out[glint] = pal.ramps["stain"][2]
    t["floor"] = out
    # ceiling: dark rock, icicle roots as pale points in clusters
    r = np.random.default_rng(31)
    V, M, _ = K.blocks(r, pal, CEIL, light=0, bevel=0.7, cracks=2, chips=3, pits=10)
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(26):
        y, x = int(r.integers(0, 64)), int(r.integers(0, 64))
        out[y, x] = pal.ramps["stain"][int(r.integers(2, 5))]
        out[(y + 1) % 64, x] = pal.ramps["stain"][1]
        out[y, (x + 1) % 64] = pal.ramps["stone"][0]
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(12, 16, 34), (24, 32, 60), (40, 54, 92), (60, 80, 120)],
                                          trim=[(70, 84, 100), (120, 138, 154), (186, 200, 210)]))
