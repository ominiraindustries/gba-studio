"""DELVE theme 'lava_forge' (G-36): forge depths in black basalt - columnar joints, cracks glowing orange with the
rock around them lit red, molten seep down a fissure, a forge vent full of coals, a cooled-crust floor split by
glowing seams, a soot ceiling lit from below. The glow uses the flame ramp (good for palette cycling later).
Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((4, 4, 6), (100, 96, 100), 10, (32, 30, 34))),        # basalt
    ("stain", P.ramp((36, 10, 8), (150, 58, 24), 5)),                       # rock lit red by the heat
    ("wood", P.ramp((8, 6, 5), (70, 52, 40), 6)),                           # charred
    ("iron", P.ramp((10, 9, 9), (108, 98, 92), 5)),
    ("rust", P.ramp((44, 18, 8), (130, 56, 20), 3)),
    ("flame", SA.FLAME),                                                    # lava, embers
    ("bone", P.ramp((24, 22, 18), (176, 160, 130), 5, (86, 76, 62))),
    ("cloth", P.ramp((24, 8, 6), (84, 30, 18), 3)),
    ("acc", P.ramp((26, 22, 22), (92, 82, 76), 4)),                        # cooled crust, ash
]
COLS = dict(heights=(11, 9, 12, 10, 12, 10), w=(10, 22), base=(0.26, 0.46), joint2=0.25, jitter=0)


def basalt(r, pal):
    """Columnar basalt: the sewer course layout turned on its side, each column rounded (lit left)."""
    ids, M = P.courses(r, COLS["heights"], COLS["w"][0], COLS["w"][1], joint2=COLS["joint2"])
    ids, M = ids.T.copy(), M.T.copy()
    V = P.stone_shade(r, ids, M, base=COLS["base"], cracks=3, chips=4, pits=14)
    for c in np.unique(ids):
        m = ids == c
        xs = np.nonzero(m.any(0))[0]
        if len(xs) and xs.max() - xs.min() < 20:
            s = (K.XX - xs.min()) / max(1, xs.max() - xs.min())
            V[m] += (0.12 - 0.24 * s)[m]
    return K.torch_fall(V, 0.4), M


def glow_line(r, pal, out, pts, heat=1.0):
    """Molten crack along a polyline; red-lit rock in a 2-3 px halo around it."""
    fl, sn = pal.ramps["flame"], pal.ramps["stain"]
    core = np.zeros((64, 64), bool)
    for (x0, y0), (x1, y1) in zip(pts[:-1], pts[1:]):
        n = int(max(abs(x1 - x0), abs(y1 - y0))) + 1
        for t in np.linspace(0, 1, n):
            core[int(y0 + (y1 - y0) * t) % 64, int(x0 + (x1 - x0) * t) % 64] = True
    d = SA.ring_dist(core, 3)
    halo = (d <= 3) & ~core
    out[halo & (d == 1)] = sn[int(3 * heat)]
    out[halo & (d == 2)] = sn[int(2 * heat)]
    out[halo & (d == 3) & ((K.XX + K.YY) % 2 == 0)] = sn[0]
    lv = (2 + heat * 1.6 + r.random((64, 64)) * 0.9).astype(int).clip(0, 4)
    out[core] = np.array(fl)[lv][core]
    return core


def crack_path(r, x, y, n, step=(-1, 2)):
    pts = [(x, y)]
    for _ in range(n):
        x += r.uniform(-3, 3)
        y += r.uniform(*step) + 2
        pts.append((x, y))
    return pts


def wall(seed, pal, n=2, feature=None):
    r = np.random.default_rng(seed)
    V, M = basalt(r, pal)
    heat = np.clip((K.YY - 44) / 20.0, 0, 1) * (P.fbm(r, 64, 64) > 0.35)     # floor glow climbing the foot
    out = K.idx(pal, "stone", V, 0.85)
    hm = (heat > 0.45) & ~M
    out[hm] = K.idx(pal, "stain", V * 0.9 + 0.25 * heat - 0.1, 0.9)[hm]
    for _ in range(n):
        glow_line(r, pal, out, crack_path(r, r.uniform(4, 60), r.uniform(-10, 30), int(r.integers(4, 9))),
                  r.uniform(0.5, 1.0))
    if feature:
        feature(r, pal, out)
    return out


def seep(r, pal, out):
    pts = [(30, 0), (31, 8), (28, 16), (30, 24), (33, 30)]
    glow_line(r, pal, out, pts, 1.0)
    fl = pal.ramps["flame"]
    for y in range(30, 64):                                          # molten tongue running down, cooling
        w = 2 + (y - 30) / 10.0
        for x in range(int(33 - w), int(33 + w) + 1):
            cool = (y - 30) / 34.0
            v = 1 - abs(x + 0.5 - 33) / (w + 0.5) - cool * 0.8 + r.uniform(-0.15, 0.15)
            if v > 0.05:
                out[y, x] = fl[int(np.clip(v * 4.5, 0, 4))] if v > 0.25 else pal.ramps["stain"][2]
            elif v > -0.3:
                out[y, x] = pal.ramps["stain"][1]


def vent(r, pal, out):
    st, ir, fl = pal.ramps["stone"], pal.ramps["iron"], pal.ramps["flame"]
    cx, top, bot = 32, 34, 56
    mouth = K.ellipse(cx, top + 7, 13, 9) & (K.YY < top + 7) | ((np.abs(K.XX + 0.5 - cx) < 13) & (K.YY >= top + 7)
                                                              & (K.YY <= bot))
    ring = SA.ring_dist(mouth, 3)
    out[(ring <= 3) & ~mouth & (K.YY <= bot + 1)] = st[1]
    out[(ring == 1) & ~mouth & (K.YY <= bot)] = st[4]
    coal = P.fbm(r, 64, 64, ((4, 4, .6), (2, 2, .4)))
    lv = np.clip((coal - 0.3) * 6 + (K.YY - top) / 8.0 - 1.0, -1, 4)
    hot = mouth & (lv >= 0)
    out[mouth] = st[0]
    out[hot] = np.array(fl)[lv.astype(int).clip(0, 4)][hot]
    out[mouth & (lv < 0) & (lv > -0.6)] = pal.ramps["stain"][1]
    for x in range(cx - 11, cx + 12, 4):                              # iron grate bars
        for y in range(top, bot + 1):
            if mouth[y, x]:
                out[y, x] = ir[3 if y < top + 6 else 2]
                if mouth[y, x + 1]:
                    out[y, x + 1] = ir[0]
    for y in (top + 8, top + 15):
        out[y, cx - 13:cx + 13][mouth[y, cx - 13:cx + 13]] = ir[3]


def paint(rng, pal):
    t = {"wall": wall(131, pal, 1), "v1": wall(132, pal, 3), "v2": wall(133, pal, 1, seep),
         "v3": wall(134, pal, 0, vent)}
    rf = np.random.default_rng(141)
    plates = np.zeros((64, 64), int)
    pts = rf.uniform(0, 64, (9, 2))
    d = np.full((64, 64), 1e9)
    d2 = np.full((64, 64), 1e9)
    for k, (px, py) in enumerate(pts):                                # voronoi crust plates (wrapping)
        dx = np.minimum(np.abs(K.XX - px), 64 - np.abs(K.XX - px))
        dy = np.minimum(np.abs(K.YY - py), 64 - np.abs(K.YY - py))
        dd = np.hypot(dx, dy)
        upd = dd < d
        d2 = np.where(upd, d, np.minimum(d2, dd))
        plates = np.where(upd, k, plates)
        d = np.minimum(d, dd)
    seam = ((d2 - d) < 0.8) & (P.fbm(rf, 64, 64) > 0.38)
    V = 0.16 + 0.1 * ((plates * 37) % 5) / 4 + 0.3 * (P.fbm(rf, 64, 64) - 0.5) + 0.12 * np.clip(d2 - d - 2, 0, 3) / 3
    out = K.idx(pal, "acc", V * 0.62, 0.8)
    near = SA.ring_dist(seam, 2)
    hal = (near <= 2) & ~seam
    out[hal] = K.idx(pal, "stain", 0.55 - 0.2 * near, 1.0)[hal]
    lv = (0.6 + 2.4 * P.fbm(rf, 64, 64)).astype(int).clip(0, 3)
    out[seam] = np.array(pal.ramps["flame"])[lv][seam]
    t["floor"] = out
    rc = np.random.default_rng(151)
    V, M, _ = K.blocks(rc, pal, dict(heights=(16, 16, 16, 16), w=(12, 24), base=(0.1, 0.22), joint2=0.3), light=0)
    out = K.idx(pal, "stone", V, 0.8)
    red = (P.fbm(rc, 64, 64) > 0.6) & ~M & ((K.XX + K.YY) % 2 == 0)
    out[red] = pal.ramps["stain"][0]
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    K.build(HERE, pal, paint, K.deco_from(pal, cloth=[(20, 8, 6), (44, 14, 10), (70, 24, 14), (96, 36, 20)],
                                          trim=[(60, 52, 46), (100, 88, 78), (150, 130, 110)]))
