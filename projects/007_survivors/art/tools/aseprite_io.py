"""Minimal writer for indexed .aseprite files (one layer, frames, tags) plus 4bpp BMP writer.

The .aseprite is the editable source; BMPs for Butano are exported from it (see forest_art.py export).
"""
import struct
import zlib


def _string(s):
    b = s.encode("utf-8")
    return struct.pack("<H", len(b)) + b


def _chunk(kind, data):
    return struct.pack("<IH", len(data) + 6, kind) + data


def write_aseprite(path, width, height, palette, frames, durations, tags, layer_name="art"):
    """palette: list of (r,g,b); index 0 is transparent. frames: list of 2D lists [y][x] of indices.
    durations: ms per frame. tags: list of (name, from, to)."""
    frame_blobs = []
    for fi, pixels in enumerate(frames):
        chunks = []
        if fi == 0:
            chunks.append(_chunk(0x2004, struct.pack("<HHHHHHB3x", 3, 0, 0, 0, 0, 0, 255) + _string(layer_name)))
            pal = struct.pack("<III8x", len(palette), 0, len(palette) - 1)
            for i, (r, g, b) in enumerate(palette):
                pal += struct.pack("<HBBBB", 0, r, g, b, 0 if i == 0 else 255)
            chunks.append(_chunk(0x2019, pal))
            if tags:
                t = struct.pack("<H8x", len(tags))
                for name, a, b in tags:
                    t += struct.pack("<HHBH6xBBBx", a, b, 0, 0, 0, 0, 0) + _string(name)
                chunks.append(_chunk(0x2018, t))
        raw = bytes(v for row in pixels for v in row)
        cel = struct.pack("<HhhBHh5xHH", 0, 0, 0, 255, 2, 0, width, height) + zlib.compress(raw, 9)
        chunks.append(_chunk(0x2005, cel))
        body = b"".join(chunks)
        header = struct.pack("<IHHHxxI", len(body) + 16, 0xF1FA, min(len(chunks), 0xFFFF),
                             durations[fi], len(chunks))
        frame_blobs.append(header + body)
    frames_data = b"".join(frame_blobs)
    head = struct.pack("<IHHHHHIHII", 128 + len(frames_data), 0xA5E0, len(frames), width, height, 8, 1, 100, 0, 0)
    head += struct.pack("<B3xHBBhhHH", 0, len(palette), 1, 1, 0, 0, 16, 16)
    head += b"\0" * (128 - len(head))
    with open(path, "wb") as f:
        f.write(head + frames_data)


def write_bmp4(path, width, height, palette, pixels):
    """Uncompressed 4bpp indexed BMP (16-colour palette, bottom-up)."""
    assert width % 8 == 0 and height % 8 == 0
    pal = list(palette) + [(0, 0, 0)] * (16 - len(palette))
    row_bytes = ((width + 1) // 2 + 3) & ~3
    data = bytearray()
    for y in range(height - 1, -1, -1):
        row = bytearray()
        for x in range(0, width, 2):
            a = pixels[y][x]
            b = pixels[y][x + 1]
            assert a < 16 and b < 16, (path, x, y)
            row.append((a << 4) | b)
        row += b"\0" * (row_bytes - len(row))
        data += row
    pal_bytes = b"".join(struct.pack("<BBBx", b, g, r) for (r, g, b) in pal)
    offset = 14 + 40 + len(pal_bytes)
    with open(path, "wb") as f:
        f.write(b"BM" + struct.pack("<IHHI", offset + len(data), 0, 0, offset))
        f.write(struct.pack("<IiiHHIIiiII", 40, width, height, 1, 4, 0, len(data), 2835, 2835, 16, 16))
        f.write(pal_bytes + data)
