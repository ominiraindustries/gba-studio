// DELVE (task G-25): first-person grid crawler. Block 1: generated floor (v2 port + hooks), Mode 4 view redrawn only
// on change, grid movement (DISENO.md 1), minimap and compass, scaled monsters / props, P1 lever gate and spike plate.

#include "bn_bg_palette_item.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_span.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_timer.h"
#include "bn_timers.h"
#include "bn_vector.h"

#include "bn_palette_bitmap_items_ui_frame.h"
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

#include "dl_gen.h"
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
volatile int test_view_ticks;          // view only (no minimap)
volatile int test_view_ticks_sum;
volatile int test_view_max_ticks;
volatile int test_map_ticks_sum;
volatile int test_boot_frames;         // frames spent generating floor 1 (approx.)
volatile int test_ticks_per_frame;
volatile int test_visible_objects;
volatile int test_spike_hits;
volatile int test_levers;
volatile unsigned test_floor_seed;

namespace
{
    using namespace dl::gen;

    constexpr unsigned run_seed = 0x00C0FFEE;   // PROVISIONAL: fixed run seed until the title screen picks one
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

            if(e.type == E_MONSTER || e.type == E_FURN || e.type == E_TRAP || e.type == E_KEY)
            {
                int c = e.y * W + e.x;

                if(gs.ent_of_cell[c] < 0 || e.type == E_MONSTER)
                {
                    gs.ent_of_cell[c] = int16_t(i);
                }
            }
        }
    }

    const bn::sprite_item* monster_item(int ent)
    {
        switch(ent % 3)
        {
        case 0:
            return &bn::sprite_items::mon_rat;
        case 1:
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
        bool monster;
        bool spikes;
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
            _text(common::variable_8x8_sprite_font)
        {
            test_ticks_per_frame = bn::timers::ticks_per_frame();

            for(int k = 0; k < 2; ++k)
            {
                _painter.blit(0, 0, bn::palette_bitmap_items::ui_frame.pixels_item());
                _painter.flip_page_now();
            }

            load_floor(0);
            message("UP: WALK   LEFT/RIGHT: TURN");
        }

        void update()
        {
            ++_frame;
            handle_input();
            update_spikes();
            update_objects();
            update_shake();

            if(_message_timer && --_message_timer == 0)
            {
                _message_sprites.clear();
            }

            if(_dirty)
            {
                redraw();
            }

            test_cell = _cell;
            test_dir = _dir;
        }

    private:
        bn::bg_palette_item _palette;
        bn::palette_bitmap_bg_ptr _bg;
        bn::palette_bitmap_bg_painter _painter;
        bn::sprite_text_generator _text;
        bn::vector<bn::sprite_ptr, 16> _object_sprites;
        bn::vector<object_sprite, 16> _objects;
        bn::vector<bn::sprite_ptr, 8> _status_sprites;
        bn::vector<bn::sprite_ptr, 16> _message_sprites;
        int _floor = 0;
        int _cell = 0;
        int _dir = 0;
        int _cooldown = 0;
        int _buffered = 0;      // 1 press buffered during the cooldown (DISENO.md 1)
        int _shake = 0;
        int _keys = 0;
        int _frame = 0;
        int _message_timer = 0;
        int _spike_state = -1;
        bool _dirty = true;
        bool _redraw_objects = true;

        void message(const char* text)
        {
            _message_sprites.clear();
            _text.set_left_alignment();
            _text.generate(-115, 70, text, _message_sprites);
            _message_timer = 180;
        }

        void load_floor(int floor)
        {
            _floor = floor;
            test_floor = floor;
            test_floor_seed = generate_floor(floor_seed(floor), floor, gs.lv);

            for(int i = 0; i < 2 * N; ++i)
            {
                gs.es.open[i] = 0;
            }

            for(int c = 0; c < N; ++c)
            {
                gs.seen[c] = 0;
            }

            index_entities();
            _cell = gs.lv.entry;
            _dir = gs.lv.entry_dir;
            _keys = 0;
            _objects.clear();
            _object_sprites.clear();
            _dirty = true;
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
            if(bn::keypad::b_pressed())
            {
                place_test_monster();
            }

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
                _dir = (_dir + (cmd == 3 ? 3 : 1)) & 3;
                _cooldown = turn_cooldown;
                _dirty = true;
                return;
            }

            static constexpr int rel[7] = { 0, 0, 2, 0, 0, 3, 1 };
            try_move((_dir + rel[cmd]) & 3, cmd == 1);
            _cooldown = step_cooldown;
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
                        gs.es.open[idx] = 1;
                        gs.es.open[h.target] = 1;
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
                    gs.es.open[idx] = 1;    // walking against a closed door opens it (R14)
                    _dirty = true;
                    return;
                }
                break;

            case K_LOCKED:
                if(gs.es.open[idx])
                {
                    pass = true;
                }
                else if(_keys > 0)
                {
                    --_keys;
                    gs.es.open[idx] = 1;
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
                    load_floor(_floor + 1);
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

            // PROVISIONAL until combat exists (next block): monsters do not block, or a corridor rat would wall
            // the floor off for good.
            if(e >= 0 && furn_blocking(lv.ents[e]))
            {
                bump();
                return;
            }

            _cell = nb;
            test_moves = test_moves + 1;
            _dirty = true;

            if(e >= 0 && lv.ents[e].type == E_KEY)
            {
                ++_keys;
                lv.ents[e].type = E_NONE;
                gs.ent_of_cell[nb] = -1;
                message("IRON KEY");
            }

            if(e >= 0 && lv.ents[e].type == E_TRAP && lv.ents[e].c == TRAP_SPIKES)
            {
                message("WAIT FOR THE SPIKES TO DROP");
            }
        }

        // PROVISIONAL debug (B is "quick item" in DISENO.md): the test monster 3 cells ahead, or as far as it fits
        void place_test_monster()
        {
            level& lv = gs.lv;
            int c = _cell;
            int target = -1;

            for(int d = 1; d <= 3; ++d)
            {
                int idx = edge_index(c, _dir);

                if(idx < 0 || ! sight_kind[lv.ed[idx] & 15])
                {
                    break;
                }

                c = neighbour(c, _dir);

                if(gs.ent_of_cell[c] < 0)
                {
                    target = c;
                }
            }

            if(target < 0 || lv.nents >= max_ents)
            {
                return;
            }

            lv.ents[lv.nents] = entity{ E_MONSTER, uint8_t(target % W), uint8_t(target / W), 0, 0, 0 };
            gs.ent_of_cell[target] = int16_t(lv.nents);
            ++lv.nents;
            _redraw_objects = true;
            _dirty = true;
        }

        // ---- redraw
        void redraw()
        {
            _dirty = false;
            bn::timer timer;
            uint16_t* page = _painter.page().data();
            dl::view::draw(page, gs.lv, gs.es, _cell, _dir, gs.vis);
            test_view_ticks = timer.elapsed_ticks();
            test_view_max_ticks = test_view_ticks > test_view_max_ticks ? test_view_ticks : test_view_max_ticks;
            dl::view::draw_minimap(page, 178, 58, gs.lv, gs.es, gs.seen, _cell, _dir);
            int ticks = timer.elapsed_ticks();
            test_map_ticks_sum = test_map_ticks_sum + (ticks - test_view_ticks);
            test_view_ticks_sum = test_view_ticks_sum + test_view_ticks;
            _painter.flip_page_later();
            test_redraw_ticks = ticks;
            test_redraw_max_ticks = ticks > test_redraw_max_ticks ? ticks : test_redraw_max_ticks;
            test_redraws = test_redraws + 1;

            for(int d = 0; d < dl::view::rows; ++d)
            {
                for(int x = 0; x < dl::view::cols; ++x)
                {
                    int c = gs.vis.cell[d][x];

                    if(c >= 0)
                    {
                        gs.seen[c] = 1;
                    }
                }
            }

            gs.seen[_cell] = 1;
            rebuild_objects();
            update_status();
        }

        void update_status()
        {
            static constexpr const char* names[4] = { "N", "E", "S", "W" };
            _status_sprites.clear();
            _text.set_center_alignment();
            _text.generate(88, -66, names[_dir], _status_sprites);
            bn::string<16> f("FLOOR ");
            f.append(bn::to_string<4>(_floor + 1));
            _text.generate(88, -52, f, _status_sprites);

            if(_keys)
            {
                bn::string<16> k("KEYS ");
                k.append(bn::to_string<4>(_keys));
                _text.generate(88, -38, k, _status_sprites);
            }
        }

        // objects in the visible cells (d >= 1): monsters interpolate from where they were drawn last time
        void rebuild_objects()
        {
            bn::vector<object_sprite, 16> old = _objects;
            _objects.clear();
            _object_sprites.clear();
            namespace geo = dl::geo;

            for(int d = dl::view::rows - 1; d >= 1; --d)
            {
                for(int xi = 0; xi < dl::view::cols; ++xi)
                {
                    int c = gs.vis.cell[d][xi];

                    if(c < 0 || gs.ent_of_cell[c] < 0 || _objects.full())
                    {
                        continue;
                    }

                    int ent = gs.ent_of_cell[c];
                    const entity& e = gs.lv.ents[ent];
                    const bn::sprite_item* item = nullptr;
                    bool monster = e.type == E_MONSTER;
                    bool spikes = e.type == E_TRAP && e.c == TRAP_SPIKES;
                    int size = geo::object_size;

                    if(monster)
                    {
                        item = monster_item(ent);
                        size = geo::monster_size;
                    }
                    else if(e.type == E_FURN)
                    {
                        item = furn_item(e.c);
                    }
                    else if(spikes)
                    {
                        item = &bn::sprite_items::prop_spikes;
                    }

                    if(! item)
                    {
                        continue;
                    }

                    int x = xi - dl::view::half;
                    bn::fixed scale = bn::fixed(geo::object_scale_num()) / geo::object_z10(d);
                    int vx = geo::object_x(d, x);

                    if(vx < 8 || vx > geo::view_w - 8)
                    {
                        continue;
                    }

                    bn::fixed sx = vx - 120;
                    bn::fixed sy = bn::fixed(geo::object_feet_y(d)) - scale * (size / 2) - 80;
                    object_sprite o{ ent, monster, spikes, sx, sy, scale, sx, sy, scale, monster_lerp_frames };

                    if(monster)
                    {
                        for(const object_sprite& p : old)
                        {
                            if(p.ent == ent && p.monster)
                            {
                                bn::fixed k = bn::fixed(p.t) / monster_lerp_frames;
                                o.from_x = p.from_x + (p.to_x - p.from_x) * k;
                                o.from_y = p.from_y + (p.to_y - p.from_y) * k;
                                o.from_s = p.from_s + (p.to_s - p.from_s) * k;
                                o.t = 0;
                            }
                        }
                    }

                    bn::sprite_ptr s = item->create_sprite(o.from_x, o.from_y);
                    s.set_scale(o.from_s);
                    s.set_bg_priority(0);

                    if(spikes)
                    {
                        s.set_tiles(item->tiles_item(), _spike_state > 0 ? 1 : 0);
                    }

                    _object_sprites.push_back(bn::move(s));
                    _objects.push_back(o);
                }
            }

            test_visible_objects = _objects.size();
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
                    test_spike_hits = test_spike_hits + 1;
                    message("OUCH! THE SPIKES");
                    bump();
                }
            }
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
