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
CAP_LINES = [-5, -3, -1, 1, 3, 5]     # free wall ends: side face of the front wall of (d, x) on line l
JAMB_SHADE, SOFFIT_SHADE, SILL_SHADE, CONTENT_SHADE = 0.62, 0.48, 0.75, 0.9
ART_EXPORT = os.path.join(PROJ, "art", "export")
UI_FRAME = os.path.join(GFX, "ui_frame.bmp")
# minimap colours (palette 240..247; ui_frame uses 192-209): floor, wall, player, door, bars, stairs, lever, dark
MAP_COLORS = [(64, 58, 50), (206, 194, 160), (250, 226, 90), (176, 112, 52), (184, 184, 196), (90, 200, 230),
              (240, 200, 60), (14, 12, 18)]
LATERALS = [-7, -5, -3, -1, 1, 3, 5, 7]
# ----------------------------------------------------------------------------------------------- geometry
def parse_geometry():
    with open(GEO_H, encoding="utf-8") as f:
        text = f.read()
    g = {m.group(1): int(m.group(2)) for m in re.finditer(r"constexpr int (\w+) = (-?\d+);", text)}
    for k in ("view_w", "view_h", "vp_x", "vp_y", "proj_num", "eye_back_z10", "fog_z10", "depths", "max_lateral"):
        if k not in g:
            sys.exit("build_view: %s missing in %s" % (k, GEO_H))
    return g


class Geo:
    def __init__(self, g):
        self.__dict__.update(g)
        self.F = g["proj_num"] / 5.0       # px per cell

    def near(self, d):
        return 10 * d + 5 - self.eye_back_z10 + 4

    def far(self, d):
        return self.near(d) + 10

    def off(self, h, z10):
        a = abs(h)
        r = 2 * ((self.proj_num // 2 * a * 2 + z10) // (2 * z10))
        return -r if h < 0 else r


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


def load_texture(stem):
    """RGBA int array (h, 64, 4). Artist export first (graphics/<stem>.bmp), then the test texture."""
    for p in (os.path.join(ART_EXPORT, stem + ".bmp"), os.path.join(GFX, stem + ".bmp"),
              os.path.join(TEST_TEX, stem + ".png")):
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
def shade(z):
    """Distance attenuation (z in cells)."""
    return np.clip(np.exp(-0.42 * (z - 1.0)), 0.0, 1.0)     # fades toward black: the far end is lost in the dark


def sample(tex, u, v):
    tu = np.clip((u * TS).astype(int), 0, TS - 1)
    tv = np.clip((v * TS).astype(int), 0, TS - 1)
    return tex[tv, tu]


def project_front(geo, tex, d, x):
    """-> (x0, y0, rgba (h, w, 4), z) for the unclipped->clipped front face or None."""
    z10 = geo.far(d)
    ux0 = geo.vp_x + geo.off(2 * x - 1, z10)
    ux1 = geo.vp_x + geo.off(2 * x + 1, z10)
    hh = geo.off(1, z10)
    uy0, uy1 = geo.vp_y - hh, geo.vp_y + hh
    x0, x1 = max(ux0, 0), min(ux1, geo.view_w)
    y0, y1 = max(uy0, 0), min(uy1, geo.view_h)
    if x1 <= x0 or y1 <= y0:
        return None
    sx = np.arange(x0, x1)
    sy = np.arange(y0, y1)
    u = (sx + 0.5 - ux0) / (ux1 - ux0)
    v = (sy + 0.5 - uy0) / (uy1 - uy0)
    U, V = np.meshgrid(u, v)
    px = sample(tex, U, V).astype(float)
    px[..., :3] *= shade(z10 / 10.0)
    return x0, y0, px


def project_lateral(geo, tex, d, l):
    """Lateral face of row d on line X = l/2, 2-px column granularity. -> (x0, y0, rgba) or None."""
    zn, zf = geo.near(d) / 10.0, geo.far(d) / 10.0
    X = l / 2.0
    W, H = geo.view_w, geo.view_h
    img = np.zeros((H, W, 4), float)
    cov = np.zeros((H, W), bool)
    any_px = False
    for p in range(W // 2):
        cx = 2 * p + 1.0                      # pair centre
        dxc = cx - geo.vp_x
        if dxc == 0 or (dxc > 0) != (X > 0):
            continue
        zc = geo.F * X / dxc
        if zc < zn or zc > zf:
            continue
        hh = geo.F * 0.5 / zc
        y0 = max(int(round(geo.vp_y - hh)), 0)
        y1 = min(int(round(geo.vp_y + hh)), H)
        if y1 <= y0:
            continue
        for sx in (2 * p, 2 * p + 1):
            dx = sx + 0.5 - geo.vp_x
            z = geo.F * X / dx if (dx > 0) == (X > 0) and dx != 0 else zc
            z = min(max(z, zn), zf)
            u = (z - zn) if X < 0 else 1.0 - (z - zn)
            # vertical texture coordinate through the column's own wall height
            sy = np.arange(y0, y1)
            v = (sy + 0.5 - (geo.vp_y - hh)) / (2 * hh)
            col = sample(tex, np.full_like(v, u), v).astype(float)
            col[:, :3] *= shade(z)
            img[y0:y1, sx] = col
            cov[y0:y1, sx] = True
            any_px = True
    if not any_px:
        return None
    ys, xs = np.nonzero(cov)
    bx0, bx1 = xs.min() & ~1, (xs.max() + 2) & ~1
    by0, by1 = ys.min(), ys.max() + 1
    return bx0, by0, img[by0:by1, bx0:bx1], cov[by0:by1, bx0:bx1]


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
    return geo.wall_thick_64 / float(TS)


def ray_dirs(geo, sx, sy):
    return (sx + 0.5 - geo.vp_x) / geo.F, -(sy + 0.5 - geo.vp_y) / geo.F


def march_fill(geo, img, cov, x0, y0, face, tex, mask, recess, cap):
    """Fills the hole pixels of a framed piece: jambs / soffit / sill of the slab and the recessed content.
    face: ("z", Z, cell x) for a front face, ("x", X, zn, zf) for a lateral one."""
    hole = cov & (img[..., 3] == 0)
    ys, xs = np.nonzero(hole)
    if len(xs) == 0:
        return
    dx, dy = ray_dirs(geo, xs + x0, ys + y0)
    T = thick(geo)
    n = len(xs)
    out = np.zeros((n, 4))
    done = np.zeros(n, bool)
    steps = 24
    prev_iu = None
    content_done = recess is None
    for k in range(steps + 1):
        q = k / steps
        with np.errstate(divide="ignore", invalid="ignore"):
            if face[0] == "z":
                s = np.full(n, face[1] + q * T)
                u = dx * s - (face[2] - 0.5)
            else:
                L = face[1]
                s = (L + np.sign(L) * q * T) / dx
                u = (s - face[2]) if L < 0 else 1.0 - (s - face[2])
        v = 0.5 - dy * s
        bad = ~np.isfinite(s) | (s <= 0)
        iu = np.floor(np.nan_to_num(u, nan=-1.0) * TS).astype(int)
        iv = np.floor(np.nan_to_num(v, nan=-1.0) * TS).astype(int)
        inb = (iu >= 0) & (iu < TS) & (iv >= 0) & (iv < TS) & ~bad
        cu, cv = np.clip(iu, 0, TS - 1), np.clip(iv, 0, TS - 1)
        solid = ~inb | ~mask[cv, cu]
        if k == 0:
            done |= solid          # entered through the wall face itself (or outside the face): leave it alone
            prev_iu = cu
            continue
        hit = solid & ~done
        if hit.any():
            # which slab side was crossed: if the previous column at the current row is solid too, it was a row
            # boundary (soffit above the eye, sill below), otherwise a column boundary (jamb)
            pu = np.clip(prev_iu, 0, TS - 1)
            horiz = ~mask[cv, pu] | ~inb
            depth_t = np.clip((q * geo.wall_thick_64), 0, TS - 1).astype(int)
            jamb_col = cap[cv, depth_t]
            soff_col = cap[depth_t, cu]
            col = np.where(horiz[:, None], soff_col[:, :3], jamb_col[:, :3]).astype(float)
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
    fill = out[:, 3] > 0
    img[ys[fill], xs[fill]] = out[fill]


def project_cap(geo, cap, d, l):
    """Side face of a free wall end: line X = l/2 from the far edge of row d to the back of the slab."""
    W, H = geo.view_w, geo.view_h
    zf = geo.far(d) / 10.0
    T = thick(geo)
    X = l / 2.0
    sy, sx = np.mgrid[0:H, 0:W]
    dx, dy = ray_dirs(geo, sx, sy)
    with np.errstate(divide="ignore", invalid="ignore"):
        s = X / dx
    ok = np.isfinite(s) & (s >= zf) & (s <= zf + T)
    Y = dy * np.nan_to_num(s)
    ok &= (Y >= -0.5) & (Y < 0.5)
    if not ok.any():
        return None
    s = np.where(ok, s, zf)
    q = np.clip(((s - zf) / T * geo.wall_thick_64).astype(int), 0, TS - 1)
    v = np.clip(((0.5 - Y) * TS).astype(int), 0, TS - 1)
    img = np.zeros((H, W, 4), float)
    img[..., :3] = cap[v, q][..., :3] * (JAMB_SHADE * shade(s))[..., None]
    img[..., 3] = np.where(ok, 255, 0)
    ys, xs = np.nonzero(ok)
    bx0, bx1 = xs.min() & ~1, (xs.max() + 2) & ~1
    by0, by1 = ys.min(), ys.max() + 1
    return bx0, by0, img[by0:by1, bx0:bx1]


def make_locked_placeholder():
    """PROVISIONAL tex_door_locked until the artist draws it: tex_door with an iron band and a big iron padlock
    (iron = the Iron Key colour). Written to tools/test_textures/, never to art/."""
    if os.path.exists(os.path.join(ART_EXPORT, "tex_door_locked.bmp")) or             os.path.exists(os.path.join(GFX, "tex_door_locked.bmp")):
        return
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
    os.makedirs(TEST_TEX, exist_ok=True)
    save_rgba(os.path.join(TEST_TEX, "tex_door_locked.png"), np.clip(a, 0, 255))


def project_floor_ceiling(geo, floor, ceil, parity):
    """Floor and ceiling; the tile of relative cell (d, x) is mirrored when (d + x + parity) is odd: a world
    checkerboard, so every step changes the picture (parity = (cell x + cell y) & 1 of the player)."""
    W, H = geo.view_w, geo.view_h
    img = np.zeros((H, W, 4), float)
    img[..., 3] = 255
    zn0 = geo.near(0) / 10.0
    fog = geo.fog_z10 / 10.0
    sx = np.arange(W) + 0.5 - geo.vp_x
    for sy in range(H):
        dy = sy + 0.5 - geo.vp_y
        z = geo.F * 0.5 / abs(dy)
        if z > fog:
            continue
        X = sx * z / geo.F
        u = np.mod(X + 0.5, 1.0)
        vv = np.mod(z - zn0, 1.0)
        flip = (np.floor(X + 0.5).astype(int) + int(np.floor(z - zn0)) + parity) & 1
        u = np.where(flip == 1, 1.0 - 1e-6 - u, u)
        tex = floor if dy > 0 else ceil
        row = sample(tex, u, np.full_like(u, vv)).astype(float)
        row[:, :3] *= shade(z) * (1.0 if dy > 0 else 0.85)
        row[:, 3] = 255
        img[sy] = row
    return 0, 0, img


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


def spans_of(x0, y0, idx, alpha):
    """Spans at 2-px granularity. A pair is drawn if either pixel is opaque; a transparent partner copies its
    neighbour. Returns list of (y, x2, n2, halfwords)."""
    h, w = idx.shape
    if x0 & 1:
        sys.exit("build_view: odd piece x")
    if w & 1:
        idx = np.concatenate([idx, idx[:, -1:]], axis=1)
        alpha = np.concatenate([alpha, np.zeros((h, 1), bool)], axis=1)
        w += 1
    res = []
    for r in range(h):
        a = alpha[r].reshape(-1, 2)
        p = idx[r].reshape(-1, 2).copy()
        on = a.any(1)
        p[~a[:, 0] & a[:, 1], 0] = p[~a[:, 0] & a[:, 1], 1]
        p[a[:, 0] & ~a[:, 1], 1] = p[a[:, 0] & ~a[:, 1], 0]
        k = 0
        n = len(on)
        while k < n:
            if not on[k]:
                k += 1
                continue
            s = k
            while k < n and on[k]:
                k += 1
            hw = [int(p[j, 0]) | (int(p[j, 1]) << 8) for j in range(s, k)]
            res.append((y0 + r, (x0 >> 1) + s, k - s, hw))
    return res


def main():
    geo = Geo(parse_geometry())
    if not os.path.isdir(ART_EXPORT):
        make_test_textures()
    make_locked_placeholder()
    used = {}
    texcache = {}

    def tex(stem, k):
        if stem not in texcache:
            base, _sep, var = stem.partition("#")
            art_var = os.path.join(ART_EXPORT, "%s_v%s.bmp" % (base, var))
            if var and os.path.exists(art_var):
                texcache[stem], used[stem] = load_texture("%s_v%s" % (base, var))
            elif var:
                t, used[stem] = load_texture(base)
                shift, mirror = WALL_VARIANTS[int(var)]
                t = np.roll(t[:TS], shift, axis=1)
                texcache[stem] = t[:, ::-1].copy() if mirror else t
            else:
                texcache[stem], used[stem] = load_texture(stem)
        return frame(texcache[stem], k)

    cap = cap_texture(tex("tex_wall_stone", 0))
    pieces = []          # (x0, y0, rgba)
    index = []           # (set, kind, d, slot) per piece: kind 0 front, 1 lateral, 2 background, 3 cap
    for si, (name, stem, k, opening, _opaque) in enumerate(SETS):
        t = tex(stem, k)
        mask = None
        recess = None
        face_t = t
        if opening:
            mask = build_mask(opening, t)
            face_t = t.copy()
            face_t[:TS][mask, 3] = 0
            recess = OPENINGS[opening][2]
        for d in range(geo.depths):
            for x in range(-geo.max_lateral, geo.max_lateral + 1):
                r = project_front(geo, face_t, d, x)
                if r:
                    if mask is not None:
                        x0, y0, img = r
                        cov = np.ones(img.shape[:2], bool)
                        march_fill(geo, img, cov, x0, y0, ("z", geo.far(d) / 10.0, x), t, mask, recess, cap)
                    pieces.append(r)
                    index.append((si, 0, d, x + geo.max_lateral))
            for li, l in enumerate(LATERALS):
                r = project_lateral(geo, face_t, d, l)
                if r:
                    x0, y0, img, cov = r
                    if mask is not None:
                        march_fill(geo, img, cov, x0, y0, ("x", l / 2.0, geo.near(d) / 10.0, geo.far(d) / 10.0),
                                   t, mask, recess, cap)
                    if (img[..., 3] > 0).any():
                        pieces.append((x0, y0, img))
                        index.append((si, 1, d, li))
    for d in range(geo.depths - 1):          # row 4 caps lie in the fog
        for ci, l in enumerate(CAP_LINES):
            r = project_cap(geo, cap, d, l)
            if r:
                pieces.append(r)
                index.append((-1, 3, d, ci))
    for parity in (0, 1):
        pieces.append(project_floor_ceiling(geo, tex("tex_floor", 0), tex("tex_ceiling", 0), parity))
        index.append((-1, 2, 0, parity))
    pal, idxs = quantize(pieces)

    # emit
    pix = []             # all halfwords; each span's data starts at an offset with the parity of its x2
    spans = []           # (y, x2, n2, off)
    ptab = {}
    for (si, kind, d, slot), (x0, y0, px), idx in zip(index, pieces, idxs):
        first = len(spans)
        for (y, x2, n2, hw) in spans_of(x0, y0, idx, px[..., 3] > 0):
            dst_parity = ((y * SCREEN_W) // 2 + x2) & 1
            if (len(pix) & 1) != dst_parity:
                pix.append(0)
            spans.append((y, x2, n2, len(pix)))
            pix.extend(hw)
        ptab[(si, kind, d, slot)] = (first, len(spans) - first, x0 >> 1, (x0 + px.shape[1] + 1) >> 1)
    if len(pix) >= 1 << 22:
        sys.exit("build_view: too much pixel data")
    nsets = len(SETS)
    lines = []
    lines.append("// GENERATED by tools/build_view.py from %s. Do not edit." % ", ".join(
        os.path.relpath(p, PROJ).replace("\\", "/") for p in sorted(set(used.values()))))
    lines.append('#include "dl_view_data.h"\n')
    lines.append("namespace dl::view_data\n{")
    lines.append("    alignas(4) const uint16_t pixels[%d] = {" % max(1, len(pix)))
    for i in range(0, len(pix), 24):
        lines.append("        " + ",".join(str(v) for v in pix[i:i + 24]) + ",")
    lines.append("    };\n")
    lines.append("    const span spans[%d] = {" % len(spans))
    for i in range(0, len(spans), 8):
        lines.append("        " + "".join("{%d,%d,%d,%d}," % s for s in spans[i:i + 8]))
    lines.append("    };\n")

    def pref(key):
        return "{%d,%d,%d,%d}" % ptab.get(key, (0, 0, 0, 0))

    ncols = 2 * geo.max_lateral + 1
    lines.append("    const piece front[set_count][depths][cols] = {")
    for si in range(nsets):
        lines.append("        {" + ",".join("{" + ",".join(pref((si, 0, d, s)) for s in range(ncols)) + "}"
                                            for d in range(geo.depths)) + "},")
    lines.append("    };\n")
    lines.append("    const piece lateral[set_count][depths][lines] = {")
    for si in range(nsets):
        lines.append("        {" + ",".join("{" + ",".join(pref((si, 1, d, s)) for s in range(len(LATERALS))) + "}"
                                            for d in range(geo.depths)) + "},")
    lines.append("    };\n")
    lines.append("    const piece cap[depths][cap_lines] = {")
    lines.append("        " + ",".join("{" + ",".join(pref((-1, 3, d, c)) for c in range(len(CAP_LINES))) + "}"
                                     for d in range(geo.depths)) + ",")
    lines.append("    };\n")
    lines.append("    const piece background[2] = { %s, %s };\n" % (pref((-1, 2, 0, 0)), pref((-1, 2, 0, 1))))
    full = [tuple(c) for c in pal] + [(0, 0, 0)] * (192 - len(pal))
    ui = Image.open(UI_FRAME).getpalette() if os.path.exists(UI_FRAME) else [0] * 768
    ui += [0] * (768 - len(ui))
    full += [(ui[i * 3] >> 3, ui[i * 3 + 1] >> 3, ui[i * 3 + 2] >> 3) for i in range(192, 256)]
    for k, c in enumerate(MAP_COLORS):          # engine minimap colours in the free UI slots 240..
        full[240 + k] = tuple(v >> 3 for v in c)
    lines.append("    const bn::color palette_colors[256] = {")
    for i in range(0, 256, 8):
        lines.append("        " + "".join("bn::color(%d,%d,%d)," % c for c in full[i:i + 8]))
    lines.append("    };\n}")
    with open(OUT_CPP, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")

    hl = ["// GENERATED by tools/build_view.py. Do not edit.", "#ifndef DL_VIEW_DATA_H", "#define DL_VIEW_DATA_H", "",
          '#include "bn_color.h"', "#include <stdint.h>", "", "namespace dl::view_data", "{",
          "    // a horizontal run of 2-px pairs: page row y, first pair x2 (pixel 2*x2), n2 pairs, data at pixels[off]",
          "    struct span", "    {", "        uint8_t y;", "        uint8_t x2;", "        uint16_t n2;",
          "        uint32_t off;", "    };", "", "    struct piece", "    {", "        uint16_t first;",
          "        uint16_t count;", "        uint8_t c0;         // covered 2-px columns [c0, c1)", "        uint8_t c1;", "    };", "", "    enum set_id : int", "    {"]
    for name, _s, _k, _o, _q in SETS:
        hl.append("        set_%s," % name)
    hl += ["        set_count", "    };", "",
           "    // opaque sets hide whatever is behind them (wall decorations behind them are not shown)",
           "    constexpr bool set_opaque[set_count] = { %s };" % ", ".join(
               "true" if q else "false" for _n, _s, _k, _o, q in SETS),
           "    constexpr int cap_lines = %d;  // free wall end side faces on lines %s" % (
               len(CAP_LINES), ", ".join(str(v) for v in CAP_LINES)),
           "    constexpr int depths = %d;" % geo.depths,
           "    constexpr int cols = %d;      // x = -%d..%d" % (2 * geo.max_lateral + 1, geo.max_lateral, geo.max_lateral),
           "    constexpr int lines = %d;     // lateral lines X = (2 * i - %d) / 2" % (len(LATERALS), len(LATERALS) - 1),
           "    constexpr int ui_first_color = 192;",
           "    constexpr int palette_used = %d;    // environment colours actually used (1..)" % (len(pal) - 1),
           "", "    extern const uint16_t pixels[];", "    extern const span spans[];",
           "    extern const piece front[set_count][depths][cols];   // [set][d][x + max lateral]",
           "    extern const piece lateral[set_count][depths][lines]; // [set][d][line]",
           "    extern const piece cap[depths][cap_lines];       // [d][cap line]",
           "    extern const piece background[2];               // [player cell parity]",
           "    constexpr int wall_variants = 4;               // set_wall .. set_wall + 3", "    extern const bn::color palette_colors[256];", "}", "", "#endif"]
    with open(OUT_H, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(hl) + "\n")
    print("build_view: %d pieces, %d spans, %d KB pixels, %d environment colours, textures: %s" % (
        len(pieces), len(spans), len(pix) * 2 // 1024, len(pal) - 1,
        ", ".join(sorted(os.path.relpath(p, PROJ).replace("\\", "/") for p in used.values()))))


if __name__ == "__main__":
    main()
