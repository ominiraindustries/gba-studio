"""Seed generator for POP PANIC block-1 art.

Writes indexed PNGs into art/_seed/out/. build_aseprite.lua turns them into art/*.aseprite
ONLY when the .aseprite does not exist yet (user edits always win).

Run: python -I art/_seed/seed_art.py   (from projects/002_pop_panic)
"""
import math
import os

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")
os.makedirs(OUT, exist_ok=True)


def save_indexed(pixels, w, h, palette, name):
    img = Image.new("P", (w, h), 0)
    img.putdata(pixels)
    flat = []
    for c in palette:
        flat += list(c)
    flat += [0] * (768 - len(flat))
    img.putpalette(flat)
    img.save(os.path.join(OUT, name + ".png"), transparency=0)
    return img


# ---------------------------------------------------------------- player
# 16x16 frames, character ~12x15, feet on row 15.
PLAYER_PAL = [
    (255, 0, 255),    # 0 transparent
    (24, 20, 44),     # 1 K outline
    (232, 56, 80),    # 2 R cap
    (152, 28, 64),    # 3 r cap shadow
    (252, 200, 152),  # 4 S skin
    (212, 132, 100),  # 5 s skin shadow
    (84, 48, 40),     # 6 H hair
    (72, 208, 192),   # 7 T jacket
    (36, 132, 144),   # 8 t jacket shadow
    (64, 72, 136),    # 9 P trousers
    (196, 116, 52),   # 10 B boots
    (120, 64, 36),    # 11 b boot sole
    (196, 200, 216),  # 12 G gun light
    (108, 112, 140),  # 13 g gun dark
    (255, 255, 255),  # 14 W white
    (252, 216, 64),   # 15 Y yellow
]
PCH = {".": 0, "K": 1, "R": 2, "r": 3, "S": 4, "s": 5, "H": 6, "T": 7, "t": 8, "P": 9,
       "B": 10, "b": 11, "G": 12, "g": 13, "W": 14, "Y": 15}

HEAD = [  # rows 0..6
    "................",
    "....KKKKK.......",
    "...KRRWRRK......",
    "...KrrrrrRRRK...",
    "..KHHSSSKKKK....",
    "..KHSSSSKSK.....",
    "...KsSSSSSK.....",
]
HEAD_HURT = [
    "................",
    "....KKKKK.......",
    "...KRRWRRK......",
    "...KrrrrrRRRK...",
    "..KHHSSSKKKK....",
    "..KHSSSKWK......",
    "...KsSSSKSK.....",
]
HEAD_UP = [
    "................",
    "....KKKKK.......",
    "...KRRWRRK......",
    "...KrrrrrRK.....",
    "..KHHSSSKSK.....",
    "..KHSSSSSSK.....",
    "...KsSSSSK......",
]
TORSO = [  # rows 7..10
    "...KKTTTTKK.....",
    "..KTTTKSKKKKKK..",
    "..KtTTKSGGGGYK..",
    "..KttTTKKKKKK...",
]
TORSO_SHOOT = [
    "...KKTTTKSSK....",
    "..KTTTTTKSK.....",
    "..KtTTTTTK......",
    "..KttTTTKK......",
]
TORSO_HURT = [
    "...KKTTTTKKSK...",
    "..KTTTTTTKSK....",
    "..KtTTTTTK......",
    "..KttTTTKK......",
]
GUN_UP = ["KYK", "KYK", "KGK", "KGK", "KGK", "KGK", "KGK", "KGK", "KGK", "KKK"]  # cols 11..13, rows 0..9
LEGS = {  # rows 11..15
    "stand": [
        "...KPPPPPK......",
        "...KPPKPPK......",
        "...KPK.KPK......",
        "..KBBK.KBBK.....",
        "..KbbK.KbbbK....",
    ],
    "w1": [
        "...KPPPPPK......",
        "..KPPK.KPPK.....",
        ".KPPK...KPPK....",
        ".KBBK...KBBBK...",
        "KbbK.....KbbbK..",
    ],
    "w2": [
        "...KPPPPPK......",
        "...KPPPPK.......",
        "...KPKPPK.......",
        "...KBKBBK.......",
        "...KbKbbbK......",
    ],
    "w3": [
        "...KPPPPPK......",
        "..KPPKKPPK......",
        "..KPK..KPPK.....",
        ".KBBK..KBBBK....",
        ".KbbK...KbbbK...",
    ],
    "w4": [
        "...KPPPPPK......",
        "....KPPPPK......",
        "....KPPKPK......",
        "...KBBKBK.......",
        "...KbbbKbK......",
    ],
}


def stamp(frame, rows, ox, oy):
    for y, row in enumerate(rows):
        for x, ch in enumerate(row):
            v = PCH[ch]
            if v and 0 <= ox + x < 16 and 0 <= oy + y < 16:
                frame[oy + y][ox + x] = v


def body(head, torso, legs, bob=0, gun=False, dx=0):
    """bob=+1 raises the upper body 1px (belt row repeated), -1 lowers it (breath)."""
    up = [[0] * 16 for _ in range(16)]
    stamp(up, head, dx, 0)
    stamp(up, torso, dx, 7)
    if gun:
        stamp(up, GUN_UP, dx + 11, 0)
    f = [[0] * 16 for _ in range(16)]
    for y in range(11):
        ny = y - bob
        if 0 <= ny < 16:
            f[ny] = up[y][:]
    if bob == 1:
        f[10] = up[10][:]
    stamp(f, LEGS[legs], dx, 11)
    return f


def rotate_ccw(f):
    r = [[f[x][15 - y] for x in range(16)] for y in range(16)]
    rows = [y for y in range(16) if any(r[y])]
    shift = 15 - rows[-1]
    out = [[0] * 16 for _ in range(16)]
    for y in rows:
        out[y + shift] = r[y]
    return out


def player():
    frames = [
        body(HEAD, TORSO, "stand"),                         # 0 idle
        body(HEAD, TORSO, "stand", bob=-1),                 # 1 idle (breath)
        body(HEAD, TORSO, "w1"),                            # 2 walk
        body(HEAD, TORSO, "w2", bob=1),                     # 3 walk
        body(HEAD, TORSO, "w3"),                            # 4 walk
        body(HEAD, TORSO, "w4", bob=1),                     # 5 walk
        body(HEAD_UP, TORSO_SHOOT, "stand", gun=True),      # 6 shoot
        body(HEAD_HURT, TORSO_HURT, "stand", dx=-1),        # 7 hurt
    ]
    frames.append(rotate_ccw(frames[7]))                    # 8 death (lying)
    px = [v for f in frames for row in f for v in row]
    save_indexed(px, 16, 16 * len(frames), PLAYER_PAL, "player")


# ---------------------------------------------------------------- balls
BALL_PAL = [
    (255, 0, 255),    # 0 transparent
    (255, 150, 150),  # 1 base light
    (240, 76, 84),    # 2 base
    (196, 36, 64),    # 3 base dark
    (128, 20, 52),    # 4 base darkest
    (36, 8, 28),      # 5 outline
    (255, 255, 255),  # 6 highlight
] + [(0, 0, 0)] * 9


def ball(size, canvas):
    r = size / 2.0
    c0 = (canvas - size) / 2.0
    cx, cy = c0 + r, c0 + r
    lx, ly = cx - r * 0.38, cy - r * 0.38          # light spot
    hx, hy = cx - r * 0.42, cy - r * 0.45          # specular highlight centre
    hr = max(0.9, r * 0.2)
    px = [0] * (canvas * canvas)
    inside = lambda x, y: (x + 0.5 - cx) ** 2 + (y + 0.5 - cy) ** 2 <= r * r
    for y in range(canvas):
        for x in range(canvas):
            if not inside(x, y):
                continue
            edge = not (inside(x - 1, y) and inside(x + 1, y) and inside(x, y - 1) and inside(x, y + 1))
            if edge:
                v = 5
            else:
                d = math.hypot(x + 0.5 - lx, y + 0.5 - ly) / (r * 1.38)
                # ordered-ish banding with a tiny checker dither on band borders
                bands = [0.32, 0.62, 0.86]
                v = 1
                for i, b in enumerate(bands):
                    if d > b:
                        v = 2 + i
                    elif size >= 24 and d > b - 0.04 and (x + y) % 2 == 0:
                        v = 2 + i
                        break
                    else:
                        break
                if math.hypot((x + 0.5 - hx) / 1.25, y + 0.5 - hy) <= hr:
                    v = 6
            px[y * canvas + x] = v
    if size == 8:  # hand-tuned tiny ball
        rows = [
            "..5555..",
            ".562215.",
            "56122235",
            "51222335",
            "52223345",
            "52333445",
            ".534445.",
            "..5555..",
        ]
        px = [int(ch) if ch != "." else 0 for row in rows for ch in row]
    return px


def balls():
    for size, canvas in ((32, 32), (24, 32), (16, 16), (8, 8)):
        save_indexed(ball(size, canvas), canvas, canvas, BALL_PAL, "ball_%d" % size)


# ---------------------------------------------------------------- harpoon
HARPOON_PAL = [
    (255, 0, 255), (24, 20, 44), (220, 224, 236), (128, 132, 160), (255, 255, 255),
    (252, 216, 64), (196, 140, 36),
] + [(0, 0, 0)] * 9
HCH = {".": 0, "K": 1, "G": 2, "g": 3, "W": 4, "Y": 5, "y": 6}
HARPOON = [
    # frame 0: tip
    "...KK...",
    "..KWGK..",
    "..KWGK..",
    ".KWGGgK.",
    ".KWGGgK.",
    "KWGGGggK",
    "KKKGgKKK",
    "...KYK..",
    "..KYyyK.",
    "...KyK..",
    "...KGK..",
    "...KgK..",
    "...KGK..",
    "...KgK..",
    "..KYyyK.",
    "...KyK..",
    # frame 1: chain segment (tiles vertically every 16 px)
    "..KKKK..",
    ".KYyyyK.",
    ".KyK.Ky.",
    ".KyK.KyK",
    ".KyK.KyK",
    ".KYyyyK.",
    "..KKKK..",
    "...KK...",
    "..KGgK..",
    "..KGgK..",
    "..KGgK..",
    "..KGgK..",
    "..KGgK..",
    "..KGgK..",
    "...KK...",
    "...KK...",
]


def harpoon():
    rows = [r.ljust(8, ".")[:8] for r in HARPOON]
    rows[18] = ".KyK.KyK"
    px = [HCH[ch] for row in rows for ch in row]
    save_indexed(px, 8, 32, HARPOON_PAL, "harpoon")


# ---------------------------------------------------------------- pop fx
FX_PAL = [
    (255, 0, 255), (255, 255, 255), (255, 244, 176), (255, 208, 64), (255, 140, 40),
    (220, 72, 40), (60, 20, 40),
] + [(0, 0, 0)] * 9


def pop_fx():
    frames = []
    cx = cy = 15.5
    for i in range(4):
        f = [0] * (32 * 32)

        def put(x, y, v):
            if 0 <= x < 32 and 0 <= y < 32:
                f[y * 32 + x] = v

        for y in range(32):
            for x in range(32):
                d = math.hypot(x - cx, y - cy)
                if i == 0:
                    if d <= 7.5:
                        put(x, y, 1 if d < 5 else 2)
                    elif d <= 9:
                        put(x, y, 3)
                elif i == 1:
                    if 9 <= d <= 11.5:
                        put(x, y, 2 if d < 10.5 else 3)
                    elif 8 <= d < 9:
                        put(x, y, 1)
        # sparks: 8 directions, distance grows
        dist = [6, 11, 13.5, 14.5][i]
        for k in range(8):
            a = k * math.pi / 4 + (math.pi / 8 if i >= 2 else 0)
            sx = int(round(cx + math.cos(a) * dist))
            sy = int(round(cy + math.sin(a) * dist))
            if i == 0:
                continue
            if i == 1:
                for dx, dy, v in ((0, 0, 1), (1, 0, 3), (-1, 0, 3), (0, 1, 3), (0, -1, 3)):
                    put(sx + dx, sy + dy, v)
            elif i == 2:
                for dx, dy, v in ((0, 0, 1), (1, 0, 3), (-1, 0, 3), (0, 1, 3), (0, -1, 3),
                                  (2, 0, 4), (-2, 0, 4), (0, 2, 4), (0, -2, 4)):
                    put(sx + dx, sy + dy, v)
            else:
                for dx, dy, v in ((0, 0, 3), (1, 0, 4), (0, 1, 4), (1, 1, 5)):
                    put(sx + dx, sy + dy, v)
        if i == 2:
            for y in range(32):
                for x in range(32):
                    d = math.hypot(x - cx, y - cy)
                    if 11.5 <= d <= 12.5 and (x + y) % 3 == 0:
                        put(x, y, 3)
        frames += f
    save_indexed(frames, 32, 128, FX_PAL, "pop_fx")


# ---------------------------------------------------------------- items + pigeon (shared palette)
ITEM_PAL = [
    (255, 0, 255),    # 0 transparent
    (24, 20, 44),     # 1 K outline
    (255, 255, 255),  # 2 W white
    (252, 220, 72),   # 3 Y yellow
    (200, 140, 32),   # 4 y gold dark
    (120, 196, 255),  # 5 b light blue
    (48, 112, 232),   # 6 B blue
    (28, 52, 148),    # 7 n dark blue
    (214, 214, 226),  # 8 g light grey
    (150, 150, 172),  # 9 G grey
    (92, 92, 116),    # 10 D dark grey
    (72, 176, 132),   # 11 N neck green
    (150, 96, 176),   # 12 V neck purple
    (244, 140, 48),   # 13 O orange
    (232, 64, 80),    # 14 R red
    (252, 176, 184),  # 15 p pink
]
ICH = {".": 0, "K": 1, "W": 2, "Y": 3, "y": 4, "b": 5, "B": 6, "n": 7, "g": 8, "G": 9, "D": 10,
       "N": 11, "V": 12, "O": 13, "R": 14, "p": 15}


def grid(rows, w=16):
    return [ICH[ch] for row in rows for ch in row.ljust(w, ".")[:w]]


ITEM_DOUBLE = [
    ".....KKKKKK.....",
    "...KKbbbbbbKK...",
    "..KbbWbbbbWbbK..",
    ".KbbWWWbbWWWbBK.",
    ".KbWWWWWWWWWWBK.",
    "KbbbbWbbbbWbbBBK",
    "KbbbbWbbbbWbBBBK",
    "KbbbbWbbbbWBBBBK",
    "KbbbBWBBBBWBBBBK",
    "KbBBBWBBBBWBBBnK",
    "KBBBBWBBBBWBBBnK",
    ".KBBBWBBBBWBBnK.",
    ".KBBBBBBBBBBnnK.",
    "..KBBBBBBBBnnK..",
    "...KKnnnnnnKK...",
    ".....KKKKKK.....",
]
ITEM_SHIELD = [
    ".....KKKKKK.....",
    "...KKYYYYYYKK...",
    "..KYYKKKKKKYYK..",
    ".KYYKbbbBBBKyyK.",
    ".KYYKbWbBBBKyyK.",
    "KYYYKbbbBBBKyyyK",
    "KYYYKbbbBBBKyyyK",
    "KYYYKbbbBBnKyyyK",
    "KYYYKbbBBBnKyyyK",
    "KYYyyKbBBnKyyyyK",
    "KYyyyKbBBnKyyyyK",
    ".KyyyyKBnKyyyyK.",
    ".KyyyyyKKyyyyyK.",
    "..KyyyyyyyyyyK..",
    "...KKyyyyyyKK...",
    ".....KKKKKK.....",
]
PIGEON_BODY = [
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "...........KKK..",
    "..........KDDDK.",
    "..........KDODK.",
    ".KK......KNVDDKK",
    "KGGK..KKKNNVKK..",
    ".KGGKKggggggVK..",
    "..KGGgggggggGK..",
    "...KKGGGGGGGK...",
    ".....KKKKKKK....",
    "................",
]
PIGEON_WINGS = [
    [  # up
        (2, ".....KK........."),
        (3, "....KggK........"),
        (4, "....KgGgK......."),
        (5, ".....KgGGK......"),
        (6, ".....KgGGGK....."),
        (7, "......KGGGK....."),
        (8, "......KGGK......"),
        (9, "......KGGK......"),
    ],
    [  # mid
        (8, "...KKKKKK......."),
        (9, "..KggggGGK......"),
        (10, "...KKKKGGK......"),
    ],
    [  # down
        (11, "......KgGGK....."),
        (12, ".....KgGGK......"),
        (13, ".....KgGK......."),
        (14, "......KK........"),
    ],
]


def items_and_pigeon():
    save_indexed(grid(ITEM_DOUBLE), 16, 16, ITEM_PAL, "item_double")
    save_indexed(grid(ITEM_SHIELD), 16, 16, ITEM_PAL, "item_shield")
    frames = []
    for wing in PIGEON_WINGS:
        f = grid(PIGEON_BODY)
        for y, row in wing:
            for x, ch in enumerate(row):
                if ch != ".":
                    f[y * 16 + x] = ICH[ch]
        frames += f
    save_indexed(frames, 16, 48, ITEM_PAL, "pigeon")


# ---------------------------------------------------------------- backgrounds
# 256x256 regular BG, visible 240x160. Rows 0..151: scene. Rows 152..159: thin dark floor
# (row 152 = accent edge, rows 153..159 flat dark so white HUD text reads on it).
# Output is 4bpp "manual": 8bpp BMP whose palette is split in banks of 16; every 8x8 tile uses
# a single bank. Local index 0 of every bank is the backdrop colour (never used in visible tiles).
FLOORS = {
    # edge accent, flat dark
    "madrid": [(212, 150, 104), (36, 28, 40)],
    "paris": [(176, 150, 196), (30, 26, 44)],
    "london": [(170, 172, 186), (28, 30, 40)],
}
BACKDROP = (16, 16, 24)


def mute(c, ref, amount):
    return tuple(int(round(a + (b - a) * amount)) for a, b in zip(c, ref))


def pack_tiles(sets, limit=15):
    """Greedy packing of tile colour sets into palettes of <= limit colours."""
    pals = []
    assign = {}
    for t in sorted(sets, key=lambda k: -len(sets[k])):
        s = sets[t]
        best, grow = None, None
        for i, p in enumerate(pals):
            u = len(p | s)
            if u <= limit and (grow is None or u - len(p) < grow):
                best, grow = i, u - len(p)
        if best is None:
            pals.append(set(s))
            best = len(pals) - 1
        else:
            pals[best] |= s
        assign[t] = best
    return pals, assign


def background(city):
    src = Image.open(os.path.join(HERE, "pixellab_%s.png" % city)).convert("RGB")
    sp = src.load()
    w, h = src.size  # 240x144
    acc = [0, 0, 0]
    n = 0
    for y in range(16, 136):
        for x in range(48, 192):
            for i in range(3):
                acc[i] += sp[x, y][i]
            n += 1
    ref = tuple(a // n for a in acc)
    vis = Image.new("RGB", (240, 160))
    vp = vis.load()
    for y in range(152):
        sy = y if y < h else y - 8      # extend the scene 8 rows by repeating its bottom band
        for x in range(240):
            ex = min(x, w - 1 - x) / (w / 2)
            amount = 0.12 + 0.20 * min(1.0, ex * 1.6)
            vp[x, y] = mute(sp[x, sy], ref, amount)
    edge, dark = FLOORS[city]
    for x in range(240):
        vp[x, 152] = edge
        for y in range(153, 160):
            vp[x, y] = dark

    for ncol in range(48, 7, -4):
        q = vis.quantize(colors=ncol, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
        pal = q.getpalette()[: ncol * 3]
        cols = [tuple(pal[i * 3: i * 3 + 3]) for i in range(ncol)]
        qd = list(q.get_flattened_data())
        # cap each tile at 15 colours
        sets = {}
        for ty in range(20):
            for tx in range(30):
                idx = [(ty * 8 + yy) * 240 + tx * 8 + xx for yy in range(8) for xx in range(8)]
                cnt = {}
                for i in idx:
                    cnt[qd[i]] = cnt.get(qd[i], 0) + 1
                if len(cnt) > 15:
                    keep = sorted(cnt, key=lambda c: -cnt[c])[:15]
                    for i in idx:
                        if qd[i] not in keep:
                            v = qd[i]
                            qd[i] = min(keep, key=lambda k: sum((a - b) ** 2 for a, b in zip(cols[k], cols[v])))
                sets[(tx, ty)] = {qd[i] for i in idx}
        pals, assign = pack_tiles(sets)
        if len(pals) <= 16:
            break
    banks = [sorted(p) for p in pals]
    palette = []
    for b in banks:
        palette += [BACKDROP] + [cols[c] for c in b] + [BACKDROP] * (15 - len(b))
    data = [0] * (256 * 256)
    for (tx, ty), b in assign.items():
        lut = {c: assign_i + 1 for assign_i, c in enumerate(banks[b])}
        for yy in range(8):
            for xx in range(8):
                y, x = ty * 8 + yy, tx * 8 + xx
                data[y * 256 + x] = b * 16 + lut[qd[y * 240 + x]]
    print(city, "colours", ncol, "4bpp palettes", len(banks))
    out = Image.new("P", (256, 256))
    out.putdata(data)
    flat = [v for c in palette for v in c]
    out.putpalette(flat + [0] * (768 - len(flat)))
    out.save(os.path.join(OUT, "bg_%s.png" % city))


if __name__ == "__main__":
    player()
    balls()
    harpoon()
    pop_fx()
    items_and_pigeon()
    for city in ("madrid", "paris", "london"):
        background(city)
    print("seeded into", OUT)
