// DELVE G-40 M1: START pause, BAG and SPELLS tabs. See dl_pause.h.

#include "dl_pause.h"

#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_sprite_palette_item.h"
#include "bn_string.h"

#include "bn_palette_bitmap_items_pause_bag.h"
#include "bn_palette_bitmap_items_pause_spells.h"
#include "bn_sprite_items_icon_equip.h"
#include "bn_sprite_items_icon_items.h"
#include "bn_sprite_items_icon_jewels.h"
#include "bn_sprite_items_icon_spells.h"
#include "bn_sprite_items_icon_status.h"
#include "bn_sprite_items_ui_cursor.h"

#include "dl_combat.h"
#include "dl_hud.h"

namespace dl
{
    namespace
    {
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

        // same colours as dl_hud.cpp: Butano shares the palettes
        DL_TEXT_PALETTE(white_colors, 29, 29, 27);
        DL_TEXT_PALETTE(gold_colors, 27, 22, 12);
        DL_TEXT_PALETTE(grey_colors, 17, 18, 19);
        DL_TEXT_PALETTE(blue_colors, 14, 21, 31);

#undef DL_TEXT_PALETTE

        constexpr bn::sprite_palette_item white_text(bn::span<const bn::color>(white_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item gold_text(bn::span<const bn::color>(gold_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item grey_text(bn::span<const bn::color>(grey_colors, 16), bn::bpp_mode::BPP_4);
        constexpr bn::sprite_palette_item blue_text(bn::span<const bn::color>(blue_colors, 16), bn::bpp_mode::BPP_4);

        constexpr int bag_x = 24, bag_y = 16, spell_x = 8, spell_y = 18, slot = 32;
        constexpr int equip_x[4] = { 180, 209, 180, 209 };
        constexpr int equip_y[4] = { 4, 4, 33, 33 };
        constexpr int spells_total = 10;    // the 5x2 grid shows every spell of the design (M1 knows 3)

        constexpr int sx(int x)
        {
            return x - 120;
        }

        constexpr int sy(int y)
        {
            return y - 80;
        }
    }

    void pause_screen::_blit(bn::palette_bitmap_bg_painter& painter)
    {
        const bn::palette_bitmap_item& item = _tab == 0 ? bn::palette_bitmap_items::pause_bag :
                                                          bn::palette_bitmap_items::pause_spells;

        for(int k = 0; k < 2; ++k)
        {
            painter.blit(0, 0, item.pixels_item());
            painter.flip_page_now();
        }
    }

    void pause_screen::open(bn::palette_bitmap_bg_painter& painter, bn::sprite_text_generator& text, combat& cb,
                            hud& hd)
    {
        _open = true;
        _tab = 0;
        _cursor = 0;
        _blit(painter);
        hd.invalidate();
        hd.set_panel_visible(false);
        _draw(text, cb);
        _describe(cb, hd);
    }

    void pause_screen::_build_bag(combat& cb)
    {
        const player_stats& p = cb.player();
        const magic_state& m = cb.magic();
        _bag.clear();

        if(p.potions > 0)
        {
            _bag.push_back(bag_entry{ 0, 0, int8_t(p.potions) });
        }

        if(p.keys > 0)
        {
            _bag.push_back(bag_entry{ 1, 0, int8_t(p.keys) });
        }

        for(int s = 0; s < SPELL_COUNT; ++s)
        {
            if(m.scrolls[s] > 0)
            {
                _bag.push_back(bag_entry{ 2, int8_t(s), int8_t(m.scrolls[s]) });
            }
        }

        if(m.ring == 1)
        {
            _bag.push_back(bag_entry{ 3, 0, 1 });
        }
    }

    void pause_screen::_draw(bn::sprite_text_generator& text, combat& cb)
    {
        const player_stats& p = cb.player();
        const magic_state& m = cb.magic();
        _sprites.clear();
        _build_bag(cb);

        auto icon = [this](const bn::sprite_item& item, int frame, int cx, int cy)
        {
            if(_sprites.full())
            {
                return;
            }

            if(bn::optional<bn::sprite_ptr> s = item.create_sprite_optional(sx(cx), sy(cy), frame))
            {
                s->set_bg_priority(0);
                _sprites.push_back(bn::move(*s));
            }
        };
        auto line = [this, &text](const bn::sprite_palette_item& pal, int x, int y, const bn::string_view& str)
        {
            text.set_palette_item(pal);
            (void) text.generate_optional(sx(x), sy(y) + 4, str, _sprites);
        };

        text.set_left_alignment();
        text.set_one_sprite_per_character(false);
        text.set_bg_priority(0);
        text.set_z_order(0);

        // tabs
        line(_tab == 0 ? gold_text : grey_text, 6, 2, "BAG");
        line(_tab == 1 ? gold_text : grey_text, 44, 2, "SPELLS");
        line(grey_text, 100, 2, "MAP");

        // equipment 2x2 (both tabs): weapon, armour (none in M1), ring, amulet (none in M1)
        icon(bn::sprite_items::icon_equip, p.weapon ? 1 : 0, equip_x[0] + 14, equip_y[0] + 14);
        icon(bn::sprite_items::icon_equip, 9, equip_x[1] + 14, equip_y[1] + 14);

        if(m.ring == 2)
        {
            icon(bn::sprite_items::icon_jewels, 0, equip_x[2] + 14, equip_y[2] + 14);
        }
        else
        {
            icon(bn::sprite_items::icon_equip, 10, equip_x[2] + 14, equip_y[2] + 14);
        }

        icon(bn::sprite_items::icon_equip, 11, equip_x[3] + 14, equip_y[3] + 14);

        // status lines
        bn::string<12> s("LV ");
        s.append(bn::to_string<4>(p.level));
        line(white_text, 184, 66, s);
        s = "STR ";
        s.append(bn::to_string<4>(p.str));
        line(white_text, 184, 76, s);
        s = "DEF ";
        s.append(bn::to_string<4>(p.def));
        line(white_text, 184, 86, s);
        s = "XP ";
        s.append(bn::to_string<4>(p.xp));
        line(white_text, 184, 96, s);

        if(_tab == 0)
        {
            for(int i = 0; i < _bag.size(); ++i)
            {
                const bag_entry& e = _bag[i];
                int x = bag_x + (i & 3) * slot;
                int y = bag_y + (i >> 2) * slot;

                switch(e.kind)
                {
                case 0:
                    icon(bn::sprite_items::icon_items, 1, x + 16, y + 16);
                    break;
                case 1:
                    icon(bn::sprite_items::icon_items, 4, x + 16, y + 16);
                    break;
                case 2:
                    icon(bn::sprite_items::icon_items, 0, x + 16, y + 16);
                    icon(bn::sprite_items::icon_spells, e.index, x + 24, y + 8);    // which spell: small badge
                    break;
                default:
                    icon(bn::sprite_items::icon_jewels, 0, x + 16, y + 16);
                    break;
                }

                bool in_belt = e.kind == 0;

                for(int b = 0; b < belt_size && e.kind == 2; ++b)
                {
                    in_belt |= m.belt[b] == 1 + e.index;
                }

                if(in_belt)
                {
                    icon(bn::sprite_items::icon_status, 4, x + 6, y + 6);
                }

                if(e.count > 1 || e.kind == 0)
                {
                    bn::string<4> c("x");
                    c.append(bn::to_string<4>(e.count));
                    line(white_text, x + 19, y + 23, c);
                }
            }
        }
        else
        {
            for(int i = 0; i < spells_total; ++i)
            {
                int x = spell_x + (i % 5) * slot;
                int y = spell_y + (i / 5) * slot;

                if(i < SPELL_COUNT && (m.known & (1 << i)))
                {
                    icon(bn::sprite_items::icon_spells, i, x + 16, y + 16);
                }
                else
                {
                    line(grey_text, x + 13, y + 12, "?");
                }
            }

            // detail of the spell under the cursor
            if(_cursor < SPELL_COUNT && (m.known & (1 << _cursor)))
            {
                const spell_stats& st = spell_table[_cursor];
                bn::string<28> a("MP ");
                a.append(bn::to_string<4>(st.mp));
                bn::string<28> b;

                switch(_cursor)
                {
                case SPELL_MISSILE:
                    a.append("   RANGE 4");
                    b.append(bn::to_string<4>(6 + p.level - 1));
                    b.append(" DAMAGE");
                    break;
                case SPELL_LIGHT:
                    b = "45 S OF BRIGHT LIGHT";
                    break;
                default:
                    b = "+30% HP";
                    break;
                }

                line(blue_text, 12, 90, a);
                line(white_text, 12, 100, b);
            }
            else
            {
                line(grey_text, 12, 90, "NOT LEARNED YET");
            }
        }

        _place_cursor();
    }

    void pause_screen::_place_cursor()
    {
        int x, y, frame = 0;

        if(_tab == 1)
        {
            x = spell_x + (_cursor % 5) * slot + 16;
            y = spell_y + (_cursor / 5) * slot + 16;
        }
        else if(_cursor < 12)
        {
            x = bag_x + (_cursor & 3) * slot + 16;
            y = bag_y + (_cursor >> 2) * slot + 16;
        }
        else
        {
            x = equip_x[_cursor - 12] - 2 + 16;
            y = equip_y[_cursor - 12] - 2 + 16;
            frame = 1;
        }

        if(_cursor_sprite.empty())
        {
            if(bn::optional<bn::sprite_ptr> s = bn::sprite_items::ui_cursor.create_sprite_optional(sx(x), sy(y), frame))
            {
                s->set_bg_priority(0);
                s->set_z_order(-1);
                _cursor_sprite.push_back(bn::move(*s));
            }
        }
        else
        {
            _cursor_sprite.front().set_position(sx(x), sy(y));
            _cursor_sprite.front().set_tiles(bn::sprite_items::ui_cursor.tiles_item(), frame);
        }
    }

    void pause_screen::_describe(combat& cb, hud& hd)
    {
        const magic_state& m = cb.magic();

        if(_tab == 1)
        {
            if(_cursor < SPELL_COUNT && (m.known & (1 << _cursor)))
            {
                hd.message(spell_table[_cursor].name, 0);
            }
            else
            {
                hd.message("?", 0);
            }

            return;
        }

        if(_cursor >= 12)
        {
            switch(_cursor - 12)
            {
            case 0:
                hd.message(weapon_table[cb.player().weapon].name, 0);
                break;
            case 2:
                hd.message(m.ring == 2 ? "RING OF MANA  +5 MP   A: OFF" : "NO RING", 0);
                break;
            default:
                hd.message("EMPTY", 0);
                break;
            }

            return;
        }

        if(_cursor >= _bag.size())
        {
            hd.message(_bag.empty() ? "START: CLOSE" : "", 0);
            return;
        }

        const bag_entry& e = _bag[_cursor];

        switch(e.kind)
        {
        case 0:
            hd.message("HEALING POTION   A: DRINK", 0);
            break;
        case 1:
            hd.message("IRON KEY", 0);
            break;
        case 2:
            hd.message(combat::item_name(ITEM_SCROLL_MISSILE + e.index), 0);
            break;
        default:
            hd.message("RING OF MANA +5 MP  A: WEAR", 0);
            break;
        }
    }

    bool pause_screen::update(bn::palette_bitmap_bg_painter& painter, bn::sprite_text_generator& text, combat& cb,
                              hud& hd)
    {
        if(bn::keypad::start_pressed() || bn::keypad::b_pressed())
        {
            _open = false;
            _sprites.clear();
            _cursor_sprite.clear();
            hd.clear_message();
            hd.set_panel_visible(true);
            return false;
        }

        bool redraw = false;
        bool moved = false;

        if(bn::keypad::select_pressed())
        {
            _tab ^= 1;
            _cursor = 0;
            _blit(painter);
            hd.invalidate();
            redraw = true;
        }
        else if(_tab == 1)
        {
            int c = _cursor;
            c = bn::keypad::left_pressed() ? (c % 5 ? c - 1 : c + 4) : bn::keypad::right_pressed() ? (c % 5 == 4 ? c - 4 : c + 1) : c;
            c = bn::keypad::up_pressed() || bn::keypad::down_pressed() ? (c + 5) % 10 : c;
            moved = c != _cursor;
            redraw = moved;     // the detail lines follow the cursor
            _cursor = c;
        }
        else
        {
            int c = _cursor;

            if(c < 12)
            {
                if(bn::keypad::left_pressed() && (c & 3))
                {
                    --c;
                }
                else if(bn::keypad::right_pressed())
                {
                    c = (c & 3) == 3 ? 12 + ((c >> 2) >= 1 ? 2 : 0) : c + 1;
                }
                else if(bn::keypad::up_pressed() && c >= 4)
                {
                    c -= 4;
                }
                else if(bn::keypad::down_pressed() && c < 8)
                {
                    c += 4;
                }
            }
            else
            {
                int e = c - 12;

                if(bn::keypad::left_pressed())
                {
                    c = e & 1 ? c - 1 : (e >= 2 ? 7 : 3);
                }
                else if(bn::keypad::right_pressed() && ! (e & 1))
                {
                    ++c;
                }
                else if(bn::keypad::up_pressed() && e >= 2)
                {
                    c -= 2;
                }
                else if(bn::keypad::down_pressed() && e < 2)
                {
                    c += 2;
                }
            }

            moved = c != _cursor;
            _cursor = c;

            if(bn::keypad::a_pressed())
            {
                if(_cursor == 14 && cb.magic().ring == 2)
                {
                    cb.toggle_ring();       // take it off: back to the bag
                    redraw = true;
                }
                else if(_cursor < _bag.size())
                {
                    const bag_entry& e = _bag[_cursor];

                    if(e.kind == 3)
                    {
                        cb.toggle_ring();   // wear it (MAGIA 3.3: +5 MP max, 1 MP / 72 f)
                        hd.message("RING OF MANA: +5 MP", 120);
                        redraw = true;
                    }
                    else if(e.kind == 0)
                    {
                        cb.drink_potion();
                        redraw = true;
                    }
                }
            }
        }

        if(redraw)
        {
            _draw(text, cb);
        }
        else if(moved)
        {
            _place_cursor();
        }

        if(moved || (redraw && ! bn::keypad::a_pressed()))
        {
            _describe(cb, hd);
        }

        return true;
    }
}
