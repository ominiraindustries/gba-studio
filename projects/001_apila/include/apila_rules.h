// Modern falling-block rules data: piece shapes, SRS kick tables, gravity and scoring tables.
// Coordinates are y-down inside the board; kick tables are written y-up (as usually published) and negated on use.

#ifndef APILA_RULES_H
#define APILA_RULES_H

namespace apila
{
    constexpr int board_columns = 10;
    constexpr int board_rows = 24;          // 4 hidden rows above the 20 visible ones
    constexpr int hidden_rows = 4;
    constexpr int visible_rows = board_rows - hidden_rows;

    enum piece_type { piece_i, piece_o, piece_t, piece_s, piece_z, piece_j, piece_l, piece_count };

    struct point
    {
        int x;
        int y;
    };

    // Rotation state 0 cells inside the piece box (4x4 for I and O, 3x3 for the rest).
    constexpr point spawn_cells[piece_count][4] = {
        { {0, 1}, {1, 1}, {2, 1}, {3, 1} },   // I
        { {1, 0}, {2, 0}, {1, 1}, {2, 1} },   // O
        { {1, 0}, {0, 1}, {1, 1}, {2, 1} },   // T
        { {1, 0}, {2, 0}, {0, 1}, {1, 1} },   // S
        { {0, 0}, {1, 0}, {1, 1}, {2, 1} },   // Z
        { {0, 0}, {0, 1}, {1, 1}, {2, 1} },   // J
        { {2, 0}, {0, 1}, {1, 1}, {2, 1} },   // L
    };

    constexpr int box_size[piece_count] = { 4, 4, 3, 3, 3, 3, 3 };

    // [from state][0 = clockwise, 1 = counterclockwise][test], y-up.
    constexpr point kicks_jlstz[4][2][5] = {
        { { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} }, { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} } },
        { { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} },     { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} } },
        { { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} },    { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} } },
        { { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} },  { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} } },
    };

    constexpr point kicks_i[4][2][5] = {
        { { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} },   { {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} } },
        { { {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} },   { {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} } },
        { { {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} },   { {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} } },
        { { {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} },   { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} } },
    };

    // T piece corners that face the flat side ("front") per rotation state, in its 3x3 box.
    constexpr point t_front_corners[4][2] = {
        { {0, 0}, {2, 0} }, { {2, 0}, {2, 2} }, { {0, 2}, {2, 2} }, { {0, 0}, {0, 2} },
    };

    // Gravity in 1/65536 rows per frame for levels 1..20: (0.8 - (level - 1) * 0.007) ^ (level - 1) s per row.
    constexpr int max_level = 20;
    constexpr int gravity_table[max_level] = {
        1092, 1377, 1768, 2311, 3075, 4169, 5759, 8107, 11634, 17026,
        25416, 38709, 60169, 95483, 154742, 256187, 433425, 749597, 1325716, 2398490,
    };

    constexpr int soft_drop_factor = 20;
    constexpr int lock_delay_frames = 30;
    constexpr int max_lock_resets = 15;
    constexpr int das_frames = 10;
    constexpr int arr_frames = 2;
    constexpr int clear_delay_frames = 20;
    constexpr int lines_per_level = 10;
    constexpr int next_count = 5;

    // Points per lines cleared (multiplied by level).
    constexpr int score_plain[5] = { 0, 100, 300, 500, 800 };
    constexpr int score_tspin_mini[3] = { 100, 200, 400 };
    constexpr int score_tspin[4] = { 400, 800, 1200, 1600 };
    constexpr int score_perfect_clear[5] = { 0, 800, 1200, 1800, 2000 };
    constexpr int score_perfect_clear_b2b_four = 3200;
}

#endif
