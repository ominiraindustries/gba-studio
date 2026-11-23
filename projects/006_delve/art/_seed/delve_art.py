"""Seed generator for DELVE art (block 1).

Writes indexed PNG seeds to art/_seed/out/. build_aseprite.lua turns each seed into art/<name>.aseprite
ONLY if that .aseprite does not exist yet (the .aseprite is the user's source of truth afterwards).
Run: python -I delve_art.py   (from any folder)
"""
import math
import os
import random

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")
CONCEPTS = os.path.join(HERE, "concepts")


def hx(s):
    return tuple(int(s[i:i + 2], 16) for i in (1, 3, 5))


# ---------------------------------------------------------------- environment palette (indices 0..63)
ENV = {}
ENV_LIST = []


def _env(name, colors):
    ENV[name] = []
    for c in colors:
        ENV[name].append(len(ENV_LIST))
        ENV_LIST.append(hx(c))


_env("k", ["#000000", "#0b0807"])                                               # 0 black (bitmap backdrop), void
_env("s", ["#16100e", "#241b17", "#352a23", "#4a3c32", "#615043", "#7b6654",   # warm torch-lit stone
           "#987f68", "#b89c80"])
_env("m", ["#1a2610", "#2c4118", "#466822", "#6a8f2e", "#98b847"])            # moss
_env("w", ["#1c0f08", "#33190d", "#4e2a14", "#6c3d1c", "#8b5427", "#ab6f33",   # wood
           "#c98f4a"])
_env("i", ["#111317", "#23262c", "#3a3e46", "#5a5f69", "#878d98", "#b7bcc4"])  # iron
_env("t", ["#7a2a0c", "#c2510f", "#ee8a1c", "#ffc44a", "#fff1b0"])            # flame / glow
_env("b", ["#6e665a", "#a59a86", "#d6cbb2", "#f1e9d4"])                       # bone
_env("c", ["#3a0f12", "#6a1a1c", "#972b25"])                                  # cloth (banner)
assert len(ENV_LIST) <= 64, len(ENV_LIST)

# ---------------------------------------------------------------- UI palette (indices 192..255)
UI = {}
UI_LIST = []


def _ui(name, colors):
    UI[name] = []
    for c in colors:
        UI[name].append(192 + len(UI_LIST))
        UI_LIST.append(hx(c))


_ui("s", ["#110d0c", "#1c1614", "#28201c", "#372c26", "#4a3d34", "#625142"])   # dark stone
_ui("i", ["#0b0d10", "#1a1e24", "#2d323a", "#465059", "#6b7480", "#9da6b1"])   # iron trim
_ui("g", ["#3a2610", "#6b4a1e", "#a67c35", "#dcb162"])                         # brass
_ui("p", ["#07080a", "#121519"])                                               # recessed plates
# HUD option A (G-32): 210.. ; 240..247 belong to the engine minimap colours (tools/build_view.py)
_ui("r", ["#2e0f10", "#5c1416", "#a8221f", "#e0503c", "#ffb09a"])              # HP: empty, dark, red, hi, pale
_ui("o", ["#2a2114", "#fff0b8"])                                               # reload: empty, glint (fill = brass)
_ui("b", ["#c9d1da", "#eef2f6"])                                               # blade steel light, edge
_ui("l", ["#2a170c", "#4e2c16", "#7a4a26"])                                    # leather grip / cork
_ui("c", ["#5d7f92", "#9fc3d4"])                                               # glass
assert len(UI_LIST) <= 48, len(UI_LIST)


def full_palette():
    pal = [(0, 0, 0)] * 256
    for i, c in enumerate(ENV_LIST):
        pal[i] = c
    for i, c in enumerate(UI_LIST):
        pal[192 + i] = c
    return pal


class Canvas:
    def __init__(self, w, h, fill=0):
        self.w, self.h = w, h
        self.p = [[fill] * w for _ in range(h)]

    def get(self, x, y, wrap=True):
        if wrap:
            return self.p[y % self.h][x % self.w]
        return self.p[y][x]

    def set(self, x, y, c, wrap=True):
        if wrap:
            self.p[y % self.h][x % self.w] = c
        elif 0 <= x < self.w and 0 <= y < self.h:
            self.p[y][x] = c

    def rect(self, x0, y0, x1, y1, c, wrap=True):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.set(x, y, c, wrap)

    def copy(self):
        n = Canvas(self.w, self.h)
        n.p = [r[:] for r in self.p]
        return n

    def save(self, name, palette):
        img = Image.new("P", (self.w, self.h))
        flat = []
        for c in palette:
            flat += list(c)
        flat += [0] * (768 - len(flat))
        img.putpalette(flat)
        img.putdata([v for r in self.p for v in r])
        os.makedirs(OUT, exist_ok=True)
        img.save(os.path.join(OUT, name + ".png"), transparency=0 if name.startswith(("mon_", "prop_", "ui_", "tex_bars")) else None) \
            if name.startswith(("mon_", "prop_", "ui_", "tex_bars")) else img.save(os.path.join(OUT, name + ".png"))
        return img


S = ENV["s"]
M = ENV["m"]
W = ENV["w"]
I = ENV["i"]
T = ENV["t"]
B = ENV["b"]
C = ENV["c"]
K = ENV["k"]


def shade(ramp, c, d):
    """Move colour c (an index inside ramp) d steps along that ramp."""
    j = ramp.index(c)
    return ramp[max(0, min(len(ramp) - 1, j + d))]


# ---------------------------------------------------------------- stone blocks (wall, floor, ceiling share it)
def blocks(cv, rows, row_h, tones, rng, mortar=(S[0], S[1]), chips=True, cracks=True, ramp=S, light=True):
    """rows: list of cyclic cut positions per row. Every feature is >= 2 px or a clean line (no 1 px noise)."""
    info = []
    for r, cuts in enumerate(rows):
        y0 = r * row_h
        n = len(cuts)
        for k in range(n):
            x0 = cuts[k]
            x1 = cuts[(k + 1) % n] + (cv.w if k == n - 1 else 0)
            base = rng.choice(tones)
            bw = x1 - x0 - 2            # inner width (2 px mortar on the right)
            bh = row_h - 2              # inner height (2 px mortar at the bottom)
            for yy in range(bh):
                for xx in range(bw):
                    c = base
                    if light:
                        if yy == 0:
                            c = shade(ramp, base, 2)
                        elif yy == 1 or xx == 0:
                            c = shade(ramp, base, 1)
                        elif yy >= bh - 2 or xx == bw - 1:
                            c = shade(ramp, base, -1)
                        # rounded corners
                        if (xx in (0, bw - 1)) and (yy in (0, bh - 1)):
                            c = shade(ramp, base, -1)
                    cv.set(x0 + xx, y0 + yy, c)
            # big facets: soft 2-tone patches, at least 4x3
            for _ in range(rng.randint(1, 2)):
                fw, fh = rng.randint(4, max(4, bw // 2)), rng.randint(3, max(3, bh // 2 - 1))
                fx, fy = x0 + rng.randint(2, max(2, bw - fw - 2)), y0 + rng.randint(2, max(2, bh - fh - 3))
                d = rng.choice((1, -1))
                for yy in range(fh):
                    for xx in range(fw):
                        if (xx in (0, fw - 1)) and (yy in (0, fh - 1)):
                            continue
                        cv.set(fx + xx, fy + yy, shade(ramp, base, d))
                # facet rim: lit top edge or shadowed bottom edge
                if d < 0:
                    for xx in range(1, fw - 1):
                        cv.set(fx + xx, fy + fh, shade(ramp, base, 1))
            if chips and rng.random() < 0.35:
                cx = x0 + (bw - 4 if rng.random() < 0.5 else 1)
                cy = y0 + (bh - 4 if rng.random() < 0.5 else 1)
                cv.rect(cx, cy, cx + 2, cy + 2, shade(ramp, base, -2))
                cv.rect(cx + 1, cy + 1, cx + 2, cy + 2, mortar[1])
            if cracks and rng.random() < 0.25 and bw > 12:
                x = x0 + rng.randint(4, bw - 6)
                y = y0
                while y < y0 + bh - 2:
                    cv.set(x, y, mortar[1])
                    cv.set(x + 1, y, shade(ramp, base, 1))
                    y += 1
                    if rng.random() < 0.4:
                        x += rng.choice((-1, 1))
            # mortar
            for yy in range(row_h):
                cv.set(x1 - 2, y0 + yy, mortar[1])
                cv.set(x1 - 1, y0 + yy, mortar[0])
            for xx in range(x0, x1):
                cv.set(xx, y0 + row_h - 2, mortar[1])
                cv.set(xx, y0 + row_h - 1, mortar[0])
            info.append((x0, y0, x1, y0 + row_h))
    return info


def moss_blob(cv, cx, cy, rx, ry, rng, drip=True):
    for y in range(-ry, ry + 1):
        for x in range(-rx, rx + 1):
            if (x / (rx + 0.5)) ** 2 + (y / (ry + 0.5)) ** 2 <= 1:
                c = M[2] if y <= 0 else M[1]
                if y < -ry // 2:
                    c = M[3]
                cv.set(cx + x, cy + y, c)
    # highlight clumps 2x1 on top
    for _ in range(max(1, rx // 2)):
        x = cx + rng.randint(-rx + 1, rx - 2)
        cv.set(x, cy - ry, M[4])
        cv.set(x + 1, cy - ry, M[4])
    if drip:
        for _ in range(rng.randint(0, 2)):
            x = cx + rng.randint(-rx + 1, rx - 1)
            ln = rng.randint(2, 5)
            for y in range(ln):
                cv.set(x, cy + ry + y, M[1])
                cv.set(x + 1, cy + ry + y, M[0] if y == ln - 1 else M[1])


WALL_ROWS = [[6, 30, 50], [18, 40, 62], [2, 28, 46], [14, 36, 56]]


def tex_wall_stone(seed=7):
    rng = random.Random(seed)
    cv = Canvas(64, 64)
    blocks(cv, WALL_ROWS, 16, [S[4], S[4], S[5], S[3]], rng)
    # moss growing out of the mortar joints, mostly low on the wall
    for (mx, my, rx, ry) in [(12, 46, 5, 2), (40, 30, 4, 2), (54, 62, 6, 2), (26, 14, 3, 1)]:
        moss_blob(cv, mx, my, rx, ry, rng)
    return cv


def tex_floor(seed=11):
    rng = random.Random(seed)
    cv = Canvas(64, 64)
    blocks(cv, [[0, 34], [18, 48]], 32, [S[3], S[4], S[3]], rng, cracks=True)
    for (mx, my, rx, ry) in [(33, 31, 3, 1), (2, 62, 4, 1)]:
        moss_blob(cv, mx, my, rx, ry, rng, drip=False)
    # stains: 3x2 dark blobs
    for (x, y) in [(8, 10), (44, 44), (26, 50)]:
        cv.rect(x, y, x + 3, y + 1, S[2])
        cv.rect(x + 1, y + 2, x + 2, y + 2, S[2])
    return cv


def tex_ceiling(seed=5):
    rng = random.Random(seed)
    cv = Canvas(64, 64)
    blocks(cv, [[4, 26, 46], [14, 36, 58], [0, 22, 44], [10, 32, 54]], 16, [S[2], S[3], S[2]], rng,
           chips=False, cracks=False)
    # wooden beam crossing the view (seen from below), y 22..37
    y0, y1 = 22, 37
    for y in range(y0 - 3, y0):                       # shadow above (soot)
        for x in range(64):
            cv.set(x, y, S[1] if y == y0 - 1 else S[2] if cv.get(x, y) not in (S[0], S[1]) else cv.get(x, y))
    for y in range(y0, y1 + 1):
        for x in range(64):
            r = y - y0
            c = W[3]
            if r == 0:
                c = W[1]
            elif r == 1:
                c = W[2]
            elif r >= 14:
                c = W[5] if r == 14 else W[1]        # lit lower edge facing the torch, then dark lip
            elif r in (5, 10) and (x // 9) % 3 != 0:
                c = W[2]                              # long grain lines
            elif r in (3, 12):
                c = W[4]
            cv.set(x, y, c)
    for y in range(y1 + 1, y1 + 3):                   # shadow under the beam
        for x in range(64):
            cv.set(x, y, S[1])
    # iron straps with rivets wrapping the beam
    for sx in (10, 42):
        for y in range(y0 - 1, y1 + 2):
            for x in range(sx, sx + 4):
                c = I[2]
                if x == sx:
                    c = I[3]
                elif x == sx + 3:
                    c = I[1]
                cv.set(x, y, c)
        for ry in (y0 + 3, y1 - 4):
            cv.rect(sx + 1, ry, sx + 2, ry + 1, I[4])
            cv.set(sx + 2, ry + 1, I[3])
    return cv


# ---------------------------------------------------------------- arch helpers (door, stairs, arch)
def arch_masks(cx, top, r_out, ring, x_floor=63):
    """Returns functions: inside(x,y) of the opening and ring(x,y) for the stone ring + jambs."""
    def d(x, y):
        return math.hypot(x + 0.5 - cx, y + 0.5 - top)

    def opening(x, y):
        if y < top:
            return d(x, y) <= r_out - ring
        return abs(x + 0.5 - cx) <= r_out - ring

    def outer(x, y):
        if y < top:
            return d(x, y) <= r_out
        return abs(x + 0.5 - cx) <= r_out

    return opening, outer, d


def draw_arch_ring(cv, cx, top, r_out, ring, segs=9):
    opening, outer, d = arch_masks(cx, top, r_out, ring)
    for y in range(64):
        for x in range(64):
            if outer(x, y) and not opening(x, y):
                if y < top:
                    ang = math.atan2(top - (y + 0.5), x + 0.5 - cx)          # 0..pi
                    seg = ang / math.pi * segs
                    frac = seg - math.floor(seg)
                    keystone = int(seg) == segs // 2
                    rr = d(x, y)
                    if frac < 0.12 or frac > 0.92:
                        c = S[1]
                    elif rr > r_out - 1.2:
                        c = S[2]
                    elif rr < r_out - ring + 1.2:
                        c = S[7] if keystone else S[6]
                    else:
                        c = S[6] if keystone else S[5]
                else:
                    side = x < cx
                    inner = (x == int(cx - r_out + ring) - 1) if side else (x == int(cx + r_out - ring))
                    edge = (x == int(cx - r_out)) if side else (x == int(cx + r_out) - 1)
                    yy = (y - top) % 10
                    if yy >= 8:
                        c = S[1] if yy == 8 else S[2]
                    elif inner:
                        c = S[6]
                    elif edge:
                        c = S[2]
                    elif yy == 0:
                        c = S[6]
                    else:
                        c = S[5]
                cv.set(x, y, c, wrap=False)
    # dark outline just outside the ring so it separates from the wall
    for y in range(64):
        for x in range(64):
            if not outer(x, y):
                for dx, dy in ((1, 0), (-1, 0), (0, 1)):
                    xx, yy = x + dx, y + dy
                    if 0 <= xx < 64 and 0 <= yy < 64 and outer(xx, yy) and not opening(xx, yy):
                        cv.set(x, y, S[1], wrap=False)
                        break
    return opening


def door_leaf(cv, opening, x0, x1, y_top):
    planks = list(range(x0, x1 + 1, 8))
    for y in range(y_top, 64):
        for x in range(x0, x1 + 1):
            if not opening(x, y):
                continue
            k = (x - x0) // 8
            px = (x - x0) % 8
            base = W[4] if k % 2 == 0 else W[3]
            c = base
            if px == 0:
                c = W[5] if base == W[4] else W[4]
            elif px == 7:
                c = W[1]
            elif px == 6:
                c = W[2]
            elif px == 3 and (y + k * 7) % 23 < 12:
                c = shade(W, base, -1)                 # long grain streak
            # shadow under the arch ring
            if not opening(x, y - 3):
                c = shade(W, c, -2)
            cv.set(x, y, c, wrap=False)
    # iron bands with rivets
    for by in (22, 46):
        for y in range(by, by + 4):
            for x in range(x0, x1 + 1):
                if opening(x, y):
                    cv.set(x, y, I[3] if y == by else I[0] if y == by + 3 else I[2], wrap=False)
        for x in range(x0 + 3, x1, 8):
            cv.rect(x, by + 1, x + 1, by + 2, I[4], wrap=False)
            cv.set(x + 1, by + 2, I[3], wrap=False)
    return planks


def tex_door():
    cv = tex_wall_stone()
    cx, top, r_out, ring = 32, 26, 24, 4
    opening = draw_arch_ring(cv, cx, top, r_out, ring)
    door_leaf(cv, opening, 12, 51, 0)
    # ring handle
    hx_, hy = 41, 36
    cv.rect(hx_ - 2, hy - 6, hx_ + 1, hy - 3, I[2], wrap=False)
    cv.rect(hx_ - 1, hy - 5, hx_, hy - 4, I[4], wrap=False)
    for y in range(-5, 6):
        for x in range(-5, 6):
            r = math.hypot(x, y)
            if 3.0 <= r <= 4.6:
                c = I[4] if (x + y) < -2 else I[1] if (x + y) > 2 else I[3]
                cv.set(hx_ + x, hy + 2 + y, c, wrap=False)
            elif 4.6 < r <= 5.6 and y > 0:
                cv.set(hx_ + x, hy + 2 + y, W[1], wrap=False)   # cast shadow
    # threshold
    for y in range(60, 64):
        for x in range(8, 56):
            cv.set(x, y, S[6] if y == 60 else S[4] if y < 63 else S[2], wrap=False)
    return cv


def tex_door_open():
    cv = tex_wall_stone()
    cx, top, r_out, ring = 32, 26, 24, 4
    opening = draw_arch_ring(cv, cx, top, r_out, ring)
    for y in range(64):
        for x in range(64):
            if opening(x, y):
                c = K[0]
                if y >= 50:                       # faint floor going in
                    c = S[1] if y >= 56 else K[1]
                cv.set(x, y, c, wrap=False)
    # door leaf swung inwards, seen almost edge-on against the left jamb
    for y in range(4, 60):
        for x in range(12, 19):
            if not opening(x, y):
                continue
            px = x - 12
            # leaf narrows towards the far edge (foreshortened)
            if px > 6 - (y < 30) * 1:
                continue
            c = W[3] if px < 2 else W[2] if px < 5 else W[1]
            if px == 0:
                c = W[4]
            cv.set(x, y, c, wrap=False)
        for by in (22, 46):
            if by <= y < by + 3:
                for x in range(12, 18):
                    if opening(x, y):
                        cv.set(x, y, I[3] if y == by else I[2], wrap=False)
    # light spilling on the threshold
    for y in range(60, 64):
        for x in range(8, 56):
            cv.set(x, y, S[6] if y == 60 else S[4] if y < 63 else S[2], wrap=False)
    return cv


def tex_stairs_down():
    cv = tex_wall_stone()
    cx, top, r_out, ring = 32, 28, 26, 4
    opening = draw_arch_ring(cv, cx, top, r_out, ring, segs=11)
    x_in0, x_in1 = int(cx - r_out + ring), int(cx + r_out - ring) - 1
    for y in range(64):
        for x in range(64):
            if opening(x, y):
                cv.set(x, y, K[0], wrap=False)
    # steps: nearest at the bottom, each further one higher, narrower and darker
    steps = [(63, 5, S[6], S[4]), (57, 4, S[5], S[3]), (52, 4, S[4], S[2]), (47, 3, S[3], S[1]),
             (43, 3, S[2], S[1]), (40, 2, S[1], K[1])]
    for k, (yb, h, tread, riser) in enumerate(steps):
        inset = k * 2
        for y in range(yb - h + 1, yb + 1):
            for x in range(x_in0 + inset, x_in1 - inset + 1):
                c = tread if y == yb - h + 1 or (h > 3 and y == yb - h + 2) else riser
                cv.set(x, y, c, wrap=False)
    # side walls of the stairwell converging inward (dark stone)
    for y in range(36, 64):
        k = max(0, (63 - y) // 4)
        w = max(0, 12 - k * 2) if y > 36 else 0
        for x in range(x_in0, x_in0 + min(w, 2 * min(6, (y - 34) // 4 + 1))):
            if cv.get(x, y, False) == K[0]:
                cv.set(x, y, S[1], wrap=False)
        for x in range(x_in1 - min(w, 2 * min(6, (y - 34) // 4 + 1)) + 1, x_in1 + 1):
            if cv.get(x, y, False) == K[0]:
                cv.set(x, y, S[1], wrap=False)
    return cv


# ---------------------------------------------------------------- wall decorations (variants of the stone wall)
def tex_wall_torch(frame=0):
    cv = tex_wall_stone()
    cx = 32
    # soot + warm glow on the stones around the torch (big soft rings, no dithering)
    for y in range(64):
        for x in range(64):
            v = cv.get(x, y, False)
            if v in S:
                r = math.hypot((x - cx) * 0.9, y - 22)
                if r < 12:
                    cv.set(x, y, shade(S, v, 3), wrap=False)
                elif r < 20:
                    cv.set(x, y, shade(S, v, 2), wrap=False)
                elif r < 28 and v not in (S[0], S[1]):
                    cv.set(x, y, shade(S, v, 1), wrap=False)
            if v in M and math.hypot((x - cx) * 0.9, y - 22) < 20:
                cv.set(x, y, shade(M, v, 1), wrap=False)
    # sconce: iron bracket
    cv.rect(cx - 6, 40, cx + 5, 43, I[2], wrap=False)
    cv.rect(cx - 6, 40, cx + 5, 40, I[4], wrap=False)
    cv.rect(cx - 6, 43, cx + 5, 43, I[0], wrap=False)
    cv.rect(cx - 2, 44, cx + 1, 52, I[2], wrap=False)
    cv.rect(cx - 2, 44, cx - 2, 52, I[3], wrap=False)
    cv.rect(cx + 1, 44, cx + 1, 52, I[1], wrap=False)
    cv.rect(cx - 3, 52, cx + 2, 54, I[1], wrap=False)
    cv.rect(cx - 2, 53, cx + 1, 53, I[3], wrap=False)
    # torch handle (wood) in the cup
    cv.rect(cx - 3, 32, cx + 2, 40, W[3], wrap=False)
    cv.rect(cx - 3, 32, cx - 2, 40, W[5], wrap=False)
    cv.rect(cx + 2, 32, cx + 2, 40, W[1], wrap=False)
    cv.rect(cx - 4, 30, cx + 3, 32, W[1], wrap=False)            # charred wrap
    # flame: two frames, big layered tongues
    sway = 1 if frame else -1
    layers = [(T[0], 9.0, 22), (T[1], 7.4, 19), (T[2], 5.6, 15), (T[3], 3.8, 11), (T[4], 2.2, 6)]
    for col, half_w, hgt in layers:
        for dy in range(hgt + (1 if frame else 0)):
            t = dy / (hgt + 1)
            w = half_w * (1 - t ** 1.6)
            off = sway * t * t * 2.5
            y = 30 - dy
            for x in range(int(cx - w + off - 0.5), int(cx + w + off + 0.5)):
                cv.set(x, y, col, wrap=False)
    return cv


def tex_bars():
    """Iron portcullis: index 0 is see-through between the bars."""
    cv = Canvas(64, 64, 0)
    bars_x = [2, 14, 26, 38, 50, 62]
    for bx in bars_x:
        for y in range(64):
            for x in (bx - 2, bx - 1, bx, bx + 1):
                c = I[2]
                if x == bx - 2:
                    c = I[1]
                elif x == bx - 1:
                    c = I[4]
                elif x == bx:
                    c = I[3]
                else:
                    c = I[1]
                cv.set(x, y, c)
    for by in (8, 36):
        for y in range(by, by + 4):
            for x in range(64):
                cv.set(x, y, I[4] if y == by else I[0] if y == by + 3 else I[2])
        for bx in bars_x:
            cv.rect(bx - 1, by + 1, bx, by + 2, I[5])
    # spikes at the bottom
    for bx in bars_x:
        for k in range(5):
            cv.set(bx - 1, 59 + k, I[4] if k < 3 else I[3])
            cv.set(bx, 59 + k, I[2])
    return cv


def tex_wall_lever(up):
    cv = tex_wall_stone()
    cx, cy = 32, 34
    # iron plate with a slot
    for y in range(cy - 12, cy + 13):
        for x in range(cx - 8, cx + 8):
            c = I[2]
            if y == cy - 12 or x == cx - 8:
                c = I[3]
            elif y == cy + 12 or x == cx + 7:
                c = I[0]
            cv.set(x, y, c, wrap=False)
    for (rx, ry) in ((cx - 6, cy - 10), (cx + 4, cy - 10), (cx - 6, cy + 9), (cx + 4, cy + 9)):
        cv.rect(rx, ry, rx + 1, ry + 1, I[4], wrap=False)
    cv.rect(cx - 2, cy - 9, cx + 1, cy + 9, K[0], wrap=False)          # slot
    cv.rect(cx - 2, cy - 9, cx + 1, cy - 9, I[0], wrap=False)
    # pivot
    cv.rect(cx - 3, cy - 2, cx + 2, cy + 2, I[3], wrap=False)
    cv.rect(cx - 2, cy - 1, cx + 1, cy + 1, I[4], wrap=False)
    # handle: thick rod angled up or down to the right, wooden grip and knob at the end
    sgn = -1 if up else 1
    ang = math.radians(60)
    ex, ey = cx + math.cos(ang) * 24, cy + sgn * math.sin(ang) * 24
    for y in range(64):
        for x in range(64):
            vx, vy = ex - cx, ey - cy
            t = max(0.0, min(1.0, ((x + 0.5 - cx) * vx + (y + 0.5 - cy) * vy) / (vx * vx + vy * vy)))
            qx, qy = cx + vx * t, cy + vy * t
            d = math.hypot(x + 0.5 - qx, y + 0.5 - qy)
            if d <= 1.9:
                side = (x + 0.5 - qx) * -1 + (y + 0.5 - qy) * -1      # lit from the upper left
                grip = t > 0.62
                ramp = W if grip else I
                c = (ramp[5] if grip else I[5]) if side > 0.6 else (ramp[4] if grip else I[3]) if side > -0.6                     else (ramp[1] if grip else I[0])
                cv.set(x, y, c, wrap=False)
    kx, ky = int(round(ex)), int(round(ey))
    for y in range(-4, 5):
        for x in range(-4, 5):
            r = math.hypot(x, y)
            if r <= 3.6:
                c = W[6] if x + y < -2 else W[4] if x + y < 2 else W[2]
                if r > 2.9:
                    c = W[0]
                cv.set(kx + x, ky + y, c, wrap=False)
    cv.rect(cx - 3, cy - 3, cx + 2, cy + 2, I[0], wrap=False)
    cv.rect(cx - 2, cy - 2, cx + 1, cy + 1, I[3], wrap=False)
    cv.rect(cx - 2, cy - 2, cx, cy - 2, I[5], wrap=False)
    return cv


def tex_wall_chains():
    cv = tex_wall_stone()
    for (cx, top, ln) in ((20, 6, 36), (44, 6, 30)):
        cv.rect(cx - 3, top - 2, cx + 2, top + 1, I[2], wrap=False)          # wall ring plate
        cv.rect(cx - 3, top - 2, cx + 2, top - 2, I[4], wrap=False)
        for k in range(0, ln, 6):
            y = top + 2 + k
            if (k // 6) % 2 == 0:                       # link seen front: ring 4x6
                cv.rect(cx - 2, y, cx + 1, y + 5, I[3], wrap=False)
                cv.rect(cx - 1, y + 1, cx, y + 4, K[0], wrap=False)
                cv.rect(cx - 2, y, cx - 2, y + 5, I[4], wrap=False)
            else:                                       # link edge-on: 2 px bar
                cv.rect(cx - 1, y, cx, y + 5, I[4], wrap=False)
                cv.set(cx, y + 5, I[2], wrap=False)
        y = top + 2 + ln                                # shackle
        cv.rect(cx - 4, y, cx + 3, y + 5, I[3], wrap=False)
        cv.rect(cx - 2, y + 1, cx + 1, y + 4, K[0], wrap=False)
        cv.rect(cx - 4, y, cx + 3, y, I[5], wrap=False)
    return cv


def tex_wall_crack():
    cv = tex_wall_stone()
    rng = random.Random(3)
    x = 30
    pts = []
    for y in range(4, 60):
        pts.append((x, y))
        if rng.random() < 0.45:
            x += rng.choice((-1, 1))
    for (x, y) in pts:
        cv.set(x - 1, y, S[2], wrap=False)
        cv.set(x, y, K[0], wrap=False)
        cv.set(x + 1, y, K[0] if y % 7 < 4 else S[0], wrap=False)
        cv.set(x + 2, y, S[6], wrap=False)                       # lit lip on the right
    for (bx, by, d) in ((30, 20, -1), (31, 42, 1)):               # side branches
        for k in range(7):
            cv.set(bx + d * k, by + k // 2, K[0], wrap=False)
            cv.set(bx + d * k, by + k // 2 + 1, S[6], wrap=False)
    # a faint draught of air: slightly cooler (moss-free) stones around
    return cv


def tex_wall_dart():
    cv = tex_wall_stone()
    for (hx_, hy) in ((22, 26), (42, 26), (32, 40)):
        cv.rect(hx_ - 3, hy - 3, hx_ + 2, hy + 2, S[3], wrap=False)
        cv.rect(hx_ - 3, hy - 3, hx_ + 2, hy - 3, S[1], wrap=False)
        cv.rect(hx_ - 2, hy - 2, hx_ + 1, hy + 1, K[0], wrap=False)
        cv.rect(hx_ - 2, hy + 2, hx_ + 1, hy + 2, S[6], wrap=False)
    return cv


def tex_wall_banner():
    cv = tex_wall_stone()
    x0, x1 = 18, 45
    cv.rect(x0 - 3, 4, x1 + 3, 7, W[3], wrap=False)                       # pole
    cv.rect(x0 - 3, 4, x1 + 3, 4, W[5], wrap=False)
    cv.rect(x0 - 3, 7, x1 + 3, 7, W[1], wrap=False)
    for y in range(8, 56):
        for x in range(x0, x1 + 1):
            tail = y > 46 and abs(x - (x0 + x1) / 2) < (y - 46) * 1.4
            if tail:
                continue
            c = C[2] if (x - x0) % 9 < 6 else C[1]
            if x in (x0, x1):
                c = C[0]
            if y in (10, 11) or y in (42, 43):
                c = T[2] if y in (10, 42) else T[0]                       # gold trim
            cv.set(x, y, c, wrap=False)
    # emblem: a simple pale tower shape, big blocks
    cx = (x0 + x1) // 2
    cv.rect(cx - 5, 20, cx + 4, 36, B[2], wrap=False)
    cv.rect(cx - 7, 16, cx + 6, 20, B[2], wrap=False)
    for k in range(-7, 7, 4):
        cv.rect(cx + k, 14, cx + k + 1, 15, B[2], wrap=False)
    cv.rect(cx - 2, 28, cx + 1, 36, C[0], wrap=False)
    cv.rect(cx + 4, 20, cx + 4, 36, B[0], wrap=False)
    return cv


def tex_wall_skulls():
    cv = tex_wall_stone()
    x0, x1, y0, y1 = 10, 53, 18, 45
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            c = K[1] if y < y0 + 6 else S[1]
            cv.set(x, y, c, wrap=False)
    cv.rect(x0 - 2, y0 - 2, x1 + 2, y0 - 1, S[6], wrap=False)
    cv.rect(x0 - 2, y1 + 1, x1 + 2, y1 + 3, S[6], wrap=False)
    cv.rect(x0 - 2, y1 + 3, x1 + 2, y1 + 3, S[3], wrap=False)
    for sx in (18, 32, 46):
        skull(cv, sx, 38)
    return cv


def skull(cv, cx, by):
    for y in range(-11, 1):
        for x in range(-6, 6):
            r = ((x + 0.5) / 6.2) ** 2 + ((y + 6) / 6.5) ** 2
            if r <= 1 or (y > -3 and abs(x + 0.5) < 4):
                c = B[2]
                if x <= -4 or y <= -10:
                    c = B[3]
                if x >= 4:
                    c = B[1]
                cv.set(cx + x, by + y, c, wrap=False)
    cv.rect(cx - 4, by - 7, cx - 2, by - 5, K[0], wrap=False)
    cv.rect(cx + 1, by - 7, cx + 3, by - 5, K[0], wrap=False)
    cv.rect(cx - 1, by - 4, cx, by - 3, B[0], wrap=False)
    for x in range(-3, 4, 2):
        cv.set(cx + x, by - 1, B[0], wrap=False)


def tex_arch():
    cv = tex_wall_stone()
    opening = draw_arch_ring(cv, 32, 24, 28, 4, segs=11)
    for y in range(64):
        for x in range(64):
            if opening(x, y):
                cv.set(x, y, 0, wrap=False)       # see-through: the renderer draws what lies behind
    return cv


TEXTURES = {
    "tex_wall_stone": tex_wall_stone, "tex_door": tex_door, "tex_door_open": tex_door_open,
    "tex_floor": tex_floor, "tex_ceiling": tex_ceiling, "tex_stairs_down": tex_stairs_down,
    "tex_bars": tex_bars,
    "tex_wall_lever_up": lambda: tex_wall_lever(True), "tex_wall_lever_down": lambda: tex_wall_lever(False),
    "tex_wall_chains": tex_wall_chains, "tex_wall_crack": tex_wall_crack, "tex_wall_dart": tex_wall_dart,
    "tex_wall_banner": tex_wall_banner, "tex_wall_skulls": tex_wall_skulls, "tex_arch": tex_arch,
}


def env_palette64():
    return ENV_LIST + [(0, 0, 0)] * (64 - len(ENV_LIST))


def save_png(name, rows, palette, transparent=False):
    img = Image.new("P", (len(rows[0]), len(rows)))
    flat = []
    for c in palette:
        flat += list(c)
    img.putpalette(flat)
    img.putdata([v for r in rows for v in r])
    os.makedirs(OUT, exist_ok=True)
    if transparent:
        img.save(os.path.join(OUT, name + ".png"), transparency=0)
    else:
        img.save(os.path.join(OUT, name + ".png"))


def build_textures():
    for name, fn in TEXTURES.items():
        save_png(name, fn().p, env_palette64(), transparent=name in ("tex_bars", "tex_arch"))
    # torch: 2 frames stacked vertically (tag "flame")
    f0, f1 = tex_wall_torch(0).p, tex_wall_torch(1).p
    save_png("tex_wall_torch", f0 + f1, env_palette64())


if __name__ == "__main__":
    import sys
    build_textures()
    if "ui" in sys.argv or "all" in sys.argv:
        import delve_ui
        delve_ui.build()
    print("ok", len(ENV_LIST), "env colours,", len(UI_LIST), "ui colours")
