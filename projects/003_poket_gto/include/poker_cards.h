// Cards, deck and a 7-card hand evaluator for POKER DOJO. No dynamic memory.

#ifndef POKER_CARDS_H
#define POKER_CARDS_H

#include "bn_seed_random.h"

namespace poker
{
    // Rank 0..12 = 2..A, suit 0..3 = spades, hearts, diamonds, clubs. Card id = rank * 4 + suit.
    struct card
    {
        int rank = 0;
        int suit = 0;

        [[nodiscard]] constexpr int id() const
        {
            return rank * 4 + suit;
        }

        [[nodiscard]] static constexpr card from_id(int id)
        {
            return card{ id / 4, id % 4 };
        }
    };

    constexpr const char* rank_names[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    constexpr char rank_chars[13] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

    enum hand_category
    {
        high_card, one_pair, two_pair, three_of_a_kind, straight, flush, full_house, four_of_a_kind, straight_flush
    };

    constexpr const char* category_names[9] = {
        "HIGH CARD", "PAIR", "TWO PAIR", "THREE OF A KIND", "STRAIGHT", "FLUSH", "FULL HOUSE", "FOUR OF A KIND",
        "STRAIGHT FLUSH",
    };

    class deck
    {

    public:
        void shuffle(bn::seed_random& random)
        {
            for(int i = 0; i < 52; ++i)
            {
                _cards[i] = i;
            }

            for(int i = 51; i > 0; --i)
            {
                int j = random.get_unbiased_int(i + 1);
                int t = _cards[i];
                _cards[i] = _cards[j];
                _cards[j] = t;
            }

            _next = 0;
        }

        [[nodiscard]] card draw()
        {
            return card::from_id(_cards[_next++]);
        }

    private:
        int _cards[52] = {};
        int _next = 0;
    };

    // Highest rank of a 5-long run in a 13-bit rank mask (A-2-3-4-5 counts with top 3), or -1.
    [[nodiscard]] constexpr int straight_top(int mask)
    {
        for(int top = 12; top >= 4; --top)
        {
            int run = 0x1F << (top - 4);

            if((mask & run) == run)
            {
                return top;
            }
        }

        constexpr int wheel = (1 << 12) | 0xF;
        return (mask & wheel) == wheel ? 3 : -1;
    }

    // Score of the best 5-card hand in `count` cards (5..7): category << 20 | five kickers of 4 bits.
    // Higher is better; equal scores split.
    [[nodiscard]] constexpr int evaluate(const card* cards, int count)
    {
        int rank_count[13] = {};
        int suit_count[4] = {};
        int suit_mask[4] = {};
        int all_mask = 0;

        for(int i = 0; i < count; ++i)
        {
            const card& c = cards[i];
            ++rank_count[c.rank];
            ++suit_count[c.suit];
            suit_mask[c.suit] |= 1 << c.rank;
            all_mask |= 1 << c.rank;
        }

        for(int s = 0; s < 4; ++s)
        {
            if(suit_count[s] >= 5)
            {
                int top = straight_top(suit_mask[s]);

                if(top >= 0)
                {
                    return (straight_flush << 20) | (top << 16);
                }
            }
        }

        int quads = -1;
        int trips[2] = { -1, -1 };
        int pairs[3] = { -1, -1, -1 };
        int trips_count = 0;
        int pairs_count = 0;

        for(int r = 12; r >= 0; --r)
        {
            if(rank_count[r] == 4)
            {
                quads = r;
            }
            else if(rank_count[r] == 3 && trips_count < 2)
            {
                trips[trips_count++] = r;
            }
            else if(rank_count[r] == 2 && pairs_count < 3)
            {
                pairs[pairs_count++] = r;
            }
        }

        auto kickers = [&](int exclude_a, int exclude_b, int needed) {
            int result = 0;

            for(int r = 12; r >= 0 && needed > 0; --r)
            {
                if(rank_count[r] && r != exclude_a && r != exclude_b)
                {
                    result = (result << 4) | r;
                    --needed;
                }
            }

            return result;
        };

        if(quads >= 0)
        {
            return (four_of_a_kind << 20) | (quads << 16) | (kickers(quads, -1, 1) << 12);
        }

        if(trips_count && (trips_count > 1 || pairs_count))
        {
            int pair = trips_count > 1 ? trips[1] : pairs[0];

            if(trips_count > 1 && pairs_count && pairs[0] > pair)
            {
                pair = pairs[0];
            }

            return (full_house << 20) | (trips[0] << 16) | (pair << 12);
        }

        for(int s = 0; s < 4; ++s)
        {
            if(suit_count[s] >= 5)
            {
                int result = 0;
                int needed = 5;

                for(int r = 12; r >= 0 && needed > 0; --r)
                {
                    if(suit_mask[s] & (1 << r))
                    {
                        result = (result << 4) | r;
                        --needed;
                    }
                }

                return (flush << 20) | result;
            }
        }

        int top = straight_top(all_mask);

        if(top >= 0)
        {
            return (straight << 20) | (top << 16);
        }

        if(trips_count)
        {
            return (three_of_a_kind << 20) | (trips[0] << 16) | (kickers(trips[0], -1, 2) << 8);
        }

        if(pairs_count >= 2)
        {
            return (two_pair << 20) | (pairs[0] << 16) | (pairs[1] << 12) | (kickers(pairs[0], pairs[1], 1) << 8);
        }

        if(pairs_count)
        {
            return (one_pair << 20) | (pairs[0] << 16) | (kickers(pairs[0], -1, 3) << 4);
        }

        return (high_card << 20) | kickers(-1, -1, 5);
    }

    [[nodiscard]] constexpr int category_of(int score)
    {
        return score >> 20;
    }

    // Starting hand class 0..168 in the classic 13x13 grid: row/column by rank A..2, pairs on the diagonal,
    // suited above it (row < column), offsuit below.
    [[nodiscard]] constexpr int hand_class(card a, card b)
    {
        int hi = a.rank > b.rank ? a.rank : b.rank;
        int lo = a.rank > b.rank ? b.rank : a.rank;
        int row_hi = 12 - hi;
        int row_lo = 12 - lo;

        if(hi == lo)
        {
            return row_hi * 13 + row_hi;
        }

        return a.suit == b.suit ? row_hi * 13 + row_lo : row_lo * 13 + row_hi;
    }

    // Self checks of the evaluator, evaluated at compile time.
    namespace detail
    {
        constexpr card c(int rank, int suit)
        {
            return card{ rank, suit };
        }

        constexpr card royal[7] = { c(12, 0), c(11, 0), c(10, 0), c(9, 0), c(8, 0), c(0, 1), c(1, 2) };
        constexpr card wheel[7] = { c(12, 0), c(0, 1), c(1, 2), c(2, 3), c(3, 0), c(9, 1), c(10, 2) };
        constexpr card boat[7] = { c(5, 0), c(5, 1), c(5, 2), c(7, 0), c(7, 1), c(7, 2), c(1, 3) };
        constexpr card two_pairs[7] = { c(5, 0), c(5, 1), c(7, 2), c(7, 0), c(9, 1), c(9, 2), c(1, 3) };

        static_assert(category_of(evaluate(royal, 7)) == straight_flush);
        static_assert(category_of(evaluate(wheel, 7)) == straight && ((evaluate(wheel, 7) >> 16) & 15) == 3);
        static_assert(category_of(evaluate(boat, 7)) == full_house && ((evaluate(boat, 7) >> 12) & 15) == 5);
        static_assert(category_of(evaluate(two_pairs, 7)) == two_pair &&
                      ((evaluate(two_pairs, 7) >> 8) & 15) == 5);   // kicker is the third pair's 7
        static_assert(hand_class(c(12, 0), c(12, 1)) == 0);          // AA top-left
        static_assert(hand_class(c(12, 0), c(11, 0)) == 1);          // AKs right of AA
        static_assert(hand_class(c(12, 0), c(11, 1)) == 13);         // AKo below AA
        static_assert(hand_class(c(0, 0), c(0, 1)) == 168);          // 22 bottom-right
    }
}

#endif
