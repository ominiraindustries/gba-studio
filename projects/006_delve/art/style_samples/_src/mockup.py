"""Compose the 240x160 mockups from build_view.build_pose (resting view) + sprites + HUD A frame."""
import importlib
import os
import sys

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import gen  # noqa: E402

sys.path.insert(1, gen.MOCK_SRC)
import make_mockups as MM  # noqa: E402

OPEN = {(0, 0), (1, 0), (2, 0), (3, 0), (1, 1), (1, 2), (2, 1), (2, 2), (-1, 0)}
DOOR_EDGE = ("z", 3, 0)          # far end of the corridor (lost in the dark)
TORCH_EDGE = ("x", 1, -1)        # left wall of (1, 0); the right side of (1, 0) opens into a branch


def edge_of(c):
    k, d, x = c["kind"], c["d"], c["x"]
    if k == 0:
        return ("z", d, x)
    if k == 2:
        return ("z", d - 1, x)
    if k == 1:
        return ("x", d, x)
    return ("x", d, x - 1)


def is_wall(e):
    a, d, x = e
    c1, c2 = ((d, x), (d + 1, x)) if a == "z" else ((d, x), (d, x + 1))
    return (c1 in OPEN) != (c2 in OPEN)


def vertex_wall(vd, vx):
    es = [("z", vd - 1, vx - 1), ("z", vd - 1, vx), ("x", vd - 1, vx - 1), ("x", vd, vx - 1)]
    return any(is_wall(e) for e in es)


def paste(dst, piece):
    if piece is None:
        return
    x0, y0, img = piece
    h, w = img.shape[:2]
    sub = dst[y0:y0 + h, x0:x0 + w]
    m = img[..., 3] > 0
    sub[m] = img[m][:, :3]


def render(name, v, fade=0.42):
    os.environ["DL_PROJ"] = v["mirror"]
    if "build_view" in sys.modules:
        bv = importlib.reload(sys.modules["build_view"])
    else:
        import build_view as bv
    geo = bv.Geo(bv.parse_geometry())
    if fade is not None:
        bv.FADE = fade
    bv.make_locked_placeholder()
    sets = list(bv.SETS) + [("wall_torch", "tex_wall_torch", 0, None, True)]

    def tex(stem):
        return bv.frame(bv.load_texture(stem)[0], 0)
    stone = tex("tex_wall_stone")
    cap = bv.cap_texture(stone)
    floor, ceil = tex("tex_floor"), tex("tex_ceiling")
    set_tex = []
    for (_n, stem, _k, opening, _q) in sets:
        t = tex(stem.split("#")[0])
        if opening:
            mask = bv.build_mask(opening, t)
            ft = t.copy()
            ft[:64][mask, 3] = 0
            set_tex.append((ft, mask, bv.OPENINGS[opening][2], t))
        else:
            set_tex.append((t, None, None, t))
    order, bgs = bv.build_pose(geo, bv.Pose(geo, 0.0, 0.0, 0.0), set_tex, stone, cap, floor, ceil)
    pieces = [r for c in order for r in c["pieces"] if r is not None] + list(bgs)
    pal, _ = bv.quantize(pieces)
    view = bgs[0][2][..., :3].copy()
    names = [s[0] for s in sets]
    for c in order:
        if c["kind"] == 4:
            if vertex_wall(c["d"], c["x"]):
                paste(view, c["pieces"][0])
            continue
        e = edge_of(c)
        if not is_wall(e):
            continue
        if e == DOOR_EDGE:
            si = names.index("door_closed")
        elif e == TORCH_EDGE:
            si = names.index("wall_torch")
        else:
            si = (hash(e) & 3) if True else 0
            si = [0, 1, 2, 3][(e[1] * 7 + e[2] * 3 + (e[0] == "x")) % 4]
        paste(view, c["pieces"][si])
    # torch glow (the game adds a soft additive glow sprite): flame at X=-0.5, Z=2.075, Y=0.5-10/64
    F = geo.F
    z = geo.near(1) / 10.0 + 0.5
    fx, fy = geo.vp_x + F * -0.5 / z, geo.vp_y - F * (0.5 - 10 / 64) / z
    yy, xx = np.mgrid[0:geo.view_h, 0:geo.view_w]
    r = np.hypot((xx - fx) / 1.0, (yy - fy) / 1.15)
    g = np.clip(1 - r / 40.0, 0, 1) ** 2
    view += g[..., None] * np.array([84, 40, 12])
    # skeleton at d = 1: z = cell centre - 0.2, 64 texels = 1 wall height
    sk, cols = v["sk"]
    zs = geo.P + geo.back - 0.2
    sc = geo.F / zs / 64.0
    sz = int(round(64 * sc))
    ski = np.array(Image.fromarray(sk.astype(np.uint8)).resize((sz, sz), Image.NEAREST))
    rgb = np.array(cols, float)[ski] * min(1.0, bv.shade(zs) * 1.25)
    feet = geo.vp_y + geo.F * 0.5 / zs
    x0, y0 = int(round(geo.vp_x - sz / 2)), int(round(feet - sz))
    m = ski > 0
    sub = view[y0:y0 + sz, x0:x0 + sz]
    sub[m] = rgb[m]
    # snap to the generated 5-bit view palette (what the GBA would show)
    p8 = pal.astype(float) * 255 / 31
    flatv = np.clip(view, 0, 255).reshape(-1, 3)
    out = np.zeros_like(flatv)
    for i in range(0, len(flatv), 4096):
        blk = flatv[i:i + 4096]
        k = ((blk[:, None, :] - p8[None, :, :]) ** 2).sum(-1).argmin(1)
        out[i:i + 4096] = p8[k]
    view = out.reshape(view.shape)
    # sprite pixels keep their own (sprite) palette
    sub = view[y0:y0 + sz, x0:x0 + sz]
    sub[m] = np.round(rgb[m] / 8) * 8
    # HUD A frame on top
    img = Image.new("RGBA", (240, 160), (0, 0, 0, 255))
    img.paste(Image.fromarray(np.clip(view, 0, 255).astype(np.uint8), "RGB"), (0, 0))
    ui = Image.open(os.path.join(gen.DL, "graphics", "ui_frame.bmp"))
    uia = np.array(ui)
    uirgba = np.array(ui.convert("RGBA"))
    uirgba[uia == 0, 3] = 0
    img.alpha_composite(Image.fromarray(uirgba, "RGBA"))
    MM.compass_ribbon(img, 208, 6, "S")
    MM.text(img, 208, 17, "FLOOR 1", MM.WHITE, "center")
    img.paste(MM.real_minimap(), (178, 31))
    MM.icon(img, 181, 97, MM.KEY)
    MM.text(img, 197, 101, "x1", MM.WHITE)
    MM.icon(img, 5, 124, MM.DAGGER, 2)
    MM.bar(img, (4, 152, 38, 157), 0.7, MM.GOLD, (255, 230, 160), (40, 32, 20))
    MM.text(img, 46, 127, "HP", MM.RED_HI)
    MM.bar(img, (60, 127, 156, 135), 24 / 30, MM.RED, MM.RED_HI, MM.RED_EMPTY)
    MM.text(img, 160, 127, "24/30", MM.WHITE)
    MM.icon(img, 200, 126, MM.POTION)
    MM.text(img, 217, 127, "x3", MM.WHITE)
    MM.text(img, 217, 136, "B", MM.GOLD)
    MM.text(img, 46, 149, "A SKELETON BLOCKS THE WAY", MM.WHITE)
    out = os.path.join(gen.OUTROOT, name)
    img = img.convert("RGB")
    img.save(os.path.join(out, "mockup.png"))
    img.resize((720, 480), Image.NEAREST).save(os.path.join(out, "mockup_3x.png"))
    print(name, "build_view palette:", len(pal) - 1, "colours (of 191)")
    return img


def main():
    vs = gen.main()
    only = sys.argv[1:]
    shots = []
    for name, v in vs.items():
        if only and name not in only:
            continue
        shots.append(render(name, v))
    # side-by-side contact sheet (2x)
    if len(shots) > 1:
        sheet = Image.new("RGB", (480 * len(shots) + 8 * (len(shots) - 1), 320), (40, 40, 40))
        for i, s in enumerate(shots):
            sheet.paste(s.resize((480, 320), Image.NEAREST), (i * 488, 0))
        sheet.save(os.path.join(HERE, "sheet.png"))
        sheet.save(os.path.join(gen.OUTROOT, "compare_2x.png"))


if __name__ == "__main__":
    main()
