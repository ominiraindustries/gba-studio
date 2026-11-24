"""Block 4 art (G-51): meta screens and new weapons.

    python -I art/tools/build_block4.py seed [names]  # shop_icons, stage_cards, fx_lance, fx_star, character palettes
    python -I art/tools/build_block4.py extend         # icons +10..16, FX palette gold (3, 11, 13) + fx_rune gold 2-3
                                                       # (copies in old_style/pre_g51/; no-op once done)
Then: python -I art/tools/build_art.py export
Palette files (`*_palette.aseprite`) are exported as palette items only (see build_art.export).
"""
import json
import os
import random
import shutil
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_block2 import ART, blank, put, get, rim, shift, line, disc, save, pal, hexc, read_aseprite  # noqa: E402
from build_hordes import tri  # noqa: E402
from build_block2 import line as _line  # noqa: E402


def line(px, x0, y0, x1, y1, c, thick=1):
    _line(px, round(x0), round(y0), round(x1), round(y1), c, thick)

GFX = os.path.join(os.path.dirname(ART), "graphics")
MASTER = json.load(open(os.path.join(ART, "palette_c.json")))["master"]


def backup(name):
    d = os.path.join(ART, "old_style", "pre_g51")
    os.makedirs(d, exist_ok=True)
    dst = os.path.join(d, name + ".aseprite")
    if not os.path.exists(dst):
        shutil.copy2(os.path.join(ART, name + ".aseprite"), dst)


def rect(m, x0, y0, x1, y1, c):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            put(m, x, y, c)


def bmp_palette(name):
    im = Image.open(os.path.join(GFX, name + ".bmp"))
    p = im.getpalette()[:48]
    return [tuple(p[i:i + 3]) for i in range(0, 48, 3)]


def nearest_master(rgb):
    def d(h):
        c = hexc(h)
        return 2 * (c[0] - rgb[0]) ** 2 + 4 * (c[1] - rgb[1]) ** 2 + 3 * (c[2] - rgb[2]) ** 2
    return min(MASTER, key=d)


# ---------------------------------------------------------------- shop_icons (UI palette = hud_xp)
# UI: 1 #07060c outline, 2 #0c1626, 3 #fd3ffd, 4 #21263b, 5 #34466e, 6/7 #4b8787, 8 #c8dfeb, 9 #191935, 10 #21263b,
# 11 #46155a, 12 #e020c0, 13 #ff9cf0, 14 #e63c3c, 15 #c8dfeb
O, PN, MG, DK, ST, TL, WH, NV, PU, MA, PK, RD = 1, 2, 3, 4, 5, 6, 8, 9, 11, 12, 13, 14


def heart(m, cx, cy, s, body, hi, dark):
    disc(m, cx - s * 0.5, cy - s * 0.25, s * 0.62, body)
    disc(m, cx + s * 0.5, cy - s * 0.25, s * 0.62, body)
    tri(m, (round(cx - s * 1.12), round(cy)), (round(cx + s * 1.12), round(cy)), (round(cx), round(cy + s * 1.1)), body)
    for y in range(16):
        for x in range(16):
            if m[y][x] == body and get(m, x + 1, y + 1) == 0:
                m[y][x] = dark
    put(m, round(cx - s * 0.6), round(cy - s * 0.4), hi)
    put(m, round(cx - s * 0.6) + 1, round(cy - s * 0.6), hi)


def icon_might():
    m = blank(16, 16)
    line(m, 13, 1, 5, 9, WH, 2)          # blade
    line(m, 13, 2, 6, 9, ST, 1)
    line(m, 3, 7, 8, 12, MA, 1)          # guard
    line(m, 3, 8, 7, 12, PU, 1)
    line(m, 4, 10, 2, 12, PU, 2)         # grip
    disc(m, 1.5, 13.5, 1.2, MG)          # pommel
    put(m, 13, 1, WH); put(m, 14, 1, WH)
    return rim(m, O)


def icon_vitality():
    m = blank(16, 16)
    heart(m, 7.5, 6.5, 5.2, RD, PK, PU)
    put(m, 4, 4, WH)
    return rim(m, O)


def icon_haste():
    m = blank(16, 16)
    # boot facing right
    rect(m, 5, 3, 9, 10, ST); rect(m, 5, 9, 13, 12, ST)
    rect(m, 5, 13, 13, 13, DK); rect(m, 6, 3, 9, 4, WH)
    rect(m, 10, 10, 13, 11, TL)
    # wing at the heel
    for i, (x, y) in enumerate(((4, 4), (3, 5), (2, 6), (1, 7), (2, 8), (3, 8))):
        put(m, x, y, WH)
    line(m, 1, 5, 4, 7, TL); line(m, 0, 8, 4, 9, TL)
    # speed lines
    for y in (5, 8):
        line(m, 11, y, 14, y, MG)
    return rim(m, O)


def icon_magnet():
    m = blank(16, 16)
    disc(m, 7.5, 7, 6.2, RD)
    disc(m, 7.5, 7, 3.0, 0)
    rect(m, 4, 7, 11, 15, 0)
    rect(m, 1, 7, 4, 11, RD); rect(m, 11, 7, 14, 11, RD)
    rect(m, 1, 12, 4, 14, WH); rect(m, 11, 12, 14, 14, WH)
    for y in range(16):
        for x in range(16):
            if m[y][x] == RD and (x <= 2 or get(m, x - 1, y) == 0):
                m[y][x] = PU
    put(m, 5, 2, PK); put(m, 4, 3, PK)
    return rim(m, O)


def icon_greed():
    m = blank(16, 16)
    disc(m, 7.5, 9.5, 5.4, MA)            # coin pouch
    rect(m, 6, 2, 9, 4, MA); rect(m, 5, 4, 10, 4, PU)
    put(m, 5, 1, MA); put(m, 10, 1, MA)
    for y in range(16):
        for x in range(16):
            if m[y][x] == MA and (get(m, x + 1, y + 1) == 0 or x >= 11):
                m[y][x] = PU
    disc(m, 5.5, 8, 1.3, PK)
    # coin on top: white disc with steel rim
    disc(m, 10.5, 11.5, 2.8, ST); disc(m, 10.5, 11.5, 2.0, WH); put(m, 10, 11, ST); put(m, 11, 12, ST)
    return rim(m, O)


def icon_growth():
    m = blank(16, 16)
    tri(m, (2, 8), (9, 8), (5, 1), MA)     # magenta gem
    tri(m, (2, 8), (9, 8), (5, 14), PU)
    line(m, 5, 2, 3, 8, PK); put(m, 5, 4, WH)
    line(m, 6, 9, 8, 9, MG)
    rect(m, 11, 6, 12, 14, TL)             # up arrow
    tri(m, (9, 7), (14, 7), (11, 2), TL)
    put(m, 11, 3, WH); put(m, 11, 4, WH)
    return rim(m, O)


def icon_armor():
    m = blank(16, 16)
    rect(m, 2, 1, 13, 8, ST)
    tri(m, (2, 8), (13, 8), (7, 15), ST)
    tri(m, (3, 8), (12, 8), (7, 14), ST)
    for y in range(16):
        for x in range(16):
            if m[y][x] == ST and x >= 8:
                m[y][x] = DK
    line(m, 3, 2, 12, 2, WH); line(m, 3, 2, 3, 8, WH)
    disc(m, 7.5, 7, 2.2, MA); put(m, 7, 6, MG); put(m, 7, 5, PK)
    return rim(m, O)


def icon_recovery():
    m = blank(16, 16)
    heart(m, 7.5, 6.5, 5.2, RD, PK, PU)
    rect(m, 6, 4, 9, 11, WH); rect(m, 4, 6, 11, 9, WH)
    rect(m, 7, 5, 8, 10, PK); rect(m, 5, 7, 10, 8, PK)
    return rim(m, O)


def icon_reroll():
    m = blank(16, 16)
    for a in range(0, 360, 4):
        import math
        r = 5.3
        x = 7.5 + r * math.cos(math.radians(a)); y = 7.5 + r * math.sin(math.radians(a))
        if 20 < a < 160 or 200 < a < 340:
            c = TL if a < 180 else WH
            put(m, round(x), round(y), c)
            put(m, round(7.5 + (r - 1) * math.cos(math.radians(a))), round(7.5 + (r - 1) * math.sin(math.radians(a))), c)
    tri(m, (11, 1), (15, 4), (11, 6), WH)     # arrow heads
    tri(m, (4, 9), (0, 11), (4, 14), TL)
    disc(m, 7.5, 7.5, 1.3, MG)
    return rim(m, O)


def hexagon(m, cx, cy, r, c):
    import math
    pts = [(cx + r * math.cos(math.radians(30 + 60 * k)), cy + r * math.sin(math.radians(30 + 60 * k))) for k in range(6)]
    for y in range(16):
        for x in range(16):
            inside = True
            for k in range(6):
                (x0, y0), (x1, y1) = pts[k], pts[(k + 1) % 6]
                if (x1 - x0) * (y + 0.5 - y0) - (y1 - y0) * (x + 0.5 - x0) < 0:
                    inside = False; break
            if inside:
                put(m, x, y, c)


def icon_charged():
    m = blank(16, 16)
    hexagon(m, 8, 8, 7.2, ST)
    hexagon(m, 8, 8, 5.6, NV)
    for y in range(8, 16):
        for x in range(16):
            if m[y][x] == NV:
                m[y][x] = MA
    for x in range(16):
        if m[8][x] == MA:
            m[8][x] = PK
    put(m, 7, 4, WH); put(m, 8, 5, MG)
    return rim(m, O)


def icon_revival():
    m = blank(16, 16)
    disc(m, 7.5, 4.5, 3.2, MA); disc(m, 7.5, 4.5, 1.5, 0)    # ankh loop
    rect(m, 2, 8, 13, 9, MA)
    rect(m, 6, 8, 9, 15, MA)
    for y in range(16):
        for x in range(16):
            if m[y][x] == MA and (get(m, x - 1, y) == 0 or get(m, x, y - 1) == 0):
                m[y][x] = PK
    put(m, 7, 2, WH); put(m, 3, 8, WH); put(m, 7, 10, WH)
    return rim(m, O)


SHOP = [("might", icon_might), ("vitality", icon_vitality), ("haste", icon_haste), ("magnet", icon_magnet),
        ("greed", icon_greed), ("growth", icon_growth), ("armor", icon_armor), ("recovery", icon_recovery),
        ("reroll", icon_reroll), ("charged", icon_charged), ("revival", icon_revival)]


def shop_icons():
    p = bmp_palette("hud_xp")
    frames = [fn() for _, fn in SHOP]
    save("shop_icons", 16, 16, p, frames, [100] * len(frames), [(n, k, k) for k, (n, _) in enumerate(SHOP)])


# ---------------------------------------------------------------- phase palettes (ground / props / characters)
# Index-for-index recolours of the existing palettes (same tiles, other palette).
GROUND_PALETTES = {   # ground.bmp: 1 #07060c 2-3 #0c0d1c 4 #191935 5 #1a3645 6-7 #0c1626 8 #122635 9 #2f3551
    "ground_graveyard": ["#07060c", "#07060c", "#0c0d1c", "#0c0d1c", "#1c242c", "#2d434a", "#0c1626", "#0c1626",
                         "#1c242c", "#344d53"],
    "ground_keep": ["#07060c", "#07060c", "#190922", "#190922", "#2a012f", "#4b1530", "#200027", "#200027",
                    "#290c36", "#4b1530"],
}
PROPS_PALETTES = {    # props: 1 #07060c 2 #0c0d1c 3 #0c1626 4 #1c242c 5 #2a343d 6 #2f3551 7 #34466e 8 #4b8787
                      # 9 #8ca4b4 10 #1a3645 11 #22525b 12 #abc4d2 13 #c8dfeb 14 #e020c0 15 #abc4d2
    "props_graveyard": ["#07060c", "#07060c", "#0c0d1c", "#0c1626", "#1c242c", "#2d434a", "#2a343d", "#344d53",
                        "#4b8787", "#8ca4b4", "#1e434e", "#2a5d69", "#abc4d2", "#c8dfeb", "#fd3ffd", "#abc4d2"],
    "props_keep": ["#07060c", "#07060c", "#0c0d1c", "#190922", "#1c242c", "#2a343d", "#2f3551", "#4b1530",
                   "#e63c3c", "#8ca4b4", "#2a012f", "#bc161b", "#abc4d2", "#c8dfeb", "#ffc838", "#abc4d2"],
}
# Sprite recolours (same tiles; DISENO_CONTENIDO C2 / C4).
SPRITE_PALETTES = {
    # wizard: 1 #0c0d1c 2 #290c36 3 #191935 4 #2d1153 5 #21263b 6 #3b2171 7 #36435b 8 #fd3ffd crystal 9 #8ca4b4 rim
    "wizard_cinda": ("wizard", {2: "#4b1530", 3: "#190922", 4: "#bc161b", 5: "#2a343d", 6: "#e63c3c", 7: "#2a343d",
                                8: "#ffc838", 9: "#eecf74"}),
    "wizard_morrow": ("wizard", {2: "#122635", 3: "#0c1626", 4: "#1e434e", 5: "#1c242c", 6: "#2a5d69", 7: "#2d434a",
                                 8: "#40e8f8", 9: "#abc4d2"}),
    # goblin/orc (pieles verdes): 7 #1a3645 8 #2f3551 9 #36435b 12 #346675 14 #a06818 eyes
    "rotten": ("goblin", {4: "#1c242c", 5: "#2a343d", 7: "#22525b", 8: "#2a5d69", 9: "#4b8787", 12: "#4b8787",
                          10: "#2d434a", 14: "#e63c3c"}),
    # bat/skeleton (no-muertos): 7 #3b2171 wings 12 #fd3ffd 9-10 reds 13-15 bones
    "crow": ("bat", {6: "#21263b", 7: "#2f3551", 8: "#36435b", 12: "#ffc838", 9: "#a06818", 10: "#ffc838",
                     13: "#36435b", 14: "#8ca4b4", 15: "#abc4d2"}),
    "grey_flesh": ("skeleton", {13: "#2a343d", 14: "#36435b", 15: "#8ca4b4", 11: "#344d53", 6: "#2d434a",
                                7: "#1c242c", 10: "#fd3ffd", 9: "#e020c0"}),
    "red_bone": ("skeleton", {13: "#bc161b", 14: "#e63c3c", 15: "#ff9cf0", 11: "#4b1530", 6: "#2a012f",
                              7: "#46155a", 10: "#ffc838", 9: "#a06818"}),
    # ghost: 3 rim 5-7 body 8-10 eyes
    "violet": ("ghost", {3: "#3b2171", 4: "#2d1153", 5: "#46155a", 6: "#e020c0", 7: "#ff9cf0", 2: "#2a012f"}),
}


def recolour(base_pal, mapping):
    out = list(base_pal)
    for i, h in mapping.items():
        out[i] = hexc(h)
    return out


def palette_files():
    """<name>_palette.aseprite: a preview of the sprite (or ground/props crop) in the recoloured palette.
    The export writes only its palette (sprite_palette, or bg_palette for ground_/props_)."""
    for name, (base, mapping) in SPRITE_PALETTES.items():
        w, h, p, fr, durs, tags = read_aseprite(base)
        p = (list(p) + [(0, 0, 0)] * 16)[:16]
        frames = fr[:4]
        save(name + "_palette", w, h, recolour(p, mapping), frames, [150] * len(frames), [("preview", 0, len(frames) - 1)])
    for name, cols in list(GROUND_PALETTES.items()) + list(PROPS_PALETTES.items()):
        src = "ground" if name.startswith("ground") else "props"
        im = Image.open(os.path.join(GFX, src + ".bmp"))
        crop = im.crop((0, 0, 128, 128) if src == "ground" else (0, 0, 256, 136))
        px = list(crop.getdata())
        cw, ch = crop.size
        frame = [px[y * cw:(y + 1) * cw] for y in range(ch)]
        p = [hexc(c) for c in cols] + [(0, 0, 0)] * (16 - len(cols))
        save(name + "_palette", cw, ch, p, [frame], [100], [("preview", 0, 0)])


# ---------------------------------------------------------------- stage_cards 64x64
def scene(ground_cols, props_cols, pieces, seed):
    g = Image.open(os.path.join(GFX, "ground.bmp")).convert("P")
    gp = [c for h in ground_cols for c in hexc(h)]
    g.putpalette(gp + [0] * (768 - len(gp)))
    canvas = g.crop((64, 64, 192, 192)).convert("RGBA")
    props = Image.open(os.path.join(GFX, "props.bmp")).convert("P")
    pp = [c for h in props_cols for c in hexc(h)]
    props.putpalette(pp + [0] * (768 - len(pp)))
    lay = {e["name"]: e for e in json.load(open(os.path.join(ART, "props_layout.json")))}
    for name, x, y in pieces:
        e = lay[name]
        piece = props.crop((e["x"], e["y"], e["x"] + e["w"], e["y"] + e["h"]))
        mask = Image.new("L", piece.size)
        mask.putdata([0 if v == 0 else 255 for v in piece.getdata()])
        canvas.paste(piece.convert("RGBA"), (x, y), mask)
    return canvas


def card_from(images, frame_cols):
    """Downscale 128->60 scenes, quantise jointly to the master palette, frame them."""
    small = [im.resize((60, 60), Image.BOX).convert("RGB") for im in images]
    cols = {}
    for im in small:
        for c in im.getdata():
            h = nearest_master(c)
            cols[h] = cols.get(h, 0) + 1
    keep = set(frame_cols)
    budget = 15 - len(keep)
    for h, _ in sorted(cols.items(), key=lambda kv: -kv[1]):
        if len(keep) >= 15:
            break
        keep.add(h)
    palette = [(0, 0, 0)] + [hexc(h) for h in sorted(keep, key=lambda h: sum(hexc(h)))]
    idx = {c: i for i, c in enumerate(palette)}

    def q(rgb):
        return min(range(1, len(palette)), key=lambda i: 2 * (palette[i][0] - rgb[0]) ** 2 +
                   4 * (palette[i][1] - rgb[1]) ** 2 + 3 * (palette[i][2] - rgb[2]) ** 2)
    frames = []
    for im in small:
        m = blank(64, 64)
        cache = {}
        for y in range(60):
            for x in range(60):
                c = im.getpixel((x, y))
                if c not in cache:
                    cache[c] = q(c)
                m[y + 2][x + 2] = cache[c]
        frames.append(m)
    return palette, idx, frames


def frame_border(m, idx, outer, inner, hi):
    for i in range(64):
        for (x, y) in ((i, 0), (i, 63), (0, i), (63, i)):
            m[y][x] = idx[hexc(outer)]
        for (x, y) in ((i, 1), (i, 62), (1, i), (62, i)):
            if 1 <= x <= 62 and 1 <= y <= 62:
                m[y][x] = idx[hexc(inner)]
    for (x, y) in ((1, 1), (62, 1), (1, 62), (62, 62)):
        m[y][x] = idx[hexc(hi)]


def stage_cards():
    meadow = scene(["#07060c", "#07060c", "#0c0d1c", "#0c0d1c", "#191935", "#1a3645", "#0c1626", "#0c1626",
                    "#122635", "#2f3551"],
                   ["#07060c", "#07060c", "#0c0d1c", "#0c1626", "#1c242c", "#2a343d", "#2f3551", "#34466e", "#4b8787",
                    "#8ca4b4", "#1a3645", "#22525b", "#abc4d2", "#c8dfeb", "#e020c0", "#abc4d2"],
                   [("tree", 4, 10), ("mushrooms", 70, 78), ("rock", 86, 30), ("tall_grass", 40, 92),
                    ("rune_stone", 66, 40)], 1)
    grave = scene(GROUND_PALETTES["ground_graveyard"], PROPS_PALETTES["props_graveyard"],
                  [("arch", 50, 0), ("log", 6, 60), ("log", 40, 84), ("stump", 84, 86), ("pine", 0, 0)], 2)
    keep = scene(GROUND_PALETTES["ground_keep"], PROPS_PALETTES["props_keep"],
                 [("ruin_wall", 0, 16), ("pillar", 84, 4), ("pillar_stump", 60, 84), ("rocks_small", 8, 96)], 3)
    # locked: the graveyard scene sunk into darkness
    lock = grave.point(lambda v: v // 3)
    palette, idx, frames = card_from([meadow, grave, keep, lock], ["#07060c", "#34466e", "#8ca4b4", "#c8dfeb",
                                                                    "#fd3ffd"])
    borders = [("#07060c", "#34466e", "#8ca4b4"), ("#07060c", "#344d53" if hexc("#344d53") in idx else "#34466e",
                                                     "#8ca4b4"),
               ("#07060c", "#34466e", "#8ca4b4"), ("#07060c", "#34466e", "#34466e")]
    for m, (a, b, c) in zip(frames, borders):
        frame_border(m, idx, a, b, c)
    # padlock on the locked card
    m = frames[3]
    st, hi, wh, dk = idx[hexc("#34466e")], idx[hexc("#8ca4b4")], idx[hexc("#c8dfeb")], idx[hexc("#07060c")]
    disc(m, 31.5, 25, 8, st); disc(m, 31.5, 25, 5, 0)
    for y in range(17, 34):
        for x in range(20, 44):
            if m[y][x] == 0:
                m[y][x] = frames[1][y][x] if False else m[y][x]
    # restore the scene inside the shackle hole
    for y in range(16, 34):
        for x in range(20, 44):
            if m[y][x] == 0:
                m[y][x] = dk
    rect(m, 21, 28, 42, 46, st)
    rect(m, 22, 29, 41, 45, hi)
    rect(m, 23, 30, 40, 44, st)
    disc(m, 31.5, 35, 2.2, dk); rect(m, 31, 36, 32, 41, dk)
    line(m, 23, 29, 40, 29, wh)
    m2 = rim([[c if (16 <= y <= 47 and 19 <= x <= 44 and c in (st, hi, wh)) else 0 for x, c in enumerate(r)]
              for y, r in enumerate(m)], dk)
    for y in range(64):
        for x in range(64):
            if m2[y][x] == dk and m[y][x] not in (st, hi, wh):
                m[y][x] = dk
    save("stage_cards", 64, 64, palette, frames, [100] * 4,
         [("meadow", 0, 0), ("graveyard", 1, 1), ("keep", 2, 2), ("locked", 3, 3)])


# ---------------------------------------------------------------- FX palette gold + new FX
# FX palette: 1 #190922 2 #c8dfeb 3 (=2) 4 #ff9cf0 5 #e020c0 6 #e63c3c 7 #c0fcff 8 #40e8f8 9 #1a7f9c 10 #346675
# 11 (=2) 12 #fd3ffd 13 (=5) 14 #3b2171 15 #07060c.  3, 11 and 13 duplicate other entries: their pixels are moved to
# the twin and the slots become gold: 3 #a06818, 11 #ffc838, 13 #fff4b0 (no visible change to the old FX).
FX_FILES = ["fx_spark", "fx_rune", "fx_frost", "fx_impact", "fx_poof", "fx_aura", "fx_nova", "fx_nova_spark",
            "fx_blink", "fx_chain", "shadow_small", "shadow_big"]
GOLD = {3: "#a06818", 11: "#ffc838", 13: "#fff4b0"}
TWIN = {3: 2, 11: 2, 13: 5}


def fx_palette():
    p = bmp_palette("fx_spark")
    for i, h in GOLD.items():
        p[i] = hexc(h)
    return p


def extend_fx_gold():
    w, h, p, fr, durs, tags = read_aseprite("fx_spark")
    if tuple(p[11]) == hexc("#ffc838"):
        print("FX palette already has gold"); return
    newp = fx_palette()
    for name in FX_FILES:
        if not os.path.exists(os.path.join(ART, name + ".aseprite")):
            continue
        w, h, p, fr, durs, tags = read_aseprite(name)
        backup(name)
        fr = [[[TWIN.get(c, c) for c in row] for row in f] for f in fr]
        if name == "fx_rune":
            fr = fr[:2]
            durs = durs[:2]
            gold = {2: 13, 11: 11, 12: 11, 14: 3}
            fr += [[[gold.get(c, c) for c in row] for row in f] for f in fr]
            fr[3] = [[(13 if c == 11 else c) for c in row] for row in fr[3]]
            durs += [120, 120]
            tags = [t for t in tags if t[0] != "gold"] + [("gold", 2, 3)]
        save(name, w, h, newp, fr, durs, tags)


def fx_lance():
    """32x16 horizontal beam segment (tiles end to end): 0 thin start-up, 1 full, 2 fading."""
    P = fx_palette()
    frames = []
    for k in range(3):
        m = blank(32, 16)
        half = (2, 5, 3)[k]
        for x in range(32):
            wob = (x // 4 + k) % 2
            for dy in range(-half, half + 1):
                y = 8 + dy
                a = abs(dy)
                c = 2 if a <= (0 if k == 0 else 1) else (8 if a <= half - 2 else (9 if a < half else 14))
                if k == 2 and (x + y) % 2 and a >= 1:
                    continue
                put(m, x, y, c)
            if k == 1:
                put(m, x, 8 - half - 1 - wob, 14)
                put(m, x, 8 + half + 1 + wob, 14)
        if k >= 1:     # moon sparkles riding the beam
            for sx in ((3, 19) if k == 1 else (11, 27)):
                put(m, sx, 8 - half, 7); put(m, sx + 1, 8 - half - 1, 2)
                put(m, sx + 6, 8 + half, 7)
        frames.append(m)
    save("fx_lance", 32, 16, P, frames, [40, 60, 60], [("beam", 0, 2)])


def star(m, cx, cy, r, c_out, c_in, c_core):
    import math
    pts = []
    for k in range(10):
        rr = r if k % 2 == 0 else r * 0.45
        a = math.radians(-90 + 36 * k)
        pts.append((cx + rr * math.cos(a), cy + rr * math.sin(a)))
    for k in range(10):
        tri(m, (round(cx), round(cy)), (round(pts[k][0]), round(pts[k][1])),
            (round(pts[(k + 1) % 10][0]), round(pts[(k + 1) % 10][1])), c_out)
    disc(m, cx, cy, r * 0.42, c_in)
    put(m, round(cx), round(cy), c_core)


def fx_star():
    """Starfall: gold star dropping from the upper right with a magenta trail; 3 = impact flash."""
    P = fx_palette()
    frames = []
    for k, (cx, cy) in enumerate(((10, 4), (8.5, 7), (7.5, 10))):
        m = blank(16, 16)
        for i in range(1, 6):              # trail
            tx, ty = cx + i * 1.0, cy - i * 1.3
            put(m, round(tx), round(ty), 12 if i < 3 else 5)
            if i < 3:
                put(m, round(tx) + 1, round(ty), 4)
        star(m, cx, cy, 3.8, 11, 13, 2)
        frames.append(m)
    m = blank(16, 16)
    for (dx, dy) in ((0, -6), (5, -3), (5, 3), (0, 6), (-5, 3), (-5, -3)):
        line(m, 7.5, 11, 7.5 + dx * 0.9, 11 + dy * 0.6, 11)
    disc(m, 7.5, 11, 2.6, 13); disc(m, 7.5, 11, 1.2, 2)
    frames.append(m)
    frames = [rim(f, 3) for f in frames]
    save("fx_star", 16, 16, P, frames, [60, 60, 60, 80], [("fall", 0, 3)])


# ---------------------------------------------------------------- icons +10..16 (recogibles palette)
# 1 #07060c 2 #1a7f9c 3 #40e8f8 4 #a06818 5 #346675 6 #8ca4b4 7 #c8dfeb 8 #46155a 9 #e63c3c 10 #fd3ffd 11 #4b1530
# 12 #ffc838 13 #fff4b0 14 #c8dfeb 15 #2a343d
def ic_moon_lance():
    m = blank(16, 16)
    disc(m, 5.5, 6, 4.6, 7); disc(m, 7.5, 4.8, 3.9, 0)       # crescent
    for y in range(16):
        for x in range(16):
            if m[y][x] == 7 and get(m, x + 1, y - 1) == 0:
                m[y][x] = 6
    line(m, 3, 12, 15, 12, 3); line(m, 3, 13, 15, 13, 2); line(m, 5, 11, 15, 11, 2)
    put(m, 15, 12, 7); put(m, 14, 12, 7)
    return rim(m, 1)


def ic_lens():
    m = blank(16, 16)
    disc(m, 6.5, 6.5, 5.2, 6); disc(m, 6.5, 6.5, 4.0, 2); disc(m, 6.5, 6.5, 3.0, 5)
    put(m, 5, 4, 3); put(m, 4, 5, 3); put(m, 5, 5, 7)
    line(m, 10, 10, 14, 14, 4, 2); line(m, 11, 10, 14, 13, 12)
    return rim(m, 1)


def ic_ward_stone():
    m = blank(16, 16)
    tri(m, (2, 13), (13, 13), (7, 1), 15)                   # standing stone
    rect(m, 3, 9, 12, 14, 15)
    for y in range(16):
        for x in range(16):
            if m[y][x] == 15 and x <= 5:
                m[y][x] = 6
    line(m, 7, 5, 7, 11, 3); line(m, 5, 7, 9, 7, 3); put(m, 6, 10, 2); put(m, 8, 10, 2)
    return rim(m, 1)


def ic_starfall():
    m = blank(16, 16)
    for i in range(1, 7):
        put(m, 8 + i, 7 - i, 10 if i < 4 else 8)
        put(m, 9 + i, 7 - i, 10 if i < 3 else 8)
    star(m, 6.5, 9, 5.2, 12, 13, 7)
    return rim(m, 1)


def ic_blizzard():
    m = blank(16, 16)
    for (dx, dy) in ((0, 1), (1, 0), (1, 1), (1, -1)):
        line(m, 7.5 - 6.5 * dx, 7.5 - 6.5 * dy, 7.5 + 6.5 * dx, 7.5 + 6.5 * dy, 3)
    for (x, y) in ((7, 2), (8, 2), (7, 13), (8, 13), (2, 7), (2, 8), (13, 7), (13, 8)):
        put(m, x, y, 7)
    disc(m, 7.5, 7.5, 2.2, 2); disc(m, 7.5, 7.5, 1.1, 7)
    for (x, y) in ((1, 1), (14, 2), (2, 14), (13, 13)):
        put(m, x, y, 14)
    return rim(m, 1)


def ic_ward_of_ages():
    import math
    m = blank(16, 16)
    for k in range(6):
        a = math.radians(60 * k - 90)
        x, y = 7.5 + 5.2 * math.cos(a), 7.5 + 5.2 * math.sin(a)
        disc(m, x, y, 1.6, 12); put(m, round(x), round(y), 13)
    tri(m, (5, 5), (10, 5), (7, 11), 15); rect(m, 5, 4, 10, 6, 15)     # small shield in the ring
    line(m, 7, 5, 7, 8, 4); put(m, 8, 5, 4)
    return rim(m, 1)


def ic_locked():
    m = blank(16, 16)
    disc(m, 7.5, 5.5, 4, 6); disc(m, 7.5, 5.5, 2.3, 0); rect(m, 4, 6, 11, 8, 0)
    rect(m, 3, 7, 12, 14, 15); rect(m, 3, 7, 12, 7, 6)
    disc(m, 7.5, 10, 1.2, 1); put(m, 7, 12, 1); put(m, 8, 12, 1)
    return rim(m, 1)


NEW_ICONS = [("moon_lance", ic_moon_lance), ("lens", ic_lens), ("ward_stone", ic_ward_stone),
             ("starfall", ic_starfall), ("blizzard", ic_blizzard), ("ward_of_ages", ic_ward_of_ages),
             ("locked", ic_locked)]


def extend_icons():
    w, h, p, frames, durs, tags = read_aseprite("icons")
    if any(t[0] == "moon_lance" for t in tags):
        print("icons already extended (block 4)"); return
    backup("icons")
    assert len(frames) == 10
    for name, fn in NEW_ICONS:
        tags.append((name, len(frames), len(frames)))
        frames.append(fn()); durs.append(100)
    save("icons", 16, 16, p, frames, durs, tags)


NEW = {"shop_icons": shop_icons, "stage_cards": stage_cards, "fx_lance": fx_lance, "fx_star": fx_star}

if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        only = sys.argv[2:]
        for name, fn in NEW.items():
            if only and name not in only:
                continue
            if os.path.exists(os.path.join(ART, name + ".aseprite")):
                print("exists, skipped:", name); continue
            fn()
        if not only or "palettes" in only:
            if any(os.path.exists(os.path.join(ART, n + "_palette.aseprite"))
                   for n in list(SPRITE_PALETTES) + list(GROUND_PALETTES) + list(PROPS_PALETTES)):
                print("palette files exist, skipped")
            else:
                palette_files()
    elif cmd == "extend":
        extend_fx_gold()
        extend_icons()
