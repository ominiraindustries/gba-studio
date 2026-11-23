"""Seed greybox graphics (gb_*) for 004_disco_vendetta.

Strict top-down view: no fronts, no depth edges, no cast shadows. Characters are one frame drawn facing east and
rotated in hardware (affine sprites); frame 1 knocked out, frame 2 white hit flash, frame 3 dead (not rotated).
Map tiles, splat tiles (floor blood layer), bullets, aim dots, pickups, blood particles, combo bar and arrow.
Run `python -I projects/004_disco_vendetta/tools/gen_greybox.py`. Only writes graphics/gb_*; never touches art/.
"""
import json
import math
import random
import struct
from pathlib import Path

GFX = Path(__file__).resolve().parent.parent / "graphics"
MAGENTA = (255, 0, 255)


def write_bmp(path, width, height, palette, pixels):
    pal = list(palette) + [(0, 0, 0)] * (16 - len(palette))
    row_bytes = ((width * 4 + 31) // 32) * 4
    data = bytearray()
    for y in range(height - 1, -1, -1):
        row = bytearray(row_bytes)
        for x in range(width):
            row[x // 2] |= (pixels[y][x] & 15) << (4 if x % 2 == 0 else 0)
        data += row
    pal_bytes = b"".join(struct.pack("<BBBB", b, g, r, 0) for (r, g, b) in pal)
    offset = 14 + 40 + len(pal_bytes)
    header = struct.pack("<2sIHHI", b"BM", offset + len(data), 0, 0, offset)
    info = struct.pack("<IiiHHIIiiII", 40, width, height, 1, 4, 0, len(data), 2835, 2835, 16, 0)
    path.write_bytes(header + info + pal_bytes + data)


def save(name, width, height, palette, pixels, meta):
    write_bmp(GFX / (name + ".bmp"), width, height, palette, pixels)
    (GFX / (name + ".json")).write_text(json.dumps(meta, indent=4) + "\n")


def blank(w, h):
    return [[0] * w for _ in range(h)]


def character(body, skin, arm):
    """4 frames of 16x16: 0 facing east (rotated in hardware), 1 knocked, 2 white flash, 3 dead.
    Indices: 1 outline, 2 body, 3 skin (head), 4 arm/weapon, 5 blood, 6 white."""
    pal = [MAGENTA, (25, 15, 10), body, skin, arm, (110, 15, 15), (255, 255, 255)]
    base = blank(16, 16)
    for y in range(16):
        for x in range(16):
            cx, cy = x + 0.5 - 8, y + 0.5 - 8
            v = (cx / 3.2) ** 2 + (cy / 5.6) ** 2   # shoulders, wide across the facing direction
            if v <= 1:
                base[y][x] = 1 if v > 0.6 else 2
    for x in range(10, 15):                         # arm forward from the right shoulder
        base[10][x] = 4
    for y in range(16):
        for x in range(16):
            if (x + 0.5 - 8.8) ** 2 + (y + 0.5 - 8) ** 2 <= 2.6 ** 2:
                base[y][x] = 3
    knocked = blank(16, 16)                         # lying flat: long body, head to the side
    for y in range(16):
        for x in range(16):
            cx, cy = x + 0.5 - 7, y + 0.5 - 8
            v = (cx / 6) ** 2 + (cy / 3.5) ** 2
            if v <= 1:
                knocked[y][x] = 1 if v > 0.6 else 2
            if (x + 0.5 - 13) ** 2 + (y + 0.5 - 8) ** 2 <= 2.3 ** 2:
                knocked[y][x] = 3
    white = [[6 if p else 0 for p in row] for row in base]
    dead = blank(16, 16)
    for y in range(16):
        for x in range(16):
            cx, cy = x + 0.5 - 8, y + 0.5 - 8
            if (cx / 7) ** 2 + (cy / 5) ** 2 <= 1:
                dead[y][x] = 5
            if (cx / 5) ** 2 + (cy / 3) ** 2 <= 1:
                dead[y][x] = 2
            if (x + 0.5 - 12) ** 2 + (y + 0.5 - 8) ** 2 <= 2.2 ** 2:
                dead[y][x] = 3
    pixels = [row for f in (base, knocked, white, dead) for row in f]
    return pal, pixels


def strip(tiles, size=8):
    w = size * len(tiles)
    px = blank(w, size)
    for i, tile in enumerate(tiles):
        for y in range(size):
            for x in range(size):
                px[y][i * size + x] = tile[y][x]
    return w, px


def main():
    random.seed(1975)
    # Readability: player white-cream, thug dark maroon, gunman blue (none alike).
    for name, body, arm in (("gb_player", (245, 238, 215), (60, 60, 70)),
                            ("gb_thug", (140, 25, 45), (70, 15, 25)),
                            ("gb_gunman", (50, 90, 200), (25, 30, 60))):
        pal, px = character(body, (230, 180, 140), arm)
        save(name, 16, 64, pal, px, {"type": "sprite", "height": 16})

    # Bullets 8x8: 0 player (yellow), 1 enemy (white core, red rim), 2 muzzle flash, 3 player trail,
    # 4 enemy trail (red; drawn one frame behind the bullet).
    pal = [MAGENTA, (255, 230, 40), (210, 160, 0), (255, 255, 255), (230, 20, 20), (255, 255, 230)]
    frames = []
    for core, halo in ((1, 2), (3, 4)):
        f = blank(8, 8)
        for y in range(2, 6):
            for x in range(2, 6):
                f[y][x] = core if 3 <= x <= 4 and 3 <= y <= 4 else halo
        frames.append(f)
    f = blank(8, 8)
    for y in range(8):
        for x in range(8):
            d = abs(x - 3.5) + abs(y - 3.5)
            f[y][x] = 5 if d <= 2 else (1 if d <= 3.5 else 0)
    frames.append(f)
    for halo in (2, 4):
        f = blank(8, 8)
        for y in range(3, 5):
            for x in range(3, 5):
                f[y][x] = halo
        frames.append(f)
    save("gb_bullet", 8, 40, pal, [r for f in frames for r in f], {"type": "sprite", "height": 8})

    # Aim dots 8x8 (a plain 2x2 dot): 0 fists (white), 1 pistol (cream), 2 locked on an enemy (red).
    pal = [MAGENTA, (255, 255, 255), (255, 240, 200), (255, 30, 30)]
    frames = []
    for c in (1, 2, 3):
        f = blank(8, 8)
        for y in range(3, 5):
            for x in range(3, 5):
                f[y][x] = c
        frames.append(f)
    save("gb_dot", 8, 24, pal, [r for f in frames for r in f], {"type": "sprite", "height": 8})

    # Weapon pickups 16x16: 0 pistol, 1 bat, 2 empty pistol (grey).
    pal = [MAGENTA, (30, 30, 35), (150, 150, 160), (150, 100, 50), (255, 255, 255), (90, 90, 90), (60, 60, 60)]
    px = blank(16, 48)
    for base, (dark, light) in ((0, (1, 2)), (32, (6, 5))):
        for y in range(6, 9):
            for x in range(3, 13):
                px[base + y][x] = light if y > 6 else dark
        for y in range(9, 13):
            for x in range(9, 12):
                px[base + y][x] = dark
    for i in range(12):
        px[16 + 2 + i][3 + i] = 3
        px[16 + 2 + i][4 + i] = 3
    save("gb_pickups", 16, 48, pal, px, {"type": "sprite", "height": 16})

    # Blood particles 8x8: 0 big drop, 1 medium, 2 small.
    pal = [MAGENTA, (90, 5, 5), (170, 20, 20)]
    frames = []
    for r in (2.2, 1.5, 0.9):
        f = blank(8, 8)
        for y in range(8):
            for x in range(8):
                d = math.hypot(x + 0.5 - 4, y + 0.5 - 4)
                if d <= r:
                    f[y][x] = 2 if d <= r - 0.8 else 1
        frames.append(f)
    save("gb_blood", 8, 24, pal, [r for f in frames for r in f], {"type": "sprite", "height": 8})

    # Combo bar 32x8, 17 frames: 0..32 px filled in 2 px steps.
    pal = [MAGENTA, (20, 10, 10), (255, 170, 40), (80, 50, 30)]
    frames = []
    for n in range(17):
        f = blank(32, 8)
        for y in range(2, 6):
            for x in range(32):
                f[y][x] = 1 if y in (2, 5) or x in (0, 31) else (2 if x < n * 2 else 3)
        frames.append(f)
    save("gb_bar", 32, 136, pal, [r for f in frames for r in f], {"type": "sprite", "height": 8})

    # Arrow 16x16 pointing east (rotated in hardware).
    pal = [MAGENTA, (255, 200, 40), (60, 20, 0)]
    f = blank(16, 16)
    for y in range(16):
        for x in range(16):
            cx, cy = x + 0.5 - 8, abs(y + 0.5 - 8)
            if (-6 <= cx <= 6 and cy <= 1.5) or (0 <= cx <= 7 and cy <= 7 - cx):
                f[y][x] = 1
    for y in range(16):
        for x in range(16):
            if f[y][x] == 0 and any(0 <= y + dy < 16 and 0 <= x + dx < 16 and f[y + dy][x + dx] == 1
                                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                f[y][x] = 2
    save("gb_arrow", 16, 16, pal, f, {"type": "sprite", "height": 16})

    # Map tiles 8x8, flat top-down (no fronts, no depth edges). Two BGs share this set: floor layer and splats.
    #  0 empty (transparent)  1 floor  2 floor checker (unused)  3 wall plain  4 wall edge top  5 wall edge left
    #  6 wall edge top+left (flip for the other sides)  7 door plank vertical  8 door plank horizontal
    #  9 window horizontal  10 window vertical  11 bar/table top  12 open doorway  13 exit closed  14 exit open
    # 15-20 splats  21 puddle quarter (flip for the 4 cells)
    pal = [(12, 8, 6), (120, 70, 45), (105, 60, 38), (55, 38, 30), (20, 12, 8), (190, 130, 60),
           (130, 190, 210), (150, 95, 50), (100, 60, 30), (90, 5, 5), (150, 15, 15), (60, 200, 90),
           (200, 50, 40), (110, 160, 180)]
    F, W, O, DOOR, GLASS, BAR, BARD, BL1, BL2, GREEN, RED, FRAME = 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13

    def fill(c):
        return [[c] * 8 for _ in range(8)]

    tiles = [fill(0), fill(F), [[2 if (x // 4 + y // 4) % 2 else 1 for x in range(8)] for y in range(8)], fill(W)]
    tiles.append([[O if y == 0 else W for x in range(8)] for y in range(8)])
    tiles.append([[O if x == 0 else W for x in range(8)] for y in range(8)])
    tiles.append([[O if x == 0 or y == 0 else W for x in range(8)] for y in range(8)])
    tiles.append([[DOOR if x >= 6 else F for x in range(8)] for y in range(8)])  # plank on the cell's right side
    tiles.append([[DOOR if y >= 6 else F for x in range(8)] for y in range(8)])  # plank on the cell's bottom side
    tiles.append([[FRAME if y == 5 else (GLASS if y >= 6 else F) for x in range(8)] for y in range(8)])
    tiles.append([[FRAME if x == 5 else (GLASS if x >= 6 else F) for x in range(8)] for y in range(8)])
    tiles.append([[BARD if x == 0 or y == 0 or x == 7 or y == 7 else BAR for x in range(8)] for y in range(8)])
    tiles.append([[BARD if (x + y) % 7 == 0 else F for x in range(8)] for y in range(8)])

    def cross(c):
        return [[c if x == y or x == 7 - y else (O if x in (0, 7) or y in (0, 7) else F) for x in range(8)]
                for y in range(8)]

    tiles.append(cross(RED))
    tiles.append(cross(GREEN))
    for i in range(6):  # splats: random blobs over transparent
        t = fill(0)
        cx, cy = random.uniform(2.5, 5.5), random.uniform(2.5, 5.5)
        for _ in range(3 + i % 3):
            bx, by, br = cx + random.uniform(-2, 2), cy + random.uniform(-2, 2), random.uniform(0.8, 2.2)
            for y in range(8):
                for x in range(8):
                    if math.hypot(x + 0.5 - bx, y + 0.5 - by) <= br:
                        t[y][x] = BL2 if random.random() < 0.6 else BL1
        tiles.append(t)
    t = fill(0)  # puddle: top-left quarter of a 16x16 disc centred at (8,8)
    for y in range(8):
        for x in range(8):
            d = math.hypot(x + 0.5 - 8, y + 0.5 - 8)
            if d <= 7.5:
                t[y][x] = BL1 if d > 6 else BL2
    tiles.append(t)
    w, px = strip(tiles)
    save("gb_tiles", w, 8, pal, px, {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})
    save("gb_tiles_palette", 16, 8, pal, [[i for i in range(16)] for _ in range(8)],
         {"type": "bg_palette", "bpp_mode": "bpp_4"})
    print("greybox graphics written")


if __name__ == "__main__":
    main()
