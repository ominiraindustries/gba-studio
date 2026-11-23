"""HUD art seeds for DELVE, layout A of HUD.md (G-32).

ui_frame (240x160, 8 bpp, UI indices 192..239 only + 0 for the transparent 3D view hole). Boxes match
art/hud_mockups/hud_a_panel.png; src/dl_hud.cpp draws the dynamic parts on top (bars, key, glow) and the text
sprites. Rects (x0, y0, x1, y1 inclusive):
  view hole        (0, 0, 175, 119)
  compass + floor  (178, 2, 237, 27)    tape letters at y 6, FLOOR at y 17, tape dots at y 9
  minimap well     (178, 30, 237, 91)   the engine draws 60x60 at (178, 31)
  keys / status    (178, 94, 237, 117)  key icon 16x16 at (181, 97) (hidden by the HUD when keys == 0)
  weapon           (2, 122, 39, 157)    sword icon 32x28 at (4, 124); reload bar (4, 152, 37, 156)
  HP + quick item  (42, 122, 237, 145)  HP bar (60, 127, 155, 134); potion well (198, 124, 235, 143)
  message line     (42, 147, 237, 158)  28 characters of common_variable_8x8 at (46, 149)

ui_arrow (8x16, 16 colours, sprite): damage-direction arrow pointing left (the HUD flips it for the right edge).
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from delve_art import UI, Canvas, full_palette, save_png

US, UIr, UG, UP = UI["s"], UI["i"], UI["g"], UI["p"]
UR, UO, UB, UL, UC = UI["r"], UI["o"], UI["b"], UI["l"], UI["c"]
OUTLINE = UIr[0]


def bricks(cv, x0, y0, x1, y1):
    """24x8 running-bond bricks of dark stone with a lit top edge and a few chips (mockup style)."""
    cv.rect(x0, y0, x1, y1, US[0], wrap=False)
    for row, y in enumerate(range(y0, y1 + 1, 8)):
        off = 0 if row % 2 == 0 else 12
        for k, x in enumerate(range(x0 - 24 + off, x1 + 1, 24)):
            bx0, bx1 = max(x0, x + 1), min(x1, x + 22)
            by0, by1 = y + 1, min(y1, y + 6)
            if bx0 > bx1 or by0 > by1:
                continue
            h = (x * 7 + y * 13 + row * 5) % 5
            base = (US[3], US[4], US[3], US[4], US[2])[h]
            cv.rect(bx0, by0, bx1, by1, base, wrap=False)
            cv.rect(bx0, by0, bx1, by0, US[4] if base != US[4] else US[5], wrap=False)    # lit top
            cv.rect(bx0, by1, bx1, by1, US[1], wrap=False)                                 # shadow bottom
            if bx0 == x + 1:
                cv.rect(bx0, by0, bx0, by1 - 1, US[4] if base != US[4] else US[5], wrap=False)
            if h == 1 and bx1 - bx0 > 10:                                                  # chip / crack
                cx = bx0 + 6 + (x // 3) % 8
                cv.set(cx, by0 + 2, US[1], wrap=False)
                cv.set(cx + 1, by0 + 3, US[1], wrap=False)
                cv.set(cx + 1, by0 + 2, US[0], wrap=False)


def rivet(cv, x, y):
    cv.rect(x, y, x + 1, y + 1, UG[2], wrap=False)
    cv.set(x, y, UG[3], wrap=False)
    cv.set(x + 1, y + 1, UG[1], wrap=False)


def box(cv, x0, y0, x1, y1, fill=UP[1], rivets=True):
    """Steel-rimmed plate (mockup box()): steel outline, dark inner rim, plate fill, brass rivets."""
    cv.rect(x0, y0, x1, y1, fill, wrap=False)
    cv.rect(x0, y0, x1, y0, UIr[4], wrap=False)
    cv.rect(x0, y0, x0, y1, UIr[4], wrap=False)
    cv.rect(x0, y1, x1, y1, UIr[3], wrap=False)
    cv.rect(x1, y0, x1, y1, UIr[3], wrap=False)
    cv.set(x0, y0, UIr[5], wrap=False)
    cv.rect(x0 + 1, y0 + 1, x1 - 1, y0 + 1, UIr[1], wrap=False)
    cv.rect(x0 + 1, y0 + 1, x0 + 1, y1 - 1, UIr[1], wrap=False)
    cv.rect(x0 + 1, y1 - 1, x1 - 1, y1 - 1, UIr[1], wrap=False)
    cv.rect(x1 - 1, y0 + 1, x1 - 1, y1 - 1, UIr[1], wrap=False)
    if rivets and x1 - x0 > 12 and y1 - y0 > 12:
        for rx, ry in ((x0 + 2, y0 + 2), (x1 - 3, y0 + 2), (x0 + 2, y1 - 3), (x1 - 3, y1 - 3)):
            rivet(cv, rx, ry)


def trough(cv, x0, y0, x1, y1, empty):
    cv.rect(x0, y0, x1, y1, OUTLINE, wrap=False)
    cv.rect(x0 + 1, y0 + 1, x1 - 1, y1 - 1, empty, wrap=False)


def outline_icon(cv, x0, y0, w, h, col=OUTLINE):
    """1-px dark outline (4-neighbour) around the non-zero pixels of the icon box."""
    src = [[cv.get(x0 + i, y0 + j, False) for i in range(w)] for j in range(h)]
    for j in range(h):
        for i in range(w):
            if src[j][i] != BG_MARK:
                continue
            for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                ni, nj = i + di, j + dj
                if 0 <= ni < w and 0 <= nj < h and src[nj][ni] != BG_MARK:
                    cv.set(x0 + i, y0 + j, col, wrap=False)
                    break


BG_MARK = -1


def stamp(cv, x0, y0, rows, cmap):
    for j, row in enumerate(rows.strip("\n").split("\n")):
        for i, ch in enumerate(row):
            if ch != ".":
                cv.set(x0 + i, y0 + j, cmap[ch], wrap=False)


def sword(cv, x0, y0):
    """32x28 longsword, hilt bottom-left, tip top-right; light from the top-left (HUD.md: arma 32x32)."""
    w, h = 32, 28
    ic = Canvas(w, h, BG_MARK)
    for x in range(10, 30):                     # blade: 4 px tall diagonal band, 45 degrees
        yc = 28 - x
        cols = [(yc - 2, UB[1]), (yc - 1, UB[0]), (yc, UIr[5]), (yc + 1, UIr[4])]
        if x >= 27:                              # taper to the tip
            cols = cols[x - 26:]
        for y, c in cols:
            if 0 <= y < h:
                ic.set(x, y, c, wrap=False)
    for x in range(13, 25):                     # fuller (groove) down the middle
        ic.set(x, 27 - x, UIr[4], wrap=False)
    for t in range(-4, 5):                      # cross-guard, perpendicular, 2 px thick, rounded ends
        if abs(t) == 4:
            ic.set(9 + t, 17 + t, UG[1], wrap=False)
            continue
        for dx, c in ((0, UG[3] if t < 0 else UG[2]), (1, UG[2] if t < 0 else UG[1]), (2, UG[1])):
            x, y = 8 + t + dx, 17 + t
            if 0 <= x < w and 0 <= y < h:
                ic.set(x, y, c, wrap=False)
    for k, x in enumerate(range(4, 9)):         # leather grip with wrap stripes
        yc = 27 - x
        ic.set(x, yc - 1, UL[2] if k % 2 else UL[1], wrap=False)
        ic.set(x, yc, UL[1] if k % 2 else UL[0], wrap=False)
        ic.set(x + 1, yc, UL[1], wrap=False)
    pommel = (".YY.", "Yyyo", "yyoo", ".oo.")
    for j, row in enumerate(pommel):             # round brass pommel
        for i, ch in enumerate(row):
            if ch != ".":
                ic.set(1 + i, 22 + j, {"Y": UG[3], "y": UG[2], "o": UG[1]}[ch], wrap=False)
    for j in range(h):
        for i in range(w):
            cv.set(x0 + i, y0 + j, ic.p[j][i], wrap=False)
    outline_icon(cv, x0, y0, w, h)
    for j in range(h):
        for i in range(w):
            if cv.get(x0 + i, y0 + j, False) == BG_MARK:
                cv.set(x0 + i, y0 + j, UP[1], wrap=False)


POTION = """
......kkkk......
.....kLlLLk.....
.....kllLlk.....
......kkkk......
......kCck......
......kCck......
....kkkCckkk....
...kCRRRrrrrk...
..kCRWRrrrrrrk..
..kRWRrrrrrrmk..
..kRRrrrrrrrmk..
..krrrrrrrrmmk..
..krrrrrrrmmmk..
...kmrrrrmmmk...
....kkmmmmkk....
................
"""
KEY = """
................
....kkkk........
...kyYYyk.......
..kyokkoyk......
..kyk..kyk......
..kyk..kyk......
..kyokkoyk......
...kyyyok.......
....kyok........
....kyok........
....kyokkk......
....kyyyyk......
....kyokk.......
....kyyyok......
....kyoook......
.....kkkk.......
"""
ICON_MAP = {"k": OUTLINE, "L": UL[2], "l": UL[1], "C": UC[1], "c": UC[0], "R": UR[3], "r": UR[2], "m": UR[1],
            "W": UR[4], "y": UG[2], "Y": UG[3], "o": UG[1]}


def build():
    cv = Canvas(240, 160, 0)
    bricks(cv, 176, 0, 239, 119)
    bricks(cv, 0, 120, 239, 159)
    cv.rect(176, 0, 176, 120, OUTLINE, wrap=False)                # shadow line along the view edges
    cv.rect(0, 120, 176, 120, OUTLINE, wrap=False)

    # right panel
    box(cv, 178, 2, 237, 27)
    for k in (-14, -7, 7, 14):                                    # compass tape dots
        cv.set(208 + k, 9, UIr[4], wrap=False)
        cv.set(208 + k, 10, UIr[1], wrap=False)
    box(cv, 178, 30, 237, 91, UP[0], rivets=False)
    box(cv, 178, 94, 237, 117)
    stamp(cv, 181, 97, KEY, ICON_MAP)

    # bottom strip
    box(cv, 2, 122, 39, 157)
    sword(cv, 4, 124)
    trough(cv, 4, 152, 37, 156, UO[0])
    box(cv, 42, 122, 237, 145)
    trough(cv, 60, 127, 155, 134, UR[0])
    box(cv, 198, 124, 235, 143, UP[0], rivets=False)
    stamp(cv, 200, 126, POTION, ICON_MAP)
    box(cv, 42, 147, 237, 158, UP[0], rivets=False)

    cv.rect(0, 0, 175, 119, 0, wrap=False)                        # keep the hole exact
    save_png("ui_frame", cv.p, full_palette(), transparent=True)


ARROW = """
.......k
......kk
.....kRk
....kRRk
...kRWRk
..kRWRRk
.kRWRRRk
kRWRRRrk
krrrrrrk
.kmrrrrk
..kmrrrk
...kmrrk
....kmrk
.....kmk
......kk
.......k
"""


def build_arrow():
    pal = [(0, 0, 0)] * 16
    pal[0] = (255, 0, 255)
    pal[1], pal[2], pal[3], pal[4], pal[5] = (20, 4, 4), (110, 16, 18), (200, 32, 30), (250, 96, 72), (255, 196, 170)
    cmap = {"k": 1, "m": 2, "r": 3, "R": 4, "W": 5}
    rows = [[cmap.get(ch, 0) for ch in r] for r in ARROW.strip("\n").split("\n")]
    save_png("ui_arrow", rows, pal, transparent=True)


if __name__ == "__main__":
    build()
    build_arrow()
