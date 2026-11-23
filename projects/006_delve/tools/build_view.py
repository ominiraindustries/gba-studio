#!/usr/bin/env python
"""DELVE view pieces builder (task G-25).

Projects flat 64x64 textures onto every piece of the first-person view defined in include/dl_view_geometry.h
(front faces, lateral faces, floor/ceiling), applies distance attenuation, quantizes everything to one 256-colour
palette (0 black, 1-191 environment, 192-255 reserved for the UI) and writes the data the game compiles:

    include/dl_view_data.h   piece tables declarations (generated)
    src/dl_view_data.cpp     spans + pixels + palette (generated)

Textures: art/export/tex_*.bmp exported by the artist from art/*.aseprite (read only), otherwise the test textures
in tools/test_textures/ (generated here when art/export does not exist). UI colours 192-255 come from the palette of
graphics/ui_frame.bmp. Indexed images use index 0 as transparent; RGB(A) images use
alpha 0 or magenta (255, 0, 255) as transparent. This tool never touches art/.

Run from the repo root:   python -I projects/006_delve/tools/build_view.py
"""
import glob
import os
import re
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(HERE)
GEO_H = os.path.join(PROJ, "include", "dl_view_geometry.h")
OUT_H = os.path.join(PROJ, "include", "dl_view_data.h")
OUT_CPP = os.path.join(PROJ, "src", "dl_view_data.cpp")
GFX = os.path.join(PROJ, "graphics")
TEST_TEX = os.path.join(HERE, "test_textures")
TS = 64
SCREEN_W = 240

# piece sets: (enum name, texture file stem, frame index in a vertical strip, opening kind or None, opaque)
# Wall decorations (torch, banner, chains, lever) are sprites placed by the game (dl_deco_items.h), not textures.
SETS = [
    ("wall", "tex_wall_stone", 0, None, True),
    ("wall_b", "tex_wall_stone#1", 0, None, True),      # variants picked per edge (hash): see WALL_VARIANTS
    ("wall_c", "tex_wall_stone#2", 0, None, True),
    ("wall_d", "tex_wall_stone#3", 0, None, True),
    ("wall_skulls", "tex_wall_skulls", 0, None, True),
    ("wall_crack", "tex_wall_crack", 0, None, True),
    ("wall_dart", "tex_wall_dart", 0, None, True),
    ("door_closed", "tex_door", 0, "door", True),
    ("door_locked", "tex_door_locked", 0, "door", True),
    ("door_open", "tex_door_open", 0, "door", False),
    ("arch", "tex_arch", 0, "arch", False),
    ("bars", "tex_bars", 0, "bars", False),
    ("stairs", "tex_stairs_down", 0, "stairs", True),
]
# indexed textures where index 0 is the see-through hole (art/LÉEME.md); in the rest index 0 is real black
SEE_THROUGH = {"tex_bars", "tex_arch", "tex_door_open"}
# Wall thickness (dl_view_geometry.h wall_thick_64): openings get jambs, a soffit and a recessed leaf / gate.
# kind: (stem whose index 0 marks the opening, mode, recess of the content as a fraction of the thickness)
OPENINGS = {
    "door": ("tex_door_open", "door", 0.5),
    "arch": ("tex_arch", "columns", None),
    "bars": ("tex_bars", "rect", 0.5),
    "stairs": ("tex_stairs_down", "columns", 1.0),
}
# Wall variants "stem#k": the artist's tex_wall_stone_v<k>.bmp when it exists, else procedural from the base
# (PROVISIONAL until drawn by hand): 1 = mirrored, 2 = courses shifted 21 texels, 3 = shifted 37 and mirrored.
WALL_VARIANTS = {1: (0, True), 2: (21, False), 3: (37, True)}
JAMB_SHADE, SOFFIT_SHADE, SILL_SHADE, CONTENT_SHADE = 0.62, 0.48, 0.75, 0.9
PILLAR_FRONT_SHADE = 0.72       # front face of a vertex block: a wall end reads as a block (user capture 2.png)
MIN_Z = 0.05                    # near clip (cells)
ART_EXPORT = os.path.join(PROJ, "art", "export")
UI_FRAME = os.path.join(GFX, "ui_frame.bmp")
# minimap colours (palette 240..247; ui_frame uses 192-209): floor, wall, player, door, bars, stairs, lever, dark
MAP_COLORS = [(64, 58, 50), (206, 194, 160), (250, 226, 90), (176, 112, 52), (184, 184, 196), (90, 200, 230),
              (240, 200, 60), (14, 12, 18)]
# ----------------------------------------------------------------------------------------------- geometry
def parse_geometry():
    with open(GEO_H, encoding="utf-8") as f:
        text = f.read()
    g = {m.group(1): int(m.group(2)) for m in re.finditer(r"constexpr int (\w+) = (-?\d+);", text)}
    for k in ("view_w", "view_h", "vp_x", "vp_y", "proj_num", "eye_back_z10", "fog_z10", "depths", "max_lateral",
              "wall_thick_64", "cell_pitch_64", "fade_x1000"):
        if k not in g:
            sys.exit("build_view: %s missing in %s" % (k, GEO_H))
    return g


class Geo:
    """View geometry (dl_view_geometry.h, G-31 pitch model): cells of width 1, wall bands of thickness t between
    them, pitch P = 1 + t. Base frame: X from the centre of the player's column, Z from the resting eye (cells).
    near(d) / far(d): tenths of a cell of the near / far face line of row d; column x spans X = x P -+ 0.5."""

    def __init__(self, g):
        self.__dict__.update(g)
        self.F = g["proj_num"] / 5.0                 # px per cell
        self.t = g["wall_thick_64"] / 64.0           # wall band thickness (cells)
        self.P = g["cell_pitch_64"] / 64.0           # cell pitch
        self.back = g["eye_back_z10"] / 10.0
        global FADE
        FADE = g["fade_x1000"] / 1000.0

    def near(self, d):
        return 10.0 * (d * self.P + self.back - 0.5)

    def far(self, d):
        return self.near(d) + 10


# ----------------------------------------------------------------------------------------------- textures
def save_rgba(path, arr):
    Image.fromarray(arr.astype(np.uint8), "RGBA").save(path)


def noise(rng, shape, amp):
    return rng.integers(-amp, amp + 1, shape)


def make_test_textures():
    os.makedirs(TEST_TEX, exist_ok=True)
    rng = np.random.default_rng(1234)
    yy, xx = np.mgrid[0:TS, 0:TS]

    def solid(rgb):
        a = np.zeros((TS, TS, 4), int)
        a[..., :3] = rgb
        a[..., 3] = 255
        return a

    def bricks(base, mortar, bh=16, bw=32):
        a = solid(base)
        a[..., :3] += noise(rng, (TS, TS, 1), 10)
        row = yy // bh
        shift = (row % 2) * (bw // 2)
        m = ((yy % bh) == 0) | (((xx + shift) % bw) == 0)
        a[m, :3] = mortar
        sh = ((yy % bh) == bh - 1)
        a[sh, :3] = (a[sh, :3] * 0.7).astype(int)
        return a

    def clear():
        return np.zeros((TS, TS, 4), int)

    out = {}
    wall = bricks((112, 104, 96), (50, 46, 44))
    out["tex_wall_stone"] = wall
    # door: closed (planks in a stone frame) + open (frame, leaf against the left jamb, see-through)
    closed = wall.copy()
    inner = (xx >= 10) & (xx < 54) & (yy >= 8)
    plank = solid((120, 78, 40))[..., :3] + noise(rng, (TS, TS, 1), 8)
    closed[inner, :3] = plank[inner]
    closed[inner & ((xx - 10) % 11 == 0), :3] = (70, 44, 22)
    closed[inner & ((yy == 20) | (yy == 50)), :3] = (60, 60, 66)
    closed[(xx >= 46) & (xx < 50) & (yy >= 34) & (yy < 38), :3] = (220, 200, 90)
    opened = wall.copy()
    opened[inner, 3] = 0
    leaf = (xx >= 10) & (xx < 16) & (yy >= 8)
    opened[leaf, :3] = (90, 58, 30)
    opened[leaf, 3] = 255
    out["tex_door"] = np.concatenate([closed, opened], axis=0)
    arch = wall.copy()
    hole = ((xx - 32) ** 2 + (yy - 26) ** 2 < 22 ** 2) & (yy < 26) | ((xx >= 10) & (xx < 54) & (yy >= 26))
    arch[hole, 3] = 0
    out["tex_arch"] = arch
    bars = clear()
    bm = ((xx % 10) < 2) | (yy % 21 < 2) | (yy < 3)
    bars[bm, :3] = (70, 72, 80)
    bars[bm & ((xx % 10) == 0), :3] = (120, 124, 136)
    bars[bm, 3] = 255
    out["tex_bars"] = bars
    stairs = wall.copy()
    op = (xx >= 8) & (xx < 56) & (yy >= 10)
    stairs[op, :3] = (8, 8, 10)
    for k in range(6):
        y0 = 30 + k * 6
        w = 22 - k * 3
        m = (yy >= y0) & (yy < y0 + 3) & (abs(xx - 32) < w)
        stairs[m, :3] = (int(120 - k * 16),) * 3
    out["tex_stairs_down"] = stairs
    floor = bricks((84, 76, 66), (36, 32, 30), 32, 32)
    out["tex_floor"] = floor
    ceil = bricks((54, 50, 52), (26, 24, 26), 32, 32)
    out["tex_ceiling"] = ceil
    # decor overlays (transparent except the feature)
    torch = clear()
    torch[(xx >= 30) & (xx < 34) & (yy >= 24) & (yy < 38), :] = (90, 70, 50, 255)
    torch[(xx >= 26) & (xx < 38) & (yy >= 36) & (yy < 39), :] = (60, 60, 66, 255)
    flame = ((xx - 32) ** 2 + ((yy - 18) * 0.7) ** 2) < 30
    torch[flame] = (250, 170, 40, 255)
    torch[((xx - 32) ** 2 + ((yy - 19) * 0.7) ** 2) < 8] = (255, 240, 140, 255)
    out["dec_torch"] = torch
    banner = clear()
    bn = (xx >= 22) & (xx < 42) & (yy >= 6) & (yy < 44 - abs(xx - 32) // 2)
    banner[bn] = (150, 30, 30, 255)
    banner[bn & (abs(xx - 32) < 3) & (yy > 14) & (yy < 30)] = (220, 190, 60, 255)
    banner[(xx >= 20) & (xx < 44) & (yy >= 4) & (yy < 6)] = (90, 70, 50, 255)
    out["dec_banner"] = banner
    chains = clear()
    for cx in (20, 44):
        chains[(abs(xx - cx) < 2) & (yy < 40) & ((yy // 3) % 2 == 0)] = (100, 100, 110, 255)
        chains[(abs(xx - cx) < 3) & (yy >= 40) & (yy < 44)] = (130, 130, 140, 255)
    out["dec_chains"] = chains
    skulls = clear()
    for cx in (20, 32, 44):
        m = ((xx - cx) ** 2 + (yy - 54) ** 2) < 25
        skulls[m] = (215, 210, 190, 255)
        skulls[m & ((yy == 54) & ((xx == cx - 2) | (xx == cx + 2)))] = (20, 20, 20, 255)
    out["dec_skulls"] = skulls
    crack = clear()
    cx = 30
    for y in range(4, 60):
        cx += int(rng.integers(-1, 2))
        crack[y, cx:cx + 2] = (20, 18, 18, 255)
    out["dec_crack"] = crack
    niche = clear()
    nm = (xx >= 20) & (xx < 44) & (yy >= 20) & (yy < 46)
    niche[nm] = (24, 22, 22, 255)
    niche[nm & (yy < 24)] = (10, 10, 10, 255)
    niche[(xx >= 18) & (xx < 46) & (yy >= 46) & (yy < 49)] = (140, 130, 120, 255)
    out["dec_niche"] = niche
    lever = np.zeros((2 * TS, TS, 4), int)
    for f, up in ((0, True), (1, False)):
        o = f * TS
        lever[o + 30:o + 40, 26:38] = (70, 70, 78, 255)
        if up:
            for k in range(14):
                lever[o + 32 - k, 31 + k // 3:33 + k // 3] = (150, 110, 60, 255)
            lever[o + 15:o + 19, 34:39] = (200, 40, 40, 255)
        else:
            for k in range(14):
                lever[o + 36 + k, 31 + k // 3:33 + k // 3] = (150, 110, 60, 255)
            lever[o + 49:o + 53, 34:39] = (60, 200, 60, 255)
    out["dec_lever"] = lever
    dart = clear()
    dart[((xx - 32) ** 2 + (yy - 32) ** 2) < 6] = (6, 6, 6, 255)
    dart[((xx - 32) ** 2 + (yy - 32) ** 2 >= 6) & ((xx - 32) ** 2 + (yy - 32) ** 2 < 12)] = (60, 56, 52, 255)
    out["dec_dart"] = dart
    lock = clear()
    lock[(xx >= 40) & (xx < 48) & (yy >= 30) & (yy < 40)] = (200, 170, 60, 255)
    lock[(xx >= 43) & (xx < 45) & (yy >= 33) & (yy < 37)] = (20, 20, 20, 255)
    out["dec_lock"] = lock
    for name, arr in out.items():
        p = os.path.join(TEST_TEX, name + ".png")
        if not os.path.exists(p):
            save_rgba(p, np.clip(arr, 0, 255))


THEME_EXPORT = None     # export dir of the theme being loaded (None = theme 0, art/export)


def load_texture(stem):
    """RGBA int array (h, 64, 4). The theme's export first, then the artist export of theme 0 (art/export,
    graphics/<stem>.bmp), then the test texture."""
    first = [os.path.join(THEME_EXPORT, stem + ".bmp")] if THEME_EXPORT else []
    for p in first + [os.path.join(ART_EXPORT, stem + ".bmp"), os.path.join(GFX, stem + ".bmp"),
                      os.path.join(TEST_TEX, stem + ".png")]:
        if os.path.exists(p):
            im = Image.open(p)
            if im.mode == "P":
                idx = np.array(im)
                rgba = np.array(im.convert("RGBA")).astype(int)
                if stem in SEE_THROUGH:
                    rgba[idx == 0, 3] = 0
            else:
                rgba = np.array(im.convert("RGBA")).astype(int)
                mag = (rgba[..., 0] == 255) & (rgba[..., 1] == 0) & (rgba[..., 2] == 255)
                rgba[mag, 3] = 0
            return rgba, p
    sys.exit("build_view: texture %s not found" % stem)


def frame(tex, k):
    return tex[k * TS:(k + 1) * TS] if tex.shape[0] >= (k + 1) * TS else tex[:TS]


# ----------------------------------------------------------------------------------------------- projection
FADE = 0.3


def shade(z):
    """Distance attenuation (z in cells), dl_view_geometry.h fade_x1000: fades toward black with the distance."""
    return np.clip(np.exp(-FADE * (z - 1.0)), 0.0, 1.0)


def sample(tex, u, v):
    tu = np.clip((u * TS).astype(int), 0, TS - 1)
    tv = np.clip((v * TS).astype(int), 0, TS - 1)
    return tex[tv, tu]


def load_index(stem):
    """Raw palette indices (h, 64) of an indexed texture, or None."""
    for p in (os.path.join(ART_EXPORT, stem + ".bmp"), os.path.join(GFX, stem + ".bmp")):
        if os.path.exists(p):
            im = Image.open(p)
            return np.array(im) if im.mode == "P" else None
    return None


def build_mask(kind, tex_stem_rgba):
    """Opening mask (64x64 bool, True = hole in the wall slab) of an opening kind."""
    stem, mode, _recess = OPENINGS[kind]
    idx = load_index(stem)
    hole = (idx[:TS] == 0) if idx is not None else (tex_stem_rgba[:TS, :, 3] == 0)
    m = np.zeros((TS, TS), bool)
    cols = np.nonzero(hole.any(0))[0]
    if len(cols) == 0:
        return m
    if mode == "rect":
        rows = np.nonzero(hole.any(1))[0]
        m[rows.min():rows.max() + 1, cols.min():cols.max() + 1] = True
        return m
    bottom = None
    if mode == "door":
        # the leaf of the closed door reaches lower than the dark hole of the open one: open down to the last row
        # where the closed door differs from the plain wall in the middle column (the threshold stays in front)
        closed, stone = load_index("tex_door"), load_index("tex_wall_stone")
        if closed is not None and stone is not None:
            diff = np.nonzero(closed[:TS, TS // 2] != stone[:TS, TS // 2])[0]
            bottom = int(diff.max()) if len(diff) else None
    for c in range(cols.min(), cols.max() + 1):
        r = np.nonzero(hole[:, c])[0]
        if len(r):
            m[r.min():(bottom if bottom is not None else r.max()) + 1, c] = True
    return m


def cap_texture(stone):
    """Wall slab side faces: the stone texture turned 90 degrees (u = depth into the slab)."""
    return np.rot90(stone[:TS]).copy()


def thick(geo):
    return geo.t


def make_locked_placeholder(door=None):
    """PROVISIONAL tex_door_locked until the artist draws it: tex_door with an iron band and a big iron padlock
    (iron = the Iron Key colour). Written to tools/test_textures/, never to art/. With `door` (a theme's tex_door,
    G-37) the image is returned instead of written."""
    theme_door = door is not None
    if theme_door:
        pass
    elif os.path.exists(os.path.join(ART_EXPORT, "tex_door_locked.bmp")) or             os.path.exists(os.path.join(GFX, "tex_door_locked.bmp")):
        return
    else:
        door, _p = load_texture("tex_door")
    a = door[:TS].copy()
    yy, xx = np.mgrid[0:TS, 0:TS]
    light, mid, dark = (176, 184, 200), (112, 118, 134), (34, 36, 44)
    band = (yy >= 30) & (yy < 35) & (xx >= 18) & (xx < 52)
    a[band, :3] = mid
    a[band & (yy == 30), :3] = light
    a[band & (yy == 34), :3] = dark
    for bx in (21, 48):
        a[(yy >= 31) & (yy < 33) & (xx >= bx) & (xx < bx + 2), :3] = light
    shackle = (((xx - 35) ** 2 + (yy - 37) ** 2) <= 36) & (((xx - 35) ** 2 + (yy - 37) ** 2) >= 12) & (yy < 38)
    a[shackle, :3] = dark
    a[shackle & (xx < 35), :3] = mid
    body = (xx >= 29) & (xx < 42) & (yy >= 37) & (yy < 48)
    a[body, :3] = mid
    a[body & ((xx == 29) | (yy == 47) | (xx == 41)), :3] = dark
    a[body & (yy == 37), :3] = light
    a[(xx >= 34) & (xx < 37) & (yy >= 40) & (yy < 45), :3] = dark
    a[..., 3] = 255
    if theme_door:
        return a
    os.makedirs(TEST_TEX, exist_ok=True)
    save_rgba(os.path.join(TEST_TEX, "tex_door_locked.png"), np.clip(a, 0, 255))


DIR_DX = (0, 1, 0, -1)      # N, E, S, W (dl_gen.h dx / dy)
DIR_DY = (-1, 0, 1, 0)


def project_floor_ceiling(geo, floor, ceil, parity, direction, pose=None):
    """Floor and ceiling of a camera pose (None = resting view). Tiles are fixed to the WORLD (u along world x, v
    along world y, G-31: a turn shows the same floor at both ends); the tile of a world cell is mirrored when
    (cell x + cell y) is odd: parity = (x + y) & 1 of the base cell, direction = its facing (rotates the tiles)."""
    pose = pose or Pose(geo, 0.0, 0.0, 0.0)
    global FADE
    W, H = geo.view_w, geo.view_h
    img = np.zeros((H, W, 4), float)
    img[..., 3] = 255
    fog = geo.fog_z10 / 10.0
    ry = pose.ry
    with np.errstate(divide="ignore", invalid="ignore"):
        t = np.where(ry < 0, -0.5 / ry, 0.5 / ry)
    ok = np.isfinite(t) & (t > 0) & (t <= fog)
    t = np.where(ok, t, 1.0)
    X = pose.ex + pose.rx * t
    Zr = pose.ez + pose.rz * t - geo.eye_back_z10 / 10.0      # from the base cell centre
    f = (DIR_DX[direction], DIR_DY[direction])
    r = (DIR_DX[(direction + 1) & 3], DIR_DY[(direction + 1) & 3])
    wx = (X * r[0] + Zr * f[0]) / geo.P + 0.5       # one tile per cell pitch (cell + band)
    wy = (X * r[1] + Zr * f[1]) / geo.P + 0.5
    cx, cy = np.floor(wx), np.floor(wy)
    u, v = wx - cx, wy - cy
    flip = (cx.astype(int) + cy.astype(int) + parity) & 1
    u = np.where(flip == 1, 1.0 - 1e-6 - u, u)
    below = ry < 0
    col = np.where(below[..., None], sample(floor, u, v), sample(ceil, u, v)).astype(float)
    col[..., :3] *= (shade(t) * np.where(below, 1.0, 0.85))[..., None]
    col[~ok, :3] = 0
    img[..., :3] = col[..., :3]
    return 0, 0, img


# ----------------------------------------------------------------------------------------------- G-31 motion poses
# Smooth grid movement: MOTION_FRAMES real intermediate camera poses per move kind, each a full set of pieces.
# Base frame = resting eye frame of the base cell (X right in cells, 0 = centre column; Z forward in cells from the
# resting eye; row d spans Z = d + 0.2 .. d + 1.2). step: eye (0, t); strafe: eye (t, 0); turn right: yaw = t * 90
# deg with the eye on the orbit centre - 0.7 * forward, so both ends are the resting views. Back / left moves play
# the same poses in reverse from the destination (dl_view.h begin_motion).
MOTION_FRAMES = 5
MOTION_KINDS = ("step", "strafe", "turn")
GRID_D = (-4, 6)            # runtime grid of base-relative cells (dl_view_motion_data.h)
GRID_X = (-5, 5)
MOTION_BIN = os.path.join(PROJ, "src", "dl_view_motion.bin")
MOTION_S = os.path.join(PROJ, "src", "dl_view_motion_blob.s")
MOTION_H = os.path.join(PROJ, "include", "dl_view_motion_data.h")
MOTION_CPP = os.path.join(PROJ, "src", "dl_view_motion_data.cpp")
U = 320                     # dl::geo u per cell


class Pose:
    """Camera at (ex, ez) of the base frame turned by yaw (radians, + = right); rays per unit camera depth."""

    def __init__(self, geo, ex, ez, yaw):
        self.ex, self.ez, self.yaw = ex, ez, yaw
        sy, sx = np.mgrid[0:geo.view_h, 0:geo.view_w]
        u = (sx + 0.5 - geo.vp_x) / geo.F
        self.ry = -(sy + 0.5 - geo.vp_y) / geo.F
        c, s = np.cos(yaw), np.sin(yaw)
        self.rx = u * c + s
        self.rz = c - u * s
        self.fwd = (s, c)


def motion_poses(geo):
    out = []
    back = geo.eye_back_z10 / 10.0
    for kind in MOTION_KINDS:
        for k in range(1, MOTION_FRAMES + 1):
            t = k / (MOTION_FRAMES + 1.0)
            if kind == "step":
                out.append((kind, Pose(geo, 0.0, t * geo.P, 0.0)))
            elif kind == "strafe":
                out.append((kind, Pose(geo, t * geo.P, 0.0, 0.0)))
            else:
                th = t * np.pi / 2
                out.append((kind, Pose(geo, -back * np.sin(th), back - back * np.cos(th), th)))
    return out


def pose_face(geo, pose, axis, plane, a0, a1, sel=None):
    """Ray hits of the plane Z = plane (axis "z", along = X) or X = plane (axis "x", along = Z), along in [a0, a1),
    unit wall height, near clip, fog cut. sel: flat pixel indices to test (default all). -> ok, t, along, v."""
    rx, ry, rz = pose.rx.ravel(), pose.ry.ravel(), pose.rz.ravel()
    if sel is not None:
        rx, ry, rz = rx[sel], ry[sel], rz[sel]
    with np.errstate(divide="ignore", invalid="ignore"):
        if axis == "z":
            t = (plane - pose.ez) / rz
            al = pose.ex + rx * t
        else:
            t = (plane - pose.ex) / rx
            al = pose.ez + rz * t
    ok = np.isfinite(t) & (t >= MIN_Z) & (t <= geo.fog_z10 / 10.0) & np.isfinite(al) & (al >= a0) & (al < a1)
    t = np.where(ok, t, 1.0)
    Y = ry * t
    ok &= (Y >= -0.5) & (Y < 0.5)
    return ok, t, np.where(ok, al, a0), 0.5 - Y


def facing(pose, axis, lo, hi):
    """Visible face of a band lo..hi along an axis: (plane, sgn into the band) or None when the eye is inside it."""
    e = pose.ez if axis == "z" else pose.ex
    if e < lo:
        return lo, 1
    if e > hi:
        return hi, -1
    return None


def face_u(axis, sgn, al, a0, a1):
    """Texture u along the viewer's right when facing the face (matches the resting front / lateral pieces)."""
    t = (al - a0) / (a1 - a0)
    if axis == "z":
        return t if sgn > 0 else 1.0 - t
    return 1.0 - t if sgn > 0 else t


def march_pose(geo, pose, sel, face, tex, mask, recess, cap):
    """march_fill for any camera pose: the hole pixels `sel` (flat indices) of the face (axis, plane, sgn, a0, a1)
    -> (filled mask, rgba) of the jambs / soffit / sill and the recessed content."""
    axis, plane, sgn, a0, a1 = face
    rx, ry, rz = pose.rx.ravel()[sel], pose.ry.ravel()[sel], pose.rz.ravel()[sel]
    T = thick(geo)
    n = len(sel)
    out = np.zeros((n, 4))
    done = np.zeros(n, bool)
    steps = 24
    prev_iu = None
    content_done = recess is None
    for k in range(steps + 1):
        q = k / steps
        pq = plane + sgn * q * T
        with np.errstate(divide="ignore", invalid="ignore"):
            if axis == "z":
                s = (pq - pose.ez) / rz
                al = pose.ex + rx * s
            else:
                s = (pq - pose.ex) / rx
                al = pose.ez + rz * s
            u = face_u(axis, sgn, al, a0, a1)
        v = 0.5 - ry * s
        bad = ~np.isfinite(s) | (s <= 0)
        iu = np.floor(np.nan_to_num(u, nan=-1.0, posinf=-1.0, neginf=-1.0) * TS).astype(int)
        iv = np.floor(np.nan_to_num(v, nan=-1.0, posinf=-1.0, neginf=-1.0) * TS).astype(int)
        inb = (iu >= 0) & (iu < TS) & (iv >= 0) & (iv < TS) & ~bad
        cu, cv = np.clip(iu, 0, TS - 1), np.clip(iv, 0, TS - 1)
        solid = ~inb | ~mask[cv, cu]
        if k == 0:
            done |= solid
            prev_iu = cu
            continue
        hit = solid & ~done
        if hit.any():
            pu = np.clip(prev_iu, 0, TS - 1)
            horiz = ~mask[cv, pu] | ~inb
            depth_t = np.clip((q * geo.wall_thick_64), 0, TS - 1).astype(int)
            col = np.where(horiz[:, None], cap[depth_t, cu][:, :3], cap[cv, depth_t][:, :3]).astype(float)
            fac = np.where(horiz, np.where(v < 0.5, SOFFIT_SHADE, SILL_SHADE), JAMB_SHADE)
            col *= (fac * shade(np.clip(s, 0.01, 99)))[:, None]
            out[hit, :3] = col[hit]
            out[hit, 3] = 255
            done |= hit
        if not content_done and q >= recess - 1e-9:
            content_done = True
            texel = tex[cv, cu]
            show = ~done & inb & (texel[:, 3] > 0)
            col = texel[:, :3].astype(float) * (CONTENT_SHADE * shade(np.clip(s, 0.01, 99)))[:, None]
            out[show, :3] = col[show]
            out[show, 3] = 255
            done |= show
        prev_iu = cu
    return out[:, 3] > 0, out


def crop_flat(geo, sel, rgba):
    """Pixels (flat indices sel, rgba rows) -> (x0 even, y0, rgba crop) or None."""
    if len(sel) == 0:
        return None
    ys, xs = np.divmod(sel, geo.view_w)
    bx0, bx1 = xs.min() & ~1, (xs.max() + 2) & ~1
    by0, by1 = ys.min(), ys.max() + 1
    img = np.zeros((by1 - by0, bx1 - bx0, 4))
    img[ys - by0, xs - bx0] = rgba
    return bx0, by0, img


def build_pose(geo, pose, set_tex, stone, cap, floor, ceil):
    """-> (ordered slots, backgrounds[8]). A slot: dict(kind 0..3 = side of the eye-side cell / 4 = vertex, d, x,
    axis, px, pz, pieces: list per set (edge) or [piece] (vertex))."""
    P, t = geo.P, geo.t
    cand = []

    def add(kind, d, x, axis, plane, centre, box, geo_ok):
        cand.append(dict(kind=kind, d=d, x=x, axis=axis, plane=plane, centre=centre, box=box, ok=geo_ok))

    for d in range(GRID_D[0], GRID_D[1]):            # Z bands: far edge of (d, x)
        lo = geo.far(d) / 10.0
        for x in range(GRID_X[0] + 1, GRID_X[1]):
            f = facing(pose, "z", lo, lo + t)
            if f is None:
                continue
            a0, a1 = x * P - 0.5, x * P + 0.5
            ok, tt, al, v = pose_face(geo, pose, "z", f[0], a0, a1)
            if ok.any():
                kind, cd = (0, d) if f[1] > 0 else (2, d + 1)
                add(kind, cd, x, "z", f, x * P, (a0, a1, lo, lo + t), (ok, tt, al, v, a0, a1))
    for d in range(GRID_D[0] + 1, GRID_D[1]):        # X bands: right edge of (d, x)
        a0, a1 = geo.near(d) / 10.0, geo.far(d) / 10.0
        for x in range(GRID_X[0], GRID_X[1]):
            lo = x * P + 0.5
            f = facing(pose, "x", lo, lo + t)
            if f is None:
                continue
            ok, tt, al, v = pose_face(geo, pose, "x", f[0], a0, a1)
            if ok.any():
                kind, cx = (1, x) if f[1] > 0 else (3, x + 1)
                add(kind, d, cx, "x", f, (a0 + a1) / 2, (lo, lo + t, a0, a1), (ok, tt, al, v, a0, a1))
    for vd in range(GRID_D[0] + 1, GRID_D[1] + 1):   # vertex squares between rows vd - 1, vd and columns vx - 1, vx
        z0 = geo.far(vd - 1) / 10.0
        for vx in range(GRID_X[0] + 1, GRID_X[1] + 1):
            x0 = (vx - 1) * P + 0.5
            parts = []
            for axis, lo, a0, a1 in (("z", z0, x0, x0 + t), ("x", x0, z0, z0 + t)):
                f = facing(pose, axis, lo, lo + t)
                if f is None:
                    continue
                ok, tt, al, v = pose_face(geo, pose, axis, f[0], a0, a1)
                if ok.any():
                    parts.append((axis, f, ok, tt, al, v, a0, a1))
            if parts:
                add(4, vd, vx, "p", None, None, (x0, x0 + t, z0, z0 + t), parts)

    # paint
    slots = []
    for c in cand:
        if c["kind"] == 4:
            sel_all, rgba_all = [], []
            for axis, f, ok, t, al, v, a0, a1 in c["ok"]:
                sel = np.nonzero(ok.ravel())[0]
                tt, uu, vv = t.ravel()[sel], face_u(axis, f[1], al.ravel()[sel], a0, a1), v.ravel()[sel]
                # stone at the wall texel density (64 texels per cell)
                tq = al.ravel()[sel] - a0
                rev = (axis == "z") == (f[1] < 0)
                uu = np.mod(1.0 - tq if rev else tq, 1.0)
                nf = abs(pose.fwd[1] if axis == "z" else pose.fwd[0])
                col = sample(stone, uu, vv).astype(float)
                col[:, :3] *= ((1.0 - (1.0 - PILLAR_FRONT_SHADE) * nf) * shade(tt))[:, None]
                col[:, 3] = 255
                sel_all.append(sel)
                rgba_all.append(col)
            sel = np.concatenate(sel_all)
            r = crop_flat(geo, sel, np.concatenate(rgba_all))
            c["pieces"] = [r]
            ys, xs = np.divmod(sel, geo.view_w)
        else:
            ok, t, al, v, a0, a1 = c["ok"]
            axis, (plane, sgn) = c["axis"], c["plane"]
            sel = np.nonzero(ok.ravel())[0]
            tt, uu, vv = t.ravel()[sel], face_u(axis, sgn, al.ravel()[sel], a0, a1), v.ravel()[sel]
            k = shade(tt)
            c["pieces"] = []
            for (stem_t, mask, recess, tfull) in set_tex:
                col = sample(stem_t, uu, vv).astype(float)
                col[:, :3] *= k[:, None]
                if mask is not None:
                    holes = col[:, 3] == 0
                    if holes.any():
                        fill, rgba = march_pose(geo, pose, sel[holes], (axis, plane, sgn, a0, a1), tfull, mask,
                                                recess, cap)
                        hc = col[holes]
                        hc[fill] = rgba[fill]
                        col[holes] = hc
                keep = col[:, 3] > 0
                c["pieces"].append(crop_flat(geo, sel[keep], col[keep]))
            ys, xs = np.divmod(sel, geo.view_w)
        c["bbox"] = (ys.min(), ys.max() + 1, xs.min(), xs.max() + 1)
        bx = c["box"]
        cxm, czm = (bx[0] + bx[1]) / 2 - pose.ex, (bx[2] + bx[3]) / 2 - pose.ez
        c["depth"] = cxm * pose.fwd[0] + czm * pose.fwd[1]
        slots.append(c)

    # painter's order: separating-axis rule on the XZ boxes for every pair that overlaps on screen
    n = len(slots)
    after = [[] for _ in range(n)]
    indeg = [0] * n

    def rule(A, B, lo, hi, e):
        # A on the low side of B along an axis (A[hi] <= B[lo]): eye beyond -> A first (1), eye before -> B first
        if A[hi] <= B[lo] + 1e-9:
            if e >= B[lo] - 1e-9:
                return 1
            if e <= A[hi] + 1e-9:
                return -1
        elif B[hi] <= A[lo] + 1e-9:
            if e >= A[lo] - 1e-9:
                return -1
            if e <= B[hi] + 1e-9:
                return 1
        return 0

    for i in range(n):
        bi = slots[i]["bbox"]
        for j in range(i + 1, n):
            bj = slots[j]["bbox"]
            if bi[0] >= bj[1] or bj[0] >= bi[1] or bi[2] >= bj[3] or bj[2] >= bi[3]:
                continue
            A, B = slots[i]["box"], slots[j]["box"]
            rs = {rule(A, B, 0, 1, pose.ex), rule(A, B, 2, 3, pose.ez)} - {0}
            if len(rs) != 1:
                continue
            a, b = (i, j) if rs.pop() == 1 else (j, i)
            after[a].append(b)
            indeg[b] += 1
    import heapq
    heap = [(-slots[i]["depth"], i) for i in range(n) if indeg[i] == 0]
    heapq.heapify(heap)
    order, placed = [], [False] * n
    while len(order) < n:
        if not heap:        # cycle: take the farthest remaining
            i = max((k for k in range(n) if not placed[k]), key=lambda k: slots[k]["depth"])
            indeg[i] = 0
            heap.append((-slots[i]["depth"], i))
        _dk, i = heapq.heappop(heap)
        if placed[i]:
            continue
        placed[i] = True
        order.append(slots[i])
        for b in after[i]:
            indeg[b] -= 1
            if indeg[b] == 0 and not placed[b]:
                heapq.heappush(heap, (-slots[b]["depth"], b))
    for c in order:
        if c["kind"] == 4:
            c["px"], c["pz"] = c["box"][0] + geo.t / 2, c["box"][2] + geo.t / 2
            c["axis_id"] = 2
        elif c["axis"] == "z":
            c["px"], c["pz"], c["axis_id"] = c["centre"], c["plane"][0], 0
        else:
            c["px"], c["pz"], c["axis_id"] = c["plane"][0], c["centre"], 1
    bgs = [project_floor_ceiling(geo, floor, ceil, p, dr, pose) for dr in range(4) for p in range(2)]
    return order, bgs


def spans_fast(x0, y0, idx, alpha):
    """spans_of() with numpy: -> list of (y, x2, n2, halfword array)."""
    h, w = idx.shape
    if w & 1:
        idx = np.concatenate([idx, idx[:, -1:]], axis=1)
        alpha = np.concatenate([alpha, np.zeros((h, 1), bool)], axis=1)
        w += 1
    a = alpha.reshape(h, -1, 2)
    p = idx.reshape(h, -1, 2).copy()
    m0 = ~a[..., 0] & a[..., 1]
    p[..., 0][m0] = p[..., 1][m0]
    m1 = a[..., 0] & ~a[..., 1]
    p[..., 1][m1] = p[..., 0][m1]
    on = a.any(2)
    hw = (p[..., 0].astype(np.uint16) | (p[..., 1].astype(np.uint16) << 8))
    pad = np.zeros((h, on.shape[1] + 2), np.int8)
    pad[:, 1:-1] = on
    dd = np.diff(pad, axis=1)
    st = np.argwhere(dd == 1)
    en = np.argwhere(dd == -1)
    return [(y0 + r, (x0 >> 1) + s, e - s, hw[r, s:e]) for (r, s), (_r2, e) in zip(st, en)]


def paste(master, other):
    """other piece (x0, y0, rgba) resampled into the frame of the master piece (theme 0): rgba of the master's size."""
    mx, my, mimg = master
    out = np.zeros_like(mimg)
    if other is None:
        return out
    ox, oy, oimg = other
    x0, y0 = max(mx, ox), max(my, oy)
    x1, y1 = min(mx + mimg.shape[1], ox + oimg.shape[1]), min(my + mimg.shape[0], oy + oimg.shape[0])
    if x1 > x0 and y1 > y0:
        out[y0 - my:y1 - my, x0 - mx:x1 - mx] = oimg[y0 - oy:y1 - oy, x0 - ox:x1 - ox]
    return out


def quarter_spans(x0, y0, idx, alpha):
    """Quarter-resolution spans (G-37 motion poses): 2x2-px blocks aligned to even page rows and 2-px columns; a
    block takes its first opaque pixel (row-major) and is on when any pixel is. -> [(half row, x2, n2, bytes)]."""
    h, w = idx.shape
    top, bot, right = y0 & 1, (y0 + h) & 1, w & 1
    idx = np.pad(idx, ((top, bot), (0, right)))
    alpha = np.pad(alpha, ((top, bot), (0, right)))
    H, W = idx.shape[0] // 2, idx.shape[1] // 2
    a4 = alpha.reshape(H, 2, W, 2).transpose(0, 2, 1, 3).reshape(H, W, 4)
    p4 = idx.reshape(H, 2, W, 2).transpose(0, 2, 1, 3).reshape(H, W, 4)
    on = a4.any(2)
    val = np.take_along_axis(p4, a4.argmax(2)[..., None], 2)[..., 0].astype(np.uint8)
    pad = np.zeros((H, W + 2), np.int8)
    pad[:, 1:-1] = on
    dd = np.diff(pad, axis=1)
    st = np.argwhere(dd == 1)
    en = np.argwhere(dd == -1)
    Y0 = (y0 - top) >> 1
    return [(Y0 + r, (x0 >> 1) + s, e - s, val[r, s:e]) for (r, s), (_r2, e) in zip(st, en)]


def build_motion(geo, themes, luts, quarter_motion):
    """Renders every motion pose for every theme and writes dl_view_motion_data.h / .cpp, the shared geometry blob
    (pieces + spans, src/dl_view_motion.bin), one pixel bank per theme (src/dl_view_theme_<k>.bin, same layout:
    a span's offset is valid in every bank) and the .s wrapper. Pieces, spans and the transparency come from
    theme 0; the other themes only change the pixel values (G-37). -> (geometry bytes, bank bytes)."""
    poses = motion_poses(geo) + [("rest", Pose(geo, 0.0, 0.0, 0.0))]
    nt = len(themes)
    pix = [[] for _ in range(nt)]   # per theme: list of uint16 arrays (same lengths in every theme)
    npix = 0
    spans = []          # (y, x2, n2, off)
    pieces = []         # (first, count, c0, c1)
    slot_rows = []
    pose_rows = []
    dedup = {}          # (parity, contents in every theme) -> offset
    stats = dict(alpha_lost=0, dedup=0)

    def put(k, arr):
        pix[k].append(arr)

    def pad_to(mod4):
        nonlocal npix
        while npix % 4 != mod4:
            for k in range(nt):
                put(k, np.zeros(1, np.uint8))
            npix += 1

    def emit(rs, share=True, quarter=False):
        """One piece for every theme. Full resolution: spans of 2-px pairs (halfwords, offset in halfwords with the
        page's word parity). quarter (motion poses, G-37): 2x2-px blocks, one byte per 2-px column on two page
        rows, span y = half row, offset in bytes."""
        nonlocal npix
        r = rs[0]
        if r is None:
            pieces.append((0, 0, 0, 0))
            return
        x0, y0, img = r
        alpha = img[..., 3] > 0
        idxs = []
        for k in range(nt):
            im = img if k == 0 else paste(r, rs[k])
            if k:
                stats["alpha_lost"] += int((alpha & ~(im[..., 3] > 0)).sum())
            c = to555(im[..., :3])
            idxs.append(luts[k][c[..., 0] * 1024 + c[..., 1] * 32 + c[..., 2]])
        cut = quarter_spans if quarter else spans_fast
        runs = [cut(x0, y0, idxs[k], alpha) for k in range(nt)]
        first = len(spans)
        for j, (y, x2, n2, d0) in enumerate(runs[0]):
            ds = [runs[k][j][3].view(np.uint8) if not quarter else runs[k][j][3] for k in range(nt)]
            key = None
            if share:
                key = (quarter, x2 & 1, b"".join(d.tobytes() for d in ds))
                off = dedup.get(key)
                if off is not None:
                    spans.append((y, x2, n2, off))
                    stats["dedup"] += len(ds[0])
                    continue
            if not quarter:
                pad_to(2 * (x2 & 1))
            off = npix if quarter else npix // 2
            spans.append((y, x2, n2, off))
            if key is not None:
                dedup[key] = off
            for k in range(nt):
                put(k, ds[k])
            npix += len(ds[0])
        pieces.append((first, len(spans) - first, x0 >> 1, (x0 + img.shape[1] + 1) >> 1))

    for pi, (kind, pose) in enumerate(poses):
        q = quarter_motion and kind != "rest"
        built = [build_pose(geo, pose, T["sets_tex"], T["stone"], T["cap"], T["floor"], T["ceil"]) for T in themes]
        order, bgs = built[0]
        first_slot = len(slot_rows)
        for si, c in enumerate(order):
            slot_rows.append((c["d"], c["x"], c["kind"], c["axis_id"], int(round(c["px"] * U)),
                              int(round(c["pz"] * U)), len(pieces)))
            for pj, r in enumerate(c["pieces"]):
                emit([r] + [built[k][0][si]["pieces"][pj] for k in range(1, nt)], quarter=q)
        bg0 = len(pieces)
        for bj, r in enumerate(bgs):
            # contiguous image: 120 rows x 88 halfwords (quarter: 60 half rows x 88 bytes)
            emit([r] + [built[k][1][bj] for k in range(1, nt)], share=False, quarter=q)
        pose_rows.append((first_slot, len(order), int(round(pose.ex * U)), int(round(pose.ez * U)),
                          int(round(np.cos(pose.yaw) * 16384)), int(round(np.sin(pose.yaw) * 16384)), bg0))
        print("build_view: pose %s %d: %d slots" % (kind, pi % MOTION_FRAMES + 1, len(order)), flush=True)
    pad_to(0)
    pa = np.array(pieces, dtype=[("first", "<u4"), ("count", "<u2"), ("c0", "u1"), ("c1", "u1")])
    sa = np.array(spans, dtype=[("y", "u1"), ("x2", "u1"), ("n2", "<u2"), ("off", "<u4")])
    blob = pa.tobytes() + sa.tobytes()
    with open(MOTION_BIN, "wb") as f:
        f.write(blob)
    import hashlib
    digest = hashlib.sha1(blob)
    for old in glob.glob(os.path.join(PROJ, "src", "dl_view_theme_*.bin")):
        os.remove(old)
    bank_bytes = 0
    for k in range(nt):
        bank = np.concatenate(pix[k]).astype(np.uint8).tobytes()
        bank_bytes = len(bank)
        digest.update(bank)
        with open(THEME_BIN % k, "wb") as f:
            f.write(bank)
    sl = ["@ GENERATED by tools/build_view.py (G-31 motion poses, G-37 themes). Do not edit. blobs %s, geometry %d "
          "bytes, %d theme banks of %d bytes" % (digest.hexdigest()[:16], len(blob), nt, bank_bytes),
          "    .section .rodata", "    .align 2", "    .global dl_view_motion_blob", "dl_view_motion_blob:",
          "    .incbin \"../src/dl_view_motion.bin\""]
    for k in range(nt):
        sl += ["    .align 2", "    .global dl_view_theme_%d" % k, "dl_view_theme_%d:" % k,
               "    .incbin \"../src/dl_view_theme_%d.bin\"" % k]
    with open(MOTION_S, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(sl) + "\n")
    hl = ["// GENERATED by tools/build_view.py (G-31 motion poses, G-37 themes). Do not edit.",
          "#ifndef DL_VIEW_MOTION_DATA_H", "#define DL_VIEW_MOTION_DATA_H", "", "#include <stdint.h>", "",
          '#include "dl_view_data.h"', "",
          "extern \"C\" const uint8_t dl_view_motion_blob[];"] + [
          "extern \"C\" const uint8_t dl_view_theme_%d[];" % k for k in range(nt)] + [
          "", "namespace dl::view_motion_data", "{",
          "    constexpr int motion_frames = %d;    // intermediate poses per move kind" % MOTION_FRAMES,
          "    constexpr int kinds = %d;            // step, strafe, turn (right)" % len(MOTION_KINDS),
          "    constexpr int pitch_u = %d;          // cell pitch in u (dl_view_geometry.h)" % int(round(geo.P * U)),
          "    constexpr int grid_d0 = %d, grid_d1 = %d;    // base-relative cells d = grid_d0 .. grid_d1" % GRID_D,
          "    constexpr int grid_x0 = %d, grid_x1 = %d;" % GRID_X,
          "    constexpr int vertex_kind = 4;",
          "", "    // piece: spans [first, first + count) of the motion span table, 2-px columns [c0, c1)",
          "    struct piece", "    {", "        uint32_t first;", "        uint16_t count;", "        uint8_t c0;",
          "        uint8_t c1;", "    };", "",
          "    // slot in painter's order: kind 0..3 = face of the edge on side kind (0 ahead, 1 right, 2 back, 3 left)",
          "    // of base-relative cell (d, x), pieces [piece + set]; kind 4 = vertex block between rows d - 1, d and",
          "    // columns x - 1, x, one piece. axis 0: face plane Z = pz_u (centre X px_u), 1: plane X = px_u (centre",
          "    // Z pz_u), 2: block centre (px_u, pz_u); u = 1/320 cell in the resting eye frame of the base cell.",
          "    struct slot", "    {", "        int8_t d;", "        int8_t x;", "        uint8_t kind;",
          "        uint8_t axis;", "        int16_t px_u;", "        int16_t pz_u;", "        uint32_t piece;", "    };",
          "", "    // pose: eye (ex_u, ez_u), yaw cos / sin (q14), floor and ceiling pieces [bg + dir * 2 + parity]",
          "    struct pose", "    {", "        uint16_t first_slot;", "        uint16_t slots;", "        int16_t ex_u;",
          "        int16_t ez_u;", "        int16_t cos_q14;", "        int16_t sin_q14;", "        uint32_t bg;",
          "    };", "",
          "    constexpr uint32_t spans_offset = %d;     // bytes into dl_view_motion_blob" % pa.nbytes,
          "    constexpr uint32_t theme_bank_bytes = %d;  // pixel bank of one theme (dl_view_theme_<k>)" % bank_bytes,
          "    // motion poses at quarter resolution (G-37 ROM budget): their spans are 2x2-px blocks, y = half row,",
          "    // one byte per 2-px column at byte offset off; floor / ceiling 60 half rows x 88 bytes",
          "    constexpr bool motion_quarter = %s;" % ("true" if quarter_motion else "false"),
          "", "    extern const slot slots[%d];" % len(slot_rows),
          "    constexpr int rest_pose = kinds * motion_frames;    // the resting view",
          "    extern const pose poses[kinds * motion_frames + 1];  // [kind * motion_frames + k - 1], rest_pose", "",
          "    // pixel bank of every theme (same layout) and the one in use (dl::themes::apply)",
          "    extern const uint16_t* const theme_pixels[dl::view_data::theme_count];",
          "    extern const uint16_t* current_pixels;", "",
          "    inline const piece* pieces()", "    {",
          "        return reinterpret_cast<const piece*>(dl_view_motion_blob);", "    }", "",
          "    inline const dl::view_data::span* spans()", "    {",
          "        return reinterpret_cast<const dl::view_data::span*>(dl_view_motion_blob + spans_offset);", "    }", "",
          "    inline const uint16_t* pixels()", "    {", "        return current_pixels;", "    }", "}", "",
          "#endif"]
    with open(MOTION_H, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(hl) + "\n")
    cl = ["// GENERATED by tools/build_view.py (G-31 motion poses, G-37 themes). Do not edit.",
          '#include "dl_view_motion_data.h"', "", "namespace dl::view_motion_data", "{",
          "    const slot slots[%d] = {" % len(slot_rows)]
    for i in range(0, len(slot_rows), 6):
        cl.append("        " + "".join("{%d,%d,%d,%d,%d,%d,%d}," % s for s in slot_rows[i:i + 6]))
    cl += ["    };", "", "    const pose poses[kinds * motion_frames + 1] = {"]
    cl += ["        {%d,%d,%d,%d,%d,%d,%d}," % p for p in pose_rows]
    cl += ["    };", "", "    const uint16_t* const theme_pixels[dl::view_data::theme_count] = {"]
    cl += ["        reinterpret_cast<const uint16_t*>(dl_view_theme_%d)," % k for k in range(nt)]
    cl += ["    };", "", "    const uint16_t* current_pixels = reinterpret_cast<const uint16_t*>(dl_view_theme_0);",
           "}"]
    with open(MOTION_CPP, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(cl) + "\n")
    print("build_view: motion %d poses, %d slots, %d pieces, %d spans, geometry %d KB, %d theme banks of %d KB "
          "(span dedup saved %d KB per bank), transparency differing from theme 0: %d px" % (
              len(poses), len(slot_rows), len(pieces), len(spans), len(blob) // 1024, nt, bank_bytes // 1024,
              stats["dedup"] * 2 // 1024, stats["alpha_lost"]))
    return len(blob), bank_bytes


# ----------------------------------------------------------------------------------------------- quantize + emit
def to555(rgb):
    return np.clip(np.round(rgb / 255.0 * 31), 0, 31).astype(int)


def quantize(pieces):
    """pieces: list of (x0, y0, rgba float). Returns (palette list of 192 (r,g,b) 5-bit, index arrays)."""
    cols = []
    for (_x, _y, px) in pieces:
        m = px[..., 3] > 0
        cols.append(to555(px[m][:, :3]))
    allc = np.concatenate(cols)
    keys = allc[:, 0] * 1024 + allc[:, 1] * 32 + allc[:, 2]
    uniq, counts = np.unique(keys, return_counts=True)
    keep = uniq != 0
    uniq, counts = uniq[keep], counts[keep]
    if len(uniq) <= 191:
        pal = [((k >> 10) & 31, (k >> 5) & 31, k & 31) for k in uniq]
    else:
        # weighted median cut via PIL on the unique colours repeated by (clipped) frequency
        w = np.clip(counts // 8 + 1, 1, 64)
        rep = np.repeat(uniq, w)
        arr = np.stack([(rep >> 10) & 31, (rep >> 5) & 31, rep & 31], axis=1).astype(np.uint8) * 8
        im = Image.fromarray(arr.reshape(1, -1, 3), "RGB")
        q = im.quantize(colors=191, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
        p = q.getpalette()[:191 * 3]
        pal = sorted({(p[i * 3] >> 3, p[i * 3 + 1] >> 3, p[i * 3 + 2] >> 3) for i in range(191)} - {(0, 0, 0)})
    pal_arr = np.array([(0, 0, 0)] + pal, int)           # index 0 = black
    ucol = np.stack([(uniq >> 10) & 31, (uniq >> 5) & 31, uniq & 31], axis=1)
    near = ((ucol[:, None, :] - pal_arr[None, :, :]) ** 2).sum(-1)
    near[:, 0] = 1 << 30                                  # only true black maps to 0
    lut = dict(zip(uniq.tolist(), near.argmin(1).tolist()))
    lut[0] = 0
    out = []
    for (x0, y0, px) in pieces:
        c = to555(px[..., :3])
        k = c[..., 0] * 1024 + c[..., 1] * 32 + c[..., 2]
        idx = np.vectorize(lambda v: lut.get(v, 0))(k)
        out.append(idx)
    return pal_arr, out


def palette_lut(pal_arr):
    """555 key -> palette index (nearest, only true black maps to 0), for every 32768 colours."""
    keys = np.arange(32768)
    rgb = np.stack([(keys >> 10) & 31, (keys >> 5) & 31, keys & 31], axis=1)
    lut = np.zeros(32768, np.uint8)
    for i in range(0, 32768, 4096):
        dd = ((rgb[i:i + 4096, None, :] - pal_arr[None, :, :]) ** 2).sum(-1)
        dd[:, 0] = 1 << 30
        lut[i:i + 4096] = dd.argmin(1)
    lut[0] = 0
    return lut


THEMES_DIR = os.path.join(PROJ, "art", "themes")
THEME_BIN = os.path.join(PROJ, "src", "dl_view_theme_%d.bin")
# measured (G-37): geometry 3.15 MB + 8.91 MB per theme at full resolution -> 3 themes fit
FULL_RES_MAX_THEMES = 3
ROM_VIEW_BUDGET = 30 * 1024 * 1024     # view data (geometry + theme banks); the rest of the ROM is < 1 MB (G-37)


def discover_themes(extra, skip_art):
    """[(name, export dir or None)]: theme 0 = art/export ("sewer"), then art/themes/<name>/export in name order,
    then the --theme NAME=DIR extras."""
    themes = [("sewer", None)]
    if not skip_art and os.path.isdir(THEMES_DIR):
        for name in sorted(os.listdir(THEMES_DIR)):
            d = os.path.join(THEMES_DIR, name, "export")
            if os.path.isdir(d) and glob.glob(os.path.join(d, "tex_*.bmp")):
                themes.append((name, d))
    for spec in extra:
        name, _sep, d = spec.partition("=")
        if not os.path.isdir(d):
            sys.exit("build_view: --theme %s: no such directory" % spec)
        themes.append((name, d))
    return themes


def load_theme(name, export_dir):
    """Textures of one theme -> dict(sets_tex, stone, cap, floor, ceil, used). A file missing from the theme's export
    falls back to theme 0 (reported); the opening masks always come from theme 0 (shared geometry)."""
    global THEME_EXPORT
    THEME_EXPORT = export_dir
    used = {}
    texcache = {}

    def own(stem):
        return export_dir is None or os.path.exists(os.path.join(export_dir, stem + ".bmp"))

    def tex(stem, k):
        if stem not in texcache:
            base, _sep, var = stem.partition("#")
            art_var = [os.path.join(export_dir or ART_EXPORT, "%s_v%s.bmp" % (base, var))]
            if var and any(os.path.exists(p) for p in art_var):
                texcache[stem], used[stem] = load_texture("%s_v%s" % (base, var))
            elif var:
                t, used[stem] = load_texture(base)
                shift, mirror = WALL_VARIANTS[int(var)]
                t = np.roll(t[:TS], shift, axis=1)
                texcache[stem] = t[:, ::-1].copy() if mirror else t
            elif stem == "tex_door_locked" and export_dir and not own(stem):
                texcache[stem] = make_locked_placeholder(tex("tex_door", 0))   # PROVISIONAL, from the theme's door
                used[stem] = "placeholder from %s" % used["tex_door"]
            else:
                texcache[stem], used[stem] = load_texture(stem)
        return frame(texcache[stem], k)

    cap = cap_texture(tex("tex_wall_stone", 0))
    stone = tex("tex_wall_stone", 0)
    floor, ceil = tex("tex_floor", 0), tex("tex_ceiling", 0)
    sets_tex = []
    for (_name, stem, k, opening, _opaque) in SETS:
        t = tex(stem, k)
        if opening:
            mask = build_mask(opening, t)
            face_t = t.copy()
            face_t[:TS][mask, 3] = 0
            sets_tex.append((face_t, mask, OPENINGS[opening][2], t))
        else:
            sets_tex.append((t, None, None, t))
    THEME_EXPORT = None
    if export_dir:
        borrowed = sorted(s for s, p in used.items() if isinstance(p, str) and os.path.dirname(p) != export_dir
                          and not p.startswith("placeholder"))
        if borrowed:
            print("build_view: theme %s borrows from theme 0: %s" % (name, ", ".join(borrowed)))
    return dict(name=name, sets_tex=sets_tex, stone=stone, cap=cap, floor=floor, ceil=ceil, used=used)


def main():
    import argparse
    ap = argparse.ArgumentParser(description="DELVE view pieces builder")
    ap.add_argument("--theme", action="append", default=[], metavar="NAME=DIR",
                    help="extra theme from an export dir (tests); art/themes/*/export are found by themselves")
    ap.add_argument("--no-art-themes", action="store_true", help="ignore art/themes (theme 0 + --theme only)")
    ap.add_argument("--motion-res", choices=("auto", "full", "quarter"), default="auto",
                    help="motion poses resolution; auto = full while the themes fit (%d), else quarter" %
                    FULL_RES_MAX_THEMES)
    ap.add_argument("--max-themes", type=int, default=0, help="keep only the first N themes")
    args = ap.parse_args()
    geo = Geo(parse_geometry())
    if not os.path.isdir(ART_EXPORT):
        make_test_textures()
    make_locked_placeholder()
    found = discover_themes(args.theme, args.no_art_themes)
    if args.max_themes:
        found = found[:args.max_themes]
    themes = [load_theme(n, d) for n, d in found]
    print("build_view: %d themes: %s" % (len(themes), ", ".join(n for n, _d in found)))

    # palette of each theme from its resting view (every set at every slot + all floor / ceiling images)
    pals, luts = [], []
    for T in themes:
        order, bgs = build_pose(geo, Pose(geo, 0.0, 0.0, 0.0), T["sets_tex"], T["stone"], T["cap"], T["floor"],
                                T["ceil"])
        rest_pieces = [r for c in order for r in c["pieces"] if r is not None] + list(bgs)
        pal, _idxs = quantize(rest_pieces)
        pals.append(pal)
        luts.append(palette_lut(pal))
    quarter = args.motion_res == "quarter" or (args.motion_res == "auto" and len(themes) > FULL_RES_MAX_THEMES)
    print("build_view: motion poses at %s resolution" % ("quarter" if quarter else "full"))
    geo_bytes, bank_bytes = build_motion(geo, themes, luts, quarter)
    total = geo_bytes + bank_bytes * len(themes)
    print("build_view: ROM view data %.2f MB (geometry %.2f + %d x %.2f), budget %.2f MB" % (
        total / 1048576, geo_bytes / 1048576, len(themes), bank_bytes / 1048576, ROM_VIEW_BUDGET / 1048576))

    used_all = sorted({p for T in themes for p in T["used"].values() if not p.startswith("placeholder")})
    lines = ["// GENERATED by tools/build_view.py from %s. Do not edit." % ", ".join(
        os.path.relpath(p, PROJ).replace("\\", "/") for p in used_all if p.startswith(PROJ)),
        '#include "dl_view_data.h"\n', "namespace dl::view_data\n{"]
    ui = Image.open(UI_FRAME).getpalette() if os.path.exists(UI_FRAME) else [0] * 768
    ui += [0] * (768 - len(ui))
    lines.append("    const bn::color theme_palette_colors[theme_count][256] = {")
    for ti, pal in enumerate(pals):
        full = [tuple(c) for c in pal] + [(0, 0, 0)] * (192 - len(pal))
        full += [(ui[i * 3] >> 3, ui[i * 3 + 1] >> 3, ui[i * 3 + 2] >> 3) for i in range(192, 256)]
        for k, c in enumerate(MAP_COLORS):          # engine minimap colours in the free UI slots 240..
            full[240 + k] = tuple(v >> 3 for v in c)
        lines.append("        {   // %d %s" % (ti, themes[ti]["name"]))
        for i in range(0, 256, 8):
            lines.append("            " + "".join("bn::color(%d,%d,%d)," % c for c in full[i:i + 8]))
        lines.append("        },")
    lines.append("    };\n")
    lines.append("    const bn::color* const palette_colors = theme_palette_colors[0];\n")
    # wall decoration palettes (art/themes/<name>/deco_palette.json: A torch / chains / lever, B banner, G glow);
    # theme 0 and themes without the file use the sprites' own palettes (graphics/deco_*_front.bmp)
    def bmp_pal(stem):
        p = Image.open(os.path.join(GFX, stem + ".bmp")).getpalette()[:48]
        return [tuple(v >> 3 for v in p[i * 3:i * 3 + 3]) for i in range(16)]

    base_deco = [bmp_pal("deco_torch_front"), bmp_pal("deco_banner_front")]
    lines.append("    const bn::color theme_deco_colors[theme_count][2][16] = {")
    glows = []
    for T, (_n, d) in zip(themes, found):
        jp = os.path.join(os.path.dirname(d), "deco_palette.json") if d else None
        deco, glow = base_deco, (31, 22, 11)
        if jp and os.path.exists(jp):
            import json
            with open(jp, encoding="utf-8") as f:
                js = json.load(f)
            hexc = lambda h: tuple(int(h[k:k + 2], 16) >> 3 for k in (1, 3, 5))
            deco = [[hexc(h) for h in js[k][:16]] for k in ("A", "B")]
            g = max((hexc(h) for h in js["G"][1:8]), key=sum)
            glow = tuple(v * 31 // max(1, max(g)) for v in g)
        glows.append(glow)
        lines.append("        {   // %s" % T["name"])
        for pal in deco:
            lines.append("            { %s }," % "".join("bn::color(%d,%d,%d)," % c for c in pal))
        lines.append("        },")
    lines.append("    };\n")
    lines.append("    const uint8_t theme_glow_rgb[theme_count][3] = { %s };\n" % ", ".join(
        "{%d,%d,%d}" % g for g in glows))
    lines.append("    const char* const theme_names[theme_count] = { %s };" % ", ".join(
        '"%s"' % T["name"] for T in themes))
    lines.append("}")
    with open(OUT_CPP, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")

    hl = ["// GENERATED by tools/build_view.py. Do not edit.", "#ifndef DL_VIEW_DATA_H", "#define DL_VIEW_DATA_H", "",
          '#include "bn_color.h"', "#include <stdint.h>", "", "namespace dl::view_data", "{",
          "    // a horizontal run of 2-px pairs: page row y, first pair x2 (pixel 2*x2), n2 pairs, data at pixels[off]",
          "    struct span", "    {", "        uint8_t y;", "        uint8_t x2;", "        uint16_t n2;",
          "        uint32_t off;", "    };", "", "    enum set_id : int", "    {"]
    for name, _s, _k, _o, _q in SETS:
        hl.append("        set_%s," % name)
    hl += ["        set_count", "    };", "",
           "    // opaque sets hide whatever is behind them (wall decorations behind them are not shown)",
           "    constexpr bool set_opaque[set_count] = { %s };" % ", ".join(
               "true" if q else "false" for _n, _s, _k, _o, q in SETS),
           "    constexpr int ui_first_color = 192;",
           "    constexpr int palette_used = %d;    // environment colours actually used (1..), largest theme" % (
               max(len(p) for p in pals) - 1),
           "    constexpr int wall_variants = 4;               // set_wall .. set_wall + 3",
           "",
           "    // dungeon themes (G-37): theme 0 = art/export, then art/themes/<name>/export by name",
           "    constexpr int theme_count = %d;" % len(themes),
           "    extern const char* const theme_names[theme_count];",
           "    extern const bn::color theme_palette_colors[theme_count][256];",
           "    extern const bn::color* const palette_colors;     // theme 0",
           "    // wall decoration sprite palettes per theme: [A torch / chains / lever, B banner][16]; torch glow colour",
           "    extern const bn::color theme_deco_colors[theme_count][2][16];",
           "    extern const uint8_t theme_glow_rgb[theme_count][3];", "}", "", "#endif"]
    with open(OUT_H, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(hl) + "\n")
    for T, pal in zip(themes, pals):
        print("build_view: theme %s: %d environment colours" % (T["name"], len(pal) - 1))
    if total > ROM_VIEW_BUDGET:
        sys.exit("build_view: ROM budget exceeded (%.2f MB > %.2f MB): use --max-themes" % (
            total / 1048576, ROM_VIEW_BUDGET / 1048576))


if __name__ == "__main__":
    main()
