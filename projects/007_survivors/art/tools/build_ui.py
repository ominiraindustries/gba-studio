"""Art set 2 of 007_survivors: HUD, upgrade card, level-up burst, edge arrow, wave alert, title screen.

    python -I art/tools/build_ui.py seed [names...]   # creates the missing .aseprite files (never overwrites)
    python -I art/tools/build_ui.py palettes          # graphics/text_*.bmp palettes for the text generator
    python -I art/tools/build_ui.py mock DIR          # 240x160 mockups (1x + 3x) built from graphics/ only

Export is shared with set 1: python -I art/tools/build_art.py export
Sources: art/raw/ui/ (PixelLab: card_0.png frame, title_a.png illustration); everything else is drawn here.
"""
import json
import math
import os
import re
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import aseprite_io  # noqa: E402
import build_art as B  # noqa: E402

ART, GFX, RAW = B.ART, B.GFX, os.path.join(B.RAW, "ui")
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(ART)))

# UI palette (shared by every hud_* / card / fx_levelup / logo sprite)
PAL_UI = [B.hexc(c) for c in [
    "#ff00ff",  # 0 transparent
    "#140c1c",  # 1 outline
    "#3b2650",  # 2 purple dark (empty bars, panels)
    "#8a55c8",  # 3 purple (rune gems)
    "#4a2a18",  # 4 brown dark
    "#7a4a24",  # 5 brown
    "#a8703a",  # 6 brown light
    "#d89a2a",  # 7 gold
    "#ffe27a",  # 8 gold light
    "#d8bc8c",  # 9 parchment shadow
    "#f4e6c0",  # 10 parchment
    "#1e5ab0",  # 11 blue dark (XP)
    "#3ea0f0",  # 12 blue
    "#a8f0ff",  # 13 cyan light
    "#d02838",  # 14 red
    "#ffffff",  # 15 white
]]
O, PD, PU, BD, BR, BL, GO, GL, PS, PA, XD, XB, XL, RE, WH = range(1, 16)


def canvas(w, h):
    return [[0] * w for _ in range(h)]


def rect(c, x0, y0, x1, y1, v):
    for y in range(max(0, y0), min(len(c), y1 + 1)):
        for x in range(max(0, x0), min(len(c[0]), x1 + 1)):
            c[y][x] = v


def outline(c, v=O):
    """Add a 1-px outline (4-neighbour) around opaque pixels, inside the canvas."""
    h, w = len(c), len(c[0])
    out = [r[:] for r in c]
    for y in range(h):
        for x in range(w):
            if c[y][x] == 0 and any(0 <= x + dx < w and 0 <= y + dy < h and c[y + dy][x + dx] not in (0, v)
                                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                out[y][x] = v
    return out


def hflip(c):
    return [r[::-1] for r in c]


def vflip(c):
    return [r[:] for r in c[::-1]]


# ---------------------------------------------------------------- HUD bars
def xp_piece(fill):
    """32x8 body piece of the XP bar; fill = 0..32 filled pixels (left to right)."""
    c = canvas(32, 8)
    rect(c, 0, 0, 31, 0, O); rect(c, 0, 7, 31, 7, O)
    rect(c, 0, 1, 31, 1, GL); rect(c, 0, 6, 31, 6, BR)        # gold rim, top lit / bottom shaded
    rect(c, 0, 2, 31, 5, PD)                                   # empty groove
    for x in range(32):
        if x % 8 == 7:
            c[5][x] = O if x >= fill else c[5][x]              # tiny tick marks in the groove
    if fill:
        rect(c, 0, 2, fill - 1, 2, XL); rect(c, 0, 3, fill - 1, 4, XB); rect(c, 0, 5, fill - 1, 5, XD)
        if fill < 32:
            rect(c, fill - 1, 2, fill - 1, 5, WH)              # bright leading edge
    return c


def xp_caps():
    """8x8 end caps of the XP bar: left = blue XP gem in a gold socket, right = gold finial."""
    L = ["..1111..", ".177771.", "1781dd71", "17dbbx71", "17bbxx71", "177xx771", ".177771.", "..1111.."]
    R = ["........", "1111....", "88871...", "777871..", "777871..", "55571...", "1111....", "........"]
    k = {"1": O, "7": GO, "8": GL, "5": BR, "d": XL, "b": XB, "x": XD, ".": 0}
    return [[k[ch] for ch in row] for row in L], [[k[ch] for ch in row] for row in R]


def hp_piece(fill):
    """32x8 health bar under the wizard: 20 px bar centred, inner 18 px; fill 0..18."""
    c = canvas(32, 8)
    rect(c, 6, 2, 25, 6, O)
    rect(c, 7, 3, 24, 5, PD)
    if fill:
        rect(c, 7, 3, 6 + fill, 3, WH if fill > 4 else RE)
        rect(c, 7, 4, 6 + fill, 5, RE)
        rect(c, 7, 5, 6 + fill, 5, BD)
        rect(c, 7, 3, 6 + fill, 3, GL) if fill <= 4 else None
    return c


HEART = ["..........", ".11...11..", "1ww1.1rr1.", "1wrr1rrrr1", "1rrrrrrrd1", ".1rrrrrd1.", "..1rrrd1..",
         "...1rd1...", "....11...."]


def hearts():
    """8x8: full, half, empty."""
    base = [".11.11..", "1wr1rr1.", "1rrrrd1.", "1rrrrd1.", ".1rrd1..", "..1d1...", "...1....", "........"]
    full = [[{"1": O, "w": WH, "r": RE, "d": BD, ".": 0}[ch] for ch in row] for row in base]
    empty = [[{"1": O, "w": PU, "r": PD, "d": PD, ".": 0}[ch] for ch in row] for row in base]
    half = [[full[y][x] if x < 4 else empty[y][x] for x in range(8)] for y in range(8)]
    return [full, half, empty]


def plaque():
    """64x16 dark panel with a gold rim for the timer / level text, split in two 32x16 halves."""
    c = canvas(64, 16)
    rect(c, 2, 0, 61, 15, O); rect(c, 0, 2, 63, 13, O); rect(c, 1, 1, 62, 14, O)
    rect(c, 2, 1, 61, 1, GL); rect(c, 1, 2, 1, 13, GO); rect(c, 62, 2, 62, 13, BR); rect(c, 2, 14, 61, 14, BR)
    rect(c, 2, 2, 61, 13, PD)
    rect(c, 3, 2, 60, 2, PU)                                   # inner top highlight
    for x, y in ((1, 1), (62, 1), (1, 14), (62, 14)):
        c[y][x] = GO
    for x in (4, 59):                                          # small rune studs on the sides
        c[7][x] = PU; c[8][x] = PU
    return [[r[:32] for r in c], [r[32:] for r in c]]


# ---------------------------------------------------------------- slots (pickups palette, with the icons)
def slots():
    """16x16 slot frames with the 8 icons inside: weapon (gold rim) frames 0-8, passive (blue rim) 9-17.
    Frame 0 / 9 = empty slot. Uses the pickups palette (same as icons)."""
    icons_im = Image.open(os.path.join(GFX, "icons.bmp")).convert("P")
    n = icons_im.height // 16
    icons = [[[icons_im.getpixel((x, k * 16 + y)) for x in range(16)] for y in range(16)] for k in range(n)]
    frames = []
    for rim, lit, dark in ((12, 13, 11), (6, 7, 5)):           # PAL_PICKUPS: 11-13 golds, 5-7 blues
        for k in range(-1, n):
            c = canvas(16, 16)
            rect(c, 1, 0, 14, 15, rim); rect(c, 0, 1, 15, 14, rim)
            rect(c, 1, 0, 14, 0, lit); rect(c, 0, 1, 0, 14, lit)
            rect(c, 1, 15, 14, 15, dark); rect(c, 15, 1, 15, 14, dark)
            rect(c, 1, 1, 14, 14, 1)                           # dark inside (outline colour)
            if k >= 0:
                for y in range(1, 15):
                    for x in range(1, 15):
                        if icons[k][y][x]:
                            c[y][x] = icons[k][y][x]
            else:
                rect(c, 6, 7, 9, 8, rim if False else dark)    # faint dash: empty slot
            frames.append(c)
    return frames


# ---------------------------------------------------------------- edge arrow, alert
def arrows():
    """16x16 arrows: gold e, ne, n (shrine / chest) then red e, ne, n (boss / danger). Mirror for the rest."""
    E = ["................", "................", "........1.......", "........11......", "........1h1.....",
         "11111111h@@1....", "1hhhhhhhh@@@1...", "1@@@@@@@@@@@@1..", "1@@@@@@@@@@@@d1.", "1dddddddd@@@d1..",
         "11111111d@@d1...", "........1dd1....", "........1d1.....", "........11......", "........1.......",
         "................"]
    NE = ["................", "................", "......11111111..", "......1hhhhhh1..", ".......1h@@@@1..",
          "........1@@@@1..", ".......1h@@@@1..", "......1h@@@@@1..", ".....1h@@@d1@1..", "....1h@@@d1.11..",
          "...1h@@@d1......", "..1h@@@d1.......", "..1@@@d1........", "...1dd1.........", "....11..........",
          "................"]
    out = []
    for lit, mid, dark in ((GL, GO, BR), (WH, RE, BD)):
        k = {"1": O, "h": lit, "@": mid, "d": dark, ".": 0}
        e = [[k[ch] for ch in row] for row in E]
        ne = [[k[ch] for ch in row] for row in NE]
        n = [[e[x][15 - y] for x in range(16)] for y in range(16)]  # rotate E 90 deg counter-clockwise
        out += [e, ne, n]
    return out


def alert():
    """16x16 '!' warning: red diamond with a white bang, frame 1 = bright (flash)."""
    frames = []
    for bright in (False, True):
        c = canvas(16, 16)
        for y in range(16):
            for x in range(16):
                d = abs(x - 7.5) + abs(y - 7.5)
                if d <= 7.0:
                    c[y][x] = (GL if bright else RE) if d <= 5.6 else O
                    if d > 5.6 and d <= 6.4:
                        c[y][x] = WH if bright else (GL if (x + y) < 15 else BD)
        body = GO if bright else RE
        for y in range(2, 14):
            for x in range(2, 14):
                if c[y][x] in (GL, RE) and abs(x - 7.5) + abs(y - 7.5) <= 5.6:
                    c[y][x] = body
        rect(c, 7, 3, 8, 9, WH if not bright else O)
        rect(c, 7, 11, 8, 12, WH if not bright else O)
        frames.append(c)
    return frames


# ---------------------------------------------------------------- level-up burst
def levelup_burst():
    """64x64 sunburst behind the wizard: 12 rays, 4 frames (grow, then turn)."""
    frames = []
    for f in range(4):
        c = canvas(64, 64)
        grow = (0.45, 0.75, 1.0, 1.0)[f]
        turn = (0, 0, 0, math.pi / 12)[f]
        for y in range(64):
            for x in range(64):
                dx, dy = x - 31.5, y - 31.5
                r = math.hypot(dx, dy)
                a = (math.atan2(dy, dx) + turn) % (2 * math.pi)
                k = a / (2 * math.pi / 12)
                ray = int(k)
                w = abs(k - ray - 0.5) * 2                     # 0 at ray centre, 1 at edge
                length = (30 if ray % 2 == 0 else 21) * grow
                width = 0.55 * (1 - r / max(length, 1)) + 0.12
                if r < length and w < width:
                    c[y][x] = WH if (w < width * 0.35 and r < length * 0.7) else (GL if r < length * 0.85 else GO)
                if r < 7 * grow:
                    c[y][x] = WH
                elif r < 9 * grow and c[y][x] == 0:
                    c[y][x] = GL
        if f >= 1:                                             # sparkles on a ring
            for i in range(8):
                a = i * math.pi / 4 + f * 0.4
                sx, sy = int(31.5 + 26 * math.cos(a)), int(31.5 + 26 * math.sin(a))
                for ddx, ddy, v in ((0, 0, WH), (1, 0, GL), (-1, 0, GL), (0, 1, GL), (0, -1, GL)):
                    if 0 <= sx + ddx < 64 and 0 <= sy + ddy < 64:
                        c[sy + ddy][sx + ddx] = v
        frames.append(c)
    return frames


# ---------------------------------------------------------------- upgrade card (PixelLab frame, cleaned)
def card():
    """64x80 card, frame 0 normal, frame 1 selected. Icon socket centred at (32, 18), 20x20."""
    im = Image.open(os.path.join(RAW, "card_0.png")).convert("RGBA")
    c = canvas(64, 80)
    for y in range(80):
        for x in range(64):
            r, g, b, a = im.getpixel((x, y))
            if a >= 128:
                c[y][x] = B.nearest(PAL_UI, (r, g, b))
    # interior: flood the transparent hole from the centre with parchment
    stack, seen = [(32, 40)], set()
    while stack:
        x, y = stack.pop()
        if (x, y) in seen or not (0 <= x < 64 and 0 <= y < 80):
            continue
        seen.add((x, y))
        if c[y][x] in (0, PA, PS, XL, WH, GL) or (x, y) == (32, 40):
            if c[y][x] == 0 or c[y][x] in (XL, WH):
                c[y][x] = PA
            stack += [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    # parchment: drop stray non-parchment colours inside, keep a soft shadow under the top border
    xs = [x for (x, y) in seen]; ys = [y for (x, y) in seen]
    x0, x1, y0, y1 = min(xs), max(xs), min(ys), max(ys)
    for (x, y) in seen:
        c[y][x] = PS if (y == y0 or x == x0) else PA
    # icon socket: ring of 20x20 at top centre
    cx, cy = 32, 18
    for y in range(cy - 10, cy + 10):
        for x in range(cx - 10, cx + 10):
            d = math.hypot(x + 0.5 - cx, y + 0.5 - cy)
            if (x, y) in seen and 8.6 <= d < 10:
                c[y][x] = PS
    # divider under the name line (y = 46): gold dash with a rune dot
    for x in range(x0 + 6, x1 - 5):
        if (x, 46) in seen:
            c[46][x] = PS
    for x, v in ((31, PU), (32, PU), (30, GO), (33, GO)):
        c[46][x] = v
    normal = c
    sel = [r[:] for r in c]
    remap = {O: GL, BD: GO, BR: GL, BL: GL, PU: XL, PD: PU, PS: PS}
    for y in range(80):
        for x in range(64):
            v = sel[y][x]
            if (x, y) in seen:
                continue
            edge = x in (0, 63) or y in (0, 79) or v == 0
            if v == O and (x in (0, 63) or y in (0, 79) or any(
                    not (0 <= x + dx < 64 and 0 <= y + dy < 80) or c[y + dy][x + dx] == 0
                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))):
                sel[y][x] = WH if (x + y) % 2 == 0 else GL      # glowing outer rim
            elif v in (BD, BR, BL):
                sel[y][x] = {BD: BR, BR: BL, BL: GO}[v]       # lighter wood
            elif v == PU:
                sel[y][x] = XL                                 # rune gems glow cyan
            elif not edge and v == PD:
                sel[y][x] = PU
    for (x, y) in seen:
        if sel[y][x] == PS and not (y == y0 or x == x0):
            continue
    return [normal, sel]


# ---------------------------------------------------------------- logo
GLYPHS = {  # 11x15 ascii glyphs (x = ink) for the "MAGIC & DESTROY" logo, same 3 px stroke as H E X B O U N D
    "M": ["xxx.....xxx", "xxxx...xxxx", "xxxxx.xxxxx", "xxxxxxxxxxx", "xxx.xxx.xxx", "xxx..x..xxx"]
         + ["xxx.....xxx"] * 9,
    "A": [".xxxxxxxxx."] + ["xxxxxxxxxxx"] * 2 + ["xxx.....xxx"] * 3 + ["xxxxxxxxxxx"] * 3 + ["xxx.....xxx"] * 6,
    "G": [".xxxxxxxxx."] + ["xxxxxxxxxxx"] * 2 + ["xxx.....xxx"] + ["xxx........"] * 3 + ["xxx..xxxxxx"] * 2
         + ["xxx.....xxx"] * 3 + ["xxxxxxxxxxx"] * 2 + [".xxxxxxxxx."],
    "I": ["..xxxxxxx.."] * 3 + ["....xxx...."] * 9 + ["..xxxxxxx.."] * 3,
    "C": [".xxxxxxxxx."] + ["xxxxxxxxxxx"] * 2 + ["xxx.....xxx"] + ["xxx........"] * 7 + ["xxx.....xxx"]
         + ["xxxxxxxxxxx"] * 2 + [".xxxxxxxxx."],
    "S": [".xxxxxxxxx."] + ["xxxxxxxxxxx"] * 2 + ["xxx.....xxx"] + ["xxx........"] * 2
         + ["xxxxxxxxxx.", "xxxxxxxxxxx", ".xxxxxxxxxx"] + ["........xxx"] * 2 + ["xxx.....xxx"]
         + ["xxxxxxxxxxx"] * 2 + [".xxxxxxxxx."],
    "T": ["xxxxxxxxxxx"] * 3 + ["....xxx...."] * 12,
    "R": ["xxxxxxxxx..", "xxxxxxxxxx.", "xxxxxxxxxxx"] + ["xxx.....xxx"] * 3
         + ["xxxxxxxxxx.", "xxxxxxxxx..", "xxxxxxxxx..", "xxx...xxx..", "xxx...xxxx.", "xxx....xxx.",
            "xxx....xxxx", "xxx.....xxx", "xxx.....xxx"],
    "Y": ["xxx.....xxx"] * 3 + ["xxxx...xxxx", ".xxxx.xxxx.", "..xxxxxxx..", "...xxxxx..."] + ["....xxx...."] * 8,
    "&": ["...xxxxx.....", "..xxxxxxx....", "..xx...xx....", "..xx...xx....", "..xxxxxxx....", "...xxxxx.....",
          "..xxxxxx..xxx", ".xxxxxxxx.xxx", "xxx..xxxxxxx.", "xxx...xxxxx..", "xxx....xxx...", "xxx...xxxxx..",
          "xxx..xxxxxxx.", ".xxxxxxx.xxxx", "..xxxxx...xxx"],  # 13 wide
}


def letter(ch):
    """11x15 bold letters (the & is 13 wide); 1 = ink."""
    g = canvas(11, 15)
    if ch in GLYPHS:
        rows = GLYPHS[ch]
        assert len(rows) == 15 and all(len(r) == len(rows[0]) for r in rows), ch
        return [[1 if v == "x" else 0 for v in r] for r in rows]
    F = lambda a, b, c_, d: rect(g, a, b, c_, d, 1)
    if ch == "H":
        F(0, 0, 2, 14); F(8, 0, 10, 14); F(0, 6, 10, 8)
    elif ch == "E":
        F(0, 0, 2, 14); F(0, 0, 10, 2); F(0, 6, 8, 8); F(0, 12, 10, 14)
    elif ch == "X":
        for y in range(15):
            t = y * 8 / 14
            F(round(t), y, round(t) + 2, y); F(round(8 - t), y, round(8 - t) + 2, y)
    elif ch == "B":
        F(0, 0, 2, 14); F(0, 0, 8, 2); F(0, 6, 8, 8); F(0, 12, 8, 14); F(8, 1, 10, 6); F(8, 8, 10, 13)
        g[0][9] = 0; g[6][10] = 0; g[8][10] = 0; g[14][9] = 0
        F(9, 1, 9, 1)
    elif ch == "O":
        F(0, 1, 2, 13); F(8, 1, 10, 13); F(1, 0, 9, 2); F(1, 12, 9, 14)
    elif ch == "U":
        F(0, 0, 2, 13); F(8, 0, 10, 13); F(1, 12, 9, 14)
    elif ch == "N":
        F(0, 0, 2, 14); F(8, 0, 10, 14)
        for y in range(15):
            t = round(y * 6 / 14)
            F(2 + t, y, 3 + t, y)
    elif ch == "D":
        F(0, 0, 2, 14); F(0, 0, 7, 2); F(0, 12, 7, 14); F(8, 2, 10, 12); F(7, 1, 8, 1); F(7, 13, 8, 13)
        F(8, 1, 8, 13)
    return g


def logo(lines=("MAGIC &", "DESTROY")):
    """192x(32*rows) logo: bevelled letters (2x), extrusion, dark outline, rune underline (style C via palette).

    One line -> 192x64 (the old HEXBOUND layout); two lines -> 192x96. Each line is centred; a space is 6 px.
    """
    if isinstance(lines, str):
        lines = (lines,)
    W, H = 192, 64 if len(lines) == 1 else 96
    ink = canvas(W, H)
    lw, gap, sw = 22, 2, 6
    adv = lambda ch: sw if ch == " " else (2 * len(GLYPHS[ch][0]) if ch in GLYPHS else lw)
    widths = [sum(adv(ch) for ch in ln) + (len(ln) - 1) * gap for ln in lines]
    tops = [6] if len(lines) == 1 else [6, 43]
    for ln, top, total in zip(lines, tops, widths):
        x = (W - total) // 2
        for ch in ln:
            if ch == " ":
                x += sw + gap
                continue
            g = letter(ch)
            for y in range(15):
                for gx in range(len(g[0])):
                    if g[y][gx]:
                        for sy in range(2):
                            for sx in range(2):
                                ink[top + y * 2 + sy][x + gx * 2 + sx] = 1
            x += adv(ch) + gap
    c = canvas(W, H)
    # extrusion (3 px down-right)
    for d in (3, 2, 1):
        for y in range(H):
            for x in range(W):
                if ink[y][x] and y + d < H and x + d // 2 < W and not ink[y + d][x + d // 2]:
                    c[y + d][x + d // 2] = PD if d == 3 else PU
    # face: vertical gradient per line + highlight on top edge
    for top in tops:
        for y in range(top, top + 30):
            for x in range(W):
                if ink[y][x]:
                    t = (y - top) / 30
                    v = GL if t < 0.38 else (GO if t < 0.72 else BL)
                    if y > 0 and not ink[y - 1][x]:
                        v = WH
                    elif y + 1 < H and not ink[y + 1][x]:
                        v = BR
                    c[y][x] = v
        # horizontal sheen band (bevel)
        for x in range(W):
            if ink[top + 13][x] and c[top + 13][x] == GO:
                c[top + 13][x] = GL
    # rune underline under the widest line: bar with a central hex gem
    total = max(widths)
    ox = (W - total) // 2
    uy = tops[-1] + 39 if len(lines) == 1 else tops[-1] + 37
    for x in range(ox + 8, ox + total - 8):
        c[uy][x] = GO; c[uy + 1][x] = BR
    cx = W // 2
    gem = ["...111...", "..13331..", ".1333331.", "1333f3331", "1333333d1", ".133333d1", "..1ddd1..",
           "...111..."]
    km = {"1": O, "3": PU, "f": WH, "d": PD, ".": None}
    for y, row in enumerate(gem):
        for x, ch in enumerate(row):
            if km[ch] is not None:
                c[uy - 3 + y][cx - 4 + x] = km[ch]
    for sx in (ox + 4, ox + total - 6):                        # small diamonds at the bar ends
        for dx, dy in ((0, 0), (1, 0), (0, 1), (1, 1)):
            c[uy + dy][sx + dx] = GL
    return outline(c)


# ---------------------------------------------------------------- title background (8 bpp)
def title_bg():
    im = Image.open(os.path.join(RAW, "title_a.png")).convert("RGB")
    assert im.size == (240, 160), im.size
    q = im.quantize(colors=63, method=Image.MEDIANCUT, dither=Image.NONE)
    p = q.getpalette()[:63 * 3]
    cols = [tuple(p[i:i + 3]) for i in range(0, len(p), 3)]
    order = sorted(range(len(cols)), key=lambda i: sum(cols[i]))
    remap = {old: new + 1 for new, old in enumerate(order)}
    pal = [(255, 0, 255)] + [cols[i] for i in order]
    pal += [(0, 0, 0)] * (256 - len(pal))
    W, H = 256, 256
    pix = canvas(W, H)
    darkest = 1
    for y in range(H):
        for x in range(W):
            pix[y][x] = remap[q.getpixel((x, y))] if x < 240 and y < 160 else darkest
    return pal, pix


# ---------------------------------------------------------------- seed
def specs():
    xp = [xp_piece(f) for f in range(0, 33, 2)]
    capl, capr = xp_caps()
    hp = [hp_piece(f) for f in range(0, 19, 2)]
    pl = plaque()
    ar = arrows()
    return {
        "hud_xp": ((32, 8), PAL_UI, [("fill", xp, 100)]),
        "hud_xp_cap": ((8, 8), PAL_UI, [("left", [capl], 100), ("right", [capr], 100)]),
        "hud_hp": ((32, 8), PAL_UI, [("fill", hp, 100)]),
        "hud_heart": ((8, 8), PAL_UI, [("full", hearts()[:1], 100), ("half", hearts()[1:2], 100),
                                       ("empty", hearts()[2:], 100)]),
        "hud_plaque": ((32, 16), PAL_UI, [("left", [pl[0]], 100), ("right", [pl[1]], 100)]),
        "hud_slots": ((16, 16), B.PAL_PICKUPS, [("weapon", slots()[:9], 100), ("passive", slots()[9:], 100)]),
        "hud_arrow": ((16, 16), PAL_UI, [("gold", ar[:3], 100), ("red", ar[3:], 100)]),
        "hud_alert": ((16, 16), PAL_UI, [("blink", alert(), 150)]),
        "fx_levelup": ((64, 64), PAL_UI, [("burst", levelup_burst(), 60)]),
        "card": ((64, 80), PAL_UI, [("normal", card()[:1], 100), ("selected", card()[1:], 100)]),
        "logo": ((192, 96), PAL_UI, [("logo", [logo()], 100)]),
    }


def seed(only):
    for name, ((w, h), pal, tags) in specs().items():
        if only and name not in only:
            continue
        path = os.path.join(ART, name + ".aseprite")
        if os.path.exists(path):
            print("keep (exists):", path); continue
        frames, durs, tg = [], [], []
        for tag, frs, ms in tags:
            a = len(frames)
            for fr in frs:
                assert len(fr) == h and len(fr[0]) == w, (name, tag)
                frames.append(fr); durs.append(ms)
            tg.append((tag, a, len(frames) - 1))
        p = list(pal) + [(0, 0, 0)] * (16 - len(pal))
        aseprite_io.write_aseprite(path, w, h, p, frames, durs, tg)
        print("seeded", name, len(frames), "frames")
    path = os.path.join(ART, "title_bg.aseprite")
    if (not only or "title_bg" in only) and not os.path.exists(path):
        pal, pix = title_bg()
        aseprite_io.write_aseprite(path, 256, 256, pal, [pix], [100], [])
        print("seeded title_bg")


# ---------------------------------------------------------------- text palettes for bn::sprite_text_generator
TEXT_PALETTES = {  # name -> (glyph colour = font index 14, shadow = font index 12)
    "text_ink": ("#4a2a18", "#d8bc8c"),    # dark brown ink on the card parchment (shadow blends in)
    "text_gold": ("#ffe27a", "#4a2a18"),   # "LEVEL UP!", level number
    "text_red": ("#ff5060", "#140c1c"),    # warnings, low HP
}


def text_palettes():
    """graphics/text_*.bmp (sprite_palette): Butano font palette with glyph/shadow recoloured.
    Use: text_generator.set_palette_item(bn::sprite_palette_items::text_ink)."""
    base = os.path.join(ROOT, "engine", "butano", "common", "graphics", "common_variable_8x8_font.bmp")
    fp = Image.open(base).getpalette()[:48]
    fp = [tuple(fp[i:i + 3]) for i in range(0, 48, 3)]
    for name, (glyph, shadow) in TEXT_PALETTES.items():
        pal = list(fp)
        pal[14], pal[12] = B.hexc(glyph), B.hexc(shadow)
        pix = [[(x + (8 if y >= 4 else 0)) % 16 for x in range(8)] for y in range(8)]
        aseprite_io.write_bmp4(os.path.join(GFX, name + ".bmp"), 8, 8, pal, pix)
        with open(os.path.join(GFX, name + ".json"), "w") as f:
            json.dump({"type": "sprite_palette", "bpp_mode": "bpp_4"}, f, indent=4)
        print("palette", name)


# ---------------------------------------------------------------- mockups (graphics/ only)
def font():
    base = os.path.join(ROOT, "engine", "butano", "common")
    im = Image.open(os.path.join(base, "graphics", "common_variable_8x8_font.bmp")).convert("P")
    hdr = open(os.path.join(base, "include", "common_variable_8x8_sprite_font.h"), encoding="utf-8").read()
    widths = [int(m) for m in re.findall(r"^\s*(\d+),\s*//", hdr, re.M)]
    return im, widths


def text(scene, s, x, y, color=(255, 255, 255), center=False, shadow=(20, 12, 28)):
    im, widths = FONT
    tw = sum(widths[ord(ch) - 32] for ch in s)
    if center:
        x -= tw // 2
    for ch in s:
        k = ord(ch) - 32
        for yy in range(8):
            for xx in range(8):
                v = im.getpixel((xx, (k - 1) * 8 + yy)) if k > 0 else 0
                if v == 14 and 0 <= x + xx < 240 and 0 <= y + yy < 160:   # 14 = glyph, 12 = shadow; strip starts at '!'
                    scene.putpixel((x + xx, y + yy), color + (255,))
                elif v == 12 and 0 <= x + xx < 240 and 0 <= y + yy < 160:
                    None if shadow is None else scene.putpixel((x + xx, y + yy), shadow + (255,))
        x += widths[k]
    return tw


def frames(name):
    return B.load_bmp_frames(name)


def mock(out):
    global FONT
    FONT = font()
    os.makedirs(out, exist_ok=True)
    tmp = os.path.join(out, "_contact")
    B.contact(tmp)
    base = Image.open(os.path.join(tmp, "contact_1x.png")).convert("RGBA")
    xp, cap, hp, heart, pl, sl, ar, al = (frames(n) for n in ("hud_xp", "hud_xp_cap", "hud_hp", "hud_heart",
                                                               "hud_plaque", "hud_slots", "hud_arrow", "hud_alert"))

    def hud(scene, xp_px):
        put = lambda im, x, y: scene.alpha_composite(im, (x, y))
        for i in range(7):                                    # XP bar: 7 pieces of 32 px from x=4
            f = max(0, min(32, xp_px - i * 32)) // 2
            put(xp[f], 4 + i * 32, 2)
        put(cap[0], 1, 2); put(cap[1], 4 + 7 * 32 - 4, 2)
        put(pl[0], 88, 11); put(pl[1], 120, 11)
        text(scene, "04:59", 120, 15, center=True)
        text(scene, "LV 7", 206, 15, (255, 226, 122))
        for i, k in enumerate((1, 2, 3)):
            put(sl[k], 2 + i * 17, 13)
        for i, k in enumerate((4, 5)):
            put(sl[9 + k], 2 + i * 17, 30)
        for i, k in enumerate((0, 0, 1)):
            put(heart[k], 200 + i * 9, 150)
        put(hp[7], 104, 96)                                   # under the wizard (wizard at 104,64)
        put(ar[0], 222, 72); put(ar[4], 4, 140)               # gold arrow right, red arrow up (flipped below)
        put(al[0], 112, 142)
    s1 = base.copy()
    hud(s1, 150)
    ar_up_red = ar[5]
    s1.alpha_composite(ar_up_red, (60, 146))
    save(s1, out, "mock_hud")

    # level up: dimmed scene, burst behind the wizard, 3 cards
    s2 = base.copy()
    dim = Image.new("RGBA", s2.size, (16, 8, 32, 150))
    s2.alpha_composite(dim)
    hud(s2, 232)
    s2.alpha_composite(frames("fx_levelup")[2], (88, 48))
    s2.alpha_composite(frames("wizard")[0], (104, 64))
    cards = frames("card"); cbase = frames("card_base"); icons = frames("icons")
    text(s2, "LEVEL UP!", 120, 30, (255, 226, 122), center=True)
    text(s2, "PICK ONE", 120, 40, center=True)
    names = (("Spark|Bolt", "Lv 2", "+1 bolt"), ("Frost|Ring", "NEW!", "Freeze"), ("Swift|Boots", "Lv 1", "Faster"))
    for i, (ic, (n1, n2, n3)) in enumerate(zip((0, 2, 3), names)):
        x = 12 + i * 76; y = 58 if i != 1 else 54
        st = 1 if i == 1 else 0
        s2.alpha_composite(cards[st], (x, y)); s2.alpha_composite(cbase[st], (x, y + 48))
        s2.alpha_composite(icons[ic], (x + 24, y + 10))
        for j, part in enumerate(n1.split('|')):
            text(s2, part, x + 32, y + 28 + j * 9, (40, 20, 24), center=True, shadow=None)
        text(s2, n2, x + 32, y + 50, (138, 85, 200) if n2 == "NEW!" else (74, 42, 24), center=True, shadow=None)
        text(s2, n3, x + 32, y + 60, (74, 42, 24), center=True, shadow=None)
    save(s2, out, "mock_levelup")

    # title
    t = Image.open(os.path.join(GFX, "title_bg.bmp")).convert("RGBA").crop((0, 0, 240, 160))
    lg = frames("logo")
    for k, im in enumerate(lg):
        t.alpha_composite(im, (24 + (k % 3) * 64, 4 + (k // 3) * 32))
    text(t, "PRESS START", 120, 148, center=True)
    save(t, out, "mock_title")


def save(im, out, name):
    im.save(os.path.join(out, name + "_1x.png"))
    im.resize((720, 480), Image.NEAREST).save(os.path.join(out, name + "_3x.png"))
    print("->", os.path.join(out, name + "_3x.png"))


if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        seed(sys.argv[2:])
    elif cmd == "palettes":
        text_palettes()
    elif cmd == "mock":
        mock(sys.argv[2])
