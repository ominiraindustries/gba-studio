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

# piece sets: (enum name, texture file stem, frame index in a vertical strip)
SETS = [
    ("wall", "tex_wall_stone", 0),
    ("wall_torch", "tex_wall_torch", 0),
    ("wall_banner", "tex_wall_banner", 0),
    ("wall_chains", "tex_wall_chains", 0),
    ("wall_skulls", "tex_wall_skulls", 0),
    ("wall_crack", "tex_wall_crack", 0),
    ("wall_dart", "tex_wall_dart", 0),
    ("wall_lever_up", "tex_wall_lever_up", 0),
    ("wall_lever_down", "tex_wall_lever_down", 0),
    ("door_closed", "tex_door", 0),
    ("door_open", "tex_door_open", 0),
    ("arch", "tex_arch", 0),
    ("bars", "tex_bars", 0),
    ("stairs", "tex_stairs_down", 0),
]
# indexed textures where index 0 is the see-through hole (art/LÉEME.md); in the rest index 0 is real black
SEE_THROUGH = {"tex_bars", "tex_arch"}
ART_EXPORT = os.path.join(PROJ, "art", "export")
UI_FRAME = os.path.join(GFX, "ui_frame.bmp")
# minimap colours (palette 240..247; ui_frame uses 192-209): floor, wall, player, door, bars, stairs, lever, dark
MAP_COLORS = [(64, 58, 50), (206, 194, 160), (250, 226, 90), (176, 112, 52), (184, 184, 196), (90, 200, 230),
              (240, 200, 60), (14, 12, 18)]
LATERALS = [-5, -3, -1, 1, 3, 5]
# ----------------------------------------------------------------------------------------------- geometry
def parse_geometry():
    with open(GEO_H, encoding="utf-8") as f:
        text = f.read()
    g = {m.group(1): int(m.group(2)) for m in re.finditer(r"constexpr int (\w+) = (-?\d+);", text)}
    for k in ("view_w", "view_h", "vp_x", "vp_y", "proj_num", "eye_back_z10", "fog_z10"):
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
    return np.clip(1.0 - (z - 1.2) * 0.24, 0.18, 1.0)


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
            any_px = True
    if not any_px:
        return None
    ys, xs = np.nonzero(img[..., 3] > 0)
    if len(xs) == 0:
        return None
    bx0, bx1 = xs.min() & ~1, (xs.max() + 2) & ~1
    by0, by1 = ys.min(), ys.max() + 1
    return bx0, by0, img[by0:by1, bx0:bx1]


def project_floor_ceiling(geo, floor, ceil):
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
    used = {}
    texcache = {}

    def tex(stem, k):
        if stem not in texcache:
            texcache[stem], used[stem] = load_texture(stem)
        return frame(texcache[stem], k)

    pieces = []          # (x0, y0, rgba)
    index = []           # (set, kind, d, slot) per piece
    for si, (name, stem, k) in enumerate(SETS):
        t = tex(stem, k)
        for d in range(geo.depths):
            for x in range(-2, 3):
                r = project_front(geo, t, d, x)
                if r:
                    pieces.append(r)
                    index.append((si, 0, d, x + 2))
            for li, l in enumerate(LATERALS):
                r = project_lateral(geo, t, d, l)
                if r:
                    pieces.append(r)
                    index.append((si, 1, d, li))
    bg = project_floor_ceiling(geo, tex("tex_floor", 0), tex("tex_ceiling", 0))
    pieces.append(bg)
    index.append((-1, 2, 0, 0))
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
        ptab[(si, kind, d, slot)] = (first, len(spans) - first)
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
        f, n = ptab.get(key, (0, 0))
        return "{%d,%d}" % (f, n)

    lines.append("    const piece front[set_count][depths][5] = {")
    for si in range(nsets):
        lines.append("        {" + ",".join("{" + ",".join(pref((si, 0, d, s)) for s in range(5)) + "}"
                                            for d in range(geo.depths)) + "},")
    lines.append("    };\n")
    lines.append("    const piece lateral[set_count][depths][6] = {")
    for si in range(nsets):
        lines.append("        {" + ",".join("{" + ",".join(pref((si, 1, d, s)) for s in range(6)) + "}"
                                            for d in range(geo.depths)) + "},")
    lines.append("    };\n")
    lines.append("    const piece background = %s;\n" % pref((-1, 2, 0, 0)))
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
          "        uint16_t count;", "    };", "", "    enum set_id : int", "    {"]
    for name, _s, _k in SETS:
        hl.append("        set_%s," % name)
    hl += ["        set_count", "    };", "",
           "    constexpr int depths = %d;" % geo.depths,
           "    constexpr int ui_first_color = 192;",
           "    constexpr int palette_used = %d;    // environment colours actually used (1..)" % (len(pal) - 1),
           "", "    extern const uint16_t pixels[];", "    extern const span spans[];",
           "    extern const piece front[set_count][depths][5];      // [set][d][x + 2]",
           "    extern const piece lateral[set_count][depths][6];    // [set][d][line]: X = -2.5,-1.5,-0.5,0.5,1.5,2.5",
           "    extern const piece background;", "    extern const bn::color palette_colors[256];", "}", "", "#endif"]
    with open(OUT_H, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(hl) + "\n")
    print("build_view: %d pieces, %d spans, %d KB pixels, %d environment colours, textures: %s" % (
        len(pieces), len(spans), len(pix) * 2 // 1024, len(pal) - 1,
        ", ".join(sorted(os.path.relpath(p, PROJ).replace("\\", "/") for p in used.values()))))


if __name__ == "__main__":
    main()
