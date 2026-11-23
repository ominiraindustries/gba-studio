"""Monster (64x64) and floor prop (32x32) seeds for DELVE: 16 colours, index 0 transparent, vertical strips.

Monsters come from PixelLab concepts in concepts/ (generated inside the subscription), reduced to 15 colours,
given a 2 px dark outline that survives the hardware scale (0.55 / 0.35) and animated by hand-made transforms:
  idle (2): base, breath (upper body 1 px down)
  attack (2): anticipation (rears up, 1.06 tall), strike (lunges, 1.12 bigger)
  hit (1): recoils 0.94 and flashes lighter.
"""
import math
import os

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")
CONCEPTS = os.path.join(HERE, "concepts")


def lum(c):
    return 0.299 * c[0] + 0.587 * c[1] + 0.114 * c[2]


def load_quantized(path, colors=14, tweak=None):
    im = Image.open(path).convert("RGBA")
    if tweak:
        im = tweak(im)
    w, h = im.size
    px = im.load()
    opaque = [(x, y) for y in range(h) for x in range(w) if px[x, y][3] >= 128]
    strip = Image.new("RGB", (len(opaque), 1))
    strip.putdata([px[x, y][:3] for (x, y) in opaque])
    q = strip.quantize(colors=colors, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    qd = list(q.get_flattened_data()) if hasattr(q, "get_flattened_data") else list(q.getdata())
    colors = max(qd) + 1
    qpal = q.getpalette()[:colors * 3]
    pal = [tuple(qpal[i * 3:i * 3 + 3]) for i in range(colors)]
    order = sorted(range(colors), key=lambda i: lum(pal[i]))
    remap = {old: new + 2 for new, old in enumerate(order)}      # 1 = outline black, 2.. = colours dark->light
    pal_sorted = [pal[i] for i in order]
    grid = [[0] * w for _ in range(h)]
    for k, (x, y) in enumerate(opaque):
        grid[y][x] = remap[qd[k]]
    palette = [(255, 0, 255), (12, 8, 10)] + pal_sorted
    return grid, palette


def add_outline(grid, idx=1):
    h, w = len(grid), len(grid[0])
    out = [r[:] for r in grid]
    for y in range(h):
        for x in range(w):
            if grid[y][x] == 0:
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    xx, yy = x + dx, y + dy
                    if 0 <= xx < w and 0 <= yy < h and grid[yy][xx] != 0:
                        out[y][x] = idx
                        break
    return out


def darken_edges(grid, palette, idx=1):
    """Silhouette pixels that are already dark become the outline colour (cleaner 2 px rim)."""
    h, w = len(grid), len(grid[0])
    out = [r[:] for r in grid]
    for y in range(h):
        for x in range(w):
            v = grid[y][x]
            if v > 1 and lum(palette[v]) < 40:
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    xx, yy = x + dx, y + dy
                    if not (0 <= xx < w and 0 <= yy < h) or grid[yy][xx] in (0, 1):
                        out[y][x] = idx
                        break
    return out


def place(grid, size, bottom_margin=1):
    """Centre horizontally and stand on the bottom of a size x size canvas."""
    h, w = len(grid), len(grid[0])
    xs = [x for y in range(h) for x in range(w) if grid[y][x]]
    ys = [y for y in range(h) for x in range(w) if grid[y][x]]
    x0, x1, y0, y1 = min(xs), max(xs), min(ys), max(ys)
    out = [[0] * size for _ in range(size)]
    ox = (size - (x1 - x0 + 1)) // 2 - x0
    oy = size - bottom_margin - 1 - y1
    for y in range(h):
        for x in range(w):
            if grid[y][x] and 0 <= x + ox < size and 0 <= y + oy < size:
                out[y + oy][x + ox] = grid[y][x]
    return out


def scale_about_feet(grid, sx, sy, dx=0, dy=0):
    h, w = len(grid), len(grid[0])
    cx, fy = w / 2, h - 1
    out = [[0] * w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            u = (x + 0.5 - dx - cx) / sx + cx
            v = (y + 0.5 - dy - fy - 1) / sy + fy + 1
            ui, vi = int(math.floor(u)), int(math.floor(v))
            if 0 <= ui < w and 0 <= vi < h:
                out[y][x] = grid[vi][ui]
    return out


def breath(grid, split_from_bottom):
    h = len(grid)
    out = [r[:] for r in grid]
    split = h - split_from_bottom
    for y in range(split, 0, -1):
        out[y] = grid[y - 1][:]
    out[0] = [0] * len(grid[0])
    return out


def flash(grid, palette):
    """Hit frame: every colour moves to a lighter one of similar hue in the same palette."""
    lut = {0: 0, 1: 1}
    for i in range(2, len(palette)):
        r, g, b = palette[i]
        target = tuple(min(255, int(c * 1.5 + 50)) for c in (r, g, b))
        best = min(range(2, len(palette)),
                   key=lambda j: sum((palette[j][k] - target[k]) ** 2 for k in range(3)))
        lut[i] = best
    return [[lut[v] for v in row] for row in grid]


def brighten(gain, lift):
    """Concept tweak: dungeon floors are dark, so dark monsters get lifted mid-tones (outline stays black)."""
    def f(im):
        px = im.load()
        for y in range(im.height):
            for x in range(im.width):
                r, g, b, a = px[x, y]
                if a >= 128 and r + g + b > 60:
                    px[x, y] = tuple(min(255, int(c * gain + lift)) for c in (r, g, b)) + (a,)
        return im
    return f


def monster(name, concept, breath_split, tweak=None):
    grid, pal = load_quantized(os.path.join(CONCEPTS, concept), tweak=tweak)
    grid = darken_edges(grid, pal)
    grid = add_outline(grid)
    base = place(grid, 64, bottom_margin=0)
    frames = [
        base,
        breath(base, breath_split),
        scale_about_feet(base, 0.97, 1.06),
        scale_about_feet(base, 1.12, 1.12, dy=1),
        flash(scale_about_feet(base, 0.94, 0.94, dx=2), pal),
    ]
    save(name, frames, pal)


def save(name, frames, pal):
    w = len(frames[0][0])
    rows = [r for f in frames for r in f]
    img = Image.new("P", (w, len(rows)))
    flat = []
    for c in pal + [(0, 0, 0)] * (16 - len(pal)):
        flat += list(c)
    img.putpalette(flat)
    img.putdata([v for r in rows for v in r])
    os.makedirs(OUT, exist_ok=True)
    img.save(os.path.join(OUT, name + ".png"), transparency=0)


# ---------------------------------------------------------------- props
def prop_frames(name, concepts):
    """Several concepts quantized together (one shared 16-colour palette), one frame each."""
    ims = [Image.open(os.path.join(CONCEPTS, c)).convert("RGBA") for c in concepts]
    strip = Image.new("RGBA", (32, 32 * len(ims)))
    for k, im in enumerate(ims):
        strip.alpha_composite(im, (0, 32 * k))
    tmp = os.path.join(OUT, "_strip_" + name + ".png")
    os.makedirs(OUT, exist_ok=True)
    strip.save(tmp)
    grid, pal = load_quantized(tmp, colors=13)
    os.remove(tmp)
    frames = []
    for k in range(len(ims)):
        g = [r[:] for r in grid[32 * k:32 * (k + 1)]]
        g = add_outline(darken_edges(g, pal))
        frames.append(place(g, 32, bottom_margin=0))
    save(name, frames, pal)


def prop_from_concept(name, concept, extra=None):
    grid, pal = load_quantized(os.path.join(CONCEPTS, concept), colors=13)
    grid = darken_edges(grid, pal)
    grid = add_outline(grid)
    base = place(grid, 32, bottom_margin=0)
    frames = [base] + ([extra(base, pal)] if extra else [])
    save(name, frames, pal)


PROP_PAL = [(255, 0, 255), (12, 8, 10), (35, 30, 30), (58, 50, 46), (88, 76, 66), (123, 102, 84),
            (152, 127, 104), (184, 156, 128), (60, 63, 70), (90, 95, 105), (135, 141, 152), (183, 188, 196),
            (230, 233, 238), (78, 42, 20), (139, 84, 39)]


def prop_spikes():
    """Frame 1: spikes down (stone plate with holes). Frame 2: spikes up. Lies flat on the floor."""
    def plate():
        g = [[0] * 32 for _ in range(32)]
        # floor plate seen in perspective: trapezoid y 22..31
        for y in range(22, 32):
            t = (y - 22) / 9
            half = 11 + t * 5
            for x in range(32):
                if abs(x + 0.5 - 16) <= half:
                    c = 5 if y > 23 else 7
                    if abs(x + 0.5 - 16) > half - 1 or y == 31:
                        c = 1
                    elif y == 30:
                        c = 3
                    g[y][x] = c
        holes = []
        for row, yy in enumerate((24, 27)):
            n = 4
            t = (yy - 22) / 9
            half = 11 + t * 5 - 3
            for k in range(n):
                x = int(16 - half + k * (2 * half) / (n - 1))
                holes.append((x, yy + row))
                g[yy + row][x] = 1
                g[yy + row][x + 1] = 1
                g[yy + row + 1][x] = 4
                g[yy + row + 1][x + 1] = 4
        return g, holes

    down, holes = plate()
    up = [r[:] for r in down]
    for (x, y) in holes:
        hgt = 12 if y < 27 else 15
        for k in range(hgt):
            yy = y - k
            if k < hgt - 3:
                up[yy][x - 1] = 1
                up[yy][x] = 11
                up[yy][x + 1] = 9
                up[yy][x + 2] = 1
            else:
                up[yy][x] = 12 if k < hgt - 1 else 1
                up[yy][x + 1] = 1 if k == hgt - 1 else 9
            if yy - 1 >= 0 and k == hgt - 1:
                up[yy - 1][x] = 1
    save("prop_spikes", [down, up], PROP_PAL)


def floor_plate(rows_from=22, fill=5, rim=7):
    g = [[0] * 32 for _ in range(32)]
    for y in range(rows_from, 32):
        t = (y - rows_from) / max(1, 31 - rows_from)
        half = 11 + t * 5
        for x in range(32):
            if abs(x + 0.5 - 16) <= half:
                c = fill if y > rows_from + 1 else rim
                if abs(x + 0.5 - 16) > half - 1 or y == 31:
                    c = 1
                elif y == 30:
                    c = 3
                g[y][x] = c
    return g


def prop_plate():
    """Pressure plate: a raised iron-rimmed stone slab, flat on the floor (frame 2: pressed, 1 px lower)."""
    up = floor_plate(23, 6, 7)
    for y in range(25, 30):
        for x in range(32):
            if up[y][x] not in (0, 1) and abs(x + 0.5 - 16) < 9 + (y - 25):
                up[y][x] = 9 if y == 25 else 10 if (x + y) % 6 == 0 and y in (27,) else 8
    down = [[0] * 32 for _ in range(32)]
    for y in range(31, 0, -1):
        down[y] = up[y - 1][:] if y > 23 else [0] * 32
    down = [r[:] for r in down]
    for x in range(32):
        if up[31][x]:
            down[31][x] = 1
    save("prop_plate", [up, down], PROP_PAL)


def prop_pit():
    """Open pit: black hole with a lit stone lip, flat on the floor."""
    g = floor_plate(20, 4, 6)
    for y in range(22, 31):
        t = (y - 20) / 11
        half = 11 + t * 5 - 3
        for x in range(32):
            if abs(x + 0.5 - 16) <= half and y < 30:
                g[y][x] = 1 if y > 23 else 2
    for y in range(24, 30):                       # far inner wall of the pit, faintly lit
        t = (y - 20) / 11
        half = 11 + t * 5 - 3
        for x in range(32):
            if abs(x + 0.5 - 16) <= half and y in (22, 23):
                g[y][x] = 3
    for x in range(32):
        if g[22][x] == 2:
            g[22][x] = 4
    save("prop_pit", [g], PROP_PAL)


def build():
    monster("mon_rat", "pixellab_rat.png", breath_split=10, tweak=brighten(1.25, 12))
    monster("mon_skeleton", "pixellab_skel2.png", breath_split=14, tweak=brighten(1.2, 10))
    monster("mon_slime", "pixellab_slime2.png", breath_split=8)
    prop_from_concept("prop_barrel", "pixellab_barrel.png")
    prop_from_concept("prop_bones", "pixellab_bones2.png")
    prop_from_concept("prop_crate", "pixellab_crate.png")
    prop_from_concept("prop_statue", "pixellab_statue.png")
    prop_from_concept("prop_table", "pixellab_table.png")
    prop_frames("prop_chest", ["pixellab_chest.png", "pixellab_chest_open.png"])
    prop_spikes()
    prop_plate()
    prop_pit()


if __name__ == "__main__":
    build()
    print("ok")
