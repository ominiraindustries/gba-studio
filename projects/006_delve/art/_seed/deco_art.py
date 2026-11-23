"""Wall decoration sprites for DELVE (deco_*): torch, banner, chains, lever, plus the torch glow.

Each deco is modelled as a few 3D primitives in WALL coordinates (u = 0..31 across the face, v = 0..63 down the
wall, 1 unit = 1 wall texel; w = out of the wall towards the viewer) and rendered by splatting with a z-buffer,
Lambert lighting quantised to the palette ramps, a cast shadow on the wall and a 1 px dark outline.
Views: front (face straight ahead) and left (face on the left wall, seen at a grazing angle: wall-parallel extent
squeezed to 0.32, protrusion drawn 1:1 to the right). right = mirror of left.

Every sprite is 32x64 = full wall height: row v of the sprite is wall texel row v (eye level at row 32), so the
engine scales it like the wall texture (scale = wall height in px / 64) and places the anchor (ANCHORS) on the face.

Writes art/_seed/out/deco_*_{shadow,object}.png (indexed strips) and prints the anchors.
Run: python -I deco_art.py
"""
import math
import os

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")
W, H = 32, 64


def hx(s):
    return tuple(int(s[i:i + 2], 16) for i in (1, 3, 5))


# Palette A (torch, chains, lever) and B (banner): 0 transparent, 1 outline, 2-3 wall shadow (stone s2/s3).
PAL_A = [hx(c) for c in ("#ff00ff", "#0b0807", "#352a23", "#4a3c32",
                         "#1a1c21", "#33363d", "#555962", "#8a8d94", "#d8b88e",     # 4-8 iron, warm highlight
                         "#33190d", "#6c3d1c", "#ab6f33",                           # 9-11 wood
                         "#a8360c", "#ee7a1c", "#ffc44a", "#fff3c0")]               # 12-15 flame
PAL_B = [hx(c) for c in ("#ff00ff", "#0b0807", "#352a23", "#4a3c32",
                         "#1a1c21", "#33363d", "#555962", "#8a8d94", "#d8b88e",     # 4-8 iron
                         "#3a0f12", "#6a1a1c", "#972b25", "#c4473a",                # 9-12 cloth
                         "#6b4a1e", "#a67c35", "#e0b860")]                          # 13-15 gold
PAL_G = [hx(c) for c in ("#ff00ff", "#80603e", "#966a3a", "#b07636", "#cc8834", "#e49e3e", "#f6ba56",
                         "#ffd67e")] + [(0, 0, 0)] * 8
IRON, WOOD, CHAR = [4, 5, 6, 7, 8], [9, 10, 11], [1, 9, 10]
CLOTH, GOLD, BLADE = [9, 10, 11, 12], [13, 14, 15], [5, 6, 7, 8]
OUTLINE, SH_CORE, SH_RIM = 1, 2, 3
DARKER = {8: 6, 7: 5, 6: 5, 5: 4, 4: 4, 11: 10, 10: 9, 9: 9}     # same slots in PAL_A and PAL_B for iron


def norm(a):
    m = math.sqrt(sum(c * c for c in a)) or 1.0
    return tuple(c / m for c in a)


def dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def cross(a, b):
    return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


class View:
    def __init__(self, kind, xl=16.0):
        self.kind, self.xl = kind, xl
        # direction TOWARDS the light (player's torch: front, upper left / nearer side of a lateral wall)
        self.light = norm((-0.45, -0.65, 0.62)) if kind == "front" else norm((-0.75, -0.5, 0.45))

    def proj(self, u, v, w):
        if self.kind == "front":
            k = 1 + 0.006 * w
            return 16 + (u - 16) * k, 32 + (v - 32) * k
        return self.xl + (u - 16) * 0.32 + w, 32 + (v - 32) * (1 - 0.008 * (u - 16))

    def near(self, u, w):
        return w if self.kind == "front" else w - 0.32 * u

    def shadow_point(self, u, v, w):
        L = self.light
        return u - L[0] * w / L[2], v - L[1] * w / L[2]


class Canvas:
    def __init__(self, view, flame_at=None):
        self.view = view
        self.col = [[0] * W for _ in range(H)]
        self.z = [[-1e9] * W for _ in range(H)]
        self.shadow = [[False] * W for _ in range(H)]
        self.flame_at = flame_at
        self.cast_plane = None            # w of the surface that receives the shadow of the parts drawn now
        self.oshadow = [[False] * W for _ in range(H)]
        self.wb = [[0.0] * W for _ in range(H)]

    def splat(self, u, v, w, n, ramp, cast=True, bias=0.0):
        x, y = self.view.proj(u, v, w)
        xi, yi = int(math.floor(x)), int(math.floor(y))
        if cast and w > 0.2:
            su, sv = self.view.shadow_point(u, v, w)
            sx, sy = self.view.proj(su, sv, 0)
            sxi, syi = int(math.floor(sx)), int(math.floor(sy))
            if 0 <= sxi < W and 0 <= syi < H:
                self.shadow[syi][sxi] = True
            if self.cast_plane is not None and w > self.cast_plane + 0.5:
                L = self.view.light
                k = (w - self.cast_plane) / L[2]
                ox, oy = self.view.proj(u - L[0] * k, v - L[1] * k, self.cast_plane)
                oxi, oyi = int(math.floor(ox)), int(math.floor(oy))
                if 0 <= oxi < W and 0 <= oyi < H:
                    self.oshadow[oyi][oxi] = True
        if not (0 <= xi < W and 0 <= yi < H):
            return
        z = self.view.near(u, w)
        if z < self.z[yi][xi]:
            return
        lam = max(0.0, dot(n, self.view.light))
        b = 0.1 + 0.78 * lam + bias
        if self.flame_at:
            d = (self.flame_at[0] - u, self.flame_at[1] - v, self.flame_at[2] - w)
            dist2 = dot(d, d)
            b += 0.75 * max(0.0, dot(n, norm(d))) / (1 + dist2 / 110.0)
        idx = ramp[max(0, min(len(ramp) - 1, int(b * len(ramp))))]
        self.z[yi][xi] = z
        self.wb[yi][xi] = w
        self.col[yi][xi] = idx

    # ---------------------------------------------------------------- primitives
    def box(self, u0, u1, v0, v1, w0, w1, ramp, step=0.2, bias=0.0):
        faces = [((0, 0, 1), lambda a, b: (a, b, w1), (u0, u1), (v0, v1)),
                 ((0, -1, 0), lambda a, b: (a, v0, b), (u0, u1), (w0, w1)),
                 ((0, 1, 0), lambda a, b: (a, v1, b), (u0, u1), (w0, w1)),
                 ((-1, 0, 0), lambda a, b: (u0, a, b), (v0, v1), (w0, w1)),
                 ((1, 0, 0), lambda a, b: (u1, a, b), (v0, v1), (w0, w1))]
        for n, f, ra, rb in faces:
            a = ra[0]
            while a <= ra[1]:
                b = rb[0]
                while b <= rb[1]:
                    self.splat(*f(a, b), n, ramp, bias=bias)
                    b += step
                a += step

    def cyl(self, p0, p1, r0, ramp, r1=None, caps=True):
        r1 = r0 if r1 is None else r1
        axis = tuple(p1[i] - p0[i] for i in range(3))
        length = math.sqrt(dot(axis, axis))
        ax = norm(axis)
        ref = (0, 0, 1) if abs(ax[2]) < 0.9 else (1, 0, 0)
        e1 = norm(cross(ax, ref))
        e2 = cross(ax, e1)
        steps = max(2, int(length / 0.15))
        for i in range(steps + 1):
            t = i / steps
            r = r0 + (r1 - r0) * t
            c = tuple(p0[k] + axis[k] * t for k in range(3))
            na = max(12, int(r * 14))
            for j in range(na):
                th = 2 * math.pi * j / na
                n = tuple(e1[k] * math.cos(th) + e2[k] * math.sin(th) for k in range(3))
                self.splat(c[0] + n[0] * r, c[1] + n[1] * r, c[2] + n[2] * r, n, ramp)
        if caps:
            for c, r, sgn in ((p0, r0, -1), (p1, r1, 1)):
                rr = 0.0
                while rr <= r:
                    na = max(6, int(rr * 14))
                    for j in range(na):
                        th = 2 * math.pi * j / na
                        o = tuple((e1[k] * math.cos(th) + e2[k] * math.sin(th)) * rr for k in range(3))
                        self.splat(c[0] + o[0], c[1] + o[1], c[2] + o[2], tuple(a * sgn for a in ax), ramp)
                    rr += 0.15

    def sphere(self, c, r, ramp, bias=0.0):
        nt = max(8, int(r * 10))
        for i in range(nt + 1):
            th = math.pi * i / nt
            nphi = max(8, int(r * 20 * math.sin(th)) + 1)
            for j in range(nphi):
                ph = 2 * math.pi * j / nphi
                n = (math.sin(th) * math.cos(ph), -math.cos(th), math.sin(th) * math.sin(ph))
                self.splat(c[0] + n[0] * r, c[1] + n[1] * r, c[2] + n[2] * r, n, ramp, bias=bias)

    def torus(self, c, axis, R, r, ramp, Rb=None, e1=None):
        """Ring around `axis`; Rb stretches it along e1 (chain links)."""
        ax = norm(axis)
        if e1 is None:
            ref = (0, 1, 0) if abs(ax[1]) < 0.9 else (1, 0, 0)
            e1 = norm(cross(ax, ref))
        e1 = norm(e1)
        e2 = cross(ax, e1)
        Rb = R if Rb is None else Rb
        na = max(16, int(max(R, Rb) * 18))
        for i in range(na):
            th = 2 * math.pi * i / na
            radial = tuple(e1[k] * math.cos(th) * Rb + e2[k] * math.sin(th) * R for k in range(3))
            rn = norm(tuple(e1[k] * math.cos(th) * R + e2[k] * math.sin(th) * Rb for k in range(3)))
            for j in range(12):
                ph = 2 * math.pi * j / 12
                n = tuple(rn[k] * math.cos(ph) + ax[k] * math.sin(ph) for k in range(3))
                self.splat(c[0] + radial[0] + n[0] * r, c[1] + radial[1] + n[1] * r, c[2] + radial[2] + n[2] * r,
                           n, ramp)

    def surface(self, fn, u0, u1, v0, v1, step=0.15):
        """fn(u, v) -> None or (du, w, normal, ramp)."""
        u = u0
        while u <= u1:
            v = v0
            while v <= v1:
                r = fn(u, v)
                if r:
                    du, w, n, ramp = r
                    self.splat(u + du, v, w, n, ramp)
                v += step
            u += step

    # ---------------------------------------------------------------- finishing
    def flame(self, bx, by, Hf, Wf, phase, amp, lean, tongue=None):
        layers = [(12, 1.0, 1.0), (13, 0.78, 0.86), (14, 0.54, 0.64), (15, 0.28, 0.36)]
        fl = [[0] * W for _ in range(H)]
        for color, sw, sh in layers:
            hh = Hf * sh
            for yi in range(H):
                h = by - (yi + 0.5)
                if h < -1.5 or h > hh:
                    continue
                t = max(0.0, h) / hh
                prof = (1 - t) ** 0.75 * (0.6 + 0.4 * min(1.0, t / 0.25))
                hw = Wf / 2 * sw * prof
                if h < 0:
                    hw *= 0.75
                tt = max(0.0, h) / Hf
                c = bx + lean * tt * 3 + amp * tt * math.sin(phase + tt * 4.0)
                for xi in range(W):
                    if abs(xi + 0.5 - c) <= hw:
                        fl[yi][xi] = color
        if tongue:
            tx, ty = tongue
            for yy in range(ty, ty + 3):
                for xx in range(tx, tx + 2):
                    if 0 <= yy < H and 0 <= xx < W and fl[yy][xx] == 0:
                        fl[yy][xx] = 13 if yy > ty else 12
        return fl

    cast_z = None

    def finish(self, flame=None):
        # close the dotted cast-shadow mask (no 1 px holes)
        os_ = self.oshadow
        self.oshadow = [[os_[y][x] or ((0 < x < W - 1 and os_[y][x - 1] and os_[y][x + 1]) or
                                       (0 < y < H - 1 and os_[y - 1][x] and os_[y + 1][x]))
                         for x in range(W)] for y in range(H)]
        # despeckle: an object pixel unlike all 4 neighbours takes the most common neighbour colour
        col = [r[:] for r in self.col]
        for y in range(1, H - 1):
            for x in range(1, W - 1):
                c = self.col[y][x]
                nb = [self.col[y + dy][x + dx] for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))]
                if c and all(n != c for n in nb) and all(n for n in nb):
                    col[y][x] = max(set(nb), key=nb.count)
        self.col = col
        obj = [[self.col[y][x] != 0 for x in range(W)] for y in range(H)]
        if flame:
            for y in range(H):
                for x in range(W):
                    if flame[y][x]:
                        obj[y][x] = False
        # shadow (core + 1 px softer rim), then outline, then object, then flame
        sh = [[0] * W for _ in range(H)]
        for y in range(H):
            for x in range(W):
                if self.shadow[y][x]:
                    sh[y][x] = SH_CORE
        for y in range(H):
            for x in range(W):
                if sh[y][x] == 0 and any(0 <= x + dx < W and 0 <= y + dy < H and sh[y + dy][x + dx] == SH_CORE
                                         for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    sh[y][x] = SH_RIM
        obj_layer = [[0] * W for _ in range(H)]
        for y in range(H):
            for x in range(W):
                if obj[y][x]:
                    obj_layer[y][x] = self.col[y][x]
                    if self.oshadow[y][x] and self.cast_z is not None and abs(self.wb[y][x] - self.cast_z) < 0.35:
                        obj_layer[y][x] = DARKER.get(self.col[y][x], self.col[y][x])
                elif any(0 <= x + dx < W and 0 <= y + dy < H and obj[y + dy][x + dx]
                         for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    obj_layer[y][x] = OUTLINE
                if flame and flame[y][x]:
                    obj_layer[y][x] = flame[y][x]
        for y in range(H):
            for x in range(W):
                if obj_layer[y][x]:
                    sh[y][x] = 0
        return sh, obj_layer


# ==================================================================== models (wall coordinates)
FLAME_FRAMES = [  # height, width, phase, sway, tongue (u offset, v offset from tip) - irregular rhythm
    (17, 12.0, 0.0, 1.2, None), (20, 11.0, 1.9, 2.2, (1, -4)), (15, 13.0, 3.4, 0.8, None), (18.5, 11.5, 5.0, 1.8, None)]
FLAME_MS = [110, 70, 140, 90]
FLAME_POS = (16, 12, 10.5)


def model_torch(cv):
    cv.box(12.5, 19.5, 36, 50, 0, 1.2, IRON, bias=-0.12)      # wall plate (darker than the bracket)
    cv.box(14, 18, 37.5, 48.5, 1.2, 2.0, IRON, bias=-0.05)    # raised boss
    cv.cast_plane, cv.cast_z = 2.0, 2.0
    cv.cyl((16, 44, 2.0), (16, 44, 6.0), 1.2, IRON)           # arm out of the wall
    cv.sphere((16, 44, 6.4), 1.7, IRON)                       # knuckle
    cv.cyl((16, 43.5, 6.8), (16, 35, 9.6), 1.1, IRON)         # arm up to the cup
    cv.sphere((16, 39.5, 8.4), 1.25, IRON)                    # collar
    cv.cyl((16, 35, 10), (16, 29, 10), 1.5, IRON, r1=3.2)     # cup (frustum)
    cv.torus((16, 29, 10), (0, 1, 0), 3.2, 0.7, IRON, e1=(1, 0, 0))   # cup rim
    cv.cyl((16, 32, 10), (16, 21.5, 10.3), 1.5, WOOD)         # stick
    cv.cyl((16, 21.5, 10.3), (16, 19.5, 10.4), 1.5, CHAR)     # charred top
    cv.cast_plane = None


def flame_for(cv, frame, lean):
    Hf, Wf, ph, amp, tongue = FLAME_FRAMES[frame]
    bx, by = cv.view.proj(16, 20, 10.4)
    tg = None
    if tongue:
        tg = (int(bx + tongue[0] + lean * 3), int(by - Hf + tongue[1]))
    return cv.flame(bx, by, Hf, Wf, ph, amp, lean, tg)


def model_banner(cv, frame):
    for uu in (7, 25):
        cv.box(uu - 1.5, uu + 1.5, 3.5, 6.5, 0, 0.8, IRON)
        cv.cyl((uu, 5, 0.8), (uu, 5, 4.5), 0.8, IRON)
    cv.cyl((3.2, 5, 4.5), (28.8, 5, 4.5), 1.0, IRON)
    cv.sphere((2.4, 5, 4.5), 1.6, IRON)
    cv.sphere((29.6, 5, 4.5), 1.6, IRON)
    amp_sway = 1.0 if frame == 0 else -1.0
    ph = 0.0 if frame == 0 else 0.9

    def fn(u, v):
        bottom = 46 + 7 * min(1.0, abs(u - 16) / 10)
        if v < 6.2 or v > bottom:
            return None
        t = (v - 6.2) / 44
        k = 2 * math.pi / 8
        a = 0.8 * (0.3 + 0.7 * t)
        w = 4.2 - 1.2 * t + a * math.sin(k * (u - 6) + ph)
        dwdu = a * k * math.cos(k * (u - 6) + ph)
        n = norm((-dwdu, 0.15, 1))
        du = amp_sway * t * t * 1.6
        r = (u - 16) ** 2 + ((v - 24) * 1.0) ** 2
        if u < 7.6 or u > 24.4 or v < 8.4 or bottom - 5 < v < bottom - 3.2:
            ramp = GOLD
        elif 30 <= v <= 31.8 and abs(u - 16) < 4.2:
            ramp = GOLD                                           # cross guard
        elif abs(u - 16) < 1.2 and 14.5 < v < 39:
            ramp = BLADE                                          # blade
        elif 4.4 ** 2 <= r <= 6.2 ** 2:
            ramp = GOLD                                           # ring
        else:
            ramp = CLOTH
        return du, w, n, ramp

    cv.surface(fn, 6, 26, 6.2, 53.5)


def link_chain(cv, pts, w_of, spacing=2.3):
    """Alternating chain links along a polyline in (u, v); w_of(s) gives the distance from the wall."""
    seg = []
    total = 0.0
    for a, b in zip(pts, pts[1:]):
        L = math.dist(a, b)
        seg.append((a, b, total, L))
        total += L
    s, k = 0.0, 0
    while s <= total:
        for a, b, s0, L in seg:
            if s0 <= s <= s0 + L:
                t = (s - s0) / L
                u, v = a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t
                d = norm((b[0] - a[0], b[1] - a[1], 0))
                break
        w = w_of(s / total)
        side = norm(cross(d, (0, 0, 1)))
        axis = (0, 0, 1) if k % 2 == 0 else side       # flat link, then edge-on link
        cv.torus((u, v, w), axis, 0.95, 0.55, IRON, Rb=1.6, e1=d)
        s += spacing
        k += 1


def manacle(cv, c):
    cv.sphere((c[0], c[1] - 3.6, c[2]), 1.1, IRON)                     # hinge
    cv.torus(c, (0.6, 0.15, 0.78), 3.0, 0.85, IRON)                   # cuff
    cv.box(c[0] - 0.9, c[0] + 0.9, c[1] + 2.4, c[1] + 4.2, c[2] - 0.6, c[2] + 0.9, IRON, step=0.15)   # lock


def model_chains(cv):
    for uu in (9, 23):
        cv.box(uu - 2.2, uu + 2.2, 8, 12.4, 0, 1.0, IRON)
        cv.torus((uu, 13.4, 1.8), (1, 0, 0), 1.5, 0.6, IRON, e1=(0, 1, 0))   # eye bolt ring
    sag = [(9 + 14 * i / 10, 15 + 9 * math.sin(math.pi * i / 10)) for i in range(11)]
    link_chain(cv, sag, lambda t: 2.4 + 0.6 * math.sin(math.pi * t))
    link_chain(cv, [(9, 15.5), (8.6, 28), (8.2, 38.5)], lambda t: 2.4 + 0.5 * t)
    link_chain(cv, [(23, 15.5), (23.6, 32), (24.0, 45.5)], lambda t: 2.4 + 0.6 * t)
    manacle(cv, (8.0, 43.6, 3.0))
    manacle(cv, (24.2, 50.8, 3.1))


def model_lever(cv, up):
    b = -0.14
    cv.box(10, 15, 22, 42, 0, 1.6, IRON, bias=b)
    cv.box(17, 22, 22, 42, 0, 1.6, IRON, bias=b)
    cv.box(15, 17, 22, 24.5, 0, 1.6, IRON, bias=b)
    cv.box(15, 17, 39.5, 42, 0, 1.6, IRON, bias=b)
    cv.box(15, 17, 24.5, 39.5, 0, 0.3, [1, 4, 4])             # slot floor (dark)
    for uu in (11.6, 20.4):
        for vv in (23.6, 40.4):
            cv.sphere((uu, vv, 1.6), 0.95, IRON)              # rivets
    cv.cast_plane, cv.cast_z = 1.6, 1.6
    cv.cyl((16, 32, 1.6), (16, 32, 3.2), 2.7, IRON)           # pivot boss
    tip = (16, 16.5, 6.4) if up else (16, 47.5, 6.4)
    base = (16, 32, 3.4)
    cv.cyl(base, tip, 1.0, IRON)
    g0 = tuple(base[k] + (tip[k] - base[k]) * 0.5 for k in range(3))
    cv.cyl(g0, tip, 1.55, WOOD)                               # wooden grip
    cv.sphere(tip, 1.9, IRON)                                 # iron knob
    cv.sphere((16, 32, 3.6), 1.2, IRON)                       # axle cap
    cv.cast_plane = None


# ==================================================================== build
DECOS = {  # name: (model, frames, xl for the lateral view, palette, anchor (u, v) on the wall)
    "torch": (lambda cv, f: model_torch(cv), 4, 6.0, PAL_A, (16, 43)),
    "banner": (model_banner, 2, 9.0, PAL_B, (16, 5)),
    "chains": (lambda cv, f: model_chains(cv), 1, 9.0, PAL_A, (16, 10)),
    "lever": (lambda cv, f: model_lever(cv, f == 0), 2, 9.0, PAL_A, (16, 32)),
}


def render(name, view_kind, frame):
    model, frames, xl, pal, anchor = DECOS[name]
    view = View(view_kind, xl)
    cv = Canvas(view, FLAME_POS if name == "torch" else None)
    model(cv, frame)
    fl = flame_for(cv, frame, 0.0 if view_kind == "front" else 0.8) if name == "torch" else None
    return cv.finish(fl)


def save_strip(path, frames, pal):
    fw, fh = len(frames[0][0]), len(frames[0])
    im = Image.new("P", (fw, fh * len(frames)))
    flat = []
    for c in pal + [(0, 0, 0)] * (16 - len(pal)):
        flat += list(c)
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.putdata([v for fr in frames for row in fr for v in row])
    im.save(path)


def glow():
    g = [[0] * 64 for _ in range(64)]
    for y in range(64):
        for x in range(64):
            r = math.hypot(x + 0.5 - 32, (y + 0.5 - 32) * 1.08) / 31.5
            if r < 1:
                g[y][x] = 1 + min(6, int((1 - r) ** 1.25 * 7))
    return g


def mirror(fr):
    return [row[::-1] for row in fr]


def main():
    os.makedirs(OUT, exist_ok=True)
    print("anchors (sprite px of the wall contact point; 32x64 sprites, row = wall texel row):")
    for name, (model, frames, xl, pal, anchor) in DECOS.items():
        for kind in ("front", "left"):
            shs, objs = [], []
            for f in range(frames):
                sh, ob = render(name, kind, f)
                shs.append(sh)
                objs.append(ob)
            ax, ay = View(kind, xl).proj(anchor[0], anchor[1], 0)
            outs = [(kind, shs, objs, ax)]
            if kind == "left":
                outs.append(("right", [mirror(s) for s in shs], [mirror(o) for o in objs], W - ax))
            for k, s, o, axx in outs:
                base = "deco_%s_%s" % (name, k)
                save_strip(os.path.join(OUT, base + "_shadow.png"), s, pal)
                save_strip(os.path.join(OUT, base + "_object.png"), o, pal)
                print("  %-20s frames %d  anchor (%.1f, %.1f)" % (base, frames, axx, ay))
    save_strip(os.path.join(OUT, "deco_torch_glow_object.png"), [glow()], PAL_G)
    print("  deco_torch_glow       64x64, centre on the flame")


if __name__ == "__main__":
    main()
