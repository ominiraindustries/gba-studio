"""Seed greybox graphics (gb_*) for 003_poket_gto: felt table, card face/back, suit icons, button cursor.

Run `python -I projects/003_poket_gto/tools/gen_greybox.py`. Only writes graphics/gb_*.bmp/.json; never touches art/.
"""
import json
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


# 4-colour deck: spades black, hearts red, diamonds blue, clubs green (readable at 1x).
SUIT_SHAPES_8 = {
    "s": ["...#....", "..###...", ".#####..", "#######.", "#######.", "..#.#...", ".#####..", "........"],
    "h": [".##.##..", "#######.", "#######.", "#######.", ".#####..", "..###...", "...#....", "........"],
    "d": ["...#....", "..###...", ".#####..", "#######.", ".#####..", "..###...", "...#....", "........"],
    "c": ["..###...", "..###...", "##.#.##.", "#######.", "##.#.##.", "...#....", "..###...", "........"],
}
SUIT_ORDER = "shdc"   # frame index = suit index in the game


def main():
    # Suit icons: one palette, colour index per suit (1 black, 2 red, 3 blue, 4 green).
    pal = [MAGENTA, (20, 20, 30), (210, 30, 40), (30, 90, 220), (20, 150, 60)]
    px = blank(8, 32)
    for i, s in enumerate(SUIT_ORDER):
        for y, row in enumerate(SUIT_SHAPES_8[s]):
            for x, c in enumerate(row):
                if c == "#":
                    px[i * 8 + y][x] = i + 1
    save("gb_suits", 8, 32, pal, px, {"type": "sprite", "height": 8})

    # Big suit icons 16x16 (the 8x8 shapes scaled x2).
    px = blank(16, 64)
    for i, s in enumerate(SUIT_ORDER):
        for y, row in enumerate(SUIT_SHAPES_8[s]):
            for x, c in enumerate(row):
                if c == "#":
                    for dy in range(2):
                        for dx in range(2):
                            px[i * 16 + y * 2 + dy][x * 2 + dx] = i + 1
    save("gb_suits_big", 16, 64, pal, px, {"type": "sprite", "height": 16})

    # Card face 24x32 inside a 32x32 sprite (left aligned at x 4). 1 white, 2 border grey, 3 shadow.
    pal = [MAGENTA, (250, 250, 245), (90, 90, 100), (40, 50, 40)]
    px = blank(32, 32)
    for y in range(32):
        for x in range(4, 28):
            corner = (x in (4, 27)) and (y in (0, 31))
            if corner:
                continue
            px[y][x] = 2 if x in (4, 27) or y in (0, 31) else 1
    save("gb_card", 32, 32, pal, px, {"type": "sprite", "height": 32})

    # Card back: red with white frame and a diamond lattice.
    pal = [MAGENTA, (250, 250, 245), (180, 40, 50), (120, 20, 30)]
    px = blank(32, 32)
    for y in range(32):
        for x in range(4, 28):
            if (x in (4, 27)) and (y in (0, 31)):
                continue
            if x in (4, 27) or y in (0, 31):
                px[y][x] = 1
            else:
                px[y][x] = 3 if (x + y) % 4 == 0 or (x - y) % 4 == 0 else 2
    save("gb_card_back", 32, 32, pal, px, {"type": "sprite", "height": 32})

    # Selection cursor for action buttons: 64x16 corner frame in the middle of a 64x32 sprite.
    pal = [MAGENTA, (255, 220, 60), (120, 80, 0)]
    px = blank(64, 32)
    for y in range(16):
        for x in range(64):
            edge = x in (0, 1, 62, 63) or y in (0, 1, 14, 15)
            near_corner = (x < 10 or x > 53) and (y < 6 or y > 9)
            if edge and near_corner:
                px[y + 8][x] = 1
    save("gb_cursor", 64, 32, pal, px, {"type": "sprite", "height": 32})

    # Felt table background 256x256: dark room, green oval table with a wood rail.
    pal = [(0, 0, 0), (25, 30, 40), (30, 110, 60), (25, 95, 50), (110, 70, 40), (80, 50, 30), (35, 125, 70)]
    px = blank(256, 256)
    cx, cy, rx, ry = 120, 70, 116, 66
    for y in range(256):
        for x in range(256):
            if y >= 160 or x >= 240:
                px[y][x] = 1
                continue
            d = ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2
            if d <= 0.86:
                px[y][x] = 6 if d < 0.35 else (2 if (x + y) % 7 else 3)
            elif d <= 1.0:
                px[y][x] = 4 if d < 0.93 else 5
            else:
                px[y][x] = 1
    save("gb_table", 256, 256, pal, px, {"type": "regular_bg", "bpp_mode": "bpp_4"})
    print("greybox graphics written")


if __name__ == "__main__":
    main()
