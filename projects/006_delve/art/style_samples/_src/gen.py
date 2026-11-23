"""G-33: build the 3 dark-retro samples, run (a copy of) build_view.py on them and compose 240x160 mockups."""
import importlib
import os
import shutil
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import paint as P  # noqa: E402

REPO = r"C:\Users\David\Desktop\GBA"
DL = os.path.join(REPO, "projects", "006_delve")
OUTROOT = os.path.join(DL, "art", "style_samples")
MOCK_SRC = os.path.join(DL, "art", "hud_mockups", "_src")
FLAME = [(58, 12, 8), (142, 38, 10), (214, 96, 22), (248, 172, 58), (255, 236, 176)]

VARIANTS = {
    "a_cold_dungeon": dict(
        stone=((7, 8, 12), (122, 128, 142), (46, 50, 62)), n_stone=10,
        stain=((24, 20, 18), (98, 80, 64)), wood=((14, 11, 10), (84, 68, 54)), iron=((12, 13, 17), (104, 108, 118)),
        rust=((40, 22, 14), (110, 58, 30)),
        bone=((18, 19, 23), (168, 168, 156), (80, 82, 84)), sk_iron=((30, 32, 38), (120, 124, 132)),
        cloth=((18, 20, 28), (62, 66, 82)), eye=(130, 210, 235),
        wall=dict(heights=(16, 15, 17, 16), w=(18, 30), base=(0.34, 0.56), jitter=0, joint2=0.3),
        floor=dict(heights=(21, 22, 21), w=(20, 34), base=(0.26, 0.42)),
        ceil=dict(heights=(32, 32), w=(28, 40), base=(0.14, 0.26)),
        stain_n=9, damp=0.35, slime=False, seed=11),
    "b_wet_sewer": dict(
        stone=((5, 8, 7), (92, 106, 88), (32, 44, 38)), n_stone=10,
        stain=((14, 26, 12), (104, 132, 50)), wood=((11, 11, 8), (72, 66, 44)), iron=((10, 13, 11), (86, 96, 84)),
        rust=((34, 26, 10), (100, 74, 26)),
        bone=((18, 20, 14), (160, 158, 122), (78, 82, 60)), sk_iron=((22, 28, 20), (98, 110, 90)),
        cloth=((14, 20, 12), (58, 72, 40)), eye=(160, 230, 80),
        wall=dict(heights=(11, 10, 11, 10, 11, 11), w=(12, 20), base=(0.32, 0.52), jitter=0, joint2=0.2),
        floor=dict(heights=(16, 16, 16, 16), w=(14, 22), base=(0.22, 0.38)),
        ceil=dict(heights=(22, 21, 21), w=(18, 30), base=(0.13, 0.25)),
        stain_n=16, damp=0.75, slime=True, seed=23),
    "c_ancient_crypt": dict(
        stone=((9, 7, 7), (128, 116, 100), (54, 46, 40)), n_stone=10,
        stain=((26, 18, 11), (112, 88, 58)), wood=((13, 9, 8), (92, 66, 46)), iron=((13, 11, 11), (102, 94, 90)),
        rust=((44, 20, 10), (120, 60, 28)),
        bone=((22, 17, 14), (174, 158, 128), (86, 72, 58)), sk_iron=((34, 28, 24), (126, 116, 104)),
        cloth=((26, 13, 11), (90, 40, 30)), eye=(236, 96, 40),
        wall=dict(heights=(24, 18, 22), w=(16, 38), base=(0.30, 0.50), jitter=1, joint2=0.45),
        floor=dict(heights=(30, 34), w=(22, 40), base=(0.24, 0.44)),
        ceil=dict(heights=(30, 34), w=(30, 44), base=(0.13, 0.24)),
        stain_n=7, damp=0.25, slime=False, seed=37),
}


def build_palette(v):
    pal = P.Pal()
    a, b, m = v["stone"]
    pal.add("stone", P.ramp(a, b, v["n_stone"], m))
    pal.add("stain", P.ramp(*v["stain"], 5))
    pal.add("wood", P.ramp(*v["wood"], 6))
    pal.add("iron", P.ramp(*v["iron"], 5))
    pal.add("rust", P.ramp(*v["rust"], 3))
    pal.add("flame", FLAME)
    return pal


def surface(rng, v, pal, lay, kind):
    ids, M = P.courses(rng, lay["heights"], lay["w"][0], lay["w"][1], jitter=lay.get("jitter", 0),
                       joint2=lay.get("joint2", 0.25))
    flat = kind != "wall"
    V = P.stone_shade(rng, ids, M, base=lay["base"], bevel=0.6 if flat else 1.0,
                      cracks=4 if kind != "ceil" else 2, chips=6 if "crypt" in v["name"] else 3,
                      pits=16 if not flat else 10)
    h, w = V.shape
    S = np.zeros((h, w))
    if kind == "wall":
        S = np.maximum(S, P.drips(rng, M, v["stain_n"], 5, 26))
        yy = np.arange(h)[:, None] / h
        S = np.maximum(S, np.clip((yy - (1 - v["damp"])) / v["damp"], 0, 1) * P.fbm(rng, h, w) * 1.3)
    elif kind == "floor":
        S = np.clip((P.fbm(rng, h, w) - 0.55) * 2.6, 0, 1) * (1.0 if v["slime"] else 0.6)
    else:
        S = np.clip((P.fbm(rng, h, w) - 0.5) * 2.0, 0, 1) * 0.8          # soot
    V = V * (1 - 0.40 * S)
    if kind == "wall":      # carried-torch light: the top and bottom of the wall sink into darkness
        V = V * (0.55 + 0.45 * np.sin(np.pi * (np.arange(h)[:, None] + 6) / 76.0))
    n = len(pal.ramps["stone"])
    lv = P.quant(np.clip(V / 0.80, 0, 1), n, 0.9)
    out = pal.idx("stone", lv)
    ns = len(pal.ramps["stain"])
    if kind != "ceil":
        st = (S > 0.55) & ~M if kind == "wall" else (S > 0.45) & ~M
        sl = P.quant(np.clip(V / 0.7, 0, 1), ns, 0.9)
        out[st] = pal.idx("stain", sl)[st]
    if kind == "floor" and v["slime"]:          # standing water: dark with a cold glint line
        water = (P.fbm(rng, h, w) > 0.62)
        out[water] = pal.ramps["stone"][1]
        glint = water & ~np.roll(water, 1, 0) & (np.arange(w)[None, :] % 3 != 0)
        out[glint] = pal.ramps["stone"][5]
    out[M] = np.where(V[M] < 0.05, pal.ramps["stone"][0], pal.ramps["stone"][1])
    return out, ids, M


def door_tex(rng, v, pal, wall_idx):
    hole = np.array(Image.open(os.path.join(DL, "art", "export", "tex_door_open.bmp")))[:64] == 0
    out = wall_idx.copy()
    # dressed arch stones around the opening, recessed shadow ring next to it
    ys, xs = np.nonzero(hole)
    cy, cx = ys.min() + 14, (xs.min() + xs.max()) / 2.0
    dist = np.full(hole.shape, 99)
    for k in range(5, 0, -1):
        grown = hole.copy()
        for _ in range(k):
            g = grown.copy()
            g[1:] |= grown[:-1]
            g[:-1] |= grown[1:]
            g[:, 1:] |= grown[:, :-1]
            g[:, :-1] |= grown[:, 1:]
            grown = g
        dist[grown & ~hole] = k
    yy, xx = np.mgrid[0:64, 0:64]
    ang = np.arctan2(yy - cy, xx - cx)
    seg = np.floor((ang + np.pi) / (np.pi / 9)).astype(int)
    ring = (dist <= 4) & ~hole
    n = len(pal.ramps["stone"])
    base = 0.40 + 0.10 * ((seg * 37) % 5) / 4.0
    V = base + 0.25 * (P.fbm(rng, 64, 64) - 0.5) - 0.06 * (dist - 1) + 0.10 * (yy < cy)
    V[ring & (seg != np.roll(seg, 1, 1))] = 0.05
    V[dist == 1] = 0.06                       # shadowed reveal
    lv = P.quant(np.clip(V / 0.8, 0, 1), n, 0.9)
    out[ring] = pal.idx("stone", lv)[ring]
    P.door_leaf(rng, pal, out, hole, None)
    return out


def save_idx(path, idx, pal_flat, transparent0=False):
    im = Image.fromarray(idx.astype(np.uint8), "P")
    im.putpalette(pal_flat)
    if transparent0 and path.endswith(".png"):
        im.save(path, transparency=0)
    else:
        im.save(path)


def main():
    os.environ.setdefault("DL_PROJ", os.path.join(HERE, "mirror"))
    for name, v in VARIANTS.items():
        v["name"] = name
        rng = np.random.default_rng(v["seed"])
        pal = build_palette(v)
        out = os.path.join(OUTROOT, name)
        os.makedirs(out, exist_ok=True)
        wall, ids, M = surface(rng, v, pal, v["wall"], "wall")
        floor, _, _ = surface(rng, v, pal, v["floor"], "floor")
        ceil, _, _ = surface(rng, v, pal, v["ceil"], "ceil")
        door = door_tex(rng, v, pal, wall)
        torch = P.torch_sprite(rng, pal)
        torch_wall = wall.copy()
        tm = torch > 0
        torch_wall[:, 16:48][tm] = torch[tm]
        sk, skcols = P.skeleton(rng, v["eye"], P.ramp(*v["bone"][:2], 8, v["bone"][2]),
                                P.ramp(*v["sk_iron"], 3), P.ramp(*v["cloth"], 3))
        flat = pal.flat()
        used = sorted(set(np.unique(np.concatenate([a.ravel() for a in (wall, floor, ceil, door, torch)]))) - {0})
        print(name, "view palette entries:", len(pal.cols) - 1, "used by textures:", len(used))
        for stem, arr in (("wall", wall), ("floor", floor), ("ceiling", ceil), ("door", door)):
            save_idx(os.path.join(out, "tex_%s.png" % stem), arr, flat)
        save_idx(os.path.join(out, "deco_torch_front.png"), torch, flat, True)
        skflat = [c for col in skcols for c in col] + [0] * (768 - 3 * len(skcols))
        save_idx(os.path.join(out, "mon_skeleton.png"), sk, skflat, True)
        sw = np.repeat(np.repeat(np.arange(len(pal.cols))[None, :], 8, 0), 8, 1)
        save_idx(os.path.join(out, "palette.png"), sw, flat)
        # mirror project for the scratch copy of build_view.py
        mir = os.path.join(HERE, "mirror_" + name)
        for d in ("include", "src", "graphics", os.path.join("art", "export")):
            os.makedirs(os.path.join(mir, d), exist_ok=True)
        shutil.copy(os.path.join(DL, "include", "dl_view_geometry.h"), os.path.join(mir, "include"))
        shutil.copy(os.path.join(DL, "graphics", "ui_frame.bmp"), os.path.join(mir, "graphics"))
        ex = os.path.join(mir, "art", "export")
        door_open = door.copy()
        door_open[np.array(Image.open(os.path.join(DL, "art", "export", "tex_door_open.bmp")))[:64] == 0] = 0
        stems = {"tex_wall_stone": wall, "tex_floor": floor, "tex_ceiling": ceil, "tex_door": door,
                 "tex_door_open": door_open, "tex_wall_torch": torch_wall}
        for s in ("tex_wall_skulls", "tex_wall_crack", "tex_wall_dart", "tex_arch", "tex_bars", "tex_stairs_down"):
            stems[s] = wall
        for s, arr in stems.items():
            save_idx(os.path.join(ex, s + ".bmp"), arr, flat)
        v["mirror"] = mir
        v["sk"] = (sk, skcols)
    return VARIANTS


if __name__ == "__main__":
    main()
