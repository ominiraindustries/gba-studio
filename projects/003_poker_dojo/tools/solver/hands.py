"""Card, combo and starting-hand class conventions shared by the solver.

Card id = rank * 4 + suit, rank 0..12 = 2..A, suit 0..3 (same as include/poker_cards.h).

Class index 0..168 follows the classic 13x13 matrix: row and column are rank positions ordered A..2
(position 0 = A, 12 = 2), index = row * 13 + col.
  row == col -> pocket pair
  col >  row -> suited   (above the diagonal), high = row, low = col
  row >  col -> offsuit  (below the diagonal), high = col, low = row
"""

import numpy as np

RANK_CHARS = "23456789TJQKA"
NUM_CLASSES = 169


def rank_to_pos(rank):
    """Rank 0..12 (2..A) -> matrix position 0..12 (A..2)."""
    return 12 - rank


def class_index(rank_a, rank_b, suited):
    """Class index from two card ranks (0..12 = 2..A) and suitedness."""
    hi, lo = max(rank_a, rank_b), min(rank_a, rank_b)
    ph, pl = rank_to_pos(hi), rank_to_pos(lo)
    if hi == lo:
        return ph * 13 + ph
    if suited:
        return ph * 13 + pl
    return pl * 13 + ph


def class_name(idx):
    row, col = divmod(idx, 13)
    r_row, r_col = RANK_CHARS[12 - row], RANK_CHARS[12 - col]
    if row == col:
        return r_row + r_col
    if col > row:
        return r_row + r_col + "s"
    return r_col + r_row + "o"


CLASS_NAMES = [class_name(i) for i in range(NUM_CLASSES)]


def class_kind(idx):
    row, col = divmod(idx, 13)
    return "pair" if row == col else ("suited" if col > row else "offsuit")


def build_combos():
    """All 1326 two-card combos (c1 > c2) with their class and 52-bit card mask."""
    c1, c2 = [], []
    for a in range(52):
        for b in range(a):
            c1.append(a)
            c2.append(b)
    c1 = np.array(c1, dtype=np.int64)
    c2 = np.array(c2, dtype=np.int64)
    cls = np.array([class_index(a >> 2, b >> 2, (a & 3) == (b & 3)) for a, b in zip(c1, c2)], dtype=np.int64)
    bits = (np.uint64(1) << c1.astype(np.uint64)) | (np.uint64(1) << c2.astype(np.uint64))
    return c1, c2, cls, bits


COMBO_C1, COMBO_C2, COMBO_CLASS, COMBO_BITS = build_combos()
NUM_COMBOS = len(COMBO_C1)  # 1326
CLASS_COMBOS = np.bincount(COMBO_CLASS, minlength=NUM_CLASSES)  # 6 / 4 / 12


def pair_weights():
    """W[a, b] = number of (combo of a, combo of b) pairs with no shared card (card removal)."""
    share = (COMBO_BITS[:, None] & COMBO_BITS[None, :]) != 0
    onehot = np.zeros((NUM_COMBOS, NUM_CLASSES), dtype=np.int64)
    onehot[np.arange(NUM_COMBOS), COMBO_CLASS] = 1
    disjoint = (~share).astype(np.int64)
    return onehot.T @ disjoint @ onehot
