# Block 2 (G-49) headless checks for 007_survivors with tools/scripts/gbarun.py (no window).
# Usage (repo root): python -I projects/007_survivors/tests/b2run.py CASE OUTDIR
#   stress   DBG God + Stress 56 (mixed horde) + Max power, 40 s circling; prints test_tiles_used / test_cpu_max
#   cards    stress + open a level-up (DBG Level up) for the VRAM peak with cards open
#   start    DBG Start at mm:ss (START_AT env, row value index) then profile C kiting for SECONDS
# The SELECT menu rows: 11 effect toggles, then DBG rows (see _draw_fx_menu in src/hb_game.cpp).
import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
ROM = os.path.join(ROOT, "projects", "007_survivors", "007_survivors.gba")
FX_ROWS = 11
ROW_GOD, ROW_MAX, ROW_START, ROW_STRESS, ROW_FORCE = FX_ROWS, FX_ROWS + 1, FX_ROWS + 2, FX_ROWS + 3, FX_ROWS + 4


class Inputs:
    def __init__(self, start=60):
        self.f = start
        self.parts = []

    def press(self, key, gap=6):
        self.parts.append(f"{self.f}:{key}")
        self.f += gap

    def row(self, row, key="a", right=0):
        # From the top of the open menu: down to the row, optional right presses (value), then key.
        for _ in range(row):
            self.press("down", 4)
        for _ in range(right):
            self.press("right", 4)
        self.press(key, 8)
        for _ in range(row):
            self.press("up", 4)

    def kite(self, frames, a_every=150):
        # Profile C (DISENO_FASES E6): 8 directions of 40 f in order; A every 2.5 s (cards / Nova).
        dirs = ["right", "right+down", "down", "down+left", "left", "left+up", "up", "up+right"]
        end = self.f + frames
        k = 0
        while self.f < end:
            d = dirs[k % 8]
            self.parts.append(f"{self.f}-{self.f + 39}:{d}")
            k += 1
            self.f += 40
        t = end - frames
        while t < end:
            self.parts.append(f"{t + 20}:a")
            t += a_every

    def __str__(self):
        return ";".join(self.parts)


def menu(inp, rows):
    inp.press("start", 30)   # title -> run
    inp.press("select", 10)
    for r in rows:
        inp.row(*r)
    inp.press("select", 10)


def main():
    case, out = sys.argv[1], sys.argv[2]
    seconds = int(os.environ.get("SECONDS_RUN", "40"))
    inp = Inputs()
    checks = []
    if case in ("stress", "cards"):
        menu(inp, [(ROW_GOD,), (ROW_MAX,), (ROW_STRESS,)])
        inp.kite(seconds * 60)
        checks = ["test_tiles_used.32<=900"]
    elif case == "start":
        idx = int(os.environ.get("START_AT", "0"))
        rows = [(ROW_GOD,)] if os.environ.get("GOD", "1") == "1" else []
        rows.append((ROW_START, "a", idx))
        force = os.environ.get("FORCE")
        if force is not None:
            rows.append((ROW_FORCE, "a", int(force)))
        menu(inp, rows)
        inp.kite(seconds * 60)
    frames = inp.f + 30
    shots = os.environ.get("SHOTS") or ",".join(str(x) for x in range(inp.f - seconds * 60 + 120, frames, 600))
    cmd = [sys.executable, "-I", os.path.join(ROOT, "tools", "scripts", "gbarun.py"), ROM, "--frames", str(frames),
           "--shot", shots, "--input", str(inp), "--out", out, "--timeout", "600", "--nonblank"]
    syms = []
    for sym in ["test_tiles_used", "test_cpu_max", "test_cpu_max_plain", "test_cpu_max_enemies", "test_time_s",
                "test_level", "test_hp", "test_kills", "test_boss_hp", "test_mode", "test_enemies", "test_gold",
                "test_dead_at", "test_boss_phase", "test_wolf_crouches", "test_wolf_dashes", "test_wolf_dash_hits", "test_events", "test_prof_ember", "test_prof_chain"]:
        syms.append(f"{sym}.32>=-99999999")
    if os.environ.get("PROF"):
        syms += [f"test_prof_worst+{4 * k}.32>=-99999999" for k in range(14)]
    cmd += ["--check", ",".join(checks + syms)]
    rc = subprocess.call(cmd, stdout=subprocess.DEVNULL)
    import json
    res = json.load(open(os.path.join(out, "result.json")))
    print(" ".join(f"{c['check'].split('>=')[0].replace('.32', '')[5:]}={c['value']}" for c in res["checks"]), "ok" if res["ok"] else "FAIL")
    sys.exit(rc)


if __name__ == "__main__":
    main()
