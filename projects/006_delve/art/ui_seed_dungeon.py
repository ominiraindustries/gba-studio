"""DELVE HUD, restyle 2 (G-38): "mazmorrero puro" - carved dark stone, blackened iron, bone and skulls, worn leather.

Same boxes as before (src/dl_hud.cpp hard-codes them; see art/LÉEME.md). Writes seed PNGs and, with --apply,
rebuilds art/ui_frame.aseprite + art/ui_arrow.aseprite (deleting the previous ones: only do it if the user has not
edited them) and exports graphics/ui_frame.bmp + graphics/ui_arrow.bmp.

  python -I ui_seed_dungeon.py            seeds only (art/ui_seed_out/ui_frame.png, ui_arrow.png)
  python -I ui_seed_dungeon.py --apply    seeds + .aseprite + export

Palette indices the code uses (keep them): 205-207 reload fill / weapon glow (now dull bone), 209 plate (key box
fill), 210-214 HP (empty, dark, red, hi, pale), 215-216 reload empty / glint. 240-247 belong to the minimap.
"""
import os
import subprocess
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
GFX = os.path.join(os.path.dirname(HERE), "graphics")
OUT = os.path.join(HERE, "ui_seed_out")
ASE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"


def hx(s):
    return tuple(int(s[i:i + 2], 16) for i in (1, 3, 5))


PAL = [(0, 0, 0)] * 256
NAMED = {}


def put(i0, name, cols):
    NAMED[name] = list(range(i0, i0 + len(cols)))
    for k, c in enumerate(cols):
        PAL[i0 + k] = hx(c)


put(192, "s", ["#0a0908", "#15120f", "#221e1a", "#302a24", "#423a32", "#5a5046"])    # carved stone, cold grey-brown
put(198, "i", ["#040404", "#0e0d0c", "#1a1816", "#2a2724", "#433e38", "#686056"])    # blackened iron
put(204, "bd", ["#3b342a"])                                                           # bone shadow
put(205, "bn", ["#5f5545", "#8a7e66", "#b3a688"])         # bone (code: reload fill, weapon glow)
put(208, "p", ["#050404", "#0a0908"])                     # recessed plate (209 = code 'plate')
put(210, "r", ["#1a0b0a", "#3a100e", "#6a1a16", "#8e2c24", "#b87060"])               # HP: desaturated blood
put(215, "o", ["#100e0c", "#d4c8ac"])                     # reload empty, glint
put(217, "b", ["#959894", "#c2c3bc"])                     # dull blade steel
put(219, "l", ["#1c130d", "#311f15", "#4a3222", "#5e4430"])                          # worn leather
put(223, "c", ["#34403f"])                                # dull glass
put(224, "k", ["#16130f", "#252019", "#2f2920", "#cbbf9f"])                          # bone darks + highlight
put(228, "u", ["#24160e", "#3c2616", "#56381e"])          # rust
put(231, "c2", ["#5a6a68"])                               # glass glint
put(232, "mp", ["#0a0c18", "#121c40", "#2a4a9c", "#5a86d4", "#a8c8ee"])              # MP (G-40): empty..pale
put(237, "belt", ["#050404"])                             # belt-slot glow ring: = plate; the code pulses it blue
S, I, P, R, O, B, L = NAMED["s"], NAMED["i"], NAMED["p"], NAMED["r"], NAMED["o"], NAMED["b"], NAMED["l"]
BONE = [NAMED["k"][0], NAMED["k"][1], NAMED["k"][2], NAMED["bd"][0]] + NAMED["bn"] + [NAMED["k"][3]]  # 8 dark->light
U = NAMED["u"]
GLASS = [NAMED["c"][0], NAMED["c2"][0]]

BAY = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def vnoise(rng, h, w, c):
    g = rng.random((h // c + 2, w // c + 2))
    y, x = np.arange(h) / c, np.arange(w) / c
    y0, x0 = y.astype(int), x.astype(int)
    fy, fx = y - y0, x - x0
    fy, fx = fy * fy * (3 - 2 * fy), fx * fx * (3 - 2 * fx)
    a, b, cc, d = g[y0][:, x0], g[y0][:, x0 + 1], g[y0 + 1][:, x0], g[y0 + 1][:, x0 + 1]
    top = a + (b - a) * fx
    bot = cc + (d - cc) * fx
    return top + (bot - top) * fy[:, None]


def fbm(rng, h, w):
    return (0.5 * vnoise(rng, h, w, 8) + 0.3 * vnoise(rng, h, w, 4) + 0.2 * vnoise(rng, h, w, 2))


def q(v, ramp, dith=0.7, y=0, x=0):
    n = len(ramp)
    b = BAY[y % 4, x % 4]
    return ramp[int(np.clip(np.floor(v * n + (b - 0.5) * dith), 0, n - 1))]


class Cv:
    def __init__(self):
        self.p = np.zeros((160, 240), int)

    def set(self, x, y, c):
        if 0 <= x < 240 and 0 <= y < 160:
            self.p[y, x] = c

    def rect(self, x0, y0, x1, y1, c):
        self.p[y0:y1 + 1, x0:x1 + 1] = c


# ------------------------------------------------------------------------------------------------ stone
def stone(cv, rng, x0, y0, x1, y1):
    """Rough ashlar: irregular blocks, chisel-pitted, lit from the top-left, deep joints; low key."""
    h, w = y1 - y0 + 1, x1 - x0 + 1
    N = fbm(rng, h, w)
    V = np.zeros((h, w))
    joint = np.zeros((h, w), bool)
    y, row = 0, 0
    while y < h:
        bh = int(rng.integers(7, 11))
        x = -int(rng.integers(0, 14))
        while x < w:
            bw = int(rng.integers(11, 26))
            base = rng.uniform(0.28, 0.5)
            for yy in range(y, min(h, y + bh)):
                for xx in range(max(0, x), min(w, x + bw)):
                    dt, db, dl, dr = yy - y, y + bh - 1 - yy, xx - x, x + bw - 1 - xx
                    v = base + 0.35 * (N[yy, xx] - 0.5)
                    if dt == 1 or dl == 1:
                        v += 0.16
                    if db == 1 or dr == 1:
                        v -= 0.16
                    if dt == 0 or dl == 0:
                        joint[yy, xx] = True
                    V[yy, xx] = v
            x += bw
        y += bh
        row += 1
    for _ in range(w * h // 70):                                  # chisel pits
        yy, xx = int(rng.integers(0, h)), int(rng.integers(0, w))
        V[yy, xx] -= 0.25
        if xx + 1 < w:
            V[yy, xx + 1] += 0.08
    for _ in range(max(1, w * h // 900)):                         # cracks
        yy, xx = int(rng.integers(0, h)), int(rng.integers(0, w))
        for _k in range(int(rng.integers(4, 10))):
            if 0 <= yy < h and 0 <= xx < w:
                V[yy, xx] = 0.02
            yy += 1
            xx += int(rng.integers(-1, 2))
    for yy in range(h):
        for xx in range(w):
            cv.p[y0 + yy, x0 + xx] = S[0] if joint[yy, xx] else q(np.clip(V[yy, xx], 0, 0.99), S, 0.7, yy, xx)


# ------------------------------------------------------------------------------------------------ iron frame
def nail(cv, x, y):
    cv.set(x, y, I[5])
    cv.set(x + 1, y, I[4])
    cv.set(x, y + 1, I[3])
    cv.set(x + 1, y + 1, I[0])


def box(cv, rng, x0, y0, x1, y1, fill=P[1], brackets=True, straps=True, tex=True):
    """A recess cut into the stone: 2 px chiselled stone lip (top/left lit, bottom/right in shadow), a black gap and
    a dark plate; blackened iron L brackets on the corners and iron straps across long lips, with nail heads.
    No clean bevel: every lip pixel is chipped and uneven. Cast shadow down and right onto the wall."""
    N = fbm(rng, y1 - y0 + 1, x1 - x0 + 1)
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            v = N[y - y0, x - x0]
            cv.p[y, x] = (P[0] if v < 0.40 else S[1] if v > 0.70 and rng.random() < 0.25 else fill) if tex else fill
    for x in range(x0, x1 + 2):                                   # cast shadow outside (bottom, right)
        if y1 + 1 < 160 and x < 240 and cv.p[y1 + 1, x] in S:
            cv.set(x, y1 + 1, S[0])
    for y in range(y0, y1 + 2):
        if x1 + 1 < 240 and y < 160 and cv.p[y, x1 + 1] in S:
            cv.set(x1 + 1, y, S[0])

    def lip(x, y, lit, inner):
        r = rng.random()
        if r < 0.08:
            c = S[1]                                              # chip
        elif lit:
            c = q(0.74 + 0.3 * (N[y - y0, x - x0] - 0.5) - 0.18 * inner, S, 0.9, y, x)
        else:
            c = q(0.22 + 0.2 * (N[y - y0, x - x0] - 0.5) + 0.12 * inner, S, 0.9, y, x)
        cv.set(x, y, c)
    for x in range(x0, x1 + 1):
        lip(x, y0, True, 0)
        lip(x, y0 + 1, True, 1)
        lip(x, y1, False, 0)
        lip(x, y1 - 1, False, 1)
    for y in range(y0, y1 + 1):
        lip(x0, y, True, 0)
        lip(x0 + 1, y, True, 1)
        lip(x1, y, False, 0)
        lip(x1 - 1, y, False, 1)
    if straps and x1 - x0 > 60:                                   # iron straps over the long lips
        for x in range(x0 + 30, x1 - 20, 46):
            for yy in (y0, y0 + 1, y1 - 1, y1):
                cv.set(x, yy, I[4] if yy <= y0 + 1 else I[3])
                cv.set(x + 1, yy, I[3] if yy <= y0 + 1 else I[2])
                cv.set(x + 2, yy, I[1])
            nail(cv, x, y0)
            nail(cv, x, y1 - 1)
            for k in range(2, 5):                                 # rust run below the lower strap
                if y1 + k < 160 and cv.p[y1 + k - 1, x + 1] in S:
                    cv.set(x + 1, y1 + k - 1, U[1] if k < 4 else U[0])
    if brackets and x1 - x0 > 12 and y1 - y0 > 12:
        for cx, cy, sx, sy in ((x0, y0, 1, 1), (x1, y0, -1, 1), (x0, y1, 1, -1), (x1, y1, -1, -1)):
            for k in range(7):
                for t in range(3):
                    lit = (sy > 0 and t == 0) or (sx > 0 and t == 0)
                    c = I[4] if lit else I[3] if t == 1 else I[2]
                    if k == 6:
                        c = I[1]
                    cv.set(cx + sx * k, cy + sy * t, c)
                    cv.set(cx + sx * t, cy + sy * k, c)
            nail(cv, cx + sx * 1 - (sx < 0), cy + sy * 1 - (sy < 0))


def trough(cv, x0, y0, x1, y1, empty):
    cv.rect(x0, y0, x1, y1, I[0])
    cv.rect(x0 + 1, y0 + 1, x1 - 1, y1 - 1, empty)
    for x in range(x0, x1 + 1):                                   # lip below the slot catches a little light
        cv.set(x, y1 + 1, I[3]) if cv.p[min(159, y1 + 1), x] in I + P else None


# ------------------------------------------------------------------------------------------------ bone
SKULL = """
...4556...
..456775..
.45677765.
.4566665..
42002002.4
.4200200 3
.34566543.
..3240423.
...24242..
....1111..
"""
SKULL_SMALL = """
.3553.
356673
200203
.4554.
.3443.
..22..
"""
HORNS_L = ["...11", "..12.", ".23..", "34...", "4...."]


def stamp(cv, x0, y0, art, cmap):
    for j, row in enumerate(art.strip("\n").split("\n")):
        for i, ch in enumerate(row):
            if ch in cmap:
                cv.set(x0 + i, y0 + j, cmap[ch])


BMAP = {str(k): BONE[k] for k in range(8)}
BMAP["0"] = I[0]


def skull_shadow(cv, x0, y0, art):
    rows = art.strip("\n").split("\n")
    for j, row in enumerate(rows):
        for i, ch in enumerate(row):
            if ch in BMAP and ch != " ":
                x, y = x0 + i + 1, y0 + j + 1
                if 0 <= y < 160 and 0 <= x < 240 and cv.p[y, x] in S + I + P:
                    cv.set(x, y, S[0] if cv.p[y, x] in S else I[0])


def skull(cv, x0, y0, art=SKULL):
    skull_shadow(cv, x0, y0, art)
    stamp(cv, x0, y0, art, BMAP)


# ------------------------------------------------------------------------------------------------ icons
def sword(cv, rng, x0, y0):
    """32x28 notched old longsword: dull steel, iron guard, leather grip, iron pommel; light from the top-left."""
    pl = P[1]
    cv.rect(x0, y0, x0 + 31, y0 + 27, pl)
    pts = {}
    for x in range(10, 30):
        yc = 28 - x
        cols = [(yc - 2, I[4]), (yc - 1, B[1]), (yc, B[0]), (yc + 1, I[3])]
        if x >= 27:
            cols = cols[x - 26:]
        for y, c in cols:
            if 0 <= y < 28:
                pts[(x, y)] = c
    for x in (15, 21, 24):                                        # notches in the edge
        pts.pop((x, 28 - x - 2), None)
        pts[(x, 28 - x - 1)] = I[3]
    for x in range(12, 25):                                       # fuller and dark pitting
        pts[(x, 27 - x)] = I[4] if rng.random() < 0.7 else I[3]
    for t in range(-4, 5):                                        # blackened iron guard
        for dx, c in ((0, I[5] if t < 0 else I[4]), (1, I[3]), (2, I[2])):
            pts[(8 + t + dx, 17 + t)] = c
    for k, x in enumerate(range(4, 9)):                           # worn leather grip
        yc = 27 - x
        pts[(x, yc - 1)] = L[3] if k % 2 else L[2]
        pts[(x, yc)] = L[1] if k % 2 else L[0]
        pts[(x + 1, yc)] = L[1]
    for (i, j), ch in {(1, 22): "4", (2, 22): "5", (0, 23): "4", (1, 23): "3", (2, 23): "3", (3, 23): "2",
                       (0, 24): "3", (1, 24): "2", (2, 24): "2", (3, 24): "1", (1, 25): "1", (2, 25): "1"}.items():
        pts[(i + 1, j)] = I[int(ch)]                              # iron pommel
    for (x, y), c in pts.items():
        cv.set(x0 + x, y0 + y, c)
    for (x, y) in pts:                                            # drop shadow down-right onto the plate
        sx, sy = x0 + x + 1, y0 + y + 1
        if (x + 1, y + 1) not in pts and cv.p[sy, sx] == pl:
            cv.set(sx, sy, P[0])


POTION = """
......kkkk......
.....kLlLLk.....
.....kllLlk.....
......kkkk......
......kCck......
......kCck......
....kkkCckkk....
...kCRRRrrrrk...
..kCRWRrrrrrrk..
..kRWRrrrrrrmk..
..kRRrrrrrrrmk..
..krrrrrrrrmmk..
..krrrrrrrmmmk..
...kmrrrrmmmk...
....kkmmmmkk....
................
"""
KEY = """
................
....kkkk........
...kyYYyk.......
..kyokkoyk......
..kyk..kyk......
..kyk..kyk......
..kyokkoyk......
...kyyyok.......
....kyok........
....kyok........
....kyokkk......
....kyyyyk......
....kyokk.......
....kyyyok......
....kyoook......
.....kkkk.......
"""
ICON_MAP = {"k": I[0], "L": L[3], "l": L[2], "C": GLASS[1], "c": GLASS[0], "R": R[3], "r": R[2], "m": R[1],
            "W": R[4], "y": I[5], "Y": BONE[6], "o": I[3]}


def build():
    rng = np.random.default_rng(38)
    cv = Cv()
    stone(cv, rng, 176, 0, 239, 119)
    stone(cv, rng, 0, 120, 239, 159)
    cv.rect(176, 0, 177, 120, S[0])                               # deep cut along the view edges
    cv.rect(0, 120, 177, 121, S[0])
    for y in range(0, 120, 2):
        cv.set(177, y, S[1])
    # right panel
    box(cv, rng, 178, 2, 237, 27)
    for k in (-14, -7, 7, 14):                                    # compass tape dots: nail heads
        cv.set(208 + k, 9, I[4])
        cv.set(208 + k, 10, I[1])
    box(cv, rng, 178, 30, 237, 91, P[0], brackets=False, tex=False)
    box(cv, rng, 178, 94, 237, 117, tex=False)                   # key area must stay flat 'plate' 
    stamp(cv, 181, 97, KEY, ICON_MAP)
    skull(cv, 221, 99)                                            # carved skull boss on the key plate
    # bottom strip
    box(cv, rng, 2, 122, 39, 157)
    sword(cv, rng, 4, 124)
    trough(cv, 4, 152, 37, 156, O[0])
    box(cv, rng, 42, 122, 237, 145)
    trough(cv, 60, 127, 155, 134, R[0])
    box(cv, rng, 198, 124, 235, 143, P[0], brackets=False)
    stamp(cv, 200, 126, POTION, ICON_MAP)
    box(cv, rng, 42, 147, 237, 158, P[0], brackets=False, straps=False)
    # small skulls bolted on the frame joints, horns of blackened iron
    for x in range(86, 126):                                      # a femur laid along the lip, skull on top
        end = min(x - 86, 125 - x)
        rows = (120, 121, 122, 123, 124) if end < 3 and end != 1 else (121, 122, 123)
        for k, y in enumerate(rows):
            c = BONE[7] if k == 0 else BONE[5] if k == 1 else BONE[3] if k < len(rows) - 1 else BONE[1]
            cv.set(x, y, c)
        cv.set(x + 1, rows[-1] + 1, S[0])
    skull(cv, 101, 120, SKULL_SMALL)
    skull(cv, 37, 144, SKULL_SMALL)
    magic_hud(cv)
    cv.rect(0, 0, 175, 119, 0)                                    # keep the hole exact
    cv.p[120, 0:176][cv.p[120, 0:176] == 0] = S[0]
    return cv.p


# ------------------------------------------------------------------------------------------------ G-40 magic HUD
def recess(cv, x0, y0, x1, y1, fill=None, lit=None):
    """Carved socket: shadowed top/left rim, torch-lit bottom/right lip, dark plate inside."""
    cv.rect(x0, y0, x1, y1, I[0])
    cv.rect(x0 + 1, y0 + 1, x1, y1, S[2] if lit is None else lit)
    cv.rect(x0 + 1, y0 + 1, x1 - 1, y1 - 1, P[0] if fill is None else fill)
    cv.set(x1, y0, S[1])
    cv.set(x0, y1, S[1])


def magic_hud(cv):
    """MAGIA_Y_TESOROS 5.2: MP channel (60,137,96,7), 4 status sockets 8x8, belt glow ring. Same boxes as before."""
    MP = NAMED["mp"]
    trough(cv, 60, 137, 155, 143, MP[0])
    cv.rect(180, 96, 235, 115, P[1])                              # the key-plate skull gives way to the sockets
    cv.rect(212, 96, 235, 115, P[1])
    for x, y in ((214, 97), (224, 97), (214, 107), (224, 107)):
        recess(cv, x - 1, y - 1, x + 8, y + 8)
    stamp(cv, 181, 97, KEY, ICON_MAP)
    belt = NAMED["belt"][0]
    for x in range(200, 234):
        for y in (126, 141):
            if cv.p[y, x] == P[0]:
                cv.p[y, x] = belt
    for y in range(126, 142):
        for x in (200, 233):
            if cv.p[y, x] == P[0]:
                cv.p[y, x] = belt


# ------------------------------------------------------------------------------------------------ G-40 pause screens
TABS = (("bag", 2, 33), ("spells", 39, 94), ("map", 97, 128))


def slot32(cv, x, y):
    recess(cv, x + 1, y + 1, x + 30, y + 30, lit=S[4])
    cv.rect(x + 2, y + 2, x + 29, y + 2, I[1])                     # inner shadow under the top rim
    cv.rect(x + 2, y + 2, x + 2, y + 29, I[1])


def pause_screen(hud, tab):
    """Full 240x160 palette_bitmap for the pause (Mode 4, UI indices only). The bottom strip is the HUD's, unchanged."""
    rng = np.random.default_rng({"bag": 401, "spells": 402, "map": 403, "altar": 404}[tab])
    cv = Cv()
    cv.p = hud.copy()
    stone(cv, rng, 0, 0, 239, 119)
    cv.rect(176, 0, 177, 120, S[0])
    box(cv, rng, 0, 12, 175, 119, tex=False)
    tabs = (("altar", 2, 113),) if tab == "altar" else TABS
    for name, x0, x1 in tabs:
        if name == tab:                                           # raised plate, open into the page, iron-capped
            box(cv, rng, x0, 0, x1, 13, P[1], brackets=False, straps=False, tex=False)
            cv.rect(x0 + 2, 12, x1 - 2, 13, P[1])
            for x in range(x0, x1 + 1):
                cv.set(x, 0, I[4] if x % 5 else I[5])
                cv.set(x, 1, I[2])
            nail(cv, x0 + 1, 1)
            nail(cv, x1 - 2, 1)
        else:                                                     # sunken slot cut in the stone
            recess(cv, x0, 1, x1, 11, lit=S[3])
    if tab == "bag":
        for j in range(3):
            for i in range(4):
                slot32(cv, 24 + 32 * i, 16 + 32 * j)
        for j in range(1, 3):
            for i in range(1, 4):
                nail(cv, 24 + 32 * i - 1, 16 + 32 * j - 1)
    elif tab == "spells":
        for j in range(2):
            for i in range(5):
                slot32(cv, 8 + 32 * i, 18 + 32 * j)
        recess(cv, 8, 88, 167, 111, lit=S[4])
    elif tab == "map":
        recess(cv, 39, 11, 136, 108, lit=S[4])
        cv.rect(40, 12, 135, 107, P[0])
    else:
        for x in (40, 72, 104):
            slot32(cv, x, 40)
            recess(cv, x + 2, 74, x + 29, 84, lit=S[4])
    box(cv, rng, 178, 2, 237, 61)
    for x, y in ((180, 4), (209, 4), (180, 33), (209, 33)):
        recess(cv, x, y, x + 26, y + 26, lit=S[4])
        cv.rect(x + 1, y + 1, x + 25, y + 1, I[1])
        cv.rect(x + 1, y + 1, x + 1, y + 25, I[1])
    box(cv, rng, 178, 63, 237, 118, tex=False)
    return cv.p


ARROW = """
.......k
......kk
.....kRk
....kRRk
...kRWRk
..kRWRRk
.kRWRRRk
kRWRRRrk
krrrrrrk
.kmrrrrk
..kmrrrk
...kmrrk
....kmrk
.....kmk
......kk
.......k
"""


def build_arrow():
    pal = [(0, 0, 0)] * 16
    pal[0] = (255, 0, 255)
    pal[1], pal[2], pal[3], pal[4], pal[5] = (14, 6, 5), (74, 16, 14), (132, 30, 24), (176, 64, 48), (214, 170, 140)
    cmap = {"k": 1, "m": 2, "r": 3, "R": 4, "W": 5}
    rows = [[cmap.get(ch, 0) for ch in r] for r in ARROW.strip("\n").split("\n")]
    return np.array(rows), pal


def save(path, arr, pal):
    im = Image.fromarray(arr.astype(np.uint8), "P")
    flat = [c for col in pal for c in col]
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.save(path, transparency=0)


def main():
    os.makedirs(OUT, exist_ok=True)
    frame = build()
    assert not ((frame >= 240) & (frame <= 247)).any()
    save(os.path.join(OUT, "ui_frame.png"), frame, PAL)
    arr, apal = build_arrow()
    save(os.path.join(OUT, "ui_arrow.png"), arr, apal)
    print("ui colours used:", sorted(set(np.unique(frame)) - {0}))
    pauses = {}
    for tab in ("bag", "spells", "map", "altar"):
        pz = pause_screen(frame, tab)
        assert not ((pz >= 240) & (pz <= 247)).any() and not (pz == 0).any()
        pauses["pause_" + tab] = pz
        save(os.path.join(OUT, "pause_" + tab + ".png"), pz, PAL)
    if "--apply" not in sys.argv:
        return
    for name in pauses:                                           # pause screens: created only if missing
        dst = os.path.join(HERE, name + ".aseprite")
        if not os.path.exists(dst):
            subprocess.run([ASE, "-b", "--script-param", "in=" + os.path.join(OUT, name + ".png"),
                            "--script-param", "out=" + dst, "--script-param", "fh=160",
                            "--script-param", "tags=", "--script-param", "bg=0", "--script-param", "ncol=256",
                            "--script", os.path.join(HERE, "_seed", "build_aseprite.lua")], check=True,
                           stdout=subprocess.DEVNULL)
        subprocess.run([ASE, "-b", dst, "--sheet-type", "vertical", "--sheet", os.path.join(GFX, name + ".bmp")],
                       check=True, stdout=subprocess.DEVNULL)
        with open(os.path.join(GFX, name + ".json"), "w") as f:
            f.write('{ "type": "palette_bitmap" }' + chr(10))
    for name, fh, ncol in (("ui_frame", 160, 256), ("ui_arrow", 16, 16)):
        dst = os.path.join(HERE, name + ".aseprite")
        if os.path.exists(dst):
            os.remove(dst)
        subprocess.run([ASE, "-b", "--script-param", "in=" + os.path.join(OUT, name + ".png"),
                        "--script-param", "out=" + dst, "--script-param", "fh=%d" % fh,
                        "--script-param", "tags=", "--script-param", "bg=0", "--script-param", "ncol=%d" % ncol,
                        "--script", os.path.join(HERE, "_seed", "build_aseprite.lua")], check=True,
                       stdout=subprocess.DEVNULL)
        subprocess.run([ASE, "-b", dst, "--sheet-type", "vertical", "--sheet", os.path.join(GFX, name + ".bmp")],
                       check=True, stdout=subprocess.DEVNULL)
    print("applied")


if __name__ == "__main__":
    main()
