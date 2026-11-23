"""Shared kit for DELVE dungeon themes (G-36). Each art/themes/<theme>/seed.py defines a palette and a painter and
calls build(); this file does the common part, the same way art/_seed/sewer_art.py + seed_all.py do for the sewer:

  - door, open door and locked door from the theme wall (sewer_art.door_set: exact legacy opening masks)
  - arch, bars, stairs and the legacy wall overlays (crack, dart, skulls, banner, chains, lever, torch) re-coloured
    onto the theme wall by material (sewer_art.remap), with the same index-0 holes the engine reads
  - seed PNGs in <theme>/_out/, art/themes/<theme>/tex_*.aseprite created ONLY when missing (never overwritten),
    every tex_*.aseprite exported to <theme>/export/tex_*.bmp (same names and sizes as art/export/)
  - deco_*: the sprites are the same drawings; only their 16-colour palettes change per theme. The theme palettes
    go to <theme>/deco_palette.json and recoloured copies of graphics/deco_*.bmp to <theme>/export/deco_*.bmp.

  python -I seed.py              seeds + missing .aseprite + export
  python -I seed.py export-only  export only (after editing the .aseprite files)
  python -I seed.py reseed       DELETE this theme's tex_*.aseprite and rebuild them (only if nobody edited them)
"""
import json
import os
import subprocess
import sys

import numpy as np
from PIL import Image

THEMES = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(THEMES)
SEED = os.path.join(ART, "_seed")
GFX = os.path.join(os.path.dirname(ART), "graphics")
sys.path.insert(0, SEED)
import sewer_art as SA  # noqa: E402
import sewer_paint as P  # noqa: E402

ASE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
DITH = SA.DITH
TEXTURES = ["tex_wall_stone", "tex_wall_stone_v1", "tex_wall_stone_v2", "tex_wall_stone_v3", "tex_floor",
            "tex_ceiling", "tex_door", "tex_door_open", "tex_door_locked", "tex_arch", "tex_bars",
            "tex_stairs_down", "tex_wall_crack", "tex_wall_dart", "tex_wall_skulls", "tex_wall_banner",
            "tex_wall_chains", "tex_wall_lever_up", "tex_wall_lever_down", "tex_wall_torch"]
LEGACY = ("tex_arch", "tex_bars", "tex_stairs_down", "tex_wall_crack", "tex_wall_dart", "tex_wall_skulls",
          "tex_wall_banner", "tex_wall_chains", "tex_wall_lever_up", "tex_wall_lever_down", "tex_wall_torch")
REQUIRED = ("stone", "stain", "wood", "iron", "rust", "flame", "bone", "cloth")
DECO = ["deco_%s_%s" % (n, v) for n in ("torch", "banner", "chains", "lever") for v in ("front", "left", "right")]
YY, XX = np.mgrid[0:64, 0:64]


# ------------------------------------------------------------------------------------------------ helpers
def palette(ramps):
    """ramps: list of (name, colours). Must contain REQUIRED (sewer_art.remap / door_set read them by name)."""
    pal = P.Pal()
    for name, cols in ramps:
        pal.add(name, cols)
    for r in REQUIRED:
        assert r in pal.ramps, r
    assert len(pal.cols) <= 60, len(pal.cols)
    return pal


def idx(pal, ramp, V, hi=0.8, dith=DITH):
    """Values 0..hi -> indices of a ramp with ordered dither."""
    n = len(pal.ramps[ramp])
    return pal.idx(ramp, P.quant(np.clip(V / hi, 0, 1), n, dith))


def torch_fall(V, k=0.45, off=6):
    """Carried-torch light: the top and bottom of a wall sink into darkness (same curve as the sewer)."""
    return V * ((1 - k) + k * np.sin(np.pi * (YY + off) / 76.0))


def ellipse(cx, cy, rx, ry):
    return ((XX + 0.5 - cx) / rx) ** 2 + ((YY + 0.5 - cy) / ry) ** 2 <= 1.0


def poly(pts):
    """Filled polygon mask (even-odd) on the 64x64 grid."""
    x, y = XX + 0.5, YY + 0.5
    inside = np.zeros((64, 64), bool)
    n = len(pts)
    for i in range(n):
        (x0, y0), (x1, y1) = pts[i], pts[(i + 1) % n]
        cond = (y0 > y) != (y1 > y)
        xi = (x1 - x0) * (y - y0) / ((y1 - y0) + 1e-9) + x0
        inside ^= cond & (x < xi)
    return inside


def edge(mask, dy, dx):
    """Pixels of mask whose neighbour at (dy, dx) is outside the mask."""
    return mask & ~np.roll(np.roll(mask, dy, 0), dx, 1)


def crystal(pal, out, ramp, base, tip, w, rng, lit=0.75):
    """A faceted crystal prism from base to tip (x, y), w = half width: lit left facet, dark right facet,
    a bright ridge and a dark rim."""
    bx, by = base
    tx, ty = tip
    ax, ay = tx - bx, ty - by
    L = np.hypot(ax, ay)
    nx, ny = -ay / L, ax / L
    pts = [(bx + nx * w, by + ny * w), (bx + ax * 0.78 + nx * w * 0.8, by + ay * 0.78 + ny * w * 0.8), (tx, ty),
           (bx + ax * 0.78 - nx * w * 0.8, by + ay * 0.78 - ny * w * 0.8), (bx - nx * w, by - ny * w)]
    m = poly(pts)
    s = ((XX + 0.5 - bx) * nx + (YY + 0.5 - by) * ny) / w           # -1 .. 1 across the prism
    t = ((XX + 0.5 - bx) * ax + (YY + 0.5 - by) * ay) / (L * L)       # 0 base .. 1 tip
    V = np.where(s > 0, lit * (0.7 + 0.3 * t), lit * 0.35 * (0.6 + 0.4 * t))
    V = V + 0.25 * (np.abs(s) < 0.18) + 0.1 * (rng.random((64, 64)) - 0.5)
    r = pal.ramps[ramp]
    out[m] = idx(pal, ramp, V, 1.0, 0.5)[m]
    rim = m & ~(np.roll(m, 1, 0) & np.roll(m, -1, 0) & np.roll(m, 1, 1) & np.roll(m, -1, 1))
    out[rim & (s < 0)] = r[0]
    return m


def blocks(rng, pal, lay, ramp="stone", hi=0.8, light=0.45, **kw):
    """Coursed blocks shaded like the sewer (sewer_paint.courses + stone_shade); returns (V, M, ids)."""
    ids, M = P.courses(rng, lay["heights"], lay["w"][0], lay["w"][1], joint2=lay.get("joint2", 0.2),
                       jitter=lay.get("jitter", 0))
    V = P.stone_shade(rng, ids, M, base=lay["base"], **kw)
    if light:
        V = torch_fall(V, light)
    return V, M, ids


# ------------------------------------------------------------------------------------------------ build
def save_idx(path, arr, pal):
    cols = pal.cols + [(0, 0, 0)] * (64 - len(pal.cols))
    SA.save_idx(path, arr, cols)


def ase(args):
    subprocess.run([ASE, "-b"] + args, check=True, stdout=subprocess.DEVNULL)


def deco_recolour(tdir, deco):
    """deco = dict(A=16 colours, B=16 colours, G=16 colours). Same pixels as graphics/deco_*.bmp."""
    with open(os.path.join(tdir, "deco_palette.json"), "w", encoding="utf-8") as f:
        json.dump({k: ["#%02x%02x%02x" % tuple(c) for c in v] for k, v in deco.items()}, f, indent=1)
    for name in DECO + ["deco_torch_glow"]:
        im = Image.open(os.path.join(GFX, name + ".bmp"))
        key = "G" if name.endswith("glow") else "B" if "banner" in name else "A"
        flat = [c for col in deco[key] for c in col]
        im.putpalette(flat + [0] * (768 - len(flat)))
        im.save(os.path.join(tdir, "export", name + ".bmp"))


def deco_from(pal, flame=None, cloth=None, trim=None, glow=None, warm=(208, 162, 106)):
    """Theme deco palettes in the slot layout of art/_seed/deco_art.py (A: 1-3 wall shadow, 4-8 iron, 9-11 wood,
    12-15 flame; B: 9-12 cloth, 13-15 trim; G: 1-7 glow)."""
    c = pal.cols
    st, ir, wd = pal.ramps["stone"], pal.ramps["iron"], pal.ramps["wood"]
    shadow = [c[st[0]], c[st[1]], c[st[2]]]
    iron = [c[ir[0]], c[ir[1]], c[ir[2]], c[ir[4]], warm]
    wood = [c[wd[0]], c[wd[2]], c[wd[4]]]
    flame = flame or [(168, 54, 12), (238, 122, 28), (255, 196, 74), (255, 243, 192)]
    cloth = cloth or [c[i] for i in pal.ramps["cloth"]] + [c[pal.ramps["cloth"][-1]]]
    trim = trim or [c[ir[1]], c[ir[3]], c[ir[4]]]
    glow = glow or [(128, 96, 62), (150, 106, 58), (176, 118, 54), (204, 136, 52), (228, 158, 62), (246, 186, 86),
                    (255, 214, 126)]
    A = [(255, 0, 255)] + shadow + iron + wood + flame
    B = [(255, 0, 255)] + shadow + iron + cloth[:4] + trim[:3]
    G = [(255, 0, 255)] + glow + [(0, 0, 0)] * 8
    return dict(A=A, B=B, G=G)


def build(tdir, pal, paint, deco, seed=36):
    """paint(rng, pal) -> dict(wall, v1, v2, v3, floor, ceil) of 64x64 index arrays (indices into pal)."""
    out = os.path.join(tdir, "_out")
    exp = os.path.join(tdir, "export")
    os.makedirs(out, exist_ok=True)
    os.makedirs(exp, exist_ok=True)
    if "reseed" in sys.argv:                  # the artist's own regeneration: deletes this theme's .aseprite
        for stem in TEXTURES:
            if os.path.exists(os.path.join(tdir, stem + ".aseprite")):
                os.remove(os.path.join(tdir, stem + ".aseprite"))
    if "export-only" not in sys.argv:
        rng = np.random.default_rng(seed)
        t = paint(rng, pal)
        wall = t["wall"]
        tex = {"tex_wall_stone": wall, "tex_wall_stone_v1": t["v1"], "tex_wall_stone_v2": t["v2"],
               "tex_wall_stone_v3": t["v3"], "tex_floor": t["floor"], "tex_ceiling": t["ceil"]}
        tex["tex_door"], tex["tex_door_open"], tex["tex_door_locked"] = SA.door_set(rng, pal, wall)
        old_wall = SA.legacy("tex_wall_stone")
        for stem in LEGACY:
            old = SA.legacy(stem)
            frames = [SA.remap(old[k:k + 64], old_wall, wall, pal, rng) for k in range(0, old.shape[0], 64)]
            if stem in ("tex_arch", "tex_stairs_down"):
                for f, o in zip(frames, range(0, old.shape[0], 64)):
                    SA.arch_ring(rng, pal, f, old[o:o + 64] == 0)
            tex[stem] = np.concatenate(frames, axis=0)
        if "post" in t:
            t["post"](tex)
        for stem, arr in tex.items():
            save_idx(os.path.join(out, stem + ".png"), arr, pal)
        used = sorted(set(np.unique(np.concatenate([a.ravel() for a in tex.values()]))))
        print(os.path.basename(tdir), "palette", len(pal.cols), "used", len(used))
        for stem in TEXTURES:
            dst = os.path.join(tdir, stem + ".aseprite")
            if os.path.exists(dst):
                continue
            bg = "0" if stem in ("tex_bars", "tex_arch") else "1"
            tags = "flame:1-2:150" if stem == "tex_wall_torch" else ""
            ase(["--script-param", "in=" + os.path.join(out, stem + ".png"), "--script-param", "out=" + dst,
                 "--script-param", "fh=64", "--script-param", "tags=" + tags, "--script-param", "bg=" + bg,
                 "--script-param", "ncol=64", "--script", os.path.join(SEED, "build_aseprite.lua")])
    for stem in TEXTURES:
        ase([os.path.join(tdir, stem + ".aseprite"), "--sheet-type", "vertical", "--sheet",
             os.path.join(exp, stem + ".bmp")])
    deco_recolour(tdir, deco)
    print("exported", os.path.basename(tdir))
