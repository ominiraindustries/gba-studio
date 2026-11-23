"""DELVE magic, treasure and equipment sprites (G-40 art; design MAGIA_Y_TESOROS.md, "Arte necesario").

  python -I magic_art.py           seeds only: _seed/out/<name>.png + contact sheet + pause mock-up
  python -I magic_art.py --apply   seeds + creates art/<name>.aseprite ONLY IF MISSING + exports every one to
                                   graphics/<name>.bmp/.json (from the .aseprite, so your edits win)
  python -I magic_art.py --export-only   just re-exports the existing .aseprite files

Style (art/LEEME.md): dark painted crawler, internal shading lit from the top-left (the player's torch), soft ordered
dither, no cartoon outline (the silhouette edge is a darker tone of the same material). Magic is cold blue, the torch
and gold are the only warm light, danger is red. Every sprite: 4bpp, 16 colours, index 0 transparent.
"""
import math
import os
import subprocess
import sys

import numpy as np
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
GFX = os.path.join(os.path.dirname(ART), "graphics")
OUT = os.path.join(HERE, "out")
ASE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
BAY = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0
HEX = "123456789ABCDEF"


def hx(s):
    return tuple(int(s[i:i + 2], 16) for i in (1, 3, 5))


def mkpal(hexes):
    assert len(hexes) == 15
    return [(255, 0, 255)] + [hx(h) for h in hexes]


# ------------------------------------------------------------------------------------------------ char grid
class G:
    """'.' transparent, lowercase = material (shaded), uppercase = that material's highlight, 1-9/A-F = fixed index."""

    def __init__(self, w, h, art=None, mirror=False):
        self.w, self.h = w, h
        self.a = np.full((h, w), ".", dtype="<U1")
        if art:
            self.ascii(0, 0, art, mirror)

    def set(self, x, y, ch):
        x, y = int(round(x)), int(round(y))
        if 0 <= x < self.w and 0 <= y < self.h:
            self.a[y, x] = ch

    def rect(self, x0, y0, x1, y1, ch):
        x0, y0, x1, y1 = max(0, x0), max(0, y0), min(self.w - 1, x1), min(self.h - 1, y1)
        if x1 >= x0 and y1 >= y0:
            self.a[y0:y1 + 1, x0:x1 + 1] = ch

    def ascii(self, x0, y0, art, mirror=False):
        rows = [r.strip() for r in art.strip("\n").split("\n")]
        for j, r in enumerate(rows):
            if mirror:
                r = r + r[::-1]
            assert len(r) == (self.w if x0 == 0 else len(r)), (len(r), r)
            for i, ch in enumerate(r):
                if ch != ".":
                    self.set(x0 + i, y0 + j, ch)

    def mask(self, m, ch, only=None):
        sel = m if only is None else (m & np.isin(self.a, list(only)))
        self.a[sel] = ch


def grid(w, h):
    yy, xx = np.mgrid[0:h, 0:w].astype(float)
    return xx, yy


def dseg(xx, yy, ax, ay, bx, by):
    px, py, dx, dy = xx - ax, yy - ay, bx - ax, by - ay
    t = np.clip((px * dx + py * dy) / (dx * dx + dy * dy + 1e-9), 0, 1)
    return np.hypot(px - t * dx, py - t * dy), t


def inpoly(xx, yy, pts):
    inside = np.zeros(xx.shape, bool)
    n = len(pts)
    for i in range(n):
        (x1, y1), (x2, y2) = pts[i], pts[(i + 1) % n]
        cond = ((y1 > yy) != (y2 > yy)) & (xx < (x2 - x1) * (yy - y1) / (y2 - y1 + 1e-9) + x1)
        inside ^= cond
    return inside


def dist_in(reg, cap=3):
    d = np.zeros(reg.shape, int)
    cur = reg.copy()
    for k in range(1, cap + 1):
        p = np.pad(cur, 1)
        er = cur & p[:-2, 1:-1] & p[2:, 1:-1] & p[1:-1, :-2] & p[1:-1, 2:]
        d[cur & ~er] = k
        cur = er
    d[cur] = cap + 1
    return d


def shade(g, legend, seed=1, noise=0.07, glob=0.30, dith=0.55, bright=0.0, origin=None):
    """legend: char -> (ramp dark->light, highlight index, roundness, pattern)."""
    a = g.a
    h, w = a.shape
    out = np.zeros((h, w), int)
    low = np.char.lower(a)
    solid = a != "."
    fixed = np.isin(a, list(HEX))
    rng = np.random.default_rng(seed)
    N = rng.random((h, w)) - 0.5
    yy, xx = np.mgrid[0:h, 0:w]
    ys, xs = np.nonzero(solid)
    if not len(xs):
        return out
    cx, cy = (xs.mean(), ys.mean()) if origin is None else origin
    size = max(np.ptp(xs), np.ptp(ys), 1) + 1
    gl = -((xx - cx) + (yy - cy)) / size
    tp = np.pad(~solid, 1, constant_values=True)
    lit_edge = tp[:-2, 1:-1] | tp[1:-1, :-2]
    dark_edge = tp[2:, 1:-1] | tp[1:-1, 2:]
    for m, spec in legend.items():
        ramp, hi, rnd = spec[0], spec[1], spec[2]
        pat = spec[3] if len(spec) > 3 else 0.0
        reg = (low == m) & solid & ~fixed
        if not reg.any():
            continue
        hgt = np.minimum(dist_in(reg), 3) / 3.0
        p = np.pad(hgt, 1)
        nl = (p[1:-1, 2:] - p[1:-1, :-2]) + (p[2:, 1:-1] - p[:-2, 1:-1])
        v = 0.5 + bright + rnd * 0.55 * nl + glob * gl + 0.10 * lit_edge - 0.18 * dark_edge + noise * N
        if pat:
            v -= pat * (((xx % 2) == 0) & ((yy % 2) == 0)) - pat * 0.4 * (((xx + yy) % 2) == 1)
        n = len(ramp)
        k = np.clip(np.floor(v * n + (BAY[yy % 4, xx % 4] - 0.5) * dith), 0, n - 1).astype(int)
        vals = np.array(ramp)[k]
        out[reg] = vals[reg]
        his = reg & (a != low)
        out[his] = hi
    for ch in HEX:
        out[a == ch] = int(ch, 16)
    return out


def glow(field, ramp, lo=0.1, dith=0.45):
    h, w = field.shape
    yy, xx = np.mgrid[0:h, 0:w]
    n = len(ramp)
    f = (field - lo) / (1 - lo) * n + (BAY[yy % 4, xx % 4] - 0.5) * dith
    k = np.clip(np.floor(f), 0, n - 1).astype(int)
    out = np.array(ramp)[k]
    out[field < lo] = 0
    return out


def over(base, top):
    r = base.copy()
    r[top > 0] = top[top > 0]
    return r


# ================================================================================================ ITEMS (16x16)
ITEM_PAL = mkpal(["#0b0907", "#221c16", "#3a3934", "#6a6860", "#a8a596", "#5a4024", "#9a7444", "#d6b66e",
                  "#4e0e10", "#9c2220", "#dc5a3c", "#12285a", "#2c64b4", "#86c2f0", "#f2ead0"])
ITEM_LEG = {"i": ([2, 3, 4, 5], 15, 1.0), "g": ([2, 3, 4], 5, 0.5), "p": ([6, 7, 8], 15, 0.7),
            "r": ([9, 10, 11], 15, 1.0), "u": ([12, 13, 14], 15, 1.0), "o": ([10, 11, 8], 15, 1.0),
            "w": ([2, 6, 7], 8, 0.8), "y": ([6, 7, 8], 15, 1.0), "k": ([1, 2], 3, 0.3)}

POTION = """
................
......wWWw......
......wwww......
.....ggGggg.....
......gGgg......
......grrg......
....ggrrrrgg....
...grrrrrrrrg...
..grFrrrrrrrrg..
..grFrrrrrrrrg..
..grrrrrrrrrrg..
..grrrrrrrrrrg..
...grrrrrrrrg...
....ggrrrrgg....
......gggg......
................
"""
FLASK = """
................
.......F........
.......8B.......
......BpP.......
......ppp.......
......gGg.......
.....goooog.....
....gOooooog....
...gOooooooog...
...goooooooog...
...goooooooog...
...goooooooog...
....goooooog....
.....gggggg.....
................
................
"""
SCROLL = """
................
..pppppppppppp..
.pPPPPPPPPPPPPp.
.ppppppppppppp1.
..p6666666666...
..p7777777776...
..p7667676776...
..p7777777776...
..p7676667776...
..p7777777rr6...
..p77777rRrr6...
.pppppppprrppp..
.pPPPPPPPPPPPPp.
.ppppppppppppp1.
..pppppppppppp..
................
"""
KEY = """
................
................
..iiii..........
.iIIiii.........
.iI..ii.........
.ii..ii.........
.iiiiii.........
..iiiiii........
......iii.......
.......iii......
........iii.....
.........iiii...
........iii.ii..
.........i...i..
................
................
"""
BREAD = """
................
................
................
................
.....pppppp.....
...pPPPpppppp...
..pPPp6ppp6ppp..
.pPpp6ppp6ppp6p.
.ppp6ppp6ppp6pp.
.pppppppppppppp.
.pppppppppppppp.
..pppppppppppp..
...1pppppppp1...
....11111111....
................
................
"""
GOLD = """
................
................
................
................
.......yY.......
......yYYy......
.....yyyyyy.....
....yYy6yyy6....
...yYyyy6yyyy...
..yyy6yyyy6yyy..
..yYyyyy6yyyyy..
.yyyy6yyyyy6yyy.
.yyyyyyyyyyyyyy.
..1yyyyyyyyyy1..
...1111111111...
................
"""


def items():
    fr = []
    for art, seed in ((SCROLL, 1), (POTION, 2), (POTION.replace("r", "u"), 3), (BREAD, 4), (KEY, 5),
                      (FLASK, 6), (GOLD, 7)):
        fr.append(shade(G(16, 16, art), ITEM_LEG, seed))
    names = ["scroll", "healing_potion", "mana_potion", "bread", "iron_key", "fire_flask", "gold"]
    return fr, names


# ================================================================================================ EQUIPMENT (16x16)
EQ_PAL = mkpal(["#0a0908", "#1c1916", "#34363a", "#5c6064", "#8e9294", "#c8cac4", "#3a2414", "#5e3c22",
                "#8a6038", "#1a2244", "#304c86", "#5a7cc0", "#7a5a20", "#c49a42", "#3a3834"])
EQ_LEG = {"s": ([3, 4, 5, 6], 6, 1.0), "l": ([7, 8, 9], 9, 0.8), "v": ([10, 11, 12], 12, 0.9),
          "y": ([13, 14], 6, 0.8), "w": ([2, 7, 8], 9, 0.6), "m": ([2, 3, 4, 5], 6, 0.6, 0.22),
          "x": ([2, 15], 15, 0.3)}

LEATHER = """
................
................
...lll....lll...
..lllll..lllll..
.llllll22llllll.
lllllll22lllllll
..lllll99lllll..
..lllll22lllll..
..lllll99lllll..
..llllllllllll..
..wwwwwEEwwwww..
..llllllllllll..
..llllllllllll..
...llllllllll...
................
................
"""
ROBE = """
................
......vvvv......
.....vvvvvv.....
....vv1111vv....
...vvv1111vvv...
..vvvvv11vvvvv..
.vvvvvvyyvvvvvv.
.vvvvvvyyvvvvvv.
vvvvvvvyyvvvvvvv
vvv.vvvyyvvv.vvv
vv..vvvyyvvv..vv
....vvvyyvvv....
...vvvvyyvvvv...
...vvvvyyvvvv...
..yyyyyyyyyyyy..
................
"""
CHAIN = """
................
................
....mmm..mmm....
..mmmmm..mmmmm..
.mmmmmm22mmmmmm.
mmmmmmmmmmmmmmmm
mmmmmmmmmmmmmmmm
mmm.mmmmmmmm.mmm
....mmmmmmmm....
....mmmmmmmm....
....mmmmmmmm....
....mmmmmmmm....
....mmmmmmmm....
....ssssssss....
................
................
"""
PLATE = """
................
................
..sss......sss..
.sssss2..2sssss.
ssss2ssssss2ssss
ssEs2ssssss2sEss
.ss.2ssssss2.ss.
....2ssssss2....
....ssssssss....
....ssssssss....
....22222222....
....ssssssss....
....22222222....
.....ssssss.....
................
................
"""


def blade_weapon(kind, sil=False):
    g = G(16, 16)
    xx, yy = grid(16, 16)
    S = "x" if sil else "s"
    W = "x" if sil else "w"
    Y = "x" if sil else "y"
    if kind == "dagger":
        d, t = dseg(xx, yy, 7.5, 9.5, 11.5, 5.5)
        g.mask(d <= 1.7 * np.sin(np.pi * np.clip(0.25 + 0.75 * t, 0, 1)) + 0.2, S)
        g.mask(dseg(xx, yy, 5.5, 8.5, 7.5, 10.5)[0] <= 0.75, Y)
        g.mask(dseg(xx, yy, 4, 13, 6, 11)[0] <= 0.8, W)
        g.set(3, 14, Y)
    elif kind == "sword":
        d, t = dseg(xx, yy, 6.5, 9.5, 13.5, 2.5)
        g.mask(d <= 1.05 - 0.35 * t, S)
        g.mask(dseg(xx, yy, 3, 7, 8, 12)[0] <= 0.75, Y)
        g.mask(dseg(xx, yy, 2.5, 13.5, 5, 11)[0] <= 0.8, W)
        g.set(1, 14, Y)
        g.set(2, 14, Y)
        g.set(1, 13, Y)
    elif kind == "mace":
        g.mask(dseg(xx, yy, 2, 14, 9, 7)[0] <= 0.8, W)
        for k in range(3):
            g.set(3 + k, 13 - k, "2" if not sil else "x")
        g.mask(np.hypot(xx - 10.5, yy - 5.5) <= 3.4, S)
        for ang in range(0, 360, 45):
            r = math.radians(ang)
            g.set(10.5 + 4.4 * math.cos(r), 5.5 + 4.4 * math.sin(r), S)
            g.set(10.5 + 3.6 * math.cos(r), 5.5 + 3.6 * math.sin(r), S)
        if not sil:
            g.set(9, 4, "S")
    elif kind == "axe":
        g.mask(dseg(xx, yy, 2.5, 14, 11, 4.5)[0] <= 0.8, W)
        dx, dy = 1 / math.sqrt(2), -1 / math.sqrt(2)
        nx, ny = 1 / math.sqrt(2), 1 / math.sqrt(2)
        u = (xx - 9.8) * dx + (yy - 5.8) * dy
        v = (xx - 9.8) * nx + (yy - 5.8) * ny
        head = ((u / 2.9) ** 2 + (v / 6.0) ** 2 <= 1) & ~((np.abs(v) < 2.0) & (np.abs(u) > 1.0))
        g.mask(head, S)
        if not sil:
            g.mask(head & ((u / 2.9) ** 2 + (v / 6.0) ** 2 > 0.62) & (v < -2.5), "S")
    return g


def ring_shape(g, band, gem, thick=False, setting=None):
    xx, yy = grid(16, 16)
    e_out = ((xx - 7.5) / 5.6) ** 2 + ((yy - 9.8) / 4.3) ** 2 <= 1
    ri, rj = (2.6, 1.4) if thick else (3.6, 2.3)
    e_in = ((xx - 7.5) / ri) ** 2 + ((yy - 10.2) / rj) ** 2 <= 1
    g.mask(e_out & ~e_in, band)
    if gem:
        g.mask(((xx - 7.5) / 2.6) ** 2 + ((yy - 4.6) / 2.3) ** 2 <= 1, setting or band)
        g.mask(((xx - 7.5) / 1.7) ** 2 + ((yy - 4.4) / 1.5) ** 2 <= 1, gem)
        g.set(7, 3, gem.upper())


def equipment():
    fr, names = [], []
    for kind, nm in (("dagger", "dagger"), ("sword", "short_sword"), ("mace", "mace"), ("axe", "battle_axe")):
        fr.append(shade(blade_weapon(kind), EQ_LEG, len(fr) + 11))
        names.append(nm)
    for art, nm in ((LEATHER, "leather"), (ROBE, "mage_robe"), (CHAIN, "chain_mail"), (PLATE, "plate")):
        fr.append(shade(G(16, 16, art), EQ_LEG, len(fr) + 11))
        names.append(nm)
    # empty-slot silhouettes (flat dim grey: "vacio = silueta gris del tipo")
    fr.append(shade(blade_weapon("sword", sil=True), EQ_LEG, 31, glob=0.1))
    sil = G(16, 16, PLATE)
    sil.mask(sil.a != ".", "x")
    fr.append(shade(sil, EQ_LEG, 32, glob=0.1))
    g = G(16, 16)
    ring_shape(g, "x", "x")
    fr.append(shade(g, EQ_LEG, 33, glob=0.1))
    g = G(16, 16)
    amulet_shape(g, "x", "x", "disc")
    fr.append(shade(g, EQ_LEG, 34, glob=0.1))
    names += ["empty_weapon", "empty_armor", "empty_ring", "empty_amulet"]
    return fr, names


# ================================================================================================ JEWELS (16x16)
JW_PAL = mkpal(["#0a0908", "#3e2a10", "#7c5a1c", "#c09030", "#f0d888", "#36383e", "#7c8088", "#c4c8cc",
                "#1a3888", "#5aa0f0", "#6a1018", "#e04048", "#165a2a", "#62d070", "#fbfbf0"])
JW_LEG = {"g": ([2, 3, 4, 5], 15, 1.0), "s": ([6, 7, 8], 15, 1.0), "u": ([9, 10], 15, 0.7),
          "r": ([11, 12], 15, 0.7), "n": ([13, 14], 15, 0.7), "q": ([8, 15], 15, 0.5), "x": ([6, 7], 7, 0.3)}


def amulet_shape(g, chain, pend, kind, gem=None):
    xx, yy = grid(16, 16)
    for side in (-1, 1):                                          # chain: links every other pixel
        d, t = dseg(xx, yy, 7.5 + side * 6, 0.5, 7.5 + side * 2.2, 7.5)
        sel = (d <= 0.55) & (((np.round(t * 8)).astype(int) % 2) == 0)
        g.mask(sel, chain)
    if kind == "disc":
        g.mask(np.hypot(xx - 7.5, yy - 11) <= 3.9, pend)
        if gem:
            g.mask(np.hypot(xx - 7.5, yy - 11) <= 2.0, gem)
            g.set(7, 10, gem.upper())
    elif kind == "drop":
        g.mask(np.hypot(xx - 7.5, yy - 11.5) <= 3.4, pend)
        g.mask(dseg(xx, yy, 7.5, 7.5, 7.5, 11)[0] <= 1.6, pend)
        g.mask(np.hypot(xx - 7.5, yy - 11.5) <= 2.2, gem)
        g.mask(dseg(xx, yy, 7.5, 9.2, 7.5, 11)[0] <= 0.9, gem)
        g.set(6, 10, gem.upper())
    elif kind == "coin":
        g.mask(np.hypot(xx - 7.5, yy - 11) <= 4.2, pend)
        ring = np.abs(np.hypot(xx - 7.5, yy - 11) - 2.4) <= 0.5
        g.mask(ring, "2")
        g.set(7, 10, "5")
    elif kind == "eye":
        g.mask(np.hypot(xx - 7.5, yy - 11) <= 4.0, pend)
        lens = (np.hypot(xx - 7.5, yy - 11 - 2.2) <= 3.6) & (np.hypot(xx - 7.5, yy - 11 + 2.2) <= 3.6)
        g.mask(lens, "q")
        g.mask(np.hypot(xx - 7.5, yy - 11) <= 1.4, gem)
        g.set(7, 11, "1")
    elif kind == "feather":
        d, t = dseg(xx, yy, 5, 14.5, 11, 7.5)
        g.mask(d <= 2.3 * np.sin(np.pi * np.clip(t * 0.9 + 0.1, 0, 1)), pend)
        g.mask(d <= 0.45, "s")
        g.set(10, 8, "Q")


def jewels():
    fr, names = [], []
    specs = [("ring_of_mana", "g", "u", False), ("ring_of_regeneration", "g", "n", False),
             ("ring_of_protection", "s", None, True), ("ring_of_swiftness", "s", "n", False),
             ("ring_of_light", "g", "q", False)]
    for nm, band, gem, thick in specs:
        g = G(16, 16)
        ring_shape(g, band, gem, thick)
        if nm == "ring_of_protection":                            # broad signet with a raised boss
            xx, yy = grid(16, 16)
            g.mask(((xx - 7.5) / 3.0) ** 2 + ((yy - 5.6) / 2.0) ** 2 <= 1, "s")
            g.set(7, 5, "S")
        if nm == "ring_of_swiftness":                             # twisted band: gold strands through silver
            m = (g.a == "s") & ((np.add.outer(np.arange(16), np.arange(16)) % 3) == 0)
            g.a[m] = "g"
        if nm == "ring_of_light":
            for dx, dy in ((-4, -1), (4, -1), (0, -5), (-3, -4), (3, -4)):
                g.set(7.5 + dx, 4.4 + dy, "F" if abs(dx) + abs(dy) > 4 else "4")
        fr.append(shade(g, JW_LEG, len(fr) + 41))
        names.append(nm)
    for nm, chain, pend, kind, gem in (("amulet_of_life", "g", "g", "drop", "r"),
                                       ("amulet_of_fortune", "g", "g", "coin", None),
                                       ("amulet_of_warding", "s", "s", "eye", "u"),
                                       ("amulet_of_second_wind", "s", "q", "feather", None)):
        g = G(16, 16)
        amulet_shape(g, chain, pend, kind, gem)
        fr.append(shade(g, JW_LEG, len(fr) + 41))
        names.append(nm)
    return fr, names


# ================================================================================================ SPELLS (glow)
SP_PAL = mkpal(["#06080e", "#12203c", "#22467e", "#3e78c4", "#7ab4f0", "#c6e6ff", "#fbfbf2", "#5a2a0c",
                "#b4561a", "#eca23a", "#fadc96", "#4a0c10", "#a8222a", "#46c6c8", "#3e3a36"])
BLUE = [2, 3, 4, 5, 6, 7]
FIRE = [8, 9, 10, 11, 7]
GOLDR = [8, 10, 11, 7]
CYAN = [2, 3, 14, 6, 7]
SP_LEG = {"r": ([12, 13], 7, 1.0), "s": ([1, 15], 15, 0.5)}


def f_clip(x):
    return np.clip(x, 0, 1)


def sp_missile(n=16, head=(11.5, 3.5), tail=(2.5, 12.5), ramp=BLUE):
    xx, yy = grid(n, n)
    d, t = dseg(xx, yy, tail[0], tail[1], head[0], head[1])
    I = f_clip(1 - d / (0.5 + 1.6 * t)) * (0.3 + 0.7 * t)
    I = np.maximum(I, f_clip(1 - np.hypot(xx - head[0], yy - head[1]) / 3.6) ** 0.8)
    for a in (0, 90):
        r = math.radians(a)
        dd, tt = dseg(xx, yy, head[0] - 3.5 * math.cos(r), head[1] - 3.5 * math.sin(r),
                      head[0] + 3.5 * math.cos(r), head[1] + 3.5 * math.sin(r))
        I = np.maximum(I, f_clip(1 - dd / 0.7) * (1 - np.abs(tt - 0.5) * 1.6))
    return glow(I, ramp, 0.12)


def rays(xx, yy, cx, cy, n_rays, l_long, l_short, w=0.8, start=0):
    I = np.zeros(xx.shape)
    for k in range(n_rays):
        r = math.radians(start + k * 360 / n_rays)
        L = l_long if k % 2 == 0 else l_short
        d, t = dseg(xx, yy, cx, cy, cx + L * math.cos(r), cy + L * math.sin(r))
        I = np.maximum(I, f_clip(1 - d / w) * (1 - 0.75 * t))
    return I


def sp_light():
    xx, yy = grid(16, 16)
    I = f_clip(1 - np.hypot(xx - 7.5, yy - 7.5) / 4.4) ** 0.7
    I = np.maximum(I, rays(xx, yy, 7.5, 7.5, 8, 7.4, 5.4, 0.85))
    return glow(I, GOLDR, 0.16)


def sp_heal():
    g = G(16, 16)
    xx, yy = grid(16, 16)
    g.mask(np.hypot(xx - 7.5, yy - 10) <= 3.7, "r")
    d, t = dseg(xx, yy, 7.5, 3, 7.5, 9)
    g.mask(d <= 0.4 + 2.9 * t ** 1.4, "r")
    g.set(6, 8, "R")
    g.set(6, 9, "R")
    out = shade(g, SP_LEG, 51)
    for (mx, my, big) in ((2, 4, 1), (13, 3, 1), (13, 10, 0), (2, 12, 0)):
        top = np.zeros((16, 16), int)
        top[my, mx] = 7 if big else 11
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            if big and 0 <= mx + dx < 16 and 0 <= my + dy < 16:
                top[my + dy, mx + dx] = 10
        out = over(out, top)
    return out


def sp_frost(n=16, c=7.5, L=6.6, ramp=CYAN):
    xx, yy = grid(n, n)
    I = f_clip(1 - np.hypot(xx - c, yy - c) / 2.2)
    for a in (90, 30, 150):
        r = math.radians(a)
        for s in (1, -1):
            ex, ey = c + s * L * math.cos(r), c + s * L * math.sin(r)
            d, t = dseg(xx, yy, c, c, ex, ey)
            I = np.maximum(I, f_clip(1 - d / 0.85) * (1 - 0.45 * t))
            bx, by = c + s * L * 0.6 * math.cos(r), c + s * L * 0.6 * math.sin(r)
            for b in (40, -40):
                rb = r + math.radians(b) if s > 0 else r + math.pi + math.radians(b)
                d2, t2 = dseg(xx, yy, bx, by, bx + 2.2 * math.cos(rb), by + 2.2 * math.sin(rb))
                I = np.maximum(I, f_clip(1 - d2 / 0.7) * 0.55)
    return glow(I, ramp, 0.14)


def sp_fireball():
    xx, yy = grid(16, 16)
    hx_, hy_ = 9.5, 5.5
    d, t = dseg(xx, yy, hx_, hy_, 2.0, 13.0)
    I = f_clip(1 - d / (0.6 + 2.6 * (1 - t) ** 1.2)) * (0.25 + 0.6 * (1 - t))
    I = np.maximum(I, f_clip(1 - np.hypot(xx - hx_, yy - hy_) / 4.6) ** 0.7)
    return glow(I, FIRE, 0.13)


def sp_shield():
    xx, yy = grid(16, 16)
    pts = [(2.5, 2.0), (12.5, 2.0), (12.5, 7.5), (7.5, 14.0), (2.5, 7.5)]
    ins = inpoly(xx, yy, pts)
    d = np.min([dseg(xx, yy, *pts[i], *pts[(i + 1) % 5])[0] for i in range(5)], axis=0)
    I = np.where(ins, 0.30 + 0.25 * f_clip(1 - np.hypot(xx - 6, yy - 5) / 7), 0)
    I = np.maximum(I, f_clip(1 - d / 1.15) * (ins | (d < 0.7)))
    I = np.maximum(I, f_clip(1 - dseg(xx, yy, 7.5, 4.5, 7.5, 10.5)[0] / 0.7) * 0.75)
    return glow(I, BLUE, 0.12)


def sp_reveal():
    xx, yy = grid(16, 16)
    R, c = 7.8, 4.3
    d1, d2 = np.hypot(xx - 7.5, yy - 7.5 - c) - R, np.hypot(xx - 7.5, yy - 7.5 + c) - R
    sd = np.maximum(d1, d2)
    I = np.where(sd < 0, 0.22, 0) + f_clip(1 - np.abs(sd) / 1.0) * 0.9
    r = np.hypot(xx - 7.5, yy - 7.5)
    I = np.maximum(I, np.where(r <= 2.8, 0.75 - 0.15 * (r / 2.8), 0))
    out = glow(I, BLUE, 0.12)
    out[(r <= 1.2)] = 1
    out[7, 6] = 7
    return out


def sp_blink():
    xx, yy = grid(16, 16)
    cx, cy, R = 7.5, 11.5, 5.6
    ang = np.degrees(np.arctan2(-(yy - cy), xx - cx))
    r = np.hypot(xx - cx, yy - cy)
    on = (ang > 8) & (ang < 172)
    fade = f_clip((172 - ang) / 164)
    I = np.where(on, f_clip(1 - np.abs(r - R) / 0.9) * (0.35 + 0.65 * (1 - fade)), 0)
    tipx, tipy = cx + R * math.cos(math.radians(8)), cy - R * math.sin(math.radians(8))
    for ex, ey in ((tipx - 2.6, tipy - 1.6), (tipx + 1.4, tipy - 2.4)):
        I = np.maximum(I, f_clip(1 - dseg(xx, yy, tipx, tipy, ex, ey)[0] / 0.75))
    I = np.maximum(I, f_clip(1 - np.hypot(xx - (cx - R), yy - cy) / 1.6) * 0.45)
    return glow(I, BLUE, 0.13)


def sp_haste():
    xx, yy = grid(16, 16)
    I = np.zeros((16, 16))
    for k in range(3):
        x0 = 2.0 + 4.0 * k
        a = dseg(xx, yy, x0, 3.5, x0 + 3.6, 7.5)[0]
        b = dseg(xx, yy, x0 + 3.6, 7.5, x0, 11.5)[0]
        I = np.maximum(I, f_clip(1 - np.minimum(a, b) / 0.8) * (0.4 + 0.3 * k))
    return glow(I, BLUE, 0.12)


def sp_unlock():
    xx, yy = grid(16, 16)
    r = np.hypot(xx - 7.5, yy - 7.5)
    I = f_clip(1 - r / 7.6) ** 1.3 * 0.85
    I = np.maximum(I, rays(xx, yy, 7.5, 7.5, 4, 7.5, 7.5, 0.7, 45) * 0.9)
    out = glow(I, GOLDR, 0.2)
    hole = (np.hypot(xx - 7.5, yy - 6) <= 2.2) | inpoly(xx, yy, [(6.2, 7), (8.8, 7), (9.8, 12.5), (5.2, 12.5)])
    p = np.pad(hole, 1)
    rim = (p[:-2, 1:-1] | p[2:, 1:-1] | p[1:-1, :-2] | p[1:-1, 2:]) & ~hole
    out[rim] = 15
    out[hole] = 1
    return out


def spells():
    fr = [sp_missile(), sp_light(), sp_heal(), sp_frost(), sp_fireball(), sp_shield(), sp_reveal(), sp_blink(),
          sp_haste(), sp_unlock()]
    names = ["magic_missile", "light", "heal", "frost", "fireball", "shield", "reveal", "blink", "haste", "unlock"]
    return fr, names


# ------------------------------------------------------------------------------------------------ status 8x8
ST_LIGHT = """
...A....
.A.B.A..
..BBB...
ABB7BBA.
..BBB...
.A.B.A..
...A....
........
"""
ST_SHIELD = """
.555554.
.566643.
.566643.
.566643.
..5643..
..5443..
...43...
........
"""
ST_HASTE = """
........
.4..6...
..4..6..
...4..7.
..4..6..
.4..6...
........
........
"""
ST_WIND = """
......B.
.....BA.
....BA9.
...BA9..
..BA9...
.7A9....
7.......
........
"""
ST_BELT = """
.AAA....
9A.BA...
9AB.A9..
.AB.A9..
.AAA....
........
........
........
"""


def status():
    fr = [shade(G(8, 8, a), {}, 1) for a in (ST_LIGHT, ST_SHIELD, ST_HASTE, ST_WIND, ST_BELT)]
    return fr, ["light", "shield", "haste", "second_wind", "belt_mark"]


# ------------------------------------------------------------------------------------------------ spell fx
def fx_missile(ramp=BLUE, frost=False):
    xx, yy = grid(16, 16)
    r = np.hypot(xx - 7.5, yy - 7.5)
    fr = []
    core = f_clip(1 - r / 4.2) ** 1.2
    if frost:
        fr.append(sp_frost(16, 7.5, 5.0, ramp))
        f2 = sp_frost(16, 7.5, 6.5, ramp)
        fr.append(np.rot90(f2).copy())
    else:
        fr.append(glow(np.maximum(core, rays(xx, yy, 7.5, 7.5, 4, 6.5, 6.5, 0.7, 0)), ramp, 0.12))
        fr.append(glow(np.maximum(f_clip(1 - r / 4.8) ** 1.2, rays(xx, yy, 7.5, 7.5, 4, 5.0, 5.0, 0.7, 45)),
                       ramp, 0.12))
    fr.append(glow(np.maximum(f_clip(1 - r / 3.4), rays(xx, yy, 7.5, 7.5, 8, 7.5, 5.5, 0.8, 22)), ramp, 0.12))
    ring = f_clip(1 - np.abs(r - 5.6) / 1.1) * 0.7
    rng = np.random.default_rng(9)
    sp = np.zeros((16, 16))
    for _ in range(7):
        a = rng.uniform(0, 2 * math.pi)
        rr = rng.uniform(3, 7)
        x, y = int(7.5 + rr * math.cos(a)), int(7.5 + rr * math.sin(a))
        if 0 <= x < 16 and 0 <= y < 16:
            sp[y, x] = 0.95
    fr.append(glow(np.maximum(ring, sp), ramp, 0.2))
    return fr


def fx_heal():
    """Rising warm motes: plus-shaped glints that grow, peak and fade while drifting up (4 frames)."""
    fr = []
    motes = [(4, 12, 0), (11, 13, 1), (7, 10, 2), (2, 8, 1), (13, 7, 0)]
    for f in range(4):
        out = np.zeros((16, 16), int)
        for (x, y, ph) in motes:
            age = (f + ph) % 4
            yy_ = y - 2 * f
            if not (0 <= yy_ < 16):
                continue
            size = [1, 2, 1, 0][age]
            col = [10, 7, 11, 8][age]
            out[yy_, x] = col
            for k in range(1, size + 1):
                for dx, dy in ((k, 0), (-k, 0), (0, k), (0, -k)):
                    if 0 <= x + dx < 16 and 0 <= yy_ + dy < 16:
                        out[yy_ + dy, x + dx] = 11 if k == 1 else 10
        fr.append(out)
    return fr


def fx_light_burst():
    xx, yy = grid(64, 64)
    r = np.hypot(xx - 31.5, yy - 31.5)
    fr = []
    for core, L, Ls, w, ring in ((9, 22, 15, 1.4, 0), (15, 31, 22, 1.8, 0), (0, 0, 0, 0, 25)):
        I = np.zeros((64, 64))
        if core:
            I = f_clip(1 - r / core) ** 0.7
            I = np.maximum(I, rays(xx, yy, 31.5, 31.5, 16, L, Ls, w, 11.25) * 0.85)
            I = np.maximum(I, f_clip(1 - r / (core * 2.2)) * 0.35)
        else:
            I = f_clip(1 - np.abs(r - ring) / 4) * 0.55
        fr.append(glow(I, GOLDR, 0.15, 0.6))
    return fr


def fx_fireball():
    xx, yy = grid(32, 32)
    r = np.hypot(xx - 15.5, yy - 15.5)
    ang = np.arctan2(yy - 15.5, xx - 15.5)
    fr = []
    for ph in (0, 1):
        wob = 1 + 0.18 * np.sin(ang * 5 + ph * 2.2) + 0.08 * np.sin(ang * 9 - ph * 3)
        I = f_clip(1 - r / (7.5 * wob)) ** 0.75
        fr.append(glow(I, FIRE, 0.12))
    rng = np.random.default_rng(5)
    for k, (R, smoke) in enumerate(((8, 0), (12.5, 0.25), (14.5, 0.7))):
        wob = 1 + 0.22 * np.sin(ang * 7 + k) + 0.1 * np.sin(ang * 13)
        I = f_clip(1 - r / (R * wob)) ** (0.6 + 0.5 * k)
        if smoke:
            I = I * (1 - smoke * f_clip(1 - r / (R * 0.7)))
        out = glow(I, FIRE, 0.12)
        if smoke:
            sm = (I > 0.04) & (I < 0.2) & (rng.random((32, 32)) < 0.5 * smoke + 0.2)
            out[sm & (out == 0)] = 15
            out[sm & (out == 8)] = 15
        fr.append(out)
    return fr


def fx_glow_strips():
    h = np.zeros((8, 32), int)
    v = np.zeros((32, 8), int)
    for y in range(8):
        for x in range(32):
            t = 1 - y / 8
            k = int(np.clip(np.floor(t * 4 + (BAY[y % 4, x % 4] - 0.5) * 0.9), -1, 3))
            h[y, x] = 0 if k < 0 else k + 1
    v = h.T[:, ::1].copy()
    return [h], [v]


GLOW_PALS = {
    "pal_glow_heal": ["#2a0c0a", "#8a2a1a", "#e09a50", "#fff0c0"],
    "pal_glow_light": ["#4a3418", "#b08a48", "#f0d8a0", "#fffbec"],
    "pal_glow_magic": ["#12203c", "#2a5aa8", "#7ab4f0", "#e6f4ff"],
}


def glow_pal(cols):
    return mkpal(cols + ["#000000"] * 11)


# ================================================================================================ CHESTS (32x32)
CH_PAL = mkpal(["#080806", "#151510", "#21160e", "#382618", "#563c24", "#7a5a38", "#1c1c1a", "#36352f",
                "#5c5a50", "#8e897a", "#5a3218", "#4a0a0c", "#d22a1a", "#c8a874", "#c4b894"])
CH_LEG = {"w": ([3, 4, 5, 6], 14, 0.9), "v": ([1, 3, 4], 5, 0.6), "i": ([7, 8, 9, 10], 10, 1.0),
          "u": ([11, 4], 6, 0.5), "j": ([8, 9, 10], 14, 1.0)}


def chest(kind="normal", lid=0, flash=False, teeth=False, tongue=False, open_=False):
    g = G(32, 32)
    g.rect(2, 30, 29, 30, "1")
    g.rect(4, 31, 27, 31, "2")
    g.rect(3, 18, 28, 29, "w")
    for y in (22, 26):
        g.rect(4, y, 27, y, "3")
    g.rect(3, 18, 28, 18, "i")
    g.rect(3, 29, 28, 29, "i")
    g.rect(6, 18, 7, 29, "i")
    g.rect(24, 18, 25, 29, "i")
    for (x, y) in ((6, 20), (6, 27), (24, 20), (24, 27)):
        g.set(x, y, "A")
    if open_:
        for y in range(3, 16):                                    # inner face of the lid, tilted back
            t = (y - 3) / 12
            x0, x1 = int(round(6 - 3 * t)), int(round(25 + 3 * t))
            g.rect(x0, y, x1, y, "v")
            bx = int(round(8 - 2 * t))
            g.rect(bx, y, bx + 1, y, "i")
            g.rect(31 - bx - 1, y, 31 - bx, y, "i")
        g.rect(6, 3, 25, 3, "i")
        g.rect(4, 15, 27, 15, "i")
        g.rect(3, 16, 28, 17, "1")
        g.rect(5, 17, 26, 17, "2")
        g.rect(14, 18, 17, 20, "i")
        g.set(15, 19, "1")
        g.set(16, 19, "1")
        return g
    top = 10 - lid
    half = {0: 9.0, 1: 11.5, 2: 12.5}
    for k in range(8):
        y = top + k
        hw = half.get(k, 13.0)
        x0, x1 = int(round(15.5 - hw)), int(round(15.5 + hw))
        g.rect(x0, y, x1, y, "w")
        g.rect(max(x0, 6), y, min(x1, 7), y, "i")
        g.rect(max(x0, 24), y, min(x1, 25), y, "i")
    g.rect(3, top + 7, 28, top + 7, "i")
    if kind == "locked":
        g.rect(3, top + 3, 28, top + 4, "i")
    if lid:
        g.rect(4, top + 8, 27, 17, "1")
        if teeth:
            g.rect(6, 15, 25, 16, "C") if lid >= 5 else None
            for x, ln in ((5, 3), (9, 1), (12, 2), (19, 2), (22, 1), (25, 3)):      # upper fangs
                for k in range(min(ln, lid - 1)):
                    g.set(x, top + 8 + k, "F" if k < ln - 1 else "E")
                    if ln == 3 and k == 0:
                        g.set(x + 1, top + 8, "E")
            for x, ln in ((7, 2), (11, 1), (16, 2), (21, 1), (24, 2)):              # lower fangs
                for k in range(min(ln, lid - 1)):
                    g.set(x, 17 - k, "F" if k < ln - 1 else "E")
            if tongue and lid >= 5:
                g.rect(10, 15, 21, 16, "C")
                g.rect(12, 14, 19, 14, "C")
                for (x, y) in ((13, 14), (14, 14), (12, 15), (17, 14)):
                    g.set(x, y, "D")
    hy = top + 5
    g.rect(14, hy, 17, hy + 5, "i")
    g.set(14, hy, "I")
    kh = "D" if flash else "1"
    g.set(15, hy + 2, kh)
    g.set(16, hy + 2, kh)
    g.set(15, hy + 3, kh)
    if flash:
        for (x, y) in ((14, hy + 2), (17, hy + 2), (15, hy + 1), (16, hy + 1), (16, hy + 3), (15, hy + 4)):
            g.set(x, y, "C")
    if kind == "locked":
        for (x, y) in ((14, 20), (14, 21), (17, 20), (17, 21), (15, 19), (16, 19)):
            g.set(x, y, "i")
        g.rect(15, 20, 16, 21, "1")
        g.rect(12, 22, 19, 28, "j")
        g.rect(13, 21, 18, 21, "j")
        g.set(12, 22, "J")
        g.set(13, 21, "J")
        g.rect(15, 24, 16, 24, "1")
        g.set(15, 25, "1")
        for (x, y) in ((13, 27), (18, 26), (18, 27), (17, 28)):
            g.set(x, y, "B")
    return g


def chests():
    defs = [("normal", dict()), ("normal", dict(open_=True)), ("locked", dict()), ("locked", dict(open_=True)),
            ("normal", dict(flash=True)), ("normal", dict(lid=1, flash=True))]
    fr = []
    for kind, kw in defs:
        g = chest(kind, **kw)
        fr.append(shade(g, CH_LEG, 61, origin=(15.5, 18)))
    tags = "closed:1-1:500;open:2-2:500;locked:3-3:500;locked_open:4-4:500;trap_flash:5-5:500;trap_shake:6-6:500"
    return fr, tags


def mimic():
    defs = [(dict(), 0), (dict(lid=1, teeth=True), 0), (dict(lid=3, teeth=True, flash=True), 0),
            (dict(lid=3, teeth=True, flash=True), 0.3), (dict(lid=7, teeth=True, tongue=True, flash=True), 0),
            (dict(), 0.12)]
    fr = []
    for kw, br in defs:
        g = chest("normal", **kw)
        fr.append(shade(g, CH_LEG, 61, origin=(15.5, 18), bright=br))
    fr[5] = np.vstack([np.zeros((1, 32), int), fr[5][:-1]])      # snap: lurches down 1 px
    fr[5][31] = np.where(fr[0][31] > 0, fr[0][31], fr[5][31])
    tags = "breathe:1-2:600;wake:3-4:120;bite:5-6:150"
    return fr, tags


# ================================================================================================ SHRINES (32x32, M2-M3)
SH_HEX = ["#070806", "#141612", "#252a24", "#363d34", "#4c5448", "#677060", "#8a917e", "#6a4a14", "#b48a30",
          "#ecd078", "#1c3c5c", "#3a78a8", "#8cc8e8", "#e8ecd8", "#2a1a10"]
SH_PAL = mkpal(SH_HEX)
BLOOD_PAL = mkpal(SH_HEX[:10] + ["#3a0a0a", "#801414", "#c83a2a"] + SH_HEX[13:])
MURKY_PAL = mkpal(SH_HEX[:10] + ["#1e3018", "#3e6a28", "#7ea050"] + SH_HEX[13:])
SH_LEG = {"k": ([3, 4, 5, 6, 7], 14, 0.8), "g": ([8, 9, 10], 14, 1.0), "u": ([11, 12, 13], 14, 0.6),
          "d": ([2, 3, 4], 5, 0.6)}


def floor_shadow(g, x0, x1):
    g.rect(x0, 30, x1, 30, "1")
    g.rect(x0 + 2, 31, x1 - 2, 31, "2")


def prop_gold():
    g = G(32, 32)
    xx, yy = grid(32, 32)
    floor_shadow(g, 4, 27)
    g.mask((((xx - 15.5) / 10.5) ** 2 + ((yy - 29) / 9) ** 2 <= 1) & (yy <= 29), "g")
    for (cx, cy) in ((11, 24), (18, 22), (15, 26), (21, 27), (9, 28), (14, 21)):
        g.mask((np.abs(((xx - cx) / 2.6) ** 2 + ((yy - cy) / 1.3) ** 2 - 1) < 0.35) & (yy >= cy), "8")
    for (cx, cy) in ((3, 29), (27, 29), (25, 28)):
        g.mask(((xx - cx) / 2.0) ** 2 + ((yy - cy) / 1.0) ** 2 <= 1, "g")
    for (x, y) in ((13, 20), (12, 23), (19, 21), (16, 25)):
        g.set(x, y, "G")
    return [shade(g, SH_LEG, 71, origin=(15.5, 24))]


def pedestal(g, bowl):
    floor_shadow(g, 3, 28)
    g.rect(5, 26, 26, 29, "k")
    g.rect(4, 28, 27, 29, "k")
    g.rect(10, 15, 21, 25, "k")
    g.rect(7, 12, 24, 14, "k")
    g.rect(10, 15, 21, 15, "2")
    g.rect(5, 26, 26, 26, "3")
    xx, yy = grid(32, 32)
    g.mask((np.abs(np.hypot(xx - 15.5, yy - 20) - 2.6) < 0.6), "2")
    if bowl == "coin":
        g.mask((((xx - 15.5) / 6.5) ** 2 + ((yy - 9) / 3.5) ** 2 <= 1) & (yy >= 9) & (yy <= 11), "g")
        g.mask((((xx - 15.5) / 5) ** 2 + ((yy - 9) / 3) ** 2 <= 1) & (yy < 9), "g")
        for (x, y) in ((13, 7), (17, 8), (15, 6)):
            g.set(x, y, "G")
        g.rect(14, 12, 17, 12, "1")                              # coin slot cut in the slab
    else:
        g.mask((((xx - 15.5) / 7) ** 2 + ((yy - 10) / 3.0) ** 2 <= 1) & (yy <= 11), "k")
        g.mask(((xx - 15.5) / 5.2) ** 2 + ((yy - 9.5) / 1.5) ** 2 <= 1, "u")


def prop_altar_coin():
    g = G(32, 32)
    pedestal(g, "coin")
    return [shade(g, SH_LEG, 72, origin=(15.5, 18))]


def prop_blood_altar():
    fr = []
    for f in range(2):
        g = G(32, 32)
        pedestal(g, "blood")
        if f:
            g.set(13, 9, "U")
            g.set(17, 10, "U")
        fr.append(shade(g, SH_LEG, 73, origin=(15.5, 18), bright=0.0))
        if f:
            m = fr[-1] == 11
            fr[-1][m] = 12
    return fr


def prop_fountain():
    fr = []
    xx, yy = grid(32, 32)
    for f in range(3):
        g = G(32, 32)
        floor_shadow(g, 2, 29)
        g.mask((((xx - 15.5) / 13.5) ** 2 + ((yy - 22) / 5.5) ** 2 <= 1) | ((np.abs(xx - 15.5) <= 13.5)
               & (yy >= 22) & (yy <= 29)), "k")
        g.mask(((xx - 15.5) / 11.5) ** 2 + ((yy - 21.5) / 3.6) ** 2 <= 1, "u" if f < 2 else "d")
        g.rect(14, 6, 17, 20, "k")
        g.rect(13, 5, 18, 6, "k")
        g.rect(13, 13, 18, 13, "3")
        if f < 2:
            for y in range(7, 20):
                if (y + f * 2) % 4 != 0:
                    g.set(12, y + 0, "u")
                    g.set(19, y + 0, "u")
            for (x, y) in ((8 + 4 * f, 21), (20 - 3 * f, 22), (15, 20 + f)):
                g.set(x, y, "U")
        fr.append(shade(g, SH_LEG, 74, origin=(15.5, 18)))
    return fr


# ------------------------------------------------------------------------------------------------ pause cursor
def ui_cursor():
    """Gold 2 px frame (MAGIA_Y_TESOROS 5.1): 'bag' = 32x32 slot; 'equip' = 31x31 around a 27x27 equipment slot
    (sprite at slot - 2). Worn gold, lit top-left, corner rivets; jewel palette (shared with icon_jewels)."""
    fr = []
    for size in (32, 31):
        a = np.zeros((32, 32), int)
        n = size - 1
        for y in range(size):
            for x in range(size):
                edge = min(x, y, n - x, n - y)
                if edge > 1:
                    continue
                lit = (x + y) < n
                c = (4 if edge == 0 else 3) if lit else (3 if edge == 0 else 2)
                if (x + y) % 7 == 0 and edge == 0:
                    c = 5 if lit else 3
                a[y, x] = c
        for cx, cy in ((1, 1), (n - 1, 1), (1, n - 1), (n - 1, n - 1)):
            a[cy, cx] = 15 if (cx + cy) < n else 4
        fr.append(a)
    return fr


# ================================================================================================ export
def save_png(path, arr, pal):
    im = Image.fromarray(arr.astype(np.uint8), "P")
    flat = [c for col in pal for c in col]
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.save(path, transparency=0)


def to_rgb(arr, pal, bg=(22, 22, 24)):
    lut = np.array(pal + [(0, 0, 0)] * (256 - len(pal)), np.uint8)
    rgb = lut[arr]
    rgb[arr == 0] = bg
    return rgb


SHEETS = []          # (name, frames, pal, tags, names)


def add(name, frames, pal, tags="", names=None):
    for f in frames:
        assert f.max() < 16, name
    SHEETS.append((name, frames, pal, tags, names))


def tagstr(names, ms=500):
    return ";".join("%s:%d-%d:%d" % (n, i + 1, i + 1, ms) for i, n in enumerate(names))


def build_all():
    fr, nm = items()
    add("icon_items", fr, ITEM_PAL, tagstr(nm), nm)
    fr, nm = equipment()
    add("icon_equip", fr, EQ_PAL, tagstr(nm), nm)
    fr, nm = jewels()
    add("icon_jewels", fr, JW_PAL, tagstr(nm), nm)
    add("ui_cursor", ui_cursor(), JW_PAL, "bag:1-1:500;equip:2-2:500")
    fr, nm = spells()
    add("icon_spells", fr, SP_PAL, tagstr(nm), nm)
    fr, nm = status()
    add("icon_status", fr, SP_PAL, tagstr(nm), nm)
    add("fx_missile", fx_missile(), SP_PAL, "fly:1-2:60;hit:3-4:70")
    add("fx_frost", fx_missile(CYAN, frost=True), SP_PAL, "fly:1-2:60;hit:3-4:70")
    add("fx_heal", fx_heal(), SP_PAL, "sparkle:1-4:80")
    add("fx_light_burst", fx_light_burst(), SP_PAL, "burst:1-3:45")
    add("fx_fireball", fx_fireball(), SP_PAL, "fly:1-2:70;burst:3-5:80")
    gh, gv = fx_glow_strips()
    add("fx_glow_h", gh, glow_pal(GLOW_PALS["pal_glow_light"]))
    add("fx_glow_v", gv, glow_pal(GLOW_PALS["pal_glow_light"]))
    fr, tags = chests()
    add("prop_chests", fr, CH_PAL, tags)
    fr, tags = mimic()
    add("prop_mimic", fr, CH_PAL, tags)
    add("prop_gold", prop_gold(), SH_PAL)
    add("prop_altar_coin", prop_altar_coin(), SH_PAL)
    add("prop_fountain", prop_fountain(), SH_PAL, "flow:1-2:250;dry:3-3:500")
    add("prop_blood_altar", prop_blood_altar(), BLOOD_PAL, "pulse:1-2:500")
    swatch = np.arange(16).repeat(4).reshape(8, 8)
    for pname, cols in GLOW_PALS.items():
        add(pname, [swatch], glow_pal(cols))
    add("pal_fountain_murky", [swatch], MURKY_PAL)


PALETTE_ITEMS = set(GLOW_PALS) | {"pal_fountain_murky"}


def contact_sheet(path, scale=3):
    cells = []
    for name, frames, pal, tags, names in SHEETS:
        row = [to_rgb(f, pal) for f in frames]
        cells.append((name, row))
    pad = 4
    width = max(sum(f.shape[1] + pad for f in row) for _, row in cells) + 110
    height = sum(max(f.shape[0] for f in row) + pad for _, row in cells)
    sheet = np.zeros((height, width, 3), np.uint8)
    sheet[:] = (40, 40, 44)
    y = 0
    labels = []
    for name, row in cells:
        x = 110
        for f in row:
            sheet[y:y + f.shape[0], x:x + f.shape[1]] = f
            x += f.shape[1] + pad
        labels.append((y, name))
        y += max(f.shape[0] for f in row) + pad
    im = Image.fromarray(sheet).resize((width * scale, height * scale), Image.NEAREST)
    d = ImageDraw.Draw(im)
    for y, name in labels:
        d.text((4, y * scale + 2), name, fill=(220, 220, 200))
    im.save(path)


def mockup(path):
    """Pause BAG + HUD with real icons, as the game would place them (art review only)."""
    p = os.path.join(ART, "ui_seed_out", "pause_bag.png")
    if not os.path.exists(p):
        return
    bg = Image.open(p)
    pal = bg.getpalette()
    base = np.array(bg.convert("RGB"))
    sh = {n: (fr, pl) for n, fr, pl, _t, _nm in SHEETS}

    def put(name, k, x, y):
        fr, pl = sh[name]
        f = fr[k]
        rgb = to_rgb(f, pl)
        m = f > 0
        base[y:y + f.shape[0], x:x + f.shape[1]][m] = rgb[m]
    bag = [("icon_items", 1), ("icon_items", 2), ("icon_items", 0), ("icon_items", 3), ("icon_items", 4),
           ("icon_jewels", 0), ("icon_equip", 1), ("icon_items", 5), ("icon_items", 6), ("icon_equip", 6),
           ("icon_jewels", 5), ("icon_equip", 3)]
    for i, (n, k) in enumerate(bag):
        put(n, k, 24 + 32 * (i % 4) + 8, 16 + 32 * (i // 4) + 8)
    put("icon_status", 4, 24 + 32 + 24, 16 + 3)
    for (n, k), (x, y) in zip([("icon_equip", 0), ("icon_equip", 4), ("icon_equip", 10), ("icon_equip", 11)],
                              [(180, 4), (209, 4), (180, 33), (209, 33)]):
        put(n, k, x + 5, y + 5)
    put("icon_spells", 0, 200, 126)
    put("icon_status", 0, 214, 97)
    put("icon_status", 1, 224, 97)
    fr, pl = sh["icon_equip"]
    Image.fromarray(base).resize((720, 480), Image.NEAREST).save(path)


def export(name, frames, pal, tags):
    png = os.path.join(OUT, name + ".png")
    ase = os.path.join(ART, name + ".aseprite")
    fh = frames[0].shape[0]
    if not os.path.exists(ase):
        subprocess.run([ASE, "-b", "--script-param", "in=" + png, "--script-param", "out=" + ase,
                        "--script-param", "fh=%d" % fh, "--script-param", "tags=" + tags,
                        "--script-param", "bg=0", "--script-param", "ncol=16",
                        "--script", os.path.join(HERE, "build_aseprite.lua")], check=True, stdout=subprocess.DEVNULL)
        print("created", name + ".aseprite")
    subprocess.run([ASE, "-b", ase, "--sheet-type", "vertical", "--sheet", os.path.join(GFX, name + ".bmp")],
                   check=True, stdout=subprocess.DEVNULL)
    kind = '{ "type": "sprite_palette" }' if name in PALETTE_ITEMS else '{ "type": "sprite", "height": %d }' % fh
    with open(os.path.join(GFX, name + ".json"), "w") as f:
        f.write(kind + "\n")


def main():
    os.makedirs(OUT, exist_ok=True)
    build_all()
    if "--export-only" not in sys.argv:
        for name, frames, pal, tags, _n in SHEETS:
            save_png(os.path.join(OUT, name + ".png"), np.vstack(frames), pal)
        contact_sheet(os.path.join(OUT, "magic_contact_3x.png"))
        mockup(os.path.join(OUT, "magic_mockup_3x.png"))
    if "--apply" in sys.argv or "--export-only" in sys.argv:
        for name, frames, pal, tags, _n in SHEETS:
            if "--export-only" in sys.argv and not os.path.exists(os.path.join(ART, name + ".aseprite")):
                continue
            export(name, frames, pal, tags)


if __name__ == "__main__":
    main()
