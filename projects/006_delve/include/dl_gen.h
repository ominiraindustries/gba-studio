// DELVE floor generator: C++ port of dungeon_model.py v2 (Desktop/SNES/tools/scripts/dungeon_model.py,
// DUN_GEN_VERSION 2) plus the game hooks phase of DISENO.md 6b.
//
// The v2 part is bit-exact with the Python model (same PRNG, same order of every draw, same arrays): level_bytes()
// and crc16() give the same bytes as dungeon_model.level_bytes / level_crc (tools/gen_metrics.py --fidelity).
// No dynamic memory: one generator state in EWRAM, the result in a caller-owned dl::gen::level.
// Portable C++ (no Butano include) so tools/gen_metrics.py can compile it for the PC.

#ifndef DL_GEN_H
#define DL_GEN_H

#include <stdint.h>

#include "dl_gen_tables.h"

namespace dl::gen
{
    constexpr int W = 32;
    constexpr int H = 32;
    constexpr int N = W * H;
    constexpr int max_ents = 192;   // the v2 caps add_ent at max_ent (160) but stairs / keys / rooms bypass the cap
    constexpr int max_rooms = 15;
    constexpr int max_locks = 8;
    constexpr int max_hooks = 8;

    // edge kinds (low nibble of an edge byte; high nibble = var)
    enum kind : uint8_t { K_WALL, K_OPEN, K_DOOR, K_LOCKED, K_SECRET, K_ARCH, K_BARS, K_STAIRS };
    enum layout_id : uint8_t { L_MAZE, L_ROOMS, L_CROSS, L_ARENA };
    enum ent_id : uint8_t { E_NONE, E_MONSTER, E_ITEM, E_KEY, E_TRAP, E_TELE, E_FURN, E_STAIRS, E_ENTRY, E_ROOM };
    enum special_id : uint8_t { SP_NONE, SP_TREASURE, SP_LAIR, SP_SHOP };
    enum hook_id : uint8_t { H_NONE, H_LEVER, H_PLATE };

    constexpr uint8_t C_STAIRS = 0x10;
    constexpr uint8_t C_ENT = 0x20;
    constexpr uint8_t C_ENTRY = 0x40;
    constexpr uint8_t C_CARVED = 0x80;

    constexpr int dx[4] = { 0, 1, 0, -1 };     // N, E, S, W
    constexpr int dy[4] = { -1, 0, 1, 0 };

    // dungeon_model.py v2: Rng (xorshift16 7,9,8 + 16-bit LCG; rnd(n) = (out * n) >> 16)
    struct v2_rng
    {
        unsigned s1 = 1;
        unsigned s2 = 0;

        void seed(unsigned seed32, int level, int attempt)
        {
            unsigned lo = seed32 & 0xFFFF;
            unsigned hi = (seed32 >> 16) & 0xFFFF;
            unsigned a = (lo + unsigned(level) * 0x9E37 + unsigned(attempt) * 0x6C8F) & 0xFFFF;
            unsigned b = (hi ^ ((unsigned(level) * 0x1F35) & 0xFFFF) ^ ((unsigned(attempt) * 0xA3D1) & 0xFFFF)) & 0xFFFF;

            for(int i = 0; i < 2; ++i)
            {
                a = (((a ^ (a >> 7)) * 0x2F6B) + b) & 0xFFFF;
                b = (((b ^ (b >> 5)) * 0x1D47) + a) & 0xFFFF;
            }

            s1 = a ? a : 0xACE1;
            s2 = b;

            for(int i = 0; i < 3; ++i)
            {
                next16();
            }
        }

        unsigned next16()
        {
            unsigned x = s1;
            x ^= (x << 7) & 0xFFFF;
            x ^= x >> 9;
            x ^= (x << 8) & 0xFFFF;
            s1 = x;
            unsigned s = (s2 * 25173 + 13849) & 0xFFFF;
            s2 = s;
            return x ^ (((s & 0xFF) << 8) | (s >> 8));
        }

        int rnd(int n)
        {
            return int((next16() * unsigned(n)) >> 16);
        }
    };

    struct entity
    {
        uint8_t type, x, y, a, b, c;    // the v2 6-byte record
    };

    struct room
    {
        uint8_t x, y, w, h, special;
    };

    struct lock
    {
        int16_t edge;
        uint8_t kind;
    };

    struct hook
    {
        uint8_t type;       // H_LEVER / H_PLATE
        uint8_t cell_x;
        uint8_t cell_y;
        uint8_t side;       // lever: wall side of the cell (0..3)
        int16_t target;     // BARS edge it opens
        int16_t lever_edge; // edge index of the lever wall
    };

    struct level
    {
        uint8_t ed[2 * N];  // E edges [0, N), S edges [N, 2N)
        uint8_t cl[N];
        uint8_t th[N];
        entity ents[max_ents];
        int nents;
        room rooms[max_rooms];
        int nrooms;
        lock locks[max_locks];
        int nlocks;
        int layout;
        int entry;
        int entry_dir;
        int stairs;
        int attempts;
        int fallback;
        int tunnels;
        int ncar;
        int dead_ends;
        int regions;
        // game hooks (DISENO.md 6b)
        hook hooks[max_hooks];
        int nhooks;
        int floor_table;
        int hook_removed_traps;
        int hook_changed_furn;
        int hook_removed_locks;
    };

    // topology helpers (the v2 NB / EIDX tables as arithmetic)
    [[nodiscard]] constexpr int neighbour(int c, int d)
    {
        int x = (c & (W - 1)) + dx[d];
        int y = (c >> 5) + dy[d];
        return (x >= 0 && x < W && y >= 0 && y < H) ? y * W + x : -1;
    }

    [[nodiscard]] constexpr int edge_index(int c, int d)
    {
        int x = c & (W - 1);
        int y = c >> 5;
        switch(d)
        {
        case 0:
            return y > 0 ? N + c - W : -1;
        case 1:
            return x < W - 1 ? c : -1;
        case 2:
            return y < H - 1 ? N + c : -1;
        default:
            return x > 0 ? c - 1 : -1;
        }
    }

    [[nodiscard]] constexpr int edge_a(int idx)
    {
        return idx < N ? idx : idx - N;
    }

    [[nodiscard]] constexpr int edge_b(int idx)
    {
        return idx < N ? idx + 1 : idx - N + W;
    }

    [[nodiscard]] inline int kind_at(const level& lv, int c, int d)
    {
        int i = edge_index(c, d);
        return i >= 0 ? (lv.ed[i] & 15) : K_WALL;
    }

    // passable for the connectivity graph (v2 GRAPH: also LOCKED) / for a flood without keys (v2 FLOOD)
    constexpr uint8_t graph_kind[16] = { 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
    constexpr uint8_t flood_kind[16] = { 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
    constexpr uint8_t sight_kind[16] = { 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 };

    // The v2 generate(seed32, level, tables): returns false only if the fallback failed (never in the v2 tests).
    bool generate_v2(unsigned seed32, int level_index, const tables& t, level& out);

    // Game floor: v2 + hooks + game validation (DISENO.md 6: seed + 1 up to 8 times). Returns the seed used.
    unsigned generate_floor(unsigned seed32, int level_index, level& out);

    // Hooks only (exposed for the metrics tool).
    void apply_hooks(unsigned seed32, int level_index, level& out);

    // Independent solver: stairs reachable without keys, all locks openable, all levers reachable before their bars.
    bool solvable(const level& lv, bool stairs_without_keys);

    // v2 level_bytes() / crc16()
    int level_bytes(const level& lv, uint8_t* out, int capacity);
    unsigned crc16(const uint8_t* data, int size);
}

#endif
