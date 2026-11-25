// HEXBOUND menus outside a run (DISENO_RUTA.md §R0): title, main menu, stage select, shop, records.
// All text with bn::sprite_text_generator and the common Butano fonts.

#include "hb_meta.h"

#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_shop_icons.h"
#include "bn_sprite_items_stage_cards.h"
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_palette_items_wizard_cinda_palette.h"
#include "bn_sprite_palette_items_wizard_morrow_palette.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "common_variable_16x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"

#include "hb_art.h"
#include "hb_config.h"
#include "hb_save.h"

extern volatile int test_mode;
volatile int test_screen;   // 1 title, 2 menu, 3 stage select, 4 shop, 5 records, 6 options, 7 ending, 8 credits
volatile int test_title_star;   // 1: the title shows the ending star
volatile int test_music_id;     // last music item requested (bn::music_items id)


namespace hb
{

namespace
{
    constexpr int stage_count = 3;
    constexpr int shop_rows = shop_count + 2;   // + Cinda, Morrow
    constexpr int character_prices[char_count] = { 0, 300, 600 };

    // Block 4 review: grey text for prices the gold does not reach (font fill index 14, outline 12) and a dark
    // silhouette for locked characters (every opaque index the same dark blue).
    constexpr bn::color text_grey_colors[16] = {
        bn::color(0, 31, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0),
        bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0),
        bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(1, 1, 2), bn::color(0, 0, 0), bn::color(13, 13, 15),
        bn::color(0, 0, 0)
    };
    constexpr bn::sprite_palette_item text_grey(text_grey_colors, bn::bpp_mode::BPP_4);

    constexpr bn::color silhouette_colors[16] = {
        bn::color(0, 31, 0), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6),
        bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6),
        bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6), bn::color(3, 3, 6),
        bn::color(3, 3, 6)
    };
    constexpr bn::sprite_palette_item silhouette(silhouette_colors, bn::bpp_mode::BPP_4);

    // Hex icons that are off: flat grey (block 6).
    constexpr bn::color hex_off_colors[16] = {
        bn::color(0, 31, 0), bn::color(2, 2, 4), bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12),
        bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12),
        bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12), bn::color(9, 9, 12),
        bn::color(9, 9, 12)
    };
    constexpr bn::sprite_palette_item hex_off(hex_off_colors, bn::bpp_mode::BPP_4);

    struct screen
    {
        bn::sprite_text_generator small;
        bn::sprite_text_generator big;
        bn::vector<bn::sprite_ptr, 112> texts;

        screen() :
            small(common::variable_8x8_sprite_font),
            big(common::variable_16x16_sprite_font)
        {
        }

        void clear()
        {
            texts.clear();
        }

        void grey_text(int x, int y, const bn::string_view& s)
        {
            small.set_left_alignment();
            small.set_palette_item(text_grey);
            small.generate(x, y, s, texts);
        }

        void text(int x, int y, const bn::string_view& s, bool gold = false, bool center = false)
        {
            if(center)
            {
                small.set_center_alignment();
            }
            else
            {
                small.set_left_alignment();
            }

            small.set_palette_item(gold ? art::text_gold : common::variable_8x8_sprite_font.item().palette_item());
            small.generate(x, y, s, texts);
        }

        void title(const bn::string_view& s)
        {
            big.set_center_alignment();
            big.set_palette_item(art::text_gold);
            big.generate(0, -66, s, texts);
        }
    };

    void play_sfx(const bn::sound_item& item)
    {
        if(sfx_on())
        {
            item.play(bn::fixed(0.6));
        }
    }

    void play_music(const bn::music_item& item)
    {
        test_music_id = item.id();

        if(! bn::music::playing() || bn::music::playing_item() != item)
        {
            item.play(music_volume(art::music_volume_pct));
        }
    }

    void append_time(bn::string<32>& line, int s)
    {
        line.push_back(char('0' + (s / 60) / 10));
        line.push_back(char('0' + (s / 60) % 10));
        line.push_back(':');
        line.push_back(char('0' + (s % 60) / 10));
        line.push_back(char('0' + (s % 60) % 10));
    }

    void gold_line(screen& sc, int y = -70)
    {
        bn::string<32> line("G ");
        line.append(bn::to_string<8>(int(save().gold)));
        sc.small.set_right_alignment();
        sc.small.set_palette_item(art::text_gold);
        sc.small.generate(116, y, line, sc.texts);
    }

    // ---- Shop (§C4): 11 upgrades + 2 characters; A buys, SELECT refunds everything (with confirmation).
    [[nodiscard]] int row_price(int row)
    {
        const save_data& d = save();

        if(row < shop_count)
        {
            int rank = d.shop_ranks[row];
            return rank < shop_defs[row].ranks ? shop_defs[row].prices[rank] : -1;
        }

        int character = row - shop_count + 1;
        return character_available(character) ? -1 : character_prices[character];
    }

    [[nodiscard]] bool row_locked(int row)
    {
        return (row == shop_count && ! unlocked(unlock_stage2)) || (row == shop_count + 1 && ! unlocked(unlock_stage3));
    }

    void draw_shop(screen& sc, bn::vector<bn::sprite_ptr, 8>& icons, int cursor, int top, bool confirm)
    {
        sc.clear();
        icons.clear();
        sc.title("SHOP");
        gold_line(sc);

        for(int k = 0; k < 6; ++k)
        {
            int row = top + k;

            if(row >= shop_rows)
            {
                break;
            }

            int y = -40 + k * 17;
            bool sel = row == cursor;
            bn::string<32> name;
            bn::string<32> rank;

            if(row < shop_count)
            {
                bn::sprite_ptr icon = bn::sprite_items::shop_icons.create_sprite(-100, y, row);
                icons.push_back(bn::move(icon));
                name = shop_defs[row].name;
                rank = bn::to_string<4>(int(save().shop_ranks[row]));
                rank.push_back('/');
                rank.append(bn::to_string<4>(shop_defs[row].ranks));
            }
            else
            {
                int character = row - shop_count + 1;
                // Portrait in the icon column (block 4 review): half size so it stays inside its 17 px row.
                bn::sprite_ptr icon = bn::sprite_items::wizard.create_sprite(-100, y - 2, 0);
                icon.set_scale(0.5);

                if(row_locked(row))
                {
                    icon.set_palette(silhouette);   // locked: dark silhouette + name + condition
                }
                else
                {
                    icon.set_palette(character == char_cinda ? bn::sprite_palette_items::wizard_cinda_palette :
                                                               bn::sprite_palette_items::wizard_morrow_palette);
                }

                icons.push_back(bn::move(icon));
                name = character_name(character);
                rank = character_available(character) ? "OWNED" : "";
            }

            sc.text(-86, y, name, sel);
            sc.text(-10, y, rank, sel);
            int price = row_price(row);
            bn::string<32> p;

            if(row_locked(row))
            {
                p = row == shop_count ? "Clear stage 1" : "Clear stage 2";
            }
            else if(price < 0)
            {
                p = row < shop_count ? "MAX" : "";
            }
            else
            {
                p = bn::to_string<8>(price);
                p.append(" G");
            }

            if(price > int(save().gold) || row_locked(row))
            {
                sc.grey_text(30, y, p);   // not affordable (or locked): grey
            }
            else
            {
                sc.text(30, y, p, sel && price >= 0);
            }

            if(sel)
            {
                sc.text(-118, y, ">", true);
            }
        }

        // More rows above / below the page: the gold HUD edge arrow (north frame; flipped for down), right edge.
        if(top > 0)
        {
            icons.push_back(art::edge_arrow.create_sprite(110, -40, art::arrow_north));
        }

        if(top + 6 < shop_rows)
        {
            bn::sprite_ptr down = art::edge_arrow.create_sprite(110, 45, art::arrow_north);
            down.set_vertical_flip(true);
            icons.push_back(bn::move(down));
        }

        if(! confirm && shop_spent(save()) == 0 && ! (save().unlock_bits & (unlock_cinda | unlock_morrow)) &&
           int(save().gold) >= 30)
        {
            sc.text(0, -51, "Gold buys permanent upgrades", false, true);   // R6: one line, at the moment
        }

        if(confirm)
        {
            bn::string<32> line("REFUND ");
            line.append(bn::to_string<8>(shop_spent(save())));
            line.append(" G? A: YES  B: NO");
            sc.text(0, 64, line, true, true);
        }
        else
        {
            bn::string<48> info;

            if(cursor < shop_count)
            {
                info = shop_defs[cursor].effect;
            }
            else if(row_locked(cursor))
            {
                info = cursor == shop_count ? "Cinda - Clear stage 1" : "Morrow - Clear stage 2";
            }
            else if(cursor == shop_count)
            {
                info = "Cinda: Ember Toss, +10% area, -10 HP";
            }
            else
            {
                info = "Morrow: Storm Chain, +10% speed";
            }

            sc.text(0, 56, info, false, true);
            sc.text(0, 68, "A: BUY  SELECT: REFUND  B: BACK", false, true);
        }
    }

    void shop_screen()
    {
        test_screen = 4;
        play_music(bn::music_items::music_shop);
        screen sc;
        bn::vector<bn::sprite_ptr, 8> icons;
        int cursor = 0;
        int top = 0;
        bool confirm = false;
        draw_shop(sc, icons, cursor, top, confirm);

        while(true)
        {
            bn::core::update();
            bool redraw = false;
            save_data& d = save();

            if(confirm)
            {
                if(bn::keypad::a_pressed())
                {
                    d.gold += uint32_t(shop_spent(d));

                    for(uint8_t& r : d.shop_ranks)
                    {
                        r = 0;
                    }

                    write_save();
                    play_sfx(bn::sound_items::sfx_refund);
                    confirm = false;
                    redraw = true;
                }
                else if(bn::keypad::b_pressed())
                {
                    confirm = false;
                    redraw = true;
                }
            }
            else if(bn::keypad::b_pressed())
            {
                return;
            }
            else if(bn::keypad::up_pressed() && cursor > 0)
            {
                --cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::down_pressed() && cursor < shop_rows - 1)
            {
                ++cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::select_pressed())
            {
                if(shop_spent(d) > 0)
                {
                    confirm = true;
                    play_sfx(bn::sound_items::sfx_menu_move);
                    redraw = true;
                }
                else
                {
                    play_sfx(bn::sound_items::sfx_error);
                }
            }
            else if(bn::keypad::a_pressed())
            {
                int price = row_price(cursor);

                if(row_locked(cursor) || price < 0 || price > int(d.gold))
                {
                    play_sfx(bn::sound_items::sfx_error);
                }
                else
                {
                    d.gold -= uint32_t(price);

                    if(cursor < shop_count)
                    {
                        ++d.shop_ranks[cursor];
                    }
                    else
                    {
                        d.unlock_bits |= cursor == shop_count ? unlock_cinda : unlock_morrow;
                    }

                    write_save();
                    play_sfx(bn::sound_items::sfx_buy);
                    redraw = true;
                }
            }

            if(cursor < top)
            {
                top = cursor;
            }
            else if(cursor >= top + 6)
            {
                top = cursor - 5;
            }

            if(redraw)
            {
                draw_shop(sc, icons, cursor, top, confirm);
            }
        }
    }

    // ---- Records (§C4): per stage best time / kills / level / clears, then totals.
    void records_screen()
    {
        test_screen = 5;
        screen sc;
        sc.title("RECORDS");
        const save_data& d = save();
        constexpr const char* names[stage_count] = { "Gloam Meadow", "Grim Graveyard", "Ashen Keep" };

        for(int s = 0; s < stage_count; ++s)
        {
            const stage_record& r = d.records[s];
            int y = -42 + s * 26;
            sc.text(-112, y, names[s], true);
            bn::string<32> line;

            if(s > 0 && ! (d.unlock_bits & (s == 1 ? unlock_stage2 : unlock_stage3)))
            {
                line = "Locked";
            }
            else
            {
                line = "CLEAR x";
                line.append(bn::to_string<6>(int(r.clears)));
            }

            sc.text(20, y, line);

            if(r.best_time_s == 0 && r.best_kills == 0)
            {
                sc.text(-104, y + 10, "No runs yet");
            }
            else
            {
                line = "Best ";
                append_time(line, r.best_time_s);
                line.append("  Kills ");
                line.append(bn::to_string<6>(int(r.best_kills)));
                line.append("  Lv ");
                line.append(bn::to_string<4>(int(r.best_level)));
                sc.text(-104, y + 10, line);
            }
        }

        bn::string<48> line("Runs ");
        line.append(bn::to_string<6>(int(d.runs)));
        line.append("  Deaths ");
        line.append(bn::to_string<6>(int(d.deaths)));
        sc.text(0, 40, line, false, true);
        line = "Kills ";
        line.append(bn::to_string<8>(int(d.kills_total)));
        line.append("  Gold earned ");
        line.append(bn::to_string<8>(int(d.gold_total)));
        sc.text(0, 52, line, false, true);
        sc.text(0, 68, "B: BACK", false, true);

        while(! bn::keypad::b_pressed() && ! bn::keypad::a_pressed())
        {
            bn::core::update();
        }
    }

    // Stage 2 opens when stage 1 is cleared (E3); stage 3 is block 7.
    // ---- OPTIONS (DISENO_RUTA block 4 #8, G-55.2 defaults): music, SFX, screen shake, flash, erase save (hold A 2 s).
    void draw_options(screen& sc, int cursor, int hold)
    {
        sc.clear();
        sc.title("OPTIONS");
        const save_data& d = save();
        const char* names[5] = { "Music", "Sound FX", "Screen shake", "Flash", "Erase save" };
        bool values[4] = { d.music_vol != 0, d.sfx_vol != 0, ! (d.fx_bits & option_no_shake),
                           ! (d.fx_bits & option_no_flash) };

        for(int r = 0; r < 5; ++r)
        {
            int y = -36 + r * 16;
            bool sel = r == cursor;
            sc.text(-80, y, names[r], sel);

            if(r < 4)
            {
                sc.text(40, y, values[r] ? "ON" : "OFF", sel);
            }

            if(sel)
            {
                sc.text(-96, y, ">", true);
            }
        }

        if(cursor == 4)
        {
            if(hold > 0)
            {
                bn::string<32> bar("ERASING ");

                for(int k = 0; k < 10; ++k)
                {
                    bar.push_back(k < hold * 10 / 120 ? '#' : '.');
                }

                sc.text(0, 52, bar, true, true);
            }
            else
            {
                sc.text(0, 52, "Hold A for 2 s: erases all progress", false, true);
            }
        }

        sc.text(0, 68, "A: CHANGE  B: BACK", false, true);
    }

    void options_screen()
    {
        test_screen = 6;
        screen sc;
        int cursor = 0;
        int hold = 0;
        draw_options(sc, cursor, hold);

        while(true)
        {
            bn::core::update();
            save_data& d = save();
            bool redraw = false;

            if(cursor == 4 && bn::keypad::a_held())
            {
                ++hold;

                if(hold % 12 == 0)
                {
                    redraw = true;
                }

                if(hold >= 120)
                {
                    erase_save();
                    play_sfx(bn::sound_items::sfx_refund);
                    hold = 0;
                    redraw = true;
                }
            }
            else if(hold > 0)
            {
                hold = 0;   // released before 2 s: nothing happens
                redraw = true;
            }

            if(bn::keypad::b_pressed())
            {
                return;
            }
            else if(bn::keypad::up_pressed() && cursor > 0)
            {
                --cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::down_pressed() && cursor < 4)
            {
                ++cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(cursor < 4 && (bn::keypad::a_pressed() || bn::keypad::left_pressed() ||
                                   bn::keypad::right_pressed()))
            {
                if(cursor == 0)
                {
                    d.music_vol = d.music_vol ? 0 : 100;

                    if(bn::music::playing())
                    {
                        bn::music::set_volume(music_volume(art::music_volume_pct));
                    }
                }
                else if(cursor == 1)
                {
                    d.sfx_vol = d.sfx_vol ? 0 : 100;
                }
                else
                {
                    d.fx_bits ^= cursor == 2 ? option_no_shake : option_no_flash;
                }

                write_save();
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }

            if(redraw)
            {
                draw_options(sc, cursor, hold);
            }
        }
    }

    [[nodiscard]] bool stage_open(int stage)
    {
        return stage == 0 || (stage == 1 && (save().unlock_bits & unlock_stage2)) ||
               (stage == 2 && (save().unlock_bits & unlock_stage3));
    }

    // ---- Stage select (§R0): 3 stage cards, L/R picks the character, A starts.
    // DBG (debug_menu): SELECT opens stages 2 and 3 without clearing the previous ones.
    bool stage_select(run_setup& setup)
    {
        test_screen = 3;
        screen sc;
        bn::vector<bn::sprite_ptr, 3> cards;
        bn::optional<bn::sprite_ptr> wizard;
        bn::vector<bn::sprite_ptr, hex_count> hex_icons;
        int stage = stage_open(save().last_stage) ? int(save().last_stage) : 0;
        int hex_cursor = -1;   // block 6: >= 0 while DOWN has moved the cursor to the hex row
        int character = character_available(save().character) ? int(save().character) : int(char_wendel);
        bool redraw = true;

        while(true)
        {
            if(redraw)
            {
                redraw = false;
                sc.clear();
                cards.clear();
                sc.title("CHOOSE STAGE");

                for(int s = 0; s < stage_count; ++s)
                {
                    int x = -76 + s * 76;
                    bool open = stage_open(s);
                    bn::sprite_ptr card = bn::sprite_items::stage_cards.create_sprite(x, -14 - (s == stage ? 4 : 0),
                                                                                      open ? s : 3);
                    cards.push_back(bn::move(card));
                }

                constexpr const char* names[stage_count] = { "Gloam Meadow", "Grim Graveyard", "Ashen Keep" };
                sc.text(-76 + stage * 76, -50, ">", true, true);
                sc.text(0, 26, names[stage], true, true);

                bool hexes_open = unlocked(unlock_stage2);
                hex_icons.clear();

                if(hex_cursor >= 0)
                {
                    bn::string<40> line(hex_names[hex_cursor]);
                    line.append(": ");
                    line.append(hex_descs[hex_cursor]);
                    line.append(" +");
                    line.append(bn::to_string<4>(hex_gold_pct[hex_cursor]));
                    line.append("%G");
                    sc.text(0, 37, line, false, true);
                }
                else if(stage_open(stage))
                {
                    const stage_record& r = save().records[stage];
                    if(r.best_time_s == 0 && r.best_kills == 0)
                    {
                        sc.text(0, 37, "No runs yet", false, true);
                    }
                    else
                    {
                        bn::string<32> line("Best ");
                        append_time(line, r.best_time_s);
                        line.append("  CLEAR x");
                        line.append(bn::to_string<6>(int(r.clears)));
                        sc.text(0, 37, line, false, true);
                    }
                }
                else
                {
                    sc.text(0, 37, stage == 1 ? "CLEAR GLOAM MEADOW" : "CLEAR GRIM GRAVEYARD", false, true);
                }

                bn::string<32> who("L/R: ");
                who.append(character_name(character));
                sc.text(-66, 54, who);

                if(! wizard)
                {
                    wizard = bn::sprite_items::wizard.create_sprite(-86, 58, 0);   // >= 4 px below the "Best" line
                }

                if(character == char_cinda)
                {
                    wizard->set_palette(bn::sprite_palette_items::wizard_cinda_palette);
                }
                else if(character == char_morrow)
                {
                    wizard->set_palette(bn::sprite_palette_items::wizard_morrow_palette);
                }
                else
                {
                    wizard->set_palette(bn::sprite_items::wizard.palette_item());
                }

                if(hexes_open)
                {
                    // §C5 Hexes: 5 icons right of the character (grey = off); the gold bonus next to them.
                    for(int h = 0; h < hex_count; ++h)
                    {
                        bool on = save().hexes & (1 << h);
                        bn::sprite_ptr icon = art::hex_icons.create_sprite(14 + h * 12, h == hex_cursor ? 51 : 54, h);

                        if(! on)
                        {
                            icon.set_palette(hex_off);
                        }

                        hex_icons.push_back(bn::move(icon));
                    }

                    int pct = hexes_gold_pct(save().hexes);
                    bn::string<16> bonus;

                    if(pct > 0)
                    {
                        bonus.append("+");
                        bonus.append(bn::to_string<4>(pct));
                        bonus.append("%G");
                    }
                    else if(hex_cursor < 0)
                    {
                        bonus.append("v HEX");
                    }

                    sc.text(74, 54, bonus, pct > 0);
                }

                sc.text(0, 68, hex_cursor >= 0 ? "A: TOGGLE  UP: DONE" : "A: START  B: BACK", false, true);
            }

            bn::core::update();

            if(bn::keypad::b_pressed())
            {
                return false;
            }

            if(debug_menu && bn::keypad::select_pressed())
            {
                save().unlock_bits |= unlock_stage2 | unlock_stage3;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }

            if(hex_cursor >= 0)
            {
                // Hex row: LEFT/RIGHT pick, A toggles, UP / B back to the stages.
                if(bn::keypad::left_pressed() && hex_cursor > 0)
                {
                    --hex_cursor;
                    play_sfx(bn::sound_items::sfx_menu_move);
                    redraw = true;
                }
                else if(bn::keypad::right_pressed() && hex_cursor < hex_count - 1)
                {
                    ++hex_cursor;
                    play_sfx(bn::sound_items::sfx_menu_move);
                    redraw = true;
                }
                else if(bn::keypad::a_pressed())
                {
                    save().hexes ^= uint8_t(1 << hex_cursor);
                    play_sfx(art::sfx_card);
                    redraw = true;
                }
                else if(bn::keypad::up_pressed() || bn::keypad::start_pressed())
                {
                    hex_cursor = -1;
                    play_sfx(bn::sound_items::sfx_menu_move);
                    redraw = true;
                }

                continue;
            }

            if(bn::keypad::down_pressed() && unlocked(unlock_stage2))
            {
                hex_cursor = 0;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::left_pressed() && stage > 0)
            {
                --stage;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::right_pressed() && stage < stage_count - 1)
            {
                ++stage;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
            {
                int step = bn::keypad::r_pressed() ? 1 : char_count - 1;
                int next = character;

                for(int k = 0; k < char_count; ++k)
                {
                    next = (next + step) % char_count;

                    if(character_available(next))
                    {
                        break;
                    }
                }

                if(next != character)
                {
                    character = next;
                    play_sfx(bn::sound_items::sfx_menu_move);
                }
                else
                {
                    play_sfx(bn::sound_items::sfx_error);
                }

                redraw = true;
            }
            else if(bn::keypad::a_pressed())
            {
                if(! stage_open(stage))
                {
                    play_sfx(bn::sound_items::sfx_error);
                    continue;
                }

                save().character = uint8_t(character);
                save().last_stage = uint8_t(stage);
                setup.character = character;
                setup.stage = stage;
                setup.hexes = unlocked(unlock_stage2) ? int(save().hexes) : 0;
                play_sfx(art::sfx_card);
                bn::core::update();   // consume A
                return true;
            }
        }
    }
}

// E5.4: a gold star next to the logo once the ending has been seen.
bn::optional<bn::sprite_ptr> title_star()
{
    bn::optional<bn::sprite_ptr> star;
    test_title_star = (save().flags & save_flag_ending_seen) ? 1 : 0;

    if(test_title_star)
    {
        star = art::icons.create_sprite(92, -61, art::icon_star);
    }

    return star;
}

void ending_sequence()
{
    // E5.4 step 4: 3 text screens over the 8 bpp illustration (like the title), then 20 s of credits.
    test_screen = 7;
    bn::regular_bg_ptr bg = art::ending_bg.create_bg(8, 48);
    screen sc;
    play_music(art::music_final);
    constexpr const char* lines[3] = { "The Hex Moon fades.", "Wendel walks home at dawn.",
                                       "THE END - thanks for playing" };

    for(int k = 0; k < 3; ++k)
    {
        sc.clear();
        sc.text(0, 58, lines[k], k == 2, true);

        for(int f = 0; f < 6 * 60; ++f)
        {
            bn::core::update();

            if(f >= 60 && (bn::keypad::a_pressed() || bn::keypad::start_pressed()))
            {
                break;
            }
        }
    }

    test_screen = 8;
    sc.clear();
    bn::bg_palettes::set_fade(bn::color(0, 0, 0), bn::fixed(0.6));
    sc.title(game_title);
    sc.text(0, -20, "A game by David", false, true);
    sc.text(0, 0, "Made with Butano", false, true);
    sc.text(0, 40, "Thanks for playing!", true, true);

    for(int f = 0; f < 20 * 60; ++f)
    {
        bn::core::update();

        if(f >= 30 && bn::keypad::start_pressed())
        {
            break;
        }
    }

    sc.clear();
    bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
    bn::core::update();
}

unsigned title_screen()
{
    test_screen = 1;
    // Title art (art/LÉEME.md "Bloque 2"): 8 bpp illustration + MAGIC & DESTROY logo in 9 sprite cells.
    bn::regular_bg_ptr title_bg = art::title_bg.create_bg(8, 48);
    bn::sprite_text_generator small(common::variable_8x8_sprite_font);
    small.set_center_alignment();
    bn::vector<bn::sprite_ptr, 9> logo;

    for(int k = 0; k < 9; ++k)
    {
        logo.push_back(art::logo.create_sprite(-64 + 64 * (k % 3), -65 + 32 * (k / 3), k));
    }

    bn::optional<bn::sprite_ptr> star = title_star();
    bn::vector<bn::sprite_ptr, 4> press;
    unsigned frames = 0;
    play_music(art::music_title);
    test_mode = 0;

    while(! bn::keypad::start_pressed() && ! bn::keypad::a_pressed())
    {
        ++frames;

        if(frames % 60 == 1)
        {
            press.clear();
            small.generate(0, 68, "PRESS START", press);
        }
        else if(frames % 60 == 40)
        {
            press.clear();
        }

        bn::core::update();
    }

    play_sfx(art::sfx_card);
    bn::core::update();
    return frames;
}

bool main_menu(run_setup& setup)
{
    bn::optional<bn::regular_bg_ptr> title_bg;
    bn::vector<bn::sprite_ptr, 9> logo;
    bn::optional<bn::sprite_ptr> star;
    constexpr const char* items[4] = { "PLAY", "SHOP", "RECORDS", "OPTIONS" };
    int cursor = 0;

    while(true)
    {
        test_screen = 2;
        play_music(art::music_title);

        if(! title_bg)
        {
            title_bg = art::title_bg.create_bg(8, 48);

            for(int k = 0; k < 9; ++k)
            {
                logo.push_back(art::logo.create_sprite(-64 + 64 * (k % 3), -65 + 32 * (k / 3), k));
            }

            star = title_star();
        }

        screen sc;
        bool redraw = true;
        int chosen = -1;

        while(chosen < 0)
        {
            if(redraw)
            {
                redraw = false;
                sc.clear();

                for(int i = 0; i < 4; ++i)
                {
                    bn::string<16> line(i == cursor ? "> " : "");
                    line.append(items[i]);
                    sc.text(0, 22 + i * 12, line, i == cursor, true);
                }

                gold_line(sc, 70);
            }

            bn::core::update();

            if(bn::keypad::up_pressed() && cursor > 0)
            {
                --cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::down_pressed() && cursor < 3)
            {
                ++cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
            {
                chosen = cursor;
                play_sfx(art::sfx_card);
            }
        }

        sc.clear();
        logo.clear();
        star.reset();
        title_bg.reset();
        bn::core::update();

        if(chosen == 0)
        {
            if(stage_select(setup))
            {
                return true;
            }
        }
        else if(chosen == 1)
        {
            shop_screen();
        }
        else if(chosen == 2)
        {
            records_screen();
        }
        else
        {
            options_screen();
        }

        bn::core::update();
    }
}

}
