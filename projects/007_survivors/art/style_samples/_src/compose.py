# Build 240x160 style mockups from PixelLab candidates (art/raw/style_samples) + procedural ground.
import os, sys
import numpy as np
from PIL import Image, ImageDraw
HERE = os.path.dirname(os.path.abspath(__file__))
RAW = os.path.join(HERE, "..", "..", "raw", "style_samples")
OUT = os.path.join(HERE, "..")
W, H = 240, 160
BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def hexc(h):
    return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))


def vnoise(seed, scale, octaves=3):
    rng = np.random.default_rng(seed)
    out = np.zeros((H, W))
    amp, tot = 1.0, 0.0
    for o in range(octaves):
        s = scale / (2 ** o)
        gw, gh = int(W / s) + 3, int(H / s) + 3
        g = rng.random((gh, gw))
        ys, xs = np.mgrid[0:H, 0:W] / s
        x0, y0 = xs.astype(int), ys.astype(int)
        fx, fy = xs - x0, ys - y0
        fx = fx * fx * (3 - 2 * fx)
        fy = fy * fy * (3 - 2 * fy)
        a = g[y0, x0] * (1 - fx) + g[y0, x0 + 1] * fx
        b = g[y0 + 1, x0] * (1 - fx) + g[y0 + 1, x0 + 1] * fx
        out += amp * (a * (1 - fy) + b * fy)
        tot += amp
        amp *= 0.5
    return out / tot


def ramp(cols, n, dither=True):
    t = (n - n.min()) / (n.max() - n.min() + 1e-9) * (len(cols) - 1)
    if dither:
        ys, xs = np.mgrid[0:H, 0:W]
        t = t + (BAYER[ys % 4, xs % 4] - 0.5) * 0.9
    idx = np.clip(np.round(t), 0, len(cols) - 1).astype(int)
    pal = np.array([hexc(c) for c in cols], dtype=np.uint8)
    return pal[idx]


def ground(style):
    if style == "A":
        img = ramp(["#121518", "#171b1d", "#1c2222", "#222826", "#29302c"], vnoise(1, 40))
        path = vnoise(7, 60, 2)
        m = np.abs(path - 0.5) < 0.05
        dirt = ramp(["#201d1b", "#28241f", "#302a24"], vnoise(3, 6, 2))
        img[m] = dirt[m]
    elif style == "B":
        img = ramp(["#2b5226", "#36652c", "#437a34", "#518c3c", "#62a046"], vnoise(2, 36))
        path = vnoise(9, 70, 2)
        d = np.abs(path - 0.5)
        m = d < 0.055
        dirt = ramp(["#6e4e32", "#86603c", "#9c7448", "#b08a58"], vnoise(4, 8, 2))
        img[m] = dirt[m]
        img[(d >= 0.055) & (d < 0.064)] = hexc("#4f3a26")
        rng = np.random.default_rng(5)
        for _ in range(260):
            x, y = int(rng.integers(1, W - 1)), int(rng.integers(2, H))
            if d[y, x] > 0.07:
                img[y, x] = hexc("#7cb852")
                if rng.random() < 0.5:
                    img[y - 1, x] = hexc("#7cb852")
    elif style == "C":
        img = np.zeros((H, W, 3), np.uint8)
        img[:] = hexc("#07060c")
        n = vnoise(3, 30)
        img[n > 0.62] = hexc("#0c0b16")
        rng = np.random.default_rng(8)
        for _ in range(140):
            img[int(rng.integers(0, H)), int(rng.integers(0, W))] = hexc("#161a2c")
        for _ in range(25):
            img[int(rng.integers(0, H)), int(rng.integers(0, W))] = hexc("#22304a")
    else:  # D: clean bold shapes, no noise texture
        img = np.zeros((H, W, 3), np.uint8)
        img[:] = hexc("#5fbf48")
        n = vnoise(11, 55, 1)
        img[n > 0.6] = hexc("#78d256")
        img[(n > 0.57) & (n <= 0.6)] = hexc("#6ac94f")
        img[n < 0.33] = hexc("#54ad41")
        path = vnoise(9, 70, 1)
        d = np.abs(path - 0.5)
        img[d < 0.07] = hexc("#c9a25a")
        img[d < 0.06] = hexc("#f0d088")
        rng = np.random.default_rng(4)
        for _ in range(40):
            x, y = int(rng.integers(2, W - 2)), int(rng.integers(2, H - 2))
            if d[y, x] > 0.09:
                img[y, x] = hexc(["#ffffff", "#ffe066", "#ff8fb0"][int(rng.integers(0, 3))])
                img[y + 1, x] = hexc("#3f8e33")
    return Image.fromarray(img, "RGB").convert("RGBA")


TINT = {}


def spr(folder, i, flip=False, half=False, maxc=15):
    im = Image.open(os.path.join(RAW, folder, f"{i:02d}.png")).convert("RGBA")
    a = np.array(im)
    a[a[..., 3] < 128] = 0
    a[a[..., 3] >= 128, 3] = 255
    im = Image.fromarray(a)
    im = im.crop(im.getbbox())
    if (folder, i) in TINT:
        a = np.array(im).astype(float)
        a[..., :3] = np.clip(a[..., :3] * np.array(TINT[(folder, i)]), 0, 255)
        im = Image.fromarray(a.astype(np.uint8))
    if half:
        im = im.resize((max(1, im.width // 2), max(1, im.height // 2)), Image.NEAREST)
    if flip:
        im = im.transpose(Image.FLIP_LEFT_RIGHT)
    alpha = im.getchannel("A")
    rgb = im.convert("RGB")
    cols = rgb.getcolors(100000)
    if len(cols) > maxc:  # 4bpp: 15 colours + transparent
        rgb = rgb.quantize(maxc, method=Image.MEDIANCUT, dither=Image.NONE).convert("RGB")
    out = rgb.convert("RGBA")
    out.putalpha(alpha)
    return out


RIM = {}


def rim(im, col):
    a = np.array(im)
    m = a[..., 3] > 0
    p = np.pad(m, 1)
    n = p[:-2, 1:-1] | p[2:, 1:-1] | p[1:-1, :-2] | p[1:-1, 2:]
    o = np.zeros((a.shape[0] + 2, a.shape[1] + 2, 4), np.uint8)
    ring = np.pad(n & ~m, 1) | (np.pad(m, 1) == 0) & (np.pad(np.pad(m, 1), 1)[:-2, 1:-1] | np.pad(np.pad(m, 1), 1)[2:, 1:-1] | np.pad(np.pad(m, 1), 1)[1:-1, :-2] | np.pad(np.pad(m, 1), 1)[1:-1, 2:])
    o[ring] = col
    o[1:-1, 1:-1][m] = a[m]
    return Image.fromarray(o)


def shadow(base, cx, by, w, col):
    ImageDraw.Draw(base).ellipse((cx - w // 2, by - 2, cx + w // 2, by + 2), fill=col)


def glow(base, cx, cy, r, cols):
    a = np.array(base)
    ys, xs = np.mgrid[0:H, 0:W]
    t = np.hypot(xs - cx, (ys - cy) * 1.2) / r + (BAYER[ys % 4, xs % 4] - 0.5) * 0.25
    for k, c in enumerate(cols):  # mimics a GBA additive blend layer
        m = t < (k + 1) / len(cols)
        a[m, :3] = np.clip(a[m, :3].astype(int) + np.array(hexc(c)), 0, 255)
        t = np.where(m, 9, t)
    return Image.fromarray(a)


def put(base, im, x, y):  # x, y = feet centre
    base.alpha_composite(im, (int(x - im.width // 2), int(y - im.height)))


ENEMY_POS = [(70, 52), (176, 122), (160, 42), (52, 120), (194, 86), (40, 86), (150, 146), (92, 146), (214, 156)]


def S(c, d, shadow, wiz, glowcols, enemies, gems, deco, props, hud, glow_r=20):
    return dict(c=c, d=d, shadow=shadow, wiz=wiz, glowcols=glowcols,
                enemies=[(i,) + p for i, p in zip(enemies, ENEMY_POS)],
                gems=[(i,) + p for i, p in zip(gems, [(104, 112), (138, 114), (84, 76), (180, 100), (122, 130)])],
                deco=deco, props=props, hud=hud, glow_r=glow_r)


STYLES = {
    "A": S("A_chars", "A_deco", (10, 12, 14, 255), 2, ["#30204a", "#1c1230", "#0c0818"],
           [6, 7, 9, 10, 12, 14, 16, 17, 20], [22, 23, 25, 22, 24],
           [(1, 30, 52), (5, 214, 46), (8, 36, 158)], [(30, 96, 30), (33, 132, 156), (35, 232, 124), (32, 150, 102), (31, 232, 72)],
           ("#101016", "#6a4aa8", "#a98be0")),
    "B": S("B_chars", "B_deco", (36, 66, 30, 255), 1, [],
           [6, 7, 9, 10, 12, 13, 16, 18, 19], [22, 23, 25, 24, 22],
           [(0, 30, 52), (4, 214, 46), (7, 36, 158)], [(30, 96, 30), (33, 132, 156), (34, 232, 124), (32, 150, 102), (31, 232, 72)],
           ("#2a1c14", "#3a7ad8", "#8cc8ff")),
    "C": S("C_chars", "C_deco", None, 1, ["#3a0a44", "#22062a", "#120316"],
           [6, 8, 9, 10, 12, 14, 17, 18, 20], [25, 25, 25, 25, 25],
           [(0, 30, 52), (4, 214, 46), (11, 36, 158)], [(30, 96, 30), (33, 132, 156), (34, 232, 124), (31, 232, 72)],
           ("#000000", "#e020c0", "#ff9cf0")),
    "D": S("D_chars", "D_deco", (62, 140, 50, 255), 0, [],
           [6, 8, 9, 11, 12, 14, 16, 17, 20], [22, 24, 25, 22, 24],
           [(1, 30, 52), (5, 214, 46), (10, 34, 158)], [(30, 96, 30), (33, 132, 156), (35, 232, 120), (31, 150, 102), (32, 232, 72)],
           ("#4a2a40", "#ff8a2a", "#ffe066")),
}


def build(st, bolt=(26, 152, 72), impact=(28, 192, 76)):
    C = STYLES[st]
    base = ground(st)
    wx, wy = 120, 94
    items = []
    for i, x, y in C["deco"]:
        items.append((y, spr(C["d"], i), x, y, 0))
    for i, x, y in C["props"]:
        items.append((y - 200, spr(C["c"], i), x, y, 0))
    for i, x, y in C["gems"]:
        items.append((y - 100, spr(C["c"], i, half=True), x, y, 0))
    for i, x, y in C["enemies"]:
        im = spr(C["c"], i, flip=(x > wx))
        if st in RIM:
            im = rim(im, RIM[st])
        items.append((y, im, x, y, im.width * 2 // 3))
    wim = spr(C["c"], C["wiz"])
    if st in RIM:
        wim = rim(wim, RIM[st])
    items.append((wy, wim, wx, wy, 14))
    if C["glowcols"]:
        base = glow(base, wx + 12, wy - 22, C["glow_r"], C["glowcols"])
        base = glow(base, impact[1], impact[2] - 10, 16, C["glowcols"])
        for i, x, y in C["gems"]:
            base = glow(base, x, y - 3, 7, C["glowcols"][:2])
    items.sort(key=lambda t: t[0])
    for fy, im, x, y, sw in items:
        if sw and C["shadow"]:
            shadow(base, x, y - 1, sw, C["shadow"])
        put(base, im, x, y)
    put(base, spr(C["c"], bolt[0]), bolt[1], bolt[2])
    put(base, spr(C["c"], impact[0]), impact[1], impact[2])
    o, f, hl = (hexc(c) for c in C["hud"])
    d = ImageDraw.Draw(base)
    d.rectangle((4, 3, 235, 8), fill=o)
    d.rectangle((5, 4, 150, 7), fill=f)
    d.line((5, 4, 150, 4), fill=hl)
    d.rectangle((wx - 9, wy + 2, wx + 9, wy + 5), fill=o)
    d.rectangle((wx - 8, wy + 3, wx + 4, wy + 4), fill=(230, 60, 60))
    img = base.convert("RGB")
    img.save(os.path.join(OUT, f"style_{st}.png"))
    img.resize((W * 3, H * 3), Image.NEAREST).save(os.path.join(OUT, f"style_{st}_3x.png"))


RIM["C"] = (52, 70, 110, 255)
TINT[("A_chars", 2)] = (1.12, 1.0, 1.22)
TINT[("D_chars", 22)] = (0.55, 0.85, 1.45)

if __name__ == "__main__":
    for st in (sys.argv[1:] or list("ABCD")):
        build(st)
