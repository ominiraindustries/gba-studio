"""DELVE art, style B "wet sewer" (G-33 phase 2): textures, monsters and props as seed PNGs in art/_seed/out/.

Dark retro VGA-era look: small green-black bricks, slime low on the walls, standing water on the floor, painted
shading inside every shape (no cartoon outline). Seeds only: seed_all.py turns them into art/*.aseprite when the
.aseprite is missing and exports every .aseprite.

  textures  tex_wall_stone (+ _v1.._v3), tex_floor, tex_ceiling, tex_door (+ _open, _locked), tex_arch, tex_bars,
            tex_stairs_down, tex_wall_* (crack, dart, skulls, and the legacy painted torch/banner/chains/lever)
            Openings keep the exact index-0 masks of the previous art (art/_seed/legacy/), which the engine reads.
  monsters  mon_rat, mon_skeleton, mon_slime: 'clay' models (capsules / ellipsoids, z-buffer), key light from the
            upper left + front, a thin cool rim light on the right edge, re-posed per frame (idle 2, attack 2, hit 1).
  props     prop_*: the previous props re-graded to the sewer (dark outline turned into shading, top-left edge lit).

Run: python -I sewer_art.py
"""
import os
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import sewer_paint as P  # noqa: E402

OUT = os.path.join(HERE, "out")
LEG = os.path.join(HERE, "legacy")
DITH = 0.6          # ordered-dither strength (lower = less shimmer when the view moves)

FLAME = [(58, 12, 8), (142, 38, 10), (214, 96, 22), (248, 172, 58), (255, 236, 176)]


def env_palette():
    pal = P.Pal()
    pal.add("stone", P.ramp((5, 8, 7), (92, 106, 88), 10, (32, 44, 38)))
    pal.add("stain", P.ramp((14, 26, 12), (104, 132, 50), 5))
    pal.add("wood", P.ramp((11, 11, 8), (72, 66, 44), 6))
    pal.add("iron", P.ramp((10, 13, 11), (86, 96, 84), 5))
    pal.add("rust", P.ramp((34, 26, 10), (100, 74, 26), 3))
    pal.add("flame", FLAME)
    pal.add("bone", P.ramp((24, 24, 16), (168, 162, 124), 5, (82, 84, 62)))
    pal.add("cloth", P.ramp((26, 9, 8), (104, 38, 26), 3))
    return pal


# ------------------------------------------------------------------------------------------------ surfaces
WALL = dict(heights=(11, 10, 11, 10, 11, 11), w=(12, 20), base=(0.32, 0.52), joint2=0.2)
FLOOR = dict(heights=(16, 16, 16, 16), w=(14, 22), base=(0.22, 0.38), joint2=0.25)
CEIL = dict(heights=(22, 21, 21), w=(18, 30), base=(0.13, 0.25), joint2=0.25)


def surface(rng, pal, lay, kind, stain_n=16, damp=0.75):
    ids, M = P.courses(rng, lay["heights"], lay["w"][0], lay["w"][1], joint2=lay["joint2"])
    flat = kind != "wall"
    V = P.stone_shade(rng, ids, M, base=lay["base"], bevel=0.6 if flat else 1.0,
                      cracks=4 if kind != "ceil" else 2, chips=3, pits=16 if not flat else 10)
    h, w = V.shape
    S = np.zeros((h, w))
    if kind == "wall":
        S = np.maximum(S, P.drips(rng, M, stain_n, 5, 26))
        yy = np.arange(h)[:, None] / h
        S = np.maximum(S, np.clip((yy - (1 - damp)) / damp, 0, 1) * P.fbm(rng, h, w) * 1.3)
    elif kind == "floor":
        S = np.clip((P.fbm(rng, h, w) - 0.55) * 2.6, 0, 1)
    else:
        S = np.clip((P.fbm(rng, h, w) - 0.5) * 2.0, 0, 1) * 0.8          # soot
    V = V * (1 - 0.40 * S)
    if kind == "wall":      # carried-torch light: the top and bottom of the wall sink into darkness
        V = V * (0.55 + 0.45 * np.sin(np.pi * (np.arange(h)[:, None] + 6) / 76.0))
    n = len(pal.ramps["stone"])
    lv = P.quant(np.clip(V / 0.80, 0, 1), n, DITH)
    out = pal.idx("stone", lv)
    ns = len(pal.ramps["stain"])
    if kind != "ceil":
        st = (S > 0.55) & ~M if kind == "wall" else (S > 0.45) & ~M
        sl = P.quant(np.clip(V / 0.7, 0, 1), ns, DITH)
        out[st] = pal.idx("stain", sl)[st]
    if kind == "floor":          # standing water: dark with a cold glint line
        water = (P.fbm(rng, h, w) > 0.62)
        out[water] = pal.ramps["stone"][1]
        glint = water & ~np.roll(water, 1, 0) & (np.arange(w)[None, :] % 3 != 0)
        out[glint] = pal.ramps["stone"][5]
    out[M] = np.where(V[M] < 0.05, pal.ramps["stone"][0], pal.ramps["stone"][1])
    return out


def drain(out, pal, rng):
    """Small arched drain outlet low on the wall: black mouth, iron grate, slime tongue below."""
    st, sl, ir = pal.ramps["stone"], pal.ramps["stain"], pal.ramps["iron"]
    cx, top, bot = 32, 44, 55
    for y in range(top - 2, bot + 2):
        for x in range(cx - 10, cx + 10):
            r = ((x + 0.5 - cx) / 9.5) ** 2 + ((y + 0.5 - (top + 6)) / 8.0) ** 2 if y < top + 6 else \
                ((x + 0.5 - cx) / 9.5) ** 2
            if r <= 1.0 and y <= bot:
                out[y, x] = st[0] if r < 0.78 else st[3 + (y < top + 4)]
    for x in range(cx - 7, cx + 8, 3):          # grate bars
        for y in range(top + 1, bot + 1):
            if out[y, x] == st[0]:
                out[y, x] = ir[2 if y < top + 5 else 1]
                if out[y, x + 1] == st[0]:
                    out[y, x + 1] = ir[0]
    for y in range(bot + 1, 64):                # slime tongue spilling out
        half = 4 + (y - bot) // 3
        for x in range(cx - half, cx + half):
            if rng.random() < 0.85:
                out[y, x] = sl[int(np.clip(1 + (cx - x) / 4 + rng.random() * 1.4, 0, 4))]


# ------------------------------------------------------------------------------------------------ legacy remap
# previous environment palette ranges (old art/LÉEME.md): material of each old index
OLD = [(0, 0, None), (1, 1, "void"), (2, 9, "stone"), (10, 14, "moss"), (15, 21, "wood"), (22, 27, "iron"),
       (28, 32, "flame"), (33, 36, "bone"), (37, 39, "cloth")]


def legacy(stem):
    return np.array(Image.open(os.path.join(LEG, stem + ".bmp")))


def remap(old, old_wall, new_wall, pal, rng, stone_gain=0.8):
    """Old overlay pixels (those differing from the old plain wall) re-coloured into the new ramps by material and
    relative level; plain wall pixels become the new wall; moss is dropped; index 0 stays 0."""
    out = new_wall.copy()
    ov = old != old_wall
    noise = (rng.random(old.shape) - 0.5) * 0.7
    for a, b, mat in OLD:
        sel = ov & (old >= a) & (old <= b)
        if mat is None:
            out[sel] = 0
        elif mat == "void":
            out[sel] = pal.ramps["stone"][0]
        elif mat == "moss":
            continue
        else:
            ramp = pal.ramps[mat]
            t = (old[sel] - a) / max(1, b - a)
            if mat == "stone":
                t = t * stone_gain
            lvl = np.clip(np.round(t * (len(ramp) - 1) + noise[sel]), 0, len(ramp) - 1).astype(int)
            out[sel] = np.array(ramp)[lvl]
    return out


def ring_dist(hole, kmax=5):
    dist = np.full(hole.shape, 99)
    grown = hole.copy()
    for k in range(1, kmax + 1):
        g = grown.copy()
        g[1:] |= grown[:-1]
        g[:-1] |= grown[1:]
        g[:, 1:] |= grown[:, :-1]
        g[:, :-1] |= grown[:, 1:]
        dist[g & ~grown & (dist == 99)] = k
        grown = g
    return dist


def arch_ring(rng, pal, out, hole, width=4):
    """Dressed voussoir stones around an opening (above its lowest row), with a shadowed reveal."""
    ys, xs = np.nonzero(hole)
    cy, cx, ybot = ys.min() + 14, (xs.min() + xs.max()) / 2.0, ys.max()
    dist = ring_dist(hole, width + 1)
    yy, xx = np.mgrid[0:64, 0:64]
    ang = np.arctan2(yy - cy, xx - cx)
    seg = np.floor((ang + np.pi) / (np.pi / 9)).astype(int)
    seg = np.where(yy > cy, 100 + (yy - cy) // 7, seg)          # jamb blocks below the springing line
    ring = (dist <= width) & ~hole & (yy <= ybot)
    n = len(pal.ramps["stone"])
    base = 0.40 + 0.10 * ((seg * 37) % 5) / 4.0
    V = base + 0.25 * (P.fbm(rng, 64, 64) - 0.5) - 0.06 * (dist - 1) + 0.10 * (yy < cy)
    V[ring & ((seg != np.roll(seg, 1, 1)) | (seg != np.roll(seg, 1, 0)))] = 0.05
    V[dist == 1] = 0.06
    lv = P.quant(np.clip(V / 0.8, 0, 1), n, DITH)
    out[ring] = pal.idx("stone", lv)[ring]
    return ring


def door_set(rng, pal, wall):
    """Closed, open and locked door on the new wall, with the legacy masks."""
    old_open, old_door, old_wall = legacy("tex_door_open"), legacy("tex_door"), legacy("tex_wall_stone")
    hole = old_open[:64] == 0
    ys, xs = np.nonzero(hole)
    diff = np.nonzero(old_door[:64, 32] != old_wall[:64, 32])[0]
    leaf_bot = int(diff.max())
    leaf = hole.copy()
    bot_cols = np.nonzero(hole[ys.max()])[0]
    leaf[ys.max():leaf_bot + 1, bot_cols.min():bot_cols.max() + 1] = True
    closed = wall.copy()
    arch_ring(rng, pal, closed, hole)
    P.door_leaf(rng, pal, closed, leaf, None)
    opened = closed.copy()
    opened[hole] = 0
    gap = leaf & ~hole
    opened[gap] = pal.ramps["stone"][0]              # dark sill gap where the leaf stood
    # the swung leaf, seen edge-on along the left jamb (as before)
    old_leaf = (old_open[:64] >= 15) & (old_open[:64] <= 27) & (old_open[:64] != old_wall[:64])
    opened[old_leaf] = remap(old_open[:64], old_wall[:64], wall, pal, rng)[old_leaf]
    locked = closed.copy()
    ir, ru = pal.ramps["iron"], pal.ramps["rust"]
    x0, x1 = bot_cols.min(), bot_cols.max()
    by = (ys.min() + leaf_bot) // 2 + 1
    for x in range(x0, x1 + 1):                       # heavy iron band across the leaf
        for j, l in enumerate((4, 3, 2, 2, 1, 0)):
            if leaf[by + j, x]:
                locked[by + j, x] = ir[l if rng.random() > 0.2 else max(0, l - 1)]
        if leaf[by + 6, x] and rng.random() < 0.5:
            locked[by + 6, x] = ru[1]
    cx = int((x0 + x1) // 2)                          # big padlock hanging from the band
    lock = [" .ooo. ", ".o...o.", ".o...o.", "LLLLLLL", "LHHHHML", "LHHkHML", "LHHkHML", "LHHHHML", "LMMMMML",
            " LLLLL "]
    cmap = {"o": ir[3], "L": ir[0], "H": ru[2], "M": ru[0], "k": pal.ramps["stone"][0]}
    for j, row in enumerate(lock):
        for i, ch in enumerate(row):
            if ch in cmap:
                locked[by - 2 + j, cx - 3 + i] = cmap[ch]
    return closed, opened, locked


def tex_strip(frames):
    return np.concatenate(frames, axis=0)


def save_idx(path, idx, cols, transparent0=False):
    im = Image.fromarray(idx.astype(np.uint8), "P")
    flat = [c for col in cols for c in col]
    im.putpalette(flat + [0] * (768 - len(flat)))
    if transparent0:
        im.save(path, transparency=0)
    else:
        im.save(path)


def build_textures():
    pal = env_palette()
    cols = pal.cols + [(0, 0, 0)] * (64 - len(pal.cols))
    rng = np.random.default_rng(23)
    tex = {}
    wall = surface(rng, pal, WALL, "wall")
    tex["tex_wall_stone"] = wall
    tex["tex_wall_stone_v1"] = surface(np.random.default_rng(101), pal, WALL, "wall", stain_n=10, damp=0.6)
    tex["tex_wall_stone_v2"] = surface(np.random.default_rng(202), pal, WALL, "wall", stain_n=26, damp=0.9)
    v3 = surface(np.random.default_rng(303), pal, WALL, "wall", stain_n=12, damp=0.7)
    drain(v3, pal, np.random.default_rng(304))
    tex["tex_wall_stone_v3"] = v3
    tex["tex_floor"] = surface(rng, pal, FLOOR, "floor")
    tex["tex_ceiling"] = surface(rng, pal, CEIL, "ceil")
    tex["tex_door"], tex["tex_door_open"], tex["tex_door_locked"] = door_set(rng, pal, wall)
    old_wall = legacy("tex_wall_stone")
    for stem in ("tex_arch", "tex_bars", "tex_stairs_down", "tex_wall_crack", "tex_wall_dart", "tex_wall_skulls",
                 "tex_wall_banner", "tex_wall_chains", "tex_wall_lever_up", "tex_wall_lever_down", "tex_wall_torch"):
        old = legacy(stem)
        frames = [remap(old[k:k + 64], old_wall, wall, pal, rng) for k in range(0, old.shape[0], 64)]
        if stem in ("tex_arch", "tex_stairs_down"):
            for f, o in zip(frames, range(0, old.shape[0], 64)):
                hole = old[o:o + 64] == 0
                arch_ring(rng, pal, f, hole)
        tex[stem] = tex_strip(frames)
    for stem, arr in tex.items():
        save_idx(os.path.join(OUT, stem + ".png"), arr, cols)
    used = sorted(set(np.unique(np.concatenate([a.ravel() for a in tex.values()]))))
    print("textures:", len(tex), "palette", len(pal.cols), "used", len(used))
    return pal


# ------------------------------------------------------------------------------------------------ clay models
class Clay:
    def __init__(self, h=64, w=64):
        self.h, self.w = h, w
        self.Z = np.full((h, w), -1e9)
        self.N = np.zeros((h, w, 3))
        self.MAT = np.zeros((h, w), int)
        self.PID = np.zeros((h, w), int)
        self.pid = 0
        self.yy, self.xx = np.mgrid[0:h, 0:w] + 0.5

    def put(self, mask, z, n, mat):
        sel = mask & (z > self.Z)
        self.Z[sel] = z[sel]
        self.N[sel] = n[sel]
        self.MAT[sel] = mat
        self.PID[sel] = self.pid

    def capsule(self, p0, p1, r0, r1=None, z0=0.0, z1=None, mat=1):
        self.pid += 1
        r1 = r0 if r1 is None else r1
        z1 = z0 if z1 is None else z1
        ax, ay = p1[0] - p0[0], p1[1] - p0[1]
        L2 = ax * ax + ay * ay + 1e-9
        t = np.clip(((self.xx - p0[0]) * ax + (self.yy - p0[1]) * ay) / L2, 0, 1)
        r = r0 + (r1 - r0) * t
        dx, dy = self.xx - (p0[0] + ax * t), self.yy - (p0[1] + ay * t)
        d2 = dx * dx + dy * dy
        nz = np.sqrt(np.clip(1 - d2 / (r * r), 0, 1))
        self.put(d2 <= r * r, z0 + (z1 - z0) * t + nz * r, np.stack([dx / r, dy / r, nz], -1), mat)

    def ellipse(self, c, rx, ry, z0=0.0, mat=1, flat=1.0, bump=None):
        self.pid += 1
        dx, dy = (self.xx - c[0]) / rx, (self.yy - c[1]) / ry
        d2 = dx * dx + dy * dy
        if bump is not None:
            d2 = d2 * (1 + bump)
        nz = np.sqrt(np.clip(1 - d2, 0, 1))
        n = np.stack([dx * flat, dy * flat, nz], -1)
        n /= np.linalg.norm(n, axis=-1, keepdims=True) + 1e-9
        self.put(d2 <= 1, z0 + nz * min(rx, ry), n, mat)

    def chain(self, pts, r0, r1, mat=1):
        for k, (a, b) in enumerate(zip(pts[:-1], pts[1:])):
            t0, t1 = k / (len(pts) - 1), (k + 1) / (len(pts) - 1)
            self.capsule(a[:2], b[:2], r0 + (r1 - r0) * t0, r0 + (r1 - r0) * t1, z0=a[2], z1=b[2], mat=mat)

    def light(self, rng, grime=0.10, flash=0.0):
        L = np.array([-0.55, -0.6, 0.6])
        L /= np.linalg.norm(L)
        diff = np.clip((self.N * L).sum(-1), 0, 1)
        I = (0.10 + 1.08 * diff ** 1.15 + 0.10 * self.N[..., 2]) * (1.0 - 0.18 * (self.yy / self.h))
        occ = np.zeros((self.h, self.w))
        for dy, dx in ((-1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (-2, 0), (-1, 2), (-2, -1)):
            Zn = np.roll(np.roll(self.Z, -dy, 0), -dx, 1)
            Pn = np.roll(np.roll(self.PID, -dy, 0), -dx, 1)
            Mn = np.roll(np.roll(self.MAT, -dy, 0), -dx, 1)
            occ += ((Mn > 0) & (Pn != self.PID) & (Zn > self.Z + 1.0)) * (0.16 if abs(dy) + abs(dx) == 1 else 0.09)
        I = I * (1 - np.clip(occ, 0, 0.6))
        I += grime * (P.fbm(rng, self.h, self.w, ((4, 4, .6), (2, 2, .4))) - 0.5)
        return I * (1 + flash) + 0.25 * flash

    def rim(self):
        """Warm 1 px rim on the top and side silhouette (the player's torch grazing the edges), never the bottom,
        so the shape separates from the dark sewer walls."""
        on = self.MAT > 0
        up, lf, rt = ~np.roll(on, 1, 0), ~np.roll(on, 1, 1), ~np.roll(on, -1, 1)
        return on & (up | lf | rt) & (self.N[..., 2] < 0.45)


def quant(I, n, b=None):
    return P.quant(np.clip(I, 0, 0.999), n, 0.6, b)


def frames_to_strip(frames):
    return np.concatenate(frames, axis=0)


def ik(sh, hand, l1, l2, bend):
    """Elbow position for a 2-bone arm from shoulder sh to hand (bend = +1/-1 side)."""
    dx, dy = hand[0] - sh[0], hand[1] - sh[1]
    d = min(np.hypot(dx, dy), l1 + l2 - 0.01)
    a = np.arctan2(dy, dx)
    c = np.clip((l1 * l1 + d * d - l2 * l2) / (2 * l1 * d), -1, 1)
    b = a + bend * np.arccos(c)
    return (sh[0] + l1 * np.cos(b), sh[1] + l1 * np.sin(b))


# ---- skeleton: weight on its left leg (viewer's right), hip cocked, head tilted, rusty blade in the right hand
SK_POSES = [  # lean x, crouch y, head tilt x, sword hand, sword angle (deg, 0 = right, 90 = down), free hand, flash
    dict(lean=-1.5, crouch=0.0, tilt=-2.5, hand=(19.0, 39.0), sword=124, free=(40.0, 31.0), jaw=0, flash=0),
    dict(lean=-1.0, crouch=0.8, tilt=-3.0, hand=(19.5, 39.6), sword=122, free=(40.5, 32.0), jaw=1, flash=0),
    dict(lean=-4.0, crouch=1.5, tilt=-2.5, hand=(13.0, 12.0), sword=250, free=(47.0, 36.0), jaw=1, flash=0),
    dict(lean=2.5, crouch=2.5, tilt=1.0, hand=(38.0, 40.0), sword=40, free=(49.0, 44.0), jaw=2, flash=0),
    dict(lean=-5.0, crouch=0.5, tilt=-4.0, hand=(14.0, 36.0), sword=135, free=(50.0, 34.0), jaw=2, flash=0.45),
]


def skeleton_frame(rng, p):
    c = Clay()
    lean, cr, tilt = p["lean"], p["crouch"], p["tilt"]
    # legs: viewer's left leg straight under the weight, right leg bent and stepped forward
    c.chain([(28, 44 + cr * 0.4, 0), (26.5, 53.5, 0.5), (25.5, 62, 0.5)], 2.1, 1.4)
    c.ellipse((26.5, 53.5), 2.3, 2.0, z0=1)
    c.capsule((25.5, 62.2), (20.5, 63.2), 1.3, 1.0, z0=1)
    c.chain([(36.5, 42.5 + cr * 0.4, 1), (40.0, 51.0 + cr * 0.3, 3), (40.5, 61.0, 2.5)], 2.0, 1.4)
    c.ellipse((40.0, 51.0 + cr * 0.3), 2.3, 2.0, z0=3.5)
    c.capsule((40.5, 61.5), (45.5, 62.8), 1.3, 1.0, z0=3)
    # pelvis cocked (higher on the weight side)
    c.ellipse((32, 42.5 + cr * 0.4), 7.4, 3.5, z0=0, flat=1.4)
    # spine curves from the pelvis to a chest pushed by the lean
    chx, chy = 32 + lean, 21 + cr
    c.chain([(32, 41, -2), (32 + lean * 0.4 + 0.8, 31, -2.5), (chx, chy, -2)], 1.8, 1.5)
    for i in range(6):
        y0 = chy + 1.5 + i * 2.9
        wdt = 9.4 - i * 0.55 - (1.8 if i == 5 else 0)
        cxr = chx - lean * i / 9.0
        for s in (-1, 1):
            pts = [(cxr + s * (1.5 + wdt * np.sin(t * np.pi * 0.62)), y0 + 3.2 * t ** 1.6 + s * 0.25 * i * 0.3,
                    2.2 * np.cos(t * np.pi * 0.9) - 0.5) for t in np.linspace(0, 1, 9)]
            for a, b in zip(pts[:-1], pts[1:]):
                c.capsule(a[:2], b[:2], 1.05, z0=a[2], z1=b[2], mat=6)
    c.capsule((chx, chy + 0.5), (chx - lean * 0.3, chy + 11), 1.25, 1.0, z0=2.2)
    shL, shR = (chx - 10.5, chy + 2.0), (chx + 10.5, chy + 1.0)       # right shoulder dropped (weight side)
    c.capsule((chx, chy), shL, 1.3, z0=1.5)
    c.capsule((chx, chy), shR, 1.3, z0=1.5)
    # sword arm (viewer's left)
    hand = p["hand"]
    el = ik(shL, hand, 9.5, 8.5, 1 if hand[1] > shL[1] else -1)
    c.ellipse(shL, 2.7, 2.5, z0=1)
    c.capsule(shL, el, 1.8, 1.5, z0=1, z1=2)
    c.ellipse(el, 1.9, 1.9, z0=2)
    c.capsule(el, hand, 1.3, 1.1, z0=2.5, z1=4)
    c.ellipse(hand, 2.2, 2.0, z0=4.5)
    a = np.radians(p["sword"])
    d, n = (np.cos(a), np.sin(a)), (-np.sin(a), np.cos(a))
    tip = (hand[0] + d[0] * 21, hand[1] + d[1] * 21)
    c.capsule((hand[0] + d[0] * 1.5, hand[1] + d[1] * 1.5), tip, 1.9, 1.0, z0=5, mat=2)
    c.capsule((hand[0] + n[0] * 3.5 + d[0], hand[1] + n[1] * 3.5 + d[1]),
              (hand[0] - n[0] * 3.5 + d[0], hand[1] - n[1] * 3.5 + d[1]), 0.9, z0=5.5, mat=2)
    c.capsule(hand, (hand[0] - d[0] * 3, hand[1] - d[1] * 3), 0.9, z0=5, mat=2)
    # free arm (viewer's right) hanging forward, clawed
    fh = p["free"]
    el2 = ik(shR, fh, 9.0, 8.5, -1)
    c.ellipse(shR, 2.7, 2.5, z0=1)
    c.capsule(shR, el2, 1.8, 1.5, z0=1, z1=2)
    c.ellipse(el2, 1.9, 1.9, z0=2)
    c.capsule(el2, fh, 1.3, 1.1, z0=3, z1=7)
    c.ellipse(fh, 2.0, 1.7, z0=7)
    for fx, fy in ((-2.0, 4.5), (-0.5, 5.2), (1.0, 4.8), (2.2, 3.8)):
        c.capsule((fh[0] + fx * 0.5, fh[1] + 0.7), (fh[0] + fx, fh[1] + fy), 0.7, z0=7)
    # rag hanging from the pelvis, longer on one side
    c.pid += 1
    cloth = np.zeros((64, 64), bool)
    for x in range(25, 40):
        hang = 49 + 4 * np.sin(x * 1.3) + 2 * rng.random() + (x - 25) * 0.25 + cr * 0.4
        if x in (28, 35):
            hang -= 5
        cloth[int(44 + cr * 0.4):int(hang), x] = True
    cn = P.fbm(rng, 64, 64, ((6, 2, .6), (2, 1, .4)))
    ncl = np.stack([(cn - 0.5) * 1.5, np.zeros((64, 64)) + 0.2, np.ones((64, 64))], -1)
    ncl /= np.linalg.norm(ncl, axis=-1, keepdims=True)
    c.put(cloth, np.full((64, 64), 3.5), ncl, 3)
    # skull, tilted, jaw hanging
    sx, sy = chx + tilt, 9.5 + cr
    c.ellipse((sx, sy), 6.3, 6.7, z0=3)
    c.ellipse((sx + tilt * 0.3, sy + 6.5 + p["jaw"]), 4.2, 2.5, z0=3.5, flat=1.3)
    for s in (-1, 1):
        c.ellipse((sx + s * 4.2, sy + 3.1), 2.2, 1.6, z0=4.5)
    sock = np.zeros((64, 64), bool)
    for s in (-1, 1):
        sock |= ((c.xx - (sx + s * 2.6)) / 1.9) ** 2 + ((c.yy - (sy + 1.1)) / 1.7) ** 2 <= 1
    sock |= (np.abs(c.xx - sx) <= 0.9) & (c.yy > sy + 3) & (c.yy < sy + 5.1)
    c.MAT[sock & (c.MAT == 1)] = 4
    I = c.light(rng, grime=0.14, flash=p["flash"])
    B = P.bayer(64, 64)
    out = np.zeros((64, 64), int)
    out[c.MAT == 1] = 1 + quant(I, 7, B)[c.MAT == 1]                  # 1-7 bone
    out[c.MAT == 6] = 1 + quant(I * 0.85, 7, B)[c.MAT == 6]           # ribs: a little darker
    out[c.MAT == 2] = 8 + quant(I, 2, B)[c.MAT == 2]                  # 8-9 rusty iron
    out[c.MAT == 3] = 10 + quant(I * 0.9, 2, B)[c.MAT == 3]           # 10-11 rag
    out[c.MAT == 4] = 1
    for y in range(64):
        for x in range(64):
            if c.MAT[y, x] == 2 and rng.random() < 0.15:
                out[y, x] = 8
    for x in range(int(sx) - 2, int(sx) + 3):                         # teeth
        y = int(sy + 5.5 + p["jaw"] * 0.5)
        if c.MAT[y, x] == 1:
            out[y, x] = 6 if x % 2 else 2
    out[c.rim() & (c.MAT != 4)] = 12                                  # warm torch rim
    out[sock & (c.MAT == 4)] = 13                                     # 13-15 accent: glowing cyan sockets
    for s_ in (-1, 1):
        ex, ey = int(round(sx + s_ * 2.6 - 0.5)), int(sy + 1.3)
        out[ey, ex] = 15
        out[ey, ex + (1 if s_ > 0 else -1)] = 14
        out[ey - 1, ex] = 14
    return out


SK_COLS = ([(0, 0, 0)] + P.ramp((26, 26, 20), (246, 238, 200), 7, (126, 122, 96)) +
           [(58, 44, 30), (156, 148, 128)] + [(40, 42, 26), (92, 94, 60)] + [(184, 116, 58)] +
           [(0, 120, 150), (40, 220, 255), (210, 255, 255)])


# ---- giant sewer rat, three-quarter view facing left: low pointed head, arched back, naked tail, glowing eyes
RAT_POSES = [
    dict(head=(0.0, 0.0), mouth=0, sx=1.0, claw=0.0, flash=0),
    dict(head=(0.0, 0.8), mouth=0, sx=1.02, claw=0.0, flash=0),
    dict(head=(2.0, -8.0), mouth=2, sx=0.94, claw=-3.0, flash=0),
    dict(head=(-4.0, 2.0), mouth=3, sx=1.06, claw=2.0, flash=0),
    dict(head=(4.0, -3.0), mouth=1, sx=0.98, claw=-1.0, flash=0.45),
]


def rat_frame(rng, p):
    c = Clay()
    (hx, hy), sx = p["head"], p["sx"]
    fur = P.fbm(rng, 64, 64, ((3, 2, .6), (2, 1, .4))) - 0.5
    t = np.linspace(0, 1, 12)                                            # tail, behind, curling up on the right
    c.chain([(54 + 8 * np.sin(u * 2.4), 54 - 26 * u, -6 + 2 * u) for u in t], 2.2, 0.6, mat=2)
    c.ellipse((47, 49), 12 * sx, 11, z0=-1, bump=fur * 0.5)               # haunch
    c.ellipse((38, 44 + hy * 0.15), 18 * sx, 12, z0=0, bump=fur * 0.6)    # arched back
    c.ellipse((28 + hx * 0.3, 48 + hy * 0.3), 11, 9.5, z0=4, bump=fur * 0.5)    # shoulders
    for k in range(9):                                                   # ragged ridge of fur on the spine
        x = 26 + k * 3.2
        y = 44 - 11.5 * np.sin(np.pi * (k + 1) / 10.0) + hy * 0.1
        c.capsule((x, y + 2), (x - 1.5, y - 1.5 - (k % 2)), 1.3, 0.5, z0=1)
    c.capsule((50, 55), (53, 61.5), 3.2, 2.2, z0=3)                       # hind leg and foot
    c.capsule((53, 62), (47, 63.2), 1.8, 1.4, z0=4, mat=2)
    for i, (x0, x1) in enumerate(((22, 18), (31, 29))):                   # front legs, claws toward the viewer
        cy = 62 + p["claw"] * (0.5 if i == 0 else 0.2)
        c.capsule((x0 + hx * 0.3, 51 + hy * 0.3), (x1, cy - 1.5), 2.6, 1.8, z0=8 + i, mat=1)
        c.ellipse((x1, cy - 0.5), 2.6, 1.6, z0=10 + i, mat=2)
        for k in (-1.6, 0, 1.6):
            c.capsule((x1 + k, cy), (x1 + k * 1.3 - 1.0, cy + 1.6), 0.55, z0=12, mat=3)
    hx0, hy0 = 18 + hx, 46 + hy                                          # head and long snout pointing down-left
    for k, (ex, ey) in enumerate(((22, 38), (27, 39.5))):
        c.ellipse((ex + hx, ey + hy), 3.2, 3.8, z0=5 - k, mat=2)
        c.ellipse((ex + hx - 0.3, ey + hy + 0.6), 1.8, 2.3, z0=6 - k, mat=5)
    c.ellipse((hx0 + 2, hy0), 8.0, 7.0, z0=8, bump=fur * 0.3)
    tip = (hx0 - 10, hy0 + 7 + p["mouth"] * 0.4)
    c.capsule((hx0, hy0 + 1.5), tip, 4.8, 1.9, z0=11, z1=14)
    c.ellipse((tip[0] - 0.5, tip[1]), 1.5, 1.2, z0=17, mat=2)
    I = c.light(rng, grime=0.14, flash=p["flash"])
    I = I + 0.14 * fur * (c.MAT == 1)
    B = P.bayer(64, 64)
    out = np.zeros((64, 64), int)
    out[c.MAT == 1] = 1 + quant(I, 8, B)[c.MAT == 1]                  # 1-8 fur
    out[c.MAT == 2] = 9 + quant(I, 2, B)[c.MAT == 2]                  # 9-10 naked skin
    out[c.MAT == 3] = 11
    out[c.MAT == 5] = 9
    mx, my = int(tip[0]) + 1, int(tip[1]) + 1                         # mouth and yellow incisors
    for j in range(p["mouth"] + 1):
        for i in range(0, 3 + p["mouth"]):
            if c.MAT[my + j, mx + i] > 0 or j == 0:
                out[my + j, mx + i] = 1
    out[my, mx] = out[my, mx + 1] = 11
    if p["mouth"] > 1:
        out[my + p["mouth"], mx + 1] = 11
    out[c.rim()] = 12                                                 # warm torch rim
    for k, (ex, ey) in enumerate(((hx0 - 2, hy0 - 2), (hx0 + 4, hy0 - 2.5))):
        ex, ey = int(ex), int(ey)                                     # 13-15 accent: red eyes
        out[ey, ex] = 15
        out[ey, ex + 1] = 14 if k == 0 else 13
        out[ey + 1, ex] = 13
        if k == 0:
            out[ey - 1, ex] = 13
    return out


RAT_COLS = ([(0, 0, 0)] + P.ramp((20, 20, 16), (214, 200, 162), 8, (102, 94, 72)) +
            [(78, 46, 40), (176, 122, 104)] + [(232, 210, 128)] + [(184, 116, 58)] +
            [(150, 10, 10), (255, 40, 30), (255, 210, 170)])


# ---- slime: a heaving mound of black-green ooze with a half-dissolved skull and ribs sunk inside
SL_POSES = [
    dict(sx=1.0, sy=1.0, lean=0.0, pod=0.0, flash=0),
    dict(sx=1.04, sy=0.95, lean=0.5, pod=0.0, flash=0),
    dict(sx=0.88, sy=1.22, lean=-1.0, pod=0.3, flash=0),
    dict(sx=1.14, sy=0.86, lean=1.5, pod=1.0, flash=0),
    dict(sx=1.1, sy=0.8, lean=-2.0, pod=0.2, flash=0.45),
]


def slime_frame(rng, p):
    c = Clay()
    sx, sy, ln = p["sx"], p["sy"], p["lean"]
    lumpy = P.fbm(np.random.default_rng(7), 64, 64, ((8, 8, .6), (4, 4, .4))) - 0.5
    base = 62
    c.ellipse((32, base - 1), 29 * sx, 4.5, z0=-4, flat=0.4, bump=lumpy * 0.6)        # spreading puddle
    c.ellipse((32 + ln, base - 12 * sy), 23 * sx, 13 * sy, z0=0, bump=lumpy * 0.7)
    c.ellipse((29 + ln * 2, base - 25 * sy), 14 * sx, 12 * sy, z0=2, bump=lumpy * 0.8)
    c.ellipse((44 + ln, base - 17 * sy), 9 * sx, 8 * sy, z0=4, bump=lumpy)
    c.ellipse((18 + ln, base - 14 * sy), 8 * sx, 7 * sy, z0=3, bump=lumpy)
    if p["pod"]:
        k = p["pod"]
        c.capsule((40 + ln, base - 16 * sy), (50 + 6 * k, base - 8 - 10 * k), 5, 3, z0=8, z1=12)
        c.capsule((22 + ln, base - 16 * sy), (12 - 5 * k, base - 6 - 8 * k), 4.5, 2.5, z0=8, z1=11)
    for bx, by, br in ((22, 52, 1.6), (40, 48, 1.2), (34, 58, 1.0), (27, 42, 1.0)):    # surface bubbles
        c.ellipse((bx * sx + 32 * (1 - sx) + ln, base - (base - by) * sy), br + 0.6, br + 0.6, z0=14)
    on = c.MAT > 0
    I = c.light(rng, grime=0.08, flash=p["flash"])
    H = np.array([-0.3, -0.5, 0.81])
    H /= np.linalg.norm(H)
    spec = ((c.N * H).sum(-1) > 0.97) & on
    B = P.bayer(64, 64)
    out = np.zeros((64, 64), int)
    out[on] = 1 + quant(I * 0.85, 8, B)[on]                              # 1-8 ooze
    skx, sky = 30 + ln * 1.5, base - 22 * sy                             # sunk bones, seen through the ooze
    yy, xx = c.yy, c.xx
    skull = (((xx - skx) / 6.0) ** 2 + ((yy - sky) / 5.6) ** 2 <= 1) |             ((np.abs(xx - skx) <= 3.2) & (yy > sky) & (yy < sky + 7))
    socket = (((xx - skx + 2.2) / 1.6) ** 2 + ((yy - sky - 1) / 1.5) ** 2 <= 1) |              (((xx - skx - 2.2) / 1.6) ** 2 + ((yy - sky - 1) / 1.5) ** 2 <= 1)
    ribs = np.zeros((64, 64), bool)
    fx0, fy0, fx1, fy1 = 36 + ln, base - 6 * sy, 47 + ln, base - 15 * sy
    t = np.clip(((xx - fx0) * (fx1 - fx0) + (yy - fy0) * (fy1 - fy0)) / ((fx1 - fx0) ** 2 + (fy1 - fy0) ** 2), 0, 1)
    ribs |= np.hypot(xx - fx0 - t * (fx1 - fx0), yy - fy0 - t * (fy1 - fy0)) < 1.1
    for ex, ey in ((fx0, fy0), (fx1, fy1)):
        ribs |= np.hypot(xx - ex, yy - ey) < 1.8
    lit = 9 + np.clip((I * 2.6).astype(int), 0, 2)
    m = (skull | ribs) & on
    out[m] = lit[m]
    out[c.rim()] = 12                                                     # warm torch rim
    out[socket & on & skull] = 13                                         # 13-15 accent: acid-green glow
    for s_ in (-1, 1):
        out[int(sky + 1), int(skx + s_ * 2.2)] = 14
    out[spec] = 15                                                        # wet highlights glow too
    return out


SL_COLS = ([(0, 0, 0)] + P.ramp((6, 12, 7), (168, 204, 96), 8, (34, 62, 30)) +
           [(70, 74, 46), (126, 128, 86), (186, 184, 132)] + [(184, 116, 58)] +
           [(60, 170, 20), (150, 255, 40), (236, 255, 190)])


def build_monsters():
    for name, fn, poses, cols in (("mon_skeleton", skeleton_frame, SK_POSES, SK_COLS),
                                  ("mon_rat", rat_frame, RAT_POSES, RAT_COLS),
                                  ("mon_slime", slime_frame, SL_POSES, SL_COLS)):
        frames = [fn(np.random.default_rng(5 + k), p) for k, p in enumerate(poses)]
        for f in frames:                 # feet on the last row
            rows = np.nonzero(f.any(1))[0]
            sh = 63 - rows.max()
            if sh:
                f[:] = np.roll(f, sh, 0)
        assert len(cols) == 16, (name, len(cols))
        save_idx(os.path.join(OUT, name + ".png"), frames_to_strip(frames), cols, True)
        print(name, "frames", len(frames))


# ------------------------------------------------------------------------------------------------ props
def grade(rgb):
    r, g, b = rgb
    L = 0.3 * r + 0.59 * g + 0.11 * b
    mix = 0.45
    c = [L + (v - L) * mix for v in (r, g, b)]
    c = [c[0] * 0.86, c[1] * 1.0, c[2] * 0.86]
    return tuple(int(np.clip(255 * (v / 255) ** 1.12 * 0.92, 0, 255)) for v in c)


def build_props():
    for f in sorted(os.listdir(LEG)):
        if not f.startswith("prop_"):
            continue
        im = Image.open(os.path.join(LEG, f))
        idx = np.array(im)
        pal = im.getpalette()[:48]
        cols = [tuple(pal[i * 3:i * 3 + 3]) for i in range(16)]
        lum = np.array([0.3 * c[0] + 0.59 * c[1] + 0.11 * c[2] for c in cols])
        new = [cols[0]] + [grade(c) for c in cols[1:]]
        order = [i for i in np.argsort(lum) if i != 0]          # dark -> light
        dark_i = order[0]
        out = idx.copy()
        h = idx.shape[0]
        for k in range(0, h, 32):
            fr = out[k:k + 32]
            src = idx[k:k + 32]
            opq = src != 0
            # outline pixels -> a dark tone of their own neighbourhood (shading, not a line)
            ys, xs = np.nonzero(src == dark_i)
            for y, x in zip(ys, xs):
                nb = [src[yy, xx] for yy, xx in ((y - 1, x), (y + 1, x), (y, x - 1), (y, x + 1))
                      if 0 <= yy < 32 and 0 <= xx < 32 and src[yy, xx] not in (0, dark_i)]
                if not nb:
                    continue
                target = lum[nb].mean() * 0.45
                cand = [i for i in order if lum[i] <= max(target, lum[order[1]])]
                fr[y, x] = cand[-1] if cand else order[1]
            # light the top / left silhouette edge one step, cool rim on the right edge
            up = np.zeros_like(opq)
            up[1:] = ~opq[:-1]
            up[0] = True
            left = np.zeros_like(opq)
            left[:, 1:] = ~opq[:, :-1]
            right = np.zeros_like(opq)
            right[:, :-1] = ~opq[:, 1:]
            rank = {i: r for r, i in enumerate(order)}
            for y, x in zip(*np.nonzero(opq & (up | left))):
                r = rank.get(fr[y, x])
                if r is not None:
                    fr[y, x] = order[min(len(order) - 1, r + 1)]
            rows = np.nonzero(opq.any(1))[0]
            if len(rows):
                hi = np.zeros_like(opq)
                hi[:rows.min() + int(0.55 * (rows.max() - rows.min())) + 1] = True
                fr[opq & right & ~up & hi] = dark_i
        new[dark_i] = (128, 164, 132)            # the old outline slot becomes the rim colour
        out[(idx != 0) & (out == dark_i)] = dark_i
        save_idx(os.path.join(OUT, f[:-4] + ".png"), out, new + [(0, 0, 0)] * (16 - len(new)), True)
    print("props re-graded")


if __name__ == "__main__":
    os.makedirs(OUT, exist_ok=True)
    build_textures()
    build_monsters()
    build_props()
