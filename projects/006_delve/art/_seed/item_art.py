"""PROVISIONAL item art for DELVE block 1 (G-35): floor items and the red hit edge, one shared 16-colour palette.

  python -I item_art.py      writes _seed/out/item_floor.png (32x128: bread, potion, short sword, iron key),
                             fx_edge_h.png (32x8) and fx_edge_v.png (8x32)

Wet-sewer style (art/LEEME.md): painted shading, soft dither, no cartoon outline; items rest on the bottom rows with a
dark floor shadow. seed_all.py turns them into art/<name>.aseprite only if missing.
"""
import os

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")

PAL = [
    (0, 0, 0),                                          # 0 transparent
    (8, 12, 10), (16, 22, 18), (26, 32, 28),            # 1-3 floor shadow (green-black)
    (64, 40, 20), (112, 74, 38), (164, 116, 66),        # 4-6 bread crust
    (206, 166, 108),                                    # 7 crumb / highlight
    (84, 10, 12), (150, 22, 22), (214, 58, 46),         # 8-10 red (potion, hit edge)
    (52, 62, 64), (110, 122, 124), (186, 196, 198),     # 11-13 iron / steel / glass
    (132, 104, 54),                                     # 14 cork / brass
    (232, 228, 212),                                    # 15 glint
]

BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def ramp(v, idx, x, y):
    """v in 0..1 -> one of the ramp indices, ordered-dithered (strength 0.6)."""
    n = len(idx)
    f = v * (n - 1) + (BAYER[y & 3, x & 3] - 0.5) * 0.6
    return idx[int(min(n - 1, max(0, round(f))))]


def shadow(img, cx, cy, rx, ry):
    for y in range(32):
        for x in range(32):
            d = ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2
            if d < 1 and img[y, x] == 0:
                img[y, x] = 1 if d < 0.45 else (2 if d < 0.8 else 3)


def bread():
    img = np.zeros((32, 32), np.uint8)
    shadow(img, 16, 28, 13, 3)
    for y in range(32):
        for x in range(32):
            dx, dy = (x - 16) / 11.0, (y - 22) / 6.0
            if dx * dx + dy * dy < 1 and y <= 27:
                light = 1 - (dx * 0.55 + dy * 0.8 + 0.4) / 1.6      # lit from the top left
                c = ramp(light, (4, 5, 6, 7), x, y)
                # three scores across the loaf
                if y < 22 and any(abs((x - sx) + (y - 18) * 0.6) < 0.7 for sx in (11, 16, 21)):
                    c = 5
                img[y, x] = c
    return img


def potion():
    img = np.zeros((32, 32), np.uint8)
    shadow(img, 16, 29, 9, 2.5)
    for y in range(32):
        for x in range(32):
            dx, dy = (x - 16) / 7.0, (y - 22) / 7.0
            r2 = dx * dx + dy * dy
            if r2 < 1:
                if y < 19:
                    c = ramp(0.6 - dx * 0.3, (11, 12), x, y)          # air in the glass
                else:
                    light = 1 - (dx * 0.6 + dy * 0.5 + 0.5) / 1.6
                    c = ramp(light, (8, 9, 10), x, y)
                if r2 > 0.8:
                    c = 11 if dx > 0 else 12                            # glass rim
                img[y, x] = c
    for y in range(10, 16):                                             # neck
        for x in range(14, 19):
            img[y, x] = 12 if x < 16 else 11
    for y in range(7, 10):                                              # cork
        for x in range(14, 19):
            img[y, x] = 14 if x < 17 else 5
    img[20, 12] = img[21, 12] = 15                                      # glint
    img[19, 13] = 13
    return img


def sword():
    img = np.zeros((32, 32), np.uint8)
    shadow(img, 16, 27, 14, 2.5)
    for x in range(10, 30):                                             # blade, point to the right
        half = 1 if x < 27 else 0
        for y in range(24 - half, 25 + half + 1):
            img[y, x] = 13 if y < 24 else (12 if y == 24 else 11)
    img[24, 29] = 12
    for x in range(12, 26, 3):
        img[24, x] = 15                                                 # glints along the edge
    for y in range(20, 30):                                             # cross guard
        img[y, 9] = 14 if y < 25 else 5
        img[y, 8] = 5
    for x in range(3, 8):                                               # grip (leather)
        for y in (23, 24, 25):
            img[y, x] = 4 if y == 25 else (5 if (x + y) & 1 else 6)
    for y in (22, 23, 24, 25, 26):                                      # pommel
        img[y, 2] = 14 if y < 24 else 5
    return img


def key():
    img = np.zeros((32, 32), np.uint8)
    shadow(img, 16, 28, 12, 2.5)
    for y in range(32):
        for x in range(32):
            d = ((x - 9) ** 2 + (y - 24) ** 2) ** 0.5
            if 2.2 < d < 4.6:
                img[y, x] = 13 if y < 23 and x < 10 else (12 if y < 25 else 11)
    for x in range(13, 27):
        img[23, x] = 12
        img[24, x] = 11
    for x, h in ((22, 3), (25, 3)):                                     # bit teeth
        for y in range(25, 25 + h):
            img[y, x] = img[y, x + 1] = 11
    img[23, 16] = 15
    return img


def edge(w, h):
    img = np.zeros((h, w), np.uint8)
    for y in range(h):
        for x in range(w):
            depth = y if w > h else x                                   # 0 = outer edge of the view
            v = 1 - depth / 8.0
            if v - (BAYER[y & 3, x & 3] - 0.5) * 0.5 > 0.25:
                img[y, x] = 10 if v > 0.8 else (9 if v > 0.5 else 8)
    return img


def save(path, idx):
    im = Image.fromarray(idx, "P")
    flat = [c for rgb in PAL for c in rgb]
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.save(path, transparency=0)


def main():
    os.makedirs(OUT, exist_ok=True)
    save(os.path.join(OUT, "item_floor.png"), np.vstack([bread(), potion(), sword(), key()]))
    save(os.path.join(OUT, "fx_edge_h.png"), edge(32, 8))
    save(os.path.join(OUT, "fx_edge_v.png"), edge(8, 32))
    print("item art seeds written")


if __name__ == "__main__":
    main()
