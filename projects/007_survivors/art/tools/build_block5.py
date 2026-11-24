"""Block 5 art (G-51): F2 Grim Graveyard bosses and enemy projectiles.

    python -I art/tools/build_block5.py fetch            # PixelLab sources -> art/raw/banshee, art/raw/lich
    python -I art/tools/build_block5.py seed [names]     # banshee, bone_colossus, lich, fx_arrow, fx_bone, fx_skull,
                                                         # fx_grave (creates missing .aseprite files, never overwrites)
Then: python -I art/tools/build_art.py export
"""
import glob
import io
import os
import random
import sys
import urllib.request
import zipfile

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_block2 import ART, blank, put, get, rim, shift, disc, save, pal, read_aseprite  # noqa: E402
from build_block4 import line, rect, star  # noqa: E402
from build_hordes import up2_thin, tri  # noqa: E402

RAW = os.path.join(ART, "raw")
CHARS = {"banshee": "b6fa4e23-05c0-4135-8fc6-6c37aa593dff", "lich": "e2d4e434-090a-470e-9d04-2a5f7d1f08af"}


def fetch():
    for name, cid in CHARS.items():
        req = urllib.request.Request(f"https://api.pixellab.ai/mcp/characters/{cid}/download",
                                     headers={"User-Agent": "gba-art-fetch/1.0"})
        data = urllib.request.urlopen(req, timeout=120).read()
        zipfile.ZipFile(io.BytesIO(data)).extractall(os.path.join(RAW, name))
        print("fetched", name)


def raw_frames(name, anim):
    """south frames of an animation (by display name) or the south rotation (anim=None)."""
    base = os.path.join(RAW, name)
    if anim is None:
        return [Image.open(glob.glob(os.path.join(base, "**", "rotations", "south.png"), recursive=True)[0]).convert("RGBA")]
    d = glob.glob(os.path.join(base, "**", "animations", "*" + anim + "*", "south"), recursive=True)
    assert d, (name, anim)
    return [Image.open(p).convert("RGBA") for p in sorted(glob.glob(os.path.join(d[0], "*.png")))]


def to_indices(ims, palette, size=64, anchor="bottom", box=None):
    """Common crop box for all frames (no jitter), centred horizontally, bottom-anchored, cut to size."""
    if box is None:
        bbs = [im.getbbox() for im in ims]
        box = (min(b[0] for b in bbs), min(b[1] for b in bbs), max(b[2] for b in bbs), max(b[3] for b in bbs))
    out = []
    for im in ims:
        c = im.crop(box)
        w, h = c.size
        ox = (size - w) // 2
        oy = size - h if anchor == "bottom" else (size - h) // 2
        f = blank(size, size)
        cache = {}
        for y in range(h):
            for x in range(w):
                r, g, b, a = c.getpixel((x, y))
                if a < 128:
                    continue
                k = (r, g, b)
                if k not in cache:
                    cache[k] = min(range(1, len(palette)), key=lambda i: 2 * (palette[i][0] - r) ** 2 +
                                   4 * (palette[i][1] - g) ** 2 + 3 * (palette[i][2] - b) ** 2)
                put(f, x + ox, y + oy, cache[k])
        out.append(f)
    return out, box


def outline_fix(f, rimc):
    """replace the generator's black outline by nothing and add the style C cold rim around the silhouette."""
    return rim(f, rimc)


def dissolve(f, k, n, rnd, up=0, recol=None):
    g = shift(f, 0, -up * (k + 1))
    keep = 1.0 - (k + 1) / (n + 0.5)
    g = [[(c if c and rnd.random() < keep else 0) for c in r] for r in g]
    if recol:
        g = [[recol.get(c, c) for c in r] for r in g]
    return g


# ---------------------------------------------------------------- banshee 64x64
BANSHEE = pal(["#07060c", "#191935", "#21263b", "#36435b", "#34466e", "#8ca4b4", "#abc4d2", "#c8dfeb", "#1a7f9c",
               "#40e8f8", "#c0fcff", "#46155a", "#e020c0", "#fd3ffd", "#2a343d"])
# 1 black 2-4 shroud dark 5 rim? (#34466e) 6-8 shroud light 9-11 cyan eyes/glow 12-14 magenta wail 15 grey


def banshee():
    fl = raw_frames("banshee", "float")[:4]
    wl = raw_frames("banshee", "wail")
    pool = fl + wl
    frames, box = to_indices(pool, BANSHEE, 64, anchor="center")
    flo, wai = frames[:4], frames[4:]
    wail = [wai[1], wai[min(3, len(wai) - 1)]]
    # wail: magenta sound rings around the head
    for k, f in enumerate(wail):
        for r in ((9, 13) if k == 0 else (12, 17, 22)):
            for a in range(0, 360, 6):
                import math
                x = 32 + r * math.cos(math.radians(a)); y = 18 + r * 0.7 * math.sin(math.radians(a))
                if get(f, round(x), round(y)) == 0 and (a // 6) % 3:
                    put(f, round(x), round(y), 13 if r < 15 else 12)
    rnd = random.Random(51)
    base = flo[0]
    vanish = []
    for k in range(2):     # dithered phase-out (like ghost fade) to a cyan outline
        f = [r[:] for r in base]
        for y in range(64):
            for x in range(64):
                if f[y][x] and ((x + y) % 2 if k == 0 else (x % 2 or y % 2)):
                    f[y][x] = 0
                elif f[y][x] and k == 1:
                    f[y][x] = 9
        vanish.append(f)
    death = [dissolve(base, k, 4, rnd, up=2, recol=({6: 10, 7: 10, 3: 9, 4: 9} if k >= 1 else None))
             for k in range(4)]
    flo = [rim(f, 5) for f in flo]
    wail = [rim(f, 5) for f in wail]
    save("banshee", 64, 64, BANSHEE, flo + wail + vanish + death,
         [140] * 4 + [160, 200] + [90, 90] + [90, 100, 110, 140],
         [("float", 0, 3), ("wail", 4, 5), ("vanish", 6, 7), ("death", 8, 11)])


# ---------------------------------------------------------------- lich 64x64
LICH = pal(["#07060c", "#190922", "#2a012f", "#2d1153", "#3b2171", "#46155a", "#341d60", "#8ca4b4", "#c8dfeb",
            "#e020c0", "#fd3ffd", "#ff9cf0", "#1a7f9c", "#40e8f8", "#34466e"])
# 1 black 2-7 robes violets 8-9 bone 10-12 magenta 13-14 cyan orb 15 rim


def lich():
    idle = raw_frames("lich", "idle")[:4]
    cast = raw_frames("lich", "cast")
    frames, box = to_indices(idle + cast, LICH, 64)
    idl, cst = frames[:4], frames[4:]
    pick = [cst[0], cst[1], cst[4]]          # frames 2-3 of the source carry a white swoosh: skipped
    # cast glow: magenta sparks around the raised orb (highest non-black pixel area)
    for k, f in enumerate(pick):
        top = min((y for y in range(64) for x in range(64) if f[y][x]), default=4)
        xs = [x for x in range(64) if f[top][x]]
        cx = sum(xs) / len(xs) if xs else 32
        for (dx, dy) in ((-4, 2), (4, 1), (-2, -1), (3, 5), (-5, 6)) [: 2 + k]:
            put(f, round(cx + dx), max(0, top + dy), 11)
    rnd = random.Random(52)
    base = idl[0]
    tele = []
    for k in range(3):        # teleport: squeezed into a magenta column then gone
        f = blank(64, 64)
        keep = (0.65, 0.35, 0.12)[k]
        for y in range(64):
            for x in range(64):
                c = base[y][x]
                if not c:
                    continue
                nx = round(32 + (x - 32) * keep)
                if rnd.random() < 0.85:
                    put(f, nx, y, c if k == 0 else (10 if k == 1 else 11))
        tele.append(f)
    death = [dissolve(base, k, 5, rnd, up=0, recol=({4: 8, 3: 7, 5: 8} if k >= 2 else None)) for k in range(5)]
    for k, f in enumerate(death):        # sinks into its own robes: bottom rows stay, top crumbles
        if k >= 1:
            death[k] = shift(f, 0, 3 * k)
    idl = [rim(f, 15) for f in idl]
    pick = [rim(f, 15) for f in pick]
    save("lich", 64, 64, LICH, idl + pick + tele + death,
         [150] * 4 + [100, 120, 200] + [60, 60, 80] + [90, 100, 110, 120, 160],
         [("idle", 0, 3), ("cast", 4, 6), ("teleport", 7, 9), ("death", 10, 14)])


# ---------------------------------------------------------------- bone_colossus 64x64 (skeleton_big 2x)
def bone_colossus():
    w, h, p, fr, durs, tags = read_aseprite("skeleton_big")
    p = (list(p) + [(0, 0, 0)] * 16)[:16]
    # skeleton_big palette: 2 #0c1626 4 #21263b 6 #2f3551 8 #346675 9 #e63c3c 10 #4b8787 11 #8ca4b4 12 #abc4d2
    # 13 #c8dfeb ; rim = the most common border colour
    border = {}
    for f in fr:
        for y in range(h):
            for x in range(w):
                c = f[y][x]
                if c and any(get(f, x + dx, y + dy) == 0 for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    border[c] = border.get(c, 0) + 1
    rimc = max(border, key=border.get)
    walk = [up2_thin(fr[k], rimc) for k in range(6)]
    death_src = [up2_thin(fr[k], rimc) for k in range(6, 13)]

    def bone_at(m, cx, cy, horiz=True):
        if horiz:
            rect(m, cx - 5, cy, cx + 5, cy + 1, 13)
            for ex in (cx - 6, cx + 6):
                rect(m, ex - 1, cy - 1, ex, cy + 2, 13)
        else:
            rect(m, cx, cy - 5, cx + 1, cy + 5, 13)
            for ey in (cy - 6, cy + 6):
                rect(m, cx - 1, ey - 1, cx + 2, ey, 13)
        return m

    def top_of(m):
        ys = [y for y in range(64) for x in range(64) if m[y][x]]
        xs = [x for x in range(64) if any(m[y][x] for y in range(64))]
        return min(ys), (min(xs) + max(xs)) // 2

    t0, cx0 = top_of(walk[0])
    wind = shift(walk[0], -1, 2)                        # crouch, bone raised over the skull
    wind = bone_at(wind, cx0, max(3, t0 - 4))
    line(wind, cx0 + 4, t0 + 12, cx0 + 4, t0 - 2, 12, 2)    # raised arm
    wind = rim(wind, rimc)
    rel = shift(walk[3], 2, 0)                          # lunge forward, bone leaving to the right
    rel = bone_at(rel, min(56, cx0 + 20), t0 + 8, horiz=False)
    line(rel, cx0 + 6, t0 + 14, cx0 + 14, t0 + 10, 12, 2)
    rel = rim(rel, rimc)
    collapse = [death_src[0], death_src[1], death_src[2]]
    death = [death_src[3], death_src[4], death_src[5], death_src[6]]
    rnd = random.Random(53)
    death.append(dissolve(death_src[6], 1, 3, rnd))
    save("bone_colossus", 64, 64, p, walk + [wind, rel] + collapse + death,
         [120] * 6 + [220, 160] + [110, 130, 300] + [100, 110, 120, 140, 160],
         [("walk", 0, 5), ("throw", 6, 7), ("collapse", 8, 10), ("death", 11, 15)])


# ---------------------------------------------------------------- F2 enemy projectiles (shared palette)
EPROJ = pal(["#07060c", "#2a343d", "#8ca4b4", "#c8dfeb", "#bc161b", "#e63c3c", "#fd3ffd", "#e020c0", "#46155a",
             "#40e8f8", "#1a7f9c", "#ff9cf0", "#a06818", "#ffc838", "#4b1530"])
# 1 black 2 grey 3 bone 4 bone light 5 red 6 red light 7 magenta 8 magenta dark 9 violet 10 cyan 11 cyan dark
# 12 pink 13 amber 14 gold 15 wine


def fx_arrow():
    e = blank(8, 8)
    line(e, 0, 4, 5, 4, 3)
    put(e, 0, 3, 6); put(e, 0, 5, 6); put(e, 1, 3, 5); put(e, 1, 5, 5)       # red fletching
    put(e, 6, 3, 4); put(e, 6, 4, 4); put(e, 6, 5, 4); put(e, 7, 4, 4)       # bone head
    put(e, 5, 4, 4)
    ne = blank(8, 8)
    line(ne, 1, 6, 5, 2, 3)
    put(ne, 0, 6, 6); put(ne, 1, 7, 6); put(ne, 0, 7, 5)
    put(ne, 6, 1, 4); put(ne, 7, 0, 4); put(ne, 5, 1, 4); put(ne, 6, 2, 4)
    frames = [rim(e, 15), rim(ne, 15)]
    save("fx_arrow", 8, 8, EPROJ, frames, [100, 100], [("e", 0, 0), ("ne", 1, 1)])


def fx_bone():
    frames = []
    for k in range(4):
        m = blank(8, 8)
        a, b = [((1, 3), (6, 4)), ((1, 6), (6, 1)), ((3, 1), (4, 6)), ((1, 1), (6, 6))][k]
        line(m, a[0], a[1], b[0], b[1], 3)
        for (x, y) in (a, b):
            put(m, x, y, 4); put(m, x + (1 if x < 4 else -1), y, 4)
        frames.append(rim(m, 5))
    save("fx_bone", 8, 8, EPROJ, frames, [70] * 4, [("spin", 0, 3)])


def fx_skull():
    frames = []
    for k in range(2):
        m = blank(8, 8)
        # magenta flame trail on the left/top
        for (x, y) in (((0, 3), (1, 2), (1, 4), (0, 5)) if k == 0 else ((0, 2), (1, 3), (0, 4), (1, 5))):
            put(m, x, y, 7 if x == 1 else 8)
        disc(m, 4.5, 3.5, 2.4, 3)
        rect(m, 3, 5, 6, 6, 3)
        put(m, 3, 3, 6); put(m, 5, 3, 6); put(m, 4, 5, 2); put(m, 4, 6, 1); put(m, 6, 6, 1)
        put(m, 4, 1, 4); put(m, 5, 2, 4)
        frames.append(rim(m, 8))
    save("fx_skull", 8, 8, EPROJ, frames, [90, 90], [("fly", 0, 1)])


def fx_grave():
    """RISE warning: the earth cracks and glows magenta (same language as fx_root), dirt mounds push up."""
    frames = []
    for k in range(3):
        m = blank(16, 16)
        # dirt mound
        w = 5 + 2 * k
        for x in range(8 - w, 8 + w):
            hgt = max(0, round((k + 1) * 1.3 * (1 - ((x - 7.5) / w) ** 2)))
            for y in range(13 - hgt, 14):
                put(m, x, y, 1 if y == 13 else 2)
        # crack: magenta core grows
        pts = [(3, 12), (6, 11), (8, 12), (10, 10), (13, 11)][: 3 + k]
        for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
            line(m, x0, y0, x1, y1, 8 if k == 0 else 7)
        if k == 2:
            for (x, y) in ((7, 9), (9, 8), (5, 10)):
                put(m, x, y, 12)
            put(m, 8, 6, 3); put(m, 9, 7, 3)         # bony fingers breaking through
            put(m, 6, 7, 3)
        frames.append(m)
    save("fx_grave", 16, 16, EPROJ, frames, [200, 200, 200], [("crack", 0, 2)])


NEW = {"bone_colossus": bone_colossus, "fx_arrow": fx_arrow, "fx_bone": fx_bone, "fx_skull": fx_skull,
       "fx_grave": fx_grave, "banshee": banshee, "lich": lich}

if __name__ == "__main__":
    if sys.argv[1] == "fetch":
        fetch()
    elif sys.argv[1] == "seed":
        for name, fn in NEW.items():
            if len(sys.argv) > 2 and name not in sys.argv[2:]:
                continue
            if os.path.exists(os.path.join(ART, name + ".aseprite")):
                print("exists, skipped:", name); continue
            fn()
