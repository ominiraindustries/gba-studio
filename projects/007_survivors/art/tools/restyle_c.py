"""Style C remake ("limited palette with glow", G-44): rebuild art/*.aseprite from art/old_style/*.aseprite.

Keeps every export name, frame size, tag and frame order; only colours (plus a 1 px rim) change, and the ground,
props and title are redrawn in the new style. New sprites: fx_nova, fx_nova_spark, fx_blink, wizard_ghost, hud_nova.

    python -I art/tools/restyle_c.py            # rewrite art/*.aseprite (reads only art/old_style/)
    python -I art/tools/restyle_c.py text       # graphics/text_*.bmp palettes only
Then: python -I art/tools/build_art.py export
"""
import colorsys
import glob
import json
import os
import random
import subprocess
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import aseprite_io  # noqa: E402

ART = os.path.dirname(HERE)
OLD = os.path.join(ART, "old_style")
GFX = os.path.join(os.path.dirname(ART), "graphics")
RAWC = os.path.join(ART, "raw", "style_samples")
TMP = os.path.join(ART, "raw", "_export", "old")
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(ART)))
ASEPRITE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def hexc(h):
    return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))


def H(*cs):
    return [hexc(c) for c in cs]


# ------------------------------------------------------------------ style C palettes
BG = "#07060c"
RIM_ENEMY = hexc("#3d5590")      # cold 1 px rim on every enemy (legibility on the near-black ground)
RIM_WIZARD = hexc("#b4a4f4")     # Wendel: light lilac rim, brighter than any enemy

# Fixed-index palettes: old index -> new colour (semantics kept, see LEEME)
WIZ = {2: "#2e1c5c", 3: "#5a3aa8", 4: "#8c64e0", 5: "#b898ff", 6: "#e8dcff", 7: "#9c6a78", 8: "#e2b4a8",
       9: "#dce0f4", 10: "#8890b8", 11: "#241a30", 12: "#4c3c5c", 13: "#60f0ff", 14: "#ff5ce8", 15: "#a02090"}
UI = {1: "#05050a", 2: "#14162c", 3: "#b070ff", 4: "#1c2440", 5: "#34466e", 6: "#5a74a8", 7: "#1aa8d0",
      8: "#80f4ff", 9: "#181c36", 10: "#22284a", 11: "#7a1478", 12: "#c81c28", 13: "#ffa08c", 14: "#ff3050",
      15: "#f4f0ff"}
PICK = {1: "#06060c", 2: "#2a7a20", 3: "#7ee830", 4: "#d8ff90", 5: "#1060a0", 6: "#20d0ff", 7: "#b0ffff",
        8: "#80104c", 9: "#ff3078", 10: "#ffb0d8", 11: "#7a5410", 12: "#f0c030", 13: "#fff0a0", 14: "#ffffff",
        15: "#3a2840"}
FX = {1: "#120828", 2: "#ffffff", 3: "#ffd8ff", 4: "#ff8af0", 5: "#ff30c8", 6: "#ff2840", 7: "#e0ffff",
      8: "#80f8ff", 9: "#30c8ff", 10: "#1060d0", 11: "#f0d0ff", 12: "#c070ff", 13: "#8040e0", 14: "#3a1470",
      15: "#02020a"}

INDEX_MAPS = {"wizard": WIZ, "gems": PICK, "coin": PICK, "chest": PICK, "heart": PICK, "icons": PICK,
              "hud_slots": PICK}
for n in ("card", "hud_alert", "hud_arrow", "hud_heart", "hud_hp", "hud_plaque", "hud_xp", "hud_xp_cap",
          "fx_levelup", "logo"):
    INDEX_MAPS[n] = UI
for n in ("fx_aura", "fx_frost", "fx_impact", "fx_poof", "fx_rune", "fx_spark", "shadow_big", "shadow_small"):
    INDEX_MAPS[n] = FX


def hsv(c):
    h, s, v = colorsys.rgb_to_hsv(*(x / 255 for x in c))
    return h * 360, s, v


def lum(c):
    return (int(c[0]) * 3 + int(c[1]) * 6 + int(c[2])) / 2550


# Class rules for median-cut palettes: (predicate(h, s, v, L), ramp dark->light). First match wins.
def r_dark(t):
    return lambda h, s, v, L: v < t


CLASS_RECIPES = {
    "slime": [
        (r_dark(0.2), H("#04060a")),
        (lambda h, s, v, L: s < 0.15 and v > 0.85, H("#d0ff60")),
        (lambda h, s, v, L: (h < 20 or h > 330) and s > 0.6, H("#ff2a88")),
        (lambda h, s, v, L: 30 < h < 60 and s > 0.4, H("#ffd040")),
        (lambda h, s, v, L: 215 < h < 260, H("#0f2228")),
        (lambda h, s, v, L: True, H("#12302c", "#1a4638", "#245e44", "#307a4c", "#4a9a58")),
    ],
    "undead": [
        (lambda h, s, v, L: (h < 15 or h > 340) and s > 0.6, H("#ff2850")),
        (r_dark(0.16), H("#06060c")),
        (lambda h, s, v, L: L > 0.5, H("#6a78a0", "#94a2c4", "#bcc8e4", "#e4ecfc")),
        (lambda h, s, v, L: 250 < h < 310, H("#22183a", "#33245a", "#48347a")),
        (lambda h, s, v, L: True, H("#141424", "#20243a", "#30364e")),
    ],
    "greenskin": [
        (r_dark(0.12), H("#06060c")),
        (lambda h, s, v, L: L > 0.7, H("#e8ff50")),
        (lambda h, s, v, L: 60 < h < 110 and s > 0.3, H("#16263a", "#1e3450", "#284868", "#365e82")),
        (lambda h, s, v, L: True, H("#1a1424", "#2a1e36", "#3c2a4a")),
    ],
    "orc_warchief": [
        (lambda h, s, v, L: (h < 15 or h > 340) and s > 0.6, H("#ff2a3c")),
        (lambda h, s, v, L: L > 0.6, H("#8c96bc", "#b4bedc", "#dce2f6")),
        (lambda h, s, v, L: 45 < h < 75 and s > 0.25, H("#1e3048", "#2c4466")),
        (lambda h, s, v, L: True, H("#0c0a14", "#16121f", "#221c2e", "#302840", "#423856", "#5a4e70")),
    ],
    "shrine": [
        (lambda h, s, v, L: 265 < h < 305 and s > 0.4, H("#40105a", "#8a20b0", "#e040f0", "#ffa8ff")),
        (lambda h, s, v, L: True, H("#0c0e16", "#161a26", "#222838", "#30384c", "#465068")),
    ],
}
GROUPS = {"slime": "slime", "slime_king": "slime", "bat": "undead", "skeleton": "undead",
          "skeleton_big": "undead", "goblin": "greenskin", "orc": "greenskin", "orc_warchief": "orc_warchief",
          "shrine": "shrine"}
RIMS = {n: RIM_ENEMY for n in GROUPS}
RIMS["wizard"] = RIM_WIZARD


# ------------------------------------------------------------------ io
def load_old(name):
    """-> frames [np.uint8 (fh, fw, 4)], palette rgb list (256), index frames, durations, tags."""
    os.makedirs(TMP, exist_ok=True)
    png, data = os.path.join(TMP, name + ".png"), os.path.join(TMP, name + ".json")
    subprocess.run([ASEPRITE, "-b", os.path.join(OLD, name + ".aseprite"), "--sheet-type", "vertical", "--sheet",
                    png, "--data", data, "--format", "json-array", "--list-tags"], check=True, capture_output=True)
    im = Image.open(png)
    assert im.mode == "P", name
    meta = json.load(open(data))
    fr = meta["frames"]
    fw, fh = fr[0]["frame"]["w"], fr[0]["frame"]["h"]
    p = im.getpalette()
    pal = [tuple(p[i:i + 3]) for i in range(0, len(p), 3)]
    idx = np.array(im)
    iframes = [idx[k * fh:(k + 1) * fh, :fw] for k in range(len(fr))]
    tags = [(t["name"], t["from"], t["to"]) for t in meta["meta"].get("frameTags", [])]
    return dict(fw=fw, fh=fh, pal=pal, idx=iframes, dur=[f["duration"] for f in fr], tags=tags)


def to_rgba(iframe, pal):
    out = np.zeros(iframe.shape + (4,), np.uint8)
    m = iframe > 0
    out[m, :3] = np.array(pal, np.uint8)[iframe[m]]
    out[m, 3] = 255
    return out


def save_indexed(name, frames, dur, tags, pal=None, bg0=(0, 0, 0)):
    """frames: RGBA arrays. Builds a <=15 colour palette (dark->light) unless pal is given; writes art/name.aseprite."""
    if pal is None:
        cols = set()
        for f in frames:
            cols |= {tuple(c) for c in f[f[..., 3] > 0][:, :3]}
        cols = sorted(cols, key=lum)
        if len(cols) > 15:
            big = np.concatenate([f[f[..., 3] > 0][:, :3] for f in frames])[None]
            q = Image.fromarray(big, "RGB").quantize(15, method=Image.MEDIANCUT, dither=Image.NONE)
            qp = q.getpalette()[:45]
            cols = sorted({tuple(qp[i:i + 3]) for i in range(0, 45, 3)}, key=lum)
            print(f"  {name}: quantized to {len(cols)}")
        pal = [bg0] + cols
    P = np.array(pal[1:], np.int32)
    out = []
    for f in frames:
        d = ((f[..., None, :3].astype(np.int32) - P[None, None]) ** 2).sum(-1)
        ix = d.argmin(-1) + 1
        ix[f[..., 3] == 0] = 0
        out.append(ix.astype(np.uint8))
    h, w = frames[0].shape[:2]
    aseprite_io.write_aseprite(os.path.join(ART, name + ".aseprite"), w, h, list(pal),
                               [f.tolist() for f in out], dur, tags)
    print(f"{name:14s} {w}x{h} x{len(frames)} colours {len(pal) - 1}")
    return pal


def add_rim(f, col):
    """1 px 4-neighbour rim outside the silhouette (inside the frame)."""
    m = f[..., 3] > 0
    p = np.pad(m, 1)
    n = p[:-2, 1:-1] | p[2:, 1:-1] | p[1:-1, :-2] | p[1:-1, 2:]
    ring = n & ~m
    g = f.copy()
    g[ring, :3] = col
    g[ring, 3] = 255
    return g


# ------------------------------------------------------------------ recolour
def class_map(colors, rules):
    buckets = {}
    for c in colors:
        h, s, v = hsv(c)
        L = lum(c)
        for i, (pred, ramp) in enumerate(rules):
            if pred(h, s, v, L):
                buckets.setdefault(i, []).append(c)
                break
    out = {}
    for i, cs in buckets.items():
        ramp = rules[i][1]
        cs = sorted(cs, key=lum)
        for r, c in enumerate(cs):
            k = 0 if len(cs) == 1 else round(r * (len(ramp) - 1) / (len(cs) - 1))
            if len(cs) == 1:
                k = len(ramp) // 2
            out[c] = ramp[k]
    return out


def restyle_sprites():
    data = {n: load_old(n) for n in [os.path.splitext(os.path.basename(p))[0]
                                      for p in glob.glob(os.path.join(OLD, "*.aseprite"))]}
    # median-cut groups: map colours over the union of the group
    group_maps = {}
    for g, rules in CLASS_RECIPES.items():
        cols = set()
        for n, gg in GROUPS.items():
            if gg == g:
                d = data[n]
                used = set(np.unique(np.concatenate([f.ravel() for f in d["idx"]]))) - {0}
                cols |= {d["pal"][i] for i in used}
        group_maps[g] = class_map(sorted(cols), rules)
    done = {}
    for n, d in sorted(data.items()):
        if n in ("ground", "props", "title_bg"):
            continue
        if n in INDEX_MAPS:
            m = INDEX_MAPS[n]
            pal = [(0, 0, 0)] + [hexc(m[i]) if i in m else d["pal"][i] for i in range(1, 16)]
            frames = [to_rgba(f, pal) for f in d["idx"]]
            if n == "wizard":
                frames = [add_rim(f, RIMS[n]) for f in frames]
                pal_w = list(pal)
                pal_w[1] = RIM_WIZARD
                done[n] = save_indexed(n, frames, d["dur"], d["tags"], pal_w[:16])
                ghost(d)
            else:
                done[n] = save_indexed(n, frames, d["dur"], d["tags"], pal[:16])
        elif n in GROUPS:
            m = group_maps[GROUPS[n]]
            pal = [m.get(c, c) for c in d["pal"]]
            frames = [add_rim(to_rgba(f, pal), RIMS[n]) for f in d["idx"]]
            done[n] = (GROUPS[n], frames, d)
        else:
            print("skip", n)
    # shared palettes per group
    for g in CLASS_RECIPES:
        members = [n for n, v in done.items() if isinstance(v, tuple) and v[0] == g]
        cols = set()
        for n in members:
            for f in done[n][1]:
                cols |= {tuple(c) for c in f[f[..., 3] > 0][:, :3]}
        assert len(cols) <= 15, (g, len(cols))
        pal = [(0, 0, 0)] + sorted(cols, key=lum)
        for n in members:
            save_indexed(n, done[n][1], done[n][2]["dur"], done[n][2]["tags"], pal)
    if os.path.exists(os.path.join(STYLE_C_RAW, "king.png")):
        king_from_static(data["slime_king"])
    return data


STYLE_C_RAW = os.path.join(ART, "raw", "style_c")  # PixelLab picks for the remake (king.png, title_wizard.png)


def squash(im, sx, sy, size):
    """Scale an RGBA sprite (nearest) around its bottom centre into a size x size frame."""
    w, h = max(1, round(im.width * sx)), max(1, round(im.height * sy))
    s = im.resize((w, h), Image.NEAREST)
    f = Image.new("RGBA", (size, size))
    f.alpha_composite(s, ((size - w) // 2, size - h))
    return np.array(f)


def king_from_static(d):
    """slime_king from one PixelLab still: idle 0-3 (breathing), crouch 4, air 5-6, land 7-8."""
    im = Image.open(os.path.join(STYLE_C_RAW, "king.png")).convert("RGBA")
    a = np.array(im)
    a[..., 3] = np.where(a[..., 3] >= 128, 255, 0)
    im = Image.fromarray(a)
    im = im.crop(im.getbbox())
    k = min(62 / im.width, 60 / im.height, 1.0)
    if k < 1:
        im = im.resize((round(im.width * k), round(im.height * k)), Image.NEAREST)
    plan = [(1, 1), (1.03, 0.97), (1.05, 0.94), (1.03, 0.97),   # idle
            (1.12, 0.82),                                       # crouch
            (0.9, 1.1), (0.95, 1.04),                           # air
            (1.16, 0.78), (1.06, 0.92)]                         # land
    frames = []
    for sx, sy in plan:
        sx = min(sx, 63 / im.width)
        frames.append(add_rim(squash(im, sx, sy, 64), RIM_ENEMY))
    save_indexed("slime_king", frames, d["dur"], d["tags"])


def ghost(d):
    """wizard_ghost: same frames/tags as wizard, flat silhouette (for Blink afterimages)."""
    pal = [(0, 0, 0), hexc("#7a5ce0"), hexc("#c8b8ff")]
    frames = []
    for f in d["idx"]:
        m = np.pad(f > 0, 1)
        g = np.zeros(f.shape + (4,), np.uint8)
        g[f > 0] = (122, 92, 224, 255)
        # light rim on the outer edge pixels of the silhouette
        inner = m[:-2, 1:-1] & m[2:, 1:-1] & m[1:-1, :-2] & m[1:-1, 2:]
        e = (f > 0) & ~inner
        g[e] = (200, 184, 255, 255)
        frames.append(g)
    save_indexed("wizard_ghost", frames, d["dur"], d["tags"], pal)


# ------------------------------------------------------------------ new FX (FX / UI palettes)
FX_PAL = [(0, 0, 0)] + [hexc(FX[i]) for i in range(1, 16)]
UI_PAL = [(0, 0, 0)] + [hexc(UI[i]) for i in range(1, 16)]


def canvas(w, h):
    return np.zeros((h, w, 4), np.uint8)


def dot(f, x, y, c):
    if 0 <= x < f.shape[1] and 0 <= y < f.shape[0]:
        f[y, x, :3] = c
        f[y, x, 3] = 255


def ring_frame(size, r, width, cols, dither=0.0, seed=0):
    f = canvas(size, size)
    c = (size - 1) / 2
    ys, xs = np.mgrid[0:size, 0:size]
    d = np.hypot(xs - c, ys - c)
    t = (r - d) / max(width, 0.5)  # 0 at the outer edge, 1 at the inner edge
    rng = np.random.default_rng(seed)
    for k, col in enumerate(cols):  # outer (bright) to inner (dim)
        a, b = k / len(cols), (k + 1) / len(cols)
        m = (t >= a) & (t < b)
        if dither:
            m &= (BAYER[ys % 4, xs % 4] + rng.random((size, size)) * 0.2) > dither
        f[m, :3] = col
        f[m, 3] = 255
    return f


def nova():
    W = [hexc(FX[i]) for i in (2, 3, 4, 5, 12, 13)]
    frames = []
    for k in range(8):
        r = 5 + k * 3.8
        width = [3, 4, 5, 6, 6, 5, 4, 3][k]
        cols = [W[0], W[1], W[2], W[3], W[4]] if k < 3 else [W[2], W[3], W[4], W[5]]
        f = ring_frame(64, r, width, cols, dither=[0, 0, 0, 0, 0.15, 0.3, 0.5, 0.7][k], seed=k)
        if k < 2:  # bright core flash
            core = ring_frame(64, 4 - k * 2 + 2, 4 - k * 2 + 2, [W[0], W[1]])
            f[core[..., 3] > 0] = core[core[..., 3] > 0]
        frames.append(f)
    save_indexed("fx_nova", frames, [50] * 8, [("burst", 0, 7)], FX_PAL)
    sp = []
    star = [(0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)]
    for k in range(4):
        f = canvas(8, 8)
        arm = [3, 2, 1, 0][k]
        for i in range(1, arm + 1):
            for dx, dy in ((i, 0), (-i, 0), (0, i), (0, -i)):
                dot(f, 3 + dx, 3 + dy, hexc(FX[5 if i > 1 else 4]))
        for dx, dy in star[:5 if k < 3 else 1]:
            dot(f, 3 + dx, 3 + dy, hexc(FX[4]))
        dot(f, 3, 3, hexc(FX[2 if k < 3 else 12]))
        sp.append(f)
    save_indexed("fx_nova_spark", sp, [60] * 4, [("twinkle", 0, 3)], FX_PAL)


def blink():
    C = [hexc(FX[i]) for i in (2, 7, 8, 9, 12, 13)]
    frames = []
    for k in range(6):
        f = canvas(32, 32)
        if k < 3:  # implode: ring shrinks, vertical streaks
            g = ring_frame(32, 13 - k * 4, 2, [C[1], C[3]], dither=0.0)
            f[g[..., 3] > 0] = g[g[..., 3] > 0]
        else:  # burst: column of light then sparkles
            g = ring_frame(32, 4 + (k - 3) * 5, 2, [C[2], C[4]], dither=0.2 * (k - 3), seed=k)
            f[g[..., 3] > 0] = g[g[..., 3] > 0]
        hl = [12, 14, 10, 14, 8, 4][k]
        for y in range(16 - hl, 16 + hl):
            w = 1 if abs(y - 16) > hl // 2 else 2
            for x in range(16 - w, 16 + w - 1 + 1):
                dot(f, x - 1 if w == 1 else x - 1, y, C[0] if abs(y - 16) < hl // 3 else C[2])
        rng = random.Random(k)
        for _ in range(3 + k):
            dot(f, rng.randrange(6, 26), rng.randrange(4, 28), C[rng.choice((0, 1, 4))])
        frames.append(f)
    save_indexed("fx_blink", frames, [50] * 6, [("blink", 0, 5)], FX_PAL)


def hud_nova():
    """16x16 hex rune meter: charge 0-4 (fill from the bottom), full 5-6 (glow blink)."""
    hexmask = []
    for y in range(16):
        inset = max(0, abs(y - 7.5) - 3.5)
        hexmask.append([(x >= 1 + inset and x <= 14 - inset) for x in range(16)])
    hexmask = np.array(hexmask)
    edge = hexmask & ~(np.pad(hexmask, 1)[:-2, 1:-1] & np.pad(hexmask, 1)[2:, 1:-1]
                       & np.pad(hexmask, 1)[1:-1, :-2] & np.pad(hexmask, 1)[1:-1, 2:])
    inner = hexmask & ~edge
    frames = []
    for k in range(7):
        f = canvas(16, 16)
        f[hexmask] = (*hexc(UI[1]), 255)
        f[inner] = (*hexc(UI[2]), 255)
        level = min(k, 4)
        top = 14 - level * 3  # rows below 'top' are filled
        for y in range(16):
            for x in range(16):
                if inner[y, x] and (y >= top or k >= 4):
                    f[y, x, :3] = hexc(UI[12] if k < 5 else UI[13])
                    if k >= 4 and (x + y) % 5 == 0:
                        f[y, x, :3] = hexc(UI[13] if k < 6 else UI[15])
        ec = UI[5] if k < 4 else (UI[13] if k != 6 else UI[15])
        f[edge] = (*hexc(ec), 255)
        # rune: small star in the middle
        rc = hexc(UI[15] if k >= 4 else UI[6])
        for dx, dy in ((0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)):
            f[7 + dy, 7 + dx] = (*rc, 255)
            f[7 + dy, 8 + dx] = (*rc, 255)
        frames.append(f)
    save_indexed("hud_nova", frames, [100] * 5 + [150, 150],
                 [("charge", 0, 4), ("full", 5, 6)], UI_PAL)


# ------------------------------------------------------------------ ground (512x512, few unique tiles)
GROUND_PAL = H(BG, BG, "#0b0a15", "#100f1e", "#181c30", "#22304a", "#14162a", "#0e1a1c", "#183030", "#2a3c5a")


def tile_variants(rng, base, speck_cols, n_specks, patch=None):
    t = np.full((8, 8), base, np.uint8)
    if patch is not None:
        for y in range(8):
            for x in range(8):
                if rng.random() < patch[1]:
                    t[y, x] = patch[0]
    for _ in range(n_specks):
        t[rng.randrange(8), rng.randrange(8)] = rng.choice(speck_cols)
    return t


def periodic_noise(n, scale, seed):
    rng = np.random.default_rng(seed)
    g = rng.random((n // scale, n // scale))
    ys, xs = np.mgrid[0:n, 0:n] / scale
    x0, y0 = xs.astype(int), ys.astype(int)
    fx, fy = xs - x0, ys - y0
    fx, fy = fx * fx * (3 - 2 * fx), fy * fy * (3 - 2 * fy)
    m = g.shape[0]
    a = g[y0 % m, x0 % m] * (1 - fx) + g[y0 % m, (x0 + 1) % m] * fx
    b = g[(y0 + 1) % m, x0 % m] * (1 - fx) + g[(y0 + 1) % m, (x0 + 1) % m] * fx
    return a * (1 - fy) + b * fy


def corner_cell(c, inner, outer, rng_seed):
    """16x16 cell for corner flags c=(nw,ne,sw,se): bilinear field thresholded with a Bayer edge."""
    ys, xs = np.mgrid[0:16, 0:16] / 15.0
    f = (c[0] * (1 - xs) * (1 - ys) + c[1] * xs * (1 - ys) + c[2] * (1 - xs) * ys + c[3] * xs * ys)
    f = f + (BAYER[np.mgrid[0:16, 0:16][0] % 4, np.mgrid[0:16, 0:16][1] % 4] - 0.5) * 0.35
    return np.where(f > 0.5, inner, outer).astype(np.uint8)


def build_ground():
    """Near-black ground: soft darker/lighter patches by marching squares on a periodic 32x32 vertex grid
    (16 cell shapes -> few unique tiles), sparse cold specks and a few cracks. Seamless by construction."""
    rng = random.Random(44)
    N = 32
    n1 = periodic_noise(N, 4, 3) * 0.7 + periodic_noise(N, 2, 5) * 0.3
    n2 = periodic_noise(N, 8, 11)
    lay1 = (n1 > 0.55).astype(int)           # patch level 2 (#0c0b16)
    lay2 = ((n1 > 0.70) & (n2 > 0.4)).astype(int)  # inner level 3 (#100f1e)
    img = np.full((512, 512), 1, np.uint8)
    for ty in range(N):
        for tx in range(N):
            for lay, inner, outer in ((lay1, 2, None), (lay2, 3, None)):
                c = (lay[ty][tx], lay[ty][(tx + 1) % N], lay[(ty + 1) % N][tx], lay[(ty + 1) % N][(tx + 1) % N])
                if sum(c) == 0:
                    continue
                cell = corner_cell(c, inner, 0, 0)
                reg = img[ty * 16:ty * 16 + 16, tx * 16:tx * 16 + 16]
                reg[cell > 0] = cell[cell > 0]
    # speck stamps (a handful of 8x8 variants, reused) only on flat tiles
    stamps = []
    for k in range(6):
        st = []
        for _ in range(1 + k % 3):
            st.append((rng.randrange(8), rng.randrange(8), rng.choice((4, 4, 4, 5))))
        stamps.append(st)
    for ty in range(64):
        for tx in range(64):
            t = img[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8]
            if t.min() == t.max() and rng.random() < 0.22:
                for x, y, c in rng.choice(stamps):
                    t[y, x] = c
    crack = np.array([[0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 6, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0],
                      [6, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]], np.uint8)
    for _ in range(14):
        tx, ty = rng.randrange(0, 62), rng.randrange(0, 64)
        reg = img[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 16]
        reg[crack > 0] = 6
    for _ in range(24):  # rare brighter cold glints
        tx, ty = rng.randrange(64), rng.randrange(64)
        img[ty * 8 + 3, tx * 8 + 4] = 9
    pal = list(GROUND_PAL) + [(0, 0, 0)] * (16 - len(GROUND_PAL))
    tiles = {img[y:y + 8, x:x + 8].tobytes() for y in range(0, 512, 8) for x in range(0, 512, 8)}
    aseprite_io.write_aseprite(os.path.join(ART, "ground.aseprite"), 512, 512, pal, [img.tolist()], [100], [])
    print(f"ground         512x512 unique tiles (no flip dedupe) {len(tiles)}")


# ------------------------------------------------------------------ props (same rects as hb_art.h)
PROPS_PAL = H(BG, "#06060a", "#0c0e16", "#141824", "#1c2232", "#262e42", "#323c54", "#425068", "#56687e",
              "#728aa0", "#1a3a3a", "#2a5a50", "#40e0ff", "#b0ffff", "#e040d0", "#a8b0c8")
LAYOUT = json.load(open(os.path.join(ART, "props_layout.json"))) if os.path.exists(
    os.path.join(ART, "props_layout.json")) else None
# name -> (source, index, scale) ; source "C"=C_chars 32px, "D"=C_deco 64px, "old"=recolour of the old prop
PROP_SRC = {"tree": ("D", 0, 1), "pine": ("D", 1, 1), "arch": ("D", 4, 1), "pillar": ("D", 8, 1),
            "ruin_wall": ("D", 5, 1), "rock": ("D", 10, 2), "bush": ("C", 34, 1), "bush_flowers": ("C", 35, 1),
            "stump": ("C", 33, 1), "mushrooms": ("old", 0, 1), "pillar_stump": ("D", 7, 2),
            "rune_stone": ("D", 9, 2), "tall_grass": ("C", 30, 1), "flowers": ("C", 31, 1), "pond": ("old", 0, 1),
            "crystal": ("C", 22, 1), "log": ("D", 11, 2), "rocks_small": ("C", 32, 1)}


def load_raw(src, i):
    folder = "C_chars" if src == "C" else "C_deco"
    im = Image.open(os.path.join(RAWC, folder, f"{i:02d}.png")).convert("RGBA")
    a = np.array(im)
    a[..., 3] = np.where(a[..., 3] >= 128, 255, 0)
    im = Image.fromarray(a)
    return im.crop(im.getbbox())


def half(im):
    """2x downscale keeping hard pixels: per 2x2 block, majority opaque, darkest-but-one colour."""
    a = np.array(im)
    h, w = a.shape[0] // 2 * 2, a.shape[1] // 2 * 2
    a = a[:h + (a.shape[0] % 2), :w + (a.shape[1] % 2)]
    if a.shape[0] % 2:
        a = np.concatenate([a, np.zeros((1,) + a.shape[1:], np.uint8)])
    if a.shape[1] % 2:
        a = np.concatenate([a, np.zeros((a.shape[0], 1, 4), np.uint8)], 1)
    out = np.zeros((a.shape[0] // 2, a.shape[1] // 2, 4), np.uint8)
    for y in range(out.shape[0]):
        for x in range(out.shape[1]):
            blk = a[2 * y:2 * y + 2, 2 * x:2 * x + 2].reshape(4, 4)
            op = blk[blk[:, 3] > 0]
            if len(op) >= 2:
                op = sorted(op.tolist(), key=lambda c: lum(c[:3]))
                out[y, x] = op[len(op) // 2]
    return Image.fromarray(out)


def nightify(rgb):
    """Old meadow prop colours -> style C (cold darks, neon accents for caps/water glints)."""
    h, s, v = hsv(rgb)
    L = lum(rgb)
    if s > 0.45 and v > 0.55 and (h < 50 or h > 300):  # mushroom caps, flowers
        return hexc("#40e0ff") if L < 0.6 else hexc("#b0ffff")
    if 180 < h < 240 and s > 0.3:  # water
        return hexc("#1a3a3a") if L < 0.5 else (hexc("#2a5a50") if L < 0.75 else hexc("#40e0ff"))
    k = int(min(8, L * 11))
    return PROPS_PAL[1 + k]


def build_props(old_props):
    pal = PROPS_PAL
    rects = {r["name"]: (r["x"], r["y"], r["w"], r["h"]) for r in LAYOUT_RECTS()}
    sheet = np.zeros((136, 256, 4), np.uint8)
    for name, (x, y, w, h) in rects.items():
        src, i, sc = PROP_SRC[name]
        if src == "old":
            a = old_props[y:y + h, x:x + w].copy()
            m = a[..., 3] > 0
            for yy, xx in zip(*np.nonzero(m)):
                a[yy, xx, :3] = nightify(tuple(int(c) for c in a[yy, xx, :3]))
            im = Image.fromarray(a)
            im = im.crop(im.getbbox())
        else:
            im = load_raw(src, i)
            if sc == 2:
                im = half(im)
                im = im.crop(im.getbbox())
            while im.width > w or im.height > h:
                im = half(im)
                im = im.crop(im.getbbox())
        ox = x + (w - im.width) // 2
        oy = y + h - im.height  # base on the bottom of the rect
        a = np.array(im)
        m = a[..., 3] > 0
        sheet[oy:oy + im.height, ox:ox + im.width][m] = a[m]
    P = np.array(pal[1:], np.int32)
    d = ((sheet[..., None, :3].astype(np.int32) - P[None, None]) ** 2).sum(-1)
    ix = (d.argmin(-1) + 1).astype(np.uint8)
    ix[sheet[..., 3] == 0] = 0
    aseprite_io.write_aseprite(os.path.join(ART, "props.aseprite"), 256, 136, list(pal), [ix.tolist()], [100], [])
    print("props          256x136")


def LAYOUT_RECTS():
    lay = LAYOUT
    if isinstance(lay, dict):
        lay = lay.get("props", lay.get("rects", lay))
    if isinstance(lay, dict):
        return [dict(name=k, x=v[0], y=v[1], w=v[2], h=v[3]) if isinstance(v, list) else dict(name=k, **v)
                for k, v in lay.items()]
    return lay


# ------------------------------------------------------------------ title (8 bpp, 256x256, image in 240x160)
def glow(base, cx, cy, r, cols, sy=1.2):
    a = base.astype(np.int32)
    hh, ww = a.shape[:2]
    ys, xs = np.mgrid[0:hh, 0:ww]
    t = np.hypot(xs - cx, (ys - cy) * sy) / r + (BAYER[ys % 4, xs % 4] - 0.5) * 0.25
    for k, c in enumerate(cols):
        m = t < (k + 1) / len(cols)
        a[m, :3] = np.clip(a[m, :3] + np.array(hexc(c)), 0, 255)
        t = np.where(m, 9, t)
    return a.astype(np.uint8)


def paste(base, im, x, y):
    a = np.array(im)
    m = a[..., 3] > 0
    h, w = a.shape[:2]
    reg = base[y:y + h, x:x + w]
    mm = m[:reg.shape[0], :reg.shape[1]]
    reg[mm, :3] = a[:reg.shape[0], :reg.shape[1]][mm][:, :3]


def build_title():
    W_, H_ = 240, 160
    img = np.zeros((H_, W_, 3), np.uint8)
    ys = np.mgrid[0:H_, 0:W_][0]
    xs = np.mgrid[0:H_, 0:W_][1]
    sky = H("#05040a", "#08061a", "#0e0a26", "#160e34")
    t = np.clip((ys / 100.0), 0, 1) * (len(sky) - 1) + (BAYER[ys % 4, xs % 4] - 0.5) * 0.8
    k = np.clip(np.round(t), 0, len(sky) - 1).astype(int)
    img[:] = np.array(sky, np.uint8)[k]
    rng = random.Random(3)
    for _ in range(70):
        x, y = rng.randrange(W_), rng.randrange(0, 96)
        img[y, x] = hexc(rng.choice(("#3a4a78", "#5a6aa0", "#9aa8e0", "#ffffff")))
    # moon
    mx, my = 226, 84
    m = np.hypot(xs - mx, ys - my) < 9
    img[m] = hexc("#c8d0f0")
    img[np.hypot(xs - mx - 4, ys - my + 2) < 8] = hexc("#0e0a26")
    # ground band
    gnd = (ys >= 112)
    img[gnd] = hexc(BG)
    img[gnd & (BAYER[ys % 4, xs % 4] > 0.85)] = hexc("#0c0b16")
    rgba = np.concatenate([img, np.full((H_, W_, 1), 255, np.uint8)], 2)
    # scenery silhouettes
    for src, i, x, base in (("D", 0, -6, 120), ("D", 1, 186, 122), ("D", 4, 88, 116), ("D", 11, 20, 132),
                            ("D", 8, 164, 128)):
        im = load_raw(src, i)
        paste(rgba, im, x, base - im.height)
    # Wendel with his staff, 2x, glowing
    tw = os.path.join(STYLE_C_RAW, "title_wizard.png")
    if os.path.exists(tw):
        wz = Image.open(tw).convert("RGBA")
        wz = wz.crop(wz.getbbox())
    else:
        wz = load_raw("C", 3)
        wz = wz.resize((wz.width * 2, wz.height * 2), Image.NEAREST)
    rgba = glow(rgba, 128, 104, 34, ["#3a0a44", "#22062a", "#120316"])
    for x in (40, 70, 168, 200):
        rgba = glow(rgba, x, 140, 8, ["#16083a", "#0a0420"])
    paste(rgba, wz, 120 - wz.width // 2, 140 - wz.height)
    for _ in range(14):  # gems scattered on the ground
        x, y = rng.randrange(10, 230), rng.randrange(134, 156)
        if abs(x - 120) < 24:
            continue
        rgba = glow(rgba, x, y, 5, ["#3a0a44", "#1a0420"])
        c = hexc(rng.choice(("#ff30c8", "#20d0ff", "#7ee830")))
        for dx, dy in ((0, 0), (0, -1), (1, -1), (-1, -1), (0, -2)):
            rgba[y + dy, x + dx, :3] = c
        rgba[y - 2, x, :3] = (255, 255, 255)
    rgb = Image.fromarray(rgba[..., :3], "RGB")
    q = rgb.quantize(200, method=Image.MEDIANCUT, dither=Image.NONE)
    qp = (q.getpalette() + [0] * 768)[:600]
    ix = np.array(q).astype(np.int32) + 1
    full = np.zeros((256, 256), np.uint8)
    full[:H_, :W_] = ix
    full[H_:, :] = 1 + int(np.argmin([sum(qp[3 * i:3 * i + 3]) for i in range(200)]))  # darkest
    full[:H_, W_:] = full[H_ - 1:H_, :1] * 0 + full[H_ + 1, 0]
    pal = [(0, 0, 0)] + [tuple(qp[i:i + 3]) for i in range(0, 600, 3)]
    aseprite_io.write_aseprite(os.path.join(ART, "title_bg.aseprite"), 256, 256, pal, [full.tolist()], [100], [])
    print("title_bg       256x256 colours", len(set(full.ravel().tolist())))


# ------------------------------------------------------------------ text palettes
TEXT = {"text_ink": ("#e8ecff", "#06060c"), "text_gold": ("#80f4ff", "#0a1030"), "text_red": ("#ff5070", "#14040a")}


def text_palettes():
    base = os.path.join(ROOT, "engine", "butano", "common", "graphics", "common_variable_8x8_font.bmp")
    fp = Image.open(base).getpalette()[:48]
    fp = [tuple(fp[i:i + 3]) for i in range(0, 48, 3)]
    for name, (glyph, shadow) in TEXT.items():
        pal = list(fp)
        pal[14], pal[12] = hexc(glyph), hexc(shadow)
        pix = [[(x + (8 if y >= 4 else 0)) % 16 for x in range(8)] for y in range(8)]
        aseprite_io.write_bmp4(os.path.join(GFX, name + ".bmp"), 8, 8, pal, pix)
        print("palette", name)


def main(argv):
    if argv[:1] == ["text"]:
        text_palettes()
        return
    assert os.path.isdir(OLD), "art/old_style/ missing: back up the old set first"
    data = restyle_sprites()
    nova()
    blink()
    hud_nova()
    build_ground()
    p = data["props"]
    build_props(to_rgba(p["idx"][0], p["pal"]))
    build_title()
    text_palettes()


if __name__ == "__main__":
    main(sys.argv[1:])
