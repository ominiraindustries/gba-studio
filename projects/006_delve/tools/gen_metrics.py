#!/usr/bin/env python
"""DELVE generator checks (task G-25): fidelity of the C++ port, shape metrics, hook rules and solvability.

Compiles src/dl_gen.cpp + src/dl_gen_hooks.cpp for the PC (g++ -DDL_HOST, driver tools/host/gen_host.cpp) and:
  --fidelity S L   compares crc16(level_bytes) of the C++ v2 with dungeon_model.py (the user's v2 reference, read from
                   Desktop/SNES/tools/scripts; nothing is copied) for S seeds x L levels: must be identical.
  --seeds S        game floors (v2 + hooks, generate_floor) for S seeds x --levels L: shape metrics like the study
                   (docs/estudios/dungeon_hack_generador.md section 5), hook hard rules and an independent solver.
  --ascii K        ASCII dump of the first K seeds' floor 1.
Run:  python -I projects/006_delve/tools/gen_metrics.py --fidelity 50 3 --seeds 100 --levels 3 --ascii 3
"""
import argparse
import os
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(HERE)
BUILD = os.path.join(PROJ, "build", "host")
V2 = os.path.join(os.path.expanduser("~"), "Desktop", "SNES", "tools", "scripts", "dungeon_model.py")
W = H = 32
N = W * H
K_WALL, K_OPEN, K_DOOR, K_LOCKED, K_SECRET, K_ARCH, K_BARS, K_STAIRS = range(8)
GRAPH = [0, 1, 1, 1, 0, 1, 0, 0] + [1] * 8
FLOOD = [0, 1, 1, 0, 0, 1, 0, 0] + [1] * 8
E_MONSTER, E_ITEM, E_KEY, E_TRAP, E_FURN = 1, 2, 3, 4, 6
FURN_BLOCKS = [1, 1, 0, 1, 1, 1, 0]
DX, DY = (0, 1, 0, -1), (-1, 0, 1, 0)


def build():
    os.makedirs(BUILD, exist_ok=True)
    exe = os.path.join(BUILD, "gen_host.exe")
    src = [os.path.join(PROJ, p) for p in ("src/dl_gen.cpp", "src/dl_gen_hooks.cpp", "tools/host/gen_host.cpp")]
    cmd = ["g++", "-std=c++20", "-O2", "-DDL_HOST", "-Wall", "-Wextra", "-I" + os.path.join(PROJ, "include")] + src
    subprocess.run(cmd + ["-o", exe], check=True)
    return exe


def nb(c, d):
    x, y = c % W + DX[d], c // W + DY[d]
    return y * W + x if 0 <= x < W and 0 <= y < H else -1


def eidx(c, d):
    x, y = c % W, c // W
    return (N + c - W if y > 0 else -1, c if x < W - 1 else -1, N + c if y < H - 1 else -1, c - 1 if x > 0 else -1)[d]


def read_dump(path):
    out = []
    data = open(path, "rb").read()
    p = 0
    while p < len(data):
        head = struct.unpack_from("<12i", data, p)
        p += 48
        (n,) = struct.unpack_from("<i", data, p)
        p += 4
        lb = data[p:p + n]
        p += n
        hooks = [struct.unpack_from("<4B2h", data, p + 8 * i) for i in range(8)]
        p += 64
        rooms = [struct.unpack_from("<5B", data, p + 5 * i) for i in range(15)]
        p += 75
        (nrooms,) = struct.unpack_from("<i", data, p)
        p += 4
        ed = lb[:2 * N]
        ne = lb[4 * N]
        ents = [tuple(lb[4 * N + 1 + 6 * i:4 * N + 7 + 6 * i]) for i in range(ne)]
        out.append(dict(seed=head[0], level=head[1], used=head[2], layout=head[3], entry=head[4], stairs=head[6],
                        nhooks=head[8], removed_traps=head[9], changed_furn=head[10], c_solvable=head[11],
                        ed=ed, cl=lb[2 * N:3 * N], th=lb[3 * N:4 * N], ents=ents, hooks=hooks[:head[8]],
                        rooms=rooms[:nrooms]))
    return out


def kind(lv, c, d):
    i = eidx(c, d)
    return lv["ed"][i] & 15 if i >= 0 else K_WALL


def flood(lv, start, ok_edge, blocked=()):
    seen = {start}
    q = [start]
    for c in q:
        for d in range(4):
            i = eidx(c, d)
            if i < 0:
                continue
            n = nb(c, d)
            if n in seen or n in blocked:
                continue
            if ok_edge(i, lv["ed"][i] & 15):
                seen.add(n)
                q.append(n)
    return seen


def solve(lv):
    """Independent solver: SECRET and closed BARS are walls; levers open their bars once their cell is reached;
    keys (any kind count) open locks; blocking furniture cells are impassable."""
    blocked = {e[2] * W + e[1] for e in lv["ents"] if e[0] == E_FURN and FURN_BLOCKS[e[5] if e[5] < 7 else 0]}
    locks = [i for i in range(2 * N) if (lv["ed"][i] & 15) == K_LOCKED]
    opened, levers = set(), set()
    no_keys_ok = None
    while True:
        bars_open = {h[4] for j, h in enumerate(lv["hooks"]) if j in levers}
        seen = flood(lv, lv["entry"], lambda i, k: FLOOD[k] or (k == K_LOCKED and i in opened) or
                     (k == K_BARS and i in bars_open), blocked)
        if no_keys_ok is None and not opened:
            pass
        new = {j for j, h in enumerate(lv["hooks"]) if h[2] * W + h[1] in seen} - levers
        if new:
            levers |= new
            continue
        if no_keys_ok is None:
            no_keys_ok = lv["stairs"] in seen
        keys = sum(1 for e in lv["ents"] if e[0] == E_KEY and e[2] * W + e[1] in seen) - len(opened)
        cand = [i for i in locks if i not in opened and (edge_cells(i)[0] in seen or edge_cells(i)[1] in seen)]
        if keys > 0 and cand:
            opened.add(cand[0])
            continue
        break
    return no_keys_ok and lv["stairs"] in seen and len(opened) == len(locks) and len(levers) == len(lv["hooks"])


def edge_cells(i):
    return (i, i + 1) if i < N else (i - N, i - N + W)


def metrics(lv):
    carved = [c for c in range(N) if lv["cl"][c] & 0x80]
    deg = {c: sum(GRAPH[kind(lv, c, d)] for d in range(4)) for c in carved}
    edges = sum(deg.values()) // 2
    dead = sum(1 for c in carved if deg[c] == 1)
    junc = sum(1 for c in carved if deg[c] >= 3)
    dist = {}
    q = [lv["entry"]]
    dist[lv["entry"]] = 0
    for c in q:
        for d in range(4):
            if GRAPH[kind(lv, c, d)]:
                n = nb(c, d)
                if n not in dist:
                    dist[n] = dist[c] + 1
                    q.append(n)
    rules = []
    allc = set(carved)
    for e in lv["ents"]:
        c = e[2] * W + e[1]
        if e[0] == E_TRAP:
            bend = not ((GRAPH[kind(lv, c, 0)] and GRAPH[kind(lv, c, 2)]) or
                        (GRAPH[kind(lv, c, 1)] and GRAPH[kind(lv, c, 3)]))
            if bend:
                rules.append("trap in bend")
            if dist.get(c, 99) < 6:
                rules.append("trap near entry")
        if e[0] == E_FURN and FURN_BLOCKS[e[5] if e[5] < 7 else 0]:
            rest = flood(lv, lv["entry"], lambda i, k: GRAPH[k], {c})
            if len(rest) < len(allc) - 1:
                rules.append("blocking furniture on articulation")
    return dict(open_pct=len(carved) * 100 / N, dead=dead, junc=junc, loops=edges - (len(carved) - 1),
                rooms=len(lv["rooms"]), reach=len(dist) == len(carved), rules=rules,
                traps=sum(1 for e in lv["ents"] if e[0] == E_TRAP), p1=len(lv["hooks"]),
                furn=sum(1 for e in lv["ents"] if e[0] == E_FURN), monsters=sum(1 for e in lv["ents"] if e[0] == 1))


def ascii_map(lv):
    g = [[" "] * (2 * W + 1) for _ in range(2 * H + 1)]
    marks = {lv["entry"]: "@", lv["stairs"]: ">"}
    sym = {E_MONSTER: "M", E_ITEM: "i", E_KEY: "k", E_TRAP: "^", E_FURN: "f"}
    for e in lv["ents"]:
        c = e[2] * W + e[1]
        if e[0] in sym and c not in marks:
            marks[c] = sym[e[0]]
    for h in lv["hooks"]:
        marks[h[2] * W + h[1]] = "L"
    ech = {K_WALL: None, K_OPEN: " ", K_DOOR: "D", K_LOCKED: "X", K_SECRET: "s", K_ARCH: " ", K_BARS: "B",
           K_STAIRS: "S"}
    for c in range(N):
        x, y = c % W, c // W
        if not lv["cl"][c] & 0x80:
            continue
        g[2 * y + 1][2 * x + 1] = marks.get(c, "." if lv["th"][c] == 0 else ",")
        for d, (ox, oy) in ((0, (0, -1)), (1, (1, 0)), (2, (0, 1)), (3, (-1, 0))):
            k = kind(lv, c, d)
            ch = ech.get(k)
            yy, xx = 2 * y + 1 + oy, 2 * x + 1 + ox
            g[yy][xx] = ch if ch is not None else ("-" if oy else "|")
    for yy in range(0, 2 * H + 1, 2):
        for xx in range(0, 2 * W + 1, 2):
            if any(0 <= yy + a < 2 * H + 1 and 0 <= xx + b < 2 * W + 1 and g[yy + a][xx + b] in "-|"
                   for a, b in ((0, 1), (0, -1), (1, 0), (-1, 0))):
                g[yy][xx] = "+"
    return "\n".join("".join(r).rstrip() for r in g if "".join(r).strip())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--fidelity", nargs=2, type=int, metavar=("SEEDS", "LEVELS"))
    ap.add_argument("--seeds", type=int, default=0)
    ap.add_argument("--levels", type=int, default=3)
    ap.add_argument("--ascii", type=int, default=0)
    a = ap.parse_args()
    exe = build()
    rc = 0
    if a.fidelity:
        s, l = a.fidelity
        mine = subprocess.run([exe, "crc", str(s), str(l)], capture_output=True, text=True, check=True).stdout
        ref = subprocess.run([sys.executable, "-I", V2, "crc", "--seeds", str(s), "--levels", str(l)],
                             capture_output=True, text=True, check=True).stdout
        same = mine.split() == ref.split()
        print("fidelity %d seeds x %d levels: %s" % (s, l, "IDENTICAL" if same else "DIFFERENT"))
        rc |= 0 if same else 1
    if a.seeds or a.ascii:
        n = max(a.seeds, a.ascii)
        path = os.path.join(BUILD, "floors.bin")
        subprocess.run([exe, "dump", str(n), str(a.levels), path], check=True)
        floors = read_dump(path)
        per = {}
        unsolved = c_unsolved = 0
        rule_hits = {}
        for lv in floors:
            m = metrics(lv)
            per.setdefault(lv["level"], []).append(m)
            if not solve(lv) or not m["reach"]:
                unsolved += 1
            c_unsolved += 0 if lv["c_solvable"] else 1
            for r in m["rules"]:
                rule_hits[r] = rule_hits.get(r, 0) + 1
        print("%d floors: unsolvable (python solver) %d, (C++ solver) %d, hard-rule violations %s" % (
            len(floors), unsolved, c_unsolved, rule_hits or "none"))
        print("level  open%%  dead  junc  loops  rooms  traps  P1%%  furn  monsters")
        for l in sorted(per):
            ms = per[l]
            avg = lambda k: sum(m[k] for m in ms) / len(ms)
            print("%5d  %5.1f  %4.1f  %4.1f  %5.1f  %5.1f  %5.2f  %3.0f  %4.1f  %4.1f" % (
                l + 1, avg("open_pct"), avg("dead"), avg("junc"), avg("loops"), avg("rooms"), avg("traps"),
                100 * sum(1 for m in ms if m["p1"]) / len(ms), avg("furn"), avg("monsters")))
        rc |= 1 if (unsolved or c_unsolved or rule_hits) else 0
        for lv in floors[:a.ascii * a.levels:a.levels]:
            print("\nseed %d floor 1 (seed used %d)" % (lv["seed"], lv["used"]))
            print(ascii_map(lv))
    return rc


if __name__ == "__main__":
    sys.exit(main())
