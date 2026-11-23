"""Builds the review scenes of lab_delve_art from 006_delve's exported art.

Reads ../006_delve/art/export/tex_*.bmp, ../006_delve/graphics/{ui_frame,mon_*,prop_*}.bmp and writes
graphics/scene_N.bmp (palette_bitmap 240x160) + copies of the sprites (with their .json) into graphics/.
The 3D mock uses a tiny raycaster with distance darkening, as a stand-in for the engine's build_view.py.
Run: python -I projects/lab_delve_art/tools/make_scenes.py
"""
import json
import math
import os
import shutil

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
LAB = os.path.dirname(HERE)
DELVE = os.path.join(os.path.dirname(LAB), "006_delve")
EXP = os.path.join(DELVE, "art", "export")
DGFX = os.path.join(DELVE, "graphics")
GFX = os.path.join(LAB, "graphics")

ui = Image.open(os.path.join(DGFX, "ui_frame.bmp"))
UI = [list(ui.get_flattened_data())[y * 240:(y + 1) * 240] for y in range(160)]
PAL = [tuple(ui.getpalette()[i * 3:i * 3 + 3]) for i in range(256)]


def tex(name):
    im = Image.open(os.path.join(EXP, name + ".bmp"))
    d = list(im.get_flattened_data())
    pal = [tuple(im.getpalette()[i * 3:i * 3 + 3]) for i in range(64)]
    for i, c in enumerate(pal):
        if i < 64 and c != (0, 0, 0):
            assert PAL[i] == c or PAL[i] == (0, 0, 0), name
            PAL[i] = c
    return [d[y * im.width:(y + 1) * im.width] for y in range(im.height)]


TEX = {n[:-4]: tex(n[:-4]) for n in os.listdir(EXP) if n.endswith(".bmp")}
ENV_N = max(i for i in range(64) if PAL[i] != (0, 0, 0)) + 1
_dark_cache = {}


def darken(idx, f):
    key = (idx, round(f, 2))
    if key not in _dark_cache:
        r, g, b = (c * f for c in PAL[idx])
        _dark_cache[key] = min(range(ENV_N), key=lambda j: (PAL[j][0] - r) ** 2 * 3 + (PAL[j][1] - g) ** 2 * 4
                               + (PAL[j][2] - b) ** 2 * 2)
    return _dark_cache[key]


def blank(c=0):
    return [[c] * 240 for _ in range(160)]


def overlay_ui(img):
    for y in range(160):
        for x in range(240):
            if UI[y][x]:
                img[y][x] = UI[y][x]


def blit(img, t, x0, y0, w=None, h=None, key0=False):
    h = h or len(t)
    w = w or len(t[0])
    for y in range(h):
        for x in range(w):
            if 0 <= x0 + x < 240 and 0 <= y0 + y < 160:
                v = t[y][x]
                if key0 and v == 0:
                    continue
                img[y0 + y][x0 + x] = v


def raycast(walls, floor="tex_floor", ceil="tex_ceiling", W=176, H=120, f=88.0):
    """walls: {(cx, cz): texture} solid cells; player at cell (0,0) centre, looking +z. Cell size 1, height 1."""
    img = [[0] * W for _ in range(H)]
    for sy in range(H):
        for sx in range(W):
            dx, dy, dz = (sx + 0.5 - W / 2) / f, (sy + 0.5 - H / 2) / f, 1.0
            # DDA through the grid (cell centres at integers, faces at .5)
            px, pz = 0.5, 0.5                      # shift so faces are at integers
            cx, cz = 0, 0
            step_x = 1 if dx > 0 else -1
            tdx = abs(1 / dx) if dx else 1e9
            tdz = 1.0
            tx = ((1 - px) if dx > 0 else px) * tdx if dx else 1e9
            tz = (1 - pz) * tdz
            hit, t_hit, side = None, None, None
            for _ in range(12):
                if tx < tz:
                    cx += step_x
                    t_hit, tx, side = tx, tx + tdx, "x"
                else:
                    cz += 1
                    t_hit, tz, side = tz, tz + tdz, "z"
                if (cx, cz) in walls:
                    hit = walls[(cx, cz)]
                    break
            t_plane = 0.5 / abs(dy) if dy else 1e9
            color = 0
            if hit and t_hit < t_plane:
                y_at = 0.5 + dy * t_hit           # 0 top .. 1 bottom
                if side == "z":
                    u = (px + dx * t_hit) % 1.0
                else:
                    u = (pz + t_hit) % 1.0
                    if step_x < 0:
                        u = 1 - u
                t = TEX[hit]
                v = t[min(63, int(y_at * 64))][min(63, int(u * 64))]
                if v == 0 and hit in ("tex_bars", "tex_arch"):
                    v = 0                           # (no see-through in this mock)
                color = darken(v, max(0.18, 1.12 - 0.25 * t_hit)) if v else 0
                if side == "x":
                    color = darken(color, 0.85) if color else 0
            elif t_plane < 20:
                wx, wz = px + dx * t_plane, pz + t_plane
                t = TEX[floor if dy > 0 else ceil]
                v = t[int((wz % 1.0) * 64)][int((wx % 1.0) * 64)]
                color = darken(v, max(0.18, 1.12 - 0.25 * t_plane))
            img[sy][sx] = color
    return img


def corridor():
    walls = {}
    for z in range(0, 4):
        walls[(-1, z)] = "tex_wall_stone"
        walls[(1, z)] = "tex_wall_stone"
    walls[(-1, 1)] = "tex_wall_torch"
    walls[(1, 2)] = "tex_wall_lever_up"
    walls[(-1, 2)] = "tex_wall_banner"
    walls[(0, 3)] = "tex_door"
    return walls


def room():
    walls = {}
    for z in range(0, 5):
        walls[(-2, z)] = "tex_wall_stone"
        walls[(2, z)] = "tex_wall_stone"
    for x in range(-2, 3):
        walls[(x, 4)] = "tex_wall_stone"
    walls[(0, 4)] = "tex_stairs_down"
    walls[(-1, 4)] = "tex_wall_torch"
    walls[(1, 4)] = "tex_wall_skulls"
    return walls


def save(name, img):
    im = Image.new("P", (240, 160))
    flat = []
    for c in PAL:
        flat += list(c)
    im.putpalette(flat)
    im.putdata([v for r in img for v in r])
    im.save(os.path.join(GFX, name + ".bmp"))
    with open(os.path.join(GFX, name + ".json"), "w") as f:
        json.dump({"type": "palette_bitmap"}, f)


def sheet(names):
    img = blank()
    for k, n in enumerate(names):
        t = TEX[n]
        blit(img, t[:64], 8 + (k % 3) * 80, 8 + (k // 3) * 80)
    return img


def main():
    os.makedirs(GFX, exist_ok=True)
    # 0: ui_frame + wall tiled at 1x (seams)
    img = blank()
    for ty in range(2):
        for tx in range(3):
            blit(img, TEX["tex_wall_stone"], tx * 64, ty * 64)
    overlay_ui(img)
    save("scene_0", img)
    # 1-3: every texture at 1x
    order = ["tex_wall_stone", "tex_door", "tex_door_open", "tex_floor", "tex_ceiling", "tex_stairs_down",
             "tex_wall_torch", "tex_bars", "tex_wall_lever_up", "tex_wall_lever_down", "tex_wall_chains",
             "tex_wall_crack", "tex_wall_dart", "tex_wall_banner", "tex_wall_skulls", "tex_arch"]
    for s in range(3):
        save("scene_%d" % (s + 1), sheet(order[s * 6:s * 6 + 6]))
    # 4: corridor mock, 5: room mock (monsters go on top as sprites)
    for k, walls in ((4, corridor()), (5, room())):
        img = blank()
        blit(img, raycast(walls), 0, 0)
        overlay_ui(img)
        save("scene_%d" % k, img)
    # 6: flat dark backdrop for the scale grids
    img = blank(TEX["tex_floor"][40][40])
    for y in range(80, 160):
        for x in range(240):
            img[y][x] = TEX["tex_wall_stone"][5][5] if x >= 120 else img[y][x]
    save("scene_6", img)
    # 7: stone wall tiled at 1x (wall decorations at several scales), 8: plain corridor for the deco sprites
    img = blank()
    for ty in range(3):
        for tx in range(4):
            blit(img, TEX["tex_wall_stone"], tx * 64, ty * 64)
    save("scene_7", img)
    walls = {}
    for z in range(0, 4):
        walls[(-1, z)] = "tex_wall_stone"
        walls[(1, z)] = "tex_wall_stone"
    walls[(0, 3)] = "tex_wall_stone"
    img = blank()
    blit(img, raycast(walls), 0, 0)
    overlay_ui(img)
    save("scene_8", img)
    for n in os.listdir(DGFX):
        if n.startswith(("mon_", "prop_", "deco_")) and n.endswith(".bmp"):
            shutil.copy(os.path.join(DGFX, n), os.path.join(GFX, n))
            h = 32 if n.startswith("prop_") else 64
            with open(os.path.join(GFX, n[:-4] + ".json"), "w") as f:
                json.dump({"type": "sprite", "height": h}, f)
    print("scenes ok, env colours", ENV_N)


if __name__ == "__main__":
    main()
