"""DELVE block 1 recorded-input route (G-35), planned online from emulator memory.

  python -I projects/006_delve/tests/route.py <rom built with -DDL_DEBUG_SEED> <out dir> [--death]

Replays the inputs found so far in gbarun.py (Mesen 2 headless), dumps EWRAM/IWRAM at the decision frame, reads the
level (gs), the combat data (dl_combat_data) and the test_* mirrors, and appends the next input. Route: start, walk to
the first rat, let it telegraph and step away (MISS), kill it, pick up a potion, take the stairs; with --death it then
lets a monster on floor 2 kill the player and restarts from the death screen. Writes route.txt (inputs, shot frames,
labels) and runs the whole route once more with screenshots and a contact sheet.
"""
import json
import os
import struct
import subprocess
import sys
from collections import deque

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
GBARUN = os.path.join(ROOT, "tools", "scripts", "gbarun.py")
NM = os.path.join(ROOT, "toolchain", "devkitPro", "devkitARM", "bin", "arm-none-eabi-nm.exe")
W = N = None
W = 32
N = W * W
DX = ((0, -1), (1, 0), (0, 1), (-1, 0))
GAP = 14                      # frames between movement presses (smooth move: 11 f cooldown)
K_OPEN, K_DOOR, K_ARCH, K_STAIRS = 1, 2, 5, 7
E_ITEM, E_KEY, E_TRAP, E_FURN, E_MONSTER = 2, 3, 4, 6, 1
FURN_BLOCKS = (True, True, False, True, True, True, False)
MS_IDLE, MS_CHASE, MS_WARN, MS_DYING = range(4)


def symbols(elf):
    out = subprocess.run([NM, "-C", elf], capture_output=True, text=True, check=True).stdout
    syms = {}
    for line in out.splitlines():
        p = line.split(" ", 2)
        if len(p) == 3:
            syms[p[2].strip()] = int(p[0], 16)
    return syms


class Game:
    def __init__(self, rom, out):
        self.rom, self.out = rom, out
        self.syms = symbols(os.path.splitext(rom)[0] + ".elf")
        self.inputs = []
        self.shots = []
        self.labels = []
        self.runs = 0

    def run(self, frames, shots=None, sheet=None):
        spec = ";".join("%d:%s" % fk for fk in self.inputs if fk[0] <= frames)
        args = [sys.executable, "-I", GBARUN, self.rom, "--frames", str(frames), "--out", self.out,
                "--dump", "ewram,iwram", "--timeout", "180"]
        if spec:
            args += ["--input", spec]
        if shots:
            args += ["--shot", ",".join(map(str, shots))]
        if sheet:
            args += ["--sheet", sheet]
        r = subprocess.run(args, capture_output=True, text=True)
        self.runs += 1
        if r.returncode not in (0, 1):
            raise SystemExit("gbarun failed:\n" + r.stdout[-2000:] + r.stderr[-2000:])
        self.ew = open(os.path.join(self.out, "ewram.bin"), "rb").read()
        self.iw = open(os.path.join(self.out, "iwram.bin"), "rb").read()
        self.frame = frames
        self.parse()

    def i32(self, name):
        a = self.syms[name]
        mem, base = (self.iw, 0x03000000) if a >= 0x03000000 else (self.ew, 0x02000000)
        return struct.unpack_from("<i", mem, a - base)[0]

    def parse(self):
        g = self.syms["(anonymous namespace)::gs"] - 0x02000000
        e = self.ew
        self.ed = e[g:g + 2 * N]
        nents = struct.unpack_from("<i", e, g + 5248)[0]
        self.ents = [tuple(e[g + 4096 + 6 * i:g + 4102 + 6 * i]) for i in range(nents)]
        self.entry, self.stairs = struct.unpack_from("<i", e, g + 5372)[0], struct.unpack_from("<i", e, g + 5380)[0]
        c = self.syms["dl_combat_data"] - 0x02000000
        p = struct.unpack_from("<19i", e, c)
        keys = "hp hp_max str def level xp potions keys weapon reload invuln counter busy buffered kills dodges " \
               "crits hits_taken attacks"
        self.p = dict(zip(keys.split(), p))
        nm = struct.unpack_from("<i", e, c + 716)[0]
        self.mons = []
        for i in range(nm):
            f = struct.unpack_from("<hhBBhhhhhBBBb", e, c + 76 + 20 * i)
            self.mons.append(dict(zip("ent cell kind state hp timer move cd target hit strike warn side".split(), f)))
        self.cell, self.dir = self.i32("test_cell"), self.i32("test_dir")
        self.mode, self.floor = self.i32("test_mode"), self.i32("test_floor")

    # ---- grid
    def kind(self, c, d):
        x, y = c % W, c // W
        if d == 0:
            i = N + c - W if y > 0 else -1
        elif d == 1:
            i = c if x < W - 1 else -1
        elif d == 2:
            i = N + c if y < W - 1 else -1
        else:
            i = c - 1 if x > 0 else -1
        return self.ed[i] & 15 if i >= 0 else 0

    @staticmethod
    def nb(c, d):
        return c + DX[d][0] + DX[d][1] * W

    def blocked(self, avoid_traps, monsters=True):
        b = set()
        for t, x, y, a, bb, cc in self.ents:
            if t == E_FURN and FURN_BLOCKS[cc if cc < 7 else 0]:
                b.add(y * W + x)
            if avoid_traps and t == E_TRAP:
                b.add(y * W + x)
        if monsters:
            for m in self.mons:
                b.add(m["cell"])
        return b

    def path(self, goal_cells, avoid_traps=True, monsters=True):
        """BFS over cells from the player to any goal cell; returns the cell list (without the start). Falls back
        to walking over traps, then through monsters (the fight starts when one is adjacent)."""
        block = self.blocked(avoid_traps, monsters) - set(goal_cells)
        prev = {self.cell: None}
        q = deque([self.cell])
        while q:
            c = q.popleft()
            if c in goal_cells:
                out = []
                while prev[c] is not None:
                    out.append(c)
                    c = prev[c]
                return out[::-1]
            for d in range(4):
                if self.kind(c, d) in (K_OPEN, K_DOOR, K_ARCH):
                    n = self.nb(c, d)
                    if n not in prev and n not in block:
                        prev[n] = c
                        q.append(n)
        if avoid_traps:
            return self.path(goal_cells, False, monsters)
        if monsters:
            return self.path(goal_cells, False, False)
        return None

    def face(self, d):
        """key to turn toward absolute dir d (None if facing it)."""
        r = (d - self.dir) & 3
        return None if r == 0 else ("right" if r in (1, 2) else "left")

    def press(self, key, at=None, label=None, shot_after=None):
        f = self.frame + 1 if at is None else at
        self.inputs.append((f, key))
        if label:
            self.shots.append(f + (shot_after or 4))
            self.labels.append(label)
        return f

    def step_to(self, n):
        d = next(d for d in range(4) if self.nb(self.cell, d) == n)
        k = self.face(d)
        return self.press(k or "up")


def adjacent(g, m):
    for d in range(4):
        if g.nb(g.cell, d) == m["cell"] and g.kind(g.cell, d) in (K_OPEN, K_DOOR, K_ARCH):
            return d
    return None


def main():
    rom, out = sys.argv[1], sys.argv[2]
    death = "--death" in sys.argv
    os.makedirs(out, exist_ok=True)
    g = Game(rom, os.path.join(out, "probe"))
    g.inputs.append((60, "a"))
    g.shots += [40]
    g.labels += ["title"]
    g.run(100)
    assert g.mode == 1 and g.cell == g.entry, (g.mode, g.cell, g.entry)
    g.shots.append(101)
    g.labels.append("start")
    goal = "rat"
    first_rat = None
    seen_warn = seen_miss = seen_hit = seen_potion = seen_kill = seen_dmg = False
    hp_last = g.p["hp"]
    next_frame = 101
    for it in range(400):
        g.run(next_frame)
        if g.mode == 2:
            g.shots.append(g.frame + 2)
            g.labels.append("dead")
            g.press("a", at=g.frame + 70)    # G-42: the death screen ignores A for 60 f; A = RETRY FLOOR
            g.shots.append(g.frame + 100)
            g.labels.append("restart")
            next_frame = g.frame + 110
            g.run(next_frame)
            print("restart mode", g.mode, "floor", g.floor)
            break
        if g.p["hp"] < hp_last and not seen_dmg:
            seen_dmg = True
            g.shots.append(g.frame)   # within the 6 f red edge? close enough: shot now
            g.labels.append("hit_taken")
        hp_last = g.p["hp"]
        print(g.frame, goal, "cell", g.cell, "dir", g.dir, "hp", g.p["hp"], "kills", g.p["kills"], "dodges",
              g.p["dodges"], "pot", g.p["potions"], "mons", [(m["cell"], m["state"]) for m in g.mons][:6], flush=True)
        alive = [m for m in g.mons if m["state"] != MS_DYING]
        adj = [(m, adjacent(g, m)) for m in alive if adjacent(g, m) is not None]
        warn = [(m, d) for m, d in adj if m["state"] == MS_WARN and m["target"] == g.cell]
        if g.p["dodges"] and not seen_miss:
            seen_miss = True
        if goal != "die" and warn:
            m, d = warn[0]
            if not seen_warn:
                seen_warn = True
                g.shots.append(g.frame)
                g.labels.append("telegraph")
            # step away: back, then the sides, then forward (any cell that is not the target)
            for key, rel in (("down", 2), ("l", 3), ("r", 1), ("up", 0)):
                dd = (g.dir + rel) & 3
                n = g.nb(g.cell, dd)
                if dd != d and g.kind(g.cell, dd) in (K_OPEN, K_ARCH) and n not in g.blocked(False):
                    f = g.press(key)
                    if g.p["dodges"] == 0:
                        g.shots.append(f + m["timer"] + 2)
                        g.labels.append("dodge_miss")
                    next_frame = f + GAP
                    break
            else:
                next_frame = g.frame + 4
            continue
        if adj and goal != "die":
            front = [m for m, d in adj if d == g.dir]
            if not front:
                k = g.face(adj[0][1])
                next_frame = g.press(k) + GAP
                continue
            m = front[0]
            if first_rat is None and m["kind"] == 0:
                first_rat = m["ent"]
            if m["ent"] == first_rat and g.p["dodges"] == 0:
                next_frame = g.frame + 3      # wait for its telegraph: the dodge comes first
                continue
            need = max(0, 30 + 10 * g.p["weapon"] - g.p["reload"])
            f = g.press("a", at=g.frame + 1 + need)
            g.shots.append(f + 3)
            g.labels.append("attack_crit" if g.p["counter"] > need else "attack")
            if m["hp"] <= 5 + 2 * g.p["weapon"] and not seen_kill:
                seen_kill = True
                g.shots.append(f + 9)
                g.labels.append("kill_shrink")
            next_frame = f + 6
            continue
        if goal == "rat":
            if g.p["kills"] >= 1:
                goal = "potion"
                continue
            rats = [m for m in alive if m["kind"] == 0] or alive
            ring = {g.nb(m["cell"], d) for m in rats for d in range(4)}
            if g.cell in ring or any(m["state"] == MS_CHASE for m in rats):
                next_frame = g.frame + 6      # it comes to us
                continue
            p = g.path(ring)
        elif goal == "potion":
            pots = {y * W + x for t, x, y, a, b, c in g.ents if t == E_ITEM and b == 1}
            if g.p["potions"] >= 2 or not pots:
                if not seen_potion and g.p["potions"] >= 2:
                    seen_potion = True
                goal = "stairs"
                continue
            p = g.path(pots)
            if p and len(p) == 1:
                f = g.step_to(p[0])
                if g.inputs[-1][1] == "up":
                    g.shots.append(f + 20)
                    g.labels.append("potion_pickup")
                next_frame = f + GAP
                continue
        elif goal == "stairs":
            if g.floor >= 1:
                g.shots.append(g.frame + 2)
                g.labels.append("floor2")
                if not death:
                    break
                goal = "die"
                continue
            sc = g.stairs
            sd = next((d for d in range(4) if g.kind(sc, d) == K_STAIRS), None)
            if g.cell == sc:
                k = g.face(sd)
                if k:
                    next_frame = g.press(k) + GAP
                else:
                    f = g.press("up", label="stairs_ahead", shot_after=-2)
                    next_frame = f + 90
                continue
            p = g.path({sc})
        else:   # die: walk next to the nearest monster, face it and stand still
            if adj:
                k = g.face(adj[0][1])
                next_frame = (g.press(k) + GAP) if k else g.frame + 60
                continue
            ring = {g.nb(m["cell"], d) for m in alive for d in range(4)}
            p = g.path(ring)
        if not p and goal == "potion":
            print("no path for potion: skipped", file=sys.stderr)
            goal = "stairs"
            continue
        if not p:
            print("no path for", goal, "at", g.cell, file=sys.stderr)
            next_frame = g.frame + 30
            continue
        # nothing within 5 cells: several steps per replay (stops before doors, which may need a bump to open)
        near = any(abs(m["cell"] % W - g.cell % W) + abs(m["cell"] // W - g.cell // W) <= 5 for m in alive)
        cell, dr, f = g.cell, g.dir, g.frame + 1
        for n in p[:1 if near else 6]:
            d = next(d for d in range(4) if g.nb(cell, d) == n)
            if g.kind(cell, d) == K_DOOR and n != p[0]:
                break
            r = (d - dr) & 3
            for key in ([] if r == 0 else ["right"] if r == 1 else ["left"] if r == 3 else ["right", "right"]):
                g.inputs.append((f, key))
                f += GAP
            g.inputs.append((f, "up"))
            f += GAP
            cell, dr = n, d
            if g.kind(g.nb(cell, 0) if False else cell, d) == K_STAIRS:
                break
        next_frame = f
    print("frames", g.frame, "runs", g.runs, "player", g.p)
    total = max(g.frame, max(g.shots)) + 4
    shots = sorted(set(s for s in g.shots if 1 <= s <= total))
    labels = {s: l for s, l in zip(g.shots, g.labels)}
    with open(os.path.join(out, "route.txt"), "w") as f:
        f.write(";".join("%d:%s" % fk for fk in g.inputs) + "\n")
        f.write(",".join(map(str, shots)) + "\n")
        f.write(" ".join("%d=%s" % (s, labels.get(s, "")) for s in shots) + "\n")
    final = Game(rom, os.path.join(out, "final"))
    final.inputs = g.inputs
    final.run(total, shots, os.path.join(out, "route_sheet.png"))
    perf = {k: final.i32(k) for k in final.syms if k.startswith("test_") and k.endswith(("max_ticks", "_ticks_per_frame",
            "max_x1000")) or k in ("test_deaths", "test_mode", "test_floor", "test_spike_hits", "test_palette_skips", "test_sprite_palettes_max")}
    print(json.dumps({"player": final.p, "shots": {s: labels.get(s) for s in shots}, "perf": perf}, indent=1))


if __name__ == "__main__":
    main()
