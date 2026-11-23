"""G-33 dark-retro style samples: procedural 'painted' textures + skeleton + torch for 3 variants."""
import numpy as np

BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def bayer(h, w):
    return np.tile(BAYER, (h // 4 + 1, w // 4 + 1))[:h, :w]


def ramp(c0, c1, n, mid=None, gamma=1.0):
    out = []
    for i in range(n):
        t = (i / (n - 1)) ** gamma
        if mid is not None:
            a, b, tt = (c0, mid, t * 2) if t < 0.5 else (mid, c1, t * 2 - 1)
        else:
            a, b, tt = c0, c1, t
        out.append(tuple(int(round(a[k] + (b[k] - a[k]) * tt)) for k in range(3)))
    return out


def vnoise(rng, h, w, cy, cx):
    gh, gw = max(1, h // cy), max(1, w // cx)
    g = rng.random((gh, gw))
    y = np.arange(h) / cy
    x = np.arange(w) / cx
    y0, x0 = np.floor(y).astype(int), np.floor(x).astype(int)
    fy, fx = y - y0, x - x0
    fy, fx = fy * fy * (3 - 2 * fy), fx * fx * (3 - 2 * fx)
    y0, x0 = y0 % gh, x0 % gw
    y1, x1 = (y0 + 1) % gh, (x0 + 1) % gw
    a = g[y0][:, x0]
    b = g[y0][:, x1]
    c = g[y1][:, x0]
    d = g[y1][:, x1]
    top = a + (b - a) * fx[None, :]
    bot = c + (d - c) * fx[None, :]
    return top + (bot - top) * fy[:, None]


def fbm(rng, h, w, cells=((16, 16, .45), (8, 8, .27), (4, 4, .17), (2, 2, .11))):
    v = sum(wt * vnoise(rng, h, w, cy, cx) for cy, cx, wt in cells)
    return v / sum(c[2] for c in cells)


def quant(v, n, dith=0.85, b=None):
    """v in 0..1 -> level 0..n-1 with ordered dither."""
    if b is None:
        b = bayer(*v.shape)
    return np.clip(np.floor(v * n + (b - 0.5) * dith), 0, n - 1).astype(int)


# ------------------------------------------------------------------------------------------------ layouts
def courses(rng, heights, wmin, wmax, size=64, joint2=0.25, jitter=0):
    ids = np.zeros((size, size), int)
    M = np.zeros((size, size), bool)
    y, nid = 0, 0
    for h in heights:
        mw = 2 if rng.random() < joint2 else 1
        M[y:y + mw, :] = True
        start = int(rng.integers(0, size))
        bounds, x = [], 0
        while True:
            w = int(rng.integers(wmin, wmax + 1))
            if size - (x + w) < wmin:
                break
            x += w
            bounds.append(x)
        rel_b = np.array([0] + bounds)
        for xx in range(size):
            rel = (xx - start) % size
            ids[y:y + h, xx] = nid + int((rel_b <= rel).sum())
        for b in rel_b:
            bx = (start + b) % size
            j = int(rng.integers(-jitter, jitter + 1)) if jitter else 0
            M[y:y + h, (bx + j) % size] = True
            if rng.random() < joint2 * 0.6:
                M[y:y + h, (bx + j + 1) % size] = True
        nid += len(rel_b) + 1
        y += h
    return ids, M


def edge_dist(M, cap=4):
    """distances (top, bottom, left, right) to the nearest mortar pixel, wrapping."""
    out = []
    for axis, sgn in ((0, 1), (0, -1), (1, 1), (1, -1)):
        d = np.full(M.shape, cap)
        for k in range(cap, 0, -1):
            d[np.roll(M, sgn * k, axis=axis)] = k
        out.append(d)
    return out


def walk_crack(rng, V, M, x, y, length, dark=0.10, lit=0.07):
    h, w = V.shape
    dx = rng.choice([-1, 1])
    for _ in range(length):
        if M[y % h, x % w]:
            break
        V[y % h, x % w] = dark
        if not M[(y + 1) % h, x % w]:
            V[(y + 1) % h, x % w] = min(1.0, V[(y + 1) % h, x % w] + lit)
        r = rng.random()
        if r < 0.6:
            y += 1
        elif r < 0.8:
            x += dx
        else:
            dx = -dx
            x += dx


def stone_shade(rng, ids, M, base=(0.38, 0.58), bevel=1.0, noise_amp=0.44, fine_amp=0.16, pits=14,
                cracks=3, chips=5, convex=0.07):
    h, w = ids.shape
    dt, db, dl, dr = edge_dist(M)
    nb = ids.max() + 1
    bases = rng.uniform(base[0], base[1], nb)
    V = bases[ids].copy()
    V += bevel * (0.13 * (dt == 1) + 0.06 * (dt == 2) + 0.05 * (dl == 1) + 0.02 * (dl == 2)
                  - 0.20 * (db == 1) - 0.09 * (db == 2) - 0.10 * (dr == 1) - 0.03 * (dr == 2))
    # convex face: brighter toward the upper-left of every stone
    V += convex * (np.clip((db - dt) / 4.0, -1, 1) + 0.5 * np.clip((dr - dl) / 4.0, -1, 1))
    V += noise_amp * (fbm(rng, h, w) - 0.5) + fine_amp * (rng.random((h, w)) - 0.5)
    for _ in range(pits):
        y, x = int(rng.integers(0, h)), int(rng.integers(0, w))
        if not M[y, x]:
            V[y, x] -= 0.22
            V[y, (x + 1) % w] -= 0.12
            V[(y + 1) % h, x] += 0.10
    for _ in range(cracks):
        y, x = int(rng.integers(0, h)), int(rng.integers(0, w))
        walk_crack(rng, V, M, x, y, int(rng.integers(6, 16)))
    for _ in range(chips):   # broken corners: a dark notch grows from a mortar crossing
        y, x = int(rng.integers(0, h)), int(rng.integers(0, w))
        r = int(rng.integers(2, 4))
        for yy in range(-r, r + 1):
            for xx in range(-r, r + 1):
                if abs(yy) + abs(xx) <= r:
                    V[(y + yy) % h, (x + xx) % w] -= 0.14
    V[M] = 0.03 + 0.05 * rng.random(M.sum())
    return np.clip(V, 0, 1)


def drips(rng, M, n, lmin, lmax, wmax=2):
    h, w = M.shape
    S = np.zeros((h, w))
    rows = np.nonzero(M.all(1))[0] if M.all(1).any() else np.array([0])
    for _ in range(n):
        x = int(rng.integers(0, w))
        y = int(rng.choice(rows)) + 1
        L = int(rng.integers(lmin, lmax))
        ww = int(rng.integers(1, wmax + 1))
        for k in range(L):
            a = 1.0 - k / L
            for j in range(ww):
                S[(y + k) % h, (x + j) % w] = max(S[(y + k) % h, (x + j) % w], a)
            if rng.random() < 0.12:
                x += rng.choice([-1, 1])
    return S


# ------------------------------------------------------------------------------------------------ palette
class Pal:
    def __init__(self):
        self.cols = [(0, 0, 0)]
        self.ramps = {}

    def add(self, name, cols):
        self.ramps[name] = list(range(len(self.cols), len(self.cols) + len(cols)))
        self.cols += cols

    def idx(self, name, lvl):
        r = self.ramps[name]
        return np.array(r)[np.clip(lvl, 0, len(r) - 1)]

    def flat(self):
        f = [c for col in self.cols for c in col]
        return f + [0] * (768 - len(f))


# ------------------------------------------------------------------------------------------------ door / torch
def door_leaf(rng, pal, out, hole, V_wall, wood_hi=0.62, grime=0.4):
    ys, xs = np.nonzero(hole)
    y0, y1, x0, x1 = ys.min(), ys.max(), xs.min(), xs.max()
    h, w = hole.shape
    grain = fbm(rng, h, w, ((24, 3, .5), (8, 2, .3), (4, 1, .2)))
    nplank = 5
    pw = (x1 - x0 + 1) / nplank
    V = np.zeros((h, w))
    for x in range(x0, x1 + 1):
        p = int((x - x0) / pw)
        px = (x - x0) - p * pw
        pb = 0.40 + 0.08 * ((p * 7919) % 5) / 4
        for y in range(y0, y1 + 1):
            v = pb + 0.30 * (grain[y, x] - 0.5)
            if px < 1:
                v = 0.05                     # seam
            elif px < 2:
                v += 0.10
            elif px > pw - 2:
                v -= 0.10
            v -= grime * max(0, (y - (y1 - 14)) / 14.0) * 0.5    # rot / damp at the bottom
            v -= 0.25 * max(0, 1 - (y - y0) / 6.0)             # shadow under the arch
            V[y, x] = v
    for k in range(6):   # knots / gouges
        yy, xx = int(rng.integers(y0 + 4, y1 - 4)), int(rng.integers(x0 + 2, x1 - 2))
        V[yy, xx] -= 0.2
        V[yy + 1, xx] -= 0.1
    lv = quant(np.clip(V / wood_hi, 0, 1), len(pal.ramps["wood"]), 0.7)
    out[hole] = pal.idx("wood", lv)[hole]
    # iron straps with rivets
    ni = len(pal.ramps["iron"])
    for by in (y0 + 9, y1 - 11):
        for x in range(x0, x1 + 1):
            for j, l in enumerate((3, 2, 0)):
                if hole[by + j, x]:
                    n = rng.random()
                    lvl = l if n > 0.25 else max(0, l - 1)
                    out[by + j, x] = pal.ramps["iron"][min(ni - 1, lvl)]
            if hole[by + 3, x]:
                out[by + 3, x] = pal.ramps["wood"][0]        # shadow below the strap
        for x in range(x0 + 3, x1 - 1, 7):
            out[by, x] = pal.ramps["iron"][ni - 1]
            out[by + 1, x] = pal.ramps["iron"][1]
        # rust stain below the strap
        for x in range(x0, x1 + 1):
            if rng.random() < 0.25:
                L = int(rng.integers(2, 6))
                for k in range(L):
                    if hole[by + 4 + k, x]:
                        out[by + 4 + k, x] = pal.ramps["rust"][min(len(pal.ramps["rust"]) - 1, 1 + (k < 2))]
    # ring pull
    cy, cx = (y0 + y1) // 2 + 2, x1 - 7
    for a in np.linspace(0, 2 * np.pi, 28, endpoint=False):
        yy, xx = int(round(cy + 3.2 * np.sin(a))), int(round(cx + 2.6 * np.cos(a)))
        lit = np.cos(a + 2.2)
        out[yy, xx] = pal.ramps["iron"][int(np.clip(1 + lit * 2, 0, ni - 1))]
    out[cy - 4, cx] = pal.ramps["iron"][ni - 2]
    out[cy - 4, cx + 1] = pal.ramps["iron"][0]


def torch_sprite(rng, pal):
    """32x64 indexed (palette = pal; 0 transparent): iron ring bracket on a wall plate, pitch-soaked head, flame."""
    out = np.zeros((64, 32), int)
    ir, wd, fl = pal.ramps["iron"], pal.ramps["wood"], pal.ramps["flame"]
    # wall plate
    for y in range(30, 41):
        for x in range(13, 20):
            v = 0.45 - 0.08 * (x - 13) / 6 + 0.12 * (rng.random() - 0.5)
            if y in (30,):
                v += 0.15
            if y == 40 or x == 19:
                v -= 0.2
            out[y, x] = ir[int(np.clip(v * len(ir), 0, len(ir) - 1))]
    out[32, 15] = ir[-1]
    out[38, 17] = ir[-1]
    out[33, 15] = ir[0]
    out[39, 17] = ir[0]
    # handle: slightly tilted, from (17,47) up to (15,20)
    for y in range(20, 48):
        xc = 15 + (y - 20) * 2 / 27.0
        for dx in range(-1, 2):
            x = int(round(xc + dx))
            v = (0.55, 0.40, 0.18)[dx + 1] + 0.12 * (rng.random() - 0.5)
            if (y + x) % 5 == 0:
                v -= 0.12            # grain / wrapping
            out[y, x] = wd[int(np.clip(v * len(wd), 0, len(wd) - 1))]
    # bracket arm and ring
    for y in range(33, 37):
        out[y, 16] = ir[2]
        out[y, 17] = ir[1]
    for x in range(13, 20):
        out[27, x] = ir[2] if x < 17 else ir[1]
        out[28, x] = ir[0]
    out[26, 14] = ir[3]
    for y in range(28, 34):
        out[y, 19] = ir[1]
    # pitch-soaked rag head
    for y in range(13, 21):
        for x in range(12, 19):
            r = ((x - 15.2) / 3.4) ** 2 + ((y - 17) / 4.2) ** 2
            if r <= 1:
                v = 0.08 + 0.18 * (1 - r) * (x < 15) + 0.06 * rng.random()
                if (y % 2 == 0) and x > 13:
                    v += 0.07
                out[y, x] = wd[int(np.clip(v * len(wd), 0, 2))] if v < 0.2 else ir[1]
    # flame: noisy teardrop, white core
    n = fbm(rng, 64, 32, ((6, 4, .6), (3, 2, .4)))
    for y in range(0, 17):
        for x in range(6, 25):
            t = (16 - y) / 16.0                    # 0 bottom .. 1 tip
            half = 5.8 * (1 - t) ** 0.8 * (0.7 + 0.6 * np.sin(np.pi * min(1, t * 1.6 + 0.15)))
            sway = 1.6 * t * t
            dxn = abs(x - 15.5 + sway) / max(half, 0.3)
            e = 1 - dxn - 0.45 * (n[y, x] - 0.5) * 2 * t
            if e > 0:
                heat = e * (1.15 - 0.7 * t)
                lvl = int(np.clip(heat * len(fl) * 1.1 + (BAYER[y % 4, x % 4] - .5) * .9, 0, len(fl) - 1))
                out[y, x] = fl[lvl]
    return out


# ------------------------------------------------------------------------------------------------ skeleton
def skeleton(rng, eye_rgb, pal_bone, pal_iron, pal_cloth, hunch=1.0):
    """64x64, 16 colours, index 0 transparent: capsule/sphere 'clay' model lit from the upper left + front."""
    H = W = 64
    Z = np.full((H, W), -1e9)
    N = np.zeros((H, W, 3))
    MAT = np.zeros((H, W), int)          # 0 none, 1 bone, 2 iron, 3 cloth, 4 socket
    PID = np.zeros((H, W), int)
    pid = [0]

    def put(mask, z, n, mat):
        sel = mask & (z > Z)
        Z[sel] = z[sel]
        N[sel] = n[sel]
        MAT[sel] = mat
        PID[sel] = pid[0]

    yy, xx = np.mgrid[0:H, 0:W] + 0.5

    def capsule(p0, p1, r0, r1=None, z0=0.0, mat=1, z1=None):
        pid[0] += 1
        r1 = r0 if r1 is None else r1
        z1 = z0 if z1 is None else z1
        ax, ay = p1[0] - p0[0], p1[1] - p0[1]
        L2 = ax * ax + ay * ay + 1e-9
        t = np.clip(((xx - p0[0]) * ax + (yy - p0[1]) * ay) / L2, 0, 1)
        cx, cy = p0[0] + ax * t, p0[1] + ay * t
        r = r0 + (r1 - r0) * t
        dx, dy = xx - cx, yy - cy
        d2 = dx * dx + dy * dy
        m = d2 <= r * r
        nz = np.sqrt(np.clip(1 - d2 / (r * r), 0, 1))
        n = np.stack([dx / r, dy / r, nz], -1)
        put(m, z0 + (z1 - z0) * t + nz * r, n, mat)

    def ellipse(c, rx, ry, z0=0.0, mat=1, flat=1.0):
        pid[0] += 1
        dx, dy = (xx - c[0]) / rx, (yy - c[1]) / ry
        d2 = dx * dx + dy * dy
        m = d2 <= 1
        nz = np.sqrt(np.clip(1 - d2, 0, 1))
        n = np.stack([dx * flat, dy * flat, nz], -1)
        n /= np.linalg.norm(n, axis=-1, keepdims=True) + 1e-9
        put(m, z0 + nz * min(rx, ry), n, mat)

    hx = hunch
    # legs (back to front order does not matter: z-buffer)
    capsule((27.5, 45), (25.5, 53), 2.0, 1.7, z0=0)
    capsule((36.5, 45), (38.5, 53), 2.0, 1.7, z0=0)
    ellipse((25.5, 53.5), 2.3, 2.0, z0=1)
    ellipse((38.5, 53.5), 2.3, 2.0, z0=1)
    capsule((25.5, 54), (24.5, 61.5), 1.5, 1.3, z0=0.5)
    capsule((38.5, 54), (39.5, 61.5), 1.5, 1.3, z0=0.5)
    capsule((24.5, 62), (20.5, 63), 1.3, 1.0, z0=1)
    capsule((39.5, 62), (43.5, 63), 1.3, 1.0, z0=1)
    # pelvis
    ellipse((32, 42.5), 7.2, 3.6, z0=0, flat=1.4)
    # spine and ribs
    capsule((32 + hx, 20), (32, 41), 1.6, 1.8, z0=-2)
    for i in range(6):
        y0 = 22.5 + i * 2.9
        wdt = 9.6 - i * 0.55 - (1.8 if i == 5 else 0)
        for s in (-1, 1):
            pts = []
            for k in range(9):
                t = k / 8.0
                pts.append((32 + hx * (1 - i / 6) + s * (1.5 + wdt * np.sin(t * np.pi * 0.62)),
                            y0 + 3.2 * t ** 1.6, 2.2 * np.cos(t * np.pi * 0.9) - 0.5))
            for a, b in zip(pts[:-1], pts[1:]):
                capsule(a[:2], b[:2], 1.05, z0=a[2], z1=b[2])
    capsule((32 + hx, 21.5), (32, 32), 1.25, 1.0, z0=2.2)      # sternum
    capsule((32 + hx, 21), (21.5, 22.5), 1.3, z0=1.5)          # clavicles
    capsule((32 + hx, 21), (42.5, 22.5), 1.3, z0=1.5)
    # sword arm (viewer's left): humerus, forearm, fist, rusty blade pointing down-left
    ellipse((21, 23.5), 2.7, 2.5, z0=1)
    capsule((21, 24), (17.5, 33), 1.8, 1.5, z0=1)
    ellipse((17.5, 33.5), 1.9, 1.9, z0=2)
    capsule((17.5, 34), (21.5, 41), 1.3, z0=2.5)
    capsule((18.8, 34), (22.6, 40.6), 1.0, z0=2.0)
    ellipse((22.5, 42), 2.3, 2.1, z0=4)
    capsule((24.5, 40.5), (20.0, 44.0), 1.0, z0=5, mat=2)       # crossguard
    capsule((22.0, 44.0), (11.0, 61.5), 1.7, 1.1, z0=5, mat=2)  # blade
    capsule((23.5, 41.0), (24.8, 38.8), 0.9, z0=5, mat=2)       # pommel
    # free arm (viewer's right) reaching forward, clawed hand
    ellipse((43, 23.5), 2.7, 2.5, z0=1)
    capsule((43, 24), (46.5, 32.5), 1.8, 1.5, z0=1)
    ellipse((46.5, 33), 1.9, 1.9, z0=2)
    capsule((46.5, 33.5), (44.5, 41.5), 1.3, z0=3)
    capsule((45.5, 33.5), (43.4, 41.0), 1.0, z0=2.5)
    ellipse((44.3, 42.8), 2.0, 1.7, z0=4)
    for fx, fy in ((-2.0, 4.5), (-0.5, 5.2), (1.0, 4.8), (2.2, 3.8)):
        capsule((44.3 + fx * 0.5, 43.5), (44.3 + fx, 43 + fy), 0.75, z0=4)
    # ragged loincloth
    pid[0] += 1
    cloth = np.zeros((H, W), bool)
    for x in range(25, 40):
        hang = 50 + 3 * np.sin(x * 1.7) + 2 * rng.random()
        if x in (29, 34):
            hang -= 5
        cloth[44:int(hang), x] = True
    cn = fbm(rng, H, W, ((6, 2, .6), (2, 1, .4)))
    ncl = np.stack([(cn - 0.5) * 1.5, np.zeros((H, W)) + 0.2, np.ones((H, W))], -1)
    ncl /= np.linalg.norm(ncl, axis=-1, keepdims=True)
    put(cloth, np.full((H, W), 3.5), ncl, 3)
    # skull: cranium, cheekbones, jaw
    sx = 32 + hx
    ellipse((sx, 9.5), 6.4, 6.8, z0=3)
    ellipse((sx, 16.0), 4.3, 2.6, z0=3.5, flat=1.3)
    for s in (-1, 1):
        ellipse((sx + s * 4.2, 12.6), 2.2, 1.6, z0=4.5)
    # sockets / nose cut into the skull
    sock = np.zeros((H, W), bool)
    for s in (-1, 1):
        sock |= ((xx - (sx + s * 2.6)) / 1.9) ** 2 + ((yy - 10.6) / 1.7) ** 2 <= 1
    sock |= (np.abs(xx - sx) <= 0.9) & (yy > 12.5) & (yy < 14.6)
    MAT[sock & (MAT == 1)] = 4

    # ---- lighting
    L = np.array([-0.55, -0.55, 0.63])
    L /= np.linalg.norm(L)
    diff = np.clip((N * L).sum(-1), 0, 1)
    I = (0.16 + 1.0 * diff ** 1.1) * (1.0 - 0.22 * (yy / 64.0))
    # ambient occlusion: pixels behind a nearer neighbour (other primitive) get shadowed
    occ = np.zeros((H, W))
    for dy, dx in ((-1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (-2, 0), (-1, 2), (-2, -1)):
        Zn = np.roll(np.roll(Z, -dy, 0), -dx, 1)
        Pn = np.roll(np.roll(PID, -dy, 0), -dx, 1)
        Mn = np.roll(np.roll(MAT, -dy, 0), -dx, 1)
        occ += ((Mn > 0) & (Pn != PID) & (Zn > Z + 1.0)) * (0.16 if abs(dy) + abs(dx) == 1 else 0.09)
    I = I * (1 - np.clip(occ, 0, 0.6))
    I += 0.10 * (fbm(rng, H, W, ((4, 4, .6), (2, 2, .4))) - 0.5)       # grime on the bone
    B = bayer(H, W)
    out = np.zeros((H, W), int)
    nb, ni, nc = len(pal_bone), len(pal_iron), len(pal_cloth)
    out[MAT == 1] = 1 + quant(I, nb, 0.7, B)[MAT == 1]
    out[MAT == 2] = 1 + nb + quant(I * 0.9, ni, 0.7, B)[MAT == 2]
    out[MAT == 3] = 1 + nb + ni + quant(I * 0.8, nc, 0.6, B)[MAT == 3]
    out[MAT == 4] = 1      # sockets: darkest bone
    # rust pits on the blade, teeth, glints in the sockets
    for y in range(44, 62):
        for x in range(9, 24):
            if MAT[y, x] == 2 and rng.random() < 0.18:
                out[y, x] = 1 + nb
    for x in range(int(sx) - 3, int(sx) + 3):
        out[15, x] = 1 + (nb - 2 if x % 2 else 2)
    eye = 1 + nb + ni + nc
    for s in (-1, 1):
        out[11, int(round(sx + s * 2.6 - 0.5))] = eye
    cols = [(0, 0, 0)] + list(pal_bone) + list(pal_iron) + list(pal_cloth) + [eye_rgb]
    return out, cols
