"""Style C neon accents (G-44): gems by tier and ice FX.

  python -I art/tools/neon_c.py      # adds the neons to palette_c.json, sets the shared palette entries,
                                     # redraws gems.aseprite (6 frames 8x8). Then build_art.py export.

Shared pickups palette (gems, icons, coin, heart, chest, hud_slots), entries changed:
  2 #1a7f9c cyan dark, 3 #40e8f8 cyan, 4 #a06818 gold dark, 10 #ff3838 magenta, 12 #ffc838 gold, 13 #fff4b0 gold light
Shared FX palette (fx_*, shadows): 7 #c0fcff ice light, 8 #40e8f8 ice, 9 #1a7f9c ice dark.
Gems: 0-1 = 1 XP small cyan, 2-3 = 5 XP medium magenta, 4-5 = 25 XP large gold (frame 1 of each = glint).
PISA gems.aseprite: no lo ejecutes si alguien lo ha editado a mano.
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import aseprite_io  # noqa: E402

ART = os.path.dirname(HERE)
NEONS = ["#1a7f9c", "#40e8f8", "#c0fcff", "#a06818", "#ffc838", "#fff4b0"]


def hexc(h):
    return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))


PICKUPS = {2: "#1a7f9c", 3: "#40e8f8", 4: "#a06818", 10: "#ff3838", 12: "#ffc838", 13: "#fff4b0"}
FX = {7: "#c0fcff", 8: "#40e8f8", 9: "#1a7f9c"}
PICKUP_FILES = ["gems", "icons", "coin", "heart", "chest", "hud_slots"]
FX_FILES = ["fx_aura", "fx_blink", "fx_frost", "fx_impact", "fx_nova", "fx_nova_spark", "fx_poof", "fx_rune",
            "fx_spark", "shadow_big", "shadow_small"]


def walk_chunks(b):
    nframes = struct.unpack_from("<H", b, 6)[0]
    pos = 128
    for fi in range(nframes):
        fsize = struct.unpack_from("<I", b, pos)[0]
        n2 = struct.unpack_from("<H", b, pos + 6)[0]
        n4 = struct.unpack_from("<I", b, pos + 12)[0]
        c = pos + 16
        for _ in range(n4 or n2):
            csize, ctype = struct.unpack_from("<IH", b, c)
            yield fi, c, csize, ctype
            c += csize
        pos += fsize


def set_entries(path, mapping):
    b = bytearray(open(path, "rb").read())
    done = 0
    for _, c, _, ctype in walk_chunks(b):
        if ctype != 0x2019:
            continue
        count, first, _ = struct.unpack_from("<III", b, c + 6)
        e = c + 6 + 20
        for i in range(first, first + count):
            flags = struct.unpack_from("<H", b, e)[0]
            if i in mapping:
                b[e + 2:e + 5] = bytes(hexc(mapping[i]))
                done += 1
            e += 6
            if flags & 1:
                e += 2 + struct.unpack_from("<H", b, e)[0]
    assert done == len(mapping), (path, done)
    open(path, "wb").write(b)


def read_palette(path):
    b = open(path, "rb").read()
    for _, c, _, ctype in walk_chunks(b):
        if ctype == 0x2019:
            count, first, _ = struct.unpack_from("<III", b, c + 6)
            e, pal = c + 26, []
            for _ in range(count):
                flags = struct.unpack_from("<H", b, e)[0]
                pal.append(tuple(b[e + 2:e + 5]))
                e += 6
                if flags & 1:
                    e += 2 + struct.unpack_from("<H", b, e)[0]
            return pal
    raise ValueError(path)


# '.'=0  cyan: a=2 b=3  magenta: m=8 n=10  gold: g=4 h=12 i=13  w=14 (cold white glint)
KEY = {".": 0, "a": 2, "b": 3, "m": 8, "n": 10, "g": 4, "h": 12, "i": 13, "w": 14}
GEMS = [
    # 1 XP: small cyan (4x4)
    ["........", "........", "...wb...", "..wbba..", "..bbaa..", "...aa...", "........", "........"],
    ["........", "....w...", "...ww...", "..wbbw..", "..bbaa..", "...aa...", "........", "........"],
    # 5 XP: magenta crystal (4x6) inside a dark magenta halo, like the style C sample gems
    ["..mmmm..", ".mmwnmm.", "mmwnnnmm", "mmnnnnmm", "mmnnnnmm", "mmmnnmmm", ".mmmnmm.", "..mmmm.."],
    ["..mmmm..", ".mmwwmm.", "mmwwnnmm", "mmwnnnmm", "mmnnnnmm", "mmmnnmmm", ".mmmnmm.", "..mmmm.."],
    # 25 XP: large gold (8x8)
    ["...ih...", "..iihh..", ".iihhhh.", "iihhhhhg", "hhhhhhgg", ".hhhhgg.", "..hggg..", "...gg..."],
    ["...wh...", "..wihh..", ".wiihhh.", "wiihhhhg", "hhhhhhgg", ".hhhhgg.", "..hggg..", "...gg..."],
]


def main():
    pj = os.path.join(ART, "palette_c.json")
    data = json.load(open(pj))
    for c in NEONS:
        if c not in data["master"]:
            data["master"].append(c)
    data["neons"] = NEONS + ["#ff3838", "#c81c28", "#ffa08c"]
    json.dump(data, open(pj, "w"), indent=1)
    for n in PICKUP_FILES:
        p = os.path.join(ART, n + ".aseprite")
        if n != "gems":
            set_entries(p, PICKUPS)
    for n in FX_FILES:
        set_entries(os.path.join(ART, n + ".aseprite"), FX)
    pal = read_palette(os.path.join(ART, "icons.aseprite"))
    frames = [[[KEY[ch] for ch in row] for row in g] for g in GEMS]
    aseprite_io.write_aseprite(os.path.join(ART, "gems.aseprite"), 8, 8, pal, frames, [200, 120] * 3, [])
    print("palette_c.json", len(data["master"]), "colours; pickups/fx entries set; gems.aseprite redrawn")


if __name__ == "__main__":
    main()
