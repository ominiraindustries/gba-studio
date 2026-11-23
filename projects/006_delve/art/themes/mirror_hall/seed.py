"""DELVE theme 'mirror_hall' (G-36): a hall of polished obsidian slabs with a tarnished silver ledge, tall mirror
panes that show a dim reversed corridor and a ghostly copy of whoever stands there (painted into the texture),
a cracked mirror, violet crystals breaking through, a reflecting tiled floor.
Run: python -I seed.py [export-only | reseed]"""
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import theme_kit as K  # noqa: E402
from theme_kit import P, SA  # noqa: E402

RAMPS = [
    ("stone", P.ramp((3, 2, 7), (110, 102, 138), 10, (30, 26, 48))),       # obsidian
    ("stain", P.ramp((34, 12, 52), (204, 158, 234), 5)),                    # violet crystal
    ("wood", P.ramp((8, 6, 8), (64, 54, 60), 6)),                           # ebony
    ("iron", P.ramp((10, 10, 14), (128, 128, 142), 5)),                     # tarnished silver
    ("rust", P.ramp((24, 22, 30), (72, 66, 82), 3)),                        # tarnish
    ("flame", SA.FLAME),
    ("bone", P.ramp((24, 22, 26), (180, 172, 170), 5, (86, 80, 86))),
    ("cloth", P.ramp((22, 8, 30), (84, 34, 98), 3)),                        # violet velvet
    ("acc", P.ramp((12, 14, 20), (156, 166, 182), 5)),                      # mirror glass
]
SLAB = dict(heights=(30, 34), w=(28, 36), base=(0.36, 0.5), joint2=0.0)
X0, X1, Y0, Y1 = 15, 48, 7, 56                                               # mirror pane (arched top)


def obsidian(r, pal, veins=3):
    V, M, _ = K.blocks(r, pal, SLAB, light=0.5, noise_amp=0.14, fine_amp=0.04, pits=4, cracks=0, chips=1)
    sheen = np.sin((K.XX * 0.8 + K.YY) * 0.11 + 3 * P.fbm(r, 64, 64)) > 0.9    # soft polished sheen
    V = V + 0.12 * sheen
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(veins):                                                  # thin crystal veins
        y, x = int(r.integers(0, 64)), int(r.integers(0, 64))
        for _k in range(int(r.integers(8, 20))):
            out[y % 64, x % 64] = pal.ramps["stain"][int(r.integers(0, 2))]
            y += 1
            x += int(r.choice([-1, 0, 1]))
    for x in range(64):                                                     # silver ledge
        out[30, x] = pal.ramps["iron"][4 if x % 11 else 3]
        out[31, x] = pal.ramps["iron"][2]
        out[32, x] = pal.ramps["iron"][0]
        out[33, x] = pal.ramps["stone"][0]
    return out


def pane_mask():
    m = np.zeros((64, 64), bool)
    m[Y0 + 8:Y1 + 1, X0:X1 + 1] = True
    m |= K.ellipse((X0 + X1 + 1) / 2, Y0 + 9, (X1 - X0 + 1) / 2, 9) & (K.YY < Y0 + 9)
    return m


def mirror(r, pal, out, ghost=True, cracked=False):
    ac, ir = pal.ramps["acc"], pal.ramps["iron"]
    m = pane_mask()
    frame = m & ~(np.roll(m, 2, 0) & np.roll(m, -2, 0) & np.roll(m, 2, 1) & np.roll(m, -2, 1))
    inner = m & ~frame
    cx, vy = 31.5, 30.0                                                   # vanishing point of the reflection
    V = 0.13 + 0.0 * K.XX
    dx, dy = K.XX + 0.5 - cx, K.YY + 0.5 - vy
    far = (np.abs(dx) < 4) & (np.abs(dy) < 5)
    floor = (dy > 5) & (np.abs(dx) < dy * 1.6 + 4)
    V = np.where(floor, 0.08 + 0.03 * ((K.YY // 4) % 2), V)                # reflected floor
    V = np.where(far, 0.30, V)                                           # the far end of the reflected corridor
    V = V - 0.12 * (np.abs(dx) > 12)
    for k in (-1, 1):                                                    # corridor edges converging
        ln = np.abs(dx - k * (np.abs(dy) * 1.6 + 4)) < 0.6
        V = np.where(ln & (np.abs(dy) > 5), 0.24, V)
    V = V + 0.28 * (np.abs((K.XX - K.YY * 0.55) - 2) < 1.6) + 0.12 * (np.abs((K.XX - K.YY * 0.55) - 7) < 0.8)
    V = V + 0.05 * (r.random((64, 64)) - 0.5) - 0.12 * (K.YY > Y1 - 5)
    if ghost:                                                            # pale half-seen copy of the viewer
        g = K.ellipse(cx, 25, 3.6, 4.4) | K.poly([(cx - 4, 29), (cx + 4, 29), (cx + 9, 36), (cx + 10, 56),
                                                  (cx - 10, 56), (cx - 9, 36)])
        sh = K.poly([(cx - 12, 37), (cx - 9, 36), (cx - 7, 56), (cx - 11, 56)]) | \
            K.poly([(cx + 9, 36), (cx + 12, 37), (cx + 11, 56), (cx + 7, 56)])
        gv = 0.46 + 0.2 * (K.XX < cx) - 0.2 * ((K.YY - 25) / 30.0)
        mix = ((K.XX + K.YY) % 2 == 0) | (K.YY < 34)
        V = np.where(g & mix, gv, np.where(g, (V + gv) / 2, V))
        V = np.where(sh & mix, 0.4, V)
        for ex in (-1.5, 1.5):                                           # hollow eyes
            V[24, int(cx + ex)] = 0.02
    out[inner] = K.idx(pal, "acc", V, 0.85, 0.5)[inner]
    if cracked:
        hx, hy = 38, 22
        for a in np.linspace(0, 2 * np.pi, 9, endpoint=False) + r.uniform(0, 0.5):
            x, y = float(hx), float(hy)
            L = r.uniform(10, 30)
            for k in range(int(L)):
                xi, yi = int(x), int(y)
                if inner[yi % 64, xi % 64]:
                    out[yi, xi] = ac[0] if k % 5 else ac[4]
                    if inner[yi, (xi + 1) % 64]:
                        out[yi, xi + 1] = ac[3]
                x += np.cos(a) + r.uniform(-0.3, 0.3)
                y += np.sin(a) + r.uniform(-0.3, 0.3)
        hole = K.poly([(35, 18), (42, 17), (44, 25), (37, 28)]) & inner
        out[hole] = pal.ramps["stone"][0]
        out[K.edge(hole, -1, 0) | K.edge(hole, 0, -1)] = ac[4]
    fv = 0.6 - 0.35 * (K.XX > (X0 + X1) / 2) + 0.15 * ((K.XX + K.YY) % 4 == 0)
    out[frame] = K.idx(pal, "iron", fv, 0.8)[frame]
    out[frame & K.edge(m, 1, 0)] = ir[4]
    shadow = np.roll(np.roll(m, 1, 0), 1, 1) & ~m
    out[shadow] = pal.ramps["stone"][0]


def cluster(r, pal, out, n=6, spread=14, h=(22, 44)):
    for k in range(n):
        bx = 32 + r.uniform(-spread, spread)
        lean = (bx - 32) * 0.6 + r.uniform(-4, 4)
        tip_y = 63 - r.uniform(*h) * (1 - abs(bx - 32) / (spread * 2.2))
        K.crystal(pal, out, "stain", (bx, 66), (bx + lean, tip_y), r.uniform(2.5, 4.5), r)
    glow = K.ellipse(32, 63, 22, 7) & (out < pal.ramps["stain"][0])
    out[glow & ((K.XX + K.YY) % 2 == 0)] = pal.ramps["stone"][3]


def paint(rng, pal):
    r = [np.random.default_rng(s) for s in (71, 72, 73, 74)]
    wall = obsidian(r[0], pal)
    mirror(r[0], pal, wall, ghost=False)
    v1 = obsidian(r[1], pal, 1)
    mirror(r[1], pal, v1, ghost=True)
    v2 = obsidian(r[2], pal, 2)
    mirror(r[2], pal, v2, ghost=False, cracked=True)
    v3 = obsidian(r[3], pal, 5)
    cluster(r[3], pal, v3)
    t = {"wall": wall, "v1": v1, "v2": v2, "v3": v3}
    # floor: polished tiles in two obsidian tones, with smeared vertical reflections and a few crystal grains
    rf = np.random.default_rng(81)
    check = ((K.XX // 32 + K.YY // 32) % 2) == 0
    V = np.where(check, 0.32, 0.2) + 0.06 * (P.fbm(rf, 64, 64) - 0.5)
    V += 0.12 * (P.fbm(rf, 64, 64, ((32, 2, .7), (16, 1, .3))) > 0.62)
    V[(K.XX % 32 == 0) | (K.YY % 32 == 0)] = 0.04
    V[(K.XX % 32 == 1) | (K.YY % 32 == 1)] += 0.1
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(10):
        y, x = int(rf.integers(0, 64)), int(rf.integers(0, 64))
        out[y, x] = pal.ramps["stain"][int(rf.integers(1, 4))]
    t["floor"] = out
    rc = np.random.default_rng(91)
    V, M, _ = K.blocks(rc, pal, dict(heights=(32, 32), w=(28, 36), base=(0.12, 0.2)), light=0, noise_amp=0.1)
    out = K.idx(pal, "stone", V, 0.8)
    for _ in range(4):
        x, y = rc.uniform(6, 58), rc.uniform(6, 58)
        K.crystal(pal, out, "stain", (x, y), (x + rc.uniform(-5, 5), y + rc.uniform(-5, 5)), 2.0, rc, 0.5)
    t["ceil"] = out
    return t


if __name__ == "__main__":
    pal = K.palette(RAMPS)
    witch = [(70, 30, 120), (140, 90, 220), (200, 170, 255), (244, 236, 255)]            # violet witch-fire
    glow = [(70, 52, 96), (84, 60, 118), (100, 70, 140), (120, 84, 164), (142, 104, 190), (168, 132, 214),
            (198, 170, 236)]
    K.build(HERE, pal, paint, K.deco_from(pal, flame=witch, glow=glow, warm=(176, 150, 220),
                                          cloth=[(22, 8, 30), (44, 16, 58), (66, 26, 82), (90, 40, 108)],
                                          trim=[(80, 80, 92), (130, 130, 144), (196, 196, 208)]))
