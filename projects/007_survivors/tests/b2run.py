# Block 2-3 (G-49, G-52) headless checks for 007_survivors with tools/scripts/gbarun.py (no window).
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
    inp.press("start", 30)   # title -> main menu
    inp.press("a", 20)       # PLAY -> stage select
    inp.press("a", 30)       # Gloam Meadow -> run
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
        if os.environ.get("MAX") == "1":
            rows.append((ROW_MAX,))
        if os.environ.get("META") == "1":
            rows.append((ROW_FORCE + 1,))       # DBG Meta preset (+ Moon Lance Nv5)
        force = os.environ.get("FORCE")
        if force is not None:
            rows.append((ROW_FORCE, "a", int(force)))
        menu(inp, rows)
        inp.kite(seconds * 60)
    elif case == "times":
        # Tiempos (DISENO_RUTA block 2): profile C, no God, from 0:00, SEED 1-5 = frames waited on the title.
        inp = Inputs(start=60 + 13 * int(os.environ.get("SEED", "1")))
        menu(inp, [])
        inp.kite(seconds * 60)
    elif case == "meta":
        # Block 4: META1 = fresh save, Meta preset, quit, shop (Might x5, refund, Might x1, Cinda), records,
        # stage select with Cinda. META2 (KEEP_SAVE=1) = boot again and open the shop: purchases kept.
        marks = []
        inp.press("start", 30)
        marks.append(inp.f)                      # main menu
        if os.environ.get("PHASE", "1") == "1":
            inp.press("a", 20)
            inp.press("a", 60)                   # run
            inp.press("select", 10)
            inp.row(ROW_FORCE + 1)               # DBG Meta preset
            inp.press("select", 20)
            inp.press("start", 20)
            inp.press("down", 8)
            inp.press("a", 40)                   # quit -> menu
            marks.append(inp.f)
            inp.press("down", 10)
            inp.press("a", 30)                   # SHOP
            for _ in range(5):
                inp.press("a", 12)
            marks.append(inp.f)                  # Might 5/5
            inp.press("select", 12)
            marks.append(inp.f)                  # refund prompt
            inp.press("a", 20)
            inp.press("a", 12)                   # Might 1/5
            for _ in range(11):
                inp.press("down", 6)
            inp.press("a", 20)                   # Cinda
            marks.append(inp.f)
            inp.press("b", 30)
            inp.press("down", 10)
            inp.press("a", 30)                   # RECORDS
            marks.append(inp.f)
            inp.press("b", 30)
            inp.press("up", 8)
            inp.press("up", 8)
            inp.press("a", 30)                   # stage select
            inp.press("r", 20)
            marks.append(inp.f)
            inp.press("right", 20)
            marks.append(inp.f)
        elif os.environ.get("PHASE") == "3":
            # Reroll: Meta preset, quit, buy Reroll 1, play, wait for a level-up, R.
            inp.press("a", 20)
            inp.press("a", 60)
            inp.press("select", 10)
            inp.row(ROW_FORCE + 1)
            inp.press("select", 20)
            inp.press("start", 20)
            inp.press("down", 8)
            inp.press("a", 40)
            inp.press("down", 10)
            inp.press("a", 30)
            for _ in range(8):
                inp.press("down", 6)
            inp.press("a", 20)
            inp.press("b", 30)
            inp.press("up", 10)
            inp.press("a", 20)
            inp.press("a", 60)
            # kite without A until the first level-up is waiting (cards stay open: nobody presses A)
            dirs = ["right", "down", "left", "up"]
            for k in range(40):
                inp.parts.append(f"{inp.f}-{inp.f + 39}:{dirs[k % 4]}")
                inp.f += 40
            marks.append(inp.f)
            inp.press("r", 20)
            marks.append(inp.f)
        else:
            inp.press("down", 10)
            inp.press("a", 30)
            marks.append(inp.f)                  # shop after reboot
            for _ in range(11):
                inp.press("down", 6)
            marks.append(inp.f)
        os.environ.setdefault("SHOTS", ",".join(str(m) for m in marks))
        seconds = 0
    print("kite_start", inp.f - seconds * 60)
    frames = inp.f + 30
    shots = os.environ.get("SHOTS") or ",".join(str(x) for x in range(inp.f - seconds * 60 + 120, frames, 600))
    cmd = [sys.executable, "-I", os.path.join(ROOT, "tools", "scripts", "gbarun.py"), ROM, "--frames", str(frames),
           "--shot", shots, "--input", str(inp), "--out", out, "--timeout", "600", "--nonblank"]
    syms = []
    for sym in ["test_tiles_used", "test_tiles_menu", "test_tiles_menu_mode", "test_cpu_max", "test_cpu_max_plain", "test_cpu_max_enemies", "test_time_s",
                "test_level", "test_hp", "test_kills", "test_boss_hp", "test_mode", "test_enemies", "test_gold",
                "test_dead_at", "test_boss_phase", "test_wolf_crouches", "test_wolf_dashes", "test_wolf_dash_hits", "test_events", "test_prof_ember", "test_prof_chain", "test_boss_ttk", "test_first_crouch_frame", "test_frames", "test_prof_worst_frame", "test_thorn_radius", "test_wall_onscreen", "test_thorn_first_hurt", "test_thorn_hits_max", "test_curse_elites", "test_gold_drops", "test_gold_bosses", "test_nova_pulled", "test_novas_cast", "test_charge_offscreen", "test_evolved", "test_meta_gold", "test_new_unlocks", "test_lance_hits", "test_screen", "test_level_at_600"]:
        syms.append(f"{sym}.32>=-99999999")
    if os.environ.get("PROF"):
        syms += [f"test_prof_worst+{4 * k}.32>=-99999999" for k in range(16)]
    cmd += ["--check", ",".join(checks + syms)]
    # Block 4: the run reads the meta save (Mesen keeps it in Documents/Mesen2/Saves). A fresh save by default so
    # the card pool and shop ranks are those of a first run; KEEP_SAVE=1 keeps it.
    sav = os.path.join(os.path.expanduser("~"), "Documents", "Mesen2", "Saves", "007_survivors.sav")
    if os.environ.get("KEEP_SAVE") != "1" and os.path.exists(sav):
        os.remove(sav)
    rc = subprocess.call(cmd, stdout=subprocess.DEVNULL)
    import json
    res = json.load(open(os.path.join(out, "result.json")))
    print(" ".join(f"{c['check'].split('>=')[0].replace('.32', '')[5:]}={c['value']}" for c in res["checks"]), "ok" if res["ok"] else "FAIL")
    sys.exit(rc)


if __name__ == "__main__":
    main()
