# PROVISIONAL art generator for 007_survivors block 1.
# Writes graphics/tmp_*.bmp + .json (4bpp indexed). These are placeholders until gba-artist delivers
# art/*.aseprite -> graphics/*.bmp; the game picks items in include/hb_art.h, so swapping art is a one-line change.
# Never touches art/ or non tmp_ files. Usage: python -I projects/007_survivors/tests/gen_tmp_gfx.py
import json
import math
import os
import struct

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "graphics")

# Shared sprite palette (index 0 transparent).
SPR_PAL = [
    (255, 0, 255),   # 0 transparent
    (24, 16, 40),    # 1 outline
    (72, 40, 120),   # 2 purple dark
    (128, 80, 184),  # 3 purple
    (192, 152, 232), # 4 lilac
    (240, 192, 152), # 5 skin
    (248, 248, 248), # 6 white
    (120, 72, 40),   # 7 brown
    (248, 208, 64),  # 8 yellow
    (40, 112, 56),   # 9 green dark
    (88, 184, 72),   # 10 green
    (168, 232, 112), # 11 green light
    (48, 112, 232),  # 12 blue
    (136, 216, 248), # 13 cyan
    (224, 56, 56),   # 14 red
    (176, 168, 160), # 15 bone grey
]

BG_PAL = [
    (0, 0, 0),       # 0 transparent / backdrop
    (64, 92, 60),    # 1 ground dark
    (78, 108, 70),   # 2 ground mid
    (92, 122, 80),   # 3 ground light
    (58, 80, 54),    # 4 ground shade
    (40, 44, 48),    # 5 rock outline
    (110, 112, 116), # 6 rock
    (150, 150, 150), # 7 rock light
    (90, 60, 40),    # 8 wood dark
    (130, 92, 58),   # 9 wood
    (44, 74, 44),    # 10 bush dark
    (70, 110, 60),   # 11 bush
    (56, 84, 130),   # 12 water
    (100, 140, 180), # 13 water light
    (200, 180, 120), # 14 flower yellow
    (180, 120, 160), # 15 flower pink
]


class Canvas:
    def __init__(self, w, h):
        self.w, self.h = w, h
        self.p = [[0] * w for _ in range(h)]

    def set(self, x, y, c):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.p[y][x] = c

    def get(self, x, y):
        if 0 <= x < self.w and 0 <= y < self.h:
            return self.p[y][x]
        return 0

    def rect(self, x0, y0, x1, y1, c):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.set(x, y, c)

    def ellipse(self, cx, cy, rx, ry, c):
        for y in range(int(cy - ry - 1), int(cy + ry + 2)):
            for x in range(int(cx - rx - 1), int(cx + rx + 2)):
                if ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2 <= 1.0:
                    self.set(x, y, c)

    def outline(self, c=1, x0=0, y0=0, w=None, h=None):
        w = w or self.w
        h = h or self.h
        add = []
        for y in range(y0, y0 + h):
            for x in range(x0, x0 + w):
                if self.p[y][x] == 0:
                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                        nx, ny = x + dx, y + dy
                        if x0 <= nx < x0 + w and y0 <= ny < y0 + h and self.p[ny][nx] not in (0, c):
                            add.append((x, y))
                            break
        for x, y in add:
            self.p[y][x] = c

    def blit(self, other, ox, oy):
        for y in range(other.h):
            for x in range(other.w):
                if other.p[y][x]:
                    self.set(ox + x, oy + y, other.p[y][x])


# Only the graphics with no final art yet are written (the rest were replaced by gba-artist's set).
KEEP = {"tmp_fx", "tmp_wave", "tmp_bossbar"}


def write_bmp(name, canvas, pal, meta):
    if name not in KEEP:
        return
    w, h = canvas.w, canvas.h
    row_bytes = ((w + 1) // 2 + 3) & ~3
    pixels = bytearray()
    for y in range(h - 1, -1, -1):
        row = bytearray()
        for x in range(0, w, 2):
            a = canvas.p[y][x] & 15
            b = canvas.p[y][x + 1] & 15 if x + 1 < w else 0
            row.append((a << 4) | b)
        row += bytes(row_bytes - len(row))
        pixels += row
    pal_bytes = b"".join(struct.pack("<BBBB", b, g, r, 0) for (r, g, b) in pal)
    offset = 14 + 40 + len(pal_bytes)
    header = struct.pack("<2sIHHI", b"BM", offset + len(pixels), 0, 0, offset)
    info = struct.pack("<IiiHHIIiiII", 40, w, h, 1, 4, 0, len(pixels), 2835, 2835, 16, 16)
    with open(os.path.join(OUT, name + ".bmp"), "wb") as f:
        f.write(header + info + pal_bytes + pixels)
    with open(os.path.join(OUT, name + ".json"), "w") as f:
        json.dump(meta, f, indent=4)


def strip(frames, fw, fh):
    c = Canvas(fw, fh * len(frames))
    for i, fr in enumerate(frames):
        c.blit(fr, 0, i * fh)
    return c


# ---------- wizard 16x32: 0 idle down, 1-2 walk down, 3 idle up, 4-5 walk up, 6 idle side, 7-8 walk side,
# 9 hurt, 10 dead
def wizard(view, step, hurt=False, dead=False):
    c = Canvas(16, 32)
    if dead:
        c.ellipse(8, 27, 7, 3, 3)
        c.ellipse(4, 26, 3, 2, 2)
        c.rect(10, 25, 12, 27, 5)
        c.outline()
        return c
    bob = 1 if step else 0
    robe = 14 if hurt else 3
    # robe body
    for y in range(16, 30):
        half = 3 + (y - 16) // 3
        c.rect(8 - half, y, 7 + half, y, robe)
    c.rect(5, 18, 10, 19, 2)  # belt
    # legs/feet
    if step == 1:
        c.rect(5, 29, 6, 30, 7)
    elif step == 2:
        c.rect(9, 29, 10, 30, 7)
    else:
        c.rect(5, 30, 6, 30, 7)
        c.rect(9, 30, 10, 30, 7)
    # head
    hy = 12 - bob
    c.ellipse(8, hy + 1, 3.5, 3.5, 5)
    if view == "down":
        c.set(6, hy + 1, 1)
        c.set(9, hy + 1, 1)
        c.rect(6, hy + 3, 9, hy + 6, 6)  # beard
    elif view == "side":
        c.set(10, hy + 1, 1)
        c.rect(9, hy + 3, 11, hy + 5, 6)
    else:
        c.ellipse(8, hy + 1, 3.5, 3.5, 2)
    # hat
    for i in range(9):
        y = hy - 2 - i
        half = max(0, 5 - i * 5 // 8)
        tip = 1 if view == "side" else 0
        c.rect(8 - half + (i * tip) // 3, y, 7 + half + (i * tip) // 3, y, 2 if i % 4 else 3)
    c.rect(2, hy - 2, 13, hy - 1, 2)
    c.set(8, hy - 6, 8)
    # staff
    if view != "up":
        sx = 13 if view == "down" else 3
        c.rect(sx, 14 - bob, sx, 28, 7)
        c.ellipse(sx + 0.5, 13 - bob, 1.6, 1.6, 13)
    c.outline()
    return c


wiz_frames = [wizard("down", 0), wizard("down", 1), wizard("down", 2),
              wizard("up", 0), wizard("up", 1), wizard("up", 2),
              wizard("side", 0), wizard("side", 1), wizard("side", 2),
              wizard("down", 0, hurt=True), wizard("down", 0, dead=True)]
write_bmp("tmp_wizard", strip(wiz_frames, 16, 32), SPR_PAL, {"type": "sprite", "height": 32})


# ---------- enemies 16x16, 2 frames each
def slime(f, big=1):
    s = 16 * big
    c = Canvas(s, s)
    rx = (6.5 if f == 0 else 7.2) * big
    ry = (5.0 if f == 0 else 4.2) * big
    cy = s - ry - 1.5 * big
    c.ellipse(s / 2, cy, rx, ry, 10)
    c.ellipse(s / 2 - 2 * big, cy - 2 * big, 2 * big, 1.4 * big, 11)
    for ex in (-2, 2):
        c.rect(int(s / 2 + ex * big) - big // 2, int(cy) - big // 2, int(s / 2 + ex * big), int(cy) + big - 1, 1)
    c.outline()
    return c


def bat(f):
    c = Canvas(16, 16)
    c.ellipse(8, 8, 3, 3, 2)
    wy = 4 if f == 0 else 10
    for side in (-1, 1):
        for i in range(5):
            x = 8 + side * (3 + i)
            y0 = 7 - (i * (8 - wy)) // 5 if f == 0 else 7 + i // 2
            c.rect(x, min(y0, 8), x, max(y0, 8), 3)
    c.set(7, 7, 14)
    c.set(9, 7, 14)
    c.set(6, 4, 2)
    c.set(10, 4, 2)
    c.outline()
    return c


def skeleton(f):
    c = Canvas(16, 16)
    c.ellipse(8, 4, 3.5, 3.5, 15)
    c.set(6, 4, 1)
    c.set(9, 4, 1)
    c.rect(7, 7, 8, 7, 6)
    c.rect(6, 8, 9, 11, 15)
    c.rect(6, 9, 9, 9, 1)
    leg = 1 if f else 0
    c.rect(6 - leg, 12, 6 - leg, 14, 15)
    c.rect(9 + leg, 12, 9 + leg, 14, 15)
    c.rect(4, 8 + leg, 4, 11, 15)
    c.rect(11, 9 - leg, 11, 11, 15)
    c.outline()
    return c


write_bmp("tmp_slime", strip([slime(0), slime(1)], 16, 16), SPR_PAL, {"type": "sprite", "height": 16})
write_bmp("tmp_bat", strip([bat(0), bat(1)], 16, 16), SPR_PAL, {"type": "sprite", "height": 16})
write_bmp("tmp_skeleton", strip([skeleton(0), skeleton(1)], 16, 16), SPR_PAL, {"type": "sprite", "height": 16})


def slime_king(f):
    c = slime(f, 4)
    # crown
    cy = 64 - (5.0 if f == 0 else 4.2) * 4 * 2 - 6
    base = int(cy) + 4
    c.rect(22, base, 41, base + 4, 8)
    for x in (22, 29, 36):
        c.rect(x, base - 5, x + 5, base, 8)
    c.set(31, base + 2, 14)
    c.outline()
    return c


write_bmp("tmp_slime_king", strip([slime_king(0), slime_king(1)], 64, 64), SPR_PAL,
          {"type": "sprite", "height": 64})


# ---------- 8x8 pickups and fx
def px8(draw):
    c = Canvas(8, 8)
    draw(c)
    return c


def gem(col, light):
    def d(c):
        c.ellipse(4, 4, 2.6, 3.5, col)
        c.set(3, 2, light)
        c.set(3, 3, light)
        c.outline()
    return d


def coin(c):
    c.ellipse(4, 4, 3, 3, 8)
    c.rect(4, 2, 4, 5, 7)
    c.outline()


items8 = [px8(gem(12, 13)), px8(gem(10, 11)), px8(gem(14, 6)), px8(coin)]
write_bmp("tmp_pickups", strip(items8, 8, 8), SPR_PAL, {"type": "sprite", "height": 8})


def spark(c):
    c.ellipse(4, 4, 2.5, 2.5, 8)
    c.ellipse(4, 4, 1.2, 1.2, 6)


def rune(c):
    c.ellipse(4, 4, 3.2, 3.2, 3)
    c.rect(3, 2, 4, 5, 4)
    c.rect(2, 3, 5, 4, 4)
    c.outline()


def shard(c):
    c.ellipse(4, 4, 2, 3.4, 13)
    c.rect(4, 2, 4, 5, 6)


def poof_a(c):
    c.ellipse(4, 4, 3, 3, 15)
    c.ellipse(4, 4, 1.5, 1.5, 6)


def poof_b(c):
    for x, y in ((1, 1), (6, 1), (1, 6), (6, 6), (4, 4)):
        c.set(x, y, 15)


def trail(c):
    c.set(4, 4, 6)
    c.set(3, 4, 13)
    c.set(5, 4, 13)
    c.set(4, 3, 13)
    c.set(4, 5, 13)


def arrow_r(c):
    for i in range(4):
        c.rect(3 + i, 4 - (3 - i), 3 + i, 3 + (3 - i), 8)
    c.rect(0, 3, 2, 4, 8)


def arrow_d(c):
    for i in range(4):
        c.rect(4 - (3 - i), 3 + i, 3 + (3 - i), 3 + i, 8)
    c.rect(3, 0, 4, 2, 8)


def arrow_dr(c):
    for i in range(7):
        c.set(i, i, 8)
    c.rect(4, 7, 7, 7, 8)
    c.rect(7, 4, 7, 7, 8)
    c.rect(5, 6, 6, 6, 8)
    c.rect(6, 5, 6, 6, 8)


def warn(c):
    c.rect(3, 0, 4, 4, 14)
    c.rect(3, 6, 4, 7, 14)


def hitspark(c):
    for i in range(8):
        c.set(i, i, 6)
        c.set(7 - i, i, 6)
    c.ellipse(4, 4, 1.5, 1.5, 8)


fx = [px8(spark), px8(rune), px8(shard), px8(poof_a), px8(poof_b), px8(trail), px8(arrow_r), px8(arrow_d),
      px8(arrow_dr), px8(warn), px8(hitspark)]
write_bmp("tmp_fx", strip(fx, 8, 8), SPR_PAL, {"type": "sprite", "height": 8})

# ---------- shadows (blended)
sh = Canvas(16, 8)
sh.ellipse(8, 4, 6, 2.5, 1)
write_bmp("tmp_shadow", sh, SPR_PAL, {"type": "sprite", "height": 8})
shb = Canvas(64, 32)
shb.ellipse(32, 16, 26, 10, 1)
write_bmp("tmp_shadow_big", shb, SPR_PAL, {"type": "sprite", "height": 32})

# shockwave ring 64x64 (2 frames)
def ring(r):
    c = Canvas(64, 64)
    c.ellipse(32, 32, r, r * 0.6, 13)
    c.ellipse(32, 32, r - 3, r * 0.6 - 3, 0)
    return c


write_bmp("tmp_wave", strip([ring(20), ring(31)], 64, 64), SPR_PAL, {"type": "sprite", "height": 64})


# ---------- bars 32x8, frame N = N px filled (0..32)
def bars(fill_col, back_col, h0, h1):
    frames = []
    for n in range(33):
        c = Canvas(32, 8)
        c.rect(0, h0 - 1, 31, h1 + 1, 1)
        c.rect(0, h0, 31, h1, back_col)
        if n:
            c.rect(0, h0, n - 1, h1, fill_col)
        frames.append(c)
    return strip(frames, 32, 8)


write_bmp("tmp_xpbar", bars(13, 2, 3, 5), SPR_PAL, {"type": "sprite", "height": 8})
write_bmp("tmp_hpbar", bars(10, 14, 3, 4), SPR_PAL, {"type": "sprite", "height": 8})
write_bmp("tmp_bossbar", bars(14, 2, 2, 5), SPR_PAL, {"type": "sprite", "height": 8})


# ---------- icons 16x16: 0 spark, 1 rune, 2 frost, 3 boots, 4 heart, 5 heal, 6 gold, 7 bless dmg,
# 8 bless speed, 9 bless magnet, 10 chest, 11 shrine idle, 12 shrine lit
def icon(draw):
    c = Canvas(16, 16)
    draw(c)
    c.outline()
    return c


def i_spark(c):
    for i in range(10):
        c.rect(3 + i, 12 - i, 4 + i, 12 - i, 8)
    c.ellipse(12, 4, 2.5, 2.5, 6)


def i_rune(c):
    c.ellipse(8, 8, 6, 6, 3)
    c.ellipse(8, 8, 3.5, 3.5, 4)
    c.rect(7, 4, 8, 11, 6)


def i_frost(c):
    for a in range(0, 360, 60):
        for r in range(6):
            c.set(int(8 + math.cos(math.radians(a)) * r), int(8 + math.sin(math.radians(a)) * r), 13)
    c.ellipse(8, 8, 1.5, 1.5, 6)


def i_boots(c):
    c.rect(5, 3, 9, 10, 7)
    c.rect(5, 10, 13, 13, 7)
    c.rect(5, 3, 9, 4, 8)


def i_heart(c):
    c.ellipse(5.5, 6, 3.2, 3.2, 14)
    c.ellipse(10.5, 6, 3.2, 3.2, 14)
    for y in range(7, 14):
        c.rect(2 + (y - 7), y, 13 - (y - 7), y, 14)
    c.set(5, 5, 6)


def i_heal(c):
    c.rect(6, 2, 9, 13, 10)
    c.rect(2, 6, 13, 9, 10)


def i_gold(c):
    c.ellipse(8, 8, 5.5, 5.5, 8)
    c.rect(7, 4, 8, 11, 7)


def i_bdmg(c):
    for i in range(10):
        c.set(3 + i, 12 - i, 15)
        c.set(4 + i, 12 - i, 6)
    c.rect(2, 10, 6, 11, 7)


def i_bspd(c):
    for k in range(3):
        c.rect(2 + k * 4, 4, 3 + k * 4, 11, 13)
        c.rect(4 + k * 4, 6, 4 + k * 4, 9, 13)


def i_bmag(c):
    c.rect(3, 3, 5, 12, 14)
    c.rect(10, 3, 12, 12, 14)
    c.rect(3, 10, 12, 12, 14)
    c.rect(3, 3, 5, 4, 6)
    c.rect(10, 3, 12, 4, 6)


def i_chest(c):
    c.rect(2, 5, 13, 13, 7)
    c.rect(2, 8, 13, 8, 1)
    c.rect(7, 7, 8, 10, 8)


def shrine(lit):
    def d(c):
        c.rect(4, 6, 11, 14, 15)
        c.rect(3, 13, 12, 14, 15)
        c.ellipse(8, 4, 3, 3, 13 if lit else 4)
        if lit:
            c.set(8, 4, 6)
    return d


icons = [icon(i_spark), icon(i_rune), icon(i_frost), icon(i_boots), icon(i_heart), icon(i_heal), icon(i_gold),
         icon(i_bdmg), icon(i_bspd), icon(i_bmag), icon(i_chest), icon(shrine(False)), icon(shrine(True))]
write_bmp("tmp_icons", strip(icons, 16, 16), SPR_PAL, {"type": "sprite", "height": 16})


# ---------- card 64x64 (2 frames: normal, selected)
def card(sel):
    c = Canvas(64, 64)
    c.rect(1, 1, 62, 62, 8 if sel else 4)
    c.rect(3, 3, 60, 60, 2)
    c.rect(4, 4, 59, 59, 1)
    return c


write_bmp("tmp_card", strip([card(False), card(True)], 64, 64), SPR_PAL, {"type": "sprite", "height": 64})

# ---------- meadow bg tiles (8x8 each, vertical strip)
# 0 empty, 1-4 ground, 5-8 rock, 9-12 stump, 13-16 bush, 17-20 pillar, 21-24 pond, 25 flower a, 26 flower b,
# 27 grass, 28 mushroom
tiles = [Canvas(8, 8) for _ in range(29)]


def ground(c, seed):
    import random
    r = random.Random(seed)
    for y in range(8):
        for x in range(8):
            c.p[y][x] = 2
    for _ in range(6):
        c.p[r.randrange(8)][r.randrange(8)] = r.choice((1, 3, 4))
    if seed == 3:
        c.p[3][2] = 3
        c.p[2][2] = 3
        c.p[4][5] = 1


for i in range(4):
    ground(tiles[1 + i], i)


def big(draw, first):
    c = Canvas(16, 16)
    draw(c)
    for ty in range(2):
        for tx in range(2):
            t = tiles[first + ty * 2 + tx]
            for y in range(8):
                for x in range(8):
                    t.p[y][x] = c.p[ty * 8 + y][tx * 8 + x]


def rock(c):
    c.ellipse(8, 9, 7, 6, 6)
    c.ellipse(6, 7, 3, 2, 7)
    c.outline(5)


def stump(c):
    c.rect(3, 6, 12, 14, 8)
    c.ellipse(7.5, 6, 5, 2.5, 9)
    c.ellipse(7.5, 6, 2, 1, 8)
    c.outline(5)


def bush(c):
    c.ellipse(5, 9, 4.5, 4.5, 11)
    c.ellipse(11, 9, 4.5, 4.5, 11)
    c.ellipse(8, 6, 4.5, 4.5, 11)
    c.ellipse(7, 5, 1.5, 1, 3)
    c.outline(10)


def pillar(c):
    c.rect(4, 1, 11, 15, 6)
    c.rect(3, 13, 12, 15, 7)
    c.rect(3, 0, 12, 2, 7)
    c.rect(6, 4, 6, 10, 5)
    c.outline(5)


def pond(c):
    c.ellipse(8, 8, 7.5, 6.5, 12)
    c.ellipse(6, 6, 3, 1.2, 13)
    c.outline(4)


big(rock, 5)
big(stump, 9)
big(bush, 13)
big(pillar, 17)
big(pond, 21)
for x, y in ((2, 2), (5, 5)):
    tiles[25].set(x, y, 14)
    tiles[26].set(x + 1, y - 1, 15)
for x in (2, 4, 6):
    tiles[27].rect(x, 4, x, 7, 3)
tiles[28].rect(3, 4, 4, 7, 14 - 0)
tiles[28].rect(2, 3, 5, 4, 15)
write_bmp("tmp_meadow_tiles", strip(tiles, 8, 8), BG_PAL, {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})
write_bmp("tmp_meadow_palette", Canvas(8, 8), BG_PAL,
          {"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16})
print("ok")
