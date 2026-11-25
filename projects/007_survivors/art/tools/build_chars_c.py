"""Style C characters rebuilt FROM the approved C sample sprites (G-44 follow-up).

Sources (art/raw/style_c2/):
  first/*.png       the C sample sprites (art/raw/style_samples/C_chars) trimmed to the game frame size
  wendel_c/ skel_c/ PixelLab characters made from those first frames (template walks/idle/death; side clips are
                    v3 animations started from C sample 04, the side-facing wizard; hurt = idle shaken 1 px)
  slime_c/ bat_c/   PixelLab objects made from those first frames (v3 animations)
Every colour is snapped to art/palette_c.json (the master palette extracted from style_samples/style_C.png),
then each sprite is limited to the few master colours its C sample uses (NES-like restraint, hard edges).

    python -I art/tools/build_chars_c.py           # writes wizard, wizard_ghost, slime, bat, skeleton, goblin, orc
    python -I art/tools/build_chars_c.py goblin orc  # only these
    python -I art/tools/build_chars_c.py --sheet D # also a 3x preview sheet in folder D
PISA esos .aseprite: no lo ejecutes si alguien los ha editado a mano.
"""
import glob
import json
import os
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import aseprite_io  # noqa: E402

ART = os.path.dirname(HERE)
RAW = os.path.join(ART, "raw", "style_c2")
CS = os.path.join(ART, "raw", "style_samples", "C_chars")


def hexc(h):
    return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))


MASTER = np.array([hexc(c) for c in json.load(open(os.path.join(ART, "palette_c.json")))["master"]], np.int32)


RIM_ENEMY = hexc("#34466e")   # cold 1 px rim of the style C enemies (master colour)
RIM_WIZ = hexc(os.environ.get("WIZ_RIM", "#8ca4b4"))   # 1 px rim around Wendel, same in every direction


def lum(c):
    return 0.3 * c[0] + 0.59 * c[1] + 0.11 * c[2]


def snap(rgb, pal):
    d = ((rgb[..., None, :].astype(np.int32) - pal[None]) ** 2).sum(-1)
    return d.argmin(-1)


def rgba(path):
    return np.array(Image.open(path).convert("RGBA"))


def subset_from(sources, max_cols):
    """Master colours used by the C sample(s), most used first, at most max_cols."""
    counts = {}
    for a in sources:
        m = a[..., 3] > 0
        ix = snap(a[m][:, :3], MASTER)
        for i in ix:
            counts[int(i)] = counts.get(int(i), 0) + 1
    import colorsys

    def neon(i):
        r, g, b = (v / 255 for v in MASTER[i])
        _, s, v = colorsys.rgb_to_hsv(r, g, b)
        return s > 0.55 and v > 0.6   # glow accents (eyes, runes, staff tip): always kept, even if tiny
    glow = [i for i in counts if neon(i) and counts[i] >= 2]
    rest = [i for i in sorted(counts, key=lambda i: -counts[i]) if i not in glow]
    order = glow + rest[:max(0, max_cols - len(glow))]
    return sorted([tuple(MASTER[i]) for i in order], key=lum)


def quantize(frames, sub):
    """RGBA frames -> index frames on [transparent] + sub (alpha < 128 = transparent)."""
    P = np.array(sub, np.int32)
    out = []
    for f in frames:
        ix = snap(f[..., :3], P) + 1
        ix[f[..., 3] < 128] = 0
        out.append(ix.astype(np.uint8))
    return out


def frames_of(folder):
    return [rgba(p) for p in sorted(glob.glob(os.path.join(folder, "*.png")))]


def pick(frames, n):
    """n frames evenly from a clip (keeps the order)."""
    if len(frames) == n:
        return list(frames)
    return [frames[round(i * (len(frames) - 1) / max(1, n - 1))] if n > 1 else frames[0] for i in range(n)] \
        if n > len(frames) else [frames[int(i * len(frames) / n)] for i in range(n)]


def fit(frames, w, h, bottom, center_x=None):
    """One offset for the whole clip: union bbox bottom -> row `bottom`, centred on x. Returns frames w x h."""
    ys, xs = [], []
    for f in frames:
        m = f[..., 3] >= 128
        if m.any():
            yy, xx = np.nonzero(m)
            ys += [yy.min(), yy.max()]
            xs += [xx.min(), xx.max()]
    y1 = max(ys)
    cx = (min(xs) + max(xs) + 1) / 2 if center_x is None else center_x
    dx = int(round(w / 2 - cx))
    dy = bottom - y1
    out = []
    lost = 0
    for f in frames:
        o = np.zeros((h, w, 4), np.uint8)
        fh, fw = f.shape[:2]
        for y in range(fh):
            ty = y + dy
            for x in range(fw):
                if f[y, x, 3] < 128:
                    continue
                tx = x + dx
                if 0 <= tx < w and 0 <= ty < h:
                    o[ty, tx] = f[y, x]
                else:
                    lost += 1
        out.append(o)
    if lost:
        print(f"    fit {w}x{h}: {lost} px clipped")
    return out


def vsquash(f, k, bottom=None):
    """Nearest-neighbour vertical squash to k * height, kept on the same bottom row (hard pixels)."""
    h, w = f.shape[:2]
    m = f[..., 3] >= 128
    yy = np.nonzero(m.any(1))[0]
    y0, y1 = yy.min(), yy.max()
    src_h = y1 - y0 + 1
    nh = max(1, int(round(src_h * k)))
    o = np.zeros_like(f)
    b = y1 if bottom is None else bottom
    for i in range(nh):
        sy = y0 + int(i * src_h / nh)
        o[b - nh + 1 + i] = f[sy]
    return o


def thicken(f):
    """Side-view bones are 1 px thin: grow every opaque run 1 px to the left (G-44 review: read like the front)."""
    o = f.copy()
    m = f[..., 3] >= 128
    grow = ~m[:, :-1] & m[:, 1:]
    src = f[:, 1:][grow].copy()
    o[:, :-1][grow] = src            # bones twice as wide, same colour (the front view reads 2 px limbs)
    rgb = o[..., :3].astype(np.int32)
    purple = (o[..., 3] >= 128) & (rgb[..., 2] > rgb[..., 1] + 25) & (rgb.sum(-1) < 400)
    o[purple, :3] = (75, 135, 135)   # template cloth -> the teal rib shade of the front view (#4b8787)
    return o


def flat(f, col):
    o = f.copy()
    m = o[..., 3] >= 128
    o[m, :3] = col
    return o


def rim(f, col):
    m = f[..., 3] >= 128
    p = np.pad(m, 1)
    n = p[:-2, 1:-1] | p[2:, 1:-1] | p[1:-1, :-2] | p[1:-1, 2:]
    o = f.copy()
    o[n & ~m, :3] = col
    o[n & ~m, 3] = 255
    return o


def scatter(f, keep, seed):
    rng = np.random.default_rng(seed)
    o = f.copy()
    m = o[..., 3] >= 128
    drop = m & (rng.random(m.shape) > keep)
    o[drop, 3] = 0
    return o


def write(name, frames, durs, tags, sub):
    h, w = frames[0].shape[:2]
    idx = quantize(frames, sub)
    pal = [(0, 0, 0)] + list(sub)
    aseprite_io.write_aseprite(os.path.join(ART, name + ".aseprite"), w, h, pal, [f.tolist() for f in idx], durs,
                               tags)
    print(f"{name:13s} {w}x{h} x{len(frames)} colours {len(sub)}")
    return idx, pal


def anim(char, name, direction):
    d = glob.glob(os.path.join(RAW, char, "*", "animations", name, direction))
    return frames_of(d[0])


def build():
    out = {}
    # ---------------- Wendel (C sample 00) ------------------------------------------------------------------------
    sub = subset_from([rgba(os.path.join(CS, "00.png")), rgba(os.path.join(CS, "02.png"))], 9)
    if RIM_WIZ not in sub:
        sub = sorted(sub + [RIM_WIZ], key=lum)
    # wendel_v3 = the same PixelLab character; S/N = template clips (full size), SE/NE = v3 clips from the rotations,
    # E = v3 clips from the east rotation with the staff drawn in (raw/style_c2/wendel_v3/Idle/east_staff.png).
    # Every direction has the same height (30-31 px) and gets the same 1 px rim.
    W = "wendel_v3"
    clips = [("idle", anim(W, "idle", "south"), 4, 180),
             ("idle_side", anim(W, "idle_e_staff", "east"), 4, 180),
             ("walk_down", anim(W, "walk", "south"), 6, 100),
             ("walk_side", anim(W, "walk_e_staff", "east"), 6, 100),
             ("walk_up", anim(W, "walk", "north"), 6, 100),
             ("hurt", [np.roll(anim(W, "idle", "south")[0], k, 1) for k in (-1, 1)], 2, 80),
             ("death", anim(W, "death", "south"), 7, 130),
             # 8-direction additions (appended so frames 0-34 keep their old meaning; W/SW/NW = horizontal flip)
             ("idle_se", anim(W, "idle_v3", "south-east"), 4, 180),
             ("idle_ne", anim(W, "idle_v3", "north-east"), 4, 180),
             ("idle_n", anim(W, "idle", "north"), 4, 180),
             ("walk_se", anim(W, "walk_v3", "south-east"), 6, 100),
             ("walk_ne", anim(W, "walk_v3", "north-east"), 6, 100)]
    rename = {"idle": "idle_s", "idle_side": "idle_e", "walk_down": "walk_s", "walk_side": "walk_e",
              "walk_up": "walk_n"}
    frames, durs, tags = [], [], []
    for tag, fr, n, ms in clips:
        fr = fit(pick(fr, n), 32, 32, 30)
        fr = [rim(f, RIM_WIZ) for f in fr]
        tags.append((rename.get(tag, tag), len(frames), len(frames) + n - 1))
        frames += fr
        durs += [ms] * n
    out["wizard"] = (frames, durs, tags, sub)
    ghost_sub = [hexc("#341d60"), hexc("#ff3838")]
    ghost = [rim(flat(np.where((f[..., 3:] >= 128) & (np.arange(32)[None, :, None] > -1), f, 0).astype(np.uint8),
                      ghost_sub[0]), ghost_sub[1]) if (f[..., 3] >= 128).any() else f for f in frames]
    # keep the ghost inside the frame: shrink the rim where it touches the border (rim() never grows past it)
    out["wizard_ghost"] = (ghost, durs, tags, ghost_sub)

    # ---------------- slime (C sample 06) -------------------------------------------------------------------------
    sub = subset_from([rgba(os.path.join(CS, "06.png"))], 7)
    hop = fit(frames_of(glob.glob(os.path.join(RAW, "slime_c", "animations", "*", "unknown"))[0]), 16, 16, 15)
    base = hop[0]
    light = max(sub, key=lum)
    death = [flat(base, light), vsquash(base, 0.6), scatter(vsquash(base, 0.35), 0.7, 1),
             scatter(vsquash(base, 0.2), 0.35, 2)]
    out["slime"] = (hop + death, [120] * 4 + [70] * 4, [("hop", 0, 3), ("death", 4, 7)], sub)

    # ---------------- bat + skeleton share one undead palette (C samples 09, 12) ----------------------------------
    sub = subset_from([rgba(os.path.join(CS, "09.png")), rgba(os.path.join(CS, "12.png"))], 15)
    light = max(sub, key=lum)
    fly = frames_of(glob.glob(os.path.join(RAW, "bat_c", "animations", "*", "unknown"))[0])
    fly = fit(fly, 32, 16, 15)
    b0 = fly[0]
    bdeath = [flat(b0, light), np.roll(vsquash(b0, 0.5), 0, 0), scatter(vsquash(b0, 0.3), 0.4, 3)]
    out["bat"] = (fly + bdeath, [100] * 4 + [80] * 3, [("fly", 0, 3), ("death", 4, 6)], sub)

    walk = fit([thicken(f) for f in pick(anim("skel_c", "walk", "east"), 4)], 16, 32, 31)
    sd = anim("skel_c", "death", "east")
    sd = fit(sd, 16, 32, 31)
    frames, tags = walk + sd, [("walk_e", 0, 3), ("death", 4, 7)]
    for d, tag in (("south", "walk_s"), ("south-east", "walk_se"), ("north-east", "walk_ne"), ("north", "walk_n")):
        tags.append((tag, len(frames), len(frames) + 3))
        frames += fit(pick(anim("skel_c", "walk", d), 4), 16, 32, 31)
    out["skeleton"] = (frames, [120] * 4 + [90] * 4 + [120] * 16, tags, sub)

    # ---------------- goblin + orc share the greenskin palette (C samples 16-18 goblin, 19-21 orc) ---------------
    # PixelLab v3 characters made from C 16 (goblin_c) and C 20 (orc_c); same tag layout as the skeleton.
    sub = subset_from([rgba(os.path.join(CS, "%02d.png" % i)) for i in range(16, 22)], 14)
    if RIM_ENEMY not in sub:
        sub = sorted(sub + [RIM_ENEMY], key=lum)
    for name, char, bottom in (("goblin", "goblin_c", 30), ("orc", "orc_c", 31)):
        walk = [rim(f, RIM_ENEMY) for f in fit(pick(anim(char, "walk", "east"), 4), 32, 32, bottom)]
        death = [rim(f, RIM_ENEMY) for f in fit(pick(anim(char, "death", "east"), 4), 32, 32, bottom)]
        frames, tags = walk + death, [("walk_e", 0, 3), ("death", 4, 7)]
        for d, tag in (("south", "walk_s"), ("south-east", "walk_se"), ("north-east", "walk_ne"), ("north", "walk_n")):
            tags.append((tag, len(frames), len(frames) + 3))
            frames += [rim(f, RIM_ENEMY) for f in fit(pick(anim(char, "walk", d), 4), 32, 32, bottom)]
        out[name] = (frames, [120] * 4 + [90] * 4 + [120] * 16, tags, sub)
    return out


def sheet(out, folder):
    os.makedirs(folder, exist_ok=True)
    rows = []
    for name, (frames, _, _, sub) in out.items():
        idx = quantize(frames, sub)
        pal = np.array([(7, 6, 12)] + list(sub), np.uint8)
        rows.append(np.concatenate([np.pad(pal[i], ((1, 1), (1, 1), (0, 0)), constant_values=7) for i in idx], 1))
    W = max(r.shape[1] for r in rows)
    img = np.concatenate([np.pad(r, ((0, 0), (0, W - r.shape[1]), (0, 0)), constant_values=7) for r in rows], 0)
    Image.fromarray(img).resize((img.shape[1] * 3, img.shape[0] * 3), Image.NEAREST).save(
        os.path.join(folder, "chars_c_3x.png"))


def main(argv):
    out = build()
    only = [a for a in argv if not a.startswith("-") and a in out]   # e.g. "goblin orc": write only these
    for name, (frames, durs, tags, sub) in out.items():
        if not only or name in only:
            write(name, frames, durs, tags, sub)
    if "--sheet" in argv:
        sheet(out, argv[argv.index("--sheet") + 1])


if __name__ == "__main__":
    main(sys.argv[1:])
