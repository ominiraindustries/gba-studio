"""Validation of the POKER DOJO solver against known properties.

    python -I projects/003_poker_dojo/tools/solver/test_solver.py

Needs cache/equity169.npz and cache/pushfold.npz (run solve.py first). Exits 1 on any failure.
"""

import os
import re
import shutil
import subprocess
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import equity  # noqa: E402
import evaluator as ev  # noqa: E402
import pushfold  # noqa: E402
import solve  # noqa: E402
from hands import CLASS_COMBOS, CLASS_NAMES, NUM_CLASSES, class_index, pair_weights  # noqa: E402

FAILS = []


def check(name, ok, detail=""):
    print(f"[{'OK' if ok else 'FAIL'}] {name}" + (f"  ({detail})" if detail else ""))
    if not ok:
        FAILS.append(name)


def idx(name):
    return CLASS_NAMES.index(name)


def card(s):
    return "23456789TJQKA".index(s[0]) * 4 + "shdc".index(s[1])


def hand(*cs):
    return [card(c) for c in cs]


def test_combos(pw):
    check("1326 combos (6 / 4 / 12 per class)", CLASS_COMBOS.sum() == 1326 and CLASS_COMBOS[0] == 6
          and CLASS_COMBOS[1] == 4 and CLASS_COMBOS[13] == 12)
    check("combo pairs with card removal sum to 1326 * 1225", pw.sum() == 1326 * 1225, str(pw.sum()))
    check("pair weights symmetric", np.array_equal(pw, pw.T))
    check("AA vs AA = 6 disjoint pairs, AA vs KK = 36", pw[0, 0] == 6 and pw[0, 14] == 36, f"{pw[0, 0]}, {pw[0, 14]}")
    check("class order: AA=0 AKs=1 AKo=13 KK=14 22=168", [CLASS_NAMES[i] for i in (0, 1, 13, 14, 168)]
          == ["AA", "AKs", "AKo", "KK", "22"] and class_index(12, 11, True) == 1 and class_index(0, 1, False) == 167)


def test_evaluator(eqd):
    order = [
        hand("2s", "3h", "4d", "5c", "7s", "9h", "Jd"),   # high card
        hand("2s", "2h", "4d", "5c", "7s", "9h", "Jd"),   # pair
        hand("2s", "2h", "4d", "4c", "7s", "9h", "Jd"),   # two pair
        hand("2s", "2h", "2d", "5c", "7s", "9h", "Jd"),   # trips
        hand("As", "2h", "3d", "4c", "5s", "9h", "Jd"),   # wheel
        hand("6s", "2h", "3d", "4c", "5s", "9h", "Jd"),   # six-high straight
        hand("2s", "4s", "6s", "8s", "Ts", "9h", "Jd"),   # flush
        hand("2s", "2h", "2d", "5c", "5s", "9h", "Jd"),   # full house
        hand("2s", "2h", "2d", "2c", "5s", "9h", "Jd"),   # quads
        hand("As", "2s", "3s", "4s", "5s", "9h", "Jd"),   # steel wheel
        hand("Ts", "Js", "Qs", "Ks", "As", "9h", "Jd"),   # royal
    ]
    s = ev.eval_cards(np.array(order))
    check("evaluator category order", list(s >> 20) == [0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8] and np.all(np.diff(s) > 0))
    tie = ev.eval_cards(np.array([hand("As", "Kh", "Qd", "Jc", "9s", "3h", "2d"),
                                  hand("Ah", "Kd", "Qc", "Js", "9h", "4h", "2c")]))
    check("kickers beyond the best five do not count", tie[0] == tie[1])
    # Every 7-card hand is counted 21 times (5 board + 2 hole splits). Known 7-card category counts.
    known = np.array([23294460, 58627800, 31433400, 6461620, 6180020, 4047644, 3473184, 224848, 41584]) * 21
    cats = eqd["categories"]
    check("7-card category frequencies over all 133,784,560 hands", np.array_equal(cats, known),
          f"{(cats // 21).tolist()}")


def test_equity(eqd, pw):
    e, W, T, N = eqd["equity"], eqd["wins"], eqd["ties"], eqd["totals"]
    check("wins + losses + ties = all disjoint deals", np.array_equal(W + W.T + T, N))
    check("E[a,b] + E[b,a] = 1", np.abs(e + e.T - 1).max() < 1e-12)
    for a, b, ref in (("AA", "KK", 0.8195), ("AKo", "QQ", 0.4303), ("72o", "AA", 0.1166), ("AA", "72o", 0.8834), ("KK", "AKs", 0.6602)):
        v = e[idx(a), idx(b)]
        check(f"equity {a} vs {b} ~ {ref:.2%}", abs(v - ref) < 0.003, f"{v:.4%}")
    # Independent brute force: AKs vs 22 = mean of AsKs vs 2h2d and AsKs vs 2s2h (3 + 3 of the 6 pair combos).
    import itertools
    def exact(h1, h2):
        rest = [c for c in range(52) if c not in h1 + h2]
        boards = np.array(list(itertools.combinations(rest, 5)), dtype=np.int64)
        a = ev.eval_cards(np.hstack([np.tile(h1, (len(boards), 1)), boards]))
        b = ev.eval_cards(np.hstack([np.tile(h2, (len(boards), 1)), boards]))
        return ((a > b).sum() + 0.5 * (a == b).sum()) / len(boards)
    bf = 0.5 * (exact(hand("As", "Ks"), hand("2h", "2d")) + exact(hand("As", "Ks"), hand("2s", "2h")))
    v = e[idx("AKs"), idx("22")]
    check("AKs vs 22 equals an independent brute-force enumeration", abs(v - bf) < 1e-12, f"{v:.6%} vs {bf:.6%}")
    # Equity of AA against a random hand: ~85.2 %.
    v = (e[0] * pw[0]).sum() / pw[0].sum()
    check("AA vs random ~ 85.2 %", abs(v - 0.852) < 0.002, f"{v:.3%}")


def test_pushfold(res, pw, eq):
    stacks = res["stacks"]
    check("explotability < 0.01 bb everywhere", res["expl"].max() < 0.01, f"max {res['expl'].max():.2e} bb")
    check("AA always pushes and calls", res["push"][:, :, 0].min() > 0.999 and res["call"][:, :, 0].min() > 0.999)
    s10 = int(np.where(stacks == 10)[0][0])
    pct = solve.combo_pct(res["push"][0, s10], pw)
    check("10 bb (no ante): SB pushes ~55-60 % of hands", 50 <= pct <= 65, f"{pct:.1f} %")
    for v in range(res["push"].shape[0]):
        pp = [solve.combo_pct(res["push"][v, s], pw) for s in range(len(stacks))]
        cc = [solve.combo_pct(res["call"][v, s], pw) for s in range(len(stacks))]
        check(f"variant {v}: ranges widen as the stack shrinks", np.all(np.diff(pp) <= 0.5) and np.all(np.diff(cc) <= 0.5),
              f"push {pp[-1]:.1f}..{pp[0]:.1f} %, call {cc[-1]:.1f}..{cc[0]:.1f} %")
    check("ante widens the 10 bb push range", solve.combo_pct(res["push"][1, s10], pw) >= pct)
    # Sign of the stored EV matches the action (where the action is not mixed).
    p, ep = res["push"], res["ev_push"]
    check("push where EV(push) > 0, fold where < 0", np.all((ep[p > 0.99] > -0.01)) and np.all(ep[p < 0.01] < 0.01))
    # Convergence: exploitability decreases with more iterations.
    game = pushfold.Game(eq, pw, 10.0)
    e1 = pushfold.solve(game, target=0, max_iters=100)[2]
    e2 = pushfold.solve(game, target=0, max_iters=2000)[2]
    check("exploitability converges (100 -> 2000 iterations)", e2 < e1 / 5, f"{e1:.2e} -> {e2:.2e} bb")
    # Independent cross-check of the game value with a linear program.
    try:
        from scipy.optimize import linprog
    except ImportError:
        print("[SKIP] LP cross-check (no scipy)")
        return
    for stack in (4.0, 10.0, 20.0):
        g = pushfold.Game(eq, pw, stack)
        n = NUM_CLASSES
        # max d.p + sum t  s.t. t_c <= (G^T p)_c, t_c <= 0, 0 <= p <= 1
        c = -np.concatenate([g.d, np.ones(n)])
        a_ub = np.hstack([-g.G.T, np.eye(n)])
        r = linprog(c, A_ub=a_ub, b_ub=np.zeros(n), bounds=[(0, 1)] * n + [(None, 0)] * n, method="highs")
        lp_value = g.base - r.fun
        s = int(np.where(stacks == stack)[0][0])
        cfr_value = res["value"][0, s]
        check(f"LP game value at {stack:g} bb matches CFR+", abs(lp_value - cfr_value) < 1e-3,
              f"LP {lp_value:+.5f}, CFR+ {cfr_value:+.5f} bb")


def test_headers():
    inc = os.path.normpath(os.path.join(HERE, "..", "..", "include"))
    path = os.path.join(inc, "poker_tables_pushfold.h")
    text = open(path, encoding="utf-8").read()
    m = re.search(r"constexpr uint8_t sb_push_freq.*?\n    \};", text, re.S)
    n_vals = len(re.findall(r"-?\d+,", m.group(0).split("= {", 1)[1])) if m else 0
    check("sb_push_freq has 2 * 49 * 169 entries", n_vals == 2 * 49 * 169, str(n_vals))
    data_bytes = 2 * 49 * 169 * (1 + 1 + 2 + 2)
    check("pushfold table data < 200 KB", data_bytes < 200 * 1024, f"{data_bytes / 1024:.0f} KB data, "
          f"{os.path.getsize(path) / 1024:.0f} KB source")
    gxx = os.path.normpath(os.path.join(HERE, "..", "..", "..", "..", "toolchain", "devkitPro", "devkitARM", "bin",
                                        "arm-none-eabi-g++.exe"))
    if not os.path.exists(gxx):
        gxx = shutil.which("arm-none-eabi-g++") or shutil.which("g++")
    if not gxx:
        print("[SKIP] header compile check (no g++)")
        return
    src = ('#include "poker_tables_pushfold.h"\n#include "poker_tables_equity.h"\n'
           'static_assert(poker::sb_push_freq[0][poker::pushfold_stack_index(20)][0] == 255);\n'
           'static_assert(poker::class_index(12, 11, true) == 1 && poker::class_index(0, 1, false) == 167);\n'
           'static_assert(poker::preflop_equity[0][14] > 8100 && poker::preflop_equity[0][14] < 8300);\n'
           'int main() { return poker::class_names[0][0]; }\n')
    r = subprocess.run([gxx, "-std=c++20", "-fsyntax-only", "-I", inc, "-x", "c++", "-"], input=src, text=True,
                       capture_output=True)
    check("headers compile (g++ -std=c++20 -fsyntax-only) + static_asserts", r.returncode == 0, r.stderr.strip()[:300])


def main():
    pw = pair_weights()
    eqd = equity.load()
    res = dict(np.load(solve.RESULTS_FILE))
    test_combos(pw)
    test_evaluator(eqd)
    test_equity(eqd, pw)
    test_pushfold(res, pw, eqd["equity"])
    test_headers()
    print(f"\n{'ALL OK' if not FAILS else str(len(FAILS)) + ' FAILED: ' + ', '.join(FAILS)}")
    sys.exit(1 if FAILS else 0)


if __name__ == "__main__":
    main()
