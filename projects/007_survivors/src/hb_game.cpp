// HEXBOUND block 1 run: Mossy Meadow 0:00-5:00, Slime King, level-up cards, shrines, elites, FX toggles.
// Numbers come from DISENO.md (sections cited next to each system).

#include "hb_game.h"
#include "hb_save.h"

#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_items_fx_lance.h"
#include "bn_sprite_items_fx_star.h"
#include "bn_sprite_palette_items_wizard_cinda_palette.h"
#include "bn_sprite_palette_items_wizard_morrow_palette.h"
#include "bn_sound_items.h"

#include "bn_array.h"
#include "bn_bg_palettes.h"
#include "bn_bg_tiles.h"
#include "bn_blending.h"
#include "bn_camera_ptr.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_handle.h"
#include "bn_music.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_tiles.h"
#include "bn_timer.h"
#include "bn_string.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_16x16_sprite_font.h"

#include "hb_art.h"
#include "hb_config.h"

// Mirrors of the game state for headless tests (gbarun.py --check). Global, non-static, volatile.
volatile int test_mode;          // 0 title, 1 play, 2 level up, 3 pause, 4 fx menu, 5 shrine pick, 6 dead, 7 won
volatile int test_time_s;
volatile int test_level;
volatile int test_hp;
volatile int test_enemies;
volatile int test_kills;
volatile int test_gold;
volatile int test_boss_hp;
volatile int test_cpu;           // last frame CPU usage, 1/10000 of a frame
volatile int test_cpu_max;       // worst frame since the run started (play mode only)
volatile int test_cpu_max_enemies;
volatile int test_projectiles;
volatile int test_gems;
volatile int test_frames;
volatile int test_nova;           // Hex Nova meter (0..nova_max)
volatile int test_novas_cast;
volatile int test_blinks;
volatile int test_wizard_dir;      // art::dir index the wizard is drawn facing
volatile int test_sounds;
volatile int test_last_sound_frame;
volatile int test_last_sound_id;
volatile int test_last_sound_speed;
volatile int test_cpu_nova_max;   // worst frame in which a Hex Nova was cast
// Section ticks (bn::timer, 1/ticks_per_frame of a frame) of the current and of the worst measured frame.
// 0 player, 1 director, 2 spark, 3 runes, 4 frost, 5 bolts, 6 enemies, 7 boss, 8 pickups, 9 particles,
// 10 rest of the logic, 11 engine (bn::core::update until the next frame starts), 12 ticks inside _kill_enemy,
// 13 kills in the frame, 14 ticks of _update_horde_frames (inside 6), 15 enemies alive.
volatile int test_prof[16];
volatile int test_prof_worst[16];     // worst frame without a Nova cast
volatile int test_prof_nova[16];      // worst Nova frame
volatile int test_cpu_max_plain;      // worst frame without a Nova cast
volatile int test_prof_worst_frame;    // test_frames of that frame
volatile int test_wolf_crouches;      // DASH warnings started
volatile int test_wolf_dashes;        // dashes started (each one after a 30 f crouch)
volatile int test_wolf_dash_hits;     // hits on the wizard by a dashing wolf
volatile int test_events;             // bit 0 treasure goblin, 1 split, 2 wolf pack, 3 goblin raid, 4 warchief,
                                      // 5 horn, 6 stunned, 7 double charge, 8 warchief dead, 9 treasure killed,
                                      // 10 boss rage, 11 stampede orcs, 12 elite pair, 13 cursed chest, 14 roots,
                                      // 15 thorn wall, 16 seeds, 17 treant dead, 18 hex moon, 19 stage clear
volatile int test_boss_phase;
volatile int test_dead_at;            // second of death (0 alive)
volatile int test_prof_ember;         // worst ticks of Ember Toss / Storm Chain updates
volatile int test_prof_chain;
volatile int test_boss_ttk;           // seconds from the boss entrance to its death
volatile int test_first_crouch_frame; // test_frames of the first wolf crouch (for a capture)
volatile int test_wall_onscreen = 99;   // G-52.1: fewest thorns inside 0..239 x 24..159 while the wall stands (24)
volatile int test_thorn_first_hurt = -1; // frames after the cast of the first thorn damage (>= 60)
volatile int test_thorn_hits_max;       // most thorn hits in one wall (bot: <= 2)
volatile int test_thorn_radius;        // current thorn wall radius (150 -> 110)
volatile int test_gold_drops;          // summary breakdown (E5.4)
volatile int test_gold_bosses;
volatile int test_nova_pulled;         // gems pulled by Novas with Lodestone >= 3
volatile int test_charge_offscreen;    // Warchief charges warned with the boss off screen (review: 0)
volatile int test_curse_elites;        // most elites alive during a cursed wave (<= 5)
volatile int test_level_at_600;      // level when the clock reaches 10:00 (Tiempos test)
volatile int test_evolved;           // G-55: evolution bits (1 Starfall, 2 Blizzard, 4 Ward of Ages)
volatile int test_meta_gold;         // gold banked by the last finished run
volatile int test_new_unlocks;       // unlock bits gained by the last finished run
volatile int test_lance_hits;
volatile int test_tiles_menu;         // peak with a menu open (cards, pause, summary)
volatile int test_tiles_menu_mode;
volatile int test_shots_max;          // G-56: most enemy projectiles at once (§C6 <= 12)
volatile int test_shooters_max;       // most bone archers alive (<= 6)
volatile int test_archer_warns;       // 24 f flashes started (each one before an arrow)
volatile int test_rise_min = 999;     // closest RISE crack to the wizard when it opened (>= 64)
volatile int test_rises;
volatile int test_skulls_broken;
volatile int test_wail_hits;
volatile int test_f2_events;          // 4 grave ring, 8 bone ring, 16 stampede, 32 elite pair, 64 collapse, 128 wail,
                                      // 256 intangible, 512 banshee teleport, 1024 skulls, 2048 raise, 4096 lich
                                      // teleport, 16384 haunted ghost, 32768 wraith blink, 1 murder, 2 ghost swarm
volatile int test_palette_refusals;   // spawns refused to keep a sprite palette free
volatile int test_tiles_used;         // peak bn::sprite_tiles::used_tiles_count() during play (§C6: <= 900)

// perf: the hottest per-frame loops run as ARM code from IWRAM (measured in G-45; see README "Rendimiento").
#define HB_HOT __attribute__((section(".iwram"), target("arm"), noinline))

namespace hb
{
namespace
{
    using bn::fixed;
    using bn::fixed_point;

    constexpr fixed diag = fixed(0.7071);

    [[nodiscard]] int to_int(fixed value)
    {
        return value.right_shift_integer();
    }

    [[nodiscard]] int abs_int(int value)
    {
        return value < 0 ? -value : value;
    }

    // 8-direction index (art::dir_*: 0 S, 1 SE, 2 E, 3 NE, 4 N, 5 NW, 6 W, 7 SW) of a delta, 22.5 degree sectors;
    // a zero delta keeps the current direction.
    [[nodiscard]] int dir_towards(int dx, int dy, int current)
    {
        int ax = abs_int(dx);
        int ay = abs_int(dy);
        int sx = dx > 0 ? 1 : dx < 0 ? -1 : 0;
        int sy = dy > 0 ? 1 : dy < 0 ? -1 : 0;

        if(ay * 12 < ax * 5)   // tan(22.5) ~= 5 / 12
        {
            sy = 0;
        }
        else if(ax * 12 < ay * 5)
        {
            sx = 0;
        }

        constexpr int8_t dirs[9] = { 5, 4, 3, 6, -1, 2, 7, 0, 1 };
        int dir = dirs[(sy + 1) * 3 + sx + 1];
        return dir < 0 ? current : dir;
    }

    // One 45 degree step from current towards target, the short way (180 degrees: always +1, via the next index).
    [[nodiscard]] int turn_step(int current, int target)
    {
        int diff = (target - current) & 7;

        if(diff == 0)
        {
            return current;
        }

        return (current + (diff <= 4 ? 1 : 7)) & 7;
    }

    // Unit vector (approximately) from an integer delta; returns {0, 0} for a zero delta.
    [[nodiscard]] fixed_point unit(int dx, int dy)
    {
        int ax = abs_int(dx);
        int ay = abs_int(dy);
        int big = ax > ay ? ax : ay;
        int small = ax > ay ? ay : ax;
        int len = big + (small * 3) / 8;

        if(len == 0)
        {
            return fixed_point();
        }

        return fixed_point(fixed(dx) / len, fixed(dy) / len);
    }

    [[nodiscard]] fixed_point angle_vector(int lut_angle)
    {
        int angle = lut_angle & 2047;
        return fixed_point(bn::lut_cos(angle), bn::lut_sin(angle));
    }

    [[nodiscard]] unsigned hash3(unsigned seed, int x, int y)
    {
        unsigned h = seed ^ (unsigned(x) * 73856093u) ^ (unsigned(y) * 19349663u);
        h ^= h >> 13;
        h *= 0x5bd1e995u;
        h ^= h >> 15;
        return h;
    }

    // ---------------------------------------------------------------------------------------------------------
    // Map: BG0 ground (64x64 cells, hardware wrap) + BG1 props written chunk by chunk (§3 "Mapa infinito").

    constexpr int map_cells = 64;
    constexpr int chunk_cells = 16;
    constexpr int chunk_px = chunk_cells * 8;
    constexpr int template_count = 9;
    constexpr int template_graves = 8;   // stage 2 "lápidas": 4 tombstones (the `log` piece), RISE graves (§C5)
    constexpr int template_max_props = 4;

    struct placed_prop
    {
        int8_t prop;    // -1 = none
        int8_t col;
        int8_t row;
    };

    // Chunk templates (cells inside a 16x16 chunk). Solid footprints stay inside columns/rows 1..14 and open
    // space between props is >= 32 px, so the wizard is never boxed in (R9).
    constexpr placed_prop chunk_templates[template_count][template_max_props] = {
        { { art::prop_flowers, 2, 3 }, { art::prop_tall_grass, 10, 9 }, { art::prop_rocks_small, 6, 12 }, { -1, 0, 0 } },
        { { art::prop_tree, 4, 2 }, { art::prop_flowers, 11, 11 }, { art::prop_mushrooms, 1, 11 }, { -1, 0, 0 } },
        { { art::prop_pine, 1, 1 }, { art::prop_pine, 9, 6 }, { art::prop_bush, 3, 11 }, { -1, 0, 0 } },
        { { art::prop_arch, 4, 3 }, { art::prop_pillar_stump, 1, 11 }, { art::prop_tall_grass, 11, 12 }, { -1, 0, 0 } },
        { { art::prop_ruin_wall, 4, 4 }, { art::prop_rune_stone, 2, 11 }, { art::prop_tall_grass, 10, 11 }, { -1, 0, 0 } },
        { { art::prop_rock, 2, 2 }, { art::prop_rock, 10, 9 }, { art::prop_flowers, 9, 2 }, { art::prop_stump, 3, 10 } },
        { { art::prop_log, 5, 3 }, { art::prop_mushrooms, 10, 10 }, { art::prop_crystal, 2, 10 }, { art::prop_pond, 10, 1 } },
        { { art::prop_pillar, 3, 2 }, { art::prop_pillar, 9, 4 }, { art::prop_bush_flowers, 5, 11 }, { -1, 0, 0 } },
        { { art::prop_log, 2, 2 }, { art::prop_log, 10, 3 }, { art::prop_log, 3, 10 }, { art::prop_log, 10, 10 } }
    };

    // §C5 stage 2 weights over 20 draws: empty 4, grove 2, pines 1, ruin 2, walls 2, rocks 1, mushrooms 1,
    // obelisk 2, tombstones 5 (25 %).
    constexpr int8_t graveyard_draws[20] = { 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 8, 8, 8, 8, 8 };

    struct world_map
    {
        alignas(int) bn::regular_bg_map_cell prop_cells[map_cells * map_cells];
        bn::regular_bg_map_cell templates[template_count * 2][chunk_cells * chunk_cells];
        uint8_t solids[template_count * 2][chunk_cells * chunk_cells];
        int slot_x[16];
        int slot_y[16];
        unsigned seed = 0;

        static void mark_solid(uint8_t* solid, int col, int row, int w, int h, bool flip)
        {
            for(int y = row; y < row + h; ++y)
            {
                for(int x = col; x < col + w; ++x)
                {
                    int ox = flip ? chunk_cells - 1 - x : x;

                    if(ox >= 0 && ox < chunk_cells && y >= 0 && y < chunk_cells)
                    {
                        solid[y * chunk_cells + ox] = 1;
                    }
                }
            }
        }

        int stage = 0;

        void init(unsigned map_seed, int map_stage)
        {
            seed = map_seed;
            stage = map_stage;

            for(unsigned& key : template_cache_key)
            {
                key = 0x80008000u;
            }
            bn::memory::clear(prop_cells);

            for(int t = 0; t < template_count * 2; ++t)
            {
                for(int c = 0; c < chunk_cells * chunk_cells; ++c)
                {
                    templates[t][c] = 0;
                    solids[t][c] = 0;
                }
            }

            for(int t = 0; t < template_count; ++t)
            {
                for(int flip = 0; flip < 2; ++flip)
                {
                    bn::regular_bg_map_cell* out = templates[t * 2 + flip];
                    uint8_t* solid = solids[t * 2 + flip];

                    for(const placed_prop& pp : chunk_templates[t])
                    {
                        if(pp.prop < 0)
                        {
                            continue;
                        }

                        const art::prop_def& def = art::props[pp.prop];
                        int w = def.w / 8;
                        int h = def.h / 8;

                        for(int j = 0; j < h; ++j)
                        {
                            for(int i = 0; i < w; ++i)
                            {
                                int col = pp.col + i;
                                int row = pp.row + j;

                                if(col >= chunk_cells || row >= chunk_cells)
                                {
                                    continue;
                                }

                                int src_i = flip ? w - 1 - i : i;
                                int tile = (def.y / 8 + j) * art::props_tiles_columns + def.x / 8 + src_i;
                                int out_col = flip ? chunk_cells - 1 - col : col;
                                bn::regular_bg_map_cell_info info;
                                info.set_tile_index(tile);
                                info.set_horizontal_flip(flip != 0);
                                out[row * chunk_cells + out_col] = info.cell();
                            }
                        }

                        if(def.sw)
                        {
                            mark_solid(solid, pp.col + def.sx, pp.row + def.sy, def.sw, def.sh, flip);
                        }

                        if(pp.prop == art::prop_arch)
                        {
                            mark_solid(solid, pp.col + 6, pp.row + 4, 2, 4, flip);   // right leg
                        }
                    }
                }
            }

            for(int i = 0; i < 16; ++i)
            {
                slot_x[i] = -100000;
                slot_y[i] = -100000;
            }
        }

        [[nodiscard]] int chunk_template(int kx, int ky) const
        {
            // Safe clearing: the 2x2 chunks around the start have no props (§3: nothing within 48 px of start).
            if(kx >= -1 && kx <= 0 && ky >= -1 && ky <= 0)
            {
                return -1;
            }

            // perf (G-52): every walking monster asks 2-3 times per frame; a 16-entry cache skips the hash and the
            // division (the template of a chunk never changes).
            int slot = (kx & 3) | ((ky & 3) << 2);
            unsigned key = (unsigned(kx) << 16) ^ (unsigned(ky) & 0xFFFFu);

            if(template_cache_key[slot] == key)
            {
                return template_cache_value[slot];
            }

            unsigned h = hash3(seed, kx, ky);
            int t;

            if(stage == 1)
            {
                t = graveyard_draws[(h >> 4) % 20];
            }
            else
            {
                unsigned pick = (h >> 4) % 10;
                t = pick < 3 ? 0 : int(pick) - 2;
            }
            int result = t * 2 + int(h & 1);
            template_cache_key[slot] = key;
            template_cache_value[slot] = int8_t(result);
            return result;
        }

        mutable unsigned template_cache_key[16] = { 0x80008000u, 0x80008000u, 0x80008000u, 0x80008000u,
                                                    0x80008000u, 0x80008000u, 0x80008000u, 0x80008000u,
                                                    0x80008000u, 0x80008000u, 0x80008000u, 0x80008000u,
                                                    0x80008000u, 0x80008000u, 0x80008000u, 0x80008000u };
        mutable int8_t template_cache_value[16] = {};

        [[nodiscard]] bool solid_at(int wx, int wy) const
        {
            int tpl = chunk_template(wx >> 7, wy >> 7);

            if(tpl < 0)
            {
                return false;
            }

            return solids[tpl][((wy >> 3) & 15) * chunk_cells + ((wx >> 3) & 15)];
        }

        // Writes the chunks around the camera into the reused cells; returns true if anything changed.
        bool update(int cam_x, int cam_y)
        {
            bool changed = false;
            int kx0 = (cam_x - 120 - 48) >> 7;
            int kx1 = (cam_x + 120 + 48) >> 7;
            int ky0 = (cam_y - 80 - 48) >> 7;
            int ky1 = (cam_y + 80 + 48) >> 7;

            for(int ky = ky0; ky <= ky1; ++ky)
            {
                for(int kx = kx0; kx <= kx1; ++kx)
                {
                    int sx = (kx + 2) & 3;
                    int sy = (ky + 2) & 3;
                    int slot = sy * 4 + sx;

                    if(slot_x[slot] == kx && slot_y[slot] == ky)
                    {
                        continue;
                    }

                    slot_x[slot] = kx;
                    slot_y[slot] = ky;
                    changed = true;

                    int tpl = chunk_template(kx, ky);

                    // A 64x64 regular BG map is stored as four 32x32 screen blocks (hardware layout, see
                    // bn::regular_bg_map_item::cell_index), not as 64-cell rows: writing plain rows interleaved the
                    // prop rows with the next block ("cut" props in game).
                    for(int row = 0; row < chunk_cells; ++row)
                    {
                        int map_y = sy * chunk_cells + row;
                        int map_x = sx * chunk_cells;
                        int block = (map_y / 32) * (map_cells / 32) + map_x / 32;
                        bn::regular_bg_map_cell* out = prop_cells + block * 1024 + (map_y % 32) * 32 + (map_x % 32);

                        for(int col = 0; col < chunk_cells; ++col)
                        {
                            out[col] = tpl >= 0 ? templates[tpl][row * chunk_cells + col] : bn::regular_bg_map_cell(0);
                        }
                    }
                }
            }

            return changed;
        }
    };

    // ---------------------------------------------------------------------------------------------------------
    // Content tables.

    enum enemy_type : uint8_t
    {
        type_slime,
        type_bat,
        type_skeleton,
        type_big_slime,
        type_goblin,
        type_wolf,
        type_orc,
        type_treasure,   // Treasure Goblin: goblin art with a gold palette (§C2), FLEE
        // Stage 2 Grim Graveyard (DISENO_CONTENIDO §C2): F1 tiles with another palette, plus ghost / skeleton_big.
        type_ghoul,
        type_crow,
        type_skeleton2,   // stage 2 skeleton (20 HP, 0.6, 7)
        type_walker,      // Grave Walker: RISE
        type_ghost,
        type_archer,      // Bone Archer: SHOOT
        type_wraith,
        type_brute,
        type_knight,
        type_count
    };

    // Behaviour archetypes (DISENO_CONTENIDO §C1).
    enum enemy_kind : uint8_t
    {
        kind_chase,
        kind_hop,
        kind_wave,
        kind_dash,
        kind_tank,
        kind_flee,
        kind_rise,    // CHASE that enters through a 60 f grave crack (§C1 RISE)
        kind_shoot,   // keeps 72-96 px, 24 f flash, one arrow (§C1 SHOOT)
        kind_phase    // slow WAVE, 60 f of every 240 intangible (§C1 PHASE)
    };

    struct enemy_def
    {
        const art::monster_art* look;
        int hp;              // hp_base; scaled per minute (§C1)
        fixed speed;
        int damage;
        bool flying;
        uint8_t kind;        // enemy_kind
        uint8_t half;        // half size of the body box (px)
        uint8_t xp;          // gem value
        uint8_t xp5_pct;     // chance (%) that the gem is worth 5 instead
        uint8_t splits;      // SPLIT: children (slimes) on death
        bool big;            // 32x32: counts for the <= 10 big cap (§C1)
        const bn::sprite_palette_item* pal = nullptr;   // palette variant (§C2 "Arte (paleta)"); null = the art's
        uint8_t stages = 1;  // bit per stage whose horde uses the type (its art gets a tile block)
        bool blinks = false; // BLINK (§C1): every 300 f reappears 56 px from the wizard after a 45 f shadow
    };

    // §7, DISENO_CONTENIDO §C2 (stage 1)
    const enemy_def enemy_defs[type_count] = {
        { &art::slime, 6, fixed(0.35), 4, false, kind_hop, 7, 1, 0, 0, false },      // contact 5 -> 4 (§19.1)
        { &art::bat, 3, fixed(1.1), 4, true, kind_wave, 7, 1, 0, 0, false },
        { &art::skeleton, 14, fixed(0.55), 6, false, kind_chase, 7, 1, 20, 0, false },   // damage 8 -> 6 (§19.2)
        { &art::big_slime, 30, fixed(0.3), 8, false, kind_hop, 11, 5, 0, 2, true },
        { &art::goblin, 10, fixed(0.85), 6, false, kind_chase, 9, 1, 0, 0, true },
        { &art::wolf, 18, fixed(0.7), 8, false, kind_dash, 9, 5, 0, 0, false },
        { &art::orc, 60, fixed(0.45), 12, false, kind_tank, 11, 5, 0, 0, true },
        { &art::goblin, 40, fixed(1.3), 0, false, kind_flee, 9, 0, 0, 0, true, nullptr, 3 },
        { &art::goblin, 16, fixed(0.75), 7, false, kind_chase, 9, 1, 0, 0, true, &art::rotten_palette, 2 },
        { &art::bat, 5, fixed(1.25), 5, true, kind_wave, 7, 1, 0, 0, false, &art::crow_palette, 2 },
        { &art::skeleton, 20, fixed(0.6), 7, false, kind_chase, 7, 1, 20, 0, false, nullptr, 2 },
        { &art::skeleton, 28, fixed(0.4), 9, false, kind_rise, 7, 5, 0, 0, false, &art::grey_flesh_palette, 2 },
        { &art::ghost, 12, fixed(0.6), 6, true, kind_phase, 6, 1, 0, 0, false, nullptr, 2 },
        { &art::skeleton, 16, fixed(0.5), 5, false, kind_shoot, 7, 5, 0, 0, false, &art::red_bone_palette, 2 },
        { &art::ghost, 30, fixed(0.7), 10, true, kind_phase, 6, 5, 0, 0, false, &art::violet_palette, 2, true },
        { &art::orc, 90, fixed(0.45), 14, false, kind_tank, 11, 5, 0, 0, true, &art::rotten_palette, 2 },
        { &art::skeleton_big, 60, fixed(0.55), 12, false, kind_tank, 11, 5, 0, 0, true, nullptr, 2 },
    };

    [[nodiscard]] const bn::sprite_palette_item& def_palette(int type)
    {
        const enemy_def& def = enemy_defs[type];
        return def.pal ? *def.pal : def.look->item->palette_item();
    }

    // HP scaling per minute (§C1): round(hp_base * (1 + 0.07 * minute)); bosses exempt.
    [[nodiscard]] constexpr int scaled_hp(int hp_base, int minute)
    {
        return (hp_base * (100 + 7 * minute) + 50) / 100;
    }

    // Shared animation (§C6): one tile block per drawn direction for each monster art (not per type: palette
    // variants share it), reloaded with the current frame at every animation step. Every instance of an art
    // animates in sync. Slots are assigned in enemy_defs order.
    constexpr int max_horde_arts = 8;

    enum upgrade_id : uint8_t
    {
        up_spark,
        up_rune,
        up_frost,
        up_ember,
        up_chain,
        up_lance,       // G-55
        up_boots,       // first passive
        up_heart,
        up_tome,
        up_lodestone,
        up_lens,        // G-55
        up_ward,        // G-55
        up_heal,
        up_gold,
        up_count
    };

    constexpr int weapon_or_passive_count = up_heal;
    constexpr int max_slots = 4;   // §C3: 4 weapon slots, 4 passive slots
    constexpr int max_upgrade_level = 5;

    struct upgrade_text
    {
        const char* name;
        const bn::sprite_item* icon_item;
        int icon;
        const char* desc[max_upgrade_level];
    };

    // §9
    const upgrade_text upgrade_texts[up_count] = {
        { "Spark Bolt", &art::icons, art::icon_spark_bolt, { "Auto", "+1 bolt", "+3 dmg", "+1 bolt", "Pierce" } },
        { "Rune Orbit", &art::icons, art::icon_rune_orbit, { "2 runes", "+1 rune", "Wider", "+1 rune", "Always" } },
        { "Frost Ring", &art::icons, art::icon_frost_ring, { "Slows", "+3 dmg", "Bigger", "Faster", "Freeze" } },
        { "Ember Toss", &art::icons, art::icon_ember_toss, { "Fire", "+1 ball", "Wider", "+6 dmg", "Burn+" } },
        { "Storm Chain", &art::icons, art::icon_storm_chain, { "Zap", "+1 jump", "+4 dmg", "+2 jump", "Faster" } },
        { "Moon Lance", &art::icons, 10, { "Beam", "+5 dmg", "Wider", "Behind", "Faster" } },
        { "Swift Boots", &art::icons, art::icon_swift_boots, { "+8% SPD", "+8% SPD", "+8% SPD", "+8% SPD", "+8% SPD" } },
        { "Heart Amulet", &art::icons, art::icon_heart_amulet, { "+20 HP", "+20 HP", "+20 HP", "+20 HP", "+20 HP" } },
        { "Spell Tome", &art::icons, art::icon_spell_tome, { "-6% CD", "-6% CD", "-6% CD", "-6% CD", "-6% CD" } },
        { "Lodestone", &art::icons, art::icon_magnet, { "Reach+", "Reach+", "Reach+", "Reach+", "Reach+" } },
        { "Lens of Reach", &art::icons, 11, { "+10% area", "+10% area", "+10% area", "+10% area", "+10% area" } },
        { "Ward Stone", &art::icons, 12, { "-6% hurt", "-6% hurt", "-6% hurt", "-6% hurt", "+10 f safe" } },
        { "Heal", &art::heart, 0, { "Heal 30", "", "", "", "" } },
        { "Gold", &art::coin, 0, { "+10 G", "", "", "", "" } },
    };

    // Evolutions of block 4 (§C3): weapon Nv5 + its passive -> the next boss or elite chest transforms it.
    enum evolution_bit : uint8_t
    {
        evo_starfall = 1,       // Spark Bolt + Spell Tome
        evo_blizzard = 2,       // Frost Ring + Lens of Reach
        evo_ward_of_ages = 4    // Rune Orbit + Ward Stone
    };

    enum blessing_id : uint8_t
    {
        bless_fury,
        bless_haste,
        bless_magnet,
        bless_mend,
        bless_count
    };

    // §4 2a
    const upgrade_text blessing_texts[bless_count] = {
        { "Fury", &art::icons, art::icon_star, { "DMG+30%", "60 s", "", "", "" } },
        { "Haste", &art::icons, art::icon_swift_boots, { "SPD+30%", "60 s", "", "", "" } },
        { "Magnet", &art::icons, art::icon_magnet, { "Get all", "60 s", "", "", "" } },
        { "Mend", &art::heart, 0, { "Heal 40", "now", "", "", "" } },
    };

    // ---------------------------------------------------------------------------------------------------------
    // Actors.

    enum enemy_mode : uint8_t
    {
        mode_chase,
        mode_swarm,     // Bat Swarm: straight line, despawns when done
        mode_ring       // Bone Ring: closes in at 0.4 px/f
    };

    struct enemy
    {
        bn::sprite_ptr sprite;
        bn::optional<bn::sprite_ptr> aura;   // elites only (§4 2b)
        fixed_point pos;
        fixed_point vel;
        fixed_point knock_vel;
        int hp = 0;
        int life = 0;
        uint16_t uid = 0;
        uint8_t type = 0;
        uint8_t elite = 0;      // 0 none, 1 swift, 2 shielded
        uint8_t mode = 0;
        uint8_t flash = 0;
        uint8_t knock = 0;
        uint8_t shield = 0;
        uint8_t rune_cd = 0;
        uint8_t frost_pulse = 0;
        uint8_t dir = 0;        // facing (art::dir_*), only for monsters with walk_dirs > 1
        uint8_t nova = 0;       // 1: caught by a Hex Nova, hit in a later frame (perf: spread over frames)
        int16_t slow = 0;
        int16_t freeze = 0;
        uint8_t state = 0;      // DASH: 0 walk, 1 crouch (warning), 2 dash
        uint8_t timer = 0;      // frames left in the DASH state
        uint8_t cd = 0;         // DASH recharge (frames / 2)
        uint8_t safe = 0;       // SPLIT children: frames without dealing or taking damage
        bool small = false;     // Splitting copy: does not split again
        bool tiny = false;      // Splitting elite copy: 50 % logical size
        bool flank = false;     // goblins: +30 or -30 degrees while far (§C1 CHASE flank)
        bool tinted = false;    // elite palette held (false: out of sprite palettes, the type's palette)
        bool ghostly = false;   // PHASE: intangible right now (50 % alpha)

        explicit enemy(bn::sprite_ptr&& s) :
            sprite(bn::move(s))
        {
        }
    };

    struct bolt
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        fixed_point vel;
        int life = 0;
        int damage = 0;
        int pierce = 0;
        uint16_t last_hit[3] = {};

        explicit bolt(bn::sprite_ptr&& s) :
            sprite(bn::move(s))
        {
        }
    };

    enum pickup_kind : uint8_t
    {
        pickup_gem,
        pickup_coin,
        pickup_chest,
        pickup_heart,
        pickup_cursed   // cursed chest (§C5): 2 upgrades, then 20 s of elites
    };

    enum gold_source : uint8_t
    {
        gold_drops,      // coins from monsters and elites
        gold_treasure,   // Treasure Goblin
        gold_bosses,
        gold_cards,      // "+10 gold" card
        gold_source_count
    };

    struct split_request
    {
        fixed_point pos;
        uint8_t type;
        uint8_t count;
        int hp;       // 0: the type's scaled HP
    };

    struct queued_spawn
    {
        fixed_point pos;
        fixed_point vel;
        int16_t life;
        uint8_t type;
        uint8_t mode;
        uint8_t elite_den;   // elite chance 1/den (0 none)
    };

    enum swarm_kind : uint8_t
    {
        swarm_bats,
        swarm_wolves,
        swarm_goblins,
        swarm_stampede,  // 14:00: wolves, orcs, wolves (stage 2: knights, brutes, knights)
        swarm_crows,     // stage 2 Murder of Crows: 2 LINE of 10 in X
        swarm_ghosts     // stage 2 Ghost Swarm: 16 ghosts
    };

    // Stage 2 warnings on the ground (§C1 RISE crack 60 f, BLINK shadow 45 f, boss teleport destination).
    enum mark_kind : uint8_t
    {
        mark_rise,
        mark_blink,
        mark_boss
    };

    struct mark
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        int16_t timer;
        uint8_t kind;
        uint8_t type;    // RISE: monster that comes out
        uint16_t uid;    // BLINK: the wraith
    };

    // Enemy projectiles (§C6: <= 12): archer arrows, Colossus bones (thrown or circling), Lich skulls.
    enum shot_kind : uint8_t
    {
        shot_arrow,
        shot_bone,
        shot_skull,
        shot_orbit
    };

    struct shot
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        fixed_point vel;
        int16_t life = 0;
        int16_t angle = 0;
        uint8_t kind = 0;
        uint8_t damage = 0;

        explicit shot(bn::sprite_ptr&& s) :
            sprite(bn::move(s))
        {
        }
    };

    struct pickup
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        fixed speed;
        int value = 0;
        uint8_t kind = 0;
        uint8_t source = gold_drops;
        bool flying = false;

        explicit pickup(bn::sprite_ptr&& s) :
            sprite(bn::move(s))
        {
        }
    };

    struct particle
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        fixed_point vel;
        int life = 0;
        int age = 0;
        const bn::sprite_item* item = nullptr;
        art::anim frames = { 0, 1, 1 };

        explicit particle(bn::sprite_ptr&& s) :
            sprite(bn::move(s))
        {
        }
    };

    struct damage_number
    {
        bn::vector<bn::sprite_ptr, 2> sprites;
        int life = 0;
    };

    enum class game_mode
    {
        play = 1,
        level_up = 2,
        pause = 3,
        fx_menu = 4,
        shrine_pick = 5,
        dead = 6,
        won = 7
    };

    // ---------------------------------------------------------------------------------------------------------

    class game
    {

    public:
        game(unsigned seed, fx_settings& fx, const run_setup& setup) :
            _fx(fx),
            _camera(bn::camera_ptr::create(0, 0)),
            _text(common::variable_8x8_sprite_font),
            _big_text(common::variable_16x16_sprite_font),
            _wizard(art::wizard.create_sprite(0, art::wizard_y_offset, art::wizard_idle[0].first)),
            _wizard_shadow(art::shadow_small.create_sprite(0, 5)),
            _hp_bar(art::hp_bar.create_sprite(0, 12, art::hp_bar_frames))
        {
            _random = bn::random();

            for(unsigned i = 0; i < (seed & 255); ++i)
            {
                _random.update();
            }

            _stage = setup.stage == 1 ? 1 : 0;   // stage 3 is block 7
            _map.init(seed * 2654435761u + 1, _stage);
            _grid_clear();
            _build_palettes();
            _white_pal = _white_palette().create_palette();
            // Characters (§C4): Wendel's tiles with another palette.
            _setup = setup;
            _wizard_pal = (setup.character == char_cinda ? bn::sprite_palette_items::wizard_cinda_palette :
                           setup.character == char_morrow ? bn::sprite_palette_items::wizard_morrow_palette :
                                                            art::wizard.palette_item()).create_palette();
            _wizard.set_palette(*_wizard_pal);
            const save_data& meta = save();

            for(int k = 0; k < shop_count; ++k)
            {
                _shop[k] = meta.shop_ranks[k];
            }

            _unlocks = meta.unlock_bits;

            for(int t = 0; t < type_count; ++t)
            {
                if(! (enemy_defs[t].stages & (1 << _stage)))
                {
                    continue;   // §C6: only the stage's horde arts hold tiles (<= 8 arts, <= 448 tiles)
                }

                const art::monster_art& look = *enemy_defs[t].look;
                int slot = -1;

                for(int o = 0; o < _horde_art_count; ++o)
                {
                    if(_horde_looks[o] == &look)
                    {
                        slot = o;
                    }
                }

                if(slot < 0)
                {
                    slot = _horde_art_count++;
                    _horde_looks[slot] = &look;
                    _horde_frame[slot] = 0;

                    // A block of our own (nothing else references walk frames), retargeted with set_tiles_ref at every animation step.
                    for(int d = 0; d < look.walk_dirs; ++d)
                    {
                        _horde_tiles[slot].push_back(bn::sprite_tiles_ptr::create(look.item->tiles_item(),
                                                                                      look.move[d].first));
                    }
                }

                _type_slot[t] = uint8_t(slot);
            }

            // Props first with tile offset disabled so the dynamic cells index the tile set directly.
            bn::bg_tiles::set_allow_offset(false);
            _map.update(0, 0);
            bn::regular_bg_item props_item(art::props_tiles, _stage ? art::props_graveyard_palette : art::props_palette,
                                           bn::regular_bg_map_item(_map.prop_cells[0], bn::size(map_cells, map_cells)));
            _props.emplace(props_item.create_bg(0, 0));
            _props->set_priority(2);
            _props->set_camera(_camera);
            bn::bg_tiles::set_allow_offset(true);

            _ground.emplace(art::ground.create_bg(0, 0));
            _ground->set_priority(3);
            _ground->set_camera(_camera);

            if(_stage == 1)
            {
                _ground->set_palette(art::ground_graveyard_palette);
            }

            _text.set_bg_priority(0);
            _big_text.set_bg_priority(0);

            _wizard.set_camera(_camera);
            _wizard.set_bg_priority(2);
            _wizard.set_z_order(5);
            _wizard_shadow.set_camera(_camera);
            _wizard_shadow.set_bg_priority(2);
            _wizard_shadow.set_z_order(30);
            _wizard_shadow.set_blending_enabled(true);
            _hp_bar.set_camera(_camera);
            _hp_bar.set_bg_priority(2);
            _hp_bar.set_z_order(4);
            bn::blending::set_transparency_alpha(fixed(0.5));

            // XP bar: 7 pieces from x = 4 plus end caps; clock plaque under the timer (art/LÉEME.md).
            for(int i = 0; i < art::xp_pieces; ++i)
            {
                bn::sprite_ptr bar = art::xp_bar.create_sprite(-100 + i * 32, -76, 0);
                bar.set_bg_priority(0);
                _xp_bar.push_back(bn::move(bar));
            }

            for(int i = 0; i < 2; ++i)
            {
                bn::sprite_ptr cap = art::xp_cap.create_sprite(i ? 112 : -115, -76, i);
                cap.set_bg_priority(0);
                cap.set_z_order(-1);
                _hud_frame.push_back(bn::move(cap));
                bn::sprite_ptr plaque = art::plaque.create_sprite(i ? 16 : -16, -63, i);
                plaque.set_bg_priority(0);
                plaque.set_z_order(1);
                _hud_frame.push_back(bn::move(plaque));
            }

            // Starting weapon per character; shop Vitality / Charged / Reroll / Revival (§C4).
            _levels[_setup.character == char_cinda ? up_ember : _setup.character == char_morrow ? up_chain : up_spark] = 1;
            _player_max_hp = player_hp + 10 * _shop[shop_vitality] - (_setup.character == char_cinda ? 10 : 0);
            _player_hp = _player_max_hp;
            _rerolls = _shop[shop_reroll];
            _revivals = _shop[shop_revival];

            if(_shop[shop_charged])
            {
                _nova = nova_max / 2;
            }
            _facing = fixed_point(1, 0);

            // Treasure Goblin: one 50 % window per act, 0:30-3:30 into the act (§C5), never near a boss.
            // Treasure Goblin: one 50 % window per act, 0:30-3:30 into the act; cursed chest: 60 %, 1:00-3:30 (§C5).
            for(int act = 0; act < 3; ++act)
            {
                // Review block 2: never within 20 s of an event (act 2: 6:25-6:50 or 7:20-7:50; act 3: 11:10-11:40).
                int at = act == 0 ? 30 + _random.get_int(61) :
                         act == 1 ? (_random.get_int(2) ? 385 + _random.get_int(26) : 440 + _random.get_int(31)) :
                                    670 + _random.get_int(31);
                _treasure_at[act] = _random.get_int(2) == 0 ? at * fps : -1;
                at = (act * 300 + 60 + _random.get_int(151)) * fps;
                _cursed_at[act] = _random.get_int(10) < 6 ? at : -1;
            }

            _stage_music().play(fixed(art::music_volume_pct) / 100);
            // perf: the Storm Chain sprites are created once, hidden (creating 16 in one frame cost ~60 %).
            for(int k = 0; k < chain_sprites; ++k)
            {
                bn::sprite_ptr c = _world_sprite(art::fx_chain, fixed_point(), 0, 2);
                c.set_visible(false);
                _chain_sprites.push_back(bn::move(c));
            }

            _show_hint("D-PAD: MOVE", 3 * fps);

            if(_stage == 1)
            {
                _show_banner("GRIM GRAVEYARD", art::text_gold, 2 * fps);   // E3 0:00 stage title 2 s
            }

            _refresh_hud(true);
            _apply_fx_static();
        }

        ~game()
        {
            bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
            bn::sprite_palettes::set_fade(bn::color(0, 0, 0), 0);
            bn::sprites_mosaic::set_stretch(0);
        }

        [[nodiscard]] run_end run()
        {
            while(true)
            {
                test_frames = test_frames + 1;

                switch(_mode)
                {
                case game_mode::play:
                    _update_play();
                    break;

                case game_mode::level_up:
                case game_mode::shrine_pick:
                    _update_cards();
                    break;

                case game_mode::pause:
                    if(_update_pause())
                    {
                        _commit_meta();   // quitting keeps the gold (R10)
                        return run_end::title;
                    }
                    break;

                case game_mode::fx_menu:
                    _update_fx_menu();
                    break;

                case game_mode::dead:
                case game_mode::won:
                    if(_mode == game_mode::dead)
                    {
                        ++_death_anim;
                        _wizard.set_tiles(art::wizard.tiles_item(), art::wizard_death.frame(_death_anim, false));
                    }

                    if(_end_timer > 0)
                    {
                        --_end_timer;

                        if(_end_timer == 0)
                        {
                            _show_summary();
                        }
                    }
                    else if(_summary_lock > 0)
                    {
                        if(--_summary_lock == 0)
                        {
                            _text.set_center_alignment();
                            _text.generate(0, 66, "A: RETRY    B: MENU", _menu);
                        }
                    }
                    else if(bn::keypad::a_pressed())
                    {
                        return run_end::retry;
                    }
                    else if(bn::keypad::b_pressed())
                    {
                        return run_end::title;
                    }
                    break;

                default:
                    break;
                }

                _update_screen_fade();
                test_mode = int(_mode);
                test_tiles_used = bn::max(int(test_tiles_used), bn::sprite_tiles::used_tiles_count());   // cards too

                if(_mode != game_mode::play)
                {
                    test_tiles_menu = bn::max(int(test_tiles_menu), bn::sprite_tiles::used_tiles_count());
                    test_tiles_menu_mode = test_tiles_menu == bn::sprite_tiles::used_tiles_count() ? int(_mode) :
                                                                                                    test_tiles_menu_mode;
                }
                _tick_sounds();
                bool measured = _mode == game_mode::play && _play_streak >= 2;
                _play_streak = _mode == game_mode::play ? _play_streak + 1 : 0;
                bool nova_frame = _nova_cast_frame;
                _nova_cast_frame = false;
                _prof_timer.restart();
                bn::core::update();
                test_prof[11] = _prof_timer.elapsed_ticks();

                // Only frames that were play -> play are measured (menus build text, the first frame loads BGs).
                int cpu = (bn::core::last_cpu_usage() * 10000).right_shift_integer();
                test_cpu = cpu;

                if(measured && _mode == game_mode::play && cpu > test_cpu_max)
                {
                    test_cpu_max = cpu;
                    test_cpu_max_enemies = _enemies.size();
                }

                if(measured && _mode == game_mode::play)
                {
                    if(nova_frame && cpu > test_cpu_nova_max)
                    {
                        test_cpu_nova_max = cpu;

                        for(int k = 0; k < 16; ++k)
                        {
                            test_prof_nova[k] = test_prof[k];
                        }
                    }
                    else if(! nova_frame && cpu > test_cpu_max_plain)
                    {
                        test_cpu_max_plain = cpu;
                        test_prof_worst_frame = test_frames;

                        for(int k = 0; k < 16; ++k)
                        {
                            test_prof_worst[k] = test_prof[k];
                        }
                    }
                }

                // Per-frame kill accumulators are cleared after the worst frame copied them (they always read 0).
                test_prof[12] = 0;
                test_prof[13] = 0;
            }
        }

    private:
        fx_settings& _fx;
        bn::random _random;
        world_map _map;
        bn::camera_ptr _camera;
        bn::optional<bn::regular_bg_ptr> _ground;
        bn::optional<bn::regular_bg_ptr> _props;
        bn::sprite_text_generator _text;
        bn::sprite_text_generator _big_text;

        bn::sprite_ptr _wizard;
        bn::sprite_ptr _wizard_shadow;
        bn::sprite_ptr _hp_bar;
        bn::vector<bn::sprite_ptr, art::xp_pieces> _xp_bar;
        bn::vector<bn::sprite_ptr, 4> _hud_frame;
        bn::optional<bn::sprite_ptr> _burst;
        int _burst_t = 0;
        bn::vector<bn::sprite_ptr, 8> _boss_bar;   // 6 pieces + 2 phase notches
        bn::vector<bn::sprite_ptr, 4> _hud_level;
        bn::vector<bn::sprite_ptr, 4> _hud_clock;
        bn::vector<bn::sprite_ptr, 4> _hud_gold;
        bn::optional<bn::sprite_ptr> _hud_blessing;
        bn::vector<bn::sprite_ptr, 8> _hint;
        bn::vector<bn::sprite_ptr, 8> _world_hint;
        bn::vector<bn::sprite_ptr, 96> _menu;
        bn::vector<bn::sprite_ptr, 6> _warnings;

        bn::vector<enemy, max_enemies> _enemies;
        bn::vector<bolt, max_bolts> _bolts;
        bn::vector<bn::sprite_ptr, max_bolts> _bolt_pool;   // hidden spark sprites ready for reuse
        bn::vector<bn::sprite_ptr, max_runes> _runes;
        bn::vector<bn::sprite_ptr, frost_shards> _frost;
        bn::vector<pickup, max_gems + 4> _pickups;
        bn::vector<particle, max_particles> _particles;
        bn::vector<damage_number, max_damage_numbers> _numbers;

        bn::array<bn::color, 16> _white_colors;
        bn::array<bn::color, 16> _gold_arrow_colors;   // G-55: Treasure Goblin edge arrow, gold ramp by brightness
        bn::array<bn::color, 16> _elite_colors[type_count][4];   // swift, shielded, splitting, treasure gold
        bn::array<bn::color, 16> _boss_white_colors;
        bn::array<bn::color, 16> _wolf_red_colors;
        bn::optional<bn::sprite_palette_ptr> _wolf_red_pal;
        int _crouching = 0;        // wolves crouching in the previous frame (max 3 at once)
        int _crouching_now = 0;

        // Hot-path caches (perf): enemies swap palettes and animation frames by handle, without a VRAM lookup.
        bn::optional<bn::sprite_palette_ptr> _white_pal;
        bn::optional<bn::sprite_palette_ptr> _type_pal[type_count];
        bn::vector<bn::sprite_tiles_ptr, art::drawn_dirs> _horde_tiles[max_horde_arts];
        const art::monster_art* _horde_looks[max_horde_arts] = {};
        int _horde_frame[max_horde_arts] = {};
        int _horde_next = 0;   // first art looked at this frame (round robin)
        int _horde_art_count = 0;
        uint8_t _type_slot[type_count] = {};
        int _type_alive[type_count] = {};

        // Player
        fixed_point _pos;
        fixed_point _vel;
        fixed_point _facing;
        int _player_hp = 0;
        int _player_max_hp = 0;
        int _invulnerable = 0;
        int _grace = 0;              // frames without contact damage after closing the cards (§19.3), no flash
        bool _wizard_white = false;
        int _summary_lock = 0;       // frames the summary ignores A/B (§19.5)
        bool _cards_shown = false;
        int _card_blink = 0;
        int _sel_card_sprite = -1;
        bn::optional<bn::sprite_palette_ptr> _wizard_pal;
        int _level = 1;
        int _xp = 0;
        int _pending_levels = 0;
        int _free_upgrades = 0;

        // Meta (G-55): setup, shop ranks and unlocks read at the start of the run.
        run_setup _setup;
        int _shop[shop_count] = {};
        uint32_t _unlocks = 0;
        int _rerolls = 0;
        int _revivals = 0;
        int _xp_frac = 0;           // Growth: XP hundredths
        int _recovery_t = 0;
        int _evolved = 0;           // evolution_bit
        int _evolve_flash = 0;
        bool _meta_done = false;
        int _meta_earned = 0;
        int _new_best = 0;          // 1 time, 2 kills, 4 level
        uint32_t _new_unlocks = 0;
        int _star_cd = 30;
        int _lance_cd = 60;
        int _lance_t = -1;
        bn::vector<bn::sprite_ptr, 8> _lance_sprites;
        bn::optional<bn::sprite_affine_mat_ptr> _lance_mat;
        bn::optional<bn::sprite_affine_mat_ptr> _lance_mat_back;
        int _ward_kills = 0;
        int _gold = 0;
        int _gold_from[gold_source_count] = {};   // summary breakdown (E5.4)
        int _kills = 0;
        int _levels[weapon_or_passive_count] = {};
        int _walk_anim = 0;
        int _idle_anim = 0;
        int _death_anim = 0;
        int _wizard_frame = 0;
        int _wizard_dir = 2;    // art::dir index (2 = east)
        int _wizard_turn = 0;
        bool _has_aim = false;
        int _aim_dx = 0;
        int _aim_dy = 0;
        int _blink_cd = 0;
        int _blink_invulnerable = 0;
        bool _blink_hint_done = false;
        static constexpr int nova_max = 120;   // §19.6 (was 150)   // ~50 s of early play (2 per kill, 1 per gem XP)
        int _nova = 0;
        int _nova_flash = 0;
        bool _nova_hint_done = false;
        int _last_nova_shown = -1;
        bn::vector<bn::sprite_ptr, 2> _nova_bar;
        bn::vector<bn::sprite_ptr, 2> _nova_text;

        // Weapons
        int _spark_cd = 30;
        int _rune_timer = 0;
        int _frost_cd = 60;
        int _frost_t = -1;
        uint8_t _frost_pulse_id = 0;

        // Blessing
        int _blessing = -1;
        int _blessing_t = 0;

        // Time and director
        game_mode _mode = game_mode::play;
        game_mode _menu_return = game_mode::play;
        int _t = 0;
        int _stage = 0;               // 0 Gloam Meadow, 1 Grim Graveyard
        int _spawn_cd = 0;
        int _spawns_total = 0;
        uint16_t _next_uid = 1;
        int _swarm_warning_t = 0;
        int _swarm_kind = 0;
        int _swarm_rows_left = 0;
        int _swarm_row_t = 0;
        int _rest_t = 0;              // 8 s without spawns after a boss (E1)
        int _bosses_killed = 0;
        int _boss_kind = 0;           // 0 Slime King, 1 Orc Warchief
        int _boss_max_hp = 600;
        int _big_alive = 0;
        static constexpr int max_big_alive = 10;   // 32x32 monsters (§C1, OBJ line budget)
        int _treasure_at[3] = { -1, -1, -1 };   // frame of each act's Treasure Goblin window (-1 none)
        int _cursed_at[3] = { -1, -1, -1 };     // and of its cursed chest
        int _curse_t = 0;                       // frames left of the cursed elite wave
        bool _treasure_alive = false;
        bn::optional<bn::sprite_ptr> _treasure_arrow;
        bn::vector<split_request, 8> _splits;
        bn::vector<queued_spawn, 40> _spawn_queue;
        int _spawn_queue_age = 0;
        fixed_point _swarm_dir;
        fixed_point _swarm_origin;
        bool _stress = false;
        bool _nova_cast_frame = false;
        int _nova_pending = 0;
        bool _nova_hint_shown = false;

        // Spatial buckets (perf + separation): enemies linked by index into 32 px cells of a wrapped 16x16
        // window (512x512 px; enemies live within 260 px of the wizard). Kept valid on spawn, move and remove.
        static constexpr int grid_shift = 5;
        static constexpr int grid_size = 16;
        int8_t _grid_head[grid_size * grid_size];
        int8_t _grid_next[max_enemies];
        uint8_t _grid_cell[max_enemies];
        int _number_frame = -1;
        int _nova_step = 0;
        bn::optional<bn::sprite_ptr> _nova_ring;
        int _nova_ring_t = 0;
        static constexpr int nova_sparks = 12;
        int _nova_damage = 0;
        static constexpr int nova_hit_frames = 6;
        static constexpr int nova_hits_per_frame = 12;
        bn::timer _prof_timer;

        // Feel
        int _shake = 0;
        int _level_flash = 0;
        int _hit_sound_cd = 0;
        int _gem_combo = 0;
        int _gem_combo_t = 0;
        int _hint_t = 0;
        int _world_hint_t = 0;
        bool _gem_hint_done = false;
        bool _pick_hint_done = false;
        int _last_clock_s = -1;
        int _last_level_shown = -1;
        int _last_gold_shown = -1;
        int _end_timer = 0;
        int _play_streak = 0;
        bool _summary_shown = false;

        // Menus
        int _cursor = 0;
        int _card_count = 0;
        uint8_t _card_ids[3] = {};

        // Shrine
        bn::optional<bn::sprite_ptr> _shrine;
        bn::optional<bn::sprite_ptr> _shrine_arrow;
        fixed_point _shrine_pos;
        int _shrine_touch = 0;

        // Boss (§8 Slime King)
        bn::optional<bn::sprite_ptr> _boss;
        bn::optional<bn::sprite_ptr> _boss_shadow;
        bn::optional<bn::sprite_ptr> _boss_wave;
        bool _boss_spawned = false;
        bool _boss_dead = false;
        fixed_point _boss_pos;
        fixed_point _boss_from;
        fixed_point _boss_target;
        int _boss_hp = 0;
        int _boss_state = 0;
        int _boss_timer = 0;
        int _boss_flash = 0;
        int _boss_rune_cd = 0;
        uint8_t _boss_frost_pulse = 0;
        int _boss_wave_t = 0;
        int _boss_title_t = 0;
        int _boss_fight_t = 0;
        int _boss_phase = 1;
        int _boss_invuln = 0;
        int _boss_frame = -1;
        int _boss_attacks = 0;
        int _boss_bar_shown = -1;
        bool _boss_second = false;
        bool _boss_leash = false;
        fixed_point _boss_dir;
        bn::vector<bn::sprite_ptr, 4> _boss_warn;
        bn::vector<mark, 16> _marks;
        bn::vector<shot, 12> _shots;
        bn::vector<bn::sprite_ptr, 12> _wave_ring;   // Banshee wail
        bool _boss_ghostly = false;
        int _lich_teleport_cd = 0;
        int _rise_counter = 0;
        int _rise_sound_cd = 0;

        // Thornwood Treant (block 3): roots, thorn wall, seeds.
        struct root_spike
        {
            bn::sprite_ptr sprite;
            fixed_point pos;
            int t = 0;
            int frame = 0;

            explicit root_spike(bn::sprite_ptr&& s) :
                sprite(bn::move(s))
            {
            }
        };

        struct seed_shot
        {
            bn::sprite_ptr sprite;
            fixed_point pos;
            fixed_point vel;
            int t = 0;

            explicit seed_shot(bn::sprite_ptr&& s) :
                sprite(bn::move(s))
            {
            }
        };

        bn::vector<root_spike, 6> _roots;
        bn::vector<seed_shot, 8> _seeds;
        // Thorn wall (DISENO_FASES "Decision G-52.1"): ellipse of 24 thorns centred on the wizard, camera locked.
        static constexpr int thorn_count = 24;
        static constexpr int thorn_sprout = 60;                     // harmless blinking sprouts
        static constexpr int thorn_grown = thorn_sprout + 8;        // thorns grow in 8 f, then they hurt
        static constexpr int thorn_wither = thorn_grown + 1200;     // 20 s wall (10 s closing + 10 s fixed)
        static constexpr int thorn_gone = thorn_wither + 30;        // 30 f wither (blink, harmless, not solid)
        static constexpr int thorn_cycle = thorn_gone + 600;        // 10 s free, then again while in phase 2
        bn::vector<bn::sprite_ptr, thorn_count> _thorns;
        int _thorn_rx = 104;
        int _thorn_ry = 56;
        int _cam_lock = 0;          // 0..20: camera blend from the wizard to the wall centre
        int _thorn_hits_wall = 0;
        bn::optional<bn::sprite_tiles_ptr> _thorn_tiles;   // one block shared by the 16 thorn sprites
        bn::optional<bn::sprite_palette_ptr> _thorn_pal;
        fixed_point _boss_entry;
        fixed_point _thorn_center;
        int _thorn_t = -1;          // frames since the wall rose (-1 none)
        int _thorn_hurt_cd = 0;
        int _roots_pending = 0;     // roots still to come out this slam (1 per frame)
        int _seeds_pending = 0;     // seeds still to fire (2 per frame)
        int _root_cd = 0;
        int _seed_cd = 0;
        int _wolf_cd = 0;
        bool _hex_moon = false;
        int _slowmo_t = 0;          // E5.4: logic at 1/2 after the final boss dies
        int _music_resume_t = 0;    // meadow music back after the boss jingle

        // Ember Toss / Storm Chain
        struct ember_ball
        {
            bn::sprite_ptr sprite;
            fixed_point pos;
            fixed_point vel;
            int t = 0;

            explicit ember_ball(bn::sprite_ptr&& s) :
                sprite(bn::move(s))
            {
            }
        };

        struct fire_patch
        {
            bn::sprite_ptr sprite;
            fixed_point pos;
            int life = 0;

            explicit fire_patch(bn::sprite_ptr&& s) :
                sprite(bn::move(s))
            {
            }
        };

        bn::vector<ember_ball, 2> _embers;
        bn::vector<fire_patch, 4> _fires;
        int _ember_cd = 90;
        static constexpr int chain_sprites = 16;
        bn::vector<bn::sprite_ptr, chain_sprites> _chain_sprites;
        int _chain_cd = 60;
        int _chain_show = 0;
        // perf: new sprites per frame for kill effects (death anim, poof, gem); past it they are skipped or merged.
        static constexpr int create_budget_per_frame = 4;
        // Wolves crouching at once (review block 2 said 3; G-52: 3 gave 44 dash hits in 8:00-9:00).
        static constexpr int wolf_max_crouching = 2;
        // G-55 (provisional number tweak, lead decision): frames between two wolves starting a crouch, so the pack
        // takes turns (420 f cooldown + max 2 still gave 45 dash hits in 8:00-9:00, target <= 25).
        static constexpr int wolf_pack_gap = 75;
        int _wolf_pack_cd = 0;
        int _create_budget = create_budget_per_frame;
        int _chain_jumps = 0;
        int _chain_damage = 0;
        int _chain_range = 0;
        int _chain_hits = 0;
        int _chain_sprite_k = 0;
        fixed_point _chain_from;
        uint16_t _chain_hit[8] = {};

        // DBG menu values
        int _dbg_start = 0;
        int _dbg_force = 0;
        bn::vector<bn::sprite_ptr, 12> _boss_title;

        // -----------------------------------------------------------------------------------------------------
        // Sound: Maxmod queues a few commands per frame, so at most 2 new sounds per frame.

        // Butano keeps a handle per playing sound (8 with 4 channels): track estimated lengths and keep <= 6 alive.
        int _sounds_this_frame = 0;
        int _sound_slots[6] = {};

        void _tick_sounds()
        {
            _sounds_this_frame = 0;

            for(int& slot : _sound_slots)
            {
                if(slot > 0)
                {
                    --slot;
                }
            }
        }

        void _play(const bn::sound_item& item, fixed volume = 1, fixed speed = 1)
        {
            if(_sounds_this_frame >= 2)
            {
                return;
            }

            // Lengths in frames from audio/LEEME.md (the slot is freed when the sound should have ended).
            int length = 28;

            if(item == art::sfx_level)
            {
                length = 73;
            }
            else if(item == art::sfx_boss)
            {
                length = 95;
            }
            else if(item == art::sfx_nova || item == art::sfx_wave || item == art::sfx_howl || item == art::sfx_horn)
            {
                length = 80;
            }
            else if(item == art::sfx_poof || item == art::sfx_blink || item == art::sfx_explosion ||
                    item == art::sfx_stomp || item == art::sfx_chest || item == art::sfx_coin_burst)
            {
                length = 34;
            }

            for(int& slot : _sound_slots)
            {
                if(slot == 0)
                {
                    slot = length;
                    ++_sounds_this_frame;
                    // Release at once: a finished Maxmod effect keeps its handle "active" until the channel is
                    // reused, which fills Butano's 8 sound handles (crash seen at 4:54 in the god-mode run).
                    test_sounds = test_sounds + 1;
                    test_last_sound_frame = test_frames;
                    test_last_sound_id = item.id();
                    test_last_sound_speed = speed.data();
                    bn::sound_handle handle = item.play(volume, speed, 0);
                    handle.release();
                    return;
                }
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Setup helpers

        void _build_palettes()
        {
            for(bn::color& c : _white_colors)
            {
                c = bn::color(31, 31, 31);
            }

            _boss_white_colors = _white_colors;

            // Wolf crouch warning (review block 2): the wolf palette pushed to red, blinking every 4 f.
            bn::span<const bn::color> wolf_colors = art::wolf.item->palette_item().colors_ref();

            for(int k = 0; k < 16 && k < wolf_colors.size(); ++k)
            {
                bn::color c = wolf_colors[k];
                _wolf_red_colors[k] = k == 0 ? c : bn::color(bn::min(31, 12 + c.red()), c.green() / 3, c.blue() / 3);
            }

            // G-55: the coin palette did not match the arrow's indices (it read pale blue); a gold ramp instead.
            bn::span<const bn::color> arrow_colors = art::edge_arrow.palette_item().colors_ref();

            for(int k = 0; k < 16 && k < arrow_colors.size(); ++k)
            {
                bn::color c = arrow_colors[k];
                int lum = (c.red() * 3 + c.green() * 6 + c.blue()) / 10;
                int level = bn::min(31, 6 + lum);
                _gold_arrow_colors[k] = k == 0 ? c : bn::color(level, level * 4 / 5, level / 5);
            }

            // Elite tint (§4 2b): Swift = gold, Shielded = cyan; computed from whatever palette the item has.
            const bn::color tints[4] = { bn::color(31, 26, 4), bn::color(8, 28, 31), bn::color(30, 8, 28),
                                         bn::color(31, 22, 0) };

            for(int t = 0; t < type_count; ++t)
            {
                const bn::span<const bn::color>& src = def_palette(t).colors_ref();

                for(int e = 0; e < 4; ++e)
                {
                    for(int i = 0; i < 16; ++i)
                    {
                        bn::color c = src[i];
                        // Haunted (stage 2) shares the 4th slot with the treasure gold: a pale violet.
                        bn::color tint = e == 3 && t != type_treasure ? bn::color(22, 14, 31) : tints[e];
                        int r = (c.red() + tint.red()) / 2;
                        int g = (c.green() + tint.green()) / 2;
                        int b = (c.blue() + tint.blue()) / 2;
                        _elite_colors[t][e][i] = i == 0 ? c : bn::color(r, g, b);
                    }

                    // Darkest colour becomes a bright outline so the elite reads at a glance.
                    _elite_colors[t][e][1] = e == 3 && t != type_treasure ? bn::color(22, 14, 31) : tints[e];
                }
            }
        }

        [[nodiscard]] bn::sprite_palette_item _white_palette() const
        {
            return bn::sprite_palette_item(_white_colors, bn::bpp_mode::BPP_4);
        }

        void _flash_white(enemy& e)
        {
            e.sprite.set_palette(*_white_pal);
        }

        // 16 sprite palettes (§C6): an elite palette only while 2 are still free ("create_bpp_4 failed" otherwise).
        void _tint_elite(enemy& e)
        {
            e.tinted = bn::sprite_palettes::available_colors_count() >= 48;

            if(e.tinted)
            {
                e.sprite.set_palette(_enemy_palette(e));
            }
        }

        [[nodiscard]] const bn::music_item& _stage_music() const
        {
            return _stage == 1 ? art::music_graveyard : art::music_meadow;
        }

        void _restore_palette(enemy& e)
        {
            if(e.elite && e.tinted)
            {
                e.sprite.set_palette(_enemy_palette(e));
            }
            else
            {
                e.sprite.set_palette(*_type_pal[e.type]);
            }
        }

        [[nodiscard]] bn::sprite_palette_item _enemy_palette(const enemy& e) const
        {
            if(e.elite)
            {
                return bn::sprite_palette_item(_elite_colors[e.type][e.elite - 1], bn::bpp_mode::BPP_4);
            }

            return def_palette(e.type);
        }

        void _apply_fx_static()
        {
            _wizard_shadow.set_visible(_fx.shadows);
        }

        bn::sprite_ptr _world_sprite(const bn::sprite_item& item, const fixed_point& p, int frame, int z)
        {
            bn::sprite_ptr s = item.create_sprite(p, frame);
            s.set_camera(_camera);
            s.set_bg_priority(2);
            s.set_z_order(z);
            return s;
        }

        void _show_hint(const char* text, int frames)
        {
            _hint.clear();
            _text.set_center_alignment();
            _text.generate(0, 40, text, _hint);
            _hint_t = frames;
        }

        void _show_world_hint(const char* text, const fixed_point& p, int frames)
        {
            _world_hint.clear();
            _text.set_center_alignment();
            _text.generate(p.x(), p.y() - 12, text, _world_hint);

            for(bn::sprite_ptr& s : _world_hint)
            {
                s.set_camera(_camera);
            }

            _world_hint_t = frames;
        }

        // -----------------------------------------------------------------------------------------------------
        // Play mode

        void _update_play()
        {
            if(bn::keypad::start_pressed())
            {
                _open_pause();
                return;
            }

            if(bn::keypad::select_pressed())
            {
                _open_fx_menu();
                return;
            }

            if(_slowmo_t > 0 && (--_slowmo_t & 1))
            {
                return;   // the skipped half of the frames: everything holds still
            }

            ++_t;
            _create_budget = create_budget_per_frame;
            _prof_timer.restart();
            _update_player();
            _prof(0);
            _update_director();
            _prof(1);
            _update_weapons();
            {
                bn::timer horde_timer;
                _update_horde_frames();
                test_prof[14] = horde_timer.elapsed_ticks();
            }
            test_prof[15] = _enemies.size();
            _update_enemies();
            _update_marks();
            _update_shots();
            _prof(6);
            _update_boss();
            _prof(7);
            _update_pickups();
            _prof(8);
            _update_particles();
            _prof(9);
            _update_shrine();
            _update_timers();

            if(_t == 600 * fps && test_level_at_600 == 0)
            {
                test_level_at_600 = _level;
            }

            if(_shop[shop_recovery] && _player_hp > 0 && ++_recovery_t >= 10 * fps / _shop[shop_recovery])
            {
                _recovery_t = 0;
                _player_hp = bn::min(_player_max_hp, _player_hp + 1);   // shop Recovery: 1 HP every 10 s per rank
            }

            int cam_x = to_int(_pos.x());
            int cam_y = to_int(_pos.y());

            // G-52.1: during the thorn wall the camera slides in 20 f to (centre.x, centre.y - 12) and stays there.
            if(_thorn_t >= 0 && _thorn_t < thorn_wither)
            {
                _cam_lock = bn::min(_cam_lock + 1, 20);
            }
            else if(_cam_lock > 0)
            {
                --_cam_lock;
            }

            if(_cam_lock > 0)
            {
                int lx = to_int(_thorn_center.x());
                int ly = to_int(_thorn_center.y()) - 12;
                cam_x += (lx - cam_x) * _cam_lock / 20;
                cam_y += (ly - cam_y) * _cam_lock / 20;
            }

            if(_shake > 0)
            {
                --_shake;

                if(_fx.screen_shake)
                {
                    cam_x += _random.get_int(-2, 3);
                    cam_y += _random.get_int(-2, 3);
                }
            }

            _camera.set_position(cam_x, cam_y);

            if(_map.update(to_int(_pos.x()), to_int(_pos.y())))
            {
                bn::regular_bg_map_ptr props_map = _props->map();
                props_map.reload_cells_ref();
            }

            _refresh_hud(false);

            if(_player_hp <= 0 && _revivals > 0)
            {
                // Shop Revival: once, back with 50 % HP and a Nova-like burst (§C4).
                --_revivals;
                _player_hp = _player_max_hp / 2;
                _invulnerable = 2 * fps;
                _push_enemies_out(64);
                _show_banner("REVIVED!", art::text_gold, 2 * fps);
                _play(art::sfx_nova);
                _shake = 16;
            }

            if(_player_hp <= 0)
            {
                _player_hp = 0;
                if(test_dead_at == 0)
                {
                    test_dead_at = _t / fps;   // the first death of the session (retries do not overwrite it)
                }
                _mode = game_mode::dead;
                _wizard.set_tiles(art::wizard.tiles_item(), art::wizard_death.first);
                _wizard.set_visible(true);
                _death_anim = 0;
                _end_timer = 60;
                bn::music::stop();
                _play(art::sfx_hurt);
                _play(art::jingle_defeat);
            }
            else if(_pending_levels > 0 || _free_upgrades > 0)
            {
                _open_level_up();
            }

            test_time_s = _t / fps;
            test_level = _level;
            test_hp = _player_hp;
            test_enemies = _enemies.size();
            test_kills = _kills;
            test_gold = _gold;
            test_boss_hp = _boss_hp;
            test_projectiles = _bolts.size() + _runes.size() + (_frost_t >= 0 ? _frost.size() : 0);
            test_gems = _pickups.size();
            _prof(10);
        }

        void _prof(int section)
        {
            test_prof[section] = _prof_timer.elapsed_ticks_with_restart();
        }

        [[nodiscard]] fixed _speed_multiplier() const
        {
            fixed m = 1 + fixed(0.08) * _levels[up_boots] + fixed(0.04) * _shop[shop_haste] +
                      (_setup.character == char_morrow ? fixed(0.1) : fixed(0));

            if(_blessing == bless_haste)
            {
                m *= fixed(1.3);
            }

            return m;
        }

        [[nodiscard]] bool _blocked(fixed x, fixed y) const
        {
            int px = to_int(x);
            int py = to_int(y);
            int h = player_half_box;

            if(_thorn_solid() && _outside_wall(px, py, 0))
            {
                return true;   // G-52.1: the thorns are solid for the wizard (walking and Blink)
            }

            return _map.solid_at(px - h, py - h) || _map.solid_at(px + h - 1, py - h) ||
                   _map.solid_at(px - h, py + h - 1) || _map.solid_at(px + h - 1, py + h - 1);
        }

        static fixed _approach(fixed value, fixed target, fixed step)
        {
            if(value < target)
            {
                return bn::min(value + step, target);
            }

            if(value > target)
            {
                return bn::max(value - step, target);
            }

            return value;
        }

        void _update_aim()
        {
            _has_aim = false;
            int best = 200 * 200;

            for(const enemy& e : _enemies)
            {
                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());
                int d = dx * dx + dy * dy;

                if(d < best)
                {
                    best = d;
                    _aim_dx = dx;
                    _aim_dy = dy;
                    _has_aim = true;
                }
            }

            if(_boss && _boss_state != 3)
            {
                _aim_dx = to_int(_boss_pos.x() - _pos.x());
                _aim_dy = to_int(_boss_pos.y() - _pos.y());
                _has_aim = true;
            }
        }

        // ---- B = Blink (user decision): ~40 px hop, 3 s cooldown, 10 f invulnerable, afterimages.
        void _try_blink(int ax, int ay)
        {
            if(_blink_cd > 0)
            {
                return;
            }

            fixed_point dir = ax || ay ? (ax && ay ? fixed_point(diag * ax, diag * ay) : fixed_point(ax, ay)) : _facing;
            fixed_point start = _pos;
            fixed_point end = _pos;

            for(int step = 1; step <= 5; ++step)
            {
                fixed_point p = start + dir * (step * 8);

                if(_blocked(p.x(), p.y()))
                {
                    break;
                }

                end = p;
            }

            if(end == start)
            {
                return;
            }

            _pos = end;
            _blink_cd = _cooldown(3 * fps) * (_setup.character == char_morrow ? 4 : 5) / 5;   // Morrow -20 %
            _blink_invulnerable = 10;
            _play(art::sfx_blink, fixed(0.8));
            test_blinks = test_blinks + 1;

            // Implosion where he leaves, flash where he lands, translucent ghosts in between.
            _spawn_particle(fixed_point(start.x(), start.y() + art::wizard_y_offset), fixed_point(), art::blink,
                            art::blink_out, art::blink_out.count * art::blink_out.frame_time, 4);
            _spawn_particle(fixed_point(end.x(), end.y() + art::wizard_y_offset), fixed_point(), art::blink,
                            art::blink_in, art::blink_in.count * art::blink_in.frame_time, 4);

            for(int i = 1; i <= 3; ++i)
            {
                fixed_point p = start + (end - start) * i / 4;
                _spawn_particle(fixed_point(p.x(), p.y() + art::wizard_y_offset), fixed_point(), art::wizard_ghost,
                                { _wizard_frame, 1, 1 }, 6 + i * 3, 7, _wizard.horizontal_flip());

                if(! _particles.empty())
                {
                    _particles.back().sprite.set_blending_enabled(true);
                }
            }
        }

        // ---- A = Hex Nova (user decision): meter from kills and gems, clears the screen when full.
        void _add_nova(int amount)
        {
            if(_nova >= nova_max)
            {
                return;
            }

            _nova = bn::min(nova_max, _nova + amount);
            test_nova = _nova;

            if(_nova == nova_max && ! _nova_hint_done)
            {
                _nova_hint_done = true;
                _show_hint("A: NOVA READY!", 3 * fps);
                _nova_hint_shown = true;
            }
        }

        void _cast_nova()
        {
            _nova = 0;
            test_nova = 0;
            test_novas_cast = test_novas_cast + 1;
            _nova_cast_frame = true;

            // Lodestone >= 3 (§C3): the Nova pulls every gem on the field.
            if(_levels[up_lodestone] >= 3)
            {
                int pulled = 0;

                for(pickup& g : _pickups)
                {
                    if(g.kind == pickup_gem && ! g.flying)
                    {
                        g.flying = true;
                        ++pulled;
                    }
                }

                test_nova_pulled = test_nova_pulled + pulled;
            }
            _nova_flash = 24;
            _shake = 20;
            _play(art::sfx_nova);
            _nova_damage = _damage(40);
            _nova_pending = 0;

            // Targets are fixed now (on screen at the cast); the hits land in _update_nova, nearest first.
            for(enemy& e : _enemies)
            {
                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());

                if(abs_int(dx) <= 136 && abs_int(dy) <= 96)
                {
                    e.nova = 1;
                    ++_nova_pending;
                }
            }

            _nova_step = 0;

            if(_nova_hint_shown)
            {
                _nova_hint_shown = false;
                _hint.clear();
                _hint_t = 0;
            }

            if(_boss && _boss_state != 3)
            {
                _hit_boss(_damage(60));
            }

            // fx_nova ring at the wizard, scaled up 1x -> 2x while it plays (affine, double size), plus sparks.
            _nova_ring.reset();
            _nova_ring = art::nova_ring.create_sprite_optional(_pos, art::nova_ring_anim.first);

            if(_nova_ring)
            {
                _nova_ring->set_camera(_camera);
                _nova_ring->set_bg_priority(2);
                _nova_ring->set_z_order(3);
                _nova_ring_t = 0;
            }

            for(int i = 0; i < nova_sparks; ++i)
            {
                fixed_point v = angle_vector(i * (2048 / nova_sparks) + 64) * fixed(2.5);
                _spawn_particle(_pos + v * 4, v, art::nova_spark, art::nova_spark_anim, 22, 3);
            }
        }

        void _update_nova_ring()
        {
            if(! _nova_ring)
            {
                return;
            }

            ++_nova_ring_t;
            int length = art::nova_ring_anim.count * art::nova_ring_anim.frame_time;

            if(_nova_ring_t >= length)
            {
                _nova_ring.reset();
                return;
            }

            _nova_ring->set_position(_pos);

            if(_nova_ring_t % art::nova_ring_anim.frame_time == 0)
            {
                _nova_ring->set_tiles(art::nova_ring.tiles_item(), art::nova_ring_anim.frame(_nova_ring_t, false));
            }

            _nova_ring->set_scale(1 + fixed(_nova_ring_t) / length);
        }

        // Hex Nova hits, a few per frame (perf): the cast frame used to resolve up to 56 hits and kills at once.
        // Each frame takes the pending enemies inside a growing ring, so the blast still spreads from the wizard
        // and every target is hit within nova_hit_frames frames.
        void _update_nova()
        {
            if(_nova_pending <= 0)
            {
                return;
            }

            ++_nova_step;

            int ring = (_nova_step * 160) / nova_hit_frames;
            bool last = _nova_step >= nova_hit_frames;
            int hits = 0;

            for(int i = 0; i < _enemies.size() && hits < nova_hits_per_frame; ++i)
            {
                enemy& e = _enemies[i];

                if(! e.nova)
                {
                    continue;
                }

                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());

                if(! last && abs_int(dx) + abs_int(dy) > ring)
                {
                    continue;
                }

                e.nova = 0;
                --_nova_pending;
                ++hits;

                if(_hit_enemy(i, _nova_damage, _pos))
                {
                    --i;
                }
                else
                {
                    // Enemies fly out: a long, strong knockback.
                    enemy& o = _enemies[i];
                    o.knock = 16;
                    o.knock_vel = unit(dx, dy) * 3;
                }
            }

            if(last && hits < nova_hits_per_frame)
            {
                // Anything left (recycled or out of reach) is dropped.
                for(enemy& e : _enemies)
                {
                    e.nova = 0;
                }

                _nova_pending = 0;
            }
        }

        void _update_player()
        {
            int ax = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
            int ay = int(bn::keypad::down_held()) - int(bn::keypad::up_held());
            _update_aim();

            if(_blink_cd > 0)
            {
                --_blink_cd;
            }

            if(_blink_invulnerable > 0)
            {
                --_blink_invulnerable;
            }

            if(bn::keypad::b_pressed())
            {
                _try_blink(ax, ay);
            }

            if(bn::keypad::a_pressed() && _nova >= nova_max)
            {
                _cast_nova();
            }
            fixed speed = player_speed * _speed_multiplier();
            fixed tx = speed * ax;
            fixed ty = speed * ay;

            if(ax && ay)
            {
                tx *= diag;
                ty *= diag;
            }

            fixed accel = player_accel * speed;
            fixed decel = player_decel * speed;
            _vel.set_x(_approach(_vel.x(), tx, ax ? accel : decel));
            _vel.set_y(_approach(_vel.y(), ty, ay ? accel : decel));

            fixed nx = _pos.x() + _vel.x();

            if(! _blocked(nx, _pos.y()))
            {
                _pos.set_x(nx);
            }
            else
            {
                _vel.set_x(0);
            }

            fixed ny = _pos.y() + _vel.y();

            if(! _blocked(_pos.x(), ny))
            {
                _pos.set_y(ny);
            }
            else
            {
                _vel.set_y(0);
            }

            bool moving = ax || ay;

            if(moving)
            {
                _facing = ax && ay ? fixed_point(diag * ax, diag * ay) : fixed_point(ax, ay);
                ++_walk_anim;
            }

            int frame;
            ++_idle_anim;

            // User decision: Wendel faces where he shoots (nearest target), not where he walks; with no target he
            // faces the movement direction. 8 directions, turning one 45 degree step every few frames (never a
            // 180 degree snap). Walking backwards plays the walk cycle of the aim direction.
            int target = _wizard_dir;

            if(_has_aim)
            {
                target = dir_towards(_aim_dx, _aim_dy, _wizard_dir);
            }
            else if(moving)
            {
                target = dir_towards(ax, ay, _wizard_dir);
            }

            if(_wizard_turn > 0)
            {
                --_wizard_turn;
            }
            else if(target != _wizard_dir)
            {
                _wizard_dir = turn_step(_wizard_dir, target);
                _wizard_turn = art::wizard_turn_frames - 1;
            }

            test_wizard_dir = _wizard_dir;
            int drawn = art::drawn_dir(_wizard_dir);
            frame = moving ? art::wizard_walk[drawn].frame(_walk_anim, true) : art::wizard_idle[drawn].frame(_idle_anim, true);
            _wizard.set_horizontal_flip(art::dir_flipped(_wizard_dir));

            if(_invulnerable > player_invulnerable_frames - 10)   // hurt pose for the first 10 f
            {
                frame = art::wizard_hurt.frame(player_invulnerable_frames - _invulnerable, true);
            }

            _wizard_frame = frame;
            _wizard.set_tiles(art::wizard.tiles_item(), frame);
            _wizard.set_position(_pos.x(), _pos.y() + art::wizard_y_offset);
            // Hurt: white 2 f / normal 2 f; the wizard never disappears (§19.1).
            bool white = _invulnerable > 0 && (_invulnerable & 2) != 0;

            if(white != _wizard_white)
            {
                _wizard_white = white;
                _wizard.set_palette(white ? *_white_pal : *_wizard_pal);
            }
            _wizard_shadow.set_position(_pos.x(), _pos.y() + 5);
            _hp_bar.set_position(_pos.x(), _pos.y() + 12);
            _hp_bar.set_tiles(art::hp_bar.tiles_item(), (_player_hp * art::hp_bar_frames + _player_max_hp - 1) / _player_max_hp);

            if(_invulnerable > 0)
            {
                --_invulnerable;
            }

            if(_grace > 0)
            {
                --_grace;
            }
        }

        void _hurt_player(int damage)
        {
            if(_invulnerable > 0 || _blink_invulnerable > 0 || _grace > 0 || _fx.god_mode)
            {
                return;
            }

            if(! _blink_hint_done)
            {
                _blink_hint_done = true;
                _show_hint("B: BLINK", 3 * fps);
            }

            // §C3: max(1, round(contact x (1 - Ward - Armor))); Ward Stone Nv5 +10 f invulnerability.
            int cut = 6 * _levels[up_ward] + 5 * _shop[shop_armor];
            damage = bn::max(1, (damage * (100 - cut) + 50) / 100);
            _player_hp -= damage;
            _invulnerable = player_invulnerable_frames + (_levels[up_ward] >= 5 ? 10 : 0);
            _shake = 8;
            _play(art::sfx_hurt);
        }

        // -----------------------------------------------------------------------------------------------------
        // Director (§2 spawning, §5, §6)

        [[nodiscard]] int _target_alive() const
        {
            if(_stress)
            {
                return max_enemies;
            }

            int s = _t / fps;

            if(_boss)
            {
                if(_boss_kind % 3 == 2 && _boss_fight_t > 3 * 60 * fps)
                {
                    return max_enemies;   // E5 "THE HEX MOON RISES": normal monsters at 100 % of the cap
                }

                return _boss_kind % 3 == 0 ? 12 : 16;   // DISENO_FASES E2 / E3 boss rows
            }

            if(_stage == 1)
            {
                // E3 Cap per minute; x0.6 for 10 s after the archer, wraith and brute introductions (R3).
                constexpr int caps[16] = { 16, 20, 28, 34, 40, 12, 38, 42, 46, 48, 16, 50, 52, 54, 56, 16 };
                int cap = caps[bn::min(s / 60, 15)];
                return (s >= 360 && s < 370) || (s >= 480 && s < 490) || (s >= 540 && s < 550) ? cap * 6 / 10 : cap;
            }

            if(s < 60)
            {
                return 6 + (8 * s) / 60;
            }

            // E2 Cap per minute; minute 1: 20 -> 16 -> 12 (DISENO §19.1, G-45.2); minute 5 = after the Slime King.
            constexpr int per_minute[16] = { 14, 12, 26, 32, 38, 12, 36, 40, 44, 46, 16, 48, 52, 54, 56, 16 };
            int cap = per_minute[bn::min(s / 60, 15)];

            // Review block 2 (R3): x0.6 for 10 s after the goblin, wolf and orc introductions.
            if((s >= 360 && s < 370) || (s >= 420 && s < 430) || (s >= 540 && s < 550))
            {
                cap = cap * 6 / 10;
            }

            return cap;
        }

        [[nodiscard]] bool _spawn_paused() const
        {
            int s = _t / fps;
            return (_stage == 0 && s >= 50 && s < 56) || (_t >= boss_time - pre_boss_pause && _t < boss_time) ||
                   (_t >= boss2_time - pre_boss_pause && _t < boss2_time && _bosses_killed == 1) ||
                   (_t >= boss3_time - pre_boss_pause && _t < boss3_time && _bosses_killed == 2) || _rest_t > 0 ||
                   _boss_dead;
        }

        // Weighted mixes of E2 (stage 1), minutes 2..10 (index 0 = minute 2). Type then weight; weight 0 ends.
        struct mix_entry
        {
            uint8_t type;
            uint8_t weight;
        };

        [[nodiscard]] int _pick_mixed(int minute, int r) const
        {
            static constexpr mix_entry mixes[14][5] = {
                { { type_skeleton, 50 }, { type_bat, 30 }, { type_slime, 20 }, {}, {} },                      // 2
                { { type_skeleton, 45 }, { type_bat, 25 }, { type_slime, 15 }, { type_big_slime, 15 }, {} },  // 3
                { { type_skeleton, 50 }, { type_bat, 25 }, { type_big_slime, 25 }, {}, {} },                  // 4
                { { type_slime, 50 }, { type_bat, 50 }, {}, {}, {} },                                         // 5
                { { type_goblin, 40 }, { type_skeleton, 25 }, { type_bat, 25 }, { type_big_slime, 10 }, {} }, // 6
                { { type_goblin, 35 }, { type_wolf, 25 }, { type_skeleton, 25 }, { type_bat, 15 }, {} },      // 7
                { { type_wolf, 30 }, { type_goblin, 30 }, { type_skeleton, 25 }, { type_bat, 15 }, {} },      // 8
                { { type_goblin, 35 }, { type_wolf, 25 }, { type_skeleton, 25 }, { type_orc, 15 }, {} },      // 9
                { { type_goblin, 60 }, { type_bat, 40 }, {}, {}, {} },                                        // 10
                { { type_orc, 20 }, { type_wolf, 25 }, { type_goblin, 25 }, { type_bat, 15 }, { type_skeleton, 15 } }, // 11
                { { type_orc, 25 }, { type_wolf, 25 }, { type_goblin, 20 }, { type_skeleton, 20 }, { type_big_slime, 10 } }, // 12
                { { type_orc, 30 }, { type_wolf, 30 }, { type_goblin, 20 }, { type_skeleton, 20 }, {} },      // 13
                { { type_orc, 35 }, { type_wolf, 30 }, { type_goblin, 20 }, { type_skeleton, 15 }, {} },      // 14
                { { type_goblin, 50 }, { type_wolf, 50 }, {}, {}, {} },                                       // 15
            };
            // E3 (stage 2), minutes 0..15.
            static constexpr mix_entry graveyard[16][6] = {
                { { type_ghoul, 70 }, { type_crow, 30 }, {}, {}, {}, {} },                                         // 0
                { { type_ghoul, 50 }, { type_crow, 25 }, { type_skeleton2, 25 }, {}, {}, {} },                     // 1
                { { type_ghoul, 35 }, { type_skeleton2, 35 }, { type_crow, 15 }, { type_walker, 15 }, {}, {} },    // 2
                { { type_skeleton2, 35 }, { type_ghoul, 25 }, { type_ghost, 20 }, { type_walker, 20 }, {}, {} },   // 3
                { { type_skeleton2, 35 }, { type_ghost, 25 }, { type_ghoul, 20 }, { type_walker, 20 }, {}, {} },   // 4
                { { type_ghoul, 50 }, { type_crow, 50 }, {}, {}, {}, {} },                                         // 5
                { { type_skeleton2, 30 }, { type_ghoul, 30 }, { type_ghost, 20 }, { type_archer, 20 }, {}, {} },   // 6
                { { type_archer, 25 }, { type_ghost, 25 }, { type_skeleton2, 25 }, { type_ghoul, 25 }, {}, {} },   // 7
                { { type_ghost, 20 }, { type_archer, 20 }, { type_skeleton2, 25 }, { type_ghoul, 20 },
                  { type_wraith, 15 }, {} },                                                                       // 8
                { { type_skeleton2, 25 }, { type_ghoul, 25 }, { type_archer, 20 }, { type_wraith, 15 },
                  { type_brute, 15 }, {} },                                                                        // 9
                { { type_ghoul, 50 }, { type_ghost, 50 }, {}, {}, {}, {} },                                        // 10
                { { type_skeleton2, 20 }, { type_ghoul, 20 }, { type_archer, 20 }, { type_wraith, 15 },
                  { type_brute, 15 }, { type_knight, 10 } },                                                       // 11
                { { type_skeleton2, 20 }, { type_ghoul, 20 }, { type_archer, 20 }, { type_wraith, 15 },
                  { type_brute, 15 }, { type_knight, 10 } },                                                       // 12
                { { type_brute, 20 }, { type_knight, 15 }, { type_wraith, 20 }, { type_archer, 20 },
                  { type_skeleton2, 25 }, {} },                                                                    // 13
                { { type_brute, 25 }, { type_knight, 20 }, { type_wraith, 20 }, { type_archer, 15 },
                  { type_ghoul, 20 }, {} },                                                                        // 14
                { { type_ghoul, 50 }, { type_ghost, 50 }, {}, {}, {}, {} },                                        // 15
            };

            if(_stage == 1)
            {
                const mix_entry* row = graveyard[bn::max(0, bn::min(minute, 15))];

                for(int k = 0; k < 6 && row[k].weight; ++k)
                {
                    if(r < row[k].weight)
                    {
                        return row[k].type;
                    }

                    r -= row[k].weight;
                }

                return type_ghoul;
            }

            const mix_entry* mix = mixes[bn::max(0, bn::min(minute, 15) - 2)];

            for(int k = 0; k < 5 && mix[k].weight; ++k)
            {
                if(r < mix[k].weight)
                {
                    return mix[k].type;
                }

                r -= mix[k].weight;
            }

            return type_skeleton;
        }

        // §C1 "first time": a new type enters alone and joins the draw 10 s later.
        [[nodiscard]] static int _intro_second(int type)
        {
            switch(type)
            {
            case type_big_slime:
                return 180;
            case type_goblin:
                return 360;
            case type_wolf:
                return 420;
            case type_orc:
                return 540;
            case type_crow:
                return 30;
            case type_walker:
                return 120;
            case type_ghost:
                return 180;
            case type_archer:
                return 360;
            case type_wraith:
                return 480;
            case type_brute:
                return 540;
            case type_knight:
                return 660;
            default:
                return 0;
            }
        }

        [[nodiscard]] fixed_point _spawn_point(int min_d, int max_d, bool ahead)
        {
            for(int attempt = 0; attempt < 4; ++attempt)
            {
                fixed_point dir;

                if(ahead && _random.get_int(10) < 7)
                {
                    fixed_point r = angle_vector(_random.get_int(-340, 341));
                    dir = fixed_point(_facing.x() * r.x() - _facing.y() * r.y(), _facing.x() * r.y() + _facing.y() * r.x());
                }
                else
                {
                    dir = angle_vector(_random.get_int(2048));
                }

                int d = _random.get_int(min_d, max_d + 1);
                fixed_point p = _pos + dir * d;

                if(! _map.solid_at(to_int(p.x()), to_int(p.y())))
                {
                    return p;
                }
            }

            return _pos + angle_vector(_random.get_int(2048)) * max_d;
        }

        enemy* _spawn_enemy(int type, const fixed_point& p, int elite_chance_den)
        {
            if(_enemies.full())
            {
                return nullptr;
            }

            if(! (enemy_defs[type].stages & (1 << _stage)))
            {
                // Shared helpers (cursed wave, forced elites) ask for stage 1 types: their art holds no tiles here.
                type = _stage ? int(type_skeleton2) : int(type_skeleton);
            }

            if(type == type_archer && _type_alive[type_archer] >= 6)
            {
                type = type_skeleton2;   // §C1: <= 6 shooters (queued groups and formations included)
            }

            // 16 sprite palettes (§C6): a type whose palette is not held yet only enters while one stays free
            // for banners and effects ("create_bpp_4 failed" in the Lich fight otherwise).
            if(! _type_pal[type] && bn::sprite_palettes::available_colors_count() < 32)
            {
                test_palette_refusals = test_palette_refusals + 1;
                return nullptr;
            }

            const enemy_def& def = enemy_defs[type];
            // Palettes are held only while a monster of the type is alive (16 sprite palettes in total; holding all
            // 8 types from the start ran out of palettes during the Slime King).
            if(! _type_pal[type])
            {
                _type_pal[type] = type == type_treasure ?
                            bn::sprite_palette_item(_elite_colors[type][3], bn::bpp_mode::BPP_4).create_palette() :
                            def_palette(type).create_palette();
            }

            ++_type_alive[type];

            if(type == type_archer)
            {
                test_shooters_max = bn::max(int(test_shooters_max), _type_alive[type]);
            }
            const bn::sprite_tiles_ptr& tiles = _horde_tiles[_type_slot[type]][def.look->walk_dirs > 1 ? 2 : 0];
            bn::sprite_ptr sprite = bn::sprite_ptr::create(p, def.look->item->shape_size(), tiles, *_type_pal[type]);
            sprite.set_camera(_camera);
            sprite.set_bg_priority(2);
            sprite.set_z_order(10);
            enemy& e = _enemies.emplace_back(bn::move(sprite));
            e.pos = p;
            _grid_link(_enemies.size() - 1);
            e.type = uint8_t(type);
            e.hp = scaled_hp(def.hp, _t / (60 * fps));
            e.dir = 2;
            e.uid = _next_uid++;

            if(_next_uid == 0)
            {
                _next_uid = 1;
            }

            ++_spawns_total;

            e.flank = (e.uid & 1) != 0;

            if(elite_chance_den > 0 && _t >= 2 * 60 * fps && _elites_alive() < (_stage ? 4 : 3) && ! def.big &&
               _random.get_int(elite_chance_den) == 0)
            {
                // Swift, Shielded; Splitting from 5:00 (§C2 élites, block 2); Haunted in stage 2.
                e.elite = uint8_t(1 + _random.get_int(_t >= boss_time ? 3 : 2));

                if(_stage == 1 && _random.get_int(4) == 0)
                {
                    e.elite = 4;
                }

                if(e.elite == 2)
                {
                    e.shield = 3;
                }

                e.hp *= e.elite == 1 ? 2 : 3;   // §C2 élites: x3 HP (Swift x2)

                _tint_elite(e);
                e.aura = _world_sprite(art::fx_aura, p, 0, 11);
            }

            return &e;
        }

        [[nodiscard]] int _elites_alive() const
        {
            int count = 0;

            for(const enemy& e : _enemies)
            {
                count += e.elite != 0;
            }

            return count;
        }

        enemy* _force_elite(int elite)
        {
            enemy* e = _spawn_enemy(type_skeleton, _spawn_point(80, 96, true), 0);

            if(e)
            {
                e->elite = uint8_t(elite);
                e->shield = elite == 2 ? 3 : 0;
                e->hp *= 3;
                _tint_elite(*e);
                e->aura = _world_sprite(art::fx_aura, e->pos, 0, 11);
            }

            return e;
        }

        // 13:00 ELITE PAIR (E1): two elites of the minute's mix from opposite sides, with the red elite arrow.
        void _spawn_elite_pair()
        {
            _make_room(2);
            fixed_point dir = angle_vector(_random.get_int(2048));
            test_events = test_events | 4096;

            for(int k = 0; k < 2; ++k)
            {
                fixed_point p = _pos + dir * (k ? -130 : 130);

                if(_map.solid_at(to_int(p.x()), to_int(p.y())))
                {
                    p = _spawn_point(120, 140, k == 0);
                }

                int type = _pick_type();
                type = enemy_defs[type].big ? int(_stage ? type_brute : type_orc) : type;
                _make_elite(_spawn_enemy(type, p, 0), 1 + _random.get_int(3));
            }
        }

        void _make_elite(enemy* e, int elite)
        {
            if(e && ! e->elite)
            {
                e->elite = uint8_t(elite);
                e->shield = elite == 2 ? 3 : 0;
                e->hp *= elite == 1 ? 2 : 3;
                _tint_elite(*e);
                e->aura = _world_sprite(art::fx_aura, e->pos, 0, 11);
            }
        }

        void _spawn_cursed_chest()
        {
            fixed_point p = _spawn_point(80, 100, true);

            if(_spawn_pickup(p, pickup_cursed, 2))
            {
                _show_world_hint("CURSED CHEST", p, 3 * fps);
            }
        }

        // §C5 cursed chest: "CURSED!" and 20 s of elites, one every 2 s, at most 5 alive.
        void _open_cursed_chest(const fixed_point& p)
        {
            _free_upgrades += 2;
            _curse_t = 20 * fps;
            test_events = test_events | 8192;
            _spawn_particle(p, fixed_point(), art::chest, { art::chest_cursed_open, 1, 40 }, 40, 20);
            _play(art::sfx_curse_sting);
            _show_banner("CURSED!", art::text_red, 2 * fps);
            _shake = bn::max(_shake, 8);
        }

        void _update_curse()
        {
            if(_curse_t <= 0)
            {
                return;
            }

            --_curse_t;

            if(_curse_t % (2 * fps) == 0 && _elites_alive() < 5)
            {
                int type = _pick_type();
                type = enemy_defs[type].big || type == type_treasure ? int(type_skeleton) : type;
                _make_room(1);
                _make_elite(_spawn_enemy(type, _spawn_point(110, 130, false), 0), 1 + _random.get_int(3));
                test_curse_elites = bn::max(int(test_curse_elites), _elites_alive());
            }
        }

        // Big centred banner (boss names, CURSED!, THE HEX MOON RISES, STAGE CLEAR).
        void _show_banner(const char* text, const bn::sprite_palette_item& palette, int frames)
        {
            _boss_title.clear();
            _big_text.set_center_alignment();
            _big_text.set_palette_item(palette);
            _big_text.generate(0, -30, text, _boss_title);
            _big_text.set_palette_item(common::variable_16x16_sprite_font.item().palette_item());
            _boss_title_t = frames;
        }

        void _make_room(int needed)
        {
            // Removes the farthest chasers (no drops) so a scripted event always fits in the 56 cap.
            while(_enemies.size() > max_enemies - needed)
            {
                int far_i = 0;
                int far_d = -1;

                for(int i = 0, n = _enemies.size(); i < n; ++i)
                {
                    int dx = to_int(_enemies[i].pos.x() - _pos.x());
                    int dy = to_int(_enemies[i].pos.y() - _pos.y());
                    int d = abs_int(dx) + abs_int(dy);

                    if(d > far_d)
                    {
                        far_d = d;
                        far_i = i;
                    }
                }

                _remove_enemy(far_i);
            }
        }

        [[nodiscard]] static int _grid_cell_of(int x, int y)
        {
            return ((y >> grid_shift) & (grid_size - 1)) * grid_size + ((x >> grid_shift) & (grid_size - 1));
        }

        void _grid_clear()
        {
            for(int8_t& head : _grid_head)
            {
                head = -1;
            }
        }

        void _grid_link(int i)
        {
            const enemy& e = _enemies[i];
            int cell = _grid_cell_of(to_int(e.pos.x()), to_int(e.pos.y()));
            _grid_cell[i] = uint8_t(cell);
            _grid_next[i] = _grid_head[cell];
            _grid_head[cell] = int8_t(i);
        }

        // Replaces index "from" by "to" in its bucket (to < 0 unlinks it).
        void _grid_relink(int from, int to)
        {
            int8_t* link = &_grid_head[_grid_cell[from]];

            while(*link != from)
            {
                link = &_grid_next[*link];
            }

            if(to < 0)
            {
                *link = _grid_next[from];
            }
            else
            {
                *link = int8_t(to);
                _grid_next[to] = _grid_next[from];
                _grid_cell[to] = _grid_cell[from];
            }
        }

        void _grid_refresh(int i)
        {
            const enemy& e = _enemies[i];

            if(_grid_cell_of(to_int(e.pos.x()), to_int(e.pos.y())) != _grid_cell[i])
            {
                _grid_relink(i, -1);
                _grid_link(i);
            }
        }

        // Indices of the enemies whose centre is inside the box (x +- r, y +- r, exclusive), sorted from high to low
        // so the caller can remove the one it is processing (_remove_enemy moves the last enemy into its slot).
        int _grid_query(int x, int y, int r, int8_t* out) const
        {
            int count = 0;
            int cx0 = (x - r) >> grid_shift;
            int cx1 = (x + r) >> grid_shift;
            int cy0 = (y - r) >> grid_shift;
            int cy1 = (y + r) >> grid_shift;

            for(int cy = cy0; cy <= cy1; ++cy)
            {
                for(int cx = cx0; cx <= cx1; ++cx)
                {
                    int cell = (cy & (grid_size - 1)) * grid_size + (cx & (grid_size - 1));

                    for(int i = _grid_head[cell]; i >= 0; i = _grid_next[i])
                    {
                        const enemy& e = _enemies[i];

                        if(abs_int(to_int(e.pos.x()) - x) >= r || abs_int(to_int(e.pos.y()) - y) >= r)
                        {
                            continue;
                        }

                        int k = count++;

                        while(k > 0 && out[k - 1] < i)
                        {
                            out[k] = out[k - 1];
                            --k;
                        }

                        out[k] = int8_t(i);
                    }
                }
            }

            return count;
        }

        void _remove_enemy(int i)
        {
            int type = _enemies[i].type;

            if(--_type_alive[type] == 0)
            {
                _type_pal[type].reset();

                if(type == type_wolf)
                {
                    _wolf_red_pal.reset();
                }
            }

            int last = _enemies.size() - 1;
            _grid_relink(i, -1);

            if(i != last)
            {
                _grid_relink(last, i);
                _enemies[i] = bn::move(_enemies.back());
            }

            _enemies.pop_back();
        }

        [[nodiscard]] int _pick_type()
        {
            int s = _t / fps;
            int r = _random.get_int(100);

            if(_stress)
            {
                // Worst VRAM/CPU mix (DISENO_RUTA block 2): every horde art of stage 1 at once.
                constexpr uint8_t mix[8] = { type_slime, type_skeleton, type_goblin, type_wolf, type_orc, type_bat,
                                             type_big_slime, type_goblin };
                if(_stage == 1)
                {
                    constexpr uint8_t graveyard[8] = { type_ghoul, type_skeleton2, type_crow, type_ghost, type_brute,
                                                       type_knight, type_archer, type_wraith };
                    return graveyard[(_spawns_total + r) & 7];
                }

                return mix[(_spawns_total + r) & 7];
            }

            if(_stage == 1)
            {
                // A held-back type (not introduced, the big cap, <= 6 archers: §C1) is redrawn, then a ghoul.
                int type = _pick_mixed(s / 60, r);

                for(int retry = 0; retry < 4 && _held_back(type, s); ++retry)
                {
                    type = _pick_mixed(s / 60, _random.get_int(100));
                }

                return _held_back(type, s) ? int(type_ghoul) : type;
            }

            if(s >= 120)
            {
                int type = _pick_mixed(s / 60, r);

                // A held-back type (not introduced yet, or the big cap) is redrawn from the minute's mix (review).
                for(int retry = 0; retry < 4 && (s < _intro_second(type) + 10 ||
                                                 (enemy_defs[type].big && _big_alive >= max_big_alive)); ++retry)
                {
                    type = _pick_mixed(s / 60, _random.get_int(100));
                }

                if(s < _intro_second(type) + 10 || (enemy_defs[type].big && _big_alive >= max_big_alive))
                {
                    type = type_skeleton;
                }

                return type;
            }

            if(s < 70)
            {
                return type_slime;
            }

            if(s < 120)
            {
                // §19.2: skeletons join at 1:10, 15 % ramping to 35 % at 1:40.
                int skeleton_pct = 15 + (20 * bn::min(s - 70, 30)) / 30;
                return r < skeleton_pct ? type_skeleton : type_slime;
            }

            if(s < 180)
            {
                return r < 50 ? type_skeleton : r < 80 ? type_bat : type_slime;
            }

            if(s < 240)
            {
                return r < 45 ? type_skeleton : r < 75 ? type_bat : type_slime;
            }

            return r < 55 ? type_skeleton : type_bat;
        }

        [[nodiscard]] bool _held_back(int type, int s) const
        {
            return s < _intro_second(type) + 10 || (enemy_defs[type].big && _big_alive >= max_big_alive) ||
                   (type == type_archer && _type_alive[type_archer] >= 6);
        }

        void _update_director()
        {
            int s = _t / fps;
            bool second = _t % fps == 0;

            // 0:04 one slime from the side the wizard faces (§5).
            if(_t == 4 * fps && _stage == 0)
            {
                _spawn_enemy(type_slime, _spawn_point(136, 150, true), 0);
            }

            // §19.2: no scripted skeleton pair at 1:00 any more; they enter the spawn pool from 1:10.

            if(_rest_t > 0)
            {
                --_rest_t;
            }

            _spawn_pending_splits();
            _spawn_queued();

            // Shrines every 90 s (§4 2a, E2): not with a boss alive, not at 5:00 / 6:00 (boss and rest).
            if(second && s > 0 && s % 90 == 0 && s != 360 && _t < boss3_time && ! _boss && ! _shrine && ! _boss_dead)
            {
                _spawn_shrine();
            }

            // 2:00 Bat Swarm with a 1 s warning (§5, §6); 8:00 Wolf Pack (STAMPEDE, 90 f + howl); 9:15 Goblin Raid
            // (SWARM, 60 f).
            if(_stage == 1)
            {
                _update_events_f2(s, second);
            }
            else if(_t == 2 * 60 * fps - fps)
            {
                _begin_swarm_warning(swarm_bats, fps);
            }
            else if(_t == 8 * 60 * fps - 90)
            {
                _begin_swarm_warning(swarm_wolves, 90);
                _play(art::sfx_howl);
            }
            else if(_t == (9 * 60 + 15) * fps - fps)
            {
                _begin_swarm_warning(swarm_goblins, fps);
            }

            if(_swarm_warning_t > 0)
            {
                --_swarm_warning_t;
                _update_swarm_warning();

                if(_swarm_warning_t == 0)
                {
                    _warnings.clear();
                    _spawn_swarm();
                }
            }
            else if(_swarm_rows_left > 0 && --_swarm_row_t <= 0)
            {
                _spawn_swarm();
            }

            // 4:00 Bone Ring (§6); 12:00 Double Bone Ring (2 x 16, the second 3 s later); 13:00 Elite Pair;
            // 14:00 Stampede of wolves and orcs (3 rows, 90 f + horn).
            if(_stage == 1)
            {
            }
            else if(_t == 4 * 60 * fps || ((_t == 12 * 60 * fps || _t == (12 * 60 + 3) * fps) && ! _stress))
            {
                _spawn_bone_ring();
            }
            else if(_t == 13 * 60 * fps && ! _stress)
            {
                _spawn_elite_pair();
            }
            else if(_t == 14 * 60 * fps - 90 && ! _stress)
            {
                _begin_swarm_warning(swarm_stampede, 90);
                _play(art::sfx_horn);
            }

            // Cursed chest: one 60 % window per act (§C5).
            for(int act = 0; act < 3; ++act)
            {
                if(_t == _cursed_at[act] && ! _boss)
                {
                    _spawn_cursed_chest();
                }
            }

            _update_curse();

            // First appearances (§C1): alone, from the side the wizard faces. 7:00 wolves howl 1.5 s before.
            if(second && ! _stress && _stage == 0)
            {
                if(s == 180)
                {
                    _spawn_enemy(type_big_slime, _spawn_point(136, 150, true), 0);
                }
                else if(s == 360 || s == 420)
                {
                    fixed_point p = _spawn_point(136, 150, true);
                    int type = s == 360 ? type_goblin : type_wolf;
                    _spawn_enemy(type, p, 0);
                    _spawn_enemy(type, p + fixed_point(0, 18), 0);
                }
                else if(s == 540)
                {
                    _spawn_enemy(type_orc, _spawn_point(136, 150, true), 0);
                }
            }

            if(_t == 420 * fps - 90 && ! _stress && _stage == 0)
            {
                _play(art::sfx_howl);
            }

            // Treasure Goblin: one window per act (§C5), or forced from the DBG menu.
            for(int act = 0; act < 3; ++act)
            {
                if(_t == _treasure_at[act] && ! _boss)
                {
                    _spawn_treasure();
                }
            }

            // 5:00 Slime King (§8), 10:00 Orc Warchief (E5.1).
            if(_t == boss_time && ! _boss_spawned)
            {
                _spawn_boss(_stage * 3);
            }
            else if(_t >= boss2_time && _bosses_killed == 1 && ! _boss && ! _stress)
            {
                _spawn_boss(_stage * 3 + 1);
            }
            else if(_t >= boss3_time && _bosses_killed == 2 && ! _boss && ! _stress)
            {
                _spawn_boss(_stage * 3 + 2);
            }

            if(_t < 10 * fps && ! _stress)
            {
                return;
            }

            if(_spawn_paused() && ! _stress)
            {
                return;
            }

            if(--_spawn_cd > 0)
            {
                return;
            }

            int target = _target_alive();
            int alive = _enemies.size();

            if(alive >= target)
            {
                _spawn_cd = 10;
                return;
            }

            int group;

            if(_stress)
            {
                group = 2;   // perf: 2 per frame (each new sprite costs ~5 % of a frame; 4 per frame measured +20 %)
                _spawn_cd = 1;
            }
            else if(s < 30)
            {
                group = 2 + _random.get_int(2);
                _spawn_cd = 100;
            }
            else if(s < 60)
            {
                group = 4 + _random.get_int(3);
                _spawn_cd = 130;
            }
            else
            {
                constexpr int intervals[5] = { 0, 70, 60, 50, 45 };
                group = 3 + _random.get_int(3);
                _spawn_cd = intervals[bn::min(s / 60, 4)];
            }

            group = bn::min(group, target - alive);
            bool two_sides = s >= 30;
            fixed_point base = _spawn_point(spawn_min_distance, spawn_max_distance, true);

            for(int i = 0; i < group; ++i)
            {
                if(two_sides && i == group / 2)
                {
                    base = _spawn_point(spawn_min_distance, spawn_max_distance, false);
                }

                fixed_point p = base + fixed_point(_random.get_int(-12, 13), _random.get_int(-12, 13));

                if(_map.solid_at(to_int(p.x()), to_int(p.y())))
                {
                    p = base;
                }

                // perf (G-52): groups enter through the queue, 2 per frame (5 sprites in one frame cost ~590 ticks).
                int den = s < 300 ? 40 : s < 660 ? 30 : s < 840 ? 20 : 15;   // E2 elites per minute
                if(_stress)
                {
                    _spawn_enemy(_pick_type(), p, den);   // Stress keeps its measured 2 per frame directly
                }
                else
                {
                    int type = _pick_type();

                    if(enemy_defs[type].kind != kind_rise || ! _try_rise(type))
                    {
                        _queue_spawn(type, p, mode_chase, fixed_point(), 0, den);
                    }
                }
            }
        }

        // perf: formations are queued and enter 2 per frame (20 goblins in one frame measured 2173 ticks, 50 %).
        void _queue_spawn(int type, const fixed_point& p, int mode, const fixed_point& vel, int life, int elite_den = 0)
        {
            if(! _spawn_queue.full())
            {
                _spawn_queue.push_back({ p, vel, int16_t(life), uint8_t(type), uint8_t(mode), uint8_t(elite_den) });
            }
        }

        void _spawn_queued()
        {
            int n = bn::min(2, _spawn_queue.size());

            for(int k = 0; k < n; ++k)
            {
                const queued_spawn& q = _spawn_queue[k];
                // Moving formations advance while they wait so the line keeps its shape.
                fixed_point p = q.pos + q.vel * _spawn_queue_age;

                if(enemy* e = _spawn_enemy(q.type, p, q.elite_den))
                {
                    e->mode = q.mode;
                    e->vel = q.vel;
                    e->life = q.life;
                }
            }

            _spawn_queue.erase(_spawn_queue.begin(), _spawn_queue.begin() + n);
            _spawn_queue_age = _spawn_queue.empty() ? 0 : _spawn_queue_age + 1;
        }

        // Splits are queued (from kills inside weapon loops) and spawned here, with a poof and 20 safe frames.
        void _spawn_pending_splits()
        {
            for(const split_request& r : _splits)
            {
                for(int k = 0; k < r.count; ++k)
                {
                    fixed_point p = r.pos + angle_vector(k * 2048 / r.count + 256) * 10;

                    if(enemy* e = _spawn_enemy(r.type, p, 0))
                    {
                        e->safe = 20;
                        e->small = true;
                        e->tiny = r.hp > 0;

                        if(r.hp > 0)
                        {
                            e->hp = r.hp;
                        }
                    }
                }

                _spawn_poof(r.pos, 3);
            }

            _splits.clear();
        }

        void _spawn_treasure()
        {
            if(_treasure_alive || _enemies.full())
            {
                return;
            }

            if(enemy* e = _spawn_enemy(type_treasure, _spawn_point(96, 112, true), 0))
            {
                e->life = 20 * fps;
                _treasure_alive = true;
                test_events = test_events | 1;
                _show_hint("TREASURE GOBLIN!", 2 * fps);
                _play(art::sfx_coin_burst, fixed(0.6));
                bn::sprite_ptr arrow = art::edge_arrow.create_sprite(0, 0, art::arrow_east);
                arrow.set_palette(bn::sprite_palette_item(_gold_arrow_colors, bn::bpp_mode::BPP_4));   // G-55: gold ramp
                arrow.set_bg_priority(0);
                _treasure_arrow = bn::move(arrow);
            }
        }

        void _update_treasure_arrow()
        {
            if(! _treasure_arrow)
            {
                return;
            }

            if(! _treasure_alive)
            {
                _treasure_arrow.reset();
                return;
            }

            for(const enemy& e : _enemies)
            {
                if(e.type == type_treasure)
                {
                    int dx = to_int(e.pos.x() - _pos.x());
                    int dy = to_int(e.pos.y() - _pos.y());
                    _place_edge_arrow(*_treasure_arrow, dx, dy, 0);
                    return;
                }
            }
        }

        // Gold (first 0) or red (first 3) edge arrow towards dx, dy; hidden while the target is on screen.
        void _place_edge_arrow(bn::sprite_ptr& arrow, int dx, int dy, int first)
        {
            bool on_screen = abs_int(dx) < 112 && abs_int(dy) < 72;
            arrow.set_visible(! on_screen && (_t & 16));

            if(on_screen)
            {
                return;
            }

            int adx = abs_int(dx);
            int ady = abs_int(dy);
            int frame = art::arrow_north_east;

            if(adx > ady * 2)
            {
                frame = art::arrow_east;
            }
            else if(ady > adx * 2)
            {
                frame = art::arrow_north;
            }

            arrow.set_tiles(art::edge_arrow.tiles_item(), frame + first);
            arrow.set_horizontal_flip(dx < 0);
            arrow.set_vertical_flip(dy > 0);
            arrow.set_position(bn::max(-110, bn::min(110, dx)), bn::max(-58, bn::min(70, dy)));
        }

        void _begin_swarm_warning(int kind, int frames)
        {
            int side = _random.get_int(4);
            constexpr int dx[4] = { -1, 1, 0, 0 };
            constexpr int dy[4] = { 0, 0, -1, 1 };
            _swarm_kind = kind;
            _swarm_dir = fixed_point(-dx[side], -dy[side]);
            _swarm_origin = fixed_point(dx[side] * 150, dy[side] * 110);
            _swarm_warning_t = frames;
            _swarm_rows_left = kind == swarm_wolves || kind == swarm_crows ? 2 : kind == swarm_stampede ? 3 : 1;
            _warnings.clear();

            if(kind != swarm_wolves && kind != swarm_stampede)
            {
                _play(art::sfx_wave, fixed(0.8));
            }

            for(int i = -1; i <= 1; ++i)
            {
                int wx = dx[side] ? dx[side] * 112 : i * 40;
                int wy = dy[side] ? dy[side] * 64 : i * 30;
                bn::sprite_ptr w = art::alert.create_sprite(wx, wy, 0);
                w.set_bg_priority(0);
                _warnings.push_back(bn::move(w));

                if(kind == swarm_crows)
                {
                    // The second LINE crosses from the perpendicular side (the X).
                    int qx = dy[side] ? -dy[side] * 112 : i * 40;
                    int qy = dx[side] ? dx[side] * 64 : i * 30;
                    bn::sprite_ptr q = art::alert.create_sprite(qx, qy, 0);
                    q.set_bg_priority(0);
                    _warnings.push_back(bn::move(q));
                }
            }
        }

        void _update_swarm_warning()
        {
            bool visible = (_swarm_warning_t & 8) != 0;

            for(bn::sprite_ptr& w : _warnings)
            {
                w.set_tiles(art::alert.tiles_item(), visible ? 0 : 1);
            }
        }

        void _spawn_swarm()
        {
            fixed_point perp(-_swarm_dir.y(), _swarm_dir.x());
            fixed_point origin = _pos + _swarm_origin;
            --_swarm_rows_left;

            if(_swarm_kind == swarm_crows)
            {
                // Murder of Crows (E3): 10 crows in a line at 1.6 px/f; the second row along the perpendicular.
                fixed_point dir = _swarm_rows_left == 1 ? _swarm_dir : fixed_point(_swarm_dir.y(), -_swarm_dir.x());
                fixed_point side(-dir.y(), dir.x());
                fixed_point start = _pos - dir * (dir.x() != 0 ? 150 : 110);
                _make_room(10);
                _swarm_row_t = 1;
                test_f2_events = test_f2_events | 1;

                for(int i = 0; i < 10; ++i)
                {
                    _queue_spawn(type_crow, start + side * ((i - 5) * 16) - dir * (i & 1) * 8, mode_swarm,
                                 dir * fixed(1.6), 300);
                }

                return;
            }

            if(_swarm_kind == swarm_ghosts)
            {
                _make_room(16);
                test_f2_events = test_f2_events | 2;

                for(int i = 0; i < 16; ++i)
                {
                    fixed_point p = origin + fixed_point(_random.get_int(-28, 29), _random.get_int(-28, 29));
                    _queue_spawn(type_ghost, p, mode_chase, fixed_point(), 0);
                }

                return;
            }

            if(_swarm_kind == swarm_stampede && _stage == 1)
            {
                // 14:00 Stampede (E3): rows of knights, brutes, knights straight across at 1.2 px/f.
                _make_room(3);
                test_f2_events = test_f2_events | 16;
                _swarm_row_t = 30;

                for(int i = 0; i < 3; ++i)
                {
                    int type = _swarm_rows_left == 1 ? type_brute : type_knight;

                    if(enemy* e = _spawn_enemy(type, origin + perp * ((i - 1) * 30), 0))
                    {
                        e->mode = mode_swarm;
                        e->vel = _swarm_dir * fixed(1.2);
                        e->life = 300;
                    }
                }

                return;
            }

            if(_swarm_kind == swarm_stampede && _swarm_rows_left == 1)
            {
                // 14:00 Stampede, middle row: 3 orcs straight across at 1.2 px/f.
                _make_room(3);
                test_events = test_events | 2048;
                _swarm_row_t = 20;

                for(int i = 0; i < 3; ++i)
                {
                    if(enemy* e = _spawn_enemy(type_orc, origin + perp * ((i - 1) * 30), 0))
                    {
                        e->mode = mode_swarm;
                        e->vel = _swarm_dir * fixed(1.2);
                        e->life = 300;
                        e->sprite.set_horizontal_flip(_swarm_dir.x() < 0);
                    }
                }

                return;
            }

            if(_swarm_kind == swarm_wolves || _swarm_kind == swarm_stampede)
            {
                // STAMPEDE (E1): rows of 3 dashing wolves, 20 f apart, straight across at 3 px/f.
                _make_room(3);
                test_events = test_events | 4;
                _swarm_row_t = _swarm_kind == swarm_stampede ? 30 : 20;

                for(int i = 0; i < 3; ++i)
                {
                    fixed_point p = origin + perp * ((i - 1) * 30);

                    if(enemy* e = _spawn_enemy(type_wolf, p, 0))
                    {
                        e->mode = mode_swarm;
                        e->vel = _swarm_dir * 3;
                        e->life = 130;
                        e->state = 2;
                        e->timer = 255;
                        e->sprite.set_tiles(art::wolf.item->tiles_item(), art::wolf_dash);
                        e->sprite.set_horizontal_flip(_swarm_dir.x() < 0);
                    }
                }

                return;
            }

            if(_swarm_kind == swarm_goblins)
            {
                // SWARM (E1): a compact group of 20 goblins from one side; they fan out with their flank angle.
                _make_room(20);
                test_events = test_events | 8;

                for(int i = 0; i < 20; ++i)
                {
                    fixed_point p = origin + fixed_point(_random.get_int(-28, 29), _random.get_int(-28, 29));
                    _queue_spawn(type_goblin, p, mode_chase, fixed_point(), 0);
                }

                return;
            }

            _make_room(12);

            for(int i = 0; i < 12; ++i)
            {
                fixed_point p = origin + perp * ((i - 6) * 14) - _swarm_dir * (i & 1) * 10;

                _queue_spawn(type_bat, p, mode_swarm, _swarm_dir * enemy_defs[type_bat].speed, 420);
            }
        }

        void _spawn_bone_ring(int type = type_skeleton, int count = 16)
        {
            _make_room(count);

            for(int i = 0; i < count; ++i)
            {
                fixed_point p = _pos + angle_vector(i * 2048 / count) * 150;

                _queue_spawn(type, p, mode_ring, fixed_point(), 0);
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Weapons (§9)

        [[nodiscard]] int _damage(int base) const
        {
            // §C3: max(1, base x (1 + Might + blessing)); Morrow -15 %.
            int pct = 100 + 5 * _shop[shop_might] + (_blessing == bless_fury ? 30 : 0) -
                      (_setup.character == char_morrow ? 15 : 0);
            return bn::max(1, (base * pct + 50) / 100);
        }

        // Lens of Reach +10 % area per level, Cinda +10 % (§C3, §C4).
        [[nodiscard]] int _area(int radius) const
        {
            return radius * (100 + 10 * _levels[up_lens] + (_setup.character == char_cinda ? 10 : 0)) / 100;
        }

        // Spell Tome: -6 % weapon and Blink cooldown per level (§C3).
        [[nodiscard]] int _cooldown(int base) const
        {
            return base * (100 - 6 * _levels[up_tome]) / 100;
        }

        // ---- Ember Toss (§C3): a ball thrown where the wizard looks lands 64 px away in 40 f, explodes (r 24)
        // and leaves fire for 60 f (2 damage every 10 f).
        void _update_ember()
        {
            int lv = _levels[up_ember];

            for(int k = 0; k < _embers.size();)
            {
                ember_ball& b = _embers[k];
                if(++b.t <= 0)
                {
                    b.sprite.set_visible(false);
                    ++k;
                    continue;
                }

                b.sprite.set_visible(true);
                b.pos += b.vel;
                int height = (bn::lut_sin((b.t * 512) / 40).data() * 18) >> 12;
                b.sprite.set_position(b.pos.x(), b.pos.y() - height);

                if((b.t & 3) == 0)
                {
                    b.sprite.set_tiles(art::fx_ember.tiles_item(), (b.t >> 2) & 1);
                }

                if(b.t >= 40)
                {
                    _explode(b.pos, _area(lv >= 3 ? 32 : 24), _damage(lv >= 4 ? 20 : 14));

                    if(! _fires.full())
                    {
                        bn::sprite_ptr s = _world_sprite(art::fx_fire, b.pos, 0, 28);
                        fire_patch& f = _fires.emplace_back(bn::move(s));
                        f.pos = b.pos;
                        f.life = lv >= 5 ? 120 : 60;
                    }

                    _embers.erase(_embers.begin() + k);
                    continue;
                }

                ++k;
            }

            for(int k = 0; k < _fires.size();)
            {
                fire_patch& f = _fires[k];

                if(--f.life <= 0)
                {
                    _fires.erase(_fires.begin() + k);
                    continue;
                }

                if((f.life % 5) == 0)
                {
                    f.sprite.set_tiles(art::fx_fire.tiles_item(), (f.life / 5) & 3);
                }

                if((f.life % 10) == 0)
                {
                    _area_damage(f.pos, 12, _damage(2), false);
                }

                ++k;
            }

            if(lv == 0 || --_ember_cd > 0)
            {
                return;
            }

            _ember_cd = _cooldown(lv >= 5 ? 100 : 120);
            int balls = lv >= 2 ? 2 : 1;

            for(int i = 0; i < balls && ! _embers.full(); ++i)
            {
                int angle = balls == 1 ? 0 : (i ? 114 : -114);   // +-20 degrees
                fixed_point r = angle_vector(angle);
                fixed_point d(_facing.x() * r.x() - _facing.y() * r.y(), _facing.x() * r.y() + _facing.y() * r.x());
                bn::sprite_ptr s = _world_sprite(art::fx_ember, _pos, 0, 3);
                ember_ball& b = _embers.emplace_back(bn::move(s));
                b.pos = _pos;
                b.vel = d * fixed(1.6);
                b.t = -8 * i;   // perf: the second ball lands 8 f later (two explosions in one frame)
            }
        }

        void _explode(const fixed_point& p, int radius, int damage)
        {
            _spawn_particle(p, fixed_point(), art::fx_explosion, art::explosion_anim,
                            art::explosion_anim.count * art::explosion_anim.frame_time, 4);
            _play(art::sfx_explosion, fixed(0.5));
            _area_damage(p, radius, damage, true);
        }

        // Damages every enemy whose centre is within radius of p (and the boss if its box overlaps).
        void _area_damage(const fixed_point& p, int radius, int damage, bool boss)
        {
            int x = to_int(p.x());
            int y = to_int(p.y());
            int8_t near[max_enemies];
            int count = _grid_query(x, y, radius + 6, near);
            int r2 = (radius + 6) * (radius + 6);

            for(int k = 0; k < count; ++k)
            {
                const enemy& e = _enemies[near[k]];
                int dx = to_int(e.pos.x()) - x;
                int dy = to_int(e.pos.y()) - y;

                if(dx * dx + dy * dy <= r2)
                {
                    _hit_enemy(near[k], damage, p);
                }
            }

            if(! _shots.empty())
            {
                _break_skulls(x, y, radius + 4);
            }

            if(boss && _boss && _boss_state != 3 && _boss_box_hit(x, y, radius / 2))
            {
                _hit_boss(damage);
            }
        }

        // ---- Storm Chain (§C3): bolt to the nearest enemy that jumps to the nearest unhit one within 48 px.
        void _update_chain()
        {
            if(_chain_show > 0 && --_chain_show == 0)
            {
                for(bn::sprite_ptr& s : _chain_sprites)
                {
                    s.set_visible(false);
                }
            }
            else if(_chain_show > 0 && (_chain_show & 1) == 0)
            {
                for(bn::sprite_ptr& s : _chain_sprites)
                {
                    s.set_tiles(art::fx_chain.tiles_item(), (_chain_show >> 1) % 3);
                }
            }

            _chain_step();
            int lv = _levels[up_chain];

            if(lv == 0 || --_chain_cd > 0)
            {
                return;
            }

            _chain_cd = _cooldown(lv >= 5 ? 70 : 100);
            _chain_jumps = (lv >= 4 ? 6 : lv >= 2 ? 4 : 3) + 1;
            _chain_damage = _damage(lv >= 3 ? 12 : 8);
            _chain_from = fixed_point(_pos.x(), _pos.y() - 6);
            _chain_range = 160;
            _chain_hits = 0;
            _chain_sprite_k = 0;

            if(_boss && _boss_state != 3)
            {
                int bdx = to_int(_boss_pos.x() - _pos.x());
                int bdy = to_int(_boss_pos.y() - _pos.y());

                if(bdx * bdx + bdy * bdy < 140 * 140)
                {
                    _chain_segment(_chain_from, _boss_pos, _chain_sprite_k);
                    _chain_from = _boss_pos;
                    _hit_boss(_chain_damage);
                    _chain_range = 48;
                    _chain_show = 10;
                }
            }

            _play(art::sfx_zap, fixed(0.5));
        }

        // perf: one jump per frame (a travelling bolt); all jumps in one frame measured 2471 ticks (56 %).
        void _chain_step()
        {
            if(_chain_jumps <= 0)
            {
                return;
            }

            --_chain_jumps;
            int x = to_int(_chain_from.x());
            int y = to_int(_chain_from.y());
            int range = _chain_range;
            int8_t near[max_enemies];
            int count;

            if(range > 48)
            {
                // First target: plain scan (a 160 px bucket query sorts every enemy; measured slower).
                count = _enemies.size();

                for(int k = 0; k < count; ++k)
                {
                    near[k] = int8_t(k);
                }
            }
            else
            {
                count = _grid_query(x, y, range, near);
            }

            int best = -1;
            int best_d = range * range + 1;

            for(int k = 0; k < count; ++k)
            {
                const enemy& e = _enemies[near[k]];

                if(abs_int(to_int(e.pos.x()) - x) > range || abs_int(to_int(e.pos.y()) - y) > range)
                {
                    continue;
                }

                bool done = e.safe != 0;

                for(int h = 0; h < _chain_hits; ++h)
                {
                    done |= _chain_hit[h] == e.uid;
                }

                int dx = to_int(e.pos.x()) - x;
                int dy = to_int(e.pos.y()) - y;
                int d = dx * dx + dy * dy;

                if(! done && d < best_d)
                {
                    best_d = d;
                    best = near[k];
                }
            }

            if(best < 0)
            {
                _chain_jumps = 0;
                return;
            }

            fixed_point to = _enemies[best].pos;
            _chain_hit[_chain_hits++] = _enemies[best].uid;
            _chain_segment(_chain_from, to, _chain_sprite_k);
            _chain_from = to;
            _chain_range = 48;
            _chain_show = 10;
            _hit_enemy(best, _chain_damage, to);
        }

        // A row of fx_chain sprites between two points (pooled; at most chain_sprites in total).
        void _chain_segment(const fixed_point& a, const fixed_point& b, int& sprite_k)
        {
            int dx = to_int(b.x() - a.x());
            int dy = to_int(b.y() - a.y());
            int len = abs_int(dx) + abs_int(dy);
            int pieces = bn::max(1, bn::min(4, len / 14));

            for(int k = 0; k < pieces && sprite_k < chain_sprites; ++k, ++sprite_k)
            {
                bn::sprite_ptr& s = _chain_sprites[sprite_k];
                s.set_position(a + (b - a) * (2 * k + 1) / (2 * pieces) + fixed_point(0, -4));
                s.set_vertical_flip(k & 1);
                s.set_visible(true);
            }
        }

        void _update_weapons()
        {
            _update_nova();
            _update_nova_ring();
            _update_spark();
            _prof(2);
            _update_runes();
            _prof(3);
            _update_frost();
            {
                bn::timer t;
                _update_ember();
                int e = t.elapsed_ticks_with_restart();
                _update_chain();
                int c = t.elapsed_ticks();
                test_prof_ember = bn::max(int(test_prof_ember), e);
                test_prof_chain = bn::max(int(test_prof_chain), c);
            }
            _prof(4);
            _update_lance();
            _update_stars();
            _update_bolts();
            _prof(5);
        }

        // ---- Moon Lance (§C3): beam where the wizard looks, 120 x 12, 8 f, pierces everything.
        void _update_lance()
        {
            int lv = _levels[up_lance];

            if(_lance_t >= 0)
            {
                ++_lance_t;
                int frame = _lance_t < 2 ? 0 : _lance_t < 6 ? 1 : 2;

                for(bn::sprite_ptr& sp : _lance_sprites)
                {
                    sp.set_tiles(bn::sprite_items::fx_lance.tiles_item(), frame);
                }

                if(_lance_t >= 8)
                {
                    _lance_sprites.clear();
                    _lance_t = -1;
                }
            }

            if(lv == 0 || --_lance_cd > 0)
            {
                return;
            }

            _lance_cd = _cooldown(lv >= 5 ? 60 : 90);
            int damage = _damage(lv >= 2 ? 15 : 10);
            int width = _area(lv >= 3 ? 18 : 12);
            bool behind = lv >= 4;

            // 8 directions from the facing vector.
            fixed fx = _facing.x();
            fixed fy = _facing.y();
            int sx = fx > fixed(0.38) ? 1 : fx < fixed(-0.38) ? -1 : 0;
            int sy = fy > fixed(0.38) ? 1 : fy < fixed(-0.38) ? -1 : 0;

            if(sx == 0 && sy == 0)
            {
                sx = 1;
            }

            constexpr int angles[3][3] = { { 135, 90, 45 }, { 180, 0, 0 }, { 225, 270, 315 } };
            int angle = angles[sy + 1][sx + 1];
            fixed_point dir = sx && sy ? fixed_point(fixed(sx) * diag, fixed(sy) * diag) : fixed_point(sx, sy);

            if(! _lance_mat)
            {
                _lance_mat = bn::sprite_affine_mat_ptr::create();
                _lance_mat_back = bn::sprite_affine_mat_ptr::create();
            }

            _lance_mat->set_rotation_angle(angle);
            _lance_mat->set_vertical_scale(fixed(width) / 12);
            _lance_mat_back->set_rotation_angle((angle + 180) % 360);
            _lance_mat_back->set_vertical_scale(fixed(width) / 12);
            _lance_sprites.clear();

            for(int side = 0; side < (behind ? 2 : 1); ++side)
            {
                for(int k = 0; k < 4; ++k)
                {
                    fixed_point c = _pos + fixed_point(0, -4) + dir * (side ? -(16 + 32 * k) : 16 + 32 * k);
                    bn::optional<bn::sprite_ptr> sp = bn::sprite_items::fx_lance.create_sprite_optional(c, 0);

                    if(! sp)
                    {
                        break;
                    }

                    sp->set_camera(_camera);
                    sp->set_bg_priority(2);
                    sp->set_z_order(3);
                    sp->set_affine_mat(side ? *_lance_mat_back : *_lance_mat);
                    _lance_sprites.push_back(bn::move(*sp));
                }
            }

            _lance_t = 0;
            _play(art::sfx_spark, fixed(0.4), fixed(0.7));
            int half = width / 2 + enemy_half_box;

            for(int i = _enemies.size() - 1; i >= 0; --i)
            {
                const enemy& e = _enemies[i];
                fixed_point d = e.pos - _pos;
                int along = to_int(d.x() * dir.x() + d.y() * dir.y());
                int perp = abs_int(to_int(d.x() * dir.y() - d.y() * dir.x()));

                if(perp <= half && along <= 128 && along >= (behind ? -128 : 0))
                {
                    test_lance_hits = test_lance_hits + 1;
                    _hit_enemy(i, damage, _pos);
                }
            }

            if(_boss && _boss_state != 3)
            {
                for(int k = (behind ? -7 : 0); k <= 7; ++k)
                {
                    fixed_point q = _pos + dir * (k * 16);

                    if(_boss_box_hit(to_int(q.x()), to_int(q.y()), width / 2))
                    {
                        _hit_boss(damage);
                        break;
                    }
                }
            }
        }

        // ---- Starfall (§C3 evolution): a star falls every 30 f on an enemy on screen (20 damage, r 12).
        void _update_stars()
        {
            if(! (_evolved & evo_starfall) || --_star_cd > 0)
            {
                return;
            }

            _star_cd = 30;
            int n = _enemies.size();

            if(n == 0)
            {
                return;
            }

            int start = _random.get_int(n);

            for(int k = 0; k < n; ++k)
            {
                const enemy& e = _enemies[(start + k) % n];
                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());

                if(abs_int(dx) < 112 && abs_int(dy) < 72)
                {
                    fixed_point p = e.pos;
                    _spawn_particle(p + fixed_point(0, -6), fixed_point(), bn::sprite_items::fx_star, { 0, 4, 3 }, 12, 3);
                    _area_damage(p, 12, _damage(20), true);
                    break;
                }
            }
        }

        // Evolution (§C3): weapon Nv5 + its passive; called when a boss or elite chest opens. True if one happened.
        bool _try_evolve()
        {
            struct recipe
            {
                int weapon;
                int passive;
                int bit;
                const char* text;
            };

            constexpr recipe recipes[3] = {
                { up_spark, up_tome, evo_starfall, "EVOLVED: STARFALL" },
                { up_frost, up_lens, evo_blizzard, "EVOLVED: BLIZZARD" },
                { up_rune, up_ward, evo_ward_of_ages, "EVOLVED: WARD OF AGES" },
            };

            for(const recipe& r : recipes)
            {
                if(! (_evolved & r.bit) && _levels[r.weapon] >= max_upgrade_level && _levels[r.passive] > 0)
                {
                    _evolved |= r.bit;
                    test_evolved = _evolved;
                    _show_banner(r.text, art::text_gold, 2 * fps);
                    _play(bn::sound_items::sfx_evolve);
                    _level_flash = 30;
                    _evolve_flash = 30;
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] int _upgrade_icon(int id) const
        {
            if((id == up_spark && (_evolved & evo_starfall)) || (id == up_frost && (_evolved & evo_blizzard)) ||
               (id == up_rune && (_evolved & evo_ward_of_ages)))
            {
                return id == up_spark ? 13 : id == up_frost ? 14 : 15;
            }

            return upgrade_texts[id].icon;
        }

        void _update_spark()
        {
            int lv = _levels[up_spark];

            if(lv == 0 || --_spark_cd > 0)
            {
                return;
            }

            bool starfall = _evolved & evo_starfall;
            _spark_cd = _cooldown(starfall ? 36 : lv >= 5 ? 40 : 50);
            _play(art::sfx_spark, fixed(0.3));   // one per burst, not per spark (audio/LEEME.md)
            int count = starfall ? 4 : lv >= 4 ? 3 : lv >= 2 ? 2 : 1;
            int damage = _damage(starfall ? 10 : lv >= 3 ? 8 : 5);
            int pierce = starfall ? 3 : lv >= 5 ? 2 : 0;

            // Nearest distinct targets.
            int best_i[4] = { -1, -1, -1, -1 };
            int best_d[4] = { 1 << 30, 1 << 30, 1 << 30, 1 << 30 };
            constexpr int range2 = 200 * 200;

            for(int i = 0, n = _enemies.size(); i < n; ++i)
            {
                int dx = to_int(_enemies[i].pos.x() - _pos.x());
                int dy = to_int(_enemies[i].pos.y() - _pos.y());
                int d = dx * dx + dy * dy;

                if(d > range2)
                {
                    continue;
                }

                for(int k = 0; k < count; ++k)
                {
                    if(d < best_d[k])
                    {
                        for(int m = count - 1; m > k; --m)
                        {
                            best_d[m] = best_d[m - 1];
                            best_i[m] = best_i[m - 1];
                        }

                        best_d[k] = d;
                        best_i[k] = i;
                        break;
                    }
                }
            }

            bool boss_target = _boss && _boss_state != 3;

            for(int k = 0; k < count; ++k)
            {
                fixed_point dir;

                if(boss_target && k == 0)
                {
                    dir = unit(to_int(_boss_pos.x() - _pos.x()), to_int(_boss_pos.y() - _pos.y()));
                }
                else if(best_i[k] >= 0)
                {
                    const enemy& e = _enemies[best_i[k]];
                    dir = unit(to_int(e.pos.x() - _pos.x()), to_int(e.pos.y() - _pos.y()));
                }
                else if(best_i[0] >= 0)
                {
                    const enemy& e = _enemies[best_i[0]];
                    fixed_point r = angle_vector(k * 90);
                    fixed_point d0 = unit(to_int(e.pos.x() - _pos.x()), to_int(e.pos.y() - _pos.y()));
                    dir = fixed_point(d0.x() * r.x() - d0.y() * r.y(), d0.x() * r.y() + d0.y() * r.x());
                }
                else
                {
                    // No target: the spark still flies where the wizard looks (§5 0:01).
                    fixed_point r = angle_vector((k - (count - 1) / 2) * 80);
                    dir = fixed_point(_facing.x() * r.x() - _facing.y() * r.y(), _facing.x() * r.y() + _facing.y() * r.x());
                }

                if(_bolts.full())
                {
                    break;
                }

                // perf: creating a sprite costs ~5-10 % of a frame with 150 sprites alive; reuse hidden ones.
                bn::optional<bn::sprite_ptr> s;

                if(! _bolt_pool.empty())
                {
                    s = bn::move(_bolt_pool.back());
                    _bolt_pool.pop_back();
                    s->set_position(_pos.x(), _pos.y() - 6);
                    s->set_visible(true);
                }
                else
                {
                    s = art::fx_spark.create_sprite_optional(_pos.x(), _pos.y() - 6, 0);

                    if(! s)
                    {
                        break;
                    }

                    s->set_camera(_camera);
                    s->set_bg_priority(2);
                    s->set_z_order(3);
                }

                bolt& b = _bolts.emplace_back(bn::move(*s));
                b.pos = fixed_point(_pos.x(), _pos.y() - 6);
                b.vel = dir * 3;
                b.life = 70;
                b.damage = damage;
                b.pierce = pierce;
            }
        }

        void _update_bolts()
        {
            for(int bi = 0; bi < _bolts.size();)
            {
                bolt& b = _bolts[bi];
                b.pos += b.vel;
                bool dead = --b.life <= 0;
                int bx = to_int(b.pos.x());
                int by = to_int(b.pos.y());

                int8_t near[max_enemies];
                int near_count = dead ? 0 : _grid_query(bx, by, 14, near);

                for(int k = 0; k < near_count && ! dead; ++k)
                {
                    int i = near[k];
                    enemy& e = _enemies[i];
                    int dx = to_int(e.pos.x()) - bx;
                    int dy = to_int(e.pos.y()) - by;

                    int reach = (e.tiny ? enemy_defs[e.type].half / 2 : enemy_defs[e.type].half) + 3;

                    if(abs_int(dx) < reach && abs_int(dy) < reach)
                    {
                        if(b.last_hit[0] == e.uid || b.last_hit[1] == e.uid || b.last_hit[2] == e.uid)
                        {
                            continue;
                        }

                        uint16_t uid = e.uid;
                        _spawn_hit_spark(b.pos);
                        _hit_enemy(i, b.damage, b.pos);

                        if(b.pierce > 0)
                        {
                            b.last_hit[b.pierce % 3] = uid;
                            --b.pierce;
                        }
                        else
                        {
                            dead = true;
                        }
                    }
                }

                if(! dead && _boss && _boss_state != 3 && _boss_box_hit(bx, by, 3))
                {
                    _hit_boss(b.damage);
                    _spawn_hit_spark(b.pos);
                    dead = true;
                }

                if(dead)
                {
                    b.sprite.set_visible(false);
                    _bolt_pool.push_back(bn::move(b.sprite));

                    if(bi != _bolts.size() - 1)
                    {
                        _bolts[bi] = bn::move(_bolts.back());
                    }

                    _bolts.pop_back();
                }
                else
                {
                    b.sprite.set_position(b.pos);
                    ++bi;
                }
            }
        }

        void _update_runes()
        {
            int lv = _levels[up_rune];

            if(lv == 0)
            {
                return;
            }

            bool ages = _evolved & evo_ward_of_ages;
            int count = ages ? 6 : lv >= 4 ? 4 : lv >= 2 ? 3 : 2;
            int radius = _area(ages ? 40 : lv >= 3 ? 36 : 28);
            bool always = ages || lv >= 5;
            ++_rune_timer;

            if(_rune_timer >= 300)
            {
                _rune_timer = 0;
            }

            bool active = always || _rune_timer < 180;

            if(! active)
            {
                // perf: hidden, not destroyed (re-creating them costs more than the whole rune update).
                for(bn::sprite_ptr& rune : _runes)
                {
                    rune.set_visible(false);
                }

                return;
            }

            while(_runes.size() < count)
            {
                bn::optional<bn::sprite_ptr> s = art::fx_rune.create_sprite_optional(0, 0, 0);

                if(! s)
                {
                    break;
                }

                s->set_camera(_camera);
                s->set_bg_priority(2);
                s->set_z_order(3);
                _runes.push_back(bn::move(*s));
            }

            int base = (_t * 2048) / 120;
            int damage = _damage(ages ? 8 : 4);

            if(ages && (_t & 7) == 0)
            {
                for(bn::sprite_ptr& rune : _runes)
                {
                    rune.set_tiles(art::fx_rune.tiles_item(), 2 + ((_t >> 3) & 1));   // gold runes (art/LEEME.md)
                }
            }

            for(int r = 0, rn = _runes.size(); r < rn; ++r)
            {
                fixed_point p = _pos + fixed_point(0, -4) + angle_vector(base + (r * 2048) / rn) * radius;
                _runes[r].set_position(p);
                _runes[r].set_visible(true);
                int rx = to_int(p.x());
                int ry = to_int(p.y());

                int8_t near[max_enemies];
                int near_count = _grid_query(rx, ry, enemy_half_box + 4, near);

                for(int k = 0; k < near_count; ++k)
                {
                    int i = near[k];
                    enemy& e = _enemies[i];

                    if(e.rune_cd)
                    {
                        continue;
                    }

                    int dx = to_int(e.pos.x()) - rx;
                    int dy = to_int(e.pos.y()) - ry;

                    if(abs_int(dx) < enemy_half_box + 4 && abs_int(dy) < enemy_half_box + 4)
                    {
                        e.rune_cd = 20;

                        if(_hit_enemy(i, damage, p) && ages && ++_ward_kills >= 25)
                        {
                            _ward_kills = 0;   // Ward of Ages (§C3): every 25 rune kills heal 2 HP
                            _player_hp = bn::min(_player_max_hp, _player_hp + 2);
                        }
                    }
                }

                if(_boss && _boss_state != 3 && _boss_rune_cd == 0 && _boss_box_hit(rx, ry, 4))
                {
                    _boss_rune_cd = 20;
                    _hit_boss(damage);
                }
            }
        }

        void _update_frost()
        {
            int lv = _levels[up_frost];

            if(lv == 0)
            {
                return;
            }

            if(_frost_t < 0)
            {
                if(--_frost_cd > 0)
                {
                    return;
                }

                _frost_cd = _cooldown((_evolved & evo_blizzard) ? 90 : lv >= 4 ? 110 : 150);
                _frost_t = 0;
                ++_frost_pulse_id;

                if(_frost_pulse_id == 0)
                {
                    _frost_pulse_id = 1;
                }
            }

            bool blizzard = _evolved & evo_blizzard;
            int max_r = _area(blizzard ? 96 : lv >= 3 ? 72 : 56);
            int r = (max_r * (_frost_t + 1)) / 24;
            int damage = _damage(blizzard ? 14 : lv >= 2 ? 9 : 6);

            _ensure_frost_shards();
            int frost_frame = bn::min(2, _frost_t / 8);

            for(int i = 0, n = _frost.size(); i < n; ++i)
            {
                _frost[i].set_position(_pos + angle_vector(i * 256 + _frost_t * 8) * r);

                if(_frost_t == 0)
                {
                    _frost[i].set_visible(true);
                }

                if(_frost_t % 8 == 0)
                {
                    _frost[i].set_tiles(art::fx_frost.tiles_item(), frost_frame);
                }
            }

            int r2 = r * r;

            for(int i = 0, n = _enemies.size(); i < n; ++i)
            {
                enemy& e = _enemies[i];

                if(e.frost_pulse == _frost_pulse_id)
                {
                    continue;
                }

                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());

                if(dx * dx + dy * dy <= r2)
                {
                    e.frost_pulse = _frost_pulse_id;
                    e.slow = 90;

                    if(lv >= 5)
                    {
                        e.freeze = blizzard ? 60 : 45;
                    }

                    if(_hit_enemy(i, damage, _pos))
                    {
                        --n;
                        --i;
                    }
                }
            }

            if(_boss && _boss_state != 3 && _boss_frost_pulse != _frost_pulse_id)
            {
                int dx = to_int(_boss_pos.x() - _pos.x());
                int dy = to_int(_boss_pos.y() - _pos.y());

                if(dx * dx + dy * dy <= (r + 20) * (r + 20))
                {
                    _boss_frost_pulse = _frost_pulse_id;
                    _hit_boss(damage);
                }
            }

            if(++_frost_t >= 24)
            {
                // perf: hidden, not destroyed; the next pulse shows them again.
                _frost_t = -1;

                for(bn::sprite_ptr& shard : _frost)
                {
                    shard.set_visible(false);
                }
            }
        }

        // perf (G-52): spark sprites for ~3 volleys in flight are created when Spark is levelled (a menu frame);
        // creating them on the volley frame measured ~400 ticks each with the horde alive.
        void _ensure_bolt_sprites()
        {
            int lv = _levels[up_spark];
            int count = lv >= 4 ? 3 : lv >= 2 ? 2 : 1;
            int wanted = bn::min(max_bolts, 3 * count);

            while(_bolts.size() + _bolt_pool.size() < wanted)
            {
                bn::optional<bn::sprite_ptr> s = art::fx_spark.create_sprite_optional(0, 0, 0);

                if(! s)
                {
                    break;
                }

                s->set_camera(_camera);
                s->set_bg_priority(2);
                s->set_z_order(3);
                s->set_visible(false);
                _bolt_pool.push_back(bn::move(*s));
            }
        }

        // perf (G-52): the shards are created when Frost Ring is picked (a menu frame), not on its first pulse.
        void _ensure_frost_shards()
        {
            while(_frost.size() < frost_shards)
            {
                bn::optional<bn::sprite_ptr> s = art::fx_frost.create_sprite_optional(0, 0, 0);

                if(! s)
                {
                    break;
                }

                s->set_camera(_camera);
                s->set_bg_priority(2);
                s->set_z_order(3);
                s->set_blending_enabled(_fx.frost_translucent);
                s->set_visible(false);
                _frost.push_back(bn::move(*s));
            }
        }

        // Returns true if the enemy died (it was removed from _enemies; index i now holds another enemy).
        bool _hit_enemy(int i, int damage, const fixed_point& from)
        {
            enemy& e = _enemies[i];

            if(e.safe || e.ghostly)
            {
                return false;
            }

            if((_evolved & evo_blizzard) && e.freeze > 0)
            {
                damage = damage * 3 / 2;   // Blizzard (§C3): frozen monsters take +50 % from any weapon
            }

            if(e.shield > 0)
            {
                --e.shield;
                e.flash = hit_flash_frames;

                if(_fx.hit_flash)
                {
                    _flash_white(e);
                }

                if(e.shield == 0)
                {
                    e.elite = 0;
                    _restore_palette(e);
                    e.elite = 2;
                }

                return false;
            }

            e.hp -= damage;

            if(_hit_sound_cd == 0)
            {
                _play(art::sfx_hit, fixed(0.6));
                _hit_sound_cd = 4;
            }

            if(_fx.damage_numbers)
            {
                _spawn_damage_number(e.pos, damage);
            }

            if(e.hp <= 0)
            {
                _kill_enemy(i);
                return true;
            }

            e.flash = hit_flash_frames;

            if(_fx.hit_flash)
            {
                _flash_white(e);
            }

            if(_fx.knockback && e.mode != mode_swarm)
            {
                e.knock = knockback_frames;
                e.knock_vel = unit(to_int(e.pos.x() - from.x()), to_int(e.pos.y() - from.y())) * knockback_speed;

                if(enemy_defs[e.type].kind == kind_tank)
                {
                    e.knock_vel = e.knock_vel / 4;   // TANK: knockback x0.25 (§C1)
                }
            }

            return false;
        }

        void _kill_enemy(int i)
        {
            bn::timer kill_timer;
            _kill_enemy_impl(i);
            test_prof[12] = test_prof[12] + kill_timer.elapsed_ticks();
            test_prof[13] = test_prof[13] + 1;
        }

        void _kill_enemy_impl(int i)
        {
            enemy& e = _enemies[i];
            fixed_point p = e.pos;
            ++_kills;
            _add_nova(2);

            const art::monster_art& look = *enemy_defs[e.type].look;
            const art::anim& death = look.death;
            if(_create_budget-- > 0)
            {
                int before = _particles.size();
                _spawn_particle(fixed_point(p.x(), p.y() + look.y_offset), fixed_point(), *look.item, death,
                                death.count * death.frame_time, 14, e.sprite.horizontal_flip());

                if(_particles.size() > before && enemy_defs[e.type].pal && _type_pal[e.type])
                {
                    // Palette variants die in their own colours (the art's palette would cost one more of 16).
                    _particles.back().sprite.set_palette(*_type_pal[e.type]);
                }
            }

            if(_fx.death_poof)
            {
                _spawn_poof(p, 3);
            }

            const enemy_def& def = enemy_defs[e.type];
            int value = def.xp;

            if(def.xp5_pct && _random.get_int(100) < def.xp5_pct)
            {
                value = 5;
            }

            if(e.elite)
            {
                value = 25;
            }

            if(value > 0)
            {
                _spawn_gem(p, value);
            }

            if(e.type == type_treasure)
            {
                // §C2: 30 + rnd(31) gold and one heart.
                _spawn_pickup(p + fixed_point(-6, 0), pickup_coin, 30 + _random.get_int(31), gold_treasure);
                _spawn_pickup(p + fixed_point(6, 0), pickup_heart, 30);
                _treasure_alive = false;
                test_events = test_events | 512;
                _play(art::sfx_coin_burst);
            }
            else if(e.elite)
            {
                _spawn_pickup(p + fixed_point(8, 0), pickup_chest, 1);
                _spawn_pickup(p + fixed_point(-8, 0), pickup_coin, 5);

                if(_random.get_int(4) == 0)
                {
                    _spawn_heart(p + fixed_point(0, 8));
                }
            }
            else if(_random.get_int(50) == 0)
            {
                _spawn_pickup(p + fixed_point(6, 4), pickup_coin, 1);
            }
            else if(_random.get_int(250) == 0)
            {
                _spawn_heart(p + fixed_point(-6, 4));
            }

            // SPLIT (big slime: 2 slimes) and the Splitting elite (3 copies, 30 % HP), queued (§C1, §C2).
            if(! _splits.full())
            {
                if(e.elite == 3 && ! e.small)
                {
                    _splits.push_back({ p, e.type, 3, bn::max(1, scaled_hp(def.hp, _t / (60 * fps)) * 3 * 3 / 10) });
                    test_events = test_events | 2;
                }
                else if(def.splits && ! e.small)
                {
                    _splits.push_back({ p, type_slime, def.splits, 0 });
                }
                else if(e.elite == 4)
                {
                    _splits.push_back({ p, type_ghost, 1, 0 });   // Haunted (§C2): one Ghost
                    test_f2_events = test_f2_events | 16384;
                }
            }

            _remove_enemy(i);
        }

        // -----------------------------------------------------------------------------------------------------
        // Enemies

        // Shared horde frame (§C6): advances every art's animation and retargets its direction blocks.
        void _update_horde_frames()
        {
            // perf (G-52): every art stepping on the same frame cost 632 ticks (14 % of a frame) in profile C. Arts
            // are phase-shifted, skipped while none of their monsters is alive, and at most 2 retarget per frame
            // (one that is due waits a frame).
            bool alive[max_horde_arts] = {};

            for(int t = 0; t < type_count; ++t)
            {
                if(_type_alive[t])
                {
                    alive[_type_slot[t]] = true;
                }
            }

            int budget = 2;

            for(int k = 0; k < _horde_art_count && budget > 0; ++k)
            {
                int a = _horde_next + k;
                a = a >= _horde_art_count ? a - _horde_art_count : a;
                const art::monster_art& look = *_horde_looks[a];
                int f = ((_t + a * 3) / look.move[0].frame_time) % look.move[0].count;

                if(alive[a] && f != _horde_frame[a])
                {
                    --budget;
                    _horde_frame[a] = f;
                    bn::vector<bn::sprite_tiles_ptr, art::drawn_dirs>& blocks = _horde_tiles[a];

                    for(int d = 0, n = blocks.size(); d < n; ++d)
                    {
                        blocks[d].set_tiles_ref(look.item->tiles_item(), look.move[d].first + f);
                    }
                }
            }

            _horde_next = _horde_next + 1 >= _horde_art_count ? 0 : _horde_next + 1;

            int used = bn::sprite_tiles::used_tiles_count();

            if(used > test_tiles_used)
            {
                test_tiles_used = used;
            }
        }

        HB_HOT void _update_enemies()
        {
            _crouching = _crouching_now;
            _crouching_now = 0;

            if(_wolf_pack_cd > 0)
            {
                --_wolf_pack_cd;
            }
            int px = to_int(_pos.x());
            int py = to_int(_pos.y());
            int n = _enemies.size();

            for(int i = 0; i < n;)
            {
                enemy& e = _enemies[i];
                const enemy_def& def = enemy_defs[e.type];
                int ex = to_int(e.pos.x());
                int ey = to_int(e.pos.y());
                int dx = px - ex;
                int dy = py - ey;

                if(e.rune_cd)
                {
                    --e.rune_cd;
                }

                if(e.flash && --e.flash == 0)
                {
                    _restore_palette(e);
                }

                if(e.mode == mode_swarm)
                {
                    e.pos += e.vel;

                    if(--e.life <= 0)
                    {
                        _remove_enemy(i);
                        --n;
                        continue;
                    }
                }
                else
                {
                    if(e.cd)
                    {
                        e.cd -= (_t & 1);
                    }

                    if(e.safe)
                    {
                        --e.safe;
                    }

                    if(def.kind == kind_flee && --e.life <= 0)
                    {
                        _spawn_poof(e.pos, 3);
                        _treasure_alive = false;
                        _remove_enemy(i);
                        --n;
                        continue;
                    }

                    if(e.mode == mode_chase && def.kind != kind_flee && abs_int(dx) + abs_int(dy) > recycle_distance)
                    {
                        e.pos = _spawn_point(spawn_min_distance, spawn_max_distance, true);
                        _grid_refresh(i);
                        ++i;
                        continue;
                    }

                    if(e.mode == mode_ring && abs_int(dx) + abs_int(dy) < 70)
                    {
                        e.mode = mode_chase;
                    }

                    fixed_point step;

                    if(e.knock)
                    {
                        --e.knock;
                        step = e.knock_vel;
                    }
                    else if(e.freeze > 0)
                    {
                        --e.freeze;
                    }
                    else if(def.kind == kind_dash && e.state != 0)
                    {
                        // DASH (§C1): crouch 30 f with red eyes (warning), then 20 f straight at 3 px/f.
                        step = e.state == 2 ? e.vel : fixed_point();

                        if(e.state == 1)
                        {
                            ++_crouching_now;

                            if((e.timer & 3) == 0)
                            {
                                if(e.timer & 4)
                                {
                                    // perf: one red palette held while wolves live (creating it per blink cost ~30 %).
                                    if(! _wolf_red_pal)
                                    {
                                        _wolf_red_pal = bn::sprite_palette_item(_wolf_red_colors,
                                                                                bn::bpp_mode::BPP_4).create_palette();
                                    }

                                    e.sprite.set_palette(*_wolf_red_pal);
                                }
                                else
                                {
                                    _restore_palette(e);
                                }
                            }
                        }

                        if(--e.timer == 0)
                        {
                            if(e.state == 1)
                            {
                                e.state = 2;
                                e.timer = 20;
                                _restore_palette(e);
                                e.sprite.set_tiles(def.look->item->tiles_item(), art::wolf_dash);
                                test_wolf_dashes = test_wolf_dashes + 1;
                            }
                            else
                            {
                                e.state = 0;
                                e.cd = 210;   // 420 f (G-55 lead decision; 300 f gave 41 dash hits in 8:00-9:00)
                                e.vel = fixed_point();
                                e.sprite.set_tiles(_horde_tiles[_type_slot[e.type]][0]);
                            }
                        }
                    }
                    else
                    {
                        if(((_t + e.uid) & 3) == 0 || (e.vel.x() == 0 && e.vel.y() == 0))
                        {
                            fixed speed = e.mode == mode_ring ? fixed(0.4) : def.speed;

                            if(e.elite == 1)
                            {
                                speed *= fixed(1.6);
                            }

                            fixed_point to = unit(dx, dy);
                            int dist = abs_int(dx) + abs_int(dy);

                            if(def.kind == kind_flee)
                            {
                                // Flees; tires when far so a chase stays possible (PROVISIONAL tuning).
                                to = fixed_point(-to.x(), -to.y());
                                speed = dist > 170 ? fixed(0.6) : speed;
                            }
                            else if(def.kind == kind_shoot && e.mode == mode_chase)
                            {
                                // SHOOT keeps 72-96 px (Manhattan ~84-120): backs off, stands, closes in.
                                if(dist < 84)
                                {
                                    to = fixed_point(-to.x(), -to.y());
                                    speed = speed * fixed(0.8);
                                }
                                else if(dist <= 120)
                                {
                                    speed = 0;
                                }
                            }
                            else if((e.type == type_goblin || e.type == type_ghoul) && dist > 56)
                            {
                                fixed_point r = angle_vector(e.flank ? 171 : -171);   // +-30 degrees
                                to = fixed_point(to.x() * r.x() - to.y() * r.y(), to.x() * r.y() + to.y() * r.x());
                            }

                            e.vel = to * speed;
                        }

                        step = e.vel;

                        if(def.kind == kind_hop && ((_t + e.uid * 7) & 31) >= 20)
                        {
                            // Slimes hop: still for 12 of every 32 frames, faster in between (avg = speed).
                            step = fixed_point();
                        }
                        else if(def.kind == kind_hop)
                        {
                            step = step * fixed(1.6);
                        }

                        if(def.kind == kind_dash && e.cd == 0 && e.mode == mode_chase && _crouching < wolf_max_crouching && _wolf_pack_cd == 0 &&
                           abs_int(dx) <= 100 && abs_int(dy) <= 60 && dx * dx + dy * dy <= 96 * 96)
                        {
                            // Direction fixed when the warning starts; only on screen (the crouch is the warning).
                            e.state = 1;
                            e.timer = 30;
                            ++_crouching;
                            _wolf_pack_cd = wolf_pack_gap;
                            e.vel = unit(dx, dy) * 3;
                            e.sprite.set_tiles(def.look->item->tiles_item(), art::wolf_crouch);
                            e.sprite.set_horizontal_flip(dx < 0);
                            test_wolf_crouches = test_wolf_crouches + 1;

                            if(test_first_crouch_frame == 0)
                            {
                                test_first_crouch_frame = test_frames;
                            }
                        }

                        if(e.slow > 0)
                        {
                            --e.slow;
                            step = step / 2;
                        }
                    }

                    if(def.flying)
                    {
                        e.pos += step;
                    }
                    else
                    {
                        if(step.x() != 0)
                        {
                            fixed nx = e.pos.x() + step.x();

                            if(! _map.solid_at(to_int(nx), ey))
                            {
                                e.pos.set_x(nx);
                            }
                        }

                        if(step.y() != 0)
                        {
                            fixed ny = e.pos.y() + step.y();

                            if(! _map.solid_at(to_int(e.pos.x()), to_int(ny)))
                            {
                                e.pos.set_y(ny);
                            }
                        }
                    }
                }

                if(def.kind == kind_shoot && e.mode == mode_chase && ! e.knock)
                {
                    _update_archer(e, dx, dy);
                }
                else if(def.kind == kind_phase)
                {
                    // PHASE (§C1): 60 f of every 240 intangible at 50 % alpha (neither deals nor takes damage).
                    bool ghostly = ((_t + e.uid * 53) % 240) >= 180;

                    if(ghostly != e.ghostly)
                    {
                        e.ghostly = ghostly;
                        e.sprite.set_blending_enabled(ghostly);
                    }

                    if(def.blinks && ((_t + e.uid * 31) % 300) == 0 && abs_int(dx) + abs_int(dy) < 200)
                    {
                        _begin_blink(e);
                    }
                }

                // Separation (§13): pushed away from up to 3 overlapping neighbours of the nearby buckets, so a
                // crowd spreads into a ring instead of one ball. Swarm bats keep their formation.
                if(e.mode != mode_swarm)
                {
                    int ex2 = to_int(e.pos.x());
                    int ey2 = to_int(e.pos.y());
                    int pushes = 0;
                    int probes = 0;
                    fixed_point push;
                    int cx0 = (ex2 - separation_distance) >> grid_shift;
                    int cy0 = (ey2 - separation_distance) >> grid_shift;
                    int cx1 = (ex2 + separation_distance) >> grid_shift;
                    int cy1 = (ey2 + separation_distance) >> grid_shift;

                    // Bounded work in a dense crowd: at most 3 pushes and 10 neighbours looked at.
                    for(int cy = cy0; cy <= cy1 && pushes < 3 && probes < 10; ++cy)
                    {
                        for(int cx = cx0; cx <= cx1 && pushes < 3 && probes < 10; ++cx)
                        {
                            int cell = (cy & (grid_size - 1)) * grid_size + (cx & (grid_size - 1));

                            for(int j = _grid_head[cell]; j >= 0 && pushes < 3 && probes < 10; j = _grid_next[j])
                            {
                                if(j == i)
                                {
                                    continue;
                                }

                                ++probes;
                                const enemy& o = _enemies[j];
                                int sx = ex2 - to_int(o.pos.x());
                                int sy = ey2 - to_int(o.pos.y());

                                if(abs_int(sx) < separation_distance && abs_int(sy) < separation_distance)
                                {
                                    // Identical spots split by uid so two stacked enemies do not move together.
                                    fixed side = e.uid > o.uid ? separation_push : -separation_push;
                                    push += fixed_point(sx > 0 ? separation_push : sx < 0 ? -separation_push : side,
                                                        sy > 0 ? separation_push : sy < 0 ? -separation_push : side);
                                    ++pushes;
                                }
                            }
                        }
                    }

                    if(pushes)
                    {
                        fixed nx = e.pos.x() + push.x();
                        fixed ny = e.pos.y() + push.y();

                        if(def.flying || ! _map.solid_at(to_int(nx), to_int(ny)))
                        {
                            e.pos = fixed_point(nx, ny);
                        }
                    }
                }

                _grid_refresh(i);

                // Contact damage (§7); the body box grows with the monster (32x32: 11 px half).
                // Splitting copies are 50 % of the logical size (DISENO_CONTENIDO élites).
                int reach = player_half_box + (e.tiny ? def.half / 2 : def.half) - 1;

                if(def.damage && ! e.safe && ! e.ghostly && abs_int(dx) < reach && abs_int(dy) < reach)
                {
                    if(def.kind == kind_dash && e.state == 2 && _invulnerable == 0)
                    {
                        test_wolf_dash_hits = test_wolf_dash_hits + 1;
                    }

                    _hurt_player(def.damage);
                }

                fixed_point draw(e.pos.x(), e.pos.y() + def.look->y_offset);

                if(e.type == type_bat || e.type == type_crow || def.kind == kind_phase)
                {
                    draw.set_y(draw.y() + bn::lut_sin(((_t + e.uid * 37) * 16) & 2047) * 8);
                }

                e.sprite.set_position(draw);

                const art::monster_art& look = *def.look;
                bool retile = false;

                if(look.walk_dirs > 1)
                {
                    // Turns towards the wizard one 45 degree step every 4 frames.
                    if(((_t + e.uid) & 3) == 0)
                    {
                        int target = def.kind == kind_flee ?
                                    dir_towards(to_int(e.vel.x() * 16), to_int(e.vel.y() * 16), e.dir) :
                                    dir_towards(dx, dy, e.dir);

                        if(target != e.dir)
                        {
                            e.dir = uint8_t(turn_step(e.dir, target));
                            e.sprite.set_horizontal_flip(art::dir_flipped(e.dir));
                            retile = true;
                        }
                    }
                }
                else if(def.kind == kind_flee || (def.kind == kind_dash && e.state != 0))
                {
                    e.sprite.set_horizontal_flip(e.vel.x() < 0);
                }
                else
                {
                    e.sprite.set_horizontal_flip(dx < 0);
                }

                if(retile)
                {
                    e.sprite.set_tiles(_horde_tiles[_type_slot[e.type]][art::drawn_dir(e.dir)]);
                }

                if(e.aura)
                {
                    e.aura->set_position(e.pos);

                    if((_t & 7) == 0)
                    {
                        e.aura->set_tiles(art::fx_aura.tiles_item(), (_t >> 3) & 1);
                    }
                }

                ++i;
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Stage 2 Grim Graveyard (DISENO_FASES E3): scripted events, RISE graves, enemy projectiles (§C6: <= 12).

        void _spawn_ahead(int type, int count)
        {
            fixed_point p = _spawn_point(136, 150, true);

            for(int k = 0; k < count; ++k)
            {
                _spawn_enemy(type, p + fixed_point((k & 1) * 18, (k >> 1) * 18), 0);
            }
        }

        void _update_events_f2(int s, bool second)
        {
            if(_t == 4 * fps)
            {
                _spawn_ahead(type_ghoul, 4);   // E3 0:04: 4 ghouls
            }

            if(_stress)
            {
                return;
            }

            // Murder of Crows (3:20; 12:00 twice): 2 LINE of 10 crows in X. Ghost Swarm (7:00, 13:00): 16 ghosts.
            // 14:00 Stampede of brutes and knights.
            if(_t == 200 * fps - fps || _t == 720 * fps - fps || _t == 735 * fps - fps)
            {
                _begin_swarm_warning(swarm_crows, fps);
            }
            else if(_t == 420 * fps - fps || _t == 780 * fps - fps)
            {
                _begin_swarm_warning(swarm_ghosts, fps);
            }
            else if(_t == 840 * fps - 90)
            {
                _begin_swarm_warning(swarm_stampede, 90);
                _play(art::sfx_horn);
            }

            if(_t == 240 * fps || _t == 720 * fps)
            {
                _spawn_grave_ring();
            }
            else if(_t == 560 * fps)
            {
                _spawn_bone_ring(type_skeleton2, 20);   // 9:20 Bone Ring (20 skeletons)
                test_f2_events = test_f2_events | 8;
            }
            else if(_t == 780 * fps)
            {
                _spawn_elite_pair();
            }

            if(! second)
            {
                return;
            }

            // First appearances (§C1): alone, from the side the wizard faces (the walkers from 2 graves in view).
            switch(s)
            {
            case 30:
                _spawn_ahead(type_crow, 2);
                break;
            case 120:
                for(int k = 0; k < 2; ++k)
                {
                    fixed_point p;

                    if(_find_rise_point(p, 64, 112, true))
                    {
                        _begin_rise(type_walker, p);
                    }
                }
                break;
            case 180:
                _spawn_ahead(type_ghost, 2);
                break;
            case 360:
                _spawn_ahead(type_archer, 2);
                break;
            case 480:
                _spawn_ahead(type_wraith, 2);
                break;
            case 540:
                _spawn_ahead(type_brute, 1);
                break;
            case 660:
                _spawn_ahead(type_knight, 1);
                break;
            default:
                break;
            }
        }

        // RISE point: in front of a tombstone of a nearby "lápidas" chunk at min_d..max_d px from the wizard, or
        // (any_ground) a crack in free ground. Never < 64 px (R5) and never on another mark.
        bool _find_rise_point(fixed_point& out, int min_d, int max_d, bool any_ground)
        {
            int px = to_int(_pos.x());
            int py = to_int(_pos.y());
            int kx0 = (px >> 7) - 1;
            int ky0 = (py >> 7) - 1;
            int start = _random.get_int(36);

            for(int k = 0; k < 36; ++k)
            {
                int idx = (start + k) % 36;
                int c = idx >> 2;
                int kx = kx0 + c % 3;
                int ky = ky0 + c / 3;
                int tpl = _map.chunk_template(kx, ky);

                if(tpl < 0 || tpl / 2 != template_graves)
                {
                    continue;
                }

                const placed_prop& pp = chunk_templates[template_graves][idx & 3];
                int col = (tpl & 1) ? chunk_cells - pp.col - 4 : pp.col;   // flipped chunk: the 4-cell stone mirrors
                fixed_point p((kx << 7) + col * 8 + 16, (ky << 7) + pp.row * 8 + 36);

                if(_rise_point_ok(p, min_d, max_d))
                {
                    out = p;
                    return true;
                }
            }

            if(any_ground)
            {
                for(int attempt = 0; attempt < 6; ++attempt)
                {
                    fixed_point p = _pos + angle_vector(_random.get_int(2048)) * _random.get_int(min_d, max_d + 1);

                    if(_rise_point_ok(p, min_d, max_d))
                    {
                        out = p;
                        return true;
                    }
                }
            }

            return false;
        }

        [[nodiscard]] bool _rise_point_ok(const fixed_point& p, int min_d, int max_d) const
        {
            int x = to_int(p.x());
            int y = to_int(p.y());
            int dx = x - to_int(_pos.x());
            int dy = y - to_int(_pos.y());
            int d2 = dx * dx + dy * dy;

            if(d2 < min_d * min_d || d2 > max_d * max_d || _map.solid_at(x, y) || _map.solid_at(x, y + 6))
            {
                return false;
            }

            for(const mark& m : _marks)
            {
                if(abs_int(to_int(m.pos.x()) - x) < 18 && abs_int(to_int(m.pos.y()) - y) < 18)
                {
                    return false;
                }
            }

            return true;
        }

        void _begin_rise(int type, const fixed_point& p)
        {
            if(_marks.full())
            {
                return;
            }

            int dx = to_int(p.x() - _pos.x());
            int dy = to_int(p.y() - _pos.y());
            int d = 0;

            while((d + 1) * (d + 1) <= dx * dx + dy * dy && d < 999)
            {
                ++d;
            }

            test_rise_min = bn::min(int(test_rise_min), d);
            test_rises = test_rises + 1;
            _marks.push_back({ _world_sprite(art::fx_grave, p, 0, 25), p, 60, mark_rise, uint8_t(type), 0 });

            if(_rise_sound_cd == 0)
            {
                _play(art::sfx_rise, fixed(0.6));
                _rise_sound_cd = 20;
            }
        }

        // Director walkers (E3): from a grave in view (<= 4 open); otherwise 1 of 6 from a crack in free ground;
        // the rest walk in.
        bool _try_rise(int type)
        {
            int open = 0;

            for(const mark& m : _marks)
            {
                open += m.kind == mark_rise;
            }

            if(open >= 4)
            {
                return false;
            }

            fixed_point p;

            if(! _find_rise_point(p, 64, 112, (++_rise_counter % 6) == 0))
            {
                return false;
            }

            _begin_rise(type, p);
            return true;
        }

        // GRAVE RING (E1): 16 slots on r 80 around the wizard, 1 gap every 4 (12 graves), 60 f cracks.
        void _spawn_grave_ring()
        {
            _make_room(12);
            test_f2_events = test_f2_events | 4;
            int offset = _random.get_int(128);
            int staggered = 0;

            for(int i = 0; i < 16; ++i)
            {
                fixed_point p = _pos + angle_vector(offset + i * 128) * 80;

                if((i & 3) != 3 && ! _map.solid_at(to_int(p.x()), to_int(p.y())) && ! _marks.full())
                {
                    // perf: 2 walkers come out per frame (12 new sprites in one frame cost ~60 % of it).
                    _begin_rise(type_walker, p);
                    _marks.back().timer = int16_t(60 + (++staggered) / 2);
                }
            }
        }

        void _update_marks()
        {
            if(_rise_sound_cd > 0)
            {
                --_rise_sound_cd;
            }

            for(int i = 0; i < _marks.size();)
            {
                mark& m = _marks[i];
                --m.timer;

                if(m.kind == mark_rise)
                {
                    // Crack 0-2 over 60 f (2 = bony fingers); the walker comes out harmless for 20 f (§C1 RISE).
                    if(m.timer == 40 || m.timer == 20)
                    {
                        m.sprite.set_tiles(art::fx_grave.tiles_item(), m.timer == 40 ? 1 : 2);
                    }

                    if(m.timer <= 0)
                    {
                        if(enemy* e = _spawn_enemy(m.type, m.pos, _t >= 2 * 60 * fps ? 40 : 0))
                        {
                            e->safe = 20;
                        }

                        _spawn_poof(m.pos, 2);
                    }
                }
                else if(m.kind == mark_blink)
                {
                    // BLINK (§C1): a shadow that grows for 45 f, then the wraith appears on it.
                    if(m.timer == 22)
                    {
                        m.sprite.set_item(art::shadow_big);
                    }

                    if(m.timer <= 0)
                    {
                        for(int j = 0, n = _enemies.size(); j < n; ++j)
                        {
                            if(_enemies[j].uid == m.uid)
                            {
                                _enemies[j].pos = m.pos;
                                _grid_refresh(j);
                                _spawn_poof(m.pos, 2);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    m.sprite.set_visible((m.timer & 4) != 0);   // boss teleport destination (45 / 40 f)
                }

                if(m.timer <= 0)
                {
                    _marks.erase(_marks.begin() + i);
                    continue;
                }

                ++i;
            }
        }

        void _begin_blink(const enemy& e)
        {
            if(_marks.full())
            {
                return;
            }

            fixed_point p = _pos + angle_vector(_random.get_int(2048)) * 56;
            bn::sprite_ptr sp = _world_sprite(art::shadow_small, p + fixed_point(0, 4), 0, 30);
            sp.set_blending_enabled(true);
            _marks.push_back({ bn::move(sp), p, 45, mark_blink, e.type, e.uid });
            test_f2_events = test_f2_events | 32768;
        }

        // SHOOT (§C1): 24 f white flash, then one arrow at 1.2 px/f to where the wizard was; recharge 180 f.
        void _update_archer(enemy& e, int dx, int dy)
        {
            if(e.state == 1)
            {
                if((e.timer & 3) == 0)
                {
                    if(e.timer & 4)
                    {
                        e.sprite.set_palette(*_white_pal);
                    }
                    else
                    {
                        _restore_palette(e);
                    }
                }

                if(--e.timer == 0)
                {
                    e.state = 0;
                    e.cd = 90;
                    _restore_palette(e);

                    if(_fire(shot_arrow, e.pos + fixed_point(0, -6), unit(dx, dy + 6) * fixed(1.2), 8, 220))
                    {
                        _play(art::sfx_arrow, fixed(0.5));
                    }
                }
            }
            else if(e.cd == 0 && abs_int(dx) < 112 && abs_int(dy) < 72 && ! _shots.full() && e.freeze <= 0)
            {
                e.state = 1;
                e.timer = 24;
                test_archer_warns = test_archer_warns + 1;
            }
        }

        bool _fire(int kind, const fixed_point& p, const fixed_point& vel, int damage, int life, int angle = 0)
        {
            if(_shots.full())
            {
                return false;
            }

            const bn::sprite_item& item = kind == shot_arrow ? art::fx_arrow : kind == shot_skull ? art::fx_skull :
                                                                                                art::fx_bone;
            shot& s = _shots.emplace_back(_world_sprite(item, p, 0, 6));
            s.pos = p;
            s.vel = vel;
            s.life = int16_t(life);
            s.kind = uint8_t(kind);
            s.damage = uint8_t(damage);
            s.angle = int16_t(angle);

            if(kind == shot_arrow)
            {
                int ax = abs_int(to_int(vel.x() * 64));
                int ay = abs_int(to_int(vel.y() * 64));
                s.sprite.set_tiles(item.tiles_item(), ay * 2 < ax ? 0 : 1);
                s.sprite.set_horizontal_flip(vel.x() < 0);
                s.sprite.set_vertical_flip(vel.y() > 0);
            }

            test_shots_max = bn::max(int(test_shots_max), _shots.size());
            return true;
        }

        void _update_shots()
        {
            int px = to_int(_pos.x());
            int py = to_int(_pos.y());
            const fixed rot_c = bn::lut_cos(11);   // 2 degrees per frame (2048 LUT units per turn)
            const fixed rot_s = bn::lut_sin(11);

            for(int i = 0; i < _shots.size();)
            {
                shot& s = _shots[i];
                bool gone = --s.life <= 0;

                if(s.kind == shot_orbit)
                {
                    s.angle = int16_t((s.angle + 14) & 2047);
                    gone = gone || ! _boss;
                    s.pos = _boss_pos + angle_vector(s.angle) * 40;
                }
                else
                {
                    if(s.kind == shot_skull)
                    {
                        int dx = px - to_int(s.pos.x());
                        int dy = py - to_int(s.pos.y());
                        fixed cross = s.vel.x() * dy - s.vel.y() * dx;
                        fixed sn = cross > 0 ? rot_s : -rot_s;
                        s.vel = fixed_point(s.vel.x() * rot_c - s.vel.y() * sn, s.vel.x() * sn + s.vel.y() * rot_c);
                        s.sprite.set_horizontal_flip(s.vel.x() < 0);
                    }

                    s.pos += s.vel;
                }

                if(s.kind != shot_arrow && (s.life & 3) == 0)
                {
                    const bn::sprite_item& item = s.kind == shot_skull ? art::fx_skull : art::fx_bone;
                    s.sprite.set_tiles(item.tiles_item(), (s.life >> 2) % (s.kind == shot_skull ? 2 : 4));
                }

                int sx = to_int(s.pos.x());
                int sy = to_int(s.pos.y());

                if(abs_int(sx - px) < player_half_box + 3 && abs_int(sy - py) < player_half_box + 3)
                {
                    _hurt_player(s.damage);
                    gone = gone || s.kind != shot_orbit;
                }

                // Lich skulls break with one hit of any weapon (E5.2): bolts, runes and area blasts (_area_damage).
                if(s.kind == shot_skull && ! gone)
                {
                    for(const bolt& b : _bolts)
                    {
                        if(abs_int(to_int(b.pos.x()) - sx) < 7 && abs_int(to_int(b.pos.y()) - sy) < 7)
                        {
                            gone = true;
                        }
                    }

                    for(const bn::sprite_ptr& r : _runes)
                    {
                        if(abs_int(to_int(r.x()) - sx) < 8 && abs_int(to_int(r.y()) - sy) < 8)
                        {
                            gone = true;
                        }
                    }

                    if(gone)
                    {
                        _spawn_poof(s.pos, 1);
                        test_skulls_broken = test_skulls_broken + 1;
                    }
                }

                if(gone || abs_int(sx - px) > 220 || abs_int(sy - py) > 160)
                {
                    _shots.erase(_shots.begin() + i);
                    continue;
                }

                s.sprite.set_position(s.pos);
                ++i;
            }
        }

        void _break_skulls(int x, int y, int radius)
        {
            for(int i = 0; i < _shots.size();)
            {
                const shot& s = _shots[i];
                int dx = to_int(s.pos.x()) - x;
                int dy = to_int(s.pos.y()) - y;

                if(s.kind == shot_skull && dx * dx + dy * dy <= radius * radius)
                {
                    _spawn_poof(s.pos, 1);
                    test_skulls_broken = test_skulls_broken + 1;
                    _shots.erase(_shots.begin() + i);
                    continue;
                }

                ++i;
            }
        }

        void _clear_stage2_attacks()
        {
            _shots.clear();
            _wave_ring.clear();
            _boss_warn.clear();

            for(int i = _marks.size() - 1; i >= 0; --i)
            {
                if(_marks[i].kind == mark_boss)
                {
                    _marks.erase(_marks.begin() + i);
                }
            }
        }

        // ---- Stage 2 bosses (DISENO_FASES E5.2). Shared: entrance mosaic 30 f, phase change (60 f invulnerable
        // with flash, roar, 48 px push), leash (x3 past 160 px until 120), ire (+30 % at 2:00; Lich: Hex Moon).
        static constexpr int colossus_hp = 500;    // E5.2 1000; R13: 800 gave TTK 56 s (Start at 4:30, profile C)
        static constexpr int banshee_hp = 1100;    // E5.2 4000; R13: 1400 gave TTK 60 s (Start at 9:45, profile C)
        static constexpr int lich_hp = 3800;       // E5.2 9000; scaled like the Treant (7000 -> 3000)

        enum f2_boss_state
        {
            fb_enter = 0,
            fb_roar = 1,
            fb_walk = 2,
            fb_warn = 3,     // Colossus arm up / Banshee mouth open / Lich staff glow
            fb_act = 4,      // Colossus throw recovery / Banshee wave
            fb_collapse = 5, // Colossus pile
            fb_rearm = 6,
            fb_vanish = 7    // Banshee / Lich teleport
        };

        [[nodiscard]] fixed _f2_boss_speed(fixed base, int dist)
        {
            _boss_leash = dist > 160 || (_boss_leash && dist > 120);
            fixed speed = base * (_boss_leash ? 3 : 1);

            if(_boss_kind == 5)
            {
                return _hex_moon ? speed * fixed(1.5) : speed;
            }

            return _boss_fight_t > 2 * 60 * fps ? speed * fixed(1.3) : speed;
        }

        void _update_f2_boss()
        {
            int pct = (_boss_hp * 100) / _boss_max_hp;
            int hi = _boss_kind == 3 ? 50 : _boss_kind == 4 ? 60 : 66;
            int lo = _boss_kind == 3 ? 25 : _boss_kind == 4 ? 30 : 33;
            int phase = pct < lo ? 3 : pct < hi ? 2 : 1;
            int dx = to_int(_pos.x() - _boss_pos.x());
            int dy = to_int(_pos.y() - _boss_pos.y());
            int dist = abs_int(dx) + abs_int(dy);

            if(_boss_kind == 5 && ! _hex_moon && _boss_fight_t > 3 * 60 * fps)
            {
                _hex_moon = true;
                test_events = test_events | 1024 | (1 << 18);
                _show_banner("THE HEX MOON RISES", art::text_red, 3 * fps);
                _play(art::sfx_boss);
                _shake = 16;
            }

            if(phase > _boss_phase && _boss_state != fb_enter && _boss_state != fb_collapse &&
               _boss_state != fb_vanish)
            {
                _boss_phase = phase;
                test_boss_phase = phase;
                _boss_invuln = 60;
                _boss_state = fb_roar;
                _boss_timer = 60;
                _boss_ghostly = false;
                _boss->set_blending_enabled(false);
                _boss->set_visible(true);
                _boss_warn.clear();
                _wave_ring.clear();
                _shake = 20;
                _play(art::sfx_boss);
                _push_monsters_from_boss();

                if(_boss_kind == 4 && phase == 2)
                {
                    _summon_around_boss(type_ghost, 4);
                }
            }

            if(_boss_invuln > 0)
            {
                --_boss_invuln;

                if(_boss_state == fb_roar)
                {
                    _boss_glow(_boss_invuln);
                }
            }

            switch(_boss_state)
            {
            case fb_enter:
                if(_fx.boss_mosaic)
                {
                    bn::sprites_mosaic::set_stretch(fixed(bn::max(0, _boss_timer)) / 30);
                }

                if(--_boss_timer <= 0)
                {
                    bn::sprites_mosaic::set_stretch(0);
                    _boss->set_mosaic_enabled(false);
                    _boss_state = fb_walk;
                    _boss_timer = 90;
                }
                break;

            case fb_roar:
                if(--_boss_timer <= 0)
                {
                    _boss->set_palette(_boss_item().palette_item());
                    _boss_state = fb_walk;
                    _boss_timer = 30;

                    if(_boss_kind == 3 && _boss_phase == 2)
                    {
                        _colossus_collapse();   // E5.2 < 50 %: the pile comes first
                    }
                }
                break;

            default:
                if(_boss_kind == 3)
                {
                    _update_colossus(dx, dy, dist);
                }
                else if(_boss_kind == 4)
                {
                    _update_banshee(dx, dy, dist);
                }
                else
                {
                    _update_lich(dx, dy, dist);
                }
                break;
            }

            if(_boss_kind == 3)
            {
                _boss->set_horizontal_flip(dx < 0);   // the Colossus faces right
            }

            int lift = _boss_kind == 4 ? 4 + to_int(bn::lut_sin((_t * 12) & 2047) * 3) : 0;
            _boss->set_position(_boss_pos.x(), _boss_pos.y() - 16 - lift);

            if(_boss_shadow)
            {
                _boss_shadow->set_position(_boss_pos + fixed_point(0, 14));
                _boss_shadow->set_visible(_fx.shadows && _boss_state != fb_vanish);
            }

            bool solid = _boss_state != fb_enter && _boss_state != fb_collapse && _boss_state != fb_vanish &&
                         ! _boss_ghostly;

            if(solid && _boss_box_hit(to_int(_pos.x()), to_int(_pos.y()), player_half_box))
            {
                _hurt_player(_boss_kind == 4 ? 12 : 14);
            }
        }

        void _summon_around_boss(int type, int count)
        {
            _make_room(count);

            for(int k = 0; k < count; ++k)
            {
                fixed_point p = _boss_pos + angle_vector(k * 2048 / count + 128) * 36;

                if(enemy* e = _spawn_enemy(type, p, 0))
                {
                    e->safe = 20;
                }
            }

            _spawn_poof(_boss_pos, 3);
        }

        void _boss_glow(int timer)
        {
            _boss->set_palette((timer & 4) ? bn::sprite_palette_item(_boss_white_colors, bn::bpp_mode::BPP_4) :
                                             _boss_item().palette_item());
        }

        // Bone Colossus (5:00): bones in a 20 degree fan every 150 f after a 30 f arm flash; < 50 % the pile
        // (90 f invulnerable, 6 bones circling r 40 for 3 s) every third attack; < 25 % 5 bones and 0.6 px/f.
        void _colossus_collapse()
        {
            _boss_state = fb_collapse;
            _boss_timer = 90;
            _boss_invuln = 90;
            _boss_frame = -1;
            _play(art::sfx_bone);
            test_f2_events = test_f2_events | 64;

            for(int k = 0; k < 6; ++k)
            {
                _fire(shot_orbit, _boss_pos, fixed_point(), 8, 180, k * 2048 / 6);
            }
        }

        void _update_colossus(int dx, int dy, int dist)
        {
            switch(_boss_state)
            {
            case fb_walk:
            {
                fixed speed = _f2_boss_speed(_boss_phase == 3 ? fixed(0.6) : fixed(0.4), dist);
                _boss_pos += unit(dx, dy) * speed;
                _set_boss_frame(art::colossus_walk, _t, true);

                if(--_boss_timer <= 0)
                {
                    ++_boss_attacks;

                    if(_boss_phase >= 2 && _boss_attacks % 3 == 0)
                    {
                        _colossus_collapse();
                    }
                    else
                    {
                        _boss_state = fb_warn;
                        _boss_timer = 30;
                        _set_boss_frame(art::colossus_throw, 0, false);
                    }
                }
                break;
            }

            case fb_warn:
                _boss_glow(_boss_timer);

                if(--_boss_timer <= 0)
                {
                    _boss->set_palette(_boss_item().palette_item());
                    _set_boss_frame(art::colossus_throw, art::colossus_throw.frame_time, false);
                    int n = _boss_phase == 3 ? 5 : 3;
                    fixed_point to = unit(dx, dy);

                    for(int k = 0; k < n; ++k)
                    {
                        fixed_point r = angle_vector((k - n / 2) * 114);   // 20 degrees apart
                        fixed_point v(to.x() * r.x() - to.y() * r.y(), to.x() * r.y() + to.y() * r.x());
                        _fire(shot_bone, _boss_pos + fixed_point(0, -20), v * fixed(1.4), 10, 200);
                    }

                    _play(art::sfx_bone, fixed(0.8));
                    _boss_state = fb_act;
                    _boss_timer = 15;
                }
                break;

            case fb_act:
                if(--_boss_timer <= 0)
                {
                    _boss_state = fb_walk;
                    _boss_timer = 150;
                }
                break;

            case fb_collapse:
                _set_boss_frame(art::colossus_collapse, 90 - _boss_timer, false);

                if(--_boss_timer <= 0)
                {
                    _boss_state = fb_rearm;
                    _boss_timer = 24;
                }
                break;

            case fb_rearm:
                _set_boss_frame(art::colossus_collapse, _boss_timer, false);

                if(--_boss_timer <= 0)
                {
                    _boss_state = fb_walk;
                    _boss_timer = 120;
                }
                break;

            default:
                _boss_state = fb_walk;
                break;
            }
        }

        // Banshee (10:00): floats at 90 px; Wail = 45 f open mouth + 4 "!" around, then a ring 0 -> 96 px in 30 f
        // (12, once per wail); < 60 % intangible 3 s of every 8 (50 % alpha), 4 ghosts every other cycle;
        // < 30 % teleports next to the screen edge (45 f mark) and wails every 4 s.
        void _update_banshee(int dx, int dy, int dist)
        {
            bool ghostly = _boss_phase >= 2 && _boss_state == fb_walk && (_boss_fight_t % 480) < 180;

            if(ghostly != _boss_ghostly)
            {
                _boss_ghostly = ghostly;
                _boss->set_blending_enabled(ghostly);

                if(ghostly)
                {
                    test_f2_events = test_f2_events | 256;

                    if((_boss_fight_t / 480) % 2 == 0)
                    {
                        _summon_around_boss(type_ghost, 4);
                    }
                }
            }

            switch(_boss_state)
            {
            case fb_walk:
            {
                int d2 = dx * dx + dy * dy;
                fixed speed = _f2_boss_speed(fixed(0.8), dist);
                fixed_point to = unit(dx, dy);

                if(d2 > 100 * 100 || _boss_leash)
                {
                    _boss_pos += to * speed;
                }
                else if(d2 < 80 * 80)
                {
                    _boss_pos -= to * fixed(0.6);
                }

                _set_boss_frame(art::banshee_float, _t, true);

                if(--_boss_timer <= 0 && ! _boss_ghostly)
                {
                    if(_boss_phase == 3 && ! _marks.full())
                    {
                        // Next to the screen edge, inside it (E5.2 < 30 %).
                        int side = _random.get_int(4);
                        fixed_point c = _camera.position();
                        fixed_point dest(c.x() + (side == 0 ? -92 : side == 1 ? 92 : _random.get_int(-80, 81)),
                                         c.y() + (side == 2 ? -40 : side == 3 ? 52 : _random.get_int(-30, 41)));
                        _boss_target = dest;
                        _marks.push_back({ _world_sprite(art::alert, dest, 0, 7), dest, 45, mark_boss, 0, 0 });
                        _boss_state = fb_vanish;
                        _boss_timer = 45;
                        _play(art::sfx_teleport, fixed(0.7));
                        test_f2_events = test_f2_events | 512;
                    }
                    else
                    {
                        _banshee_warn();
                    }
                }
                break;
            }

            case fb_vanish:
                _set_boss_frame(art::banshee_vanish, 45 - _boss_timer, false);
                _boss->set_visible(_boss_timer > 30 || (_boss_timer & 2));

                if(--_boss_timer <= 0)
                {
                    _boss_pos = _boss_target;
                    _boss->set_visible(true);
                    _banshee_warn();
                }
                break;

            case fb_warn:
                _set_boss_frame(art::banshee_wail, _t, true);

                for(int k = 0, n = _boss_warn.size(); k < n; ++k)
                {
                    _boss_warn[k].set_position(_boss_pos + angle_vector(k * 512 + 256) * 30 + fixed_point(0, -16));
                    _boss_warn[k].set_tiles(art::alert.tiles_item(), (_boss_timer >> 2) & 1);
                }

                if(--_boss_timer <= 0)
                {
                    _boss_warn.clear();
                    _boss_state = fb_act;
                    _boss_timer = 0;
                    _boss_attacks = 0;   // the wave hits once

                    for(int k = 0; k < 12 && ! _wave_ring.full(); ++k)
                    {
                        _wave_ring.push_back(_world_sprite(art::nova_spark, _boss_pos, 0, 6));
                    }
                }
                break;

            case fb_act:
            {
                ++_boss_timer;
                int r = (96 * _boss_timer) / 30;

                for(int k = 0, n = _wave_ring.size(); k < n; ++k)
                {
                    _wave_ring[k].set_position(_boss_pos + angle_vector(k * 2048 / n) * r);
                    _wave_ring[k].set_tiles(art::nova_spark.tiles_item(), (_boss_timer >> 2) & 3);
                }

                int d2 = dx * dx + dy * dy;

                if(! _boss_attacks && d2 <= (r + 4) * (r + 4) && d2 >= (r - 12) * (r - 12) && _blink_invulnerable == 0)
                {
                    _boss_attacks = 1;
                    _hurt_player(12);
                    test_wail_hits = test_wail_hits + 1;
                }

                if(_boss_timer >= 30)
                {
                    _wave_ring.clear();
                    _boss_state = fb_walk;
                    _boss_timer = _boss_phase == 3 ? 4 * fps - 75 : 4 * fps;
                }
                break;
            }

            default:
                _boss_state = fb_walk;
                break;
            }
        }

        void _banshee_warn()
        {
            _boss_state = fb_warn;
            _boss_timer = 45;
            _boss_warn.clear();

            for(int k = 0; k < 4; ++k)
            {
                _boss_warn.push_back(_world_sprite(art::alert, _boss_pos, 0, 7));
            }

            _play(art::sfx_wail);
            test_f2_events = test_f2_events | 128;
        }

        // The Lich (15:00): staff glow 30 f -> 5 homing skulls (2 deg/f, 1.2 px/f, 4 s, 10; one hit breaks them);
        // < 66 % every other attack raises 4 walkers (60 f cracks at 64-96 px), skulls every 5 s; < 33 % death
        // field (fade 0.5) and a teleport every 6 s (40 f smoke at the destination).
        void _update_lich(int dx, int dy, int dist)
        {
            if(_boss_phase == 3 && _boss_state == fb_walk && --_lich_teleport_cd <= 0 && ! _marks.full())
            {
                _lich_teleport_cd = 6 * fps;
                fixed_point dest = _pos + angle_vector(_random.get_int(2048)) * _random.get_int(90, 111);
                _boss_target = dest;
                _marks.push_back({ _world_sprite(art::fx_poof, dest, 0, 7), dest, 40, mark_boss, 0, 0 });
                _boss_state = fb_vanish;
                _boss_timer = 40;
                _play(art::sfx_teleport, fixed(0.7));
                test_f2_events = test_f2_events | 4096;
            }

            switch(_boss_state)
            {
            case fb_walk:
                _boss_pos += unit(dx, dy) * _f2_boss_speed(fixed(0.3), dist);
                _set_boss_frame(art::lich_idle, _t, true);

                if(--_boss_timer <= 0)
                {
                    ++_boss_attacks;

                    if(_boss_phase >= 2 && (_boss_attacks & 1) == 0)
                    {
                        int raised = 0;

                        for(int k = 0; k < 4; ++k)
                        {
                            fixed_point p;

                            if(_find_rise_point(p, 64, 96, true))
                            {
                                _begin_rise(type_walker, p);
                                ++raised;
                            }
                        }

                        test_f2_events = test_f2_events | (raised ? 2048 : 0);
                        _boss_timer = 2 * fps;
                    }
                    else
                    {
                        _boss_state = fb_warn;
                        _boss_timer = 30;
                    }
                }
                break;

            case fb_warn:
                _set_boss_frame(art::lich_cast, 30 - _boss_timer, false);
                _boss_glow(_boss_timer);

                if(--_boss_timer <= 0)
                {
                    _boss->set_palette(_boss_item().palette_item());
                    fixed_point to = unit(dx, dy);

                    for(int k = 0; k < 5; ++k)
                    {
                        fixed_point r = angle_vector((k - 2) * 200);   // a 140 degree fan that homes in
                        fixed_point v(to.x() * r.x() - to.y() * r.y(), to.x() * r.y() + to.y() * r.x());
                        _fire(shot_skull, _boss_pos + fixed_point(0, -24), v * fixed(1.2), 10, 4 * fps);
                    }

                    _play(art::sfx_skull);
                    test_f2_events = test_f2_events | 1024;
                    _boss_state = fb_walk;
                    _boss_timer = _boss_phase >= 2 ? 5 * fps : 4 * fps;
                }
                break;

            case fb_vanish:
                _set_boss_frame(art::lich_teleport, _boss_timer > 20 ? 40 - _boss_timer : _boss_timer, false);
                _boss->set_visible(_boss_timer > 22 || _boss_timer < 18);

                if(_boss_timer == 20)
                {
                    _boss_pos = _boss_target;
                }

                if(--_boss_timer <= 0)
                {
                    _boss->set_visible(true);
                    _boss_state = fb_walk;
                    _boss_timer = 60;
                }
                break;

            default:
                _boss_state = fb_walk;
                break;
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Boss: Slime King (§8)

        void _spawn_boss(int kind)
        {
            _boss_spawned = true;
            _boss_kind = kind;
            _boss_pos = _pos + _facing * (kind ? 110 : 90);   // E5: never on top, >= 96 px
            _boss_frame = -1;

            if(_map.solid_at(to_int(_boss_pos.x()), to_int(_boss_pos.y())))
            {
                _boss_pos = _pos + fixed_point(0, -100);
            }

            _boss_entry = _boss_pos;
            constexpr int hps[6] = { 450, warchief_hp, treant_hp, colossus_hp, banshee_hp, lich_hp };   // King: R13
            _boss_max_hp = hps[kind];
            _boss_ghostly = false;
            _boss_attacks = 0;
            _boss_leash = false;
            _lich_teleport_cd = 6 * fps;
            _root_cd = 90;
            _seed_cd = 3 * fps;
            _wolf_cd = 5 * fps;
            _thorn_t = -1;
            _hex_moon = false;
            _boss_hp = _boss_max_hp;
            _boss_state = 0;
            _boss_timer = 30;
            _boss_fight_t = 0;
            _boss_phase = 1;
            _boss_invuln = 0;
            test_boss_phase = 1;

            if(kind == 1)
            {
                test_events = test_events | 16;
            }

            art::music_boss.play(fixed(art::music_volume_pct) / 100);
            bn::sprite_ptr b = _world_sprite(_boss_item(), _boss_pos, 0, 8);
            b.set_mosaic_enabled(_fx.boss_mosaic);
            _boss = bn::move(b);
            bn::sprite_ptr sh = _world_sprite(art::shadow_big, _boss_pos + fixed_point(0, 22), 0, 31);
            sh.set_blending_enabled(true);
            _boss_shadow = bn::move(sh);

            for(int i = 0; i < 6; ++i)
            {
                bn::sprite_ptr bar = art::boss_bar.create_sprite(-80 + i * 32, -56, 16);
                bar.set_bg_priority(0);
                bar.set_z_order(1);
                _boss_bar.push_back(bn::move(bar));
            }

            // Notches where the boss changes phase (E5: Warchief 60 % and 30 %, King 50 % and 25 %).
            for(int k = 0; k < 2; ++k)
            {
                constexpr int hi[3] = { 50, 60, 66 };
                constexpr int lo[3] = { 25, 30, 33 };
                int pct = k ? lo[kind % 3] : hi[kind % 3];
                bn::sprite_ptr notch = art::boss_bar.create_sprite(-96 + (192 * pct) / 100, -56, art::boss_bar_notch);
                notch.set_bg_priority(0);
                notch.set_z_order(0);
                _boss_bar.push_back(bn::move(notch));
            }

            _boss_title.clear();
            _big_text.set_center_alignment();
            constexpr const char* names[6] = { "SLIME KING", "ORC WARCHIEF", "THORNWOOD TREANT", "BONE COLOSSUS",
                                               "BANSHEE", "THE LICH" };
            _big_text.generate(0, -30, names[kind], _boss_title);
            _boss_title_t = 2 * fps;
            _shake = 12;
            _play(art::sfx_boss);
        }

        [[nodiscard]] bool _boss_box_hit(int x, int y, int half) const
        {
            int dx = x - to_int(_boss_pos.x());
            int dy = y - to_int(_boss_pos.y());
            return abs_int(dx) < 24 + half && abs_int(dy) < 18 + half;
        }

        void _hit_boss(int damage)
        {
            if(_boss_invuln > 0 || _boss_ghostly)
            {
                return;
            }

            _boss_hp -= damage;
            _boss_flash = hit_flash_frames;

            if(_fx.hit_flash && _boss)
            {
                _boss->set_palette(bn::sprite_palette_item(_boss_white_colors, bn::bpp_mode::BPP_4));
            }

            if(_hit_sound_cd == 0)
            {
                _play(art::sfx_hit, fixed(0.6));
                _hit_sound_cd = 4;
            }

            if(_fx.damage_numbers)
            {
                _spawn_damage_number(_boss_pos + fixed_point(0, -20), damage);
            }
        }

        void _update_boss()
        {
            if(_boss_title_t > 0 && --_boss_title_t == 0)
            {
                _boss_title.clear();
            }

            if(_boss_wave)
            {
                if(++_boss_wave_t >= 16)
                {
                    _boss_wave.reset();
                }
                else if(_boss_wave_t == 8)
                {
                    _boss_wave->set_tiles(art::wave.tiles_item(), 1);
                }
            }

            if(_boss_rune_cd)
            {
                --_boss_rune_cd;
            }

            if(_music_resume_t > 0 && --_music_resume_t == 0)
            {
                _stage_music().play(fixed(art::music_volume_pct) / 100);
            }

            if(! _boss)
            {
                if(_boss_dead && _end_timer > 0)
                {
                    if(_end_timer == 3 * fps)
                    {
                        _show_banner("STAGE CLEAR", art::text_gold, 3 * fps);
                        _play(art::jingle_victory);
                        test_events = test_events | (1 << 19);
                    }

                    if(--_end_timer == 0)
                    {
                        _mode = game_mode::won;
                        _end_timer = 1;
                    }
                }

                return;
            }

            if(_boss_flash && --_boss_flash == 0)
            {
                _boss->set_palette(_boss_item().palette_item());
            }

            if(_boss_hp <= 0)
            {
                _kill_boss();
                return;
            }

            ++_boss_fight_t;
            _update_boss_bar();

            if(_boss_kind >= 3)
            {
                _update_f2_boss();
                return;
            }

            if(_boss_kind == 1)
            {
                _update_warchief();
                return;
            }

            if(_boss_kind == 2)
            {
                _update_treant();
                return;
            }

            bool phase2 = _boss_hp * 2 < _boss_max_hp;
            bool phase3 = _boss_hp * 4 < _boss_max_hp;
            bool rage = _boss_fight_t > 2 * 60 * fps;   // E5 ira at 2:00 of combat: +30 % speed
            int air_time = phase3 ? 30 : 45;

            if(rage && ! (test_events & 1024))
            {
                test_events = test_events | 1024;
            }
            int height = 0;

            switch(_boss_state)
            {
            case 0: // entrance (mosaic 1 -> 0 in 30 f)
                if(_fx.boss_mosaic)
                {
                    bn::sprites_mosaic::set_stretch(fixed(_boss_timer) / 30);
                }

                if(--_boss_timer <= 0)
                {
                    bn::sprites_mosaic::set_stretch(0);
                    _boss->set_mosaic_enabled(false);
                    _boss_state = 1;
                    _boss_timer = 60;
                }
                break;

            case 1: // walk
            {
                fixed_point d = unit(to_int(_pos.x() - _boss_pos.x()), to_int(_pos.y() - _boss_pos.y()));
                _boss_pos += d * (rage ? fixed(0.39) : fixed(0.3));

                if(--_boss_timer <= 0)
                {
                    _boss_state = 2;
                    _boss_timer = 12;
                    _boss->set_tiles(art::slime_king.tiles_item(), art::king_crouch);
                }
                else if(_boss_timer % art::king_idle.frame_time == 0)
                {
                    _boss->set_tiles(art::slime_king.tiles_item(), art::king_idle.frame(_t, true));
                }
                break;
            }

            case 2: // crouch
                if(--_boss_timer <= 0)
                {
                    _boss_state = 3;
                    _boss_timer = air_time;
                    _boss_from = _boss_pos;
                    _boss_target = _pos;
                    _boss->set_tiles(art::slime_king.tiles_item(), art::king_air.first);
                }
                break;

            case 3: // airborne: the shadow marks the landing spot for the whole jump (45 f, 30 f in phase 3)
            {
                int elapsed = air_time - _boss_timer;
                _boss_pos = _boss_from + (_boss_target - _boss_from) * elapsed / air_time;
                int rise = 10;
                int fall = 10;

                if(elapsed < rise)
                {
                    height = (elapsed * 90) / rise;
                }
                else if(_boss_timer <= fall)
                {
                    height = (_boss_timer * 90) / fall;
                }
                else
                {
                    height = 90;
                }

                if(--_boss_timer <= 0)
                {
                    _boss_pos = _boss_target;
                    _boss_land(phase2);
                }
                break;
            }

            case 4: // recover
                if(_boss_timer == 20)
                {
                    _boss->set_tiles(art::slime_king.tiles_item(), art::king_land.first + 1);
                }

                if(--_boss_timer <= 0)
                {
                    _boss->set_tiles(art::slime_king.tiles_item(), art::king_idle.first);
                    _boss_state = 1;
                    _boss_timer = phase3 ? (rage ? 14 : 18) : (rage ? 46 : 60);
                }
                break;

            default:
                break;
            }

            _boss->set_position(_boss_pos.x(), _boss_pos.y() - 6 - height);

            if(_boss_shadow)
            {
                fixed_point shadow_pos = _boss_state == 3 ? _boss_target : _boss_pos;
                _boss_shadow->set_position(shadow_pos + fixed_point(0, 22));
                _boss_shadow->set_visible(_fx.shadows || _boss_state == 3);
            }

            if(_boss_state != 3 && _boss_state != 0 && _boss_box_hit(to_int(_pos.x()), to_int(_pos.y()), player_half_box))
            {
                _hurt_player(10);
            }

        }

        void _update_boss_bar()
        {
            int shown = (bn::max(0, _boss_hp) * 96) / _boss_max_hp;   // 2 px per frame step, 6 pieces of 16 steps

            if(shown == _boss_bar_shown)
            {
                return;
            }

            _boss_bar_shown = shown;

            for(int i = 0; i < 6; ++i)
            {
                _boss_bar[i].set_tiles(art::boss_bar.tiles_item(), bn::max(0, bn::min(16, shown - i * 16)));
            }
        }

        // ---- Orc Warchief (DISENO_FASES E5.1): charges (40 f stomp + line of "!", 3.5 px/f up to 90 f, stunned
        // 60 f when he hits a prop), horn below 60 % (8 goblins from 2 sides), double charge below 30 %.
        // E5.1 starts at 3000; R13 rule HP = TTK target x measured DPS: profile C (God, Lv 20) did ~22 dps -> 45 s.
        static constexpr int warchief_hp = 1000;
        enum warchief_state
        {
            wc_walk = 11,
            wc_stomp = 12,
            wc_charge = 13,
            wc_stunned = 14,
            wc_horn = 15,
            wc_roar = 16
        };

        void _set_boss_frame(const art::anim& a, int ticks, bool loop)
        {
            int frame = a.frame(ticks, loop);

            if(frame != _boss_frame)
            {
                _boss_frame = frame;
                _boss->set_tiles(_boss_item().tiles_item(), frame);
            }
        }

        [[nodiscard]] const bn::sprite_item& _boss_item() const
        {
            switch(_boss_kind)
            {
            case 1:
                return art::warchief;
            case 2:
                return art::treant;
            case 3:
                return art::bone_colossus;
            case 4:
                return art::banshee;
            case 5:
                return art::lich;
            default:
                return art::slime_king;
            }
        }

        void _warchief_warning(int frames)
        {
            // Direction fixed when the warning starts; 4 "!" along it.
            _boss_dir = unit(to_int(_pos.x() - _boss_pos.x()), to_int(_pos.y() - _boss_pos.y()));
            _boss_state = wc_stomp;
            _boss_timer = frames;
            _boss_warn.clear();

            int bx = to_int(_boss_pos.x() - _camera.x());
            int by = to_int(_boss_pos.y() - _camera.y());

            if(abs_int(bx) > 96 || abs_int(by) > 56)
            {
                test_charge_offscreen = test_charge_offscreen + 1;
            }

            // Review block 2: 12 px apart, kept on screen.
            for(int k = 1; k <= 4; ++k)
            {
                fixed_point w = _boss_pos + _boss_dir * (24 + k * 12);
                w.set_x(bn::clamp(w.x(), _camera.x() - 112, _camera.x() + 112));
                w.set_y(bn::clamp(w.y(), _camera.y() - 72, _camera.y() + 72));
                _boss_warn.push_back(_world_sprite(art::alert, w, 0, 7));
            }
        }

        void _update_warchief()
        {
            int pct = (_boss_hp * 100) / _boss_max_hp;
            int phase = pct < 30 ? 3 : pct < 60 ? 2 : 1;
            fixed rage = _boss_fight_t > 2 * 60 * fps ? fixed(1.3) : fixed(1);   // E5 ira at 2:00 of combat

            if(_boss_fight_t > 2 * 60 * fps)
            {
                test_events = test_events | 1024;
            }
            int dx = to_int(_pos.x() - _boss_pos.x());
            int dy = to_int(_pos.y() - _boss_pos.y());

            if(phase > _boss_phase && _boss_state != 0)
            {
                // Phase change (E5): 60 f invulnerable flash + roar + monsters pushed 48 px away.
                _boss_phase = phase;
                test_boss_phase = phase;
                _boss_invuln = 60;
                _boss_state = wc_roar;
                _boss_timer = 60;
                _boss_warn.clear();
                _shake = 20;
                _play(art::sfx_boss);

                for(int i = 0, n = _enemies.size(); i < n; ++i)
                {
                    enemy& e = _enemies[i];
                    int ex = to_int(e.pos.x() - _boss_pos.x());
                    int ey = to_int(e.pos.y() - _boss_pos.y());

                    if(ex * ex + ey * ey < 48 * 48)
                    {
                        e.pos = _boss_pos + (ex == 0 && ey == 0 ? fixed_point(1, 0) : unit(ex, ey)) * 48;
                        _grid_refresh(i);
                    }
                }
            }

            if(_boss_invuln > 0)
            {
                --_boss_invuln;
                _boss->set_palette((_boss_invuln & 4) ? bn::sprite_palette_item(_boss_white_colors, bn::bpp_mode::BPP_4) :
                                                        art::warchief.palette_item());
            }

            switch(_boss_state)
            {
            case 0: // entrance (mosaic 1 -> 0 in 30 f)
                if(_fx.boss_mosaic)
                {
                    bn::sprites_mosaic::set_stretch(fixed(_boss_timer) / 30);
                }

                if(--_boss_timer <= 0)
                {
                    bn::sprites_mosaic::set_stretch(0);
                    _boss->set_mosaic_enabled(false);
                    _boss_state = wc_walk;
                    _boss_timer = 120;
                }
                break;

            case wc_walk:
            case wc_roar:
            {
                fixed speed = fixed(0.5) * rage;
                int dist = abs_int(dx) + abs_int(dy);
                _boss_leash = dist > 100 || (_boss_leash && dist > 64);   // review block 2: x3 from 100 to 64 px

                if(_boss_leash)
                {
                    speed *= 3;
                }

                if(_boss_state == wc_walk)
                {
                    fixed_point d = unit(dx, dy);
                    _boss_pos += d * speed;
                    _set_boss_frame(art::warchief_walk, _t, true);
                }
                else
                {
                    _set_boss_frame(art::warchief_horn, _t, true);
                }

                if(--_boss_timer <= 0)
                {
                    // Every 4 s: below 60 % every other attack is the horn.
                    if(_boss_phase >= 2 && (++_boss_attacks & 1) == 0)
                    {
                        _boss_state = wc_horn;
                        _boss_timer = 45;
                        _play(art::sfx_horn);
                    }
                    else if(abs_int(to_int(_boss_pos.x() - _camera.x())) > 96 ||
                            abs_int(to_int(_boss_pos.y() - _camera.y())) > 56)
                    {
                        _boss_timer = fps;   // review block 2: charges only start on screen; walk 1 s more
                    }
                    else
                    {
                        _boss_second = _boss_phase >= 3;
                        _warchief_warning(40);
                    }
                }
                break;
            }

            case wc_stomp:
                _set_boss_frame(art::warchief_stomp, _boss_timer, true);

                if(_boss_timer % 10 == 0)
                {
                    _spawn_particle(_boss_pos + fixed_point(_random.get_int(-16, 17), 14), fixed_point(0, fixed(-0.3)),
                                    art::fx_dust, art::dust_anim, 15, 9);
                    _play(art::sfx_stomp, fixed(0.6));
                }

                for(int k = 0; k < _boss_warn.size(); ++k)
                {
                    _boss_warn[k].set_tiles(art::alert.tiles_item(), (_boss_timer >> 2) & 1);
                }

                if(--_boss_timer <= 0)
                {
                    _boss_warn.clear();
                    _boss_state = wc_charge;
                    _boss_timer = 90;
                }
                break;

            case wc_charge:
            {
                _set_boss_frame(art::warchief_charge, _t, true);
                fixed_point next = _boss_pos + _boss_dir * (fixed(3.5) * rage);
                fixed_point front = next + _boss_dir * 20;

                if(_map.solid_at(to_int(front.x()), to_int(front.y() + 10)))
                {
                    // Hit a prop: stunned 60 f (the window).
                    _boss_state = wc_stunned;
                    _boss_timer = 60;
                    _shake = 14;
                    _boss_second = false;
                    test_events = test_events | 64;
                    _play(art::sfx_stomp);
                    _spawn_poof(front, 4);
                    break;
                }

                _boss_pos = next;

                if((_boss_timer & 7) == 0)
                {
                    _spawn_particle(_boss_pos + fixed_point(0, 14), fixed_point(), art::fx_dust, art::dust_anim, 15, 9);
                }

                if(--_boss_timer <= 0)
                {
                    if(_boss_second && abs_int(to_int(_boss_pos.x() - _camera.x())) <= 96 &&
                       abs_int(to_int(_boss_pos.y() - _camera.y())) <= 56)
                    {
                        // Phase 3: second warning of 20 f, re-aimed.
                        _boss_second = false;
                        test_events = test_events | 128;
                        _warchief_warning(20);
                    }
                    else
                    {
                        _boss_state = wc_walk;
                        _boss_timer = 4 * fps - 130;
                    }
                }
                break;
            }

            case wc_stunned:
                if(_boss_frame != art::warchief_stunned)
                {
                    _boss_frame = art::warchief_stunned;
                    _boss->set_tiles(art::warchief.tiles_item(), art::warchief_stunned);
                }

                _boss->set_visible(! (_boss_timer & 8) || _boss_timer > 40);

                if(--_boss_timer <= 0)
                {
                    _boss->set_visible(true);
                    _boss_state = wc_walk;
                    _boss_timer = 60;
                }
                break;

            case wc_horn:
                _set_boss_frame(art::warchief_horn, _boss_timer, true);
                _shake = bn::max(_shake, 2);

                if(--_boss_timer <= 0)
                {
                    // 8 goblins from 2 opposite sides.
                    test_events = test_events | 32;
                    _make_room(8);
                    fixed_point side = angle_vector(_random.get_int(2048));

                    for(int k = 0; k < 8; ++k)
                    {
                        fixed_point base = _pos + side * (k < 4 ? 140 : -140);
                        _queue_spawn(type_goblin, base + fixed_point(_random.get_int(-20, 21), _random.get_int(-20, 21)),
                                     mode_chase, fixed_point(), 0);
                    }

                    _boss_state = wc_walk;
                    _boss_timer = 90;
                }
                break;

            default:
                break;
            }

            _boss->set_horizontal_flip(_boss_state == wc_charge || _boss_state == wc_stomp ? _boss_dir.x() < 0 : dx < 0);
            _boss->set_position(_boss_pos.x(), _boss_pos.y() - 16);

            if(_boss_shadow)
            {
                _boss_shadow->set_position(_boss_pos + fixed_point(0, 14));
                _boss_shadow->set_visible(_fx.shadows);
            }

            if(_boss_state != 0 && _boss_box_hit(to_int(_pos.x()), to_int(_pos.y()), player_half_box))
            {
                _hurt_player(15);
            }
        }

        // ---- Thornwood Treant (DISENO_FASES E5.1, block 3): roots with a 40 f crack every 150 f; < 66 % a thorn
        // wall of 16 sprites at r 150 around his entry point closing to r 110 in 10 s; < 33 % seeds (30 f glow,
        // 8 radial at 1.2 px/f), 4 wolves every 20 s and roots every 120 f. 3:00 "THE HEX MOON RISES": +50 %.
        // PROVISIONAL HP: E5.1 says 7000; R13 from the Warchief's measured ~21 dps at Lv 20 (TTK 60-120 s).
        static constexpr int treant_hp = 3000;
        enum treant_state
        {
            tr_walk = 21,
            tr_slam = 22,
            tr_glow = 23,
            tr_roar = 24
        };

        void _update_treant()
        {
            int pct = (_boss_hp * 100) / _boss_max_hp;
            int phase = pct < 33 ? 3 : pct < 66 ? 2 : 1;
            int dx = to_int(_pos.x() - _boss_pos.x());
            int dy = to_int(_pos.y() - _boss_pos.y());

            if(! _hex_moon && _boss_fight_t > 3 * 60 * fps)
            {
                _hex_moon = true;
                test_events = test_events | 1024 | (1 << 18);
                _show_banner("THE HEX MOON RISES", art::text_red, 3 * fps);
                _play(art::sfx_boss);
                _shake = 16;
            }

            fixed rage = _hex_moon ? fixed(1.5) : fixed(1);

            if(phase > _boss_phase && _boss_state != 0)
            {
                _boss_phase = phase;
                test_boss_phase = phase;
                _boss_invuln = 60;
                _boss_state = tr_roar;
                _boss_timer = 60;
                _shake = 20;
                _play(art::sfx_boss);
                _push_monsters_from_boss();

                if(phase == 2)
                {
                    // G-52.1: the wall closes around the wizard (never starts outside it).
                    _thorn_center = _pos;
                    _thorn_t = 0;
                    _thorn_hits_wall = 0;
                    test_events = test_events | (1 << 15);
                    _play(art::sfx_thorn, fixed(0.8));
                }
            }

            if(_boss_invuln > 0)
            {
                --_boss_invuln;
                _boss->set_palette((_boss_invuln & 4) ? bn::sprite_palette_item(_boss_white_colors, bn::bpp_mode::BPP_4) :
                                                        art::treant.palette_item());
            }

            switch(_boss_state)
            {
            case 0: // entrance (mosaic 1 -> 0 in 30 f)
                if(_fx.boss_mosaic)
                {
                    bn::sprites_mosaic::set_stretch(fixed(_boss_timer) / 30);
                }

                if(--_boss_timer <= 0)
                {
                    bn::sprites_mosaic::set_stretch(0);
                    _boss->set_mosaic_enabled(false);
                    _boss_state = tr_walk;
                }
                break;

            case tr_walk:
            {
                fixed speed = fixed(0.25) * rage;

                if(abs_int(dx) + abs_int(dy) > 160)
                {
                    speed *= 3;   // E5: never left behind
                }

                _boss_pos += unit(dx, dy) * speed;
                _set_boss_frame(art::treant_walk, _t, true);

                if(--_root_cd <= 0)
                {
                    _boss_state = tr_slam;
                    _boss_timer = 30;
                    _play(art::sfx_slam, fixed(0.9));
                }
                else if(_boss_phase >= 3 && --_seed_cd <= 0)
                {
                    _boss_state = tr_glow;
                    _boss_timer = 30;
                }
                break;
            }

            case tr_slam:
                _set_boss_frame(art::treant_slam, 30 - _boss_timer, false);

                if(_boss_timer == 12)
                {
                    _roots_pending = 3;
                    _shake = bn::max(_shake, 6);
                    _play(art::sfx_root_crack, fixed(0.8));
                    test_events = test_events | (1 << 14);
                }

                if(--_boss_timer <= 0)
                {
                    _root_cd = (_boss_phase >= 3 ? 120 : 150) * 2 / (_hex_moon ? 3 : 2);
                    _boss_state = tr_walk;
                }
                break;

            case tr_glow:
                _set_boss_frame(art::treant_idle, _t, true);
                _boss->set_palette((_boss_timer & 4) ? bn::sprite_palette_item(_boss_white_colors, bn::bpp_mode::BPP_4) :
                                                       art::treant.palette_item());

                if(--_boss_timer <= 0)
                {
                    _boss->set_palette(art::treant.palette_item());
                    _seeds_pending = 8;
                    _seed_cd = 5 * fps;
                    _boss_state = tr_walk;
                    test_events = test_events | (1 << 16);
                    _play(art::sfx_thorn, fixed(0.6));
                }
                break;

            case tr_roar:
                if(_boss_frame != art::treant_hurt)
                {
                    _boss_frame = art::treant_hurt;
                    _boss->set_tiles(art::treant.tiles_item(), art::treant_hurt);
                }

                if(--_boss_timer <= 0)
                {
                    _boss_state = tr_walk;
                }
                break;

            default:
                break;
            }

            if(_boss_phase >= 3 && --_wolf_cd <= 0)
            {
                _wolf_cd = 20 * fps;
                _make_room(4);
                fixed_point p = _spawn_point(130, 150, false);
                _play(art::sfx_howl, fixed(0.7));

                for(int k = 0; k < 4; ++k)
                {
                    _queue_spawn(type_wolf, p + fixed_point((k & 1) * 18, (k >> 1) * 18), mode_chase, fixed_point(), 0);
                }
            }

            _update_roots();
            _update_seeds();
            _update_thorns();

            _boss->set_horizontal_flip(dx < 0);
            _boss->set_position(_boss_pos.x(), _boss_pos.y() - 16);

            if(_boss_shadow)
            {
                _boss_shadow->set_position(_boss_pos + fixed_point(0, 14));
                _boss_shadow->set_visible(_fx.shadows);
            }

            if(_boss_state != 0 && _boss_box_hit(to_int(_pos.x()), to_int(_pos.y()), player_half_box))
            {
                _hurt_player(16);
            }
        }

        // Phase change (E5): monsters within 48 px of the boss are pushed out to 48 px.
        void _push_monsters_from_boss()
        {
            for(int i = 0, n = _enemies.size(); i < n; ++i)
            {
                enemy& e = _enemies[i];
                int ex = to_int(e.pos.x() - _boss_pos.x());
                int ey = to_int(e.pos.y() - _boss_pos.y());

                if(ex * ex + ey * ey < 48 * 48)
                {
                    e.pos = _boss_pos + (ex == 0 && ey == 0 ? fixed_point(1, 0) : unit(ex, ey)) * 48;
                    _grid_refresh(i);
                }
            }
        }

        // Roots: a crack for 40 f under / next to the wizard, then spikes r 12 for 20 f (14 damage).
        void _update_roots()
        {
            if(_roots_pending > 0 && ! _roots.full())
            {
                // One per frame (a new sprite costs ~5 % of a frame): under the wizard, then two beside him.
                fixed_point p = _pos;

                if(_roots_pending < 3)
                {
                    p += angle_vector(_random.get_int(2048)) * _random.get_int(16, 29);
                }

                --_roots_pending;
                bn::sprite_ptr sprite = _world_sprite(art::fx_root, p + fixed_point(0, 2), art::root_crack.first, 30);
                root_spike& r = _roots.emplace_back(bn::move(sprite));
                r.pos = p;
            }

            for(int k = 0; k < _roots.size();)
            {
                root_spike& r = _roots[k];
                ++r.t;
                int frame = r.t < 40 ? art::root_crack.frame(r.t, true) : art::root_spike.frame(r.t - 40, false);

                if(frame != r.frame)
                {
                    r.frame = frame;
                    r.sprite.set_tiles(art::fx_root.tiles_item(), frame);
                    r.sprite.set_z_order(r.t < 40 ? 30 : 9);
                }

                if(r.t >= 40 && r.t < 60)
                {
                    int dx = to_int(_pos.x() - r.pos.x());
                    int dy = to_int(_pos.y() - r.pos.y());

                    if(dx * dx + dy * dy <= (12 + player_half_box) * (12 + player_half_box))
                    {
                        _hurt_player(14);
                    }
                }

                if(r.t >= 60)
                {
                    _roots.erase(_roots.begin() + k);
                    continue;
                }

                ++k;
            }
        }

        // Seeds: 8 radial at 1.2 px/f from the treant, 8 damage, 150 f of life (2 created per frame).
        void _update_seeds()
        {
            for(int n = 0; n < 2 && _seeds_pending > 0 && ! _seeds.full(); ++n)
            {
                --_seeds_pending;
                int k = _seeds_pending;
                bn::sprite_ptr sprite = _world_sprite(art::fx_seed, _boss_pos, 0, 9);
                seed_shot& sd = _seeds.emplace_back(bn::move(sprite));
                sd.vel = angle_vector(k * 256) * fixed(1.2);
                sd.pos = _boss_pos + sd.vel * 8;
            }

            for(int k = 0; k < _seeds.size();)
            {
                seed_shot& sd = _seeds[k];
                sd.pos += sd.vel;
                sd.sprite.set_position(sd.pos);

                if((++sd.t & 7) == 0)
                {
                    sd.sprite.set_tiles(art::fx_seed.tiles_item(), (sd.t >> 3) & 1);
                }

                int dx = to_int(_pos.x() - sd.pos.x());
                int dy = to_int(_pos.y() - sd.pos.y());

                if(abs_int(dx) < 3 + player_half_box && abs_int(dy) < 3 + player_half_box)
                {
                    _hurt_player(8);
                    sd.t = 150;
                }

                if(sd.t >= 150)
                {
                    _seeds.erase(_seeds.begin() + k);
                    continue;
                }

                ++k;
            }
        }

        // Thorn wall: 16 sprites (2 created per frame, one shared tile block) on a ring that closes from r 150 to
        // r 110 in 10 s; outside it (or touching it) the wizard takes 6 every 30 f.
        // True if the wizard's centre at (x, y) is outside the wall's solid ellipse (thorn radius - 8 - margin).
        [[nodiscard]] bool _outside_wall(int x, int y, int margin) const
        {
            int ax = _thorn_rx - 8 - margin;
            int ay = _thorn_ry - 8 - margin;
            int dx = bn::clamp(x - to_int(_thorn_center.x()), -400, 400);
            int dy = bn::clamp(y - to_int(_thorn_center.y()), -400, 400);
            return dx * dx * ay * ay + dy * dy * ax * ax > ax * ax * ay * ay;
        }

        [[nodiscard]] bool _thorn_solid() const
        {
            return _thorn_t >= 0 && _thorn_t < thorn_wither;
        }

        void _update_thorns()
        {
            if(_thorn_t < 0)
            {
                return;
            }

            ++_thorn_t;

            if(_boss_phase >= 3 && _thorn_t < thorn_wither)
            {
                _thorn_t = thorn_wither;   // < 33 %: the wall withers at once
            }

            if(_thorn_t >= thorn_cycle)
            {
                if(_boss_phase == 2)
                {
                    _thorn_center = _pos;
                    _thorn_t = 0;
                    _thorn_hits_wall = 0;
                    _play(art::sfx_thorn, fixed(0.8));
                }
                else
                {
                    _thorn_t = -1;
                    return;
                }
            }

            int closing = bn::clamp(_thorn_t - thorn_grown, 0, 600);
            _thorn_rx = 104 - closing * 16 / 600;
            _thorn_ry = 56 - closing * 8 / 600;

            if(_thorn_t >= thorn_gone)
            {
                _thorns.clear();
                _thorn_tiles.reset();
                _thorn_pal.reset();
                return;
            }

            if(! _thorn_tiles)
            {
                _thorn_tiles = bn::sprite_tiles_ptr::create(art::fx_thorn.tiles_item(), 0);
                _thorn_pal = art::fx_thorn.palette_item().create_palette();
            }

            if((_thorn_t & 15) == 0)
            {
                _thorn_tiles->set_tiles_ref(art::fx_thorn.tiles_item(), (_thorn_t >> 4) & 1);
            }

            for(int n = 0; n < 6 && ! _thorns.full(); ++n)
            {
                bn::sprite_ptr sprite = bn::sprite_ptr::create(_thorn_center, art::fx_thorn.shape_size(), *_thorn_tiles,
                                                               *_thorn_pal);
                sprite.set_camera(_camera);
                sprite.set_bg_priority(2);
                sprite.set_z_order(9);
                _thorns.push_back(bn::move(sprite));
            }

            // PROVISIONAL (no sprout art yet): sprouts and withering are the thorn sprites blinking every 4 f.
            bool blink = _thorn_t < thorn_sprout || _thorn_t >= thorn_wither;
            bool visible = ! blink || (_thorn_t & 4);

            if(_thorn_t < thorn_sprout && (_thorn_t & 3) == 0 && _boss)
            {
                _boss->set_x(_boss->x() + ((_thorn_t & 4) ? 1 : -1));   // the Treant trembles 1 px during the sprouts
            }

            bool onscreen_check = _cam_lock == 20 && _thorns.full() && _thorn_t < thorn_wither;
            int on = 0;
            int cam_x = to_int(_camera.x());
            int cam_y = to_int(_camera.y());

            for(int k = 0; k < _thorns.size(); ++k)
            {
                fixed_point v = angle_vector(k * 2048 / thorn_count);
                fixed_point p(_thorn_center.x() + v.x() * _thorn_rx, _thorn_center.y() + v.y() * _thorn_ry);

                if(_thorn_t < 40 || (_thorn_t & 1) == 0)
                {
                    _thorns[k].set_position(p);
                }

                _thorns[k].set_visible(visible && ! _cards_shown);

                if(onscreen_check)
                {
                    int sx = to_int(p.x()) - cam_x + 120;
                    int sy = to_int(p.y()) - cam_y + 80;
                    on += sx - 8 >= 0 && sx + 8 <= 240 && sy - 8 >= 24 && sy + 8 <= 160;
                }
            }

            if(onscreen_check)
            {
                test_wall_onscreen = bn::min(int(test_wall_onscreen), on);
            }

            if(_thorn_hurt_cd > 0)
            {
                --_thorn_hurt_cd;
            }

            if(_thorn_solid())
            {
                int px = to_int(_pos.x());
                int py = to_int(_pos.y());

                // Defence, not a mechanic: if he ever ends outside (the ellipse shrank), he is put back inside.
                for(int guard = 0; guard < 12 && _outside_wall(px, py, 0); ++guard)
                {
                    _pos = _thorn_center + (_pos - _thorn_center) * fixed(0.9);
                    px = to_int(_pos.x());
                    py = to_int(_pos.y());
                }

                if(_thorn_t >= thorn_grown && _thorn_hurt_cd == 0 && _outside_wall(px, py, 3))
                {
                    // Touching the thorns: 6 damage every 30 f and an 8 px push toward the centre.
                    if(test_thorn_first_hurt < 0 || _thorn_t < test_thorn_first_hurt)
                    {
                        test_thorn_first_hurt = _thorn_t;
                    }

                    ++_thorn_hits_wall;
                    test_thorn_hits_max = bn::max(int(test_thorn_hits_max), _thorn_hits_wall);
                    _hurt_player(6);
                    _thorn_hurt_cd = 30;
                    int dx = to_int(_thorn_center.x()) - px;
                    int dy = to_int(_thorn_center.y()) - py;

                    if(dx || dy)
                    {
                        fixed_point np = _pos + unit(dx, dy) * 8;

                        if(! _blocked(np.x(), np.y()))
                        {
                            _pos = np;
                        }
                    }
                }
            }

            test_thorn_radius = _thorn_rx;
        }

        void _clear_treant_attacks()
        {
            _roots.clear();
            _seeds.clear();
            _thorns.clear();
            _thorn_tiles.reset();
            _thorn_pal.reset();
            _thorn_t = -1;
            _roots_pending = 0;
            _seeds_pending = 0;
        }

        void _boss_land(bool phase2)
        {
            _boss_state = 4;
            _boss_timer = 30;
            _boss->set_tiles(art::slime_king.tiles_item(), art::king_land.first);
            _shake = 16;
            _play(art::sfx_stomp, fixed(0.8));

            _boss_wave = _world_sprite(art::wave, _boss_pos + fixed_point(0, 8), 0, 29);
            _boss_wave->set_blending_enabled(true);
            _boss_wave_t = 0;

            int dx = to_int(_pos.x() - _boss_pos.x());
            int dy = to_int(_pos.y() - _boss_pos.y());

            if(dx * dx + dy * dy <= 36 * 36)
            {
                _hurt_player(12);
            }

            if(phase2)
            {
                for(int i = 0; i < 4; ++i)
                {
                    fixed_point p = _boss_pos + angle_vector(i * 512 + 256) * 40;

                    if(! _enemies.full())
                    {
                        _spawn_enemy(type_slime, p, 0);
                    }
                }
            }
        }

        void _kill_boss()
        {
            _spawn_poof(_boss_pos, 6);
            _spawn_gem(_boss_pos, 25);
            _spawn_pickup(_boss_pos + fixed_point(12, 0), pickup_coin, 10, gold_bosses);
            _boss.reset();
            _boss_shadow.reset();
            _boss_bar.clear();
            _boss_warn.clear();
            _boss_bar_shown = -1;
            _boss_hp = 0;
            _shake = 20;
            _level_flash = 20;
            _play(art::sfx_explosion);
            bn::sprites_mosaic::set_stretch(0);
            test_boss_ttk = _boss_fight_t / fps;

            constexpr art::anim deaths[6] = { art::king_land, art::warchief_horn, art::treant_death, art::colossus_death,
                                              art::banshee_death, art::lich_death };
            _clear_stage2_attacks();

            if(_boss_kind >= 3)
            {
                _spawn_particle(_boss_pos + fixed_point(0, -16), fixed_point(), _boss_item(), deaths[_boss_kind],
                                deaths[_boss_kind].count * deaths[_boss_kind].frame_time, 8);
            }

            if(_boss_kind % 3 == 2)
            {
                // E5.4: slow motion 30 f, white flash, every normal monster poofs (no XP), every gem flies to the
                // wizard; then "STAGE CLEAR" 3 s with the jingle and the summary.
                if(_boss_kind == 2)
                {
                    _spawn_particle(_boss_pos + fixed_point(0, -16), fixed_point(), art::treant, art::treant_death, 32, 8);
                }

                _clear_treant_attacks();
                _boss_dead = true;
                ++_bosses_killed;
                test_events = test_events | (1 << 17);
                _slowmo_t = 60;   // 30 logic frames at 1/2
                _level_flash = 40;
                _end_timer = 60 + 3 * fps;
                bn::music::stop();

                for(int i = _enemies.size() - 1; i >= 0; --i)
                {
                    if(i < 6)
                    {
                        _spawn_poof(_enemies[i].pos, 2);
                    }

                    _remove_enemy(i);
                }

                for(pickup& g : _pickups)
                {
                    g.flying = g.kind == pickup_gem || g.kind == pickup_coin;
                }

                return;
            }

            if(_boss_kind == 1)
            {
                test_events = test_events | 256;
            }

            // Boss chest (§C3 cartas): 1 (60 %), 3 (30 %) or 5 (10 %) upgrades; 8 s rest (E1); the boss jingle with
            // the music stopped, then the meadow music.
            int roll = _random.get_int(10);
            _spawn_pickup(_boss_pos + fixed_point(-12, 0), pickup_chest, roll < 6 ? 1 : roll < 9 ? 3 : 5);
            ++_bosses_killed;
            _rest_t = 8 * fps;
            bn::music::stop();
            _play(art::jingle_boss_down);
            _music_resume_t = 3 * fps;
        }

        // -----------------------------------------------------------------------------------------------------
        // Pickups, gems, particles

        void _spawn_gem(const fixed_point& p, int value)
        {
            int gems = 0;
            int far_i = -1;
            int far_d = -1;

            for(int i = 0, n = _pickups.size(); i < n; ++i)
            {
                if(_pickups[i].kind != pickup_gem)
                {
                    continue;
                }

                ++gems;
                int d = abs_int(to_int(_pickups[i].pos.x() - _pos.x())) + abs_int(to_int(_pickups[i].pos.y() - _pos.y()));

                if(d > far_d)
                {
                    far_d = d;
                    far_i = i;
                }
            }

            // Over the cap (or the sprite creation budget) the value goes to the farthest gem (§13).
            if(gems >= max_gems || _pickups.full() || (_create_budget <= 0 && far_i >= 0))
            {
                if(far_i >= 0)
                {
                    pickup& g = _pickups[far_i];
                    g.value += value;
                    g.sprite.set_tiles(art::gems.tiles_item(), _gem_frame(g.value));
                }
                else
                {
                    _add_xp(value);
                }

                return;
            }

            --_create_budget;

            if(_spawn_pickup(p, pickup_gem, value) && ! _gem_hint_done)
            {
                _gem_hint_done = true;
                _show_world_hint("GRAB GEMS", p, 2 * fps);
            }
        }

        [[nodiscard]] static int _gem_frame(int value)
        {
            return value >= 25 ? art::gem_red : value >= 5 ? art::gem_green : art::gem_blue;
        }

        bool _spawn_pickup(const fixed_point& p, int kind, int value, int source = gold_drops)
        {
            if(_pickups.full())
            {
                return false;
            }

            bn::optional<bn::sprite_ptr> s;

            if(kind == pickup_chest || kind == pickup_cursed)
            {
                s = art::chest.create_sprite_optional(p, kind == pickup_cursed ? art::chest_cursed_closed : 0);
            }
            else if(kind == pickup_heart)
            {
                s = art::heart.create_sprite_optional(p, 0);
            }
            else
            {
                s = kind == pickup_coin ? art::coin.create_sprite_optional(p, 0) : art::gems.create_sprite_optional(p, _gem_frame(value));
            }

            if(! s)
            {
                return false;
            }

            s->set_camera(_camera);
            s->set_bg_priority(2);
            s->set_z_order(20);
            pickup& g = _pickups.emplace_back(bn::move(*s));
            g.pos = p;
            g.kind = uint8_t(kind);
            g.value = value;
            g.source = uint8_t(source);
            g.speed = fixed(1.5);
            return true;
        }

        // Heart (heals 30): never more than 2 on the ground (§C1).
        void _spawn_heart(const fixed_point& p)
        {
            int hearts = 0;

            for(const pickup& g : _pickups)
            {
                hearts += g.kind == pickup_heart;
            }

            if(hearts < 2)
            {
                _spawn_pickup(p, pickup_heart, 30);
            }
        }

        void _update_pickups()
        {
            // Lodestone: +25 % pickup radius per level (§C3 pasivas).
            int radius = _blessing == bless_magnet ? 400 :
                         pickup_radius * (4 + _levels[up_lodestone]) * (100 + 15 * _shop[shop_magnet]) / 400;
            int r2 = radius * radius;

            if(_gem_combo_t > 0 && --_gem_combo_t == 0)
            {
                _gem_combo = 0;
            }

            for(int i = 0; i < _pickups.size();)
            {
                pickup& g = _pickups[i];
                int dx = to_int(_pos.x() - g.pos.x());
                int dy = to_int(_pos.y() - g.pos.y());
                int d2 = dx * dx + dy * dy;

                if(g.kind == pickup_chest || g.kind == pickup_heart || g.kind == pickup_cursed)
                {
                    if(d2 < 12 * 12)
                    {
                        if(g.kind == pickup_heart)
                        {
                            _player_hp = bn::min(_player_max_hp, _player_hp + g.value);
                        }
                        else if(g.kind == pickup_cursed)
                        {
                            _open_cursed_chest(g.pos);
                        }
                        else
                        {
                            // §C3: a ready evolution comes first and takes one of the chest's items.
                            _free_upgrades += _try_evolve() ? g.value - 1 : g.value;
                        }

                        _play(g.kind == pickup_heart ? art::sfx_gem : art::sfx_chest, fixed(0.6));
                        _pickups[i] = bn::move(_pickups.back());
                        _pickups.pop_back();
                        continue;
                    }

                    ++i;
                    continue;
                }

                if(! g.flying && d2 < r2)
                {
                    g.flying = true;
                }

                if(g.flying)
                {
                    g.speed = bn::min(g.speed + fixed(0.15), gem_fly_speed + (_blessing == bless_magnet ? 2 : 0));
                    g.pos += unit(dx, dy) * g.speed;
                    g.sprite.set_position(g.pos);

                    if(_fx.gem_trail && ((_t + i) & 3) == 0)
                    {
                        _spawn_particle(g.pos, fixed_point(), art::tmp_fx, { art::tmp_trail, 1, 8 }, 8, 22);
                    }

                    if(d2 < 6 * 6)
                    {
                        _collect(g);
                        _pickups[i] = bn::move(_pickups.back());
                        _pickups.pop_back();
                        continue;
                    }
                }

                ++i;
            }
        }

        void _collect(const pickup& g)
        {
            if(g.kind == pickup_coin)
            {
                _gold += g.value;
                _gold_from[g.source] += g.value;
                _play(art::sfx_gem, fixed(0.5), fixed(0.8));
                return;
            }

            _add_xp(g.value);
            _add_nova(1);
            fixed pitch = 1 + fixed(0.03) * bn::min(_gem_combo, 8);   // <= 1.24: higher rates corrupted memory
            _play(art::sfx_gem, fixed(0.5), pitch);
            ++_gem_combo;
            _gem_combo_t = 30;
        }

        void _add_xp(int value)
        {
            // Shop Growth: +4 % XP per rank, kept in hundredths so small gems still count.
            _xp_frac += value * (100 + 4 * _shop[shop_growth]);
            _xp += _xp_frac / 100;
            _xp_frac %= 100;

            while(_xp >= xp_needed(_level + _pending_levels))
            {
                _xp -= xp_needed(_level + _pending_levels);
                ++_pending_levels;
            }
        }

        void _spawn_particle(const fixed_point& p, const fixed_point& v, const bn::sprite_item& item,
                             const art::anim& frames, int life, int z, bool flip = false)
        {
            if(_particles.full())
            {
                return;
            }

            bn::optional<bn::sprite_ptr> s = item.create_sprite_optional(p, frames.first);

            if(! s)
            {
                return;
            }

            s->set_camera(_camera);
            s->set_bg_priority(2);
            s->set_z_order(z);
            s->set_horizontal_flip(flip);
            particle& pt = _particles.emplace_back(bn::move(*s));
            pt.pos = p;
            pt.vel = v;
            pt.life = life;
            pt.item = &item;
            pt.frames = frames;
        }

        void _spawn_poof(const fixed_point& p, int count)
        {
            _play(art::sfx_poof, fixed(0.4));

            // perf: poofs only while the frame's sprite creation budget lasts (kills in bulk cost ~5 % each).
            count = bn::min(count, _create_budget - 1);

            for(int i = 0; i < count; ++i)
            {
                --_create_budget;
                fixed_point v = angle_vector(i * (2048 / count) + 256) * fixed(0.8);
                _spawn_particle(p + v * 2, v, art::fx_poof, { 0, 4, 3 }, 12, 15);
            }
        }

        void _spawn_hit_spark(const fixed_point& p)
        {
            if(_fx.hit_flash)
            {
                _spawn_particle(p, fixed_point(), art::fx_impact, { 0, 3, 2 }, 6, 2);
            }
        }

        void _update_particles()
        {
            for(int i = 0; i < _particles.size();)
            {
                particle& pt = _particles[i];

                if(--pt.life <= 0)
                {
                    _particles[i] = bn::move(_particles.back());
                    _particles.pop_back();
                    continue;
                }

                pt.pos += pt.vel;
                pt.sprite.set_position(pt.pos);

                ++pt.age;

                if(pt.frames.count > 1 && pt.age % pt.frames.frame_time == 0)
                {
                    pt.sprite.set_tiles(pt.item->tiles_item(), pt.frames.frame(pt.age, false));
                }

                ++i;
            }

            for(int i = 0; i < _numbers.size();)
            {
                damage_number& d = _numbers[i];

                if(--d.life <= 0)
                {
                    _numbers[i] = bn::move(_numbers.back());
                    _numbers.pop_back();
                    continue;
                }

                for(bn::sprite_ptr& s : d.sprites)
                {
                    s.set_y(s.y() - fixed(0.5));
                }

                ++i;
            }
        }

        void _spawn_damage_number(const fixed_point& p, int value)
        {
            // perf: text generation is the most expensive part of a hit; at most one new number per frame.
            if(_numbers.full() || _number_frame == _t)
            {
                return;
            }

            _number_frame = _t;

            damage_number& d = _numbers.emplace_back();
            _text.set_center_alignment();

            if(! _text.generate_optional(p.x(), p.y() - 10, bn::to_string<6>(value), d.sprites))
            {
                _numbers.pop_back();
                return;
            }

            for(bn::sprite_ptr& s : d.sprites)
            {
                s.set_camera(_camera);
                s.set_bg_priority(1);
            }

            d.life = 30;
        }

        // -----------------------------------------------------------------------------------------------------
        // Shrines (§4 2a)

        void _spawn_shrine()
        {
            _shrine_pos = _spawn_point(160, 220, false);
            _shrine = _world_sprite(art::shrine, _shrine_pos, 0, 12);
            bn::sprite_ptr arrow = art::edge_arrow.create_sprite(0, 0, art::arrow_east);
            arrow.set_bg_priority(0);
            _shrine_arrow = bn::move(arrow);
            _shrine_touch = 0;
        }

        void _update_shrine()
        {
            _update_treasure_arrow();

            if(! _shrine)
            {
                return;
            }

            int dx = to_int(_shrine_pos.x() - _pos.x());
            int dy = to_int(_shrine_pos.y() - _pos.y());

            if(_shrine_arrow)
            {
                _place_edge_arrow(*_shrine_arrow, dx, dy, 0);
            }

            if(abs_int(dx) < 12 && abs_int(dy) < 14)
            {
                ++_shrine_touch;
                if(_shrine_touch & 4)
                {
                    _shrine->set_palette(_white_palette());
                }
                else
                {
                    _shrine->set_palette(art::shrine.palette_item());
                }

                if(_shrine_touch >= fps)
                {
                    _shrine.reset();
                    _shrine_arrow.reset();
                    _open_shrine_pick();
                }
            }
            else if(_shrine_touch)
            {
                _shrine_touch = 0;
                _shrine->set_palette(art::shrine.palette_item());
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Timers, HUD, screen fades

        void _update_timers()
        {
            if(_hit_sound_cd)
            {
                --_hit_sound_cd;
            }

            if(_hint_t > 0 && --_hint_t == 0)
            {
                _hint.clear();
            }

            if(_world_hint_t > 0 && --_world_hint_t == 0)
            {
                _world_hint.clear();
            }

            if(_blessing >= 0)
            {
                if(--_blessing_t <= 0)
                {
                    _blessing = -1;
                    _hud_blessing.reset();
                }
                else if(_hud_blessing)
                {
                    _hud_blessing->set_visible(_blessing_t > 5 * fps || (_blessing_t & 8));
                }
            }
        }

        void _update_screen_fade()
        {
            if(_nova_flash > 0)
            {
                --_nova_flash;
                fixed intensity = _fx.level_flash ? fixed(_nova_flash) / 30 : fixed(0);
                bn::bg_palettes::set_fade(bn::color(28, 22, 31), intensity);
                bn::sprite_palettes::set_fade(bn::color(31, 28, 31), intensity / 2);
                return;
            }

            bn::sprite_palettes::set_fade(bn::color(0, 0, 0), 0);

            if(_summary_shown)
            {
                bn::bg_palettes::set_fade(bn::color(0, 0, 0), fixed(0.6));
                return;
            }

            if(_cards_shown)
            {
                bn::bg_palettes::set_fade(bn::color(0, 0, 0), fixed(0.5));
                return;
            }

            if(_level_flash > 0)
            {
                --_level_flash;

                if(_fx.level_flash)
                {
                    bn::bg_palettes::set_fade(bn::color(31, 31, 31), fixed(_level_flash) / 40);
                    return;
                }
            }

            if(_stage == 1 && (_fx.dusk || (_boss && _boss_kind == 5 && _boss_phase == 3)))
            {
                fixed dusk = _fx.dusk ? fixed(0.2) + fixed(0.25) * bn::min(_t, 15 * 60 * fps) / (15 * 60 * fps) : 0;

                if(_boss && _boss_kind == 5 && _boss_phase == 3)
                {
                    dusk = bn::max(dusk, fixed(0.5));   // E5.2 Lich < 33 %: death field
                }

                bn::bg_palettes::set_fade(bn::color(9, 3, 14), dusk);
            }
            else if(_fx.dusk)
            {
                // 0 -> 0.35 towards dark blue over the full 15 minute stage (§12).
                fixed dusk = fixed(0.35) * bn::min(_t, 15 * 60 * fps) / (15 * 60 * fps);
                bn::bg_palettes::set_fade(bn::color(2, 3, 12), dusk);
            }
            else
            {
                bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
            }
        }

        void _refresh_hud(bool force)
        {
            int need = xp_needed(_level);
            int fill = (bn::min(_xp, need) * 32 * art::xp_pieces) / need;

            for(int i = 0; i < art::xp_pieces; ++i)
            {
                _xp_bar[i].set_tiles(art::xp_bar.tiles_item(), bn::max(0, bn::min(32, fill - i * 32)) / 2);
            }

            if(_nova_bar.empty())
            {
                bn::sprite_ptr meter = art::nova_meter.create_sprite(-108, 70, 0);
                meter.set_bg_priority(0);
                _nova_bar.push_back(bn::move(meter));
            }

            bool ready = _nova >= nova_max;
            int meter_frame = ready ? art::nova_meter_full.frame(_t, true) :
                                      bn::min(art::nova_meter_charge_frames - 1,
                                              (_nova * art::nova_meter_charge_frames) / nova_max);
            _nova_bar[0].set_tiles(art::nova_meter.tiles_item(), meter_frame);

            int nova_state = ready ? 1 + ((_t >> 4) & 1) : 0;

            if(force || nova_state != _last_nova_shown)
            {
                _last_nova_shown = nova_state;
                _nova_text.clear();
                _text.set_left_alignment();

                if(nova_state == 1)
                {
                    _text.set_palette_item(art::text_gold);
                    _text.generate(-98, 71, "A", _nova_text);
                    _text.set_palette_item(common::variable_8x8_sprite_font.item().palette_item());
                }
            }

            int s = _t / fps;

            if(force || s != _last_clock_s)
            {
                _last_clock_s = s;
                bn::string<8> clock;
                int m = s / 60;
                int sec = s % 60;
                clock.push_back(char('0' + m / 10));
                clock.push_back(char('0' + m % 10));
                clock.push_back(':');
                clock.push_back(char('0' + sec / 10));
                clock.push_back(char('0' + sec % 10));
                _hud_clock.clear();
                _text.set_center_alignment();
                _text.generate(0, -64, clock, _hud_clock);
            }

            if(force || _level != _last_level_shown)
            {
                _last_level_shown = _level;
                bn::string<12> lv("LV ");
                lv.append(bn::to_string<4>(_level));
                _hud_level.clear();
                _text.set_left_alignment();
                _text.generate(-116, -64, lv, _hud_level);
            }

            if(force || _gold != _last_gold_shown)
            {
                _last_gold_shown = _gold;
                bn::string<12> g("G ");
                g.append(bn::to_string<6>(_gold));
                _hud_gold.clear();
                _text.set_right_alignment();
                _text.generate(116, -64, g, _hud_gold);
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Cards: level up (§10) and shrine blessings (§4 2a)

        void _hide_world_extras(bool hide)
        {
            // Review block 2: combat effects never over the cards or the pause.
            for(bn::sprite_ptr& f : _frost)
            {
                f.set_visible(! hide && _frost_t >= 0);
            }

            if(hide)
            {
                for(bn::sprite_ptr& c : _chain_sprites)
                {
                    c.set_visible(false);
                }

                for(ember_ball& b : _embers)
                {
                    b.sprite.set_visible(false);
                }
            }

            for(fire_patch& f : _fires)
            {
                f.sprite.set_visible(! hide);
            }

            for(root_spike& r : _roots)
            {
                r.sprite.set_visible(! hide);
            }

            for(seed_shot& sd : _seeds)
            {
                sd.sprite.set_visible(! hide);
            }

            for(bn::sprite_ptr& t : _thorns)
            {
                t.set_visible(! hide);
            }

            // Banners (CURSED!, boss names) wait until the cards close.
            for(bn::sprite_ptr& b : _boss_title)
            {
                b.set_visible(! hide);
            }

            for(pickup& g : _pickups)
            {
                g.sprite.set_visible(! hide);
            }

            if(hide)
            {
                // G-55 (§C6 VRAM <= 900 with the cards open): short-lived effects are dropped instead of hidden, so
                // their tiles are free for the cards. Nobody sees them go: the world is hidden behind the menu.
                _particles.clear();
                _numbers.clear();
            }

            for(bn::sprite_ptr& s : _hint)
            {
                s.set_visible(! hide);
            }

            for(bn::sprite_ptr& s : _world_hint)
            {
                s.set_visible(! hide);
            }
        }

        void _open_level_up()
        {
            // Build the offer: anything not maxed, shuffled; fill with Heal / Gold (§9, never an empty menu).
            uint8_t pool[up_count];
            int pool_n = 0;

            int weapons = 0;
            int passives = 0;

            for(int i = 0; i < weapon_or_passive_count; ++i)
            {
                if(_levels[i])
                {
                    (i < up_boots ? weapons : passives) += 1;
                }
            }

            for(int i = 0; i < weapon_or_passive_count; ++i)
            {
                bool slot_free = _levels[i] > 0 || (i < up_boots ? weapons : passives) < max_slots;

                if(_levels[i] < max_upgrade_level && slot_free && (_levels[i] || _upgrade_unlocked(i)))
                {
                    pool[pool_n++] = uint8_t(i);
                }
            }

            for(int i = pool_n - 1; i > 0; --i)
            {
                int j = _random.get_int(i + 1);
                uint8_t tmp = pool[i];
                pool[i] = pool[j];
                pool[j] = tmp;
            }

            _card_count = 3;

            for(int i = 0; i < 3; ++i)
            {
                _card_ids[i] = i < pool_n ? pool[i] : uint8_t(i == pool_n ? up_heal : up_gold);
            }

            if(pool_n == 0)
            {
                _card_count = 2;
                _card_ids[0] = up_heal;
                _card_ids[1] = up_gold;
            }

            _mode = game_mode::level_up;
            _cursor = 0;
            _level_flash = 20;
            _play(art::sfx_level);
            _hide_world_extras(true);
            _menu.clear();

            // Burst behind the wizard for 24 frames, then the cards (§10 "subida de nivel vistosa").
            _burst = _world_sprite(art::level_burst, _pos + fixed_point(0, -8), 0, 6);
            _burst_t = 0;
        }

        void _open_shrine_pick()
        {
            int a = _random.get_int(bless_count);
            int b = (a + 1 + _random.get_int(bless_count - 1)) % bless_count;
            _card_ids[0] = uint8_t(a);
            _card_ids[1] = uint8_t(b);
            _card_count = 2;
            _mode = game_mode::shrine_pick;
            _cursor = 0;
            _level_flash = 12;
            _play(art::sfx_level, fixed(0.8));
            _hide_world_extras(true);
            _draw_cards();
        }

        [[nodiscard]] int _card_x(int i) const
        {
            return _card_count == 3 ? -76 + i * 76 : -40 + i * 80;
        }

        void _draw_cards()
        {
            _menu.clear();
            bool shrine = _mode == game_mode::shrine_pick;
            _big_text.set_center_alignment();
            _big_text.set_palette_item(art::text_gold);
            _big_text.generate(0, -56, shrine ? "SHRINE" : "LEVEL UP!", _menu);
            _big_text.set_palette_item(common::variable_16x16_sprite_font.item().palette_item());
            _text.set_center_alignment();

            if(shrine)
            {
                _text.generate(0, 72, "Pick a blessing: D-PAD + A", _menu);
            }
            else if(_rerolls > 0)
            {
                bn::string<32> line(_pick_hint_done ? "R: REROLL x" : "D-PAD + A    R: REROLL x");
                line.append(bn::to_string<4>(_rerolls));
                _text.generate(0, 72, line, _menu);
            }
            else if(! _pick_hint_done)
            {
                _text.generate(0, 72, "PICK ONE: D-PAD + A", _menu);
            }

            for(bn::sprite_ptr& s_hud : _hud_frame)
            {
                s_hud.set_visible(false);
            }

            // G-55 (VRAM): the clock is dropped and redrawn when the cards close (_last_clock_s = -1).
            _hud_clock.clear();
            _last_clock_s = -1;

            // Card = card (64x64) + card_base (64x32) 48 px lower; texts inside the parchment (art/LÉEME.md).
            _text.set_palette_item(art::text_ink);

            if(! _cards_shown)
            {
                _set_cards_world(true);
            }

            _card_blink = 0;

            for(int i = 0; i < _card_count; ++i)
            {
                int x = _card_x(i);
                bool selected = i == _cursor;
                int cy = selected ? -8 : -4;   // the selected card stands 4 px higher and blinks (§19.7)

                if(selected)
                {
                    _sel_card_sprite = _menu.size();
                }

                bn::sprite_ptr c = art::card.create_sprite(x, cy, selected ? 1 : 0);
                c.set_bg_priority(1);
                _menu.push_back(bn::move(c));
                bn::sprite_ptr base = art::card_base.create_sprite(x, cy + 48, selected ? 1 : 0);
                base.set_bg_priority(1);
                _menu.push_back(bn::move(base));

                const upgrade_text& u = shrine ? blessing_texts[_card_ids[i]] : upgrade_texts[_card_ids[i]];
                bn::sprite_ptr icon = u.icon_item->create_sprite(x, cy - 14, u.icon);
                icon.set_bg_priority(0);
                _menu.push_back(bn::move(icon));

                // Name in two lines (the parchment is ~48 px wide).
                bn::string_view name(u.name);
                int space = -1;

                for(int c_i = 0, n = name.size(); c_i < n; ++c_i)
                {
                    if(name[c_i] == ' ')
                    {
                        space = c_i;
                        break;
                    }
                }

                if(space >= 0)
                {
                    _text.generate(x, cy, name.substr(0, space), _menu);
                    _text.generate(x, cy + 9, name.substr(space + 1), _menu);
                }
                else
                {
                    _text.generate(x, cy + 4, name, _menu);
                }

                if(shrine)
                {
                    _text.generate(x, cy + 22, u.desc[1], _menu);
                    _text.generate(x, cy + 31, u.desc[0], _menu);
                }
                else
                {
                    int id = _card_ids[i];
                    int lv = id < weapon_or_passive_count ? _levels[id] : 0;

                    if(id < weapon_or_passive_count)
                    {
                        bn::string<8> l("Lv ");
                        l.append(bn::to_string<2>(lv + 1));
                        _text.generate(x, cy + 22, lv == 0 ? bn::string<8>("NEW!") : l, _menu);
                    }

                    _text.generate(x, cy + 31, u.desc[lv], _menu);
                }
            }

            _text.set_palette_item(common::variable_8x8_sprite_font.item().palette_item());

            if(_pick_hint_done && ! shrine)
            {
                _text.generate(0, 72, "D-PAD + A", _menu);
            }
        }
        // Cards on screen: background dimmed, enemies and weapon effects hidden behind the cards (§19.7).
        void _set_cards_world(bool cards)
        {
            _cards_shown = cards;

            for(enemy& e : _enemies)
            {
                e.sprite.set_visible(! cards);

                if(e.aura)
                {
                    e.aura->set_visible(! cards);
                }
            }

            for(bolt& b : _bolts)
            {
                b.sprite.set_visible(! cards);
            }

            for(bn::sprite_ptr& r : _runes)
            {
                r.set_visible(! cards);
            }

            if(_frost_t >= 0)
            {
                for(bn::sprite_ptr& f : _frost)
                {
                    f.set_visible(! cards);
                }
            }

            _wizard.set_visible(! cards);
            _wizard_shadow.set_visible(! cards && _fx.shadows);
            _hp_bar.set_visible(! cards);
        }

        void _push_enemies_out(int radius)
        {
            for(int i = 0, n = _enemies.size(); i < n; ++i)
            {
                enemy& e = _enemies[i];
                int dx = to_int(e.pos.x() - _pos.x());
                int dy = to_int(e.pos.y() - _pos.y());

                if(dx * dx + dy * dy < radius * radius)
                {
                    fixed_point dir = dx == 0 && dy == 0 ? fixed_point(1, 0) : unit(dx, dy);
                    e.pos = _pos + dir * radius;
                    _grid_refresh(i);
                }
            }
        }

        void _update_cards()
        {
            if(_burst)
            {
                ++_burst_t;
                int frame = _burst_t < 6 ? _burst_t / 2 : 2 + ((_burst_t >> 2) & 1);
                _burst->set_tiles(art::level_burst.tiles_item(), frame);

                if(_burst_t < 24)
                {
                    return;
                }

                if(_burst_t == 24)
                {
                    if(_card_count == 3)
                    {
                        // G-55 (VRAM <= 900): the middle card covers the 64x64 burst entirely; free its 64 tiles.
                        _burst.reset();
                    }

                    _draw_cards();
                    return;
                }
            }

            if(_cards_shown && _sel_card_sprite >= 0 && _sel_card_sprite + 1 < _menu.size())
            {
                ++_card_blink;
                int frame = (_card_blink & 8) ? 0 : 1;
                _menu[_sel_card_sprite].set_tiles(art::card.tiles_item(), frame);
                _menu[_sel_card_sprite + 1].set_tiles(art::card_base.tiles_item(), frame);
            }

            if(bn::keypad::r_pressed() && _rerolls > 0 && _mode == game_mode::level_up && _cards_shown)
            {
                // Shop Reroll (§C3 "Cartas"): a new hand.
                --_rerolls;
                _open_level_up();
                _burst.reset();
                _burst_t = 0;
                _draw_cards();
                return;
            }

            if(bn::keypad::left_pressed() && _cursor > 0)
            {
                --_cursor;
                _play(art::sfx_menu, fixed(0.5));
                _draw_cards();
            }
            else if(bn::keypad::right_pressed() && _cursor < _card_count - 1)
            {
                ++_cursor;
                _play(art::sfx_menu, fixed(0.5));
                _draw_cards();
            }
            else if(bn::keypad::a_pressed())
            {
                if(_mode == game_mode::shrine_pick)
                {
                    _apply_blessing(_card_ids[_cursor]);
                }
                else
                {
                    _apply_upgrade(_card_ids[_cursor]);
                    _pick_hint_done = true;

                    if(_free_upgrades > 0)
                    {
                        --_free_upgrades;
                    }
                    else if(_pending_levels > 0)
                    {
                        --_pending_levels;
                        ++_level;
                    }
                }

                _menu.clear();
                _burst.reset();
                _burst_t = 0;

                for(bn::sprite_ptr& s_hud : _hud_frame)
                {
                    s_hud.set_visible(true);
                }
                _hide_world_extras(false);
                _set_cards_world(false);
                _mode = game_mode::play;
                _refresh_hud(true);
                _grace = 45;
                _push_enemies_out(32);

                if(_pending_levels > 0 || _free_upgrades > 0)
                {
                    _open_level_up();   // chained level ups (§10)
                }
            }
        }

        [[nodiscard]] bool _upgrade_unlocked(int id) const
        {
            switch(id)
            {
            case up_ember:
                return _unlocks & unlock_ember;
            case up_chain:
                return _unlocks & unlock_chain;
            case up_lens:
                return _unlocks & unlock_lens;
            case up_ward:
                return _unlocks & unlock_ward;
            case up_lance:
                return _unlocks & unlock_lance;
            default:
                return true;
            }
        }

        void _apply_upgrade(int id)
        {
            switch(id)
            {
            case up_heal:
                _player_hp = bn::min(_player_max_hp, _player_hp + 30);
                break;

            case up_gold:
                _gold += 10;
                _gold_from[gold_cards] += 10;
                break;

            case up_heart:
                ++_levels[id];
                _player_max_hp += 20;
                _player_hp = bn::min(_player_max_hp, _player_hp + 20);
                break;

            default:
                ++_levels[id];

                if(id == up_frost)
                {
                    _ensure_frost_shards();
                }
                else if(id == up_spark)
                {
                    _ensure_bolt_sprites();
                }
                break;
            }
        }

        void _apply_blessing(int id)
        {
            if(id == bless_mend)
            {
                _player_hp = bn::min(_player_max_hp, _player_hp + 40);
                return;
            }

            _blessing = id;
            _blessing_t = blessing_frames;
            bn::sprite_ptr icon = blessing_texts[id].icon_item->create_sprite(108, -50, blessing_texts[id].icon);
            icon.set_bg_priority(0);
            _hud_blessing = bn::move(icon);
        }

        // -----------------------------------------------------------------------------------------------------
        // Pause (START) and FX menu (SELECT, §12)

        void _open_pause()
        {
            _mode = game_mode::pause;
            _cursor = 0;
            _hide_world_extras(true);
            _draw_pause();
        }

        void _draw_pause()
        {
            _menu.clear();
            _big_text.set_center_alignment();
            _big_text.generate(0, -64, "PAUSED", _menu);
            _text.set_left_alignment();
            int y = -42;

            int y_passive = y;

            for(int i = 0; i < weapon_or_passive_count; ++i)
            {
                if(_levels[i] == 0)
                {
                    continue;
                }

                // Weapons left column, passives right (4 + 4 slots). Slots (hud_slots) exist for icons 0-7 only;
                // newer icons show bare.
                bool weapon = i < up_boots;
                int x = weapon ? -104 : 6;
                int& row_y = weapon ? y : y_passive;
                int icon = _upgrade_icon(i);
                bn::sprite_ptr slot = icon < 8 ?
                            art::slots.create_sprite(x, row_y, (weapon ? art::slot_weapon_first : art::slot_passive_first) + icon) :
                            art::icons.create_sprite(x, row_y, icon);
                slot.set_bg_priority(0);
                _menu.push_back(bn::move(slot));

                bn::string<24> line(upgrade_texts[i].name);
                line.append(" ");
                line.append(bn::to_string<2>(_levels[i]));
                _text.generate(x + 12, row_y, line, _menu);
                row_y += 20;
            }

            _text.generate(-60, 50, _cursor == 0 ? "> Resume" : "  Resume", _menu);
            _text.generate(-60, 62, _cursor == 1 ? "> Quit" : "  Quit", _menu);
        }

        bool _update_pause()
        {
            if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
            {
                _cursor = 1 - _cursor;
                _play(art::sfx_menu, fixed(0.5));
                _draw_pause();
            }
            else if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
            {
                if(_cursor == 1 && bn::keypad::a_pressed())
                {
                    return true;
                }

                _menu.clear();
                _hide_world_extras(false);
                _mode = game_mode::play;
            }

            return false;
        }

        static constexpr int fx_rows = 11;
        static constexpr int debug_rows = 6;
        static constexpr int dbg_start_count = 14;
        static constexpr int dbg_force_count = 13;

        [[nodiscard]] static const char* _dbg_start_name(int i)
        {
            constexpr const char* names[dbg_start_count] = { "4:30", "5:50", "7:00", "8:00", "9:00", "9:45",
                                                             "10:30", "11:50", "12:50", "13:50", "14:45",
                                                             "1:50", "2:50", "7:50" };
            return names[i];
        }

        [[nodiscard]] static int _dbg_start_second(int i)
        {
            constexpr int seconds[dbg_start_count] = { 270, 350, 420, 480, 540, 585, 630, 710, 770, 830, 885,
                                                       110, 170, 470 };
            return seconds[i];
        }

        [[nodiscard]] static const char* _dbg_force_name(int i)
        {
            constexpr const char* names[dbg_force_count] = { "Treasure", "Split elite", "Wolves", "Warchief",
                                                             "Wolf Pack", "Goblin Raid", "Big slime", "Cursed chest",
                                                             "Treant", "Elite pair", "Stampede", "Grave ring",
                                                             "Colossus" };
            return names[i];
        }

        // DBG "Start at mm:ss": the clock jumps and the wizard gets the level and build of profile C at that minute
        // (about 2 levels per minute: Lv 20 at 10:00, DISENO_RUTA block 2). Past 5:00 the Slime King counts as killed.
        void _dbg_start_at(int second)
        {
            // Review block 2: the tutorial hints count as seen.
            _blink_hint_done = true;
            _gem_hint_done = true;
            _pick_hint_done = true;
            _nova_hint_done = true;
            _hint.clear();
            _hint_t = 0;
            _t = second * fps;
            _spawn_cd = 0;

            if(second > 300 && _bosses_killed == 0)
            {
                _boss_spawned = true;
                _bosses_killed = 1;
            }

            if(second > 600 && _bosses_killed == 1)
            {
                _bosses_killed = 2;   // past 10:00 the Warchief counts as killed too
            }

            for(int act = 0; act < 3; ++act)
            {
                if(_treasure_at[act] >= 0 && _treasure_at[act] < _t)
                {
                    _treasure_at[act] = -1;
                }

                if(_cursed_at[act] >= 0 && _cursed_at[act] < _t)
                {
                    _cursed_at[act] = -1;
                }
            }

            constexpr uint8_t order[] = { up_spark, up_frost, up_rune, up_heart, up_ember, up_boots, up_spark,
                                          up_tome, up_chain, up_frost, up_lodestone, up_rune };
            int target = bn::max(2, (2 * second) / 60);
            int k = 0;

            for(int guard = 0; _level < target && guard < 200; ++guard, ++k)
            {
                int id = order[k % int(sizeof(order))];
                int weapons = 0;

                for(int i = 0; i < up_boots; ++i)
                {
                    weapons += _levels[i] > 0;
                }

                if(_levels[id] >= max_upgrade_level || (id < up_boots && _levels[id] == 0 && weapons >= max_slots))
                {
                    continue;
                }

                _apply_upgrade(id);
                ++_level;
            }

            _xp = 0;
            _player_hp = _player_max_hp;
        }

        void _dbg_force_event(int i)
        {
            switch(i)
            {
            case 0:
                _spawn_treasure();
                break;
            case 1:
                _force_elite(3);
                break;
            case 2:
            {
                // Stage 2: 2 wraiths (stage 1 arts hold no tiles there).
                int type = _stage ? int(type_wraith) : int(type_wolf);
                fixed_point p = _spawn_point(120, 136, true);
                _spawn_enemy(type, p, 0);
                _spawn_enemy(type, p + fixed_point(0, 20), 0);
                break;
            }
            case 3:
                _bosses_killed = bn::max(_bosses_killed, 1);
                _boss_spawned = true;

                if(! _boss)
                {
                    _spawn_boss(_stage * 3 + 1);
                }
                break;
            case 4:
                _begin_swarm_warning(swarm_wolves, 90);
                _play(art::sfx_howl);
                break;
            case 5:
                _begin_swarm_warning(swarm_goblins, fps);
                break;
            case 6:
                _spawn_enemy(_stage ? int(type_archer) : int(type_big_slime), _spawn_point(110, 130, true), 0);
                break;
            case 7:
                // In front of the wizard so a test walking forward opens it.
                _spawn_pickup(_pos + _facing * 32, pickup_cursed, 2);
                break;
            case 8:
                _bosses_killed = 2;
                _boss_spawned = true;

                if(! _boss)
                {
                    _spawn_boss(_stage * 3 + 2);
                }
                break;
            case 11:
                _spawn_grave_ring();
                break;
            case 12:
                _boss_spawned = true;

                if(! _boss)
                {
                    _spawn_boss(_stage * 3);
                }
                break;
            case 9:
                _spawn_elite_pair();
                break;
            default:
                _begin_swarm_warning(swarm_stampede, 90);
                _play(art::sfx_horn);
                break;
            }
        }

        [[nodiscard]] int _fx_row_count() const
        {
            return fx_rows + (debug_menu ? debug_rows : 0);
        }

        bool* _fx_flag(int row)
        {
            bool* flags[fx_rows] = { &_fx.hit_flash, &_fx.knockback, &_fx.damage_numbers, &_fx.screen_shake,
                                     &_fx.death_poof, &_fx.gem_trail, &_fx.level_flash, &_fx.dusk,
                                     &_fx.frost_translucent, &_fx.boss_mosaic, &_fx.shadows };
            return row < fx_rows ? flags[row] : nullptr;
        }

        // The SELECT menu covers the whole screen: the HUD hides while it is open so the rows stay readable.
        void _set_hud_visible(bool visible)
        {
            for(bn::sprite_ptr& s : _xp_bar)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _hud_frame)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _hud_level)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _hud_clock)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _hud_gold)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _nova_bar)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _nova_text)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _boss_bar)
            {
                s.set_visible(visible);
            }

            if(_hud_blessing)
            {
                _hud_blessing->set_visible(visible);
            }
        }

        void _open_fx_menu()
        {
            _mode = game_mode::fx_menu;
            _cursor = 0;
            _hide_world_extras(true);
            _set_hud_visible(false);
            _draw_fx_menu();
        }

        void _draw_fx_menu()
        {
            constexpr const char* names[fx_rows + debug_rows] = {
                "Hit flash", "Knockback", "Damage numbers", "Screen shake", "Death poof", "Gem trail",
                "Level-up flash", "Dusk", "Frost translucent", "Boss mosaic", "Shadows",
                "DBG God mode", "DBG Max power", "DBG Start at", "DBG Stress 56", "DBG Force", "DBG Meta preset"
            };

            _menu.clear();
            _text.set_left_alignment();
            _text.generate(-100, -74, "EFFECTS (SELECT: close)", _menu);
            int rows = _fx_row_count();

            for(int r = 0; r < rows; ++r)
            {
                bn::string<32> line(r == _cursor ? "> " : "  ");
                line.append(names[r]);
                _text.generate(-100, -64 + r * 8, line, _menu);

                const char* state = "";

                if(bool* f = _fx_flag(r))
                {
                    state = *f ? "ON" : "OFF";
                }
                else if(r == fx_rows)
                {
                    state = _fx.god_mode ? "ON" : "OFF";
                }
                else if(r == fx_rows + 3)
                {
                    state = _stress ? "ON" : "OFF";
                }
                else if(r == fx_rows + 2)
                {
                    state = _dbg_start_name(_dbg_start);
                }
                else if(r == fx_rows + 4)
                {
                    state = _dbg_force_name(_dbg_force);
                }
                else
                {
                    state = "A";
                }

                _text.generate(60, -62 + r * 9, state, _menu);
            }
        }

        void _update_fx_menu()
        {
            int rows = _fx_row_count();

            if(bn::keypad::up_pressed())
            {
                _cursor = (_cursor + rows - 1) % rows;
                _draw_fx_menu();
            }
            else if(bn::keypad::down_pressed())
            {
                _cursor = (_cursor + 1) % rows;
                _draw_fx_menu();
            }
            else if((bn::keypad::left_pressed() || bn::keypad::right_pressed()) &&
                    (_cursor == fx_rows + 2 || _cursor == fx_rows + 4))
            {
                int step = bn::keypad::right_pressed() ? 1 : -1;
                int& value = _cursor == fx_rows + 2 ? _dbg_start : _dbg_force;
                int count = _cursor == fx_rows + 2 ? dbg_start_count : dbg_force_count;
                value = (value + count + step) % count;
                _draw_fx_menu();
            }
            else if(bn::keypad::a_pressed())
            {
                if(bool* f = _fx_flag(_cursor))
                {
                    *f = ! *f;
                }
                else if(_cursor == fx_rows)
                {
                    _fx.god_mode = ! _fx.god_mode;
                }
                else if(_cursor == fx_rows + 1)
                {
                    // Worst legal build: 4 weapon slots (Ember Toss left out) and every passive at Nv5.
                    for(int i = 0; i < weapon_or_passive_count; ++i)
                    {
                        _levels[i] = i == up_ember || i == up_lance || i == up_lens || i == up_ward ? 0 : max_upgrade_level;
                    }

                    _player_max_hp = player_hp + 20 * max_upgrade_level;
                    _player_hp = _player_max_hp;
                    _nova = nova_max;   // test convenience: Nova ready at once
                    test_nova = _nova;
                    // perf (G-52): a real build gets its weapons one card at a time, so their cooldowns never all
                    // start on the same frame; stagger them here (all firing at once measured 130 %).
                    _spark_cd = 7;
                    _frost_cd = 23;
                    _chain_cd = 41;
                    _ensure_frost_shards();
                    _ensure_bolt_sprites();
                }
                else if(_cursor == fx_rows + 2)
                {
                    _dbg_start_at(_dbg_start_second(_dbg_start));
                }
                else if(_cursor == fx_rows + 4)
                {
                    _dbg_force_event(_dbg_force);
                }
                else if(_cursor == fx_rows + 5)
                {
                    // Meta preset (DISENO_RUTA block 4): +3000 gold and every unlock of stage 1, saved at once.
                    save().gold += 3000;
                    save().unlock_bits |= unlock_ember | unlock_chain | unlock_lens | unlock_ward | unlock_lance |
                                          unlock_stage2;
                    _unlocks = save().unlock_bits;
                    write_save();
                    _levels[up_lance] = max_upgrade_level;   // test convenience: see the Moon Lance at once
                }
                else if(_cursor == fx_rows + 3)
                {
                    _stress = ! _stress;
                }

                _play(art::sfx_menu, fixed(0.5));
                _draw_fx_menu();
            }
            else if(bn::keypad::select_pressed() || bn::keypad::b_pressed())
            {
                _menu.clear();
                _hide_world_extras(false);
                _set_hud_visible(true);
                _apply_fx_static();

                for(bn::sprite_ptr& s : _frost)
                {
                    s.set_blending_enabled(_fx.frost_translucent);
                }

                _mode = game_mode::play;
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // End of run (§3: summary and Retry on A in < 3 s)

        // §C4: bank the run in the save (gold with Greed, records, totals, unlocks). Once per run.
        void _commit_meta()
        {
            if(_meta_done || _stress)
            {
                return;
            }

            _meta_done = true;
            bool won = _mode == game_mode::won;
            int s = _t / fps;
            save_data& d = save();
            uint32_t before = d.unlock_bits;
            int earned = _gold + 5 * (s / 60) + (won ? 100 : 0);
            earned = earned * (100 + 10 * _shop[shop_greed]) / 100;
            _meta_earned = earned;
            test_meta_gold = earned;
            d.gold += uint32_t(earned);
            d.gold_total += uint32_t(earned);

            stage_record& r = d.records[_setup.stage];
            _new_best = (s > r.best_time_s ? 1 : 0) | (_kills > r.best_kills ? 2 : 0) | (_level > r.best_level ? 4 : 0);

            if(r.best_time_s == 0 && r.best_kills == 0)
            {
                _new_best = 0;   // the first run sets the records quietly
            }

            r.best_time_s = uint16_t(bn::max(int(r.best_time_s), s));
            r.best_kills = uint16_t(bn::max(int(r.best_kills), _kills));
            r.best_level = uint8_t(bn::max(int(r.best_level), _level));

            if(won)
            {
                ++r.clears;
                d.stages_cleared |= uint8_t(1 << _setup.stage);
                d.unlock_bits |= _setup.stage == 0 ? unlock_stage2 : unlock_stage3;
            }

            d.kills_total += uint32_t(_kills);
            ++d.runs;
            d.deaths += _player_hp <= 0 ? 1 : 0;
            d.boss_bits |= uint16_t(((1 << _bosses_killed) - 1) << (3 * _setup.stage));

            if(won && _setup.stage < 2)
            {
                d.last_stage = uint8_t(_setup.stage + 1);
            }

            if(s >= 5 * 60)
            {
                d.unlock_bits |= unlock_ember;
            }

            if(_bosses_killed >= 1)
            {
                d.unlock_bits |= unlock_chain;
            }

            if(d.kills_total >= 500)
            {
                d.unlock_bits |= unlock_lens;
            }

            if(_bosses_killed >= 2)
            {
                d.unlock_bits |= unlock_ward;
            }

            if(s >= 10 * 60)
            {
                d.unlock_bits |= unlock_lance;
            }

            _new_unlocks = d.unlock_bits & ~before;
            test_new_unlocks = int(_new_unlocks);
            write_save();
        }

        void _show_summary()
        {
            bool won = _mode == game_mode::won;
            _menu.clear();
            _hide_world_extras(true);
            _summary_shown = true;
            _enemies.clear();
            _grid_clear();
            _bolts.clear();
            _bolt_pool.clear();
            _runes.clear();
            _frost.clear();
            _boss_bar.clear();
            _boss_wave.reset();
            _shrine.reset();
            _shrine_arrow.reset();
            _hud_blessing.reset();
            _hint.clear();
            _big_text.set_center_alignment();
            _big_text.generate(0, -44, won ? "STAGE CLEAR" : "YOU FELL", _menu);
            _text.set_center_alignment();

            _commit_meta();
            int s = _t / fps;
            bn::string<48> line("Time ");
            line.push_back(char('0' + (s / 60) / 10));
            line.push_back(char('0' + (s / 60) % 10));
            line.push_back(':');
            line.push_back(char('0' + (s % 60) / 10));
            line.push_back(char('0' + (s % 60) % 10));
            line.append("   Level ");
            line.append(bn::to_string<4>(_level));
            line.append("   Kills ");
            line.append(bn::to_string<6>(_kills));
            _text.generate(0, -22, line, _menu);

            if(_new_best)
            {
                // §C4 "Records": NEW BEST in time, kills or level.
                line = "NEW BEST:";
                line.append((_new_best & 1) ? " TIME" : "");
                line.append((_new_best & 2) ? " KILLS" : "");
                line.append((_new_best & 4) ? " LEVEL" : "");
                _text.set_palette_item(art::text_gold);
                _text.generate(0, -11, line, _menu);
                _text.set_palette_item(common::variable_8x8_sprite_font.item().palette_item());
            }

            line = "Gold +";
            line.append(bn::to_string<6>(_meta_earned));
            line.append("   Total ");
            line.append(bn::to_string<8>(int(save().gold)));
            _text.set_palette_item(art::text_gold);
            _text.generate(0, 2, line, _menu);
            _text.set_palette_item(common::variable_8x8_sprite_font.item().palette_item());

            // E5.4: gold broken down by source.
            line = "Drops ";
            line.append(bn::to_string<6>(_gold_from[gold_drops]));
            line.append("  Goblin ");
            line.append(bn::to_string<6>(_gold_from[gold_treasure]));
            line.append("  Bosses ");
            line.append(bn::to_string<6>(_gold_from[gold_bosses]));
            line.append("  Cards ");
            line.append(bn::to_string<6>(_gold_from[gold_cards]));
            _text.generate(0, 13, line, _menu);

            // §C4 "Desbloqueos": NEW: ... (one line each, up to 3).
            int y_new = 28;

            for(int b = 0; b < 7 && y_new <= 48; ++b)
            {
                if(_new_unlocks & (1u << b))
                {
                    line = "NEW: ";
                    line.append(unlock_name(b));
                    _text.set_palette_item(art::text_gold);
                    _text.generate(0, y_new, line, _menu);
                    _text.set_palette_item(common::variable_8x8_sprite_font.item().palette_item());
                    y_new += 10;
                }
            }

            if(_new_unlocks)
            {
                _play(bn::sound_items::jingle_unlock);
            }

            test_gold_drops = _gold_from[gold_drops];
            test_gold_bosses = _gold_from[gold_bosses];

            _summary_lock = 60;
            _wizard.set_visible(false);
            _wizard_shadow.set_visible(false);
            _hp_bar.set_visible(false);
        }
    };
}

run_end play_run(unsigned seed, fx_settings& fx, const run_setup& setup)
{
    test_cpu_max = 0;
    test_cpu_nova_max = 0;
    test_cpu_max_plain = 0;
    bn::unique_ptr<game> g(new game(seed, fx, setup));
    return g->run();
}

}
