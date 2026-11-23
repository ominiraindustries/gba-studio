#include "dl_hud.h"

#include "bn_bg_palette_ptr.h"
#include "bn_color.h"
#include "bn_keypad.h"
#include "bn_palette_bitmap_roi.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_timer.h"

#include "bn_palette_bitmap_items_ui_frame.h"
#include "bn_sprite_items_ui_arrow.h"

#include "common_variable_8x8_sprite_font.h"

namespace dl
{
    namespace
    {
        // ---- frame geometry (art/_seed/delve_ui.py; inclusive pixel rects)
        constexpr int hp_x0 = 61, hp_x1 = 154, hp_y0 = 128, hp_y1 = 133;          // HP bar inside its trough
        constexpr int reload_x0 = 5, reload_x1 = 36, reload_y0 = 153, reload_y1 = 155;
        constexpr int key_x0 = 181, key_y0 = 97, key_x1 = 196, key_y1 = 112;
        constexpr int weapon_x0 = 2, weapon_y0 = 122, weapon_x1 = 39, weapon_y1 = 157;

        // ---- UI palette indices (art/_seed/delve_art.py)
        constexpr int plate = 209;
        constexpr int brass_dark = 205, brass = 206, brass_hi = 207;
        constexpr int red_empty = 210, red_dark = 211, red = 212, red_hi = 213, red_pale = 214;
        constexpr int gold_empty = 215, gold_glint = 216;

        constexpr int glow_frames = 6;
        constexpr int max_text_delay = 8;
        constexpr int low_hp_blink = 8;

        // ---- text: Butano's variable 8x8 font recoloured (index 14 = glyph, 12 = 1 px shadow)
        constexpr bn::color text_colors(int r, int g, int b, int i)
        {
            return i == 14 ? bn::color(r, g, b) : i == 12 ? bn::color(1, 1, 2) : bn::color(0, 0, 0);
        }

#define DL_TEXT_PALETTE(name, r, g, b) \
        constexpr bn::color name[16] = { \
            text_colors(r, g, b, 0), text_colors(r, g, b, 1), text_colors(r, g, b, 2), text_colors(r, g, b, 3), \
            text_colors(r, g, b, 4), text_colors(r, g, b, 5), text_colors(r, g, b, 6), text_colors(r, g, b, 7), \
            text_colors(r, g, b, 8), text_colors(r, g, b, 9), text_colors(r, g, b, 10), text_colors(r, g, b, 11), \
            text_colors(r, g, b, 12), text_colors(r, g, b, 13), text_colors(r, g, b, 14), text_colors(r, g, b, 15) }

        DL_TEXT_PALETTE(white_colors, 29, 29, 27);
        DL_TEXT_PALETTE(gold_colors, 27, 22, 12);
        DL_TEXT_PALETTE(grey_colors, 17, 18, 19);
        DL_TEXT_PALETTE(red_colors, 29, 11, 8);

#undef DL_TEXT_PALETTE

        constexpr bn::sprite_palette_item white_text(bn::span<const bn::color>(white_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item gold_text(bn::span<const bn::color>(gold_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item grey_text(bn::span<const bn::color>(grey_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item red_text(bn::span<const bn::color>(red_colors, 16), bn::bpp_mode::BPP_4);

        // screen pixel (left / centre x, top y of the 8 px line) -> sprite_text_generator coordinates
        constexpr int tx(int x)
        {
            return x - 120;
        }

        constexpr int ty(int y)
        {
            return y + 4 - 80;
        }

        constexpr int compass_y = 6;
        constexpr int compass_x[3] = { 189, 208, 227 };     // left, centre, right letter centres
        constexpr int compass_slide[] = { 0, 1, 2, 3, 4, 6 };   // tape offset by frames left (eased)
        constexpr const char* dir_names[4] = { "N", "E", "S", "W" };

        const bn::palette_bitmap_pixels_item& frame_pixels()
        {
            return bn::palette_bitmap_items::ui_frame.pixels_item();
        }
    }

    hud::hud(bn::palette_bitmap_bg_ptr& bg) :
        _text(common::variable_8x8_sprite_font),
        _grey_palette(grey_text.create_palette()),
        _gold_palette(gold_text.create_palette())
    {
        // UI colours straight from ui_frame, so the HUD does not depend on the view palette being rebuilt.
        // 240..247 stay the engine minimap colours (tools/build_view.py).
        bn::bg_palette_ptr palette = bg.palette();
        bn::span<const bn::color> ui = bn::palette_bitmap_items::ui_frame.palette_item().colors_ref();

        // damage arrows made once and hidden (creating them on a hit measured ~150 ticks); OAM only when shown
        for(int side = 0; side < 2; ++side)
        {
            bn::sprite_ptr arrow = bn::sprite_items::ui_arrow.create_sprite(side ? tx(172) : tx(4), 60 - 80);
            arrow.set_horizontal_flip(side == 1);
            arrow.set_z_order(-100);
            arrow.set_visible(false);
            _arrow_sprites.push_back(bn::move(arrow));
        }

        for(int i = 192, n = ui.size(); i < n; ++i)     // the converter drops unused trailing colours
        {
            if(i < 240 || i >= 248)
            {
                palette.set_color(i, ui[i]);
            }
        }
    }

    void hud::invalidate()
    {
        _drawn_hp_w = -1;
        _drawn_reload_w = -1;
        _drawn_keys = -1;
        _glow_drawn = false;
        _glow_timer = 0;
    }

    void hud::set_visible(bool visible)
    {
        _visible = visible;

        auto apply = [visible](auto& sprites)
        {
            for(bn::sprite_ptr& s : sprites)
            {
                s.set_visible(visible);
            }
        };
        apply(_floor_sprites);
        apply(_hp_label_sprites);
        apply(_hp_sprites);
        apply(_key_sprites);
        apply(_potion_sprites);
        apply(_message_sprites);
        apply(_arrow_sprites);

        if(! _compass_sprites.empty())
        {
            _place_compass();       // keeps the letter behind you hidden
        }
    }

    // The line is generated in update(), on a frame without a view redraw (a 28-character line measured ~700
    // ticks, 0.16 f; on top of a 0.95 f redraw it would drop a frame).
    void hud::message(const char* text, int frames)
    {
        _pending_message.clear();
        _pending_message.append(text, bn::min(bn::string_view(text).size(), _pending_message.max_size()));
        _pending_frames = frames;
        _has_pending_message = true;
    }

    void hud::clear_message()
    {
        _message_sprites.clear();
        _message_timer = 0;
        _has_pending_message = false;
    }

    void hud::_flush_message()
    {
        _has_pending_message = false;
        _message_sprites.clear();
        _text.set_palette_item(white_text);
        _text.set_left_alignment();
        _text.set_one_sprite_per_character(false);
        _text.generate(tx(46), ty(149), _pending_message, _message_sprites);
        _message_timer = _pending_frames;

        for(bn::sprite_ptr& s : _message_sprites)
        {
            s.set_visible(_visible);
        }
    }

    void hud::show_damage_arrow(int side, int frames)
    {
        _arrow_timer[side < 0 ? 0 : 1] = frames;
    }

    void hud::update_placeholder_stats()
    {
        // PLACEHOLDER (G-32): replaced by the combat code (DISENO.md 1-2).
        if(state.reload < state.reload_max)
        {
            ++state.reload;
        }

        if(bn::keypad::a_pressed())
        {
            if(state.reload >= state.reload_max)
            {
                state.reload = 0;
            }
            else
            {
                message("A: ATTACK WHEN BAR IS FULL", 120);
            }
        }

        if(bn::keypad::select_pressed())
        {
            state.hp = state.hp > 3 ? state.hp - 3 : state.hp_max;
            show_damage_arrow(_hit_side);
            _hit_side = -_hit_side;
        }
    }

    // Tape compass: the 4 letters are made once (8x8 sprites); a turn only moves them and swaps the gold palette
    // (measured: regenerating the 3 letters on every turn cost ~400 ticks, 0.09 f).
    void hud::_place_compass()
    {
        int offset = compass_slide[_compass_anim] * _compass_from;

        for(int letter = 0; letter < 4; ++letter)
        {
            int slot = (letter - state.dir + 1) & 3;        // 0 left, 1 centre (facing), 2 right, 3 behind
            bn::sprite_ptr& s = _compass_sprites[letter];

            if(slot == 3)
            {
                s.set_visible(false);
                continue;
            }

            s.set_x(tx(compass_x[slot]) + _compass_dx[letter] + offset);
            s.set_palette(slot == 1 ? _gold_palette : _grey_palette);
            s.set_visible(_visible);
        }
    }

    void hud::_update_compass()
    {
        if(_compass_sprites.empty())
        {
            _text.set_center_alignment();
            _text.set_one_sprite_per_character(true);
            _text.set_palette_item(grey_text);

            for(int letter = 0; letter < 4; ++letter)
            {
                _text.generate(tx(compass_x[0]), ty(compass_y), dir_names[letter], _compass_sprites);
                _compass_dx[letter] = (_compass_sprites.back().x() - tx(compass_x[0])).right_shift_integer();
            }

            _place_compass();
        }
        else if(_shown.dir != state.dir)
        {
            int turn = (state.dir - _shown.dir) & 3;
            _compass_from = turn == 1 ? 1 : turn == 3 ? -1 : 0;
            _compass_anim = _compass_from ? int(sizeof(compass_slide) / sizeof(compass_slide[0])) - 1 : 0;
            _place_compass();
        }

        _shown.dir = state.dir;
    }

    void hud::_rebuild_text()
    {
        _text.set_one_sprite_per_character(false);

        if(_floor_sprites.empty() || _shown.floor != state.floor)
        {
            bn::string<12> f("FLOOR ");
            f.append(bn::to_string<4>(state.floor + 1));
            _floor_sprites.clear();
            _text.set_palette_item(white_text);
            _text.set_center_alignment();
            _text.generate(tx(208), ty(17), f, _floor_sprites);
        }

        if(_hp_label_sprites.empty())
        {
            _text.set_palette_item(red_text);
            _text.set_left_alignment();
            _text.generate(tx(46), ty(127), "HP", _hp_label_sprites);
        }

        if(_hp_sprites.empty() || _shown.hp != state.hp || _shown.hp_max != state.hp_max)
        {
            bn::string<12> h = bn::to_string<4>(state.hp);
            h.push_back('/');
            h.append(bn::to_string<4>(state.hp_max));
            _hp_sprites.clear();
            _text.set_palette_item(white_text);
            _text.set_left_alignment();
            _text.generate(tx(160), ty(127), h, _hp_sprites);
        }

        if(_shown.keys != state.keys || (state.keys && _key_sprites.empty()))
        {
            _key_sprites.clear();

            if(state.keys > 0)
            {
                bn::string<8> k("x");
                k.append(bn::to_string<4>(state.keys));
                _text.set_palette_item(white_text);
                _text.set_left_alignment();
                _text.generate(tx(198), ty(101), k, _key_sprites);
            }
        }

        if(_potion_sprites.empty() || _shown.potions != state.potions)
        {
            bn::string<8> p("x");
            p.append(bn::to_string<4>(state.potions));
            _potion_sprites.clear();
            _text.set_palette_item(white_text);
            _text.set_left_alignment();
            _text.generate(tx(218), ty(127), p, _potion_sprites);
            _text.set_palette_item(gold_text);
            _text.set_one_sprite_per_character(true);
            _text.generate(tx(220), ty(135), "B", _potion_sprites);
            _text.set_one_sprite_per_character(false);
        }

        _shown = state;
    }

    // Copies a rect of the hidden page (just painted) to the shown page. Mode 4 pages are 0x06000000 and
    // 0x0600A000; Butano's painter only exposes the hidden one (Huecos in the G-32 report).
    void hud::_sync(bn::palette_bitmap_bg_painter& painter, int x0, int y0, int x1, int y1)
    {
        uint16_t* hidden = painter.page().data();
        auto shown = reinterpret_cast<uint16_t*>(reinterpret_cast<uintptr_t>(hidden) ^ 0xA000);
        int first = x0 >> 1;
        int pairs = (x1 >> 1) - first + 1;

        for(int y = y0; y <= y1; ++y)
        {
            // halfword copies: VRAM ignores byte writes the way memcpy may issue them
            const uint16_t* src = hidden + y * 120 + first;
            uint16_t* dst = shown + y * 120 + first;

            for(int i = 0; i < pairs; ++i)
            {
                dst[i] = src[i];
            }
        }
    }

    void hud::_paint_bars(bn::palette_bitmap_bg_painter& painter)
    {
        // HP: red bar, pale blink under 25 %
        int hp = bn::clamp(state.hp, 0, state.hp_max);
        int hp_w = state.hp_max > 0 ? ((hp_x1 - hp_x0 + 1) * hp + state.hp_max - 1) / state.hp_max : 0;
        int blink = hp * 4 < state.hp_max && (_frame / low_hp_blink) % 2;

        if(hp_w != _drawn_hp_w || blink != _drawn_hp_blink)
        {
            int xe = hp_x0 + hp_w - 1;

            if(hp_w > 0)
            {
                painter.rectangle(hp_x0, hp_y0, xe, hp_y0, blink ? red_pale : red_hi);
                painter.rectangle(hp_x0, hp_y0 + 1, xe, hp_y1 - 1, blink ? red_hi : red);
                painter.rectangle(hp_x0, hp_y1, xe, hp_y1, blink ? red : red_dark);
            }

            if(hp_w < hp_x1 - hp_x0 + 1)
            {
                painter.rectangle(xe + 1, hp_y0, hp_x1, hp_y1, red_empty);
            }

            _sync(painter, hp_x0, hp_y0, hp_x1, hp_y1);
            _drawn_hp_w = hp_w;
            _drawn_hp_blink = blink;
        }

        // reload: gold bar under the weapon; the weapon frame glows when it fills up
        int reload = bn::clamp(state.reload, 0, state.reload_max);
        int full_w = reload_x1 - reload_x0 + 1;
        int reload_w = state.reload_max > 0 ? full_w * reload / state.reload_max : full_w;

        if(reload_w != _drawn_reload_w)
        {
            if(reload_w == full_w && _drawn_reload_w >= 0 && _drawn_reload_w < full_w)
            {
                _glow_timer = glow_frames;
            }

            int xe = reload_x0 + reload_w - 1;

            if(reload_w > 0)
            {
                painter.rectangle(reload_x0, reload_y0, xe, reload_y0, gold_glint);
                painter.rectangle(reload_x0, reload_y0 + 1, xe, reload_y0 + 1, brass_hi);
                painter.rectangle(reload_x0, reload_y1, xe, reload_y1, brass);
            }

            if(reload_w < full_w)
            {
                painter.rectangle(xe + 1, reload_y0, reload_x1, reload_y1, gold_empty);
            }

            _sync(painter, reload_x0, reload_y0, reload_x1, reload_y1);
            _drawn_reload_w = reload_w;
        }

        bool glow = _glow_timer > 0;

        if(glow != _glow_drawn)
        {
            const bn::palette_bitmap_pixels_item& f = frame_pixels();
            int w = weapon_x1 - weapon_x0 + 1;
            int h = weapon_y1 - weapon_y0 + 1;

            if(glow)
            {
                painter.rectangle(weapon_x0, weapon_y0, weapon_x1, weapon_y0, brass_hi);
                painter.rectangle(weapon_x0, weapon_y1, weapon_x1, weapon_y1, brass_dark);
                painter.rectangle(weapon_x0, weapon_y0, weapon_x0, weapon_y1, brass_hi);
                painter.rectangle(weapon_x1, weapon_y0, weapon_x1, weapon_y1, brass_dark);
                painter.rectangle(weapon_x0 + 1, weapon_y0 + 1, weapon_x1 - 1, weapon_y0 + 1, brass);
                painter.rectangle(weapon_x0 + 1, weapon_y0 + 1, weapon_x0 + 1, weapon_y1 - 1, brass);
            }
            else
            {
                painter.blit(weapon_x0, weapon_y0, bn::palette_bitmap_roi(f, weapon_x0, weapon_y0, w, 2));
                painter.blit(weapon_x0, weapon_y1, bn::palette_bitmap_roi(f, weapon_x0, weapon_y1, w, 1));
                painter.blit(weapon_x0, weapon_y0, bn::palette_bitmap_roi(f, weapon_x0, weapon_y0, 2, h));
                painter.blit(weapon_x1, weapon_y0, bn::palette_bitmap_roi(f, weapon_x1, weapon_y0, 1, h));
            }

            _sync(painter, weapon_x0, weapon_y0, weapon_x1, weapon_y0 + 1);
            _sync(painter, weapon_x0, weapon_y1, weapon_x1, weapon_y1);
            _sync(painter, weapon_x0, weapon_y0, weapon_x0 + 1, weapon_y1);
            _sync(painter, weapon_x1, weapon_y0, weapon_x1, weapon_y1);
            _glow_drawn = glow;
        }

        // key icon: only with at least one key (HUD.md 2)
        int has_keys = state.keys > 0;

        if(has_keys != _drawn_keys)
        {
            if(has_keys)
            {
                painter.blit(key_x0, key_y0, bn::palette_bitmap_roi(frame_pixels(), key_x0, key_y0,
                                                                   key_x1 - key_x0 + 1, key_y1 - key_y0 + 1));
            }
            else
            {
                painter.rectangle(key_x0, key_y0, key_x1, key_y1, plate);
            }

            _sync(painter, key_x0, key_y0, key_x1, key_y1);
            _drawn_keys = has_keys;
        }
    }

    void hud::update(bn::palette_bitmap_bg_painter& painter, bool heavy_frame)
    {
        bn::timer timer;
        ++_frame;

        _update_compass();

        // text generation is the expensive part: wait for a light frame (at most max_text_delay frames)
        bool text_changed = _has_pending_message || _floor_sprites.empty() || _shown.floor != state.floor ||
                _shown.hp != state.hp || _shown.hp_max != state.hp_max || _shown.keys != state.keys ||
                _shown.potions != state.potions;

        if(text_changed && heavy_frame && _text_delay < max_text_delay)
        {
            ++_text_delay;
        }
        else if(text_changed)
        {
            _text_delay = 0;
            _rebuild_text();

            if(_has_pending_message)
            {
                _flush_message();
            }
        }

        if(! _visible)
        {
            set_visible(false);         // sprites made while hidden
        }

        if(_compass_anim > 0)
        {
            --_compass_anim;
            _place_compass();
        }

        if(_message_timer > 0 && --_message_timer == 0)
        {
            _message_sprites.clear();
        }

        // damage arrows: blink 4 on / 4 off while the warning lasts
        for(int side = 0; side < 2; ++side)
        {
            if(_arrow_timer[side] > 0)
            {
                --_arrow_timer[side];
            }
        }

        for(int side = 0; side < _arrow_sprites.size(); ++side)
        {
            _arrow_sprites[side].set_visible(_visible && _arrow_timer[side] > 0 && (_arrow_timer[side] / 4) % 2 == 0);
        }

        if(_glow_timer > 0)
        {
            --_glow_timer;
        }

        _paint_bars(painter);

        _last_ticks = timer.elapsed_ticks();

        if(_frame > 2)                  // frames 1-2 create every sprite; not the per-frame cost
        {
            _max_ticks = _last_ticks > _max_ticks ? _last_ticks : _max_ticks;
        }
    }

    int hud::sprites_count() const
    {
        return _compass_sprites.size() + _floor_sprites.size() + _hp_label_sprites.size() + _hp_sprites.size() +
               _key_sprites.size() + _potion_sprites.size() + _message_sprites.size() + _arrow_sprites.size();
    }

    int hud::sprite_tiles_count() const
    {
        int tiles = 0;

        auto add = [&tiles](const auto& sprites)
        {
            for(const bn::sprite_ptr& s : sprites)
            {
                tiles += s.tiles().tiles_count();
            }
        };
        add(_compass_sprites);
        add(_floor_sprites);
        add(_hp_label_sprites);
        add(_hp_sprites);
        add(_key_sprites);
        add(_potion_sprites);
        add(_message_sprites);
        add(_arrow_sprites);
        return tiles;
    }
}
