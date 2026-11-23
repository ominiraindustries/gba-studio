"""Seed greybox graphics (gb_*) for 004_disco_vendetta.

Top-down characters pre-rendered in 16 aim directions (frame d = d * 22.5 degrees clockwise from east) plus a
"dead" frame 16; map tiles; bullet; weapon pickups. Run `python -I projects/004_disco_vendetta/tools/gen_greybox.py`.
Only writes graphics/gb_*; never touches art/.
"""
import json
import math
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


def character(body, dark, skin, arm):
    """17 frames of 16x16: 16 aim directions + dead. 1 outline, 2 body, 3 skin (head), 4 arm/weapon."""
    pal = [MAGENTA, (25, 15, 10), body, skin, arm, dark]
    frames = []
    for d in range(16):
        a = math.radians(d * 22.5)
        fx, fy = math.cos(a), math.sin(a)
        px = [[0] * 16 for _ in range(16)]
        for y in range(16):
            for x in range(16):
                cx, cy = x + 0.5 - 8, y + 0.5 - 8
                # Shoulders: an ellipse wide across the facing direction.
                along = cx * fx + cy * fy
                across = -cx * fy + cy * fx
                if (along / 3.2) ** 2 + (across / 5.6) ** 2 <= 1:
                    px[y][x] = 1 if (along / 3.2) ** 2 + (across / 5.6) ** 2 > 0.6 else 2
        # Arm/weapon line forward from the right shoulder.
        for t in range(3, 8):
            x = int(round(8 + fx * t - fy * 2.5 - 0.5))
            y = int(round(8 + fy * t + fx * 2.5 - 0.5))
            if 0 <= x < 16 and 0 <= y < 16:
                px[y][x] = 4
        # Head slightly forward.
        hx, hy = 8 + fx * 0.8, 8 + fy * 0.8
        for y in range(16):
            for x in range(16):
                if (x + 0.5 - hx) ** 2 + (y + 0.5 - hy) ** 2 <= 2.6 ** 2:
                    px[y][x] = 3
        frames.append(px)
    # Dead: flattened body with a dark pool.
    px = [[0] * 16 for _ in range(16)]
    for y in range(16):
        for x in range(16):
            cx, cy = x + 0.5 - 8, y + 0.5 - 8
            if (cx / 7) ** 2 + (cy / 5) ** 2 <= 1:
                px[y][x] = 5
            if (cx / 5) ** 2 + (cy / 3) ** 2 <= 1:
                px[y][x] = 2
    frames.append(px)
    pixels = [row for f in frames for row in f]
    pal[5] = (110, 15, 15)
    return pal, pixels


def main():
    for name, body, arm in (("gb_player", (235, 225, 200), (60, 60, 70)),
                            ("gb_thug", (150, 85, 40), (200, 170, 120)),
                            ("gb_gunman", (60, 60, 90), (40, 40, 45))):
        pal, px = character(body, None, (230, 180, 140), arm)
        save(name, 16, 16 * 17, pal, px, {"type": "sprite", "height": 16})

    # Bullet 8x8: 2x2 bright core with a dim halo. Frame 0 player (yellow), frame 1 enemy (red) via indices.
    pal = [MAGENTA, (255, 240, 120), (200, 140, 40), (255, 90, 70), (160, 30, 20)]
    px = [[0] * 8 for _ in range(16)]
    for f, (core, halo) in enumerate(((1, 2), (3, 4))):
        for y in range(2, 6):
            for x in range(2, 6):
                px[f * 8 + y][x] = core if 3 <= x <= 4 and 3 <= y <= 4 else halo
    save("gb_bullet", 8, 16, pal, px, {"type": "sprite", "height": 8})

    # Weapon pickups 16x16: frame 0 pistol, frame 1 bat.
    pal = [MAGENTA, (30, 30, 35), (150, 150, 160), (150, 100, 50), (255, 255, 255)]
    px = [[0] * 16 for _ in range(32)]
    for y in range(6, 9):
        for x in range(3, 13):
            px[y][x] = 2 if y > 6 else 1
    for y in range(9, 13):
        for x in range(9, 12):
            px[y][x] = 1
    for i in range(12):
        px[16 + 2 + i][3 + i] = 3
        px[16 + 2 + i][4 + i] = 3
    save("gb_pickups", 16, 32, pal, px, {"type": "sprite", "height": 16})

    # Map tiles 8x8 (regular_bg_tiles): 0 void, 1 floor A, 2 floor B, 3 wall, 4 wall top edge, 5 door.
    pal = [(12, 8, 6), (120, 70, 45), (105, 60, 38), (45, 30, 25), (85, 55, 40), (170, 110, 50), (60, 40, 25)]
    tiles = []
    t = [[1] * 8 for _ in range(8)]
    tiles.append([[0] * 8 for _ in range(8)])
    tiles.append(t)
    tiles.append([[2 if (x // 4 + y // 4) % 2 else 1 for x in range(8)] for y in range(8)])
    tiles.append([[3] * 8 for _ in range(8)])
    tiles.append([[4 if y < 2 else 3 for x in range(8)] for y in range(8)])
    tiles.append([[5 if 1 <= x <= 6 else 6 for x in range(8)] for y in range(8)])
    w = 8 * len(tiles)
    px = [[0] * w for _ in range(8)]
    for i, tile in enumerate(tiles):
        for y in range(8):
            for x in range(8):
                px[y][i * 8 + x] = tile[y][x]
    save("gb_tiles", w, 8, pal, px, {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})
    # Matching BG palette (16 colours).
    save("gb_tiles_palette", 16, 8, pal, [[i for i in range(16)] for _ in range(8)], {"type": "bg_palette", "bpp_mode": "bpp_4"})
    print("greybox graphics written")


if __name__ == "__main__":
    main()
