"""Heads-up push/fold Nash equilibrium (SB shoves or folds, BB calls or folds) solved with CFR+.

Conventions (all amounts in big blinds, EV measured from the start of the hand, before posting):
  * both players start with S bb (effective stack), SB posts 0.5, BB posts 1;
  * with ante, each player also posts A = 0.1 bb of dead money out of the same stack;
  * SB fold: -0.5 - A.  SB push, BB fold: +1 + A.  Push and call: all-in for S each, SB nets (2 e - 1) S,
    where e is the 169x169 all-in equity (ties split).  BB fold to a push: -1 - A.
Hands are the 169 classes; the joint deal probability P[h, c] = combo pairs without shared cards / (1326 * 1225).
"""

import numpy as np

TOTAL_PAIRS = 1326 * 1225


class Game:
    def __init__(self, equity, pair_w, stack, ante=0.0):
        self.S, self.A = float(stack), float(ante)
        self.P = pair_w / TOTAL_PAIRS
        self.Ph = self.P.sum(axis=1)  # marginal of a class (both seats; P is symmetric)
        # SB utility: U(p, q) = base + p . d + p^T G q
        self.base = -0.5 - self.A
        self.d = self.Ph * (1.5 + 2.0 * self.A)
        self.G = self.P * ((2.0 * equity - 1.0) * self.S - (1.0 + self.A))

    def sb_gain(self, q):
        """Per class, probability-weighted EV(push) - EV(fold) for the SB against call strategy q."""
        return self.d + self.G @ q

    def bb_gain(self, p):
        """Per class, probability-weighted EV(call) - EV(fold) for the BB (its own utility) against push p."""
        return -(self.G.T @ p)

    def value(self, p, q):
        return self.base + p @ self.d + p @ self.G @ q

    def exploitability(self, p, q):
        """NashConv in bb per hand: SB best-response gain + BB best-response gain."""
        br_sb = self.base + np.maximum(self.sb_gain(q), 0).sum()
        br_bb = self.base + p @ self.d - np.maximum(self.bb_gain(p), 0).sum()
        return br_sb - br_bb

    def ev_tables(self, p, q):
        """Conditional EVs in bb: push - fold for each SB class; call - fold for each BB class facing a push."""
        ev_push = self.sb_gain(q) / self.Ph
        facing = self.P.T @ p  # P(BB has c and SB pushed)
        ev_call = np.where(facing > 0, self.bb_gain(p) / np.maximum(facing, 1e-300), 0.0)
        return ev_push, ev_call


def solve(game, target=1e-4, max_iters=200000, check_every=100):
    """CFR+ (regret matching+, alternating updates, linear averaging). Returns (p, q, exploitability, iters)."""
    n = len(game.d)
    rp, rf = np.zeros(n), np.zeros(n)  # SB regrets push / fold
    rc, rx = np.zeros(n), np.zeros(n)  # BB regrets call / fold
    sp, sq = np.zeros(n), np.zeros(n)
    wsum = 0.0
    p = np.full(n, 0.5)
    q = np.full(n, 0.5)
    expl = np.inf
    it = 0
    for it in range(1, max_iters + 1):
        g = game.sb_gain(q)
        u = p * g
        rp = np.maximum(rp + g - u, 0)
        rf = np.maximum(rf - u, 0)
        tot = rp + rf
        p = np.where(tot > 0, rp / np.where(tot > 0, tot, 1), 0.5)

        g = game.bb_gain(p)
        u = q * g
        rc = np.maximum(rc + g - u, 0)
        rx = np.maximum(rx - u, 0)
        tot = rc + rx
        q = np.where(tot > 0, rc / np.where(tot > 0, tot, 1), 0.5)

        sp += it * p
        sq += it * q
        wsum += it
        if it % check_every == 0:
            expl = game.exploitability(sp / wsum, sq / wsum)
            if expl < target:
                break
    return sp / wsum, sq / wsum, expl, it
