"""Seed greybox graphics (gb_*) for 002_pop_panic: player box, balls, harpoon and a plain stage background.

Run `python -I projects/002_pop_panic/tools/gen_greybox.py`. Only writes graphics/gb_*.bmp/.json; never touches art/.
"""
import json
import struct
from pathlib import Path

GFX = Path(__file__).resolve().parent.parent / "graphics"


def write_bmp(path, width, height, palette, pixels):
    """Uncompressed 4bpp indexed BMP, bottom-up rows."""
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


MAGENTA = (255, 0, 255)

# Ball palette: 1-4 base light->dark, 5 outline, 6 highlight (same layout the artist uses).
BALL_PAL = [MAGENTA, (255, 140, 140), (240, 70, 70), (200, 40, 50), (150, 20, 40), (40, 10, 20), (255, 255, 255)]


def ball(diameter, canvas):
    r = diameter / 2
    c = canvas / 2
    px = [[0] * canvas for _ in range(canvas)]
    for y in range(canvas):
        for x in range(canvas):
            dx, dy = x + 0.5 - c, y + 0.5 - c
            d = (dx * dx + dy * dy) ** 0.5
            if d <= r:
                if d > r - 1.2:
                    px[y][x] = 5
                else:
                    shade = (dx + dy) / (2 * r)  # -1 top-left .. 1 bottom-right
                    px[y][x] = 1 if shade < -0.45 else 2 if shade < 0 else 3 if shade < 0.4 else 4
        # highlight
    hx, hy = int(c - r * 0.45), int(c - r * 0.45)
    s = max(1, diameter // 10)
    for y in range(hy, hy + s):
        for x in range(hx, hx + s):
            px[y][x] = 6
    return px


def main():
    for d, canvas in ((48, 64), (32, 32), (16, 16), (8, 8)):
        save("gb_ball_%d" % d, canvas, canvas, BALL_PAL, ball(d, canvas), {"type": "sprite", "height": canvas})

    # Player: 32x32 frame, 16x24 body standing on the bottom row. 1 body, 2 outline, 3 face.
    pal = [MAGENTA, (60, 120, 240), (20, 20, 40), (250, 210, 170)]
    px = [[0] * 32 for _ in range(32)]
    for y in range(8, 32):
        for x in range(8, 24):
            edge = x in (8, 23) or y in (8, 31)
            px[y][x] = 2 if edge else (3 if 10 <= y <= 15 else 1)
    save("gb_player", 32, 32, pal, px, {"type": "sprite", "height": 32})

    # Harpoon: frame 0 tip, frame 1 rope segment (8x16 each).
    pal = [MAGENTA, (230, 230, 240), (120, 120, 140), (200, 160, 60)]
    px = [[0] * 8 for _ in range(32)]
    for y in range(16):
        w = min(3, y // 2)
        for x in range(4 - w - 1, 4 + w + 1):
            px[y][x] = 1 if y < 10 else 2
    for y in range(16, 32):
        px[y][3] = 3
        px[y][4] = 3 if (y // 2) % 2 else 2
    save("gb_harpoon", 8, 32, pal, px, {"type": "sprite", "height": 16})

    # Background 256x256: sky gradient bands, floor in rows 144-159.
    pal = [(0, 0, 0), (110, 170, 230), (130, 185, 235), (150, 200, 240), (170, 215, 245), (180, 90, 60), (140, 60, 40),
           (220, 130, 90)]
    px = [[0] * 256 for _ in range(256)]
    for y in range(256):
        for x in range(256):
            if y < 144:
                px[y][x] = 1 + min(3, y // 36)
            elif y < 160:
                by, bx = y - 144, (x + (4 if (y - 144) >= 8 else 0)) % 16
                px[y][x] = 7 if by in (0, 8) else (6 if bx == 0 else 5)
            else:
                px[y][x] = 6
    save("gb_bg", 256, 256, pal, px, {"type": "regular_bg", "bpp_mode": "bpp_4"})
    print("greybox graphics written")


if __name__ == "__main__":
    main()
