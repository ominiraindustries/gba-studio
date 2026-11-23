"""ui_frame seed (240x160, 8 bpp, only indices 192..255 + 0 for the transparent 3D view hole).

Layout (see ESPEC_VISTA.md):
  view hole         (0,0,176,120)  index 0
  panel             x 176..239: 2 px iron trim (176..177), status plate (178..237, 2..55),
                    minimap well 60x60 (178..237, 58..117), 2 px iron edge (238..239)
  bottom strip      y 120..159: iron trim (120..121), weapon well (2..57,124..141),
                    reload bar slot 52x8 (60..111,128..135) for a 48x4 bar at (62,130),
                    message plate 232x14 (4..235,143..156) for 28 chars of the 8x8 font.
"""
import os
import random
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from delve_art import UI, Canvas, full_palette, save_png

US, UIr, UG, UP = UI["s"], UI["i"], UI["g"], UI["p"]


def stone_bg(cv, rng):
    row_h = 8
    for r in range(cv.h // row_h):
        y0 = r * row_h
        x = -((r * 11) % 16)
        while x < cv.w:
            bw = rng.choice((14, 16, 18))
            base = rng.choice((US[3], US[4], US[3]))
            for yy in range(row_h):
                for xx in range(bw):
                    px, py = x + xx, y0 + yy
                    if not (0 <= px < cv.w):
                        continue
                    if yy == row_h - 1 or xx == bw - 1:
                        c = US[0]
                    elif yy == 0:
                        c = US[5] if base == US[4] else US[4]
                    elif yy == row_h - 2 or xx == bw - 2:
                        c = US[1]
                    elif xx == 0:
                        c = US[5] if base == US[4] else US[4]
                    else:
                        c = base
                    cv.p[py][px] = c
            x += bw


def plate(cv, x0, y0, x1, y1, fill=UP[1]):
    """Recessed plate: dark fill, shadowed top-left rim, lit bottom-right rim (iron)."""
    cv.rect(x0, y0, x1, y1, fill, wrap=False)
    cv.rect(x0 - 1, y0 - 1, x1 + 1, y0 - 1, UIr[1], wrap=False)
    cv.rect(x0 - 1, y0 - 1, x0 - 1, y1 + 1, UIr[1], wrap=False)
    cv.rect(x0 - 1, y1 + 1, x1 + 1, y1 + 1, UIr[4], wrap=False)
    cv.rect(x1 + 1, y0 - 1, x1 + 1, y1 + 1, UIr[4], wrap=False)


def rivet(cv, x, y):
    cv.rect(x, y, x + 1, y + 1, UG[2], wrap=False)
    cv.set(x, y, UG[3], wrap=False)
    cv.set(x + 1, y + 1, UG[1], wrap=False)


def iron_bar_v(cv, x0, x1, y0, y1):
    for x in range(x0, x1 + 1):
        c = UIr[4] if x == x0 else UIr[1] if x == x1 else UIr[3]
        cv.rect(x, y0, x, y1, c, wrap=False)


def iron_bar_h(cv, y0, y1, x0, x1):
    for y in range(y0, y1 + 1):
        c = UIr[4] if y == y0 else UIr[1] if y == y1 else UIr[3]
        cv.rect(x0, y, x1, y, c, wrap=False)


def build():
    rng = random.Random(21)
    cv = Canvas(240, 160, 0)
    stone_bg(cv, rng)
    cv.rect(0, 0, 175, 119, 0, wrap=False)                       # view hole
    # trims around the view
    iron_bar_v(cv, 176, 177, 0, 121)
    iron_bar_h(cv, 120, 121, 0, 177)
    iron_bar_v(cv, 238, 239, 0, 159)
    iron_bar_h(cv, 158, 159, 0, 239)
    iron_bar_h(cv, 0, 1, 178, 237)
    # panel: status plate + minimap well
    plate(cv, 179, 3, 236, 54)
    cv.rect(178, 57, 237, 57, UIr[1], wrap=False)
    cv.rect(178, 58, 237, 117, UP[0], wrap=False)               # minimap 60x60, no rim inside
    cv.rect(178, 118, 237, 118, UIr[4], wrap=False)
    for (x, y) in ((180, 4), (234, 4), (180, 52), (234, 52)):
        rivet(cv, x, y)
    # bottom strip: weapon well, reload slot, message plate
    plate(cv, 3, 125, 56, 140)
    plate(cv, 60, 128, 111, 135, UP[0])
    plate(cv, 5, 144, 234, 155)
    for (x, y) in ((116, 128), (232, 128), (174, 128)):
        rivet(cv, x, y)
    # brass corner bosses where the trims meet
    for (x, y) in ((175, 118), (236, 118)):
        cv.rect(x, y, x + 3, y + 3, UG[1], wrap=False)
        cv.rect(x, y, x + 2, y + 2, UG[2], wrap=False)
        cv.set(x, y, UG[3], wrap=False)
    cv.rect(0, 0, 175, 119, 0, wrap=False)                       # keep the hole exact
    save_png("ui_frame", cv.p, full_palette(), transparent=True)


if __name__ == "__main__":
    build()
