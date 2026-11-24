// HEXBOUND block 1 run: Mossy Meadow 0:00-5:00, Slime King, level-up cards, shrines, elites, FX toggles.
// Numbers come from DISENO.md (sections cited next to each system).

#include "hb_game.h"

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
#include "bn_sprite_palettes.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprites_mosaic.h"
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
volatile int test_sounds;
volatile int test_last_sound_frame;
volatile int test_last_sound_id;
volatile int test_last_sound_speed;
volatile int test_cpu_nova_max;   // worst frame in which a Hex Nova was cast
// Section ticks (bn::timer, 1/ticks_per_frame of a frame) of the current and of the worst measured frame.
// 0 player, 1 director, 2 spark, 3 runes, 4 frost, 5 bolts, 6 enemies, 7 boss, 8 pickups, 9 particles,
// 10 rest of the logic, 11 engine (bn::core::update until the next frame starts), 12 ticks inside _kill_enemy,
// 13 kills in the frame.
volatile int test_prof[14];
volatile int test_prof_worst[14];     // worst frame without a Nova cast
volatile int test_prof_nova[14];      // worst Nova frame
volatile int test_cpu_max_plain;      // worst frame without a Nova cast

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
    constexpr int template_count = 8;
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
        { { art::prop_pillar, 3, 2 }, { art::prop_pillar, 9, 4 }, { art::prop_bush_flowers, 5, 11 }, { -1, 0, 0 } }
    };

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

        void init(unsigned map_seed)
        {
            seed = map_seed;
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

            unsigned h = hash3(seed, kx, ky);
            unsigned pick = (h >> 4) % 10;
            int t = pick < 3 ? 0 : int(pick) - 2;
            return t * 2 + int(h & 1);
        }

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
        type_count
    };

    struct enemy_def
    {
        const art::monster_art* look;
        int hp;
        fixed speed;
        int damage;
        bool flying;
    };

    // §7
    const enemy_def enemy_defs[type_count] = {
        { &art::slime, 6, fixed(0.35), 5, false },
        { &art::bat, 3, fixed(1.1), 4, true },
        { &art::skeleton, 14, fixed(0.55), 6, false },   // damage 8 -> 6 (§19.2)
    };

    enum upgrade_id : uint8_t
    {
        up_spark,
        up_rune,
        up_frost,
        up_boots,
        up_heart,
        up_heal,
        up_gold,
        up_count
    };

    constexpr int weapon_or_passive_count = 5;
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
        { "Swift Boots", &art::icons, art::icon_swift_boots, { "+8% SPD", "+8% SPD", "+8% SPD", "+8% SPD", "+8% SPD" } },
        { "Heart Amulet", &art::icons, art::icon_heart_amulet, { "+20 HP", "+20 HP", "+20 HP", "+20 HP", "+20 HP" } },
        { "Heal", &art::heart, 0, { "Heal 30", "", "", "", "" } },
        { "Gold", &art::coin, 0, { "+10 G", "", "", "", "" } },
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
        uint8_t nova = 0;       // 1: caught by a Hex Nova, hit in a later frame (perf: spread over frames)
        int16_t slow = 0;
        int16_t freeze = 0;

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
        pickup_chest
    };

    struct pickup
    {
        bn::sprite_ptr sprite;
        fixed_point pos;
        fixed speed;
        int value = 0;
        uint8_t kind = 0;
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
        game(unsigned seed, fx_settings& fx) :
            _fx(fx),
            _camera(bn::camera_ptr::create(0, 0)),
            _text(common::variable_8x8_sprite_font),
            _big_text(common::variable_16x16_sprite_font),
            _wizard(art::wizard.create_sprite(0, art::wizard_y_offset, art::wizard_idle.first)),
            _wizard_shadow(art::shadow_small.create_sprite(0, 5)),
            _hp_bar(art::hp_bar.create_sprite(0, 12, art::hp_bar_frames))
        {
            _random = bn::random();

            for(unsigned i = 0; i < (seed & 255); ++i)
            {
                _random.update();
            }

            _map.init(seed * 2654435761u + 1);
            _grid_clear();
            _build_palettes();
            _white_pal = _white_palette().create_palette();
            _wizard_pal = art::wizard.palette_item().create_palette();

            for(int t = 0; t < type_count; ++t)
            {
                const art::monster_art& look = *enemy_defs[t].look;
                _type_pal[t] = look.item->palette_item().create_palette();

                for(int f = 0; f < look.move.count; ++f)
                {
                    _move_tiles[t].push_back(look.item->tiles_item().create_tiles(look.move.first + f));
                }
            }

            // Props first with tile offset disabled so the dynamic cells index the tile set directly.
            bn::bg_tiles::set_allow_offset(false);
            _map.update(0, 0);
            bn::regular_bg_item props_item(art::props_tiles, art::props_palette,
                                           bn::regular_bg_map_item(_map.prop_cells[0], bn::size(map_cells, map_cells)));
            _props.emplace(props_item.create_bg(0, 0));
            _props->set_priority(2);
            _props->set_camera(_camera);
            bn::bg_tiles::set_allow_offset(true);

            _ground.emplace(art::ground.create_bg(0, 0));
            _ground->set_priority(3);
            _ground->set_camera(_camera);

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

            _levels[up_spark] = 1;
            _player_hp = player_hp;
            _player_max_hp = player_hp;
            _facing = fixed_point(1, 0);

            _show_hint("D-PAD: MOVE", 3 * fps);
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
                            _text.generate(0, 28, "A: RETRY    B: TITLE", _menu);
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
                _tick_sounds();
                bool measured = _mode == game_mode::play && _play_streak >= 2;
                _play_streak = _mode == game_mode::play ? _play_streak + 1 : 0;
                bool nova_frame = _nova_cast_frame;
                _nova_cast_frame = false;
                _prof_timer.restart();
                bn::core::update();
                test_prof[11] = _prof_timer.elapsed_ticks();
                test_prof[12] = 0;
                test_prof[13] = 0;

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

                        for(int k = 0; k < 14; ++k)
                        {
                            test_prof_nova[k] = test_prof[k];
                        }
                    }
                    else if(! nova_frame && cpu > test_cpu_max_plain)
                    {
                        test_cpu_max_plain = cpu;

                        for(int k = 0; k < 14; ++k)
                        {
                            test_prof_worst[k] = test_prof[k];
                        }
                    }
                }
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
        bn::vector<bn::sprite_ptr, 6> _boss_bar;
        bn::vector<bn::sprite_ptr, 4> _hud_level;
        bn::vector<bn::sprite_ptr, 4> _hud_clock;
        bn::vector<bn::sprite_ptr, 4> _hud_gold;
        bn::optional<bn::sprite_ptr> _hud_blessing;
        bn::vector<bn::sprite_ptr, 8> _hint;
        bn::vector<bn::sprite_ptr, 8> _world_hint;
        bn::vector<bn::sprite_ptr, 96> _menu;
        bn::vector<bn::sprite_ptr, 4> _warnings;

        bn::vector<enemy, max_enemies> _enemies;
        bn::vector<bolt, max_bolts> _bolts;
        bn::vector<bn::sprite_ptr, max_bolts> _bolt_pool;   // hidden spark sprites ready for reuse
        bn::vector<bn::sprite_ptr, max_runes> _runes;
        bn::vector<bn::sprite_ptr, frost_shards> _frost;
        bn::vector<pickup, max_gems + 4> _pickups;
        bn::vector<particle, max_particles> _particles;
        bn::vector<damage_number, max_damage_numbers> _numbers;

        bn::array<bn::color, 16> _white_colors;
        bn::array<bn::color, 16> _elite_colors[type_count][2];
        bn::array<bn::color, 16> _boss_white_colors;

        // Hot-path caches (perf): enemies swap palettes and animation frames by handle, without a VRAM lookup.
        bn::optional<bn::sprite_palette_ptr> _white_pal;
        bn::optional<bn::sprite_palette_ptr> _type_pal[type_count];
        bn::vector<bn::sprite_tiles_ptr, 8> _move_tiles[type_count];

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
        int _gold = 0;
        int _kills = 0;
        int _levels[weapon_or_passive_count] = {};
        int _walk_anim = 0;
        int _idle_anim = 0;
        int _death_anim = 0;
        int _wizard_frame = 0;
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
        int _spawn_cd = 0;
        int _spawns_total = 0;
        uint16_t _next_uid = 1;
        int _swarm_warning_t = 0;
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
        bn::vector<bn::sprite_ptr, 10> _boss_title;

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

            int length = 10;

            if(item == art::sfx_level)
            {
                length = 30;
            }
            else if(item == art::sfx_boss)
            {
                length = 36;
            }
            else if(item == art::sfx_hurt)
            {
                length = 14;
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

            // Elite tint (§4 2b): Swift = gold, Shielded = cyan; computed from whatever palette the item has.
            const bn::color tints[2] = { bn::color(31, 26, 4), bn::color(8, 28, 31) };

            for(int t = 0; t < type_count; ++t)
            {
                const bn::span<const bn::color>& src = enemy_defs[t].look->item->palette_item().colors_ref();

                for(int e = 0; e < 2; ++e)
                {
                    for(int i = 0; i < 16; ++i)
                    {
                        bn::color c = src[i];
                        bn::color tint = tints[e];
                        int r = (c.red() + tint.red()) / 2;
                        int g = (c.green() + tint.green()) / 2;
                        int b = (c.blue() + tint.blue()) / 2;
                        _elite_colors[t][e][i] = i == 0 ? c : bn::color(r, g, b);
                    }

                    // Darkest colour becomes a bright outline so the elite reads at a glance.
                    _elite_colors[t][e][1] = tints[e];
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

        void _restore_palette(enemy& e)
        {
            if(e.elite)
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

            return enemy_defs[e.type].look->item->palette_item();
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

            ++_t;
            _prof_timer.restart();
            _update_player();
            _prof(0);
            _update_director();
            _prof(1);
            _update_weapons();
            _update_enemies();
            _prof(6);
            _update_boss();
            _prof(7);
            _update_pickups();
            _prof(8);
            _update_particles();
            _prof(9);
            _update_shrine();
            _update_timers();

            int cam_x = to_int(_pos.x());
            int cam_y = to_int(_pos.y());

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

            if(_player_hp <= 0)
            {
                _player_hp = 0;
                _mode = game_mode::dead;
                _wizard.set_tiles(art::wizard.tiles_item(), art::wizard_death.first);
                _wizard.set_visible(true);
                _death_anim = 0;
                _end_timer = 60;
                _play(art::sfx_hurt);
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
            fixed m = 1 + fixed(0.08) * _levels[up_boots];

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
            _blink_cd = 3 * fps;
            _blink_invulnerable = 10;
            _play(art::sfx_menu, 1, fixed(0.7));
            test_blinks = test_blinks + 1;

            // PROVISIONAL afterimages (fx_blink pending): translucent copies of the current wizard frame.
            for(int i = 1; i <= 3; ++i)
            {
                fixed_point p = start + (end - start) * i / 4;
                _spawn_particle(fixed_point(p.x(), p.y() + art::wizard_y_offset), fixed_point(), art::wizard,
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
            _nova_flash = 24;
            _shake = 20;
            _play(art::sfx_boss);
            _play(art::sfx_level, 1, fixed(0.8));
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

            _spawn_poof(_pos, 6);
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
            // faces the movement direction. Walking backwards plays the walk cycle of the aim side.
            int look_x = ax;
            int look_y = ay;

            if(! moving)
            {
                look_x = _facing.x() > 0 ? 1 : _facing.x() < 0 ? -1 : 0;
                look_y = _facing.y() > 0 ? 1 : _facing.y() < 0 ? -1 : 0;
            }

            if(_has_aim)
            {
                int adx = abs_int(_aim_dx);
                int ady = abs_int(_aim_dy);
                look_x = adx * 2 >= ady ? (_aim_dx < 0 ? -1 : 1) : 0;
                look_y = look_x ? 0 : (_aim_dy < 0 ? -1 : 1);
            }

            if(look_x)
            {
                frame = moving ? art::wizard_walk_side.frame(_walk_anim, true) : art::wizard_idle_side.frame(_idle_anim, true);
                _wizard.set_horizontal_flip(look_x < 0);
            }
            else if(look_y < 0)
            {
                frame = moving ? art::wizard_walk_up.frame(_walk_anim, true) : art::wizard_idle_up.frame(_idle_anim, true);
            }
            else
            {
                frame = moving ? art::wizard_walk_down.frame(_walk_anim, true) : art::wizard_idle.frame(_idle_anim, true);
            }

            if(_invulnerable > 20)
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

            _player_hp -= damage;
            _invulnerable = player_invulnerable_frames;
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

            if(_boss_spawned)
            {
                return 12;
            }

            if(s < 60)
            {
                return 6 + (8 * s) / 60;
            }

            constexpr int per_minute[5] = { 14, 16, 26, 32, 38 };   // minute 1: 20 -> 16 (DISENO §19 fallback)
            return per_minute[bn::min(s / 60, 4)];
        }

        [[nodiscard]] bool _spawn_paused() const
        {
            int s = _t / fps;
            return (s >= 50 && s < 56) || (_t >= boss_time - pre_boss_pause && _t < boss_time) || _boss_dead;
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

            const enemy_def& def = enemy_defs[type];
            bn::sprite_ptr sprite = bn::sprite_ptr::create(p, def.look->item->shape_size(), _move_tiles[type][0],
                                                           *_type_pal[type]);
            sprite.set_camera(_camera);
            sprite.set_bg_priority(2);
            sprite.set_z_order(10);
            enemy& e = _enemies.emplace_back(bn::move(sprite));
            e.pos = p;
            _grid_link(_enemies.size() - 1);
            e.type = uint8_t(type);
            e.hp = def.hp;
            e.uid = _next_uid++;

            if(_next_uid == 0)
            {
                _next_uid = 1;
            }

            ++_spawns_total;

            if(elite_chance_den > 0 && _t >= 2 * 60 * fps && _random.get_int(elite_chance_den) == 0)
            {
                e.elite = uint8_t(1 + _random.get_int(2));

                if(e.elite == 2)
                {
                    e.shield = 3;
                }

                e.sprite.set_palette(_enemy_palette(e));
                e.aura = _world_sprite(art::fx_aura, p, 0, 11);
            }

            return &e;
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

        void _update_director()
        {
            int s = _t / fps;
            bool second = _t % fps == 0;

            // 0:04 one slime from the side the wizard faces (§5).
            if(_t == 4 * fps)
            {
                _spawn_enemy(type_slime, _spawn_point(136, 150, true), 0);
            }

            // §19.2: no scripted skeleton pair at 1:00 any more; they enter the spawn pool from 1:10.

            // Shrines at 1:30, 3:00, 4:30 (§4 2a, §6).
            if(second && s > 0 && s % 90 == 0 && _t < boss_time)
            {
                _spawn_shrine();
            }

            // 2:00 Bat Swarm with a 1 s warning (§5, §6).
            if(_t == 2 * 60 * fps - fps)
            {
                _begin_swarm_warning();
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

            // 4:00 Bone Ring (§6).
            if(_t == 4 * 60 * fps)
            {
                _spawn_bone_ring();
            }

            // 5:00 Slime King (§8).
            if(_t == boss_time && ! _boss_spawned)
            {
                _spawn_boss();
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
                group = 4;   // perf: 4 per frame refills as fast as 8 every 2 frames, with half the peak
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

                _spawn_enemy(_pick_type(), p, 40);
            }
        }

        void _begin_swarm_warning()
        {
            int side = _random.get_int(4);
            constexpr int dx[4] = { -1, 1, 0, 0 };
            constexpr int dy[4] = { 0, 0, -1, 1 };
            _swarm_dir = fixed_point(-dx[side], -dy[side]);
            _swarm_origin = fixed_point(dx[side] * 150, dy[side] * 110);
            _swarm_warning_t = fps;
            _warnings.clear();

            for(int i = -1; i <= 1; ++i)
            {
                int wx = dx[side] ? dx[side] * 112 : i * 40;
                int wy = dy[side] ? dy[side] * 64 : i * 30;
                bn::sprite_ptr w = art::alert.create_sprite(wx, wy, 0);
                w.set_bg_priority(0);
                _warnings.push_back(bn::move(w));
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
            _make_room(12);
            fixed_point perp(-_swarm_dir.y(), _swarm_dir.x());
            fixed_point origin = _pos + _swarm_origin;

            for(int i = 0; i < 12; ++i)
            {
                fixed_point p = origin + perp * ((i - 6) * 14) - _swarm_dir * (i & 1) * 10;

                if(enemy* e = _spawn_enemy(type_bat, p, 0))
                {
                    e->mode = mode_swarm;
                    e->vel = _swarm_dir * enemy_defs[type_bat].speed;
                    e->life = 420;
                }
            }
        }

        void _spawn_bone_ring()
        {
            _make_room(16);

            for(int i = 0; i < 16; ++i)
            {
                fixed_point p = _pos + angle_vector(i * 128) * 150;

                if(enemy* e = _spawn_enemy(type_skeleton, p, 0))
                {
                    e->mode = mode_ring;
                }
            }
        }

        // -----------------------------------------------------------------------------------------------------
        // Weapons (§9)

        [[nodiscard]] int _damage(int base) const
        {
            return _blessing == bless_fury ? (base * 13 + 9) / 10 : base;
        }

        void _update_weapons()
        {
            _update_nova();
            _update_spark();
            _prof(2);
            _update_runes();
            _prof(3);
            _update_frost();
            _prof(4);
            _update_bolts();
            _prof(5);
        }

        void _update_spark()
        {
            int lv = _levels[up_spark];

            if(lv == 0 || --_spark_cd > 0)
            {
                return;
            }

            _spark_cd = lv >= 5 ? 40 : 50;
            int count = lv >= 4 ? 3 : lv >= 2 ? 2 : 1;
            int damage = _damage(lv >= 3 ? 8 : 5);
            int pierce = lv >= 5 ? 2 : 0;

            // Nearest distinct targets.
            int best_i[3] = { -1, -1, -1 };
            int best_d[3] = { 1 << 30, 1 << 30, 1 << 30 };
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
                int near_count = dead ? 0 : _grid_query(bx, by, enemy_half_box + 3, near);

                for(int k = 0; k < near_count && ! dead; ++k)
                {
                    int i = near[k];
                    enemy& e = _enemies[i];
                    int dx = to_int(e.pos.x()) - bx;
                    int dy = to_int(e.pos.y()) - by;

                    if(abs_int(dx) < enemy_half_box + 3 && abs_int(dy) < enemy_half_box + 3)
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

            int count = lv >= 4 ? 4 : lv >= 2 ? 3 : 2;
            int radius = lv >= 3 ? 36 : 28;
            bool always = lv >= 5;
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
            int damage = _damage(4);

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
                        _hit_enemy(i, damage, p);
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

                _frost_cd = lv >= 4 ? 110 : 150;
                _frost_t = 0;
                ++_frost_pulse_id;

                if(_frost_pulse_id == 0)
                {
                    _frost_pulse_id = 1;
                }
            }

            int max_r = lv >= 3 ? 72 : 56;
            int r = (max_r * (_frost_t + 1)) / 24;
            int damage = _damage(lv >= 2 ? 9 : 6);

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
                _frost.push_back(bn::move(*s));
            }

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
                        e.freeze = 45;
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

        // Returns true if the enemy died (it was removed from _enemies; index i now holds another enemy).
        bool _hit_enemy(int i, int damage, const fixed_point& from)
        {
            enemy& e = _enemies[i];

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
            _spawn_particle(fixed_point(p.x(), p.y() + look.y_offset), fixed_point(), *look.item, death,
                            death.count * death.frame_time, 14, e.sprite.horizontal_flip());

            if(_fx.death_poof)
            {
                _spawn_poof(p, 3);
            }

            int value = 1;

            if(e.type == type_skeleton && _random.get_int(5) == 0)
            {
                value = 5;
            }

            _spawn_gem(p, value);

            if(e.elite)
            {
                _spawn_pickup(p + fixed_point(8, 0), pickup_chest, 1);
                _spawn_pickup(p + fixed_point(-8, 0), pickup_coin, 3);
            }
            else if(_random.get_int(50) == 0)
            {
                _spawn_pickup(p + fixed_point(6, 4), pickup_coin, 1);
            }

            _remove_enemy(i);
        }

        // -----------------------------------------------------------------------------------------------------
        // Enemies

        HB_HOT void _update_enemies()
        {
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
                    if(e.mode == mode_chase && abs_int(dx) + abs_int(dy) > recycle_distance)
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
                    else
                    {
                        if(((_t + e.uid) & 3) == 0 || (e.vel.x() == 0 && e.vel.y() == 0))
                        {
                            fixed speed = e.mode == mode_ring ? fixed(0.4) : def.speed;

                            if(e.elite == 1)
                            {
                                speed *= fixed(1.6);
                            }

                            e.vel = unit(dx, dy) * speed;
                        }

                        step = e.vel;

                        if(e.type == type_slime && ((_t + e.uid * 7) & 31) >= 20)
                        {
                            // Slimes hop: still for 12 of every 32 frames, faster in between (avg 0.35).
                            step = fixed_point();
                        }
                        else if(e.type == type_slime)
                        {
                            step = step * fixed(1.6);
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

                // Contact damage (§7).
                if(abs_int(dx) < player_half_box + 6 && abs_int(dy) < player_half_box + 6)
                {
                    _hurt_player(def.damage);
                }

                fixed_point draw(e.pos.x(), e.pos.y() + def.look->y_offset);

                if(e.type == type_bat)
                {
                    draw.set_y(draw.y() + bn::lut_sin(((_t + e.uid * 37) * 16) & 2047) * 8);
                }

                e.sprite.set_position(draw);
                e.sprite.set_horizontal_flip(dx < 0);

                const art::anim& move_anim = def.look->move;

                if(((_t + e.uid) % move_anim.frame_time) == 0)
                {
                    e.sprite.set_tiles(_move_tiles[e.type][move_anim.frame(_t + e.uid, true) - move_anim.first]);
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
        // Boss: Slime King (§8)

        void _spawn_boss()
        {
            _boss_spawned = true;
            _boss_pos = _pos + _facing * 90;

            if(_map.solid_at(to_int(_boss_pos.x()), to_int(_boss_pos.y())))
            {
                _boss_pos = _pos + fixed_point(0, -80);
            }

            _boss_hp = 600;
            _boss_state = 0;
            _boss_timer = 30;
            bn::sprite_ptr b = _world_sprite(art::slime_king, _boss_pos, 0, 8);
            b.set_mosaic_enabled(_fx.boss_mosaic);
            _boss = bn::move(b);
            bn::sprite_ptr sh = _world_sprite(art::shadow_big, _boss_pos + fixed_point(0, 22), 0, 31);
            sh.set_blending_enabled(true);
            _boss_shadow = bn::move(sh);

            for(int i = 0; i < 6; ++i)
            {
                bn::sprite_ptr bar = art::boss_bar.create_sprite(-80 + i * 32, -56, art::bar_width);
                bar.set_bg_priority(0);
                _boss_bar.push_back(bn::move(bar));
            }

            _boss_title.clear();
            _big_text.set_center_alignment();
            _big_text.generate(0, -30, "SLIME KING", _boss_title);
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

            if(! _boss)
            {
                if(_boss_dead && _end_timer > 0 && --_end_timer == 0)
                {
                    _mode = game_mode::won;
                    _end_timer = 1;
                }

                return;
            }

            if(_boss_flash && --_boss_flash == 0)
            {
                _boss->set_palette(art::slime_king.palette_item());
            }

            if(_boss_hp <= 0)
            {
                _kill_boss();
                return;
            }

            bool phase2 = _boss_hp * 2 < 600;
            bool phase3 = _boss_hp * 4 < 600;
            int air_time = phase3 ? 30 : 45;
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
                _boss_pos += d * fixed(0.3);

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
                    _boss_timer = phase3 ? 18 : 60;
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

            for(int i = 0; i < 6; ++i)
            {
                int fill = (_boss_hp * 192) / 600 - i * 32;
                _boss_bar[i].set_tiles(art::boss_bar.tiles_item(), bn::max(0, bn::min(32, fill)));
            }
        }

        void _boss_land(bool phase2)
        {
            _boss_state = 4;
            _boss_timer = 30;
            _boss->set_tiles(art::slime_king.tiles_item(), art::king_land.first);
            _shake = 16;
            _play(art::sfx_boss, fixed(0.7));

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
            _boss_dead = true;
            _spawn_poof(_boss_pos, 6);
            _spawn_gem(_boss_pos, 25);
            _spawn_pickup(_boss_pos + fixed_point(12, 0), pickup_coin, 10);
            _boss.reset();
            _boss_shadow.reset();
            _boss_bar.clear();
            _boss_hp = 0;
            _shake = 20;
            _level_flash = 20;
            _play(art::sfx_boss);
            _end_timer = 120;
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

            // Over the cap the value goes to the farthest gem, which turns green/red (§13).
            if(gems >= max_gems || _pickups.full())
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

        bool _spawn_pickup(const fixed_point& p, int kind, int value)
        {
            if(_pickups.full())
            {
                return false;
            }

            bn::optional<bn::sprite_ptr> s;

            if(kind == pickup_chest)
            {
                s = art::chest.create_sprite_optional(p, 0);
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
            g.speed = fixed(1.5);
            return true;
        }

        void _update_pickups()
        {
            int radius = _blessing == bless_magnet ? 400 : pickup_radius;
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

                if(g.kind == pickup_chest)
                {
                    if(d2 < 12 * 12)
                    {
                        ++_free_upgrades;
                        _play(art::sfx_level, fixed(0.6));
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
            _xp += value;

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

            for(int i = 0; i < count; ++i)
            {
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
            if(! _shrine)
            {
                return;
            }

            int dx = to_int(_shrine_pos.x() - _pos.x());
            int dy = to_int(_shrine_pos.y() - _pos.y());
            bool on_screen = abs_int(dx) < 112 && abs_int(dy) < 72;

            if(_shrine_arrow)
            {
                _shrine_arrow->set_visible(! on_screen && (_t & 16));

                if(! on_screen)
                {
                    // Edge arrow: clamp the direction to the screen border, pick one of 8 frames.
                    int sx = bn::max(-110, bn::min(110, dx));
                    int sy = bn::max(-58, bn::min(70, dy));
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

                    _shrine_arrow->set_tiles(art::edge_arrow.tiles_item(), frame);
                    _shrine_arrow->set_horizontal_flip(dx < 0);
                    _shrine_arrow->set_vertical_flip(dy > 0);
                    _shrine_arrow->set_position(sx, sy);
                }
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

            if(_fx.dusk)
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
                for(int i = 0; i < 2; ++i)
                {
                    bn::sprite_ptr bar = art::xp_bar.create_sprite(-96 + i * 32, 72, 0);
                    bar.set_bg_priority(0);
                    _nova_bar.push_back(bn::move(bar));
                }
            }

            int nova_px = (_nova * 64) / nova_max;

            for(int i = 0; i < 2; ++i)
            {
                _nova_bar[i].set_tiles(art::xp_bar.tiles_item(), bn::max(0, bn::min(32, nova_px - i * 32)) / 2);
            }

            bool ready = _nova >= nova_max;
            int nova_state = ready ? 1 + ((_t >> 4) & 1) : 0;

            if(force || nova_state != _last_nova_shown)
            {
                _last_nova_shown = nova_state;
                _nova_text.clear();
                _text.set_left_alignment();

                if(nova_state == 1)
                {
                    _text.set_palette_item(art::text_gold);
                    _text.generate(-56, 72, "A", _nova_text);
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
            for(pickup& g : _pickups)
            {
                g.sprite.set_visible(! hide);
            }

            for(particle& p : _particles)
            {
                p.sprite.set_visible(! hide);
            }

            for(damage_number& d : _numbers)
            {
                for(bn::sprite_ptr& s : d.sprites)
                {
                    s.set_visible(! hide);
                }
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

            for(int i = 0; i < weapon_or_passive_count; ++i)
            {
                if(_levels[i] < max_upgrade_level)
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
            else if(! _pick_hint_done)
            {
                _text.generate(0, 72, "PICK ONE: D-PAD + A", _menu);
            }

            for(bn::sprite_ptr& s_hud : _hud_frame)
            {
                s_hud.set_visible(false);
            }

            for(bn::sprite_ptr& s_hud : _hud_clock)
            {
                s_hud.set_visible(false);
            }

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

        void _apply_upgrade(int id)
        {
            switch(id)
            {
            case up_heal:
                _player_hp = bn::min(_player_max_hp, _player_hp + 30);
                break;

            case up_gold:
                _gold += 10;
                break;

            case up_heart:
                ++_levels[id];
                _player_max_hp += 20;
                _player_hp = bn::min(_player_max_hp, _player_hp + 20);
                break;

            default:
                ++_levels[id];
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

            for(int i = 0; i < weapon_or_passive_count; ++i)
            {
                if(_levels[i] == 0)
                {
                    continue;
                }

                // Slots (hud_slots) live in the pause screen only, to keep the combat HUD within the sprite budget.
                int slot_frame = (i < up_boots ? art::slot_weapon_first : art::slot_passive_first) + upgrade_texts[i].icon;
                bn::sprite_ptr slot = art::slots.create_sprite(-72, y, slot_frame);
                slot.set_bg_priority(0);
                _menu.push_back(bn::move(slot));

                bn::string<24> line(upgrade_texts[i].name);
                line.append("  Lv ");
                line.append(bn::to_string<2>(_levels[i]));
                _text.generate(-58, y, line, _menu);
                y += 17;
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
        static constexpr int debug_rows = 4;

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
                "DBG God mode", "DBG Max power", "DBG Skip to 4:30", "DBG Stress 56"
            };

            _menu.clear();
            _text.set_left_alignment();
            _text.generate(-100, -74, "EFFECTS (SELECT: close)", _menu);
            int rows = _fx_row_count();

            for(int r = 0; r < rows; ++r)
            {
                bn::string<32> line(r == _cursor ? "> " : "  ");
                line.append(names[r]);
                _text.generate(-100, -62 + r * 9, line, _menu);

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
                    for(int i = 0; i < weapon_or_passive_count; ++i)
                    {
                        _levels[i] = max_upgrade_level;
                    }

                    _player_max_hp = player_hp + 20 * max_upgrade_level;
                    _player_hp = _player_max_hp;
                }
                else if(_cursor == fx_rows + 2)
                {
                    _t = 4 * 60 * fps + 30 * fps;
                    _spawn_cd = 0;
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

            int s = _t / fps;
            bn::string<32> line("Time ");
            line.push_back(char('0' + (s / 60) / 10));
            line.push_back(char('0' + (s / 60) % 10));
            line.push_back(':');
            line.push_back(char('0' + (s % 60) / 10));
            line.push_back(char('0' + (s % 60) % 10));
            _text.generate(0, -20, line, _menu);

            line = "Level ";
            line.append(bn::to_string<4>(_level));
            line.append("   Kills ");
            line.append(bn::to_string<6>(_kills));
            _text.generate(0, -8, line, _menu);

            line = "Gold ";
            line.append(bn::to_string<6>(_gold));
            _text.generate(0, 4, line, _menu);

            _summary_lock = 60;
            _wizard.set_visible(false);
            _wizard_shadow.set_visible(false);
            _hp_bar.set_visible(false);
        }
    };
}

run_end play_run(unsigned seed, fx_settings& fx)
{
    test_cpu_max = 0;
    test_cpu_nova_max = 0;
    test_cpu_max_plain = 0;
    bn::unique_ptr<game> g(new game(seed, fx));
    return g->run();
}

}
