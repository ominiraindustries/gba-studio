"""Block 6 art (G-58): relic icons, hex icons and the 3 new evolution icons. Simple style-C seeds (dark outline,
3-4 flat colours, neon only on magic and gems) in the `icons` palette, so they share its sprite palette slot.

    python -I art/tools/build_block6.py seed      # creates relics / hex_icons / evo_icons .aseprite if missing
Then: python -I art/tools/build_art.py export

Never overwrites an existing .aseprite (the user retouches them).
"""
import math
import os
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from aseprite_io import write_aseprite  # noqa: E402

TOOLS = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(TOOLS)
GFX = os.path.join(os.path.dirname(ART), "graphics")

# icons palette: 1 outline 2 teal 3 cyan 4 amber 5 slate 6 grey 7 pale 8 purple 9 red 10 magenta 11 maroon
# 12 gold 13 cream 14 pale 15 dark grey
O, TEAL, CYAN, AMBER, SLATE, GREY, PALE, PURPLE, RED, MAG, MAROON, GOLD, CREAM, PALE2, DARK = range(1, 16)


def icons_palette():
    im = Image.open(os.path.join(GFX, "icons.bmp"))
    p = im.getpalette()[:48]
    return [tuple(p[i:i + 3]) for i in range(0, 48, 3)]


def blank(n):
    return [[0] * n for _ in range(n)]


def put(m, x, y, c):
    if 0 <= y < len(m) and 0 <= x < len(m[0]):
        m[y][x] = c


def disc(m, cx, cy, r, c):
    for y in range(len(m)):
        for x in range(len(m[0])):
            if (x + 0.5 - cx) ** 2 + (y + 0.5 - cy) ** 2 <= r * r:
                m[y][x] = c


def rect(m, x0, y0, x1, y1, c):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            put(m, x, y, c)


def line(m, x0, y0, x1, y1, c):
    n = max(abs(x1 - x0), abs(y1 - y0), 1)
    for i in range(n + 1):
        put(m, round(x0 + (x1 - x0) * i / n), round(y0 + (y1 - y0) * i / n), c)


def outline(m):
    n = len(m)
    out = [r[:] for r in m]
    for y in range(n):
        for x in range(n):
            if m[y][x] == 0 and any(0 <= x + dx < n and 0 <= y + dy < n and m[y + dy][x + dx] not in (0, O)
                                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                out[y][x] = O
    return out


def from_rows(rows, keys):
    return [[keys.get(ch, 0) for ch in r] for r in rows]


# ---------------------------------------------------------------- relics (16x16, §C5 order)
def heart(m, c, shade):
    disc(m, 5.5, 6, 3.2, c)
    disc(m, 10.5, 6, 3.2, c)
    for y in range(7, 14):
        w = 14 - y
        rect(m, 8 - w, y, 7 + w, y, c)
    for y in range(9, 14):
        w = 14 - y
        put(m, 7 + w, y, shade)


def r_ember_heart():
    m = blank(16)
    heart(m, RED, MAROON)
    disc(m, 8, 8, 1.6, GOLD)
    put(m, 8, 8, CREAM)
    put(m, 4, 4, CREAM)
    return outline(m)


def r_blink_feather():
    m = blank(16)
    for i in range(11):
        x, y = 3 + i, 13 - i
        w = 2 if 2 < i < 9 else 1
        for k in range(-w, w + 1):
            put(m, x + k, y - k // 2 if k > 0 else y, CYAN if k < 0 else TEAL)
    line(m, 2, 14, 12, 4, PALE)
    put(m, 13, 2, CREAM)
    return outline(m)


def r_gold_tooth():
    m = from_rows([
        "................",
        "................",
        "...cccc..cccc...",
        "..ccccccccccccc.",
        "..cccccccccccgc.",
        "..ccccccccccggc.",
        "..ccccccccccggc.",
        "...cccccccccgg..",
        "...ccccccccgg...",
        "....cccccccc....",
        "....ccc..ccc....",
        "....ccc..ggg....",
        "....cc....gg....",
        "....cc....gg....",
        ".....c.....g....",
        "................",
    ], {"c": CREAM, "g": GOLD})
    put(m, 5, 4, PALE2)
    return outline(m)


def r_frost_sigil():
    m = blank(16)
    for a in range(0, 360, 60):
        r = math.radians(a)
        line(m, 8, 8, round(8 + 6 * math.cos(r)), round(8 + 6 * math.sin(r)), CYAN)
        bx, by = 8 + 4 * math.cos(r), 8 + 4 * math.sin(r)
        for s in (-1, 1):
            r2 = r + s * 0.9
            line(m, round(bx), round(by), round(bx + 2 * math.cos(r2)), round(by + 2 * math.sin(r2)), TEAL)
    disc(m, 8, 8, 1.6, PALE)
    return outline(m)


def r_bone_charm():
    m = blank(16)
    line(m, 3, 12, 12, 3, PALE)
    line(m, 4, 12, 12, 4, GREY)
    for cx, cy in ((3, 13), (2, 12), (13, 3), (12, 2)):
        disc(m, cx + 0.5, cy + 0.5, 1.4, PALE)
    disc(m, 8, 8, 2.0, RED)
    put(m, 7, 7, CREAM)
    return outline(m)


def r_hex_battery():
    m = blank(16)
    rect(m, 5, 3, 10, 13, PURPLE)
    rect(m, 6, 1, 9, 2, GREY)
    rect(m, 6, 7, 9, 12, MAG)
    line(m, 9, 4, 7, 8, CREAM)
    line(m, 7, 8, 9, 8, CREAM)
    line(m, 9, 8, 7, 12, CREAM)
    return outline(m)


def r_lucky_coin():
    m = blank(16)
    disc(m, 8, 8, 6, GOLD)
    disc(m, 8.5, 8.5, 4.6, AMBER)
    disc(m, 8, 8, 4.2, GOLD)
    for cx, cy in ((7, 6), (10, 7), (8, 9), (6, 9)):
        put(m, cx, cy, CREAM)
    line(m, 8, 9, 9, 11, AMBER)
    put(m, 4, 5, CREAM)
    return outline(m)


def r_glass_hourglass():
    m = blank(16)
    rect(m, 3, 1, 12, 2, AMBER)
    rect(m, 3, 13, 12, 14, AMBER)
    for y in range(3, 13):
        w = abs(y - 7.5) * 0.8 + 0.6
        rect(m, round(8 - w), y, round(7 + w), y, PALE)
    for y in range(10, 13):
        w = abs(y - 7.5) * 0.8 - 0.4
        rect(m, round(8 - w), y, round(7 + w), y, GOLD)
    rect(m, 7, 4, 8, 5, GOLD)
    line(m, 8, 6, 8, 9, GOLD)
    return outline(m)


def r_moon_shard():
    m = blank(16)
    disc(m, 8, 8, 6, PALE)
    disc(m, 10.5, 6.5, 5, 0)
    for y in range(16):
        for x in range(16):
            if m[y][x] == PALE and x + y > 15:
                m[y][x] = CYAN
    put(m, 12, 11, CREAM)
    put(m, 13, 3, CYAN)
    return outline(m)


RELICS = [r_ember_heart, r_blink_feather, r_gold_tooth, r_frost_sigil, r_bone_charm, r_hex_battery, r_lucky_coin,
          r_glass_hourglass, r_moon_shard]


# ---------------------------------------------------------------- hex icons (8x8): swarm frenzy frail dark_moon
# famine skull
def hexes():
    k = {"r": RED, "m": MAROON, "p": PURPLE, "g": MAG, "c": CREAM, "w": PALE, "a": AMBER, "o": O, "y": GOLD}
    grids = [
        ["........", ".rr..rr.", ".rr..rr.", "........", "...rr...", "...rr...", ".rr..rr.", ".rr..rr."],
        ["....rr..", "...rr...", "..rr....", ".rrrrr..", "...rr...", "..rr....", ".rr.....", "r......."],
        ["........", ".rr.rr..", "rrr.rrr.", "rrr.rrr.", ".rr.rr..", "..r.r...", "...r....", "........"],
        ["..ppp...", ".pp.....", "pp......", "pg......", "pg......", ".pg.....", "..ggg...", "........"],
        ["........", "a......a", "a......a", ".a....a.", ".aaaaaa.", "..aaaa..", "........", "........"],
        [".wwwww..", "wwwwwww.", "woowoow.", "wwwwwww.", ".ww.ww..", ".w.w.w..", "........", "........"],
    ]
    return [from_rows(g, k) for g in grids]


# ---------------------------------------------------------------- evolution icons 16x16: phoenix tempest eclipse
def e_phoenix():
    m = blank(16)
    for i in range(7):
        line(m, 8, 9, 1 + i, 3 + i // 2, RED if i % 2 else GOLD)
        line(m, 8, 9, 15 - i, 3 + i // 2, RED if i % 2 else GOLD)
    disc(m, 8, 8, 2.2, GOLD)
    line(m, 8, 10, 6, 14, RED)
    line(m, 8, 10, 10, 14, RED)
    line(m, 8, 10, 8, 14, GOLD)
    put(m, 8, 6, CREAM)
    put(m, 8, 7, CREAM)
    return outline(m)


def e_tempest():
    m = blank(16)
    for a in range(0, 300, 12):
        r = math.radians(a)
        rr = 2 + a / 60
        put(m, round(8 + rr * math.cos(r)), round(8 + rr * math.sin(r)), MAG if a > 150 else PURPLE)
    line(m, 10, 1, 6, 8, CYAN)
    line(m, 6, 8, 10, 8, CYAN)
    line(m, 10, 8, 6, 14, CYAN)
    put(m, 7, 7, PALE)
    return outline(m)


def e_eclipse():
    m = blank(16)
    disc(m, 8, 8, 6.5, CYAN)
    disc(m, 8, 8, 5.3, TEAL)
    disc(m, 8, 8, 4.8, O)
    disc(m, 8.8, 7.4, 3.2, PURPLE)
    disc(m, 9.5, 6.8, 2.6, O)
    put(m, 3, 4, PALE)
    put(m, 13, 12, CREAM)
    return outline(m)


EVOS = [e_phoenix, e_tempest, e_eclipse]


def seed():
    pal = icons_palette()
    jobs = [
        ("relics", 16, [f() for f in RELICS], ["ember_heart", "blink_feather", "gold_tooth", "frost_sigil",
                                                "bone_charm", "hex_battery", "lucky_coin", "glass_hourglass",
                                                "moon_shard"]),
        ("hex_icons", 8, hexes(), ["swarm", "frenzy", "frail", "dark_moon", "famine", "skull"]),
        ("evo_icons", 16, [f() for f in EVOS], ["phoenix", "tempest", "eclipse"]),
    ]
    for name, n, frames, tags in jobs:
        path = os.path.join(ART, name + ".aseprite")
        if os.path.exists(path):
            print("exists, skipped:", name)
            continue
        write_aseprite(path, n, n, pal, frames, [100] * len(frames), [(t, i, i) for i, t in enumerate(tags)])
        print("wrote", name, len(frames), "frames")


if __name__ == "__main__":
    if sys.argv[1:] == ["seed"]:
        seed()
    else:
        print(__doc__)
