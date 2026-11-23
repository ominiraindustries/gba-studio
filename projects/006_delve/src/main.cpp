// DELVE (task G-25): first-person grid crawler. Block 1: generated floor (v2 port + hooks), Mode 4 view redrawn only
// on change, grid movement (DISENO.md 1), minimap and compass, scaled monsters / props, P1 lever gate and spike plate.

#include "bn_bg_palette_item.h"
#include "bn_bg_palettes.h"
#include "bn_blending.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_span.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_affine_mats.h"
#include "bn_sprite_item.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_tiles.h"
#include "bn_sprites.h"
#include "bn_string.h"
#include "bn_timer.h"
#include "bn_timers.h"
#include "bn_vector.h"

#include "bn_palette_bitmap_items_ui_frame.h"
#include "bn_sprite_items_item_floor.h"
#include "bn_sprite_items_mon_rat.h"
#include "bn_sprite_items_mon_skeleton.h"
#include "bn_sprite_items_mon_slime.h"
#include "bn_sprite_items_prop_barrel.h"
#include "bn_sprite_items_prop_bones.h"
#include "bn_sprite_items_prop_chest.h"
#include "bn_sprite_items_prop_crate.h"
#include "bn_sprite_items_prop_spikes.h"
#include "bn_sprite_items_prop_statue.h"
#include "bn_sprite_items_prop_table.h"

#include "common_variable_8x8_sprite_font.h"

#include "dl_deco_items.h"
#include "dl_fx.h"
#include "dl_gen.h"
#include "dl_hud.h"
#include "dl_monster_fx.h"
#include "dl_blit.h"
#include "dl_themes.h"
#include "dl_combat.h"
#include "dl_combat_fx.h"
#include "dl_transition.h"
#include "dl_view.h"
#include "dl_view_data.h"
#include "dl_view_geometry.h"

// Mirrors for headless tests (gbarun.py --check test_x.32==...). Global, non-static, volatile.
volatile int test_cell;
volatile int test_dir;
volatile int test_floor;
volatile int test_moves;
volatile int test_bumps;
volatile int test_redraws;
volatile int test_redraw_ticks;       // last redraw, timer ticks (ticks_per_frame() = 1 frame)
volatile int test_redraw_max_ticks;
volatile int test_hud_ticks;           // HUD update, timer ticks
volatile int test_hud_max_ticks;
volatile int test_sprite_tiles;        // sprite VRAM in use, 32-byte tiles (mode 4: 512 max)
volatile int test_sprites;             // sprites in use (OAM: 128 max)
volatile int test_sprite_tiles_free;
volatile int test_hud_sprites;
volatile int test_hud_tiles;
volatile int test_view_ticks;          // view only (no minimap)
volatile int test_view_ticks_sum;
volatile int test_view_max_ticks;
volatile int test_map_ticks_sum;
volatile int test_map_max_ticks;
volatile int test_map_update_max_ticks;   // seen marks + key recompute part
volatile int test_gen_ticks;           // last floor generation, timer ticks
volatile int test_gen_max_ticks;
volatile int test_visible_decos;
volatile int test_transition_frames;
volatile int test_motion_frames;       // G-31 smooth moves: composed intermediate poses
volatile int test_motion_ticks;        // last pose, view only
volatile int test_motion_max_ticks;
volatile int test_view_items;          // pieces copied by the last resting redraw (after the coverage cull)
volatile int test_motion_items;
volatile int test_view_spans;          // spans visited / halfwords copied by the last resting redraw
volatile int test_view_halfwords;
volatile int test_prepare_max_ticks;   // visibility + grid part of a resting redraw
volatile int test_compose_max_ticks;
volatile int test_lever_frame;         // frame of the last lever sprite shown (0 up, 1 down)
volatile int test_ticks_per_frame;
volatile int test_theme;               // G-37: theme index in use (dl_themes.h)
volatile int test_visible_objects;
volatile int test_spike_hits;
volatile int test_levers;
volatile unsigned test_floor_seed;
volatile int test_mfx_ticks;           // G-34 monster FX per frame, timer ticks (last / max)
volatile int test_mfx_max_ticks;
volatile int test_mfx_heavy_max_ticks;  // ... on frames that also redrew or composed the view
volatile int test_mfx_slots;           // monsters with their own FX palette
volatile int test_sprite_palettes;     // sprite palettes in use (16 max)
volatile int test_affine_mats;         // sprite affine matrices in use (32 max)
volatile int test_cpu_x1000;           // bn::core::last_cpu_usage() x 1000
volatile int test_fx_menu_open;
volatile int test_cpu_max_x1000;
volatile int test_mode;                // G-35: 0 title, 1 playing, 2 dead
volatile unsigned test_run_seed;
volatile int test_combat_ticks;        // combat + AI per frame, timer ticks (last / max / max on heavy frames)
volatile int test_combat_max_ticks;
volatile int test_combat_heavy_max_ticks;
volatile int test_rebuild_max_ticks;   // object sprites rebuilt because a monster moved (light frames)
volatile int test_cfx_max_ticks;
volatile int test_frame_max_ticks;     // whole game update (logic + view), timer ticks
volatile int test_frame_heavy_max_ticks;
volatile int test_deaths;

namespace
{
    using namespace dl::gen;

    // Run seed: picked from the frame count when the player presses A on the title (new on every run; each floor
    // hashes it with its number). Build with USERFLAGS=-DDL_DEBUG_SEED=0x00C0FFEE for reproducible tests.
    unsigned run_seed = 0x00C0FFEE;
    constexpr int step_cooldown = 12;           // DISENO.md 1
    constexpr int turn_cooldown = 8;
    constexpr int monster_lerp_frames = 12;
    constexpr int spike_period = 150;           // down 90 f -> peek 24 f -> up 36 f (DISENO.md 6b)

    struct game_state
    {
        level lv;
        dl::view::edge_state es;
        dl::view::visibility vis;
        uint8_t seen[N];
        int16_t ent_of_cell[N];
    };

    BN_DATA_EWRAM game_state gs;

    unsigned floor_seed(int floor)
    {
        unsigned h = run_seed ^ (unsigned(floor) * 0x9E3779B9u);
        h ^= h >> 15;
        h *= 0x2C1B3C6Du;
        h ^= h >> 12;
        return h;
    }

    void index_entities()
    {
        for(int c = 0; c < N; ++c)
        {
            gs.ent_of_cell[c] = -1;
        }

        for(int i = 0; i < gs.lv.nents; ++i)
        {
            const entity& e = gs.lv.ents[i];

            if(e.type == E_ITEM || e.type == E_FURN || e.type == E_TRAP || e.type == E_KEY)
            {
                int c = e.y * W + e.x;

                if(gs.ent_of_cell[c] < 0)
                {
                    gs.ent_of_cell[c] = int16_t(i);
                }
            }
        }
    }

    const bn::sprite_item* monster_item(int ent)
    {
        switch(gs.lv.ents[ent].b)      // dl::mon_kind (dl::combat::load_floor)
        {
        case dl::MON_RAT:
            return &bn::sprite_items::mon_rat;
        case dl::MON_SLIME:
            return &bn::sprite_items::mon_slime;
        default:
            return &bn::sprite_items::mon_skeleton;
        }
    }

    const bn::sprite_item* furn_item(int type)
    {
        switch(type)
        {
        case FURN_BARREL:
            return &bn::sprite_items::prop_barrel;
        case FURN_CRATE:
            return &bn::sprite_items::prop_crate;
        case FURN_BONES:
            return &bn::sprite_items::prop_bones;
        case FURN_STATUE:
            return &bn::sprite_items::prop_statue;
        case FURN_CHEST:
            return &bn::sprite_items::prop_chest;
        case FURN_TABLE:
            return &bn::sprite_items::prop_table;
        default:
            return nullptr;     // puddle: no art yet
        }
    }

    bool furn_blocking(const entity& e)
    {
        return e.type == E_FURN && furn_blocks[e.c < FURN_COUNT ? e.c : 0];
    }

    struct object_sprite
    {
        int ent;
        int8_t d;       // base-relative cell (resting view: the player's cell and facing)
        int8_t x;
        bool monster;
        bool spikes;
        int8_t band;    // depth band of the last placement (monster FX)
        int8_t frame;   // monster animation frame shown (-1 unknown)
        bn::fixed mult; // monster scale multiplier applied (telegraph lean, death shrink)
        bn::fixed from_x, from_y, from_s;
        bn::fixed to_x, to_y, to_s;
        int t;
    };

    class game
    {
    public:
        game() :
            _palette(bn::span<const bn::color>(dl::view_data::palette_colors, 256), bn::bpp_mode::BPP_8),
            _bg(bn::palette_bitmap_bg_ptr::create(_palette)),
            _painter(_bg),
            _text(common::variable_8x8_sprite_font),
            _hud(_bg),
            _combat(_hud, _mfx),
            _cfx(_text)
        {
            test_ticks_per_frame = bn::timers::ticks_per_frame();

            for(int k = 0; k < 2; ++k)
            {
                _painter.blit(0, 0, bn::palette_bitmap_items::ui_frame.pixels_item());
                _painter.flip_page_now();
            }

            // torch glow: additive light (bottom weight 1) with a palette that fades to black at the rim, so the
            // disc never shows an edge over whatever is behind it (G-29; was a 40 % alpha blend of a bright disc)
            bn::blending::set_transparency_weights(1, 1);
            create_deco_palettes();
            show_screen("DELVE", "PRESS A", nullptr);
            _mode = 0;
        }

        void update()
        {
            ++_frame;
            test_mode = _mode;

            if(_mode != 1)
            {
                update_screen();
                return;
            }

            bn::timer frame_timer;
            int heavy_mark = test_redraws + test_motion_frames + test_transition_frames;
            bool a_pressed = false, b_pressed = false;

            if(! _menu.update(_text, _mfx) && ! bn::keypad::select_held())
            {
                handle_input();
                a_pressed = bn::keypad::a_pressed();
                b_pressed = bn::keypad::b_pressed();
            }

            test_fx_menu_open = _menu.open();

            if(dl::themes::take_changed())     // G-37 debug: theme forced from the FX menu
            {
                dl::themes::apply_for_floor(_bg.palette(), _floor);
                test_theme = dl::themes::current();
                create_deco_palettes();
                _dirty = true;
            }
            bn::timer combat_timer;
            _combat.update(_cell, _dir, a_pressed, b_pressed, ! _dirty && ! _trans);
            int combat_ticks = combat_timer.elapsed_ticks();
            update_spikes();
            if(_trans)
            {
                step_transition();
            }
            else
            {
                update_objects();
            }

            update_effects();
            update_shake();

            if(_dirty && ! _trans)
            {
                redraw();
            }

            bool heavy = heavy_mark != test_redraws + test_motion_frames + test_transition_frames;

            if(_combat.objects_dirty && ! _trans && ! heavy)
            {
                // a monster moved, died or an item went: re-place the sprites (light frames only)
                bn::timer rt;
                rebuild_objects();
                test_rebuild_max_ticks = bn::max(int(test_rebuild_max_ticks), rt.elapsed_ticks());
            }

            process_events();
            sync_monster_sprites();
            update_monster_fx(heavy);
            _cfx.update(heavy);
            test_cfx_max_ticks = _cfx.max_ticks();
            test_cell = _cell;
            test_dir = _dir;
            update_hud();
            test_combat_ticks = combat_ticks;
            test_combat_max_ticks = bn::max(int(test_combat_max_ticks), combat_ticks);
            int frame_ticks = frame_timer.elapsed_ticks();

            if(_frame > 120)
            {
                test_frame_max_ticks = bn::max(int(test_frame_max_ticks), frame_ticks);
            }

            if(heavy)
            {
                test_combat_heavy_max_ticks = bn::max(int(test_combat_heavy_max_ticks), combat_ticks);
                test_frame_heavy_max_ticks = bn::max(int(test_frame_heavy_max_ticks), frame_ticks);
            }

            if(_dead_pending)
            {
                _dead_pending = false;
                enter_dead();
            }
        }

    private:
        bn::bg_palette_item _palette;
        bn::palette_bitmap_bg_ptr _bg;
        bn::palette_bitmap_bg_painter _painter;
        bn::sprite_text_generator _text;
        dl::hud _hud;
        dl::monster_fx _mfx;
        dl::combat _combat;
        dl::combat_fx _cfx;
        dl::fx_menu _menu;
        bn::vector<bn::sprite_ptr, 32> _screen_text;   // title / death screen
        int _mode = 0;          // 0 title, 1 playing, 2 dead
        int _mode_t = 0;
        bool _dead_pending = false;
        int _hud_redraws = 0;
        bn::vector<bn::sprite_ptr, 16> _object_sprites;
        bn::vector<object_sprite, 16> _objects;

        struct deco_sprite
        {
            uint8_t kind;
            uint8_t view;
            bool glow;
            bn::fixed x, y;     // resting position (transitions move it)
        };

        // step / turn transition (dl_transition.h): 0 none, 1 forward, 2 back, 3 turn left, 4 turn right
        int _trans = 0;         // 5: smooth move (G-31)
        int _mkind = 0;
        int _mt = 0;
        bool _mreverse = false;
        int _trans_t = 0;
        bn::vector<bn::fixed, dl::view::max_decos> _deco_mat_hs;
        bn::vector<bn::fixed, dl::view::max_decos> _deco_mat_vs;

        bn::vector<bn::sprite_ptr, 24> _deco_sprites;
        bn::vector<deco_sprite, 24> _deco_info;
        bn::vector<bn::sprite_affine_mat_ptr, dl::view::max_decos> _deco_mats;
        bn::vector<int, dl::view::max_decos> _deco_mat_keys;
        bn::vector<bn::sprite_palette_ptr, 3> _glow_palettes;     // torch glow light [d = 0 / 1 / >= 2]
        bn::vector<bn::sprite_palette_ptr, 6> _deco_palettes;     // [palette A / B][band d = 0 / 1 / >= 2] (theme)
        int _flame_frame = 0;
        int _flame_timer = 0;
        int _wave_frame = 0;
        int _wave_timer = 0;
        int _page = 0;
        int _floor = 0;
        int _cell = 0;
        int _dir = 0;
        int _cooldown = 0;
        int _buffered = 0;      // 1 press buffered during the cooldown (DISENO.md 1)
        int _shake = 0;
        int _frame = 0;
        int _spike_state = -1;
        bool _dirty = true;
        bool _redraw_objects = true;

        void message(const char* text)
        {
            _hud.message(text);
        }

        void load_floor(int floor)
        {
            _floor = floor;
            test_floor = floor;
            dl::themes::apply_for_floor(_bg.palette(), floor);     // G-37: theme per floor
            test_theme = dl::themes::current();
            create_deco_palettes();
            bn::timer gen_timer;
            test_floor_seed = generate_floor(floor_seed(floor), floor, gs.lv);
            test_gen_ticks = gen_timer.elapsed_ticks();
            test_gen_max_ticks = test_gen_ticks > test_gen_max_ticks ? test_gen_ticks : test_gen_max_ticks;

            for(int i = 0; i < 2 * N; ++i)
            {
                gs.es.open[i] = 0;
            }

            for(int c = 0; c < N; ++c)
            {
                gs.seen[c] = 0;
            }

            index_entities();
            dl::view::minimap_reset(gs.lv, gs.es, gs.seen);
            _cell = gs.lv.entry;
            _dir = gs.lv.entry_dir;
            _combat.player().keys = 0;     // DISENO.md 5: keys are per floor
            _combat.load_floor(gs.lv, gs.es, gs.ent_of_cell, floor, test_floor_seed);
            _cfx.clear();
            _objects.clear();
            _object_sprites.clear();
            _mfx.clear();
            clear_decos();
            _dirty = true;
        }

        // Floor generation blocks for a while (measured: test_gen_ticks): show the panel with a "DESCENDING..."
        // line in the view instead of a frozen or black screen.
        void descend(int floor)
        {
            _objects.clear();
            _object_sprites.clear();
            clear_decos();
            _hud.clear_message();
            _hud.set_visible(false);

            for(int k = 0; k < 2; ++k)
            {
                _painter.blit(0, 0, bn::palette_bitmap_items::ui_frame.pixels_item());
                _painter.flip_page_now();
            }

            bn::vector<bn::sprite_ptr, 16> text;
            _text.set_center_alignment();
            _text.generate(dl::geo::view_w / 2 - 120, dl::geo::view_h / 2 - 80, "DESCENDING...", text);
            bn::core::update();
            load_floor(floor);
            _hud.invalidate();
            _hud.set_visible(true);
        }

        // ---- input and movement
        int read_command()
        {
            if(bn::keypad::up_held())
            {
                return 1;
            }

            if(bn::keypad::down_held())
            {
                return 2;
            }

            if(bn::keypad::left_held())
            {
                return 3;
            }

            if(bn::keypad::right_held())
            {
                return 4;
            }

            if(bn::keypad::l_held())
            {
                return 5;
            }

            if(bn::keypad::r_held())
            {
                return 6;
            }

            return 0;
        }

        int pressed_command()
        {
            if(bn::keypad::up_pressed())
            {
                return 1;
            }

            if(bn::keypad::down_pressed())
            {
                return 2;
            }

            if(bn::keypad::left_pressed())
            {
                return 3;
            }

            if(bn::keypad::right_pressed())
            {
                return 4;
            }

            if(bn::keypad::l_pressed())
            {
                return 5;
            }

            if(bn::keypad::r_pressed())
            {
                return 6;
            }

            return 0;
        }

        void handle_input()
        {
            if(_cooldown > 0)
            {
                --_cooldown;

                if(int p = pressed_command())
                {
                    _buffered = p;
                }

                return;
            }

            int cmd = _buffered ? _buffered : read_command();
            _buffered = 0;

            if(! cmd)
            {
                return;
            }

            if(cmd == 3 || cmd == 4)
            {
                const uint16_t* old_page = dl::view::front_page(_painter.page().data());
                int old_dir = _dir;
                _dir = (_dir + (cmd == 3 ? 3 : 1)) & 3;

                if(dl::fx::enabled.smooth_moves)
                {
                    // turning left plays the right turn from the new facing backwards
                    _cooldown = motion_cooldown;
                    start_motion(dl::view::MOTION_TURN, _cell, cmd == 4 ? old_dir : _dir, _cell,
                                 cmd == 4 ? _dir : old_dir, cmd == 3);
                    return;
                }

                _cooldown = turn_cooldown;
                start_turn(old_page, cmd == 4);
                return;
            }

            static constexpr int rel[7] = { 0, 0, 2, 0, 0, 3, 1 };
            try_move((_dir + rel[cmd]) & 3, cmd == 1);
            _cooldown = _trans == 5 ? motion_cooldown : step_cooldown;
        }

        void bump()
        {
            _shake = 8;
            test_bumps = test_bumps + 1;
        }

        void try_move(int d, bool forward)
        {
            level& lv = gs.lv;
            int idx = edge_index(_cell, d);

            if(idx < 0)
            {
                bump();
                return;
            }

            int k = lv.ed[idx] & 15;

            for(int i = 0; i < lv.nhooks; ++i)
            {
                hook& h = lv.hooks[i];

                if(h.type == H_LEVER && h.lever_edge == idx)
                {
                    if(! gs.es.open[idx])
                    {
                        set_open(idx);
                        set_open(h.target);
                        test_levers = test_levers + 1;
                        message("A GATE OPENED");
                        _dirty = true;
                    }
                    else
                    {
                        bump();
                    }

                    return;
                }
            }

            bool pass = false;

            switch(k)
            {
            case K_OPEN:
            case K_ARCH:
                pass = true;
                break;

            case K_DOOR:
                if(gs.es.open[idx])
                {
                    pass = true;
                }
                else
                {
                    set_open(idx);    // walking against a closed door opens it (R14)
                    _dirty = true;
                    return;
                }
                break;

            case K_LOCKED:
                if(gs.es.open[idx])
                {
                    pass = true;
                }
                else if(_combat.player().keys > 0)
                {
                    --_combat.player().keys;
                    set_open(idx);
                    message("UNLOCKED");
                    _dirty = true;
                    return;
                }
                else
                {
                    message("LOCKED. FIND A KEY.");
                    bump();
                    return;
                }
                break;

            case K_BARS:
                pass = gs.es.open[idx];

                if(! pass)
                {
                    message("BARS: FIND THE LEVER");
                }
                break;

            case K_STAIRS:
                if(forward)
                {
                    descend(_floor + 1);
                    bn::string<24> t("FLOOR ");
                    t.append(bn::to_string<4>(_floor + 1));
                    message(t.c_str());
                    return;
                }
                break;

            default:
                break;
            }

            int nb = neighbour(_cell, d);

            if(! pass || nb < 0)
            {
                bump();
                return;
            }

            int e = gs.ent_of_cell[nb];

            if(_combat.monster_blocks(nb) || (e >= 0 && furn_blocking(lv.ents[e])))
            {
                bump();
                return;
            }

            int from = _cell;
            _cell = nb;
            test_moves = test_moves + 1;

            if(dl::fx::enabled.smooth_moves)
            {
                // the back step and the left strafe play the forward / right poses from the destination backwards
                bool back = d == ((_dir + 2) & 3);
                bool left = d == ((_dir + 3) & 3);
                int kind = (forward || back) ? dl::view::MOTION_STEP : dl::view::MOTION_STRAFE;
                bool reverse = back || left;
                start_motion(kind, reverse ? nb : from, _dir, reverse ? from : nb, _dir, reverse);
            }
            else if(forward)
            {
                // the old view zooms toward the vanishing point, then the new one is drawn
                dl::view::snapshot(dl::view::front_page(_painter.page().data()), 0);
                _trans = 1;
                _trans_t = 0;
            }
            else if(d == ((_dir + 2) & 3))
            {
                // back: the new view (drawn next frame) starts zoomed in and settles
                _trans = 2;
                _trans_t = -1;
            }
            else
            {
                _dirty = true;
            }

            if(e >= 0 && (lv.ents[e].type == E_KEY || lv.ents[e].type == E_ITEM))
            {
                _combat.pickup(e);
            }

            if(e >= 0 && lv.ents[e].type == E_TRAP && lv.ents[e].c == TRAP_SPIKES)
            {
                message("WAIT FOR THE SPIKES TO DROP");

                if(_spike_state == 2)
                {
                    spike_hit();
                }
            }
        }

        // ---- redraw
        void redraw(bool flip = true)
        {
            _dirty = false;
            bn::timer timer;
            uint16_t* page = _painter.page().data();
            dl::view::blit_stat_spans = 0;
            dl::view::blit_stat_halfwords = 0;
            dl::view::draw(page, gs.lv, gs.es, _cell, _dir, gs.vis);
            test_view_ticks = timer.elapsed_ticks();
            test_view_items = dl::view::drawn_items();
            test_view_spans = dl::view::blit_stat_spans;
            test_view_halfwords = dl::view::blit_stat_halfwords;
            test_prepare_max_ticks = bn::max(int(test_prepare_max_ticks), dl::view::prepare_ticks());
            test_view_max_ticks = test_view_ticks > test_view_max_ticks ? test_view_ticks : test_view_max_ticks;

            for(int d = 0; d < dl::view::rows; ++d)
            {
                for(int x = 0; x < dl::view::cols; ++x)
                {
                    mark_seen(gs.vis.cell[d][x]);
                }
            }

            mark_seen(_cell);
            dl::view::minimap_update(gs.lv, gs.es, gs.seen);
            int map_update_ticks = timer.elapsed_ticks() - test_view_ticks;
            test_map_update_max_ticks = map_update_ticks > test_map_update_max_ticks ? map_update_ticks : test_map_update_max_ticks;
            dl::view::draw_minimap(page, _page, dl::hud::minimap_x, dl::hud::minimap_y, _cell, _dir);

            if(flip)
            {
                _page ^= 1;
            }

            int ticks = timer.elapsed_ticks();
            test_map_ticks_sum = test_map_ticks_sum + (ticks - test_view_ticks);
            test_map_max_ticks = ticks - test_view_ticks > test_map_max_ticks ? ticks - test_view_ticks : test_map_max_ticks;
            test_view_ticks_sum = test_view_ticks_sum + test_view_ticks;
            if(flip)
            {
                _painter.flip_page_later();
            }

            test_redraw_ticks = ticks;
            test_redraw_max_ticks = ticks > test_redraw_max_ticks ? ticks : test_redraw_max_ticks;
            test_redraws = test_redraws + 1;
            rebuild_objects();
            rebuild_decos();

            for(int d = 1; d < dl::view::rows; ++d)
            {
                for(int x = 0; x < dl::view::cols; ++x)
                {
                    if(gs.vis.cell[d][x] == gs.lv.stairs && gs.vis.cell[d][x] >= 0)
                    {
                        _combat.hint(dl::HINT_STAIRS, "STAIRS DOWN: WALK IN");
                    }
                }
            }
        }

        void mark_seen(int c)
        {
            if(c >= 0 && ! gs.seen[c])
            {
                gs.seen[c] = 1;
                dl::view::minimap_touch(c);
            }
        }

        // an edge changed state (door, lock, bars, lever): its minimap lines change
        void set_open(int idx)
        {
            gs.es.open[idx] = 1;
            dl::view::minimap_touch(edge_a(idx));
            dl::view::minimap_touch(edge_b(idx));
        }

        // HUD (dl_hud.h): stats the game owns today; hp / potions / reload are placeholders until combat
        void update_hud()
        {
            const dl::player_stats& p = _combat.player();
            _hud.state.hp = p.hp;
            _hud.state.hp_max = p.hp_max;
            _hud.state.potions = p.potions;
            _hud.state.keys = p.keys;
            _hud.state.reload = p.reload;
            _hud.state.reload_max = _combat.reload_max();
            _hud.state.floor = _floor;
            _hud.state.dir = _dir;
            int redraws = test_redraws;
            _hud.update(_painter, _trans || redraws != _hud_redraws);
            _hud_redraws = redraws;
            test_hud_ticks = _hud.last_ticks();
            test_hud_max_ticks = _hud.max_ticks();
            test_sprite_tiles = bn::sprite_tiles::used_tiles_count();
            test_sprites = bn::sprites::used_items_count();
            test_sprite_tiles_free = bn::sprite_tiles::available_tiles_count();
            test_hud_sprites = _hud.sprites_count();
            test_hud_tiles = _hud.sprite_tiles_count();
        }

        // world sprite of an entity in base-relative cell (d, x): item and base size, or nullptr
        const bn::sprite_item* object_item(int ent, int& size, bool& monster, bool& spikes) const
        {
            const entity& e = gs.lv.ents[ent];
            monster = e.type == E_MONSTER;
            spikes = e.type == E_TRAP && e.c == TRAP_SPIKES;
            size = monster ? dl::geo::monster_size : dl::geo::object_size;

            if(monster)
            {
                return monster_item(ent);
            }

            if(e.type == E_ITEM || e.type == E_KEY)
            {
                return &bn::sprite_items::item_floor;
            }

            if(e.type == E_FURN)
            {
                return furn_item(e.c);
            }

            return spikes ? &bn::sprite_items::prop_spikes : nullptr;
        }

        // Screen position (centre, sprite coordinates) and scale of object i at pose (kind, k) of the current base;
        // false when it is not seen (behind the eye, off the view or behind a nearer wall). Objects stand 0.2 cells
        // in front of their cell centre, toward the camera (so during a turn they slide with it).
        bool object_place(const object_sprite& o, int kind, int k, bn::fixed& sx, bn::fixed& sy, bn::fixed& scale,
                          int& band) const
        {
            namespace geo = dl::geo;
            int fx, fz;
            dl::view::forward(kind, k, fx, fz);
            int xu = geo::cell_x_u(o.x) - ((geo::object_front_u * fx) >> 14);
            int zu = geo::cell_z_u(o.d) - ((geo::object_front_u * fz) >> 14);
            int vx, z;

            if(! dl::view::project(kind, k, xu, zu, vx, z) || z < geo::cell_u / 2)
            {
                return false;
            }

            scale = bn::fixed(geo::object_ref_u) / z;
            int size = o.monster ? geo::monster_size : geo::object_size;

            if(vx < 8 || vx > geo::view_w - 8 || scale > 2)
            {
                return false;
            }

            // hidden when a wall nearer than the object is what the view shows anywhere across the middle 3/4 of
            // its sprite (the outer eighths are mostly the transparent margin of the art)
            int hw = (scale * (size * 3 / 8)).round_integer();

            if(dl::view::occluded(vx - hw, vx + hw, z))
            {
                return false;
            }

            sx = vx - 120;
            sy = bn::fixed(geo::floor_y(z)) - scale * (size / 2) - 80;
            band = geo::depth_band(z);
            return true;
        }

        void add_object(int ent, int d, int x)
        {
            int size;
            bool monster, spikes;
            const bn::sprite_item* item = object_item(ent, size, monster, spikes);

            if(! item || _objects.full())
            {
                return;
            }

            object_sprite o{ ent, int8_t(d), int8_t(x), monster, spikes, 1, -1, 1, 0, 0, 1, 0, 0, 1, monster_lerp_frames };
            const entity& e = gs.lv.ents[ent];
            int frame = spikes ? (_spike_state > 0 ? 1 : 0) :
                        e.type == E_KEY ? dl::ITEM_KEY : e.type == E_ITEM ? e.b % dl::ITEM_COUNT : 0;
            bn::sprite_ptr s = item->create_sprite(0, 0, frame);
            s.set_bg_priority(0);
            s.set_visible(false);

            _object_sprites.push_back(bn::move(s));
            _objects.push_back(o);
        }

        // objects in the visible cells (d >= 1): monsters interpolate from where they were drawn last time
        void rebuild_objects()
        {
            bn::vector<object_sprite, 16> old = _objects;
            bn::vector<bn::fixed, 16> old_x, old_y, old_s;

            for(const bn::sprite_ptr& s : _object_sprites)
            {
                old_x.push_back(s.x());
                old_y.push_back(s.y());
                old_s.push_back(s.horizontal_scale());
            }

            _objects.clear();
            _object_sprites.clear();

            for(int d = dl::view::rows - 1; d >= 1; --d)
            {
                for(int xi = 0; xi < dl::view::cols; ++xi)
                {
                    int c = gs.vis.cell[d][xi];

                    if(c >= 0 && gs.ent_of_cell[c] >= 0)
                    {
                        add_object(gs.ent_of_cell[c], d, xi - dl::view::half);
                    }

                    if(c >= 0 && _combat.monster_ent_at(c) >= 0)
                    {
                        add_object(_combat.monster_ent_at(c), d, xi - dl::view::half);
                    }
                }
            }

            for(int i = 0, n = _objects.size(); i < n; )
            {
                object_sprite& o = _objects[i];
                bn::fixed sx, sy, scale;
                int band;

                if(! object_place(o, 0, 0, sx, sy, scale, band))
                {
                    _objects.erase(_objects.begin() + i);
                    _object_sprites.erase(_object_sprites.begin() + i);
                    --n;
                    continue;
                }

                o.from_x = o.to_x = sx;
                o.from_y = o.to_y = sy;
                o.from_s = o.to_s = scale;
                o.band = int8_t(band);

                for(int j = 0, m = old.size(); j < m; ++j)
                {
                    if(old[j].ent == o.ent && o.monster)
                    {
                        // from what the screen shows now (the last pose of a smooth move or a monster in motion)
                        o.from_x = old_x[j];
                        o.from_y = old_y[j];
                        o.from_s = old_s[j];
                        o.t = 0;
                    }
                }

                bn::sprite_ptr& s = _object_sprites[i];
                s.set_position(o.from_x, o.from_y);
                s.set_scale(o.from_s);
                s.set_z_order(4 * band);       // in front of the wall decorations of its row (4 d + 2)
                s.set_visible(true);
                ++i;
            }

            test_visible_objects = _objects.size();
            _combat.objects_dirty = false;

            for(const object_sprite& o : _objects)
            {
                if(o.monster)
                {
                    _combat.hint(dl::HINT_MONSTER, "A: ATTACK WHEN BAR IS FULL");
                }
            }
        }

        // smooth move: every object of the cells either end of the move sees, placed per pose
        void collect_motion_objects()
        {
            _objects.clear();
            _object_sprites.clear();

            for(int d = 6; d >= -4; --d)
            {
                for(int x = -5; x <= 5; ++x)
                {
                    int c = dl::view::motion_cell(d, x);

                    if(c >= 0 && gs.ent_of_cell[c] >= 0)
                    {
                        add_object(gs.ent_of_cell[c], d, x);
                    }

                    if(c >= 0 && _combat.monster_ent_at(c) >= 0)
                    {
                        add_object(_combat.monster_ent_at(c), d, x);
                    }
                }
            }
        }

        void place_objects(int kind, int k)
        {
            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                object_sprite& o = _objects[i];
                bn::sprite_ptr& s = _object_sprites[i];
                bn::fixed sx, sy, scale;
                int band;

                if(! object_place(o, kind, k, sx, sy, scale, band))
                {
                    s.set_visible(false);
                    continue;
                }

                o.from_x = o.to_x = sx;
                o.from_y = o.to_y = sy;
                o.from_s = o.to_s = scale;
                o.band = int8_t(band);
                o.t = monster_lerp_frames;
                s.set_position(sx, sy);
                s.set_scale(scale);
                s.set_z_order(4 * band);
                s.set_visible(true);
            }
        }

        // ---- wall decorations (sprites on their wall face; the bitmap never contains them)
        void create_deco_palettes()
        {
            // G-37: colours of the current theme (dl_themes.h); called again after a theme change
            static bn::color colors[2][3][16];
            static constexpr int band_x100[3] = { 100, 60, 36 };   // ~ wall attenuation at d = 0, 1 and >= 2
            bool update = ! _deco_palettes.empty();

            for(int p = 0; p < 2; ++p)
            {
                const bn::color* src = dl::themes::deco_colors(p);

                for(int b = 0; b < 3; ++b)
                {
                    for(int i = 0; i < 16; ++i)
                    {
                        const bn::color& c = src[i];
                        int f = (p == 0 && dl::deco::lit_index_a(i)) ? 100 : band_x100[b];
                        colors[p][b][i] = i ? bn::color(c.red() * f / 100, c.green() * f / 100, c.blue() * f / 100) : c;
                    }

                    bn::span<const bn::color> span(colors[p][b], 16);

                    if(update)
                    {
                        _deco_palettes[p * 3 + b].set_colors(span);
                    }
                    else
                    {
                        _deco_palettes.push_back(
                                bn::sprite_palette_item(span, bn::bpp_mode::BPP_4).create_palette());
                    }
                }
            }

            // glow light per index of deco_torch_glow (1 = outer ring .. 7 = centre): added to the scene, so the
            // rim adds nothing (index 1 rounds to black in 5-bit colour) and the centre adds a warm ~45 %
            static bn::color glow_colors[3][16];
            static constexpr int ramp_x100[8] = { 0, 2, 5, 9, 15, 22, 31, 42 };
            static constexpr int band_glow_x100[3] = { 100, 85, 65 };   // d = 0, d = 1, d >= 2: small far halos need more

            for(int b = 0; b < 3; ++b)
            {
                for(int i = 0; i < 16; ++i)
                {
                    int f = i < 8 ? ramp_x100[i] * band_glow_x100[b] : 0;   // x 10000
                    const uint8_t* rgb = dl::themes::glow_rgb();
                    glow_colors[b][i] = bn::color(rgb[0] * f / 10000, rgb[1] * f / 10000, rgb[2] * f / 10000);
                }

                bn::span<const bn::color> span(glow_colors[b], 16);

                if(update)
                {
                    _glow_palettes[b].set_colors(span);
                }
                else
                {
                    _glow_palettes.push_back(bn::sprite_palette_item(span, bn::bpp_mode::BPP_4).create_palette());
                }
            }
        }

        void clear_decos()
        {
            _deco_sprites.clear();
            _deco_info.clear();
            _deco_mats.clear();
            _deco_mat_keys.clear();
            _deco_mat_hs.clear();
            _deco_mat_vs.clear();
        }

        int deco_frame(int kind) const
        {
            switch(kind)
            {
            case dl::view::DECO_TORCH:
                return _flame_frame;
            case dl::view::DECO_BANNER:
                return _wave_frame;
            case dl::view::DECO_LEVER_DOWN:
                return 1;
            default:
                return 0;
            }
        }

        const bn::sprite_affine_mat_ptr& deco_mat(int key, bn::fixed hs, bn::fixed vs)
        {
            for(int i = 0, n = _deco_mat_keys.size(); i < n; ++i)
            {
                if(_deco_mat_keys[i] == key)
                {
                    return _deco_mats[i];
                }
            }

            bn::sprite_affine_mat_ptr m = bn::sprite_affine_mat_ptr::create();
            m.set_horizontal_scale(hs);
            m.set_vertical_scale(vs);
            _deco_mats.push_back(bn::move(m));
            _deco_mat_keys.push_back(key);
            _deco_mat_hs.push_back(hs);
            _deco_mat_vs.push_back(vs);
            return _deco_mats.back();
        }

        void rebuild_decos()
        {
            namespace geo = dl::geo;
            namespace deco = dl::deco;
            clear_decos();

            for(int i = 0; i < gs.vis.ndecos; ++i)
            {
                const dl::view::deco_place& p = gs.vis.decos[i];
                const deco::art& a = deco::arts[p.kind];
                int view = p.view;      // dl::deco::VIEW_FRONT / LEFT / RIGHT
                int z_u = p.z_u;
                bn::fixed max_scale = bn::fixed(geo::deco_max_scale_x100) / 100;
                bn::fixed vs = bn::min(bn::fixed(geo::deco_ref_u) / z_u, max_scale);
                bn::fixed hs = vs;

                if(view != deco::VIEW_FRONT && dl::fx::enabled.lateral_squeeze)
                {
                    // real foreshortening |X| / Z of the face at its anchor against the 0.32 the art is drawn with
                    int ax = p.x - geo::vp_x;
                    bn::fixed k = bn::fixed(100 * (ax < 0 ? -ax : ax)) / (96 * deco::lateral_squeeze_x100);
                    hs = bn::min(vs * bn::max(bn::min(k, bn::fixed(1.5)), bn::fixed(0.6)), max_scale);
                }

                bn::fixed cx = p.x - (a.anchor_x[view] - 16) * hs;
                bn::fixed half_w = hs * 16;

                if(cx - half_w < 0 || cx + half_w > geo::view_w || _deco_sprites.size() + 2 > _deco_sprites.max_size())
                {
                    continue;
                }

                int band = geo::depth_band(z_u);
                const bn::sprite_affine_mat_ptr& mat = deco_mat(hs.data() | (vs.data() << 16), hs, vs);
                bn::fixed sx = cx - 120;
                bn::fixed sy = geo::vp_y - 80;     // the anchor row cancels out: sprite centre = eye row
                bn::sprite_ptr s = a.item[view]->create_sprite(sx, sy, deco_frame(p.kind));
                s.set_affine_mat(mat);
                s.set_bg_priority(0);
                s.set_z_order(4 * band + 2);

                // theme colours at every band (G-37); faded with the depth when deco_depth_fade
                s.set_palette(_deco_palettes[(a.palette_b ? 3 : 0) +
                        (dl::fx::enabled.deco_depth_fade ? (band >= 2 ? 2 : band) : 0)]);

                if(p.kind == dl::view::DECO_LEVER_UP || p.kind == dl::view::DECO_LEVER_DOWN)
                {
                    test_lever_frame = deco_frame(p.kind);
                }

                _deco_sprites.push_back(bn::move(s));
                _deco_info.push_back(deco_sprite{ p.kind, uint8_t(view), false, sx, sy });

                if(p.kind == dl::view::DECO_TORCH && dl::fx::enabled.torch_glow)
                {
                    bn::fixed gx = sx + (deco::glow_x[view] - 16) * hs;
                    bn::fixed gy = sy + (deco::glow_y - 32) * vs;
                    bn::sprite_ptr g = deco::torch_glow.create_sprite(gx, gy);
                    g.set_affine_mat(mat);
                    g.set_bg_priority(0);
                    g.set_z_order(4 * band + 3);     // behind its torch (no blending between sprites)
                    g.set_blending_enabled(true);
                    g.set_palette(_glow_palettes[band < 2 ? band : 2]);
                    _deco_sprites.push_back(bn::move(g));
                    _deco_info.push_back(deco_sprite{ p.kind, uint8_t(view), true, gx, gy });
                }
            }

            test_visible_decos = _deco_info.size();
        }

        // ---- step / turn transitions (the logic moved already; only the picture catches up)
        static constexpr int zoom_frames = 6;          // composed on odd frames only: 3 images at 30 Hz
                                                        // (measured: a zoom compose takes ~0.77 f)
        static constexpr int turn_frames = 6;
        static constexpr int zoom_max_q8 = 287;         // 1.12

        void start_turn(const uint16_t* old_page, bool right)
        {
            dl::view::snapshot(old_page, 0);           // the new view is drawn next frame (no lag frame)
            _trans = right ? 4 : 3;
            _trans_t = -1;
        }

        void show_world_sprites(bool visible)
        {
            for(bn::sprite_ptr& s : _deco_sprites)
            {
                s.set_visible(visible);
            }

            for(bn::sprite_ptr& s : _object_sprites)
            {
                s.set_visible(visible);
            }
        }

        // sprites follow the zoom of the bitmap: p' = vp + (p - vp) * k + (0, dy)
        void transform_world_sprites(bn::fixed k, int dy)
        {
            constexpr int vpx = dl::geo::vp_x - 120;
            constexpr int vpy = dl::geo::vp_y - 80;

            for(int i = 0, n = _deco_sprites.size(); i < n; ++i)
            {
                const deco_sprite& d = _deco_info[i];
                _deco_sprites[i].set_position(vpx + (d.x - vpx) * k, vpy + (d.y - vpy) * k + dy);
            }

            for(int i = 0, n = _deco_mats.size(); i < n; ++i)
            {
                _deco_mats[i].set_horizontal_scale(bn::min(_deco_mat_hs[i] * k, bn::fixed(2)));
                _deco_mats[i].set_vertical_scale(bn::min(_deco_mat_vs[i] * k, bn::fixed(2)));
            }

            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                const object_sprite& o = _objects[i];
                bn::sprite_ptr& s = _object_sprites[i];
                s.set_position(vpx + (o.to_x - vpx) * k, vpy + (o.to_y - vpy) * k + dy);
                s.set_scale(bn::min(o.to_s * k, bn::fixed(2)));
            }
        }

        // ---- G-31 smooth moves: real intermediate camera poses (dl_view.h), one image every 2 frames (30 fps):
        // poses 1..5 on frames 0, 2, .. 8 of the move, the new resting view on frame 10, the next move on frame 12
        static constexpr int motion_cooldown = 2 * (dl::view::motion_frames + 1) - 1;

        void start_motion(int kind, int base_cell, int base_dir, int dest_cell, int dest_dir, bool reverse)
        {
            dl::view::begin_motion(gs.lv, gs.es, base_cell, base_dir, dest_cell, dest_dir);
            _mkind = kind;
            _mreverse = reverse;
            _mt = 0;
            _trans = 5;
            _dirty = false;
            clear_decos();
            collect_motion_objects();
        }

        void smooth_step()
        {
            int t = _mt++;

            if(t & 1)
            {
                return;                 // hold the composed pose one more frame
            }

            int k = t / 2 + 1;

            if(k > dl::view::motion_frames)
            {
                _trans = 0;
                redraw();
                return;
            }

            int pk = _mreverse ? dl::view::motion_frames + 1 - k : k;
            uint16_t* page = _painter.page().data();
            bn::timer timer;
            dl::view::draw_motion(page, _mkind, pk, gs.vis);
            test_motion_ticks = timer.elapsed_ticks();
            test_motion_max_ticks = test_motion_ticks > test_motion_max_ticks ? test_motion_ticks : test_motion_max_ticks;
            test_motion_items = dl::view::drawn_items();
            test_motion_frames = test_motion_frames + 1;
            dl::view::draw_minimap(page, _page, dl::hud::minimap_x, dl::hud::minimap_y, _cell, _dir);
            _painter.flip_page_later();
            _page ^= 1;
            place_objects(_mkind, pk);
            rebuild_decos();
        }

        void step_transition()
        {
            if(_trans == 5)
            {
                smooth_step();
                return;
            }

            int t = ++_trans_t;
            uint16_t* page = _painter.page().data();
            bool done;

            if(t == 0)
            {
                // back / turn: draw the new view into the hidden page and keep it as a snapshot
                redraw(false);
                dl::view::snapshot(page, _trans == 2 ? 0 : 1);

                if(_trans != 2)
                {
                    show_world_sprites(false);
                }

                return;
            }

            if(_trans <= 2)
            {
                // forward: 1.0 -> 1.12 on the old view; back: 1.12 -> 1.0 on the new one. Head bob 1-2 px down.
                static constexpr int bob[zoom_frames + 1] = { 0, 1, 1, 2, 2, 0, 0 };

                if(! (t & 1))
                {
                    if(t >= zoom_frames)
                    {
                        if(_trans == 1)
                        {
                            _dirty = true;  // the new view is drawn this frame
                        }
                        else
                        {
                            transform_world_sprites(1, 0);
                        }

                        _trans = 0;
                    }

                    return;                 // hold the composed image one more frame
                }

                int k = t + 1;              // 2, 4, 6
                int scale = _trans == 1 ? 256 + (zoom_max_q8 - 256) * k / zoom_frames :
                        zoom_max_q8 - (zoom_max_q8 - 256) * k / zoom_frames;
                int dy = dl::fx::enabled.head_bob ? bob[t] : 0;

                if(! dl::fx::enabled.step_zoom)
                {
                    scale = 256;
                }

                bn::timer ct;
                dl::view::compose_zoom(page, 0, scale, dy);
                int ticks = ct.elapsed_ticks();
                test_compose_max_ticks = ticks > test_compose_max_ticks ? ticks : test_compose_max_ticks;
                transform_world_sprites(bn::fixed(scale) / 256, dy);
                done = false;               // ends on the next even frame
            }
            else
            {
                int offset = (dl::geo::view_w * t / turn_frames) & ~1;
                dl::view::compose_slide(page, 0, 1, dl::fx::enabled.turn_slide ? offset : dl::geo::view_w, _trans == 4);
                done = t >= turn_frames || ! dl::fx::enabled.turn_slide;
            }

            dl::view::draw_minimap(page, _page, dl::hud::minimap_x, dl::hud::minimap_y, _cell, _dir);
            _painter.flip_page_later();
            _page ^= 1;
            test_transition_frames = test_transition_frames + 1;

            if(done)
            {
                if(_trans == 1)
                {
                    _dirty = true;          // the new view is drawn next frame
                }
                else
                {
                    transform_world_sprites(1, 0);
                    show_world_sprites(true);
                }

                _trans = 0;
            }
        }

        // one place for the per-frame effects (dl_fx.h switches); G-27 hangs its effects here
        void update_effects()
        {
            const dl::deco::art& torch = dl::deco::arts[dl::view::DECO_TORCH];
            const dl::deco::art& banner = dl::deco::arts[dl::view::DECO_BANNER];
            bool flame = dl::fx::enabled.torch_flicker && ++_flame_timer >= torch.frame_ticks[_flame_frame];
            bool wave = dl::fx::enabled.banner_wave && ++_wave_timer >= banner.frame_ticks[_wave_frame];

            if(flame)
            {
                _flame_timer = 0;
                _flame_frame = (_flame_frame + 1) % torch.frames;
            }

            if(wave)
            {
                _wave_timer = 0;
                _wave_frame = (_wave_frame + 1) % banner.frames;
            }

            if(! flame && ! wave)
            {
                return;
            }

            for(int i = 0, n = _deco_info.size(); i < n; ++i)
            {
                const deco_sprite& d = _deco_info[i];

                if(! d.glow && ((flame && d.kind == dl::view::DECO_TORCH) || (wave && d.kind == dl::view::DECO_BANNER)))
                {
                    const bn::sprite_item* item = dl::deco::arts[d.kind].item[d.view];
                    _deco_sprites[i].set_tiles(item->tiles_item(), deco_frame(d.kind));
                }
            }
        }

        // G-34 monster readability (dl_monster_fx.h): palette, flashes, breathing and halo of every monster on
        // screen; runs after the redraw so new sprites never show one frame with the plain palette
        void update_monster_fx(bool heavy)
        {
            bn::timer timer;
            _mfx.begin_frame(heavy);

            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                const object_sprite& o = _objects[i];

                if(o.monster)
                {
                    _mfx.apply(o.ent, *monster_item(o.ent), _object_sprites[i], o.band);
                }
            }

            _mfx.end_frame();
            test_mfx_ticks = timer.elapsed_ticks();
            test_mfx_max_ticks = bn::max(int(test_mfx_max_ticks), int(test_mfx_ticks));

            if(heavy)
            {
                test_mfx_heavy_max_ticks = bn::max(int(test_mfx_heavy_max_ticks), int(test_mfx_ticks));
            }
            test_mfx_slots = _mfx.slots_used();
            test_sprite_palettes = (bn::sprite_palettes::used_colors_count() + 15) / 16;
            test_affine_mats = bn::sprite_affine_mats::used_count();
            test_cpu_x1000 = (bn::core::last_cpu_usage() * 1000).right_shift_integer();
            if(_frame > 120)
            {
                test_cpu_max_x1000 = bn::max(int(test_cpu_max_x1000), int(test_cpu_x1000));
            }
        }

        void update_objects()
        {
            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                object_sprite& o = _objects[i];

                if(o.t < monster_lerp_frames)
                {
                    ++o.t;
                    bn::fixed k = bn::fixed(o.t) / monster_lerp_frames;
                    bn::sprite_ptr& s = _object_sprites[i];
                    s.set_position(o.from_x + (o.to_x - o.from_x) * k, o.from_y + (o.to_y - o.from_y) * k);
                    s.set_scale(o.from_s + (o.to_s - o.from_s) * k);
                }
            }
        }

        void update_spikes()
        {
            int phase = _frame % spike_period;
            int state = phase < 90 ? 0 : (phase < 114 ? 1 : 2);

            if(state != _spike_state)
            {
                _spike_state = state;

                for(int i = 0, n = _objects.size(); i < n; ++i)
                {
                    if(_objects[i].spikes)
                    {
                        _object_sprites[i].set_tiles(bn::sprite_items::prop_spikes.tiles_item(), state > 0 ? 1 : 0);
                        _object_sprites[i].set_vertical_scale(_objects[i].to_s * (state == 1 ? bn::fixed(0.6) : 1));
                    }
                }

                int e = gs.ent_of_cell[_cell];

                if(state == 2 && e >= 0 && gs.lv.ents[e].type == E_TRAP)
                {
                    spike_hit();
                }
            }
        }

        void spike_hit()
        {
            test_spike_hits = test_spike_hits + 1;
            message("OUCH! THE SPIKES");
            bump();
            _combat.trap_hit(4 + 2 * bn::min(_floor, 2));     // DISENO.md 6b: 4 / 6 / 8, ignores DEF
        }

        // ---- G-35 combat feedback and screens
        bool object_screen(int ent, bn::fixed& x, bn::fixed& y) const
        {
            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                if(_objects[i].ent == ent && _object_sprites[i].visible())
                {
                    const bn::sprite_ptr& s = _object_sprites[i];
                    x = s.x();
                    y = s.y() - s.vertical_scale() * (dl::geo::monster_size * 3 / 8);
                    return true;
                }
            }

            return false;
        }

        void process_events()
        {
            for(const dl::event& ev : _combat.events)
            {
                switch(ev.type)
                {
                case dl::EV_MON_HIT:
                case dl::EV_MON_MISS:
                {
                    bn::fixed x, y;

                    if(object_screen(ev.ent, x, y))
                    {
                        bn::string<12> t;

                        if(ev.type == dl::EV_MON_MISS)
                        {
                            t = "MISS";
                        }
                        else
                        {
                            if(ev.crit)
                            {
                                t = "CRIT ";
                            }

                            t.append(bn::to_string<4>(ev.value));
                        }

                        _cfx.number(t.c_str(), x, y);
                    }
                    break;
                }

                case dl::EV_PLAYER_HIT:
                {
                    bn::string<12> t("-");
                    t.append(bn::to_string<4>(ev.value));
                    _cfx.number(t.c_str(), 108 - 120, 114 - 80);    // just above the HP bar (HUD.md)
                    _cfx.red_edge(6);
                    _shake = 6;
                    break;
                }

                case dl::EV_LEVEL_UP:
                    _cfx.gold_flash();
                    break;

                case dl::EV_PLAYER_DEAD:
                    _dead_pending = true;
                    break;

                default:
                    break;
                }
            }

            _combat.events.clear();
        }

        // animation frame and scale of every monster sprite: idle 0-1, anticipation 2 (+10 % scale), strike 3,
        // hit 4, death: hit frame shrinking to nothing in 16 f
        void sync_monster_sprites()
        {
            for(int i = 0, n = _objects.size(); i < n; ++i)
            {
                object_sprite& o = _objects[i];

                if(! o.monster)
                {
                    continue;
                }

                const dl::monster* m = _combat.monster_of_ent(o.ent);
                int frame = ((_frame + o.ent * 13) / 24) & 1;
                bn::fixed mult = 1;

                if(m)
                {
                    if(m->state == dl::MS_DYING)
                    {
                        frame = 4;
                        mult = bn::fixed(m->timer) / 16;
                    }
                    else if(m->hit)
                    {
                        frame = 4;
                    }
                    else if(m->strike)
                    {
                        frame = 3;
                    }
                    else if(m->state == dl::MS_WARN)
                    {
                        frame = 2;
                        mult = bn::fixed(1.1);
                    }
                }

                bn::sprite_ptr& s = _object_sprites[i];

                if(frame != o.frame)
                {
                    s.set_tiles(monster_item(o.ent)->tiles_item(), frame);
                    o.frame = int8_t(frame);
                }

                if(mult != 1 || o.mult != 1)
                {
                    bn::fixed base = o.t < monster_lerp_frames ?
                            o.from_s + (o.to_s - o.from_s) * (bn::fixed(o.t) / monster_lerp_frames) : o.to_s;
                    bn::fixed sc = base * mult;
                    s.set_scale(sc > bn::fixed(0.05) ? sc : bn::fixed(0.05));
                    o.mult = mult;
                }
            }
        }

        // title and death screens: the frame without the view, three centred lines
        void show_screen(const char* line0, const char* line1, const char* line2)
        {
            _objects.clear();
            _object_sprites.clear();
            clear_decos();
            _cfx.clear();
            _hud.clear_message();
            _hud.set_visible(false);
            _shake = 0;
            _bg.set_x(0);

            for(int k = 0; k < 2; ++k)
            {
                _painter.blit(0, 0, bn::palette_bitmap_items::ui_frame.pixels_item());
                _painter.flip_page_now();
            }

            _screen_text.clear();
            _text.set_center_alignment();
            int cx = dl::geo::view_w / 2 - 120;
            _text.generate(cx, -40, line0, _screen_text);

            if(line1)
            {
                _text.generate(cx, -20, line1, _screen_text);
            }

            if(line2)
            {
                _text.generate(cx, 0, line2, _screen_text);
            }

            _mode_t = 0;
        }

        void update_screen()
        {
            ++_mode_t;

            if(bn::keypad::a_pressed() && _mode_t > 20)
            {
                start_run();
            }
        }

        void start_run()
        {
#ifdef DL_DEBUG_SEED
            run_seed = DL_DEBUG_SEED;
#else
            unsigned h = unsigned(_frame) * 0x9E3779B9u ^ (run_seed << 7) ^ 0x00C0FFEEu;
            h ^= h >> 16;
            h *= 0x85EBCA6Bu;
            h ^= h >> 13;
            run_seed = h;
#endif
            test_run_seed = run_seed;
            _screen_text.clear();
            _combat.new_run();
            _mode = 1;
            test_mode = 1;
            descend(0);
            message("UP: WALK   LEFT/RIGHT: TURN");
        }

        void enter_dead()
        {
            test_deaths = test_deaths + 1;
            _mode = 2;
            test_mode = 2;
            bn::string<24> t("FLOOR ");
            t.append(bn::to_string<4>(_floor + 1));
            show_screen("YOU DIED", t.c_str(), "A: NEW RUN");
        }

        void update_shake()
        {
            if(_shake > 0)
            {
                --_shake;
                _bg.set_x(_shake ? ((_shake & 2) ? 2 : -2) : 0);
            }
        }
    };
}

int main()
{
    bn::core::init();
    game g;

    while(true)
    {
        g.update();
        bn::core::update();
    }
}
