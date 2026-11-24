"""007_survivors art pipeline.

  python -I build_art.py seed     -> creates art/*.aseprite from art/raw/ (PixelLab seeds) and the hand-drawn
                                     pixel tables below. NEVER overwrites an existing .aseprite.
  python -I build_art.py export   -> exports every art/*.aseprite with Aseprite (batch) to graphics/*.bmp (4bpp)
                                     + Butano .json. The .aseprite is the source of truth.
  python -I build_art.py contact  -> 240x160 mockup at 3x (scratch dir given by CONTACT_OUT or art/raw/).
"""
import glob
import json
import os
import random
import struct
import subprocess
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import aseprite_io  # noqa: E402

TOOLS = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(TOOLS)
RAW = os.path.join(ART, "raw")
GFX = os.path.join(os.path.dirname(ART), "graphics")
ASEPRITE = r"C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"


def hexc(h):
    h = h.lstrip("#")
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))


# ---------------------------------------------------------------- palettes (index 0 = transparent)
PAL_WIZARD = [hexc(c) for c in ["#ff00ff", "#1b1028", "#3a2160", "#5e3496", "#8a55c8", "#b78ae6", "#e2c6ff", "#b8705a",
                                "#f0b890", "#e8e8f0", "#9a9ab0", "#4a2a18", "#8a5a2a", "#f2c84a", "#5ad8f0", "#2a7ab0"]]
PAL_PICKUPS = [hexc(c) for c in ["#ff00ff", "#1a1420", "#1e7a3a", "#3ec85a", "#a8f0a0", "#1e4aa0", "#3a8af0", "#a8d8ff",
                                 "#8a1a2a", "#e03a4a", "#ffa0a0", "#8a5a10", "#e8b030", "#fff080", "#ffffff", "#6a3a1a"]]
PAL_FX = [hexc(c) for c in ["#ff00ff", "#1a1440", "#ffffff", "#fff6a0", "#ffd23a", "#ff8a1a", "#c0302a", "#e0ffff",
                            "#a0f0ff", "#50b8ff", "#2a5ad0", "#f0c8ff", "#c070ff", "#7a30d0", "#3a1470", "#102010"]]


def nearest(pal, rgb, start=1):
    best, bi = None, start
    for i in range(start, len(pal)):
        p = pal[i]
        d = (p[0] - rgb[0]) ** 2 * 3 + (p[1] - rgb[1]) ** 2 * 4 + (p[2] - rgb[2]) ** 2 * 2
        if best is None or d < best:
            best, bi = d, i
    return bi


def joint_palette(images, n=15):
    """Median-cut over the opaque pixels of all images -> [transparent] + n colours sorted by luminance."""
    px = []
    for im in images:
        for r, g, b, a in im.getdata():
            if a >= 128:
                px.append((r, g, b))
    strip = Image.new("RGB", (len(px), 1))
    strip.putdata(px)
    q = strip.quantize(colors=n, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[: 3 * n]
    cols = sorted({tuple(pal[i:i + 3]) for i in range(0, len(pal), 3)}, key=lambda c: c[0] * 3 + c[1] * 6 + c[2])
    return [(255, 0, 255)] + cols


def to_indices(im, pal):
    w, h = im.size
    data = list(im.getdata())
    cache = {}
    out = []
    for y in range(h):
        row = []
        for x in range(w):
            r, g, b, a = data[y * w + x]
            if a < 128:
                row.append(0)
            else:
                k = (r, g, b)
                if k not in cache:
                    cache[k] = nearest(pal, k)
                row.append(cache[k])
        out.append(row)
    return out


def remove_orphans(pix):
    """Clear opaque pixels with no opaque 4-neighbour (stray dust from generation)."""
    h, w = len(pix), len(pix[0])
    for y in range(h):
        for x in range(w):
            if pix[y][x] and all(not (0 <= y + dy < h and 0 <= x + dx < w) or not pix[y + dy][x + dx]
                                 for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                pix[y][x] = 0
    return pix


# ---------------------------------------------------------------- frame sources
def char_frames(name, anim, direction):
    fs = sorted(glob.glob(os.path.join(RAW, name, "*", "animations", anim, direction, "*.png")))
    if not fs:
        raise SystemExit(f"missing frames: {name}/{anim}/{direction}")
    return [Image.open(f).convert("RGBA") for f in fs]


def char_rot(name, direction):
    return [Image.open(glob.glob(os.path.join(RAW, name, "*", "rotations", direction + ".png"))[0]).convert("RGBA")]


def fit(frames, w, h, anchor="bottom", pad_bottom=0, label=""):
    """Place every frame on a WxH canvas with ONE offset (union bbox), so the animation does not jitter."""
    boxes = [f.getbbox() for f in frames if f.getbbox()]
    x0 = min(b[0] for b in boxes); y0 = min(b[1] for b in boxes)
    x1 = max(b[2] for b in boxes); y1 = max(b[3] for b in boxes)
    bw, bh = x1 - x0, y1 - y0
    if bw > w or bh > h:
        print(f"  warning {label}: content {bw}x{bh} does not fit {w}x{h}; cropping")
    dx = (w - bw) // 2 - x0
    dy = (h - pad_bottom - bh) - y0 if anchor == "bottom" else (h - bh) // 2 - y0
    out = []
    for f in frames:
        c = Image.new("RGBA", (w, h), (0, 0, 0, 0))
        c.alpha_composite(f.crop((max(0, -dx), max(0, -dy), f.width, f.height)), (max(0, dx), max(0, dy)))
        out.append(c)
    return out


def pick(frames, idx):
    return [frames[i] for i in idx]


def flip(frames):
    return [f.transpose(Image.FLIP_LEFT_RIGHT) for f in frames]


# ---------------------------------------------------------------- hand-drawn pixel tables
def grid(rows, keymap):
    return [[keymap.get(ch, 0) for ch in row] for row in rows]


GEM = ["...oo...", "..owmo..", ".owlmmo.", "owlmmmdo", "olmmmddo", ".ommddo.", "..oddo..", "...oo..."]
GEM2 = ["...oo...", "..owmo..", ".owwlmo.", "owwlmmdo", "olmmmddo", ".ommddo.", "..oddo..", "...oo..."]
COIN = ["..oooo..", ".owllmo.", "owlmmmdo", "olmllmdo", "olmllmdo", "olmmmddo", ".omdddo.", "..oooo.."]
COIN2 = ["...oo...", "..owmo..", "..olmo..", "..olmo..", "..olmo..", "..ommo..", "..oddo..", "...oo..."]
SPARK = ["...o....", "...y....", "..yWy...", "oyWWWyo.", "..yWy...", "...y....", "...o....", "........"]
SPARK2 = ["o.....o.", ".y...y..", "..yWy...", "..WWW...", "..yWy...", ".y...y..", "o.....o.", "........"]
RUNE = ["..PPPP..", ".PmwmmP.", "PmmwwmlP", "PmmwmwlP", "PmmwwmlP", "PmmwmmlP", ".PmwmlP.", "..PPPP.."]
IMPACT = [
    ["........", "........", "...W....", "..WWW...", "...W....", "........", "........", "........"],
    ["...y....", ".y.W.y..", "..WWW...", "yWWWWWy.", "..WWW...", ".y.W.y..", "...y....", "........"],
    ["y..o..y.", "........", "..o.o...", "o.....o.", "..o.o...", "........", "y..o..y.", "........"],
]
POOF = [
    ["........", "........", "...ll...", "..lwwl..", "..lwwl..", "...ll...", "........", "........"],
    ["........", "..llll..", ".lwwwwl.", ".lwwwvl.", ".lwwvvl.", ".lvvvvl.", "..llll..", "........"],
    [".l.ll.l.", "l.vwwv.l", ".vwvvwv.", ".wv..vw.", ".wv..vw.", ".vwvvwv.", "l.vwwv.l", ".l.ll.l."],
    ["l..l..l.", "........", ".v....v.", "........", "........", ".v....v.", "........", "l..l..l."],
]
FX_KEYS = {"W": 2, "y": 4, "o": 5, "P": 14, "m": 12, "l": 11, "w": 2, "v": 12}


def ice_shard(a, b, sparkle):
    """16x16 ice crystal (Frost Ring segment): diamond |x|/a+|y|/b<=1 lit from the top-left, dark outline."""
    pix = [[0] * 16 for _ in range(16)]
    cx, cy = 7.5, 7.5
    for y in range(16):
        for x in range(16):
            dx, dy = x - cx, y - cy
            if abs(dx) / a + abs(dy) / b <= 1.0:
                pix[y][x] = 8 if dx < -0.5 else (9 if dx < 1 else 10)
                if dy < -b / 3 and dx < 0:
                    pix[y][x] = 7
    for y in range(16):  # outline
        for x in range(16):
            if not pix[y][x] and any(0 <= y + j < 16 and 0 <= x + i < 16 and pix[y + j][x + i] not in (0, 1)
                                     for i, j in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                pix[y][x] = 1
    if sparkle:
        for x, y in ((3, 2), (12, 12), (13, 4)):
            pix[y][x] = 2
    return pix


def aura(phase):
    pix = [[0] * 16 for _ in range(16)]
    for y in range(16):
        for x in range(16):
            d = ((x - 7.5) ** 2 + (y - 7.5) ** 2) ** 0.5
            if 6.3 <= d <= 7.6:
                pix[y][x] = 6 if (x + y + phase) % 4 else 5
            elif 5.4 <= d < 6.3 and (x * 3 + y + phase * 2) % 5 == 0:
                pix[y][x] = 4
    return pix


def ellipse(w, h, color):
    pix = [[0] * w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            if ((x - (w - 1) / 2) / (w / 2)) ** 2 + ((y - (h - 1) / 2) / (h / 2)) ** 2 <= 1.0:
                pix[y][x] = color
    return pix


# ---------------------------------------------------------------- sprite specs
def specs():
    """name -> dict(size, palette, tags=[(tag, frames(list of PIL or index grids), ms)])."""
    S = {}

    # Wizard "Wendel": 32x32, own palette. walk side = east (flip for west).
    wz = {
        "idle": char_frames("wizard", "idle", "south"),
        "idle_side": char_frames("wizard", "idle", "east"),
        "walk_down": char_frames("wizard", "walk", "south"),
        "walk_side": char_frames("wizard", "walk", "east"),
        "walk_up": char_frames("wizard", "walk", "north"),
        "hurt": pick(char_frames("wizard", "hurt", "south"), [1, 2]),
        "death": char_frames("wizard", "death", "south"),
    }
    allf = [f for v in wz.values() for f in v]
    fitted = fit(allf, 32, 32, pad_bottom=1)
    k = 0
    for key in list(wz):
        n = len(wz[key]); wz[key] = fitted[k:k + n]; k += n
    S["wizard"] = dict(size=(32, 32), palette=PAL_WIZARD, tags=[
        ("idle", wz["idle"], 250), ("idle_side", wz["idle_side"], 250), ("walk_down", wz["walk_down"], 100),
        ("walk_side", wz["walk_side"], 100), ("walk_up", wz["walk_up"], 100), ("hurt", wz["hurt"], 100),
        ("death", wz["death"], 120)])

    def enemy(name, size, tags, anchor="bottom"):
        # one offset per animation (no jitter inside a loop); feet stay on the bottom row in every tag
        out = [(t, fit(fr, size[0], size[1], anchor=anchor, label=f"{name}/{t}"), ms) for t, fr, ms in tags]
        return dict(size=size, palette=None, tags=out)

    S["slime"] = enemy("slime", (16, 16), [("hop", char_frames("slime", "hop", "south"), 110),
                                           ("death", char_frames("slime", "death", "south"), 90)])
    S["bat"] = enemy("bat", (32, 16), [("fly", char_frames("bat", "fly", "south"), 80),
                                       ("death", pick(char_frames("bat", "death", "south"), [0, 1, 2]), 80)], anchor="center")
    S["skeleton"] = enemy("skeleton", (16, 32), [("walk", char_frames("skeleton", "walk", "east"), 110),
                                                 ("death", char_frames("skeleton", "death", "east"), 90)])
    S["slime_king"] = enemy("slime_king", (64, 64), [("idle", char_frames("slime_king", "idle", "south"), 160),
                                                     ("crouch", pick(char_frames("slime_king", "jump", "south"), [1]), 100),
                                                     ("air", pick(char_frames("slime_king", "jump", "south"), [3, 4]), 100),
                                                     ("land", pick(char_frames("slime_king", "jump", "south"), [6, 7]), 100)])
    S["skeleton_big"] = enemy("skeleton_big", (32, 32), [("walk", char_frames("skeleton_big", "walk", "east"), 110),
                                                         ("death", char_frames("skeleton_big", "death", "east"), 90)])
    S["goblin"] = enemy("goblin", (16, 16), [("walk", char_frames("goblin", "walk", "east"), 100)])
    S["orc"] = enemy("orc", (32, 32), [("walk", char_frames("orc", "walk", "east"), 120)])
    S["orc_warchief"] = enemy("orc_warchief", (64, 64), [("walk", char_frames("orc_warchief", "walk", "east"), 120),
                                                         ("attack", char_frames("orc_warchief", "attack", "east"), 90)])

    # Shared enemy palettes (joint median cut): slime+slime_king, skeleton+bat, goblin+orc.
    for group in (("slime", "slime_king"), ("skeleton", "bat", "skeleton_big"), ("goblin", "orc"), ("orc_warchief",)):
        imgs = [f for g in group for _, fr, _ in S[g]["tags"] for f in fr]
        pal = joint_palette(imgs)
        for g in group:
            S[g]["palette"] = pal
            S[g]["palette_group"] = "+".join(group)

    # Pickups (8x8 hand-drawn, pickups palette).
    gk = lambda o, w, l, m, d: {"o": o, "w": w, "l": l, "m": m, "d": d}
    gems = []
    for ramp in ((1, 14, 7, 6, 5), (1, 14, 4, 3, 2), (1, 14, 10, 9, 8)):  # blue 1xp, green 5xp, red 25xp
        gems += [grid(GEM, gk(*ramp)), grid(GEM2, gk(*ramp))]
    S["gems"] = dict(size=(8, 8), palette=PAL_PICKUPS, tags=[("blue", gems[0:2], 300), ("green", gems[2:4], 300),
                                                              ("red", gems[4:6], 300)])
    S["coin"] = dict(size=(8, 8), palette=PAL_PICKUPS, tags=[("spin", [grid(COIN, gk(1, 14, 13, 12, 11)),
                                                                        grid(COIN2, gk(1, 14, 13, 12, 11))], 160)])
    items = lambda i: Image.open(os.path.join(RAW, "items16", f"{i:02d}.png")).convert("RGBA")
    S["chest"] = dict(size=(16, 16), palette=PAL_PICKUPS, tags=[("closed", [items(12)], 100), ("open", [items(7)], 100)])
    S["heart"] = dict(size=(16, 16), palette=PAL_PICKUPS, tags=[("idle", [items(13)], 100)])
    # Upgrade icons 16x16: spark bolt (wand), rune orbit (blue orb), frost ring (blue gem crystal),
    # swift boots, heart amulet (necklace), magnet, spell tome, star.
    S["icons"] = dict(size=(16, 16), palette=PAL_PICKUPS, tags=[
        ("spark_bolt", [items(36)], 100), ("rune_orbit", [items(49)], 100), ("frost_ring", [items(3)], 100),
        ("swift_boots", [items(38)], 100), ("heart_amulet", [items(41)], 100), ("magnet", [items(52)], 100),
        ("spell_tome", [items(43)], 100), ("star", [items(62)], 100)])

    # Shrine 16x32 (own palette).
    shrine = fit([Image.open(glob.glob(os.path.join(RAW, "shrine", "**", "*.png"), recursive=True)[0]).convert("RGBA")],
                 16, 32)
    S["shrine"] = dict(size=(16, 32), palette=joint_palette(shrine), tags=[("idle", shrine, 100)])

    # FX (hand-drawn, fx palette).
    S["fx_spark"] = dict(size=(8, 8), palette=PAL_FX, tags=[("loop", [grid(SPARK, FX_KEYS), grid(SPARK2, FX_KEYS)], 60)])
    rune2 = {**FX_KEYS, "m": 11, "l": 2}
    S["fx_rune"] = dict(size=(8, 8), palette=PAL_FX, tags=[("loop", [grid(RUNE, FX_KEYS), grid(RUNE, rune2)], 120)])
    S["fx_frost"] = dict(size=(16, 16), palette=PAL_FX, tags=[("grow", [ice_shard(2.5, 4, False), ice_shard(4, 6, False),
                                                                          ice_shard(5, 7.4, True)], 70)])
    S["fx_impact"] = dict(size=(8, 8), palette=PAL_FX, tags=[("hit", [grid(r, FX_KEYS) for r in IMPACT], 50)])
    S["fx_poof"] = dict(size=(8, 8), palette=PAL_FX, tags=[("poof", [grid(r, FX_KEYS) for r in POOF], 70)])
    S["fx_aura"] = dict(size=(16, 16), palette=PAL_FX, tags=[("loop", [aura(0), aura(2)], 100)])
    S["shadow_small"] = dict(size=(16, 8), palette=PAL_FX, tags=[("idle", [ellipse(14, 6, 15)], 100)])
    S["shadow_big"] = dict(size=(32, 16), palette=PAL_FX, tags=[("idle", [ellipse(30, 12, 15)], 100)])
    return S


def frame_indices(fr, pal, clean):
    if isinstance(fr, list):
        # grids may be smaller than the canvas (ellipses): pad
        return fr
    pix = to_indices(fr, pal)
    return remove_orphans(pix) if clean else pix


def pad(pix, w, h):
    out = [[0] * w for _ in range(h)]
    oy, ox = (h - len(pix)) // 2, (w - len(pix[0])) // 2
    for y, row in enumerate(pix):
        for x, v in enumerate(row):
            out[oy + y][ox + x] = v
    return out


# ---------------------------------------------------------------- ground (regular BG 512x512) and props
def tileset_tiles():
    """Wang tiles of the meadow set: dict corners(NW,NE,SW,SE as 'l'/'u') -> RGBA 16x16."""
    meta = json.load(open(os.path.join(RAW, "meadow", "metadata.json")))
    sheet = Image.open(os.path.join(RAW, "meadow", "sheet.png")).convert("RGBA")
    tiles = {}
    for t in meta["tileset_data"]["tiles"]:
        c = t["corners"]
        key = "".join("u" if c[k] == "upper" else "l" for k in ("NW", "NE", "SW", "SE"))
        bb = t["bounding_box"]
        tiles[key] = sheet.crop((bb["x"], bb["y"], bb["x"] + bb["width"], bb["y"] + bb["height"]))
    return tiles


def ground_palette():
    """Meadow palette, slightly desaturated/darkened so sprites pop (DISENO: suelo apagado)."""
    base = ["#14281a", "#1f4a2a", "#2e6a34", "#3f8a3c", "#5aa84a", "#78b456", "#94c060", "#3a2418", "#6a4428",
            "#9a6a3a", "#c89a5a", "#e8c88a", "#d86a8a", "#e8d870", "#e0e0cc"]
    out = []
    for h in base:
        r, g, b = hexc(h)
        l = (r * 3 + g * 6 + b) / 10
        s = 0.62
        r, g, b = (int(l + (c - l) * s) for c in (r, g, b))
        out.append((int(r * 0.9), int(g * 0.9), int(b * 0.9)))
    return [out[2]] + out  # index 0 = backdrop (dark-mid grass); never drawn by a pixel


def build_ground():
    rnd = random.Random(7)
    tiles = tileset_tiles()
    pal = ground_palette()
    N = 32  # 32x32 tiles of 16px = 512px, vertex grid periodic so the BG wraps seamlessly
    vert = [["u"] * N for _ in range(N)]
    for _ in range(5):  # a few dirt clearings/paths
        cx, cy = rnd.randrange(N), rnd.randrange(N)
        for _ in range(rnd.randint(6, 14)):
            vert[cy % N][cx % N] = "l"
            if rnd.random() < 0.5:
                vert[(cy + 1) % N][cx % N] = "l"
            cx += rnd.choice((-1, 0, 1, 1)); cy += rnd.choice((-1, 0, 0, 1))
    img = Image.new("RGBA", (512, 512))
    for ty in range(N):
        for tx in range(N):
            key = vert[ty][tx] + vert[ty][(tx + 1) % N] + vert[(ty + 1) % N][tx] + vert[(ty + 1) % N][(tx + 1) % N]
            t = tiles[key]
            if key == "uuuu" and rnd.random() < 0.5:
                t = t.transpose(Image.FLIP_LEFT_RIGHT)
            img.alpha_composite(t, (tx * 16, ty * 16))
    pix = to_indices(img, [None] + pal[1:] if False else pal)
    # flower sprinkles (variants): tiny 1-3 px flowers in the palette's accents
    for _ in range(40):
        x, y = rnd.randrange(1, 511), rnd.randrange(1, 511)
        if vert[(y // 16)][(x // 16)] == "u" and pix[y][x] in (3, 4, 5):
            c = rnd.choice((13, 14, 15, 15))
            pix[y][x] = c
            if rnd.random() < 0.4:
                pix[y - 1][x] = 7
    for row in pix:  # nothing may use index 0
        for x, v in enumerate(row):
            if v == 0:
                row[x] = 3
    return pal, pix


PROPS = [  # name, review pack, candidate index; canvas = content rounded up to 8 px (BG tiles)
    ("tree", "props64", 1), ("pine", "props64", 2), ("ruin_wall", "props64", 8), ("arch", "props64", 12),
    ("pillar", "props64", 6), ("rock", "props32", 0), ("rocks_small", "props32", 2), ("bush", "props32", 3),
    ("bush_flowers", "props32", 4), ("stump", "props32", 5), ("mushrooms", "props32", 6),
    ("pillar_stump", "props32", 7), ("rune_stone", "props32", 9), ("tall_grass", "props32", 10),
    ("flowers", "props32", 11), ("pond", "props32", 33), ("crystal", "props32", 22), ("log", "props32", 12),
]


def props_sources():
    out = []
    for name, pack, i in PROPS:
        im = Image.open(os.path.join(RAW, pack, f"{i:02d}.png")).convert("RGBA")
        bb = im.getbbox()
        w, h = (bb[2] - bb[0] + 7) // 8 * 8, (bb[3] - bb[1] + 7) // 8 * 8
        out.append((name, fit([im], w, h)[0]))
    return out


# ---------------------------------------------------------------- commands
def seed(only):
    S = specs()
    for name, sp in S.items():
        if only and name not in only:
            continue
        path = os.path.join(ART, name + ".aseprite")
        if os.path.exists(path):
            print("keep (exists):", path)
            continue
        w, h = sp["size"]
        frames, durs, tags = [], [], []
        for tag, frs, ms in sp["tags"]:
            a = len(frames)
            for fr in frs:
                pix = frame_indices(fr, sp["palette"], clean=w >= 16)
                frames.append(pad(pix, w, h) if (len(pix), len(pix[0])) != (h, w) else pix)
                durs.append(ms)
            tags.append((tag, a, len(frames) - 1))
        pal = list(sp["palette"]) + [(0, 0, 0)] * (16 - len(sp["palette"]))
        aseprite_io.write_aseprite(path, w, h, pal, frames, durs, tags)
        print("seeded", path, len(frames), "frames")
    if (not only or "ground" in only) and not os.path.exists(os.path.join(ART, "ground.aseprite")):
        pal, pix = build_ground()
        aseprite_io.write_aseprite(os.path.join(ART, "ground.aseprite"), 512, 512, pal, [pix], [100], [])
        print("seeded ground")
    if (not only or "props" in only) and not os.path.exists(os.path.join(ART, "props.aseprite")):
        seed_props()


def seed_props():
    srcs = props_sources()
    srcs.sort(key=lambda t: -t[1].height)
    pal = joint_palette([im for _, im in srcs])
    W, H = 256, 512
    sheet = [[0] * W for _ in range(H)]
    x = y = rowh = 0
    layout = []
    for name, im in srcs:
        w, h = im.size
        if x + w > W:
            x, y, rowh = 0, y + rowh, 0
        pix = remove_orphans(to_indices(im, pal))
        for j in range(h):
            sheet[y + j][x:x + w] = pix[j]
        layout.append((name, x, y, w, h))
        x += w; rowh = max(rowh, h)
    H = (y + rowh + 7) // 8 * 8
    sheet = sheet[:H]
    aseprite_io.write_aseprite(os.path.join(ART, "props.aseprite"), W, H, pal, [sheet], [100], [])
    with open(os.path.join(ART, "props_layout.json"), "w") as f:
        json.dump([dict(name=n, x=a, y=b, w=c, h=d) for n, a, b, c, d in layout], f, indent=1)
    print("seeded props", layout)


def write_bmp4(path, w, h, pal, pix):
    aseprite_io.write_bmp4(path, w, h, pal[:16], pix)


def export():
    os.makedirs(GFX, exist_ok=True)
    tmp = os.path.join(RAW, "_export")
    os.makedirs(tmp, exist_ok=True)
    for path in sorted(glob.glob(os.path.join(ART, "*.aseprite"))):
        name = os.path.splitext(os.path.basename(path))[0]
        png = os.path.join(tmp, name + ".png")
        data = os.path.join(tmp, name + ".json")
        subprocess.run([ASEPRITE, "-b", path, "--sheet-type", "vertical", "--sheet", png, "--data", data,
                        "--format", "json-array", "--list-tags"], check=True, capture_output=True)
        im = Image.open(png)
        assert im.mode == "P", (name, im.mode)
        meta = json.load(open(data))
        fw = meta["frames"][0]["frame"]["w"]; fh = meta["frames"][0]["frame"]["h"]
        pal = (im.getpalette() + [0] * 48)[:48]  # short palettes (few colours) are padded
        pal = [tuple(pal[i:i + 3]) for i in range(0, 48, 3)]
        w, h = im.size
        px = list(im.getdata())
        pix = [px[y * w:(y + 1) * w] for y in range(h)]
        if name.endswith("_palette") and name != "props_palette":  # G-51: palette-only items (preview + palette)
            bg = name.startswith(("ground_", "props_"))
            pp = [[(x + (8 if y >= 4 else 0)) % 16 for x in range(8)] for y in range(8)]
            write_bmp4(os.path.join(GFX, name + ".bmp"), 8, 8, pal, pp)
            _json(name, {"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16} if bg
                  else {"type": "sprite_palette", "bpp_mode": "bpp_4"})
            print(f"{name:14s} palette ({'bg' if bg else 'sprite'})")
            continue
        if name in SPECIAL_EXPORT:  # set 2 (build_ui.py): 8 bpp title, sliced logo, split card
            fullpal = im.getpalette()[:768]
            fullpal = [tuple(fullpal[i:i + 3]) for i in range(0, len(fullpal), 3)]
            SPECIAL_EXPORT[name](pal, fullpal, pix, w, h, fh)
            print(f"{name:14s} {w}x{h} frame {fw}x{fh} x{len(meta['frames'])} (special)")
            continue
        assert max(px) < 16, (name, "uses colour index >= 16")
        write_bmp4(os.path.join(GFX, name + ".bmp"), w, h, pal, pix)
        if name in ("ground",):
            j = {"type": "regular_bg", "bpp_mode": "bpp_4"}
        elif name == "props":
            j = {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"}
            write_props_palette(pal)
        else:
            j = {"type": "sprite", "height": fh}
        with open(os.path.join(GFX, name + ".json"), "w") as f:
            json.dump(j, f, indent=4)
        tags = [(t["name"], t["from"], t["to"]) for t in meta["meta"].get("frameTags", [])]
        print(f"{name:14s} {w}x{h} frame {fw}x{fh} x{len(meta['frames'])} tags {tags}")


def _json(name, j):
    with open(os.path.join(GFX, name + ".json"), "w") as f:
        json.dump(j, f, indent=4)


def write_bmp8(path, w, h, pal, pix):
    """Uncompressed 8 bpp indexed BMP (256-colour palette, bottom-up)."""
    pal = list(pal)[:256] + [(0, 0, 0)] * (256 - len(pal))
    row = (w + 3) & ~3
    data = bytearray()
    for y in range(h - 1, -1, -1):
        data += bytes(pix[y]) + b"\0" * (row - w)
    pb = b"".join(struct.pack("<BBBx", b, g, r) for (r, g, b) in pal)
    off = 14 + 40 + len(pb)
    with open(path, "wb") as f:
        f.write(b"BM" + struct.pack("<IHHI", off + len(data), 0, 0, off))
        f.write(struct.pack("<IiiHHIIiiII", 40, w, h, 1, 8, 0, len(data), 2835, 2835, 256, 256))
        f.write(pb + data)


def _export_title_bg(pal, fullpal, pix, w, h, fh):
    write_bmp8(os.path.join(GFX, "title_bg.bmp"), w, h, fullpal, pix)
    _json("title_bg", {"type": "regular_bg", "bpp_mode": "bpp_8"})


def _export_logo(pal, fullpal, pix, w, h, fh):
    # 192x64 logo -> 64x32 cells, row-major (frame k at x = (k % 3) * 64, y = (k // 3) * 32)
    cells = []
    for cy in range(0, h, 32):
        for cx in range(0, w, 64):
            cells += [r[cx:cx + 64] for r in pix[cy:cy + 32]]
    write_bmp4(os.path.join(GFX, "logo.bmp"), 64, len(cells), pal, cells)
    _json("logo", {"type": "sprite", "height": 32})


def _export_card(pal, fullpal, pix, w, h, fh):
    # 64x80 frames (normal, selected) -> card 64x64 (rows 0-63) + card_base 64x32 (rows 48-79, overlaps 16)
    top, base = [], []
    for k in range(h // fh):
        fr = pix[k * fh:(k + 1) * fh]
        top += fr[0:64]
        base += fr[48:80]
    write_bmp4(os.path.join(GFX, "card.bmp"), 64, len(top), pal, top)
    write_bmp4(os.path.join(GFX, "card_base.bmp"), 64, len(base), pal, base)
    _json("card", {"type": "sprite", "height": 64})
    _json("card_base", {"type": "sprite", "height": 32})


SPECIAL_EXPORT = {"title_bg": _export_title_bg, "logo": _export_logo, "card": _export_card}


def write_props_palette(pal):
    # props.bmp is exported as regular_bg_tiles; its palette as a bg_palette item (props_palette).
    pix = [[(x + (8 if y >= 4 else 0)) % 16 for x in range(8)] for y in range(8)]
    aseprite_io.write_bmp4(os.path.join(GFX, "props_palette.bmp"), 8, 8, pal, pix)
    with open(os.path.join(GFX, "props_palette.json"), "w") as f:
        json.dump({"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16}, f, indent=4)


def load_bmp_frames(name):
    im = Image.open(os.path.join(GFX, name + ".bmp")).convert("P")
    pal = im.getpalette()
    rgba = Image.new("RGBA", im.size)
    src = im.load(); dst = rgba.load()
    for y in range(im.height):
        for x in range(im.width):
            i = src[x, y]
            if i:
                dst[x, y] = (pal[3 * i], pal[3 * i + 1], pal[3 * i + 2], 255)
    fh = json.load(open(os.path.join(GFX, name + ".json"))).get("height", im.height)
    return [rgba.crop((0, k * fh, im.width, (k + 1) * fh)) for k in range(im.height // fh)]


def contact(out_dir):
    """240x160 mockup built ONLY from the exported graphics/*.bmp, plus a 3x copy."""
    g = Image.open(os.path.join(GFX, "ground.bmp")).convert("RGB")
    scene = Image.new("RGBA", (240, 160))
    scene.paste(g.crop((96, 64, 336, 224)), (0, 0))
    props = Image.open(os.path.join(GFX, "props.bmp")).convert("P")
    ppal = props.getpalette()
    lay = json.load(open(os.path.join(ART, "props_layout.json")))
    pr = {}
    for p in lay:
        c = props.crop((p["x"], p["y"], p["x"] + p["w"], p["y"] + p["h"]))
        rgba = Image.new("RGBA", c.size)
        for yy in range(c.height):
            for xx in range(c.width):
                i = c.getpixel((xx, yy))
                if i:
                    rgba.putpixel((xx, yy), (ppal[3 * i], ppal[3 * i + 1], ppal[3 * i + 2], 255))
        pr[p["name"]] = rgba
    put = lambda im, x, y: scene.alpha_composite(im, (x, y))
    for name, x, y in (("tree", -14, -18), ("ruin_wall", 176, 100), ("pine", 196, -10), ("rock", 120, 120),
                       ("bush", 20, 118), ("stump", 150, 20)):
        if name in pr:
            put(pr[name], x if x >= 0 else 0, y if y >= 0 else 0)
    F = {n: load_bmp_frames(n) for n in ("wizard", "slime", "bat", "skeleton", "slime_king", "gems", "coin", "chest",
                                         "heart", "fx_spark", "fx_rune", "fx_frost", "fx_impact", "fx_poof",
                                         "shrine", "shadow_small", "shadow_big", "fx_aura", "goblin", "orc")
         if os.path.exists(os.path.join(GFX, n + ".bmp"))}
    sh = F.get("shadow_small", [None])[0]
    def actor(n, k, x, y, shadow=True, flipx=False):
        if n not in F:
            return
        im = F[n][k % len(F[n])]
        if flipx:
            im = im.transpose(Image.FLIP_LEFT_RIGHT)
        if shadow and sh is not None:
            s = sh.copy(); s.putalpha(Image.eval(s.getchannel("A"), lambda a: a // 2))
            put(s, x + im.width // 2 - 8, y + im.height - 6)
        put(im, x, y)
    actor("shrine", 0, 60, 18)
    actor("wizard", 0, 104, 64)
    for i, (x, y) in enumerate(((40, 80), (60, 96), (150, 90), (170, 70), (30, 50))):
        actor("slime", i, x, y)
    for i, (x, y) in enumerate(((180, 40), (200, 56), (140, 30))):
        actor("bat", i, x, y, shadow=False)
    for i, (x, y) in enumerate(((84, 120), (196, 120))):
        actor("skeleton", i, x, y, flipx=x > 120)
    actor("goblin", 1, 220, 92, flipx=True); actor("orc", 2, 0, 122)
    if "slime_king" in F:
        put(F["shadow_big"][0], 168, 2 + 50) if "shadow_big" in F else None
        actor("slime_king", 0, 152, -6, shadow=False)
    for k, (x, y) in enumerate(((128, 100), (92, 104), (136, 84), (70, 70), (210, 140))):
        put(F["gems"][(k % 3) * 2], x, y)
    put(F["coin"][0], 150, 110); put(F["chest"][0], 20, 30); put(F["heart"][0], 228 - 16, 10)
    put(F["fx_spark"][0], 132, 70); put(F["fx_spark"][1], 150, 76); put(F["fx_impact"][1], 160, 86)
    put(F["fx_poof"][2], 40, 100)
    for k in range(2):
        import math
        a = k * math.pi + 0.6
        put(F["fx_rune"][k], int(116 + 26 * math.cos(a)), int(76 + 26 * math.sin(a)))
    put(F["fx_frost"][2], 80, 60)
    put(F["fx_aura"][0], 170, 70)
    os.makedirs(out_dir, exist_ok=True)
    scene.save(os.path.join(out_dir, "contact_1x.png"))
    scene.resize((720, 480), Image.NEAREST).save(os.path.join(out_dir, "contact_3x.png"))
    print("contact sheet ->", out_dir)


if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "seed":
        seed(sys.argv[2:])
    elif cmd == "export":
        export()
    elif cmd == "contact":
        contact(sys.argv[2] if len(sys.argv) > 2 else os.path.join(RAW, "_contact"))
