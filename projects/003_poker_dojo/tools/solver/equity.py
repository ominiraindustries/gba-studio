"""Exact 169x169 preflop all-in equities with card removal.

Method: enumerate the 134,459 suit-isomorphic 5-card boards (weight = orbit size, total C(52,5)); for every
board, score the 1326 hole combos and, through a per-class cumulative count over the rank-sorted combos, add for
each combo the wins/ties/total against every class. Combos sharing a card with the hero are removed with an exact
correction (the ~100 overlapping combos per hero). Class-level sums are suit-invariant, so the result is exact.

E[a, b] = P(class a beats class b) + P(tie) / 2, averaged over all disjoint combo pairs and all boards.
"""

import itertools
import os
import sys
import time

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))  # python -I does not add the script dir

import evaluator as ev
from hands import pair_weights, COMBO_BITS, COMBO_C1, COMBO_C2, COMBO_CLASS, NUM_CLASSES, NUM_COMBOS

CACHE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "cache")
CACHE_FILE = os.path.join(CACHE_DIR, "equity169.npz")


def canonical_boards():
    """Unique boards under suit permutation. Returns (boards (K,5) int64, weights (K,) int64)."""
    boards = np.array(list(itertools.combinations(range(52), 5)), dtype=np.int64)
    best = None
    for perm in itertools.permutations(range(4)):
        perm = np.array(perm, dtype=np.int64)
        mapped = np.sort((boards >> 2) * 4 + perm[boards & 3], axis=1)
        key = mapped @ (64 ** np.arange(5, dtype=np.int64))
        best = key if best is None else np.minimum(best, key)
    keys, counts = np.unique(best, return_counts=True)
    canon = np.stack([(keys >> (6 * k)) & 63 for k in range(5)], axis=1)
    return canon, counts.astype(np.int64)


def _overlap_lists():
    """For each combo, the 101 combos sharing at least one card with it (itself included)."""
    share = (COMBO_BITS[:, None] & COMBO_BITS[None, :]) != 0
    ov = np.array([np.nonzero(row)[0] for row in share], dtype=np.int64)
    assert ov.shape == (NUM_COMBOS, 101)
    return ov


def compute(progress=True, max_boards=None):
    t0 = time.time()
    boards, weights = canonical_boards()
    assert weights.sum() == 2598960
    if max_boards:
        boards, weights = boards[:max_boards], weights[:max_boards]
    if progress:
        print(f"canonical boards: {len(boards)} ({time.time() - t0:.1f} s)", flush=True)

    ar = np.arange(NUM_COMBOS)
    combo_counts = np.zeros((NUM_COMBOS, 13), dtype=np.int64)
    np.add.at(combo_counts, (ar, COMBO_C1 >> 2), 1)
    np.add.at(combo_counts, (ar, COMBO_C2 >> 2), 1)
    combo_suits = np.zeros((NUM_COMBOS, 4), dtype=np.int64)
    np.add.at(combo_suits, (ar, COMBO_C1 & 3), 1 << (COMBO_C1 >> 2))
    np.add.at(combo_suits, (ar, COMBO_C2 & 3), 1 << (COMBO_C2 >> 2))
    ov = _overlap_lists()

    # Unweighted int32 accumulators per orbit weight (no multiply in the hot loop); merged at the end.
    acc = {}
    cat_counts = np.zeros(9, dtype=np.int64)
    rows = np.arange(1, NUM_COMBOS + 1)
    m = np.zeros((NUM_COMBOS + 1, NUM_CLASSES), dtype=np.int32)

    for n, (board, w) in enumerate(zip(boards, weights)):
        if w not in acc:
            acc[w] = [np.zeros((NUM_COMBOS, NUM_CLASSES), np.int32), np.zeros((NUM_COMBOS, NUM_CLASSES), np.int32),
                      np.zeros((NUM_COMBOS, 101), np.int32), np.zeros((NUM_COMBOS, 101), np.int32)]
        a_less, a_leq, a_ow, a_ot = acc[w]
        bmask = 0
        bcounts = np.zeros(13, dtype=np.int64)
        bsuits = np.zeros(4, dtype=np.int64)
        for c in board.tolist():
            bmask |= 1 << c
            bcounts[c >> 2] += 1
            bsuits[c & 3] |= 1 << (c >> 2)
        valid = (COMBO_BITS & np.uint64(bmask)) == 0
        score = ev.score_from_counts(combo_counts + bcounts, combo_suits | bsuits)
        cat_counts += np.bincount(score[valid] >> 20, minlength=9) * w
        # Invalid combos sort first with score -1 and count as nobody: their rows of cum[lo] / cum[hi] are 0.
        score[~valid] = -1

        order = np.argsort(score, kind="stable")
        s_sorted = score[order]
        m.fill(0)
        m[rows, COMBO_CLASS[order]] = valid[order]
        cum = np.cumsum(m, axis=0)
        a_less += cum[np.searchsorted(s_sorted, score, "left")]
        a_leq += cum[np.searchsorted(s_sorted, score, "right")]

        s_ov = score[ov]
        v_ov = valid[ov]
        a_ow += (score[:, None] > s_ov) & v_ov
        a_ot += (score[:, None] == s_ov) & v_ov

        if progress and n % 10000 == 0:
            el = time.time() - t0
            print(f"  board {n}/{len(boards)}  {el:.0f} s", flush=True)

    win = np.zeros((NUM_COMBOS, NUM_CLASSES), dtype=np.int64)
    leq = np.zeros_like(win)
    ow = np.zeros((NUM_COMBOS, 101), dtype=np.int64)
    ot = np.zeros_like(ow)
    for w, (a_less, a_leq, a_ow, a_ot) in acc.items():
        win += int(w) * a_less.astype(np.int64)
        leq += int(w) * a_leq.astype(np.int64)
        ow += int(w) * a_ow.astype(np.int64)
        ot += int(w) * a_ot.astype(np.int64)
    tie = leq - win

    # Remove the overlapping (card-sharing) opponents.
    ov_cls = COMBO_CLASS[ov]
    rr = np.repeat(ar, 101).reshape(NUM_COMBOS, 101)
    np.add.at(win, (rr, ov_cls), -ow)
    np.add.at(tie, (rr, ov_cls), -ot)

    onehot = np.zeros((NUM_COMBOS, NUM_CLASSES), dtype=np.int64)
    onehot[ar, COMBO_CLASS] = 1
    W = onehot.T @ win
    T = onehot.T @ tie
    # Every disjoint combo pair sees C(48, 5) boards.
    N = pair_weights() * (1712304 if max_boards is None else 1)
    eq = (W + 0.5 * T) / N
    elapsed = time.time() - t0
    if progress:
        print(f"equity done in {elapsed:.0f} s", flush=True)
    return {"equity": eq, "wins": W, "ties": T, "totals": N, "categories": cat_counts, "seconds": elapsed}


def load(recompute=False):
    if not recompute and os.path.exists(CACHE_FILE):
        d = np.load(CACHE_FILE)
        return {k: d[k] for k in d.files}
    res = compute()
    os.makedirs(CACHE_DIR, exist_ok=True)
    np.savez_compressed(CACHE_FILE, **{k: np.asarray(v) for k, v in res.items()})
    return res


if __name__ == "__main__":
    r = load(recompute="--recompute" in sys.argv)
    print("AA vs KK:", r["equity"][0, 14])
