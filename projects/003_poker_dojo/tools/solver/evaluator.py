"""Vectorised 7-card hand evaluator (numpy).

Score = category << 20 | five 4-bit kickers (highest first). Bigger score = better hand.
Categories: 0 high card .. 8 straight flush (same order as include/poker_cards.h).
"""

import numpy as np

_M = 1 << 13
_idx = np.arange(_M)

POP = np.array([bin(m).count("1") for m in range(_M)], dtype=np.int64)
HIGH = np.array([m.bit_length() - 1 if m else 0 for m in range(_M)], dtype=np.int64)


def _top(n):
    t = np.zeros(_M, dtype=np.int64)
    for m in range(_M):
        v, k, r = 0, 0, 12
        while r >= 0 and k < n:
            if m >> r & 1:
                v |= r << (4 * (n - 1 - k))
                k += 1
            r -= 1
        t[m] = v
    return t


TOP2, TOP3, TOP5 = _top(2), _top(3), _top(5)


def _straights():
    t = np.full(_M, -1, dtype=np.int64)
    windows = [(hi, sum(1 << (hi - k) for k in range(5))) for hi in range(12, 3, -1)]
    wheel = (1 << 12) | 0b1111
    for m in range(_M):
        for hi, w in windows:
            if m & w == w:
                t[m] = hi
                break
        else:
            if m & wheel == wheel:
                t[m] = 3
    return t


STRAIGHT = _straights()
_POW2 = (1 << np.arange(13)).astype(np.int64)


def score_from_counts(counts, suit_masks):
    """counts: (N, 13) rank counts; suit_masks: (N, 4) 13-bit rank masks per suit. Returns (N,) int64."""
    rank_mask = (counts > 0).astype(np.int64) @ _POW2
    m2 = (counts >= 2).astype(np.int64) @ _POW2
    m3 = (counts >= 3).astype(np.int64) @ _POW2
    m4 = (counts == 4).astype(np.int64) @ _POW2

    fmask = np.where(POP[suit_masks] >= 5, suit_masks, 0).max(axis=1)
    sf = STRAIGHT[fmask]
    st = STRAIGHT[rank_mask]

    q = HIGH[m4]
    quads = (7 << 20) | (q << 16) | (HIGH[rank_mask & ~(1 << q)] << 12)
    t = HIGH[m3]
    rest2 = m2 & ~(1 << t)
    full = (6 << 20) | (t << 16) | (HIGH[rest2] << 12)
    trips = (3 << 20) | (t << 16) | (TOP2[rank_mask & ~(1 << t)] << 8)
    p1 = HIGH[m2]
    p2 = HIGH[m2 & ~(1 << p1)]
    two = (2 << 20) | (p1 << 16) | (p2 << 12) | (HIGH[rank_mask & ~(1 << p1) & ~(1 << p2)] << 8)
    pair = (1 << 20) | (p1 << 16) | (TOP3[rank_mask & ~(1 << p1)] << 4)

    conds = [sf >= 0, m4 > 0, (m3 > 0) & (rest2 > 0), fmask > 0, st >= 0, m3 > 0, POP[m2] >= 2, m2 > 0]
    vals = [(8 << 20) | (sf << 16), quads, full, (5 << 20) | TOP5[fmask], (4 << 20) | (st << 16), trips, two, pair]
    return np.select(conds, vals, default=TOP5[rank_mask])


def eval_cards(cards):
    """cards: (N, 7) int card ids. Generic (slower) entry point used by tests."""
    cards = np.asarray(cards, dtype=np.int64)
    r, s = cards >> 2, cards & 3
    counts = (r[:, :, None] == np.arange(13)).sum(axis=1)
    bits = 1 << r
    suit_masks = np.stack([np.where(s == k, bits, 0).sum(axis=1) for k in range(4)], axis=1)
    return score_from_counts(counts, suit_masks)
