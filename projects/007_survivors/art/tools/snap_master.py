"""Snap the palette of art/*.aseprite to the style C master palette (art/palette_c.json).

Only palette entries change (each colour -> nearest master colour); indices, frames and tags stay as they are,
so shared palettes stay identical and fixed-index meanings are kept. Index 0 stays transparent.

    python -I art/tools/snap_master.py            # every art/*.aseprite (top level only)
    python -I art/tools/snap_master.py wizard bat # only these
Then: python -I art/tools/build_art.py export
"""
import glob
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ART = os.path.dirname(HERE)
MASTER = [tuple(int(h[i:i + 2], 16) for i in (1, 3, 5))
          for h in json.load(open(os.path.join(ART, "palette_c.json")))["master"]]


def near(c):
    return min(MASTER, key=lambda m: sum((a - b) ** 2 for a, b in zip(c, m)))


def snap_file(path):
    b = bytearray(open(path, "rb").read())
    nframes = struct.unpack_from("<H", b, 6)[0]
    pos, changed = 128, 0
    for _ in range(nframes):
        fsize = struct.unpack_from("<I", b, pos)[0]
        nchunks = struct.unpack_from("<H", b, pos + 6)[0]
        n4 = struct.unpack_from("<I", b, pos + 12)[0]
        nchunks = n4 if n4 else nchunks
        c = pos + 16
        for _ in range(nchunks):
            csize, ctype = struct.unpack_from("<IH", b, c)
            if ctype == 0x2019:
                count, first, _last = struct.unpack_from("<III", b, c + 6)
                e = c + 6 + 20
                for i in range(first, first + count):
                    flags = struct.unpack_from("<H", b, e)[0]
                    if i > 0:
                        rgb = tuple(b[e + 2:e + 5])
                        n = near(rgb)
                        if n != rgb:
                            b[e + 2:e + 5] = bytes(n)
                            changed += 1
                    e += 6
                    if flags & 1:
                        e += 2 + struct.unpack_from("<H", b, e)[0]
            elif ctype in (0x0004, 0x0011):
                packets = struct.unpack_from("<H", b, c + 6)[0]
                e, idx = c + 8, 0
                for _ in range(packets):
                    skip, n = b[e], b[e + 1] or 256
                    idx += skip
                    e += 2
                    for _ in range(n):
                        if idx > 0:
                            scale = 4 if ctype == 0x0011 else 1
                            rgb = tuple(min(255, v * scale) for v in b[e:e + 3])
                            nn = near(rgb)
                            b[e:e + 3] = bytes(v // scale for v in nn)
                        idx += 1
                        e += 3
            c += csize
        pos += fsize
    open(path, "wb").write(b)
    return changed


def main(argv):
    names = argv or [os.path.splitext(os.path.basename(p))[0] for p in sorted(glob.glob(os.path.join(ART, "*.aseprite")))]
    for n in names:
        print(f"{n:14s} {snap_file(os.path.join(ART, n + '.aseprite'))} colours snapped")


if __name__ == "__main__":
    main(sys.argv[1:])
