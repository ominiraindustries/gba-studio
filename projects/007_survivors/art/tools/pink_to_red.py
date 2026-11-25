"""G-65: replace the neon pinks/magentas with reds, palette entries only (same indices, pixels, frames, tags).

    python -I art/tools/pink_to_red.py           # recolour + report (asserts the expected pinks were found)
    python -I art/tools/pink_to_red.py --check   # report only (remaining pink-hue colours in graphics/*.bmp)
    python -I art/tools/build_art.py export      # then re-export graphics/ from the .aseprite files

What it touches:
  - art/*.aseprite: palette chunks (0x2019, and old 0x0004 / 0x0011 if present), patched in place byte for byte.
  - art/palette_c.json ("master", "neons", "wizard_bright", ...): hex strings.
  - art/tools/*.py generator constants: hex strings, so re-running a generator does not bring the pink back.
  - graphics/*.bmp palettes: in place (covers BMPs that are not exported from an .aseprite).
Exact map below; any other pink-hue entry (hue 285-335, sat > 0.35, val > 0.40) in an .aseprite/BMP is rotated to
hue 2 deg keeping saturation and value. Index 0 (transparent) and dark purples (val <= 0.40, e.g. #46155a) are kept.
"""
import colorsys
import glob
import json
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
GFX = os.path.join(os.path.dirname(ART), "graphics")

MAP = {
    "#fd3ffd": "#ff3838",  # magenta -> neon red
    "#e020c0": "#c81c28",  # fuchsia -> deep red
    "#ff9cf0": "#ffa08c",  # light pink -> light red (hot core highlight)
    "#ff70ff": "#ff5a48",  # wizard_bright crystal / glow
}


KEEP = {"#6c385a"}  # dark mauve text shadow (text_*.bmp): reads dark, not pink; left as is


def hx(c):
    return "#%02x%02x%02x" % c


def rgb(h):
    return tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))


def hsv(c):
    h, s, v = colorsys.rgb_to_hsv(*[x / 255 for x in c])
    return h * 360, s, v


def recolour(c):
    """New colour for c, or None to keep it."""
    h = hx(c)
    if h in MAP:
        return rgb(MAP[h])
    if h in KEEP:
        return None
    hue, s, v = hsv(c)
    if 285 <= hue <= 335 and s > 0.35 and v > 0.40:
        r, g, b = colorsys.hsv_to_rgb(2 / 360, s, v)
        return tuple(round(x * 255) for x in (r, g, b))
    return None


def patch_aseprite(path, changes):
    d = bytearray(open(path, "rb").read())
    nframes = struct.unpack_from("<H", d, 6)[0]
    off = 128
    for _ in range(nframes):
        fsize, _magic, old = struct.unpack_from("<IHH", d, off)
        new = struct.unpack_from("<I", d, off + 12)[0]
        c = off + 16
        for _ in range(new or old):
            csize, ctype = struct.unpack_from("<IH", d, c)
            if ctype == 0x2019:
                _n, first, last = struct.unpack_from("<III", d, c + 6)
                q = c + 26
                for i in range(first, last + 1):
                    flags = struct.unpack_from("<H", d, q)[0]
                    col = tuple(d[q + 2:q + 5])
                    nc = recolour(col) if i else None
                    if nc:
                        d[q + 2:q + 5] = bytes(nc)
                        changes.append((i, hx(col), hx(nc)))
                    q += 6
                    if flags & 1:
                        q += 2 + struct.unpack_from("<H", d, q)[0]
            elif ctype in (0x0004, 0x0011):
                scale = 4 if ctype == 0x0011 else 1  # 0x0011 stores 0-63
                q = c + 6
                packets = struct.unpack_from("<H", d, q)[0]
                q += 2
                idx = 0
                for _ in range(packets):
                    idx += d[q]
                    n = d[q + 1] or 256
                    q += 2
                    for k in range(n):
                        col = tuple(min(255, x * scale) for x in d[q:q + 3])
                        nc = recolour(col) if idx + k else None
                        if nc:
                            d[q:q + 3] = bytes(x // scale for x in nc)  # old chunk: Aseprite reads 0x2019 first
                        q += 3
                    idx += n
            c += csize
        off += fsize
    return bytes(d)


def bmp_palette(d):
    bsize = struct.unpack_from("<I", d, 14)[0]
    bpp = struct.unpack_from("<H", d, 28)[0]
    n = struct.unpack_from("<I", d, 46)[0] or (1 << bpp if bpp <= 8 else 0)
    return 14 + bsize, n


def patch_bmp(path, changes):
    d = bytearray(open(path, "rb").read())
    o, n = bmp_palette(d)
    for i in range(1, n):
        col = (d[o + 4 * i + 2], d[o + 4 * i + 1], d[o + 4 * i])
        nc = recolour(col)
        if nc:
            d[o + 4 * i:o + 4 * i + 3] = bytes((nc[2], nc[1], nc[0]))
            changes.append((i, hx(col), hx(nc)))
    return bytes(d)


def patch_text(path, changes):
    s = open(path, encoding="utf-8").read()
    t = s
    for a, b in MAP.items():
        k = len(re.findall(re.escape(a), t, re.I))
        if k:
            t = re.sub(re.escape(a), b, t, flags=re.I)
            changes.append((k, a, b))
    return s, t


def remaining():
    """Pink-hue colours (hue 280-330, sat > 0.35, val > 0.35) left in graphics/*.bmp palettes."""
    left = {}
    for p in sorted(glob.glob(os.path.join(GFX, "*.bmp"))):
        d = open(p, "rb").read()
        o, n = bmp_palette(d)
        for i in range(n):
            col = (d[o + 4 * i + 2], d[o + 4 * i + 1], d[o + 4 * i])
            h, s, v = hsv(col)
            if 280 <= h <= 330 and s > 0.35 and v > 0.35:
                left.setdefault(hx(col), set()).add((os.path.basename(p), i))
    return left


# Intentional leftovers: index-0 transparent key of the tmp_* placeholders, the dark violet of the night skies and
# outlines (val 0.353, just over the report threshold), the dark mauve of the text palettes (val 0.42).
INTENTIONAL = {"#ff00ff": "index 0 (transparent) of tmp_* placeholders",
               "#46155a": "dark violet (night skies, outlines): not pink",
               "#6c385a": "dark mauve text shadow: not pink"}


def main():
    check = "--check" in sys.argv
    found = {k: 0 for k in MAP}
    total = 0
    if not check:
        for p in sorted(glob.glob(os.path.join(ART, "*.aseprite"))):
            ch = []
            data = patch_aseprite(p, ch)
            if ch:
                open(p, "wb").write(data)
                total += len(ch)
                for _i, a, _b in ch:
                    if a in found:
                        found[a] += 1
                print(f"{os.path.basename(p):32s} " + " ".join(f"{i}:{a}->{b}" for i, a, b in ch))
        for p in sorted(glob.glob(os.path.join(GFX, "*.bmp"))):
            ch = []
            data = patch_bmp(p, ch)
            if ch:
                open(p, "wb").write(data)
                print(f"graphics/{os.path.basename(p):23s} " + " ".join(f"{i}:{a}->{b}" for i, a, b in ch))
        texts = [os.path.join(ART, "palette_c.json")] + sorted(glob.glob(os.path.join(HERE, "*.py")))
        for p in texts:
            if os.path.basename(p) == "pink_to_red.py":
                continue
            ch = []
            s, t = patch_text(p, ch)
            if t != s:
                open(p, "w", encoding="utf-8", newline="").write(t)
                print(f"{os.path.relpath(p, ART):32s} " + " ".join(f"{a}->{b} x{k}" for k, a, b in ch))
        json.load(open(os.path.join(ART, "palette_c.json")))  # still valid JSON
        print("aseprite entries changed:", total, "by source colour:", found)
        if total == 0:
            print("nothing to do (already red?)")
        else:
            missing = [k for k, v in found.items() if v == 0]
            assert not missing, f"expected pinks not found in any .aseprite: {missing}"
    left = remaining()
    print("remaining pink-hue colours in graphics/*.bmp:")
    bad = 0
    for col, where in sorted(left.items()):
        note = INTENTIONAL.get(col)
        bad += note is None
        names = sorted({w[0] for w in where})
        print(f"  {col} {'(intentional: ' + note + ')' if note else 'UNEXPECTED'} {len(names)} files: "
              f"{', '.join(names[:5])}{' ...' if len(names) > 5 else ''}")
    print("unexpected:", bad)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
