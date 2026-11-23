"""Seed greybox graphics for 001_apila: board tiles (4bpp) and 8 BG palettes.

Run once with `python -I projects/001_apila/tools/gen_greybox.py`. It writes graphics/cells.bmp,
graphics/cells_palette.bmp and their .json files. It refuses to overwrite art/*.aseprite sources.
"""
import json
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GFX = ROOT / "graphics"


def write_bmp(path, width, height, bpp, palette, pixels):
    """Uncompressed indexed BMP, bottom-up rows. pixels[y][x] are palette indices."""
    colors = 1 << bpp
    pal = list(palette) + [(0, 0, 0)] * (colors - len(palette))
    row_bytes = ((width * bpp + 31) // 32) * 4
    data = bytearray()
    for y in range(height - 1, -1, -1):
        row = bytearray(row_bytes)
        for x in range(width):
            v = pixels[y][x]
            if bpp == 8:
                row[x] = v
            else:
                shift = 4 if x % 2 == 0 else 0
                row[x // 2] |= (v & 15) << shift
        data += row
    pal_bytes = b"".join(struct.pack("<BBBB", b, g, r, 0) for (r, g, b) in pal)
    offset = 14 + 40 + len(pal_bytes)
    header = struct.pack("<2sIHHI", b"BM", offset + len(data), 0, 0, offset)
    info = struct.pack("<IiiHHIIiiII", 40, width, height, 1, bpp, 0, len(data), 2835, 2835, colors, 0)
    path.write_bytes(header + info + pal_bytes + data)


def mix(c, t, k):
    return tuple(round(a + (b - a) * k) for a, b in zip(c, t))


# Palette layout (same in every 16-color palette):
# 0 backdrop, 1 block highlight, 2 block body, 3 block shadow, 5 well, 6 grid dot, 7 wall, 8 wall light, 9 flash
BACKDROP = (20, 22, 40)
WELL = (10, 12, 24)
GRID = (40, 44, 70)
WALL = (70, 80, 130)
WALL_LIGHT = (120, 130, 190)
WHITE = (250, 250, 250)

# Palette 0: grey (game over); 1..7: pieces I, O, T, S, Z, J, L (own colour scheme).
BODIES = [
    (150, 150, 165),
    (240, 90, 170),
    (110, 230, 180),
    (240, 200, 70),
    (90, 170, 250),
    (170, 130, 240),
    (250, 130, 100),
    (170, 220, 60),
]


def palette_for(body):
    p = [(0, 0, 0)] * 16
    p[0] = BACKDROP
    p[1] = mix(body, (255, 255, 255), 0.45)
    p[2] = body
    p[3] = mix(body, (0, 0, 0), 0.45)
    p[5] = WELL
    p[6] = GRID
    p[7] = WALL
    p[8] = WALL_LIGHT
    p[9] = WHITE
    return p


def tile_empty():
    return [[0] * 8 for _ in range(8)]


def tile_well():
    t = [[5] * 8 for _ in range(8)]
    t[0][0] = 6
    return t


def tile_block():
    t = [[2] * 8 for _ in range(8)]
    for i in range(8):
        t[0][i] = 1
        t[i][0] = 1
        t[7][i] = 3
        t[i][7] = 3
    t[7][0] = 3
    t[0][7] = 1
    return t


def tile_ghost():
    t = [[5] * 8 for _ in range(8)]
    for i in range(8):
        t[0][i] = 3
        t[7][i] = 3
        t[i][0] = 3
        t[i][7] = 3
    return t


def tile_wall():
    t = [[7] * 8 for _ in range(8)]
    for i in range(8):
        t[i][0] = 8
    return t


def tile_flash():
    return [[9] * 8 for _ in range(8)]


TILES = [tile_empty(), tile_well(), tile_block(), tile_ghost(), tile_wall(), tile_flash()]


def main():
    for src in (ROOT / "art").glob("*.aseprite"):
        print("note: art source exists, leaving it alone:", src.name)
    w = 8 * len(TILES)
    pixels = [[0] * w for _ in range(8)]
    for i, t in enumerate(TILES):
        for y in range(8):
            for x in range(8):
                pixels[y][i * 8 + x] = t[y][x]
    write_bmp(GFX / "cells.bmp", w, 8, 4, palette_for(BODIES[0]), pixels)
    (GFX / "cells.json").write_text(json.dumps({"type": "regular_bg_tiles", "bpp_mode": "bpp_4"}, indent=4) + "\n")

    pal = []
    for body in BODIES:
        pal += palette_for(body)
    ppix = [[(y * 16 + x) for x in range(16)] for y in range(len(BODIES))]
    write_bmp(GFX / "cells_palette.bmp", 16, len(BODIES), 8, pal, ppix)
    (GFX / "cells_palette.json").write_text(
        json.dumps({"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16 * len(BODIES)}, indent=4) + "\n")
    print("wrote cells.bmp (%d tiles) and cells_palette.bmp (%d palettes)" % (len(TILES), len(BODIES)))


if __name__ == "__main__":
    main()
