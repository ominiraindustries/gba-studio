"""POKER DOJO offline solver: equities -> push/fold Nash for every stack -> C++ headers.

Usage (from the repo root, Git Bash or PowerShell):
    python -I projects/003_poket_gto/tools/solver/solve.py            # uses cache/equity169.npz if present
    python -I projects/003_poket_gto/tools/solver/solve.py --recompute-equity
    python -I projects/003_poket_gto/tools/solver/solve.py --no-export
"""

import os
import sys
import time

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)  # python -I does not add the script dir

import equity  # noqa: E402
import export_headers  # noqa: E402
import pushfold  # noqa: E402
from hands import CLASS_NAMES, pair_weights  # noqa: E402

STACKS = np.arange(2, 51) / 2.0  # 1.0 .. 25.0 bb, step 0.5
VARIANTS = [("no_ante", 0.0), ("ante", 0.1)]
RESULTS_FILE = os.path.join(equity.CACHE_DIR, "pushfold.npz")
TARGET_EXPL = 1e-6  # bb per hand (NashConv); the task asks for < 0.01


def solve_all(eq, pw, progress=True):
    nv, ns = len(VARIANTS), len(STACKS)
    out = {k: np.zeros((nv, ns, 169)) for k in ("push", "call", "ev_push", "ev_call")}
    out["expl"] = np.zeros((nv, ns))
    out["iters"] = np.zeros((nv, ns), dtype=np.int64)
    out["value"] = np.zeros((nv, ns))
    for v, (_, ante) in enumerate(VARIANTS):
        for s, stack in enumerate(STACKS):
            game = pushfold.Game(eq, pw, stack, ante)
            p, q, expl, it = pushfold.solve(game, target=TARGET_EXPL)
            evp, evc = game.ev_tables(p, q)
            out["push"][v, s], out["call"][v, s] = p, q
            out["ev_push"][v, s], out["ev_call"][v, s] = evp, evc
            out["expl"][v, s], out["iters"][v, s], out["value"][v, s] = expl, it, game.value(p, q)
        if progress:
            print(f"  {VARIANTS[v][0]}: max exploitability {out['expl'][v].max():.2e} bb, "
                  f"max iters {out['iters'][v].max()}", flush=True)
    out["stacks"] = STACKS
    return out


def combo_pct(freq, pw):
    """Share of the 1326 combos played (freq weighted by class combos)."""
    n = pw.sum(axis=1) / 1225.0
    return float((freq * n).sum() / 1326.0 * 100.0)


def main():
    t0 = time.time()
    eqd = equity.load(recompute="--recompute-equity" in sys.argv)
    eq = eqd["equity"]
    pw = pair_weights()
    print(f"equity ready ({time.time() - t0:.1f} s; AA vs KK = {eq[0, 14]:.4f})", flush=True)
    t1 = time.time()
    res = solve_all(eq, pw)
    print(f"push/fold solved in {time.time() - t1:.1f} s", flush=True)
    np.savez_compressed(RESULTS_FILE, **res)

    for v, (name, _) in enumerate(VARIANTS):
        print(f"[{name}]  stack  push%  call%")
        for stack in (25, 20, 15, 10, 8, 6, 4, 2, 1):
            s = int(np.where(STACKS == stack)[0][0])
            print(f"   {stack:5.1f}  {combo_pct(res['push'][v, s], pw):5.1f}  {combo_pct(res['call'][v, s], pw):5.1f}")
    s10 = int(np.where(STACKS == 10)[0][0])
    mixed = [CLASS_NAMES[i] for i in range(169) if 0.02 < res["push"][0, s10, i] < 0.98]
    print("10 bb no ante, mixed push classes:", mixed)

    if "--no-export" not in sys.argv:
        paths = export_headers.write_all(eq, res, STACKS, VARIANTS)
        for p in paths:
            print(f"wrote {p} ({os.path.getsize(p) / 1024:.0f} KB)")
    print(f"total {time.time() - t0:.1f} s")


if __name__ == "__main__":
    main()
