"""HUD mockups for DELVE (G-30). Mockups only: nothing here is read by the build.

Run: python -I projects/006_delve/art/hud_mockups/_src/make_mockups.py
Inputs: real view captures in this folder (shot_corridor.png, shot_map.png), the Butano variable 8x8 font
and mon_skeleton.bmp. Outputs: hud_<option>.png (240x160) and hud_<option>_3x.png next to this folder.
"""
import os
import re
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.dirname(HERE)
ROOT = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..', '..'))
FONT_BMP = os.path.join(ROOT, 'engine/butano/common/graphics/common_variable_8x8_font.bmp')
FONT_H = os.path.join(ROOT, 'engine/butano/common/include/common_variable_8x8_sprite_font.h')
SKEL = os.path.join(ROOT, 'projects/006_delve/graphics/mon_skeleton.bmp')

# Colours taken from ui_frame.bmp (reserved HUD indices 192-255) plus HP / reload accents.
DARK = (18, 21, 25)
BLACK = (7, 8, 10)
STEEL = (107, 116, 128)
STEEL_D = (26, 30, 36)
BRICK = [(55, 44, 38), (74, 61, 52), (98, 81, 66)]
MORTAR = (17, 13, 12)
BRONZE = [(107, 74, 30), (166, 124, 53), (220, 177, 98)]
RED = (176, 36, 32)
RED_HI = (232, 88, 64)
RED_EMPTY = (52, 20, 20)
GOLD = (220, 177, 98)
WHITE = (236, 232, 220)
GREY = (140, 144, 150)
BLUE = (60, 96, 200)

# ---------------------------------------------------------------- font (looks exactly like the in-game one)
_font = Image.open(FONT_BMP)
_widths = [int(w) for w in re.findall(r'^\s*(\d+),\s*//', open(FONT_H, encoding='utf-8').read(), re.M)]


def text_width(s):
    return sum(_widths[ord(c) - 32] for c in s)


def text(img, x, y, s, color=WHITE, align='left'):
    if align == 'center':
        x -= text_width(s) // 2
    elif align == 'right':
        x -= text_width(s)
    for c in s:
        i = ord(c) - 32
        if i == 0:  # the font image starts at '!' (no glyph for space)
            x += _widths[0]
            continue
        g = _font.crop((0, (i - 1) * 8, 8, (i - 1) * 8 + 8))
        for gy in range(8):
            for gx in range(8):
                v = g.getpixel((gx, gy))
                px, py = x + gx, y + gy
                if v and 0 <= px < img.width and 0 <= py < img.height:
                    img.putpixel((px, py), color if v == 14 else (0, 0, 0))
        x += _widths[i]
    return x


# ---------------------------------------------------------------- pixel icons (string maps)
PAL = {'k': (10, 8, 8), 'w': (230, 236, 240), 'g': (150, 158, 168), 'd': (84, 90, 100), 'y': GOLD, 'o': BRONZE[1],
       'b': (120, 72, 40), 'n': (78, 46, 28), 'r': (200, 40, 40), 'R': (250, 110, 90), 'm': (120, 20, 30),
       'c': (170, 200, 220), 'e': (60, 150, 60), 'E': (130, 210, 100), 's': (214, 160, 120), 'S': (160, 108, 78),
       'f': (255, 210, 80), 'F': (255, 140, 30), 'h': (200, 60, 20), 'l': (70, 50, 40), 'L': (110, 82, 60),
       'B': BLUE, 'C': (140, 170, 255)}

DAGGER = """
...........kk...
..........kwwk..
.........kwwgk..
........kwwgk...
.......kwwgk....
......kwwgk.....
.....kwwgk......
..kk.kwgk.......
..kykkgk........
...kyyk.........
...knyyk........
..knbk.kk.......
.knbk...........
kknk............
kok.............
.k..............
"""
POTION = """
.....kkkkk......
.....kngnk......
......kgk.......
......kck.......
.....kccck......
....kcccc.k.....
...kcRRRrrrk....
..kcRRrrrrrrk...
..kRRrrrrrrrk...
..kRrrrrrrrmk...
..krrrrrrrrmk...
..krrrrrrrmmk...
...krrrrrmmk....
....kkmmmkk.....
......kkk.......
................
"""
KEY = """
................
....kkkk........
...kyyyyk.......
..kyokkoyk......
..kyk..kyk......
..kyk..kyk......
..kyokkoyk......
...kyyyyk.......
....kyyk........
....kyyk........
....kyykkk......
....kyyyyk......
....kyykk.......
....kyyykk......
....kyyyyk......
.....kkkk.......
"""
POISON = """
...kk...
..kEEk..
.kEeek..
kEeeeek.
keeeeek.
keeeeek.
.keeek..
..kkk...
"""
BREAD = """
................
................
................
....kkkkkkk.....
..kkLLLLLLLkk...
.kLLyLLyLLyLLk..
kLLLLLLLLLLLLLk.
kLyLLyLLyLLyLLk.
kLLLLLLLLLLLLLk.
klLLLLLLLLLLLlk.
.kllllllllllllk.
..kkkkkkkkkkkk..
................
................
................
................
"""


def icon(img, x, y, art, scale=1):
    rows = [r for r in art.strip('\n').split('\n')]
    for j, row in enumerate(rows):
        for i, ch in enumerate(row):
            if ch != '.':
                for sy in range(scale):
                    for sx in range(scale):
                        img.putpixel((x + i * scale + sx, y + j * scale + sy), PAL[ch])


# ---------------------------------------------------------------- hand sprites for option B (drawn, provisional)
def outline(sprite, col=(10, 8, 8)):
    w, h = sprite.size
    src = sprite.copy()
    for y in range(h):
        for x in range(w):
            if src.getpixel((x, y))[3] == 0:
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < w and 0 <= ny < h and src.getpixel((nx, ny))[3]:
                        sprite.putpixel((x, y), col + (255,))
                        break
    return sprite


def weapon_hand():
    s = Image.new('RGBA', (72, 64), (0, 0, 0, 0))
    d = ImageDraw.Draw(s)
    # blade (pointing up-left into the view), guard, grip
    d.polygon([(20, 6), (24, 4), (46, 30), (41, 35)], fill=PAL['g'] + (255,))
    d.line([(22, 5), (44, 31)], fill=PAL['w'] + (255,), width=2)
    d.line([(36, 38), (50, 24)], fill=PAL['o'] + (255,), width=4)
    d.line([(37, 37), (49, 25)], fill=PAL['y'] + (255,), width=1)
    # fist
    d.ellipse([(40, 30), (60, 48)], fill=PAL['S'] + (255,))
    d.ellipse([(42, 31), (57, 44)], fill=PAL['s'] + (255,))
    for k in range(4):
        d.line([(43 + k * 4, 33), (43 + k * 4, 37)], fill=PAL['S'] + (255,))
    # sleeve / bracer
    d.polygon([(48, 44), (62, 38), (72, 52), (72, 64), (56, 64)], fill=PAL['n'] + (255,))
    d.polygon([(50, 44), (60, 40), (66, 48), (54, 52)], fill=PAL['b'] + (255,))
    d.line([(52, 52), (66, 46)], fill=PAL['o'] + (255,), width=1)
    return outline(s)


def torch_hand():
    s = Image.new('RGBA', (56, 72), (0, 0, 0, 0))
    d = ImageDraw.Draw(s)
    d.polygon([(22, 18), (28, 18), (26, 50), (20, 50)], fill=PAL['L'] + (255,))  # stick
    d.line([(23, 20), (22, 48)], fill=PAL['b'] + (255,))
    d.ellipse([(16, 4), (34, 24)], fill=PAL['h'] + (255,))  # flame
    d.ellipse([(19, 8), (31, 22)], fill=PAL['F'] + (255,))
    d.ellipse([(22, 12), (28, 21)], fill=PAL['f'] + (255,))
    d.polygon([(25, 0), (29, 8), (21, 8)], fill=PAL['F'] + (255,))
    d.ellipse([(12, 40), (32, 56)], fill=PAL['S'] + (255,))  # fist
    d.ellipse([(14, 41), (29, 52)], fill=PAL['s'] + (255,))
    d.polygon([(6, 72), (14, 50), (30, 52), (34, 72)], fill=PAL['n'] + (255,))  # sleeve
    d.polygon([(12, 56), (16, 50), (30, 53), (30, 60)], fill=PAL['b'] + (255,))
    return outline(s)


def glow(img, cx, cy, r, strength=0.35):
    """Warm torch light on the left of the view (option B), additive."""
    px = img.load()
    for y in range(max(0, cy - r), min(img.height, cy + r)):
        for x in range(max(0, cx - r), min(img.width, cx + r)):
            t = 1 - ((x - cx) ** 2 + (y - cy) ** 2) ** 0.5 / r
            if t > 0:
                a = strength * t
                p = px[x, y]
                px[x, y] = (min(255, int(p[0] + 90 * a)), min(255, int(p[1] + 50 * a)), min(255, int(p[2] + 10 * a)))


# ---------------------------------------------------------------- HUD pieces
def bricks(img, rect):
    x0, y0, x1, y1 = rect
    d = ImageDraw.Draw(img)
    d.rectangle([x0, y0, x1 - 1, y1 - 1], fill=MORTAR)
    row = 0
    for y in range(y0, y1, 8):
        off = 0 if row % 2 == 0 else 12
        for x in range(x0 - 24 + off, x1, 24):
            c = BRICK[(x * 7 + y * 3) // 24 % 3 if (x // 24 + row) % 3 else 1]
            if max(x0, x + 1) > min(x1 - 1, x + 22):
                continue
            d.rectangle([max(x0, x + 1), y + 1, min(x1 - 1, x + 22), min(y1 - 1, y + 6)], fill=c)
            d.line([max(x0, x + 1), y + 1, min(x1 - 1, x + 22), y + 1], fill=BRICK[2] if c != BRICK[2] else BRICK[1])
        row += 1


def box(img, rect, fill=DARK, rivets=True):
    x0, y0, x1, y1 = rect
    d = ImageDraw.Draw(img)
    d.rectangle([x0, y0, x1 - 1, y1 - 1], fill=fill, outline=STEEL)
    d.rectangle([x0 + 1, y0 + 1, x1 - 2, y1 - 2], outline=STEEL_D)
    if rivets and x1 - x0 > 12 and y1 - y0 > 12:
        for rx, ry in ((x0 + 2, y0 + 2), (x1 - 4, y0 + 2), (x0 + 2, y1 - 4), (x1 - 4, y1 - 4)):
            d.rectangle([rx, ry, rx + 1, ry + 1], fill=BRONZE[1])
            img.putpixel((rx, ry), BRONZE[2])


def bar(img, rect, frac, fill, hi, empty, border=BLACK):
    x0, y0, x1, y1 = rect
    d = ImageDraw.Draw(img)
    d.rectangle([x0, y0, x1 - 1, y1 - 1], fill=empty, outline=border)
    w = int((x1 - x0 - 2) * frac)
    if w > 0:
        d.rectangle([x0 + 1, y0 + 1, x0 + w, y1 - 2], fill=fill)
        d.line([x0 + 1, y0 + 1, x0 + w, y0 + 1], fill=hi)


def damage_arrow(img, x, y, side):
    """8x16 red arrow pointing out of the view edge (ESPEC_VISTA reserved gutter)."""
    d = ImageDraw.Draw(img)
    if side == 'left':
        pts = [(x, y + 8), (x + 7, y), (x + 7, y + 15)]
    else:
        pts = [(x + 7, y + 8), (x, y), (x, y + 15)]
    d.polygon(pts, fill=(220, 30, 30), outline=(20, 0, 0))
    d.line([pts[0], pts[1]], fill=(255, 120, 100))


def compass_ribbon(img, cx, y, heading, w=46):
    order = 'NESW'
    i = order.index(heading)
    left, right = order[(i - 1) % 4], order[(i + 1) % 4]
    # Turning left from S faces E: the letter on the left is the one you face after LEFT.
    text(img, cx - w // 2 + 2, y, left, GREY)
    text(img, cx, y, heading, GOLD, 'center')
    text(img, cx + w // 2 - 2, y, right, GREY, 'right')
    d = ImageDraw.Draw(img)
    for k in (-14, -7, 7, 14):
        d.point((cx + k, y + 3), fill=STEEL)


def paste_skeleton(img, cx, feet_y, scale=1.0):
    sk = Image.open(SKEL).convert('RGBA')
    pal_key = sk.getpixel((0, 0))
    fr = sk.crop((0, 0, 64, 64))
    data = [(0, 0, 0, 0) if p[:3] == (255, 0, 255) else p for p in fr.getdata()]
    fr.putdata(data)
    if scale != 1.0:
        fr = fr.resize((int(64 * scale), int(64 * scale)), Image.NEAREST)
    bbox = fr.getbbox()
    img.alpha_composite(fr, (cx - fr.width // 2, feet_y - bbox[3]))


# ---------------------------------------------------------------- views from the real capture
def real_view():
    return Image.open(os.path.join(HERE, 'shot_corridor.png')).convert('RGBA').crop((0, 0, 176, 120))


def real_minimap():
    return Image.open(os.path.join(HERE, 'shot_map.png')).convert('RGBA').crop((178, 58, 238, 118))


def widen(view, new_w, new_h, horizon_old=48, horizon_new=48):
    """Approximate a wider/taller view for the mockup: stretch only the outer bands (near side walls, near floor
    and ceiling), which is what perspective does there. The centre keeps the real 1:1 pixels and sprite scale."""
    w, h = view.size
    band = 40
    extra = (new_w - w) // 2
    left = view.crop((0, 0, band, h)).resize((band + extra, h), Image.NEAREST)
    right = view.crop((w - band, 0, w, h)).resize((band + extra, h), Image.NEAREST)
    mid = view.crop((band, 0, w - band, h))
    wide = Image.new('RGBA', (new_w, h))
    wide.paste(left, (0, 0))
    wide.paste(mid, (band + extra, 0))
    wide.paste(right, (new_w - band - extra, 0))
    top_extra = horizon_new - horizon_old
    if top_extra > 0:
        top = wide.crop((0, 0, new_w, 24)).resize((new_w, 24 + top_extra), Image.NEAREST)
        wide2 = Image.new('RGBA', (new_w, h + top_extra))
        wide2.paste(top, (0, 0))
        wide2.paste(wide.crop((0, 24, new_w, h)), (0, 24 + top_extra))
        wide = wide2
    h2 = wide.height
    if new_h > h2:
        bot = wide.crop((0, h2 - 32, new_w, h2)).resize((new_w, 32 + new_h - h2), Image.NEAREST)
        wide3 = Image.new('RGBA', (new_w, new_h))
        wide3.paste(wide.crop((0, 0, new_w, h2 - 32)), (0, 0))
        wide3.paste(bot, (0, h2 - 32))
        wide = wide3
    return wide.crop((0, 0, new_w, new_h))


def save(img, name):
    img = img.convert('RGB')
    img.save(os.path.join(OUT, name + '.png'))
    img.resize((720, 480), Image.NEAREST).save(os.path.join(OUT, name + '_3x.png'))


# ================================================================ option A: classic framed panel, refined
def option_a():
    img = Image.new('RGBA', (240, 160), BLACK + (255,))
    v = real_view()
    paste_skeleton(v, 88, 81)
    img.paste(v, (0, 0))
    damage_arrow(img, 1, 52, 'left')
    # right panel 64x120
    bricks(img, (176, 0, 240, 120))
    box(img, (178, 2, 238, 28))
    compass_ribbon(img, 208, 6, 'S')
    text(img, 208, 17, 'FLOOR 1', WHITE, 'center')
    box(img, (178, 30, 238, 92), BLACK, rivets=False)
    img.paste(real_minimap(), (178, 31))
    box(img, (178, 94, 238, 118))
    icon(img, 181, 97, KEY)
    text(img, 197, 101, 'x1', WHITE)
    icon(img, 216, 102, POISON)
    text(img, 226, 102, '', WHITE)
    # bottom strip 240x40
    bricks(img, (0, 120, 240, 160))
    box(img, (2, 122, 40, 158))  # hand
    icon(img, 5, 124, DAGGER, 2)
    bar(img, (4, 152, 38, 157), 0.7, GOLD, (255, 230, 160), (40, 32, 20))
    box(img, (42, 122, 238, 146))  # HP + quick item
    text(img, 46, 127, 'HP', RED_HI)
    bar(img, (60, 127, 156, 135), 24 / 30, RED, RED_HI, RED_EMPTY)
    text(img, 160, 127, '24/30', WHITE)
    box(img, (198, 124, 236, 144), BLACK, rivets=False)
    icon(img, 200, 126, POTION)
    text(img, 217, 127, 'x3', WHITE)
    text(img, 217, 136, 'B', GOLD)
    box(img, (42, 147, 238, 159), BLACK, rivets=False)
    text(img, 46, 149, 'A: ATTACK WHEN BAR IS FULL', WHITE)
    save(img, 'hud_a_panel')


# ================================================================ option B: full-screen view, minimal overlay
def option_b():
    v = widen(real_view(), 240, 160, 48, 64)
    img = Image.new('RGBA', (240, 160), BLACK + (255,))
    img.paste(v, (0, 0))
    glow(img, 30, 130, 90, 0.45)
    paste_skeleton(img, 120, 97)
    damage_arrow(img, 1, 68, 'left')
    th = torch_hand()
    img.alpha_composite(th, (8, 160 - th.height + 6))
    wh = weapon_hand()
    img.alpha_composite(wh, (240 - wh.width - 6, 160 - wh.height - 4))
    # top-left: HP plate
    d = ImageDraw.Draw(img)
    d.rectangle([2, 2, 77, 13], fill=(8, 8, 10), outline=STEEL_D)
    bar(img, (4, 5, 44, 11), 24 / 30, RED, RED_HI, RED_EMPTY)
    text(img, 47, 4, '24', WHITE)
    text(img, 61, 4, '/30', GREY)
    # top-centre compass, top-right floor
    d.rectangle([97, 2, 143, 13], fill=(8, 8, 10), outline=STEEL_D)
    compass_ribbon(img, 120, 4, 'S')
    d.rectangle([210, 2, 237, 13], fill=(8, 8, 10), outline=STEEL_D)
    text(img, 224, 4, 'F1', GOLD, 'center')
    # quick item bottom-centre, reload under the weapon hand
    d.rectangle([97, 140, 131, 157], fill=(8, 8, 10), outline=STEEL_D)
    icon(img, 99, 141, POTION)
    text(img, 116, 145, 'x3', WHITE)
    bar(img, (188, 154, 236, 158), 0.7, GOLD, (255, 230, 160), (40, 32, 20))
    # message on a dark plate under the compass
    msg = 'A: ATTACK WHEN BAR IS FULL'
    w = text_width(msg)
    d.rectangle([120 - w // 2 - 3, 18, 120 + w // 2 + 3, 29], fill=(8, 8, 10))
    text(img, 120, 20, msg, WHITE, 'center')
    save(img, 'hud_b_overlay')


# ================================================================ option C: wide view + bottom bar (Lands of Lore)
def option_c():
    v = widen(real_view(), 240, 120)
    v = v.crop((0, 0, 240, 112))
    paste_skeleton(v, 120, 81)
    img = Image.new('RGBA', (240, 160), BLACK + (255,))
    img.paste(v, (0, 0))
    damage_arrow(img, 1, 52, 'left')
    bricks(img, (0, 112, 240, 160))
    d = ImageDraw.Draw(img)
    d.line([0, 112, 239, 112], fill=STEEL)
    # weapon hand + reload
    box(img, (2, 115, 40, 151))
    icon(img, 5, 117, DAGGER, 2)
    bar(img, (2, 152, 40, 158), 0.7, GOLD, (255, 230, 160), (40, 32, 20))
    # centre: HP row and items row
    box(img, (42, 115, 156, 147))
    text(img, 46, 119, 'HP', RED_HI)
    bar(img, (60, 119, 114, 127), 24 / 30, RED, RED_HI, RED_EMPTY)
    text(img, 117, 119, '24/30', WHITE)
    icon(img, 45, 130, POTION)
    text(img, 62, 134, 'x3', WHITE)
    text(img, 76, 134, 'B', GOLD)
    icon(img, 88, 130, KEY)
    text(img, 104, 134, 'x1', WHITE)
    icon(img, 142, 134, POISON)
    # compass medallion + floor
    box(img, (158, 115, 196, 147))
    compass_ribbon(img, 177, 119, 'S', 34)
    text(img, 177, 134, 'F1', GOLD, 'center')
    # minimap 45x32 (3 px cells would give 15x10; mockup crops the real 4 px one)
    box(img, (198, 115, 238, 147), BLACK, rivets=False)
    img.paste(real_minimap().crop((6, 14, 44, 44)), (199, 116))
    # message line
    box(img, (42, 148, 238, 159), BLACK, rivets=False)
    text(img, 45, 150, 'A: ATTACK WHEN BAR IS FULL', WHITE)
    save(img, 'hud_c_bar')


if __name__ == '__main__':
    option_a()
    option_b()
    option_c()
    print('ok')
