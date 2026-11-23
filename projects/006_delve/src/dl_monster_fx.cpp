#include "dl_monster_fx.h"

#include "bn_color.h"
#include "bn_keypad.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_math.h"
#include "bn_span.h"
#include "bn_sprite_item.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"

#include "dl_fx.h"

namespace dl
{
    namespace
    {
        constexpr int appear_frames = 16;
        constexpr int tele_frames = 32;        // two 16-frame pulses
        constexpr int release_frames = 20;     // grace before a slot unseen is freed (turns hide sprites 6 f)
        constexpr int breath_step = 683;       // 65536 / 96: one breath every 96 frames

        // x 256 (shifts, not divisions: the GBA has no divide instruction)
        constexpr int fade_gentle_q8[5] = { 256, 256, 218, 166, 128 };   // 100 / 100 / 85 / 65 / 50 % by band
        constexpr int fade_walls_q8[5] = { 256, 154, 92, 92, 92 };       // 100 / 60 / 36 %: the wall decorations

        bool accent_index(int i)
        {
            return i >= 13;     // 13 halo, 14 colour, 15 core: the full-bright accents of the monster art
        }
    }

    void monster_fx::begin_frame(bool heavy)
    {
        ++_frame;
        _heavy = heavy;

        for(slot& s : _slots)
        {
            s.touched = false;
        }

        int halo_mode = dl::fx::enabled.monster_halo;

        if(halo_mode && halo_mode != _halo_palette_mode)
        {
            // silhouette: every opaque index one colour (dark outline, or the light the additive backlight adds)
            static bn::color colors[16];
            bn::color c = halo_mode == 1 ? bn::color(1, 2, 1) : bn::color(7, 9, 7);

            for(int i = 0; i < 16; ++i)
            {
                colors[i] = c;
            }

            bn::span<const bn::color> span(colors, 16);

            if(_halo_palette)
            {
                _halo_palette->set_colors(span);
            }
            else
            {
                _halo_palette = bn::sprite_palette_ptr::create_new_optional(
                        bn::sprite_palette_item(span, bn::bpp_mode::BPP_4));
            }

            _halo_palette_mode = halo_mode;
        }
    }

    void monster_fx::apply(int ent, const bn::sprite_item& item, bn::sprite_ptr& sprite, int band)
    {
        slot* found = nullptr;
        slot* free_slot = nullptr;

        for(slot& s : _slots)
        {
            if(s.ent == ent)
            {
                found = &s;
                break;
            }

            if(s.ent < 0 && ! free_slot)
            {
                free_slot = &s;
            }
        }

        if(! found)
        {
            // keep 2 sprite palettes free for whatever the game creates next (decos, HUD): Butano asserts when
            // none is left, this only drops the effect
            if(! free_slot || ! sprite.visible() || _heavy || bn::sprite_palettes::available_colors_count() < 48)
            {
                return;         // no slot: the monster keeps the plain palette of its art
            }

            free_slot->palette = bn::sprite_palette_ptr::create_new_optional(item.palette_item());

            if(! free_slot->palette)
            {
                return;
            }

            found = free_slot;
            found->ent = ent;
            found->key = -1;
            found->fade_q = -1;
            found->tele = 0;
            found->breathing = false;
            found->flash = dl::fx::enabled.monster_appear_flash ? appear_frames : 0;
        }

        slot& s = *found;
        s.touched = true;
        s.idle = 0;

        if(sprite.palette() != *s.palette)
        {
            sprite.set_palette(*s.palette);
        }


        if(! _heavy)
        {
            _update_palette(s, item, band);     // a band change waits one frame (~200 ticks with the divisions)
        }

        if(sprite.visible())
        {
            if(s.flash > 0)
            {
                --s.flash;
            }

            if(s.tele > 0)
            {
                --s.tele;
            }
        }

        if(! _heavy)
        {
            _update_fade(s);
            _update_breath(s, sprite, band);
        }

        _update_halo(s, item, sprite);
    }

    void monster_fx::end_frame()
    {
        for(slot& s : _slots)
        {
            if(s.ent >= 0 && ! s.touched)
            {
                if(s.halo)
                {
                    s.halo->set_visible(false);
                }

                if(++s.idle > release_frames)
                {
                    _release(s);
                }
            }
        }
    }

    void monster_fx::telegraph(int ent)
    {
        for(slot& s : _slots)
        {
            if(s.ent == ent && dl::fx::enabled.monster_attack_flash)
            {
                s.tele = tele_frames;
            }
        }
    }

    void monster_fx::telegraph_all()
    {
        for(slot& s : _slots)
        {
            if(s.ent >= 0)
            {
                telegraph(s.ent);
            }
        }
    }

    void monster_fx::clear()
    {
        for(slot& s : _slots)
        {
            _release(s);
        }
    }

    void monster_fx::refresh()
    {
        for(slot& s : _slots)
        {
            s.key = -1;
            s.fade_q = -1;
        }

        _halo_palette_mode = -1;
    }

    int monster_fx::slots_used() const
    {
        int n = 0;

        for(const slot& s : _slots)
        {
            n += s.ent >= 0;
        }

        return n;
    }

    void monster_fx::_release(slot& s)
    {
        s.ent = -1;
        s.idle = 0;
        s.palette.reset();
        s.halo.reset();
    }

    void monster_fx::_update_palette(slot& s, const bn::sprite_item& item, int band)
    {
        const dl::fx::switches& fx = dl::fx::enabled;
        int b = bn::min(bn::max(band, 0), 4);
        bool tint = fx.monster_torch_tint && b <= 1;
        int key = b | (fx.monster_fade << 3) | (tint << 5) | (fx.monster_accents << 6);

        if(key == s.key)
        {
            return;
        }

        s.key = key;
        bn::span<const bn::color> src = item.palette_item().colors_ref();
        bn::color colors[16];
        int f_band = fx.monster_fade == 1 ? fade_gentle_q8[b] : fx.monster_fade == 2 ? fade_walls_q8[b] : 256;

        for(int i = 0; i < 16; ++i)
        {
            const bn::color& c = src[i];
            bool accent = fx.monster_accents && accent_index(i);

            if(i == 0 || accent)
            {
                colors[i] = c;
                continue;
            }

            int r = (c.red() * f_band) >> 8;
            int g = (c.green() * f_band) >> 8;
            int bl = (c.blue() * f_band) >> 8;

            if(tint)
            {
                // warm lift of the carried torch: more red, a little green, less blue
                r = bn::min(31, ((r * 294) >> 8) + 3);     // x 1.15 + 3
                g = bn::min(31, ((g * 269) >> 8) + 1);     // x 1.05 + 1
                bl = (bl * 230) >> 8;                       // x 0.9
            }

            colors[i] = bn::color(r, g, bl);
        }

        s.palette->set_colors(bn::span<const bn::color>(colors, 16));
    }

    void monster_fx::_update_fade(slot& s)
    {
        // q: 0 none, 1..32 white flash level, 33..64 red telegraph level
        int q = 0;

        if(s.tele > 0)
        {
            int t = (tele_frames - s.tele) & 15;
            int tri = t < 8 ? t : 16 - t;                       // 0 .. 8 .. 0
            q = 32 + bn::max(1, tri * 22 / 8);                  // up to 0.69
        }
        else if(s.flash > 0)
        {
            q = s.flash * 21 / appear_frames;                   // 0.66 -> 0
        }

        if(q == s.fade_q)
        {
            return;
        }

        s.fade_q = q;

        if(q == 0)
        {
            s.palette->set_fade_intensity(0);
        }
        else if(q > 32)
        {
            s.palette->set_fade(bn::color(31, 4, 2), bn::fixed(q - 32) / 32);
        }
        else
        {
            s.palette->set_fade(bn::color(31, 31, 28), bn::fixed(q) / 32);
        }
    }

    void monster_fx::_update_breath(slot& s, bn::sprite_ptr& sprite, int band)
    {
        if(! sprite.affine_mat())
        {
            return;
        }

        bn::fixed y = sprite.y();
        bn::fixed vs = sprite.vertical_scale();

        // the game moved or rescaled the sprite since the last breath: that is the new resting pose
        if(! s.breathing || y != s.set_y || vs != s.set_vs)
        {
            s.base_y = y;
            s.base_vs = vs;
        }

        if(! dl::fx::enabled.monster_breathe || band < 1)
        {
            if(s.breathing && y == s.set_y && vs == s.set_vs)
            {
                sprite.set_y(s.base_y);
                sprite.set_vertical_scale(s.base_vs);
            }

            s.breathing = false;
            return;
        }

        unsigned angle = unsigned(_frame * breath_step + s.ent * 20011) & 0xFFFF;
        bn::fixed a = bn::sin(bn::fixed_t<16>::from_data(int(angle))) * bn::fixed(0.03);
        bn::fixed nvs = bn::min(s.base_vs * (1 + a), bn::fixed(2));
        bn::fixed ny = s.base_y - (nvs - s.base_vs) * 32;      // half the 64 px art: the feet stay put
        sprite.set_vertical_scale(nvs);
        sprite.set_y(ny);
        s.set_y = sprite.y();
        s.set_vs = sprite.vertical_scale();
        s.breathing = true;
    }

    void monster_fx::_update_halo(slot& s, const bn::sprite_item& item, const bn::sprite_ptr& sprite)
    {
        int mode = dl::fx::enabled.monster_halo;

        if(! mode || ! _halo_palette)
        {
            s.halo.reset();
            return;
        }

        if(! s.halo)
        {
            if(_heavy)
            {
                return;
            }

            s.halo = item.create_sprite_optional(0, 0);

            if(! s.halo)
            {
                return;
            }

            s.halo->set_palette(*_halo_palette);
            s.halo->set_bg_priority(0);
        }

        bn::sprite_ptr& h = *s.halo;

        if(! sprite.visible())
        {
            h.set_visible(false);
            return;
        }

        // 10 % larger silhouette with the same feet line (a centred one would show a slab under the feet)
        bn::fixed k = 1.1;
        bn::fixed vs = sprite.vertical_scale();
        bn::fixed hvs = bn::min(vs * k, bn::fixed(2));
        bn::fixed hhs = bn::min(sprite.horizontal_scale() * k, bn::fixed(2));
        h.set_position(sprite.x(), sprite.y() - (hvs - vs) * 32);

        if(_heavy)
        {
            return;     // scale and z order follow next frame (measured: up to ~165 ticks on a redraw frame)
        }

        if(h.horizontal_scale() != hhs || h.vertical_scale() != hvs)
        {
            h.set_scale(hhs, hvs);
        }

        if(h.z_order() != sprite.z_order() + 1)
        {
            h.set_z_order(sprite.z_order() + 1);
        }

        if(h.blending_enabled() != (mode == 2))
        {
            h.set_blending_enabled(mode == 2);
        }

        if(! h.visible())
        {
            h.set_visible(true);
        }
    }

    // ---- hidden FX menu

    namespace
    {
        constexpr int menu_items = 8;

        const char* const item_names[menu_items] = {
            "FADE", "TORCH TINT", "BRIGHT EYES", "APPEAR FLASH", "ATTACK FLASH", "HALO", "BREATHE", "TEST ATTACK"
        };

        // measured costs (G-34 README); PAL = one sprite palette per monster (shared by every palette effect)
        const char* const item_costs[menu_items] = {
            "1 PALETTE/MONSTER", "1 PALETTE/MONSTER", "1 PALETTE/MONSTER", "PALETTE FADE", "PALETTE FADE",
            "+1 SPRITE +1 MATRIX", "0.1 % CPU", "A: PULSE NOW"
        };

        const char* value_text(int item)
        {
            const dl::fx::switches& fx = dl::fx::enabled;

            switch(item)
            {
            case 0:
                return fx.monster_fade == 0 ? "NONE" : fx.monster_fade == 1 ? "GENTLE" : "WALLS";
            case 1:
                return fx.monster_torch_tint ? "ON" : "OFF";
            case 2:
                return fx.monster_accents ? "ON" : "OFF";
            case 3:
                return fx.monster_appear_flash ? "ON" : "OFF";
            case 4:
                return fx.monster_attack_flash ? "ON" : "OFF";
            case 5:
                return fx.monster_halo == 0 ? "OFF" : fx.monster_halo == 1 ? "DARK" : "GLOW";
            case 6:
                return fx.monster_breathe ? "ON" : "OFF";
            default:
                return "";
            }
        }

        void change(int item, int delta)
        {
            dl::fx::switches& fx = dl::fx::enabled;

            switch(item)
            {
            case 0:
                fx.monster_fade = (fx.monster_fade + 3 + delta) % 3;
                break;
            case 1:
                fx.monster_torch_tint = ! fx.monster_torch_tint;
                break;
            case 2:
                fx.monster_accents = ! fx.monster_accents;
                break;
            case 3:
                fx.monster_appear_flash = ! fx.monster_appear_flash;
                break;
            case 4:
                fx.monster_attack_flash = ! fx.monster_attack_flash;
                break;
            case 5:
                fx.monster_halo = (fx.monster_halo + 3 + delta) % 3;
                break;
            case 6:
                fx.monster_breathe = ! fx.monster_breathe;
                break;
            default:
                break;
            }
        }
    }

    bool fx_menu::update(bn::sprite_text_generator& text, monster_fx& mfx)
    {
        bool combo = bn::keypad::select_held() && bn::keypad::l_pressed();

        if(! _open)
        {
            if(! combo)
            {
                return false;
            }

            _open = true;
            _draw(text);
            return true;
        }

        if(combo || bn::keypad::b_pressed())
        {
            _open = false;
            _sprites.clear();
            return true;
        }

        bool redraw = false;

        if(bn::keypad::up_pressed())
        {
            _cursor = (_cursor + menu_items - 1) % menu_items;
            redraw = true;
        }
        else if(bn::keypad::down_pressed())
        {
            _cursor = (_cursor + 1) % menu_items;
            redraw = true;
        }
        else if(bn::keypad::left_pressed() || bn::keypad::right_pressed() || bn::keypad::a_pressed())
        {
            if(_cursor == menu_items - 1)
            {
                if(bn::keypad::a_pressed())
                {
                    // close so the pulse can be seen
                    mfx.telegraph_all();
                    _open = false;
                    _sprites.clear();
                    return true;
                }
            }
            else
            {
                change(_cursor, bn::keypad::left_pressed() ? -1 : 1);
                mfx.refresh();
                redraw = true;
            }
        }

        if(redraw)
        {
            _draw(text);
        }

        return true;
    }

    void fx_menu::_draw(bn::sprite_text_generator& text)
    {
        // one string per line and the cost of the selected item only: few sprite tiles (VRAM is tight in mode 4
        // with monsters, halos and decos on screen); generate_optional drops lines instead of asserting
        _sprites.clear();
        text.set_left_alignment();
        text.set_bg_priority(0);
        text.set_z_order(-8);

        int y = -72;
        (void) text.generate_optional(-116, y, "MONSTER FX  B: CLOSE", _sprites);

        for(int i = 0; i < menu_items; ++i)
        {
            y += 10;
            bn::string<32> line;
            line.append(i == _cursor ? "> " : "  ");
            line.append(item_names[i]);
            line.append(": ");
            line.append(value_text(i));
            (void) text.generate_optional(-116, y, line, _sprites);
        }

        bn::string<32> cost("COST: ");
        cost.append(item_costs[_cursor]);
        (void) text.generate_optional(-116, y + 12, cost, _sprites);
    }
}
