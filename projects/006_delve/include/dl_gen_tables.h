// DELVE floor generator tables (task G-25).
//
// v2_bands: dungeon_model.py v2 DEFAULT_BANDS (Desktop/SNES/tools/scripts/dungeon_model.py) unchanged; they are the
// reference for the bit-exact fidelity test (tools/gen_metrics.py --fidelity).
// game_bands: what DELVE plays (DISENO.md section 6): band 0 with maze_pct = 0, main_pct = 0 (locks off the main
// path; the hooks turn any remaining one into a door), lock_free = 1, monsters >= 8 cells.
// Hook tables (DISENO.md section 6b): reinterpretation of the v2 trap type, furniture type and wall var per floor.
// Block 1 uses the floor 1 table on every floor.

#ifndef DL_GEN_TABLES_H
#define DL_GEN_TABLES_H

#include <stdint.h>

namespace dl::gen
{
    // DunCount of the v2: chance %, dice count, dice sides, bonus
    struct count
    {
        uint8_t pct;
        uint8_t dice_n;
        uint8_t dice_sides;
        uint8_t bonus;
    };

    // Same columns and order as the v2 Band namedtuple.
    struct band
    {
        uint8_t layout_mask, rooms_min, rooms_max, side_min, side_max, door_pct, join_pct, n_locks, key_kinds,
                main_pct, stairs_min;
        count monsters, items, traps, teleports, secrets, furniture;
        uint8_t tier_min, tier_max, treasure_pct, lair_pct, shop_pct;
        uint8_t open_pct, open_lo, open_hi, dead_lo, dead_hi, room_pct, straight, nclu_max, maze_pct;
    };

    struct tables
    {
        const band* bands;
        int band_count;
        int band_levels;    // levels per band (v2: 5)
        int lock_free;      // levels without locks (v2: 2)
        int max_ent;        // v2: 160
        int spawn_clear;    // v2 SPAWN_CLEAR: 6
    };

    constexpr band v2_bands[] = {
        { 0b0010, 4, 7, 3, 5, 40, 45, 1, 2, 60, 30,
          { 100, 1, 6, 4 }, { 100, 1, 4, 3 }, { 50, 1, 2, 0 }, { 0, 0, 0, 0 }, { 30, 1, 2, 0 }, { 60, 1, 3, 0 },
          0, 1, 20, 0, 0,
          28, 18, 42, 2, 9, 30, 88, 2, 8 },
        { 0b0110, 5, 8, 3, 6, 45, 42, 2, 3, 60, 32,
          { 100, 2, 4, 5 }, { 100, 1, 6, 3 }, { 70, 1, 3, 1 }, { 30, 1, 1, 0 }, { 50, 1, 3, 0 }, { 60, 1, 3, 1 },
          1, 3, 30, 25, 25,
          32, 22, 46, 2, 10, 32, 86, 2, 10 },
        { 0b1110, 5, 9, 3, 6, 45, 40, 3, 3, 60, 34,
          { 100, 2, 4, 7 }, { 100, 1, 6, 3 }, { 80, 1, 3, 2 }, { 50, 1, 2, 0 }, { 60, 1, 3, 1 }, { 60, 1, 3, 1 },
          2, 5, 35, 35, 30,
          36, 26, 50, 2, 11, 34, 84, 3, 12 },
        { 0b0110, 6, 10, 3, 6, 50, 38, 4, 4, 60, 36,
          { 100, 3, 4, 8 }, { 100, 1, 6, 2 }, { 90, 1, 4, 2 }, { 60, 1, 2, 0 }, { 60, 1, 3, 1 }, { 60, 1, 3, 1 },
          4, 7, 40, 45, 25,
          40, 30, 52, 3, 13, 36, 82, 3, 8 },
        { 0b1110, 6, 10, 3, 6, 55, 35, 5, 4, 60, 38,
          { 100, 3, 4, 10 }, { 100, 1, 6, 1 }, { 100, 1, 4, 2 }, { 70, 1, 3, 0 }, { 70, 1, 3, 1 }, { 60, 1, 3, 1 },
          6, 9, 40, 60, 20,
          44, 34, 56, 3, 14, 38, 80, 3, 14 },
    };

    // DISENO.md 6: band 0 for floors 1-3 with maze_pct 0, generic keys (key_kinds 1); the rest as the v2.
    constexpr band game_bands[] = {
        { 0b0010, 4, 7, 3, 5, 40, 45, 1, 1, 0, 30,
          { 100, 1, 6, 4 }, { 100, 1, 4, 3 }, { 50, 1, 2, 0 }, { 0, 0, 0, 0 }, { 30, 1, 2, 0 }, { 60, 1, 3, 0 },
          0, 1, 20, 0, 0,
          28, 18, 42, 2, 9, 30, 88, 2, 0 },
        v2_bands[1], v2_bands[2], v2_bands[3], v2_bands[4],
    };

    constexpr tables v2_tables = { v2_bands, 5, 5, 2, 160, 6 };
    constexpr tables game_tables = { game_bands, 5, 3, 1, 160, 8 };

    // v2 shape of the dense MAZE variant
    constexpr int maze_open = 78, maze_lo = 58, maze_hi = 92, maze_dead_lo = 0, maze_dead_hi = 40, maze_straight = 30;
    constexpr int lock_far_min = 6;
    constexpr int lock_near_min = 10;
    constexpr int max_attempts = 6;

    // ------------------------------------------------------------------------------------------- hook tables
    enum trap_type : uint8_t
    {
        TRAP_SPIKES,    // floor 1 in the hooks' floor byte
        TRAP_DART,      // 2
        TRAP_RUNE,      // 3
        TRAP_PIT,       // 5 (hook, block 3)
        TRAP_COUNT
    };

    constexpr bool trap_dodgeable[TRAP_COUNT] = { true, true, false, false };

    enum furn_type : uint8_t
    {
        FURN_BARREL,
        FURN_CRATE,
        FURN_BONES,
        FURN_STATUE,
        FURN_CHEST,
        FURN_TABLE,
        FURN_PUDDLE,
        FURN_COUNT
    };

    constexpr bool furn_blocks[FURN_COUNT] = { true, true, false, true, true, true, false };

    enum decor : uint8_t
    {
        DECOR_NONE,
        DECOR_TORCH,
        DECOR_BANNER,
        DECOR_CHAINS,
        DECOR_SKULLS,
        DECOR_CRACK,    // fixed: every SECRET edge (block 2)
        DECOR_NICHE,
        DECOR_COUNT
    };

    struct floor_table
    {
        uint8_t trap_of_t[3];       // v2 trap t = rnd(3)
        uint8_t furn_of_b[8];       // v2 furniture b = rnd(8)
        uint8_t decor_of_var[16];   // v2 wall / door / arch var
        uint8_t max_traps;
        uint8_t p1_count;           // P1 shortcut gates (lever + bars)
        uint8_t p1_min_steps;       // a and b at least this far apart by path
        uint8_t trap_min_entry;     // traps at least this far from the entry
        uint8_t trap_spacing;       // between traps
        uint8_t monster_trap_gap;   // no monster closer than this to a trap (0 = rule off)
    };

    // DISENO.md 6b, floor 1 "The Cellars" (block 1 uses it on every floor)
    constexpr floor_table floor_tables[] = {
        { { TRAP_SPIKES, TRAP_SPIKES, TRAP_SPIKES },
          { FURN_BARREL, FURN_BARREL, FURN_BARREL, FURN_CRATE, FURN_BONES, FURN_BONES, FURN_PUDDLE, FURN_PUDDLE },
          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DECOR_TORCH },
          1, 1, 12, 6, 4, 4 },
    };
    constexpr int floor_table_count = int(sizeof(floor_tables) / sizeof(floor_tables[0]));
}

#endif
