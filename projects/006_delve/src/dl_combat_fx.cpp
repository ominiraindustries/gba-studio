// DELVE combat feedback (G-35). See dl_combat_fx.h.

#include "dl_combat_fx.h"

#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_sprite_text_generator.h"
#include "bn_timer.h"

#include "bn_sprite_items_fx_edge_h.h"
#include "bn_sprite_items_fx_edge_v.h"

namespace dl
{
    namespace
    {
        constexpr int popup_frames = 30;
        constexpr int flash_frames = 20;

        // view rectangle (0, 0, 176, 120) in sprite coordinates: x - 120, y - 80
        // 32x8 / 8x32 pieces: 6 across the top and bottom (176 px), 4 down each side (120 px), overlapping at the end
        constexpr int edge_h_x[6] = { 16 - 120, 48 - 120, 80 - 120, 112 - 120, 144 - 120, 160 - 120 };
        constexpr int edge_v_y[4] = { 16 - 80, 48 - 80, 80 - 80, 104 - 80 };
    }

    combat_fx::combat_fx(bn::sprite_text_generator& text) :
        _text(text)
    {
    }

    void combat_fx::number(const char* text, bn::fixed x, bn::fixed y)
    {
        if(_pending.full())
        {
            _pending.erase(_pending.begin());
        }

        pending p;
        p.text = text;
        p.x = x;
        p.y = y;
        _pending.push_back(p);
    }

    void combat_fx::red_edge(int frames)
    {
        if(_edge.empty())
        {
            for(int i = 0; i < 6; ++i)
            {
                for(int bottom = 0; bottom < 2; ++bottom)
                {
                    bn::sprite_ptr s = bn::sprite_items::fx_edge_h.create_sprite(edge_h_x[i], bottom ? 116 - 80 : 4 - 80);
                    s.set_vertical_flip(bottom);
                    s.set_bg_priority(0);
                    s.set_z_order(-50);
                    _edge.push_back(bn::move(s));
                }
            }

            for(int i = 0; i < 4; ++i)
            {
                for(int right = 0; right < 2; ++right)
                {
                    bn::sprite_ptr s = bn::sprite_items::fx_edge_v.create_sprite(right ? 172 - 120 : 4 - 120, edge_v_y[i]);
                    s.set_horizontal_flip(right);
                    s.set_bg_priority(0);
                    s.set_z_order(-50);
                    _edge.push_back(bn::move(s));
                }
            }
        }

        _edge_t = frames;
    }

    void combat_fx::gold_flash()
    {
        _flash_t = flash_frames;
    }

    void combat_fx::clear()
    {
        _popups.clear();
        _pending.clear();
        _edge.clear();
        _edge_t = 0;

        if(_flash_t)
        {
            _flash_t = 0;
            bn::bg_palettes::set_fade_intensity(0);
        }
    }

    void combat_fx::update(bool heavy)
    {
        bn::timer timer;

        for(int i = 0, n = _popups.size(); i < n; )
        {
            popup& p = _popups[i];

            if(++p.t >= popup_frames)
            {
                _popups.erase(_popups.begin() + i);
                --n;
                continue;
            }

            if(p.t & 1)
            {
                for(bn::sprite_ptr& s : p.sprites)
                {
                    s.set_y(s.y() - 1);
                }
            }

            ++i;
        }

        if(! heavy && ! _pending.empty())
        {
            if(_popups.full())
            {
                _popups.erase(_popups.begin());
            }

            pending& q = _pending.front();
            popup p;
            _text.set_center_alignment();
            _text.set_bg_priority(0);
            _text.set_z_order(-60);
            _text.generate(q.x, q.y, q.text, p.sprites);
            _text.set_bg_priority(3);
            _text.set_z_order(0);
            _popups.push_back(bn::move(p));
            _pending.erase(_pending.begin());
        }

        if(_edge_t > 0 && --_edge_t == 0)
        {
            _edge.clear();
        }

        if(_flash_t > 0)
        {
            --_flash_t;
            bn::bg_palettes::set_fade(bn::color(31, 26, 6), bn::fixed(_flash_t) / (flash_frames * 3 / 2));
        }

        _last_ticks = timer.elapsed_ticks();
        _max_ticks = _last_ticks > _max_ticks ? _last_ticks : _max_ticks;
    }
}
