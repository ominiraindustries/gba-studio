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
volatile int test_screen;   // 1 title, 2 menu, 3 stage select, 4 shop, 5 records (headless tests)

namespace hb
{

namespace
{
    constexpr int stage_count = 3;
    constexpr int shop_rows = shop_count + 2;   // + Cinda, Morrow
    constexpr int character_prices[char_count] = { 0, 300, 600 };

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
        item.play(bn::fixed(0.6));
    }

    void play_music(const bn::music_item& item)
    {
        if(! bn::music::playing() || bn::music::playing_item() != item)
        {
            item.play(bn::fixed(art::music_volume_pct) / 100);
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
                bn::sprite_ptr icon = bn::sprite_items::wizard.create_sprite(-100, y - 4, 0);
                icon.set_palette(character == char_cinda ? bn::sprite_palette_items::wizard_cinda_palette :
                                                           bn::sprite_palette_items::wizard_morrow_palette);
                icons.push_back(bn::move(icon));
                name = row_locked(row) ? "?" : character_name(character);
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

            sc.text(30, y, p, sel && price >= 0 && price <= int(save().gold));

            if(sel)
            {
                sc.text(-118, y, ">", true);
            }
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
    [[nodiscard]] bool stage_open(int stage)
    {
        return stage == 0 || (stage == 1 && (save().unlock_bits & unlock_stage2));
    }

    // ---- Stage select (§R0): 3 stage cards, L/R picks the character, A starts. Stage 3 "COMING SOON".
    // DBG (debug_menu): SELECT opens stage 2 without clearing stage 1.
    bool stage_select(run_setup& setup)
    {
        test_screen = 3;
        screen sc;
        bn::vector<bn::sprite_ptr, 3> cards;
        bn::optional<bn::sprite_ptr> wizard;
        int stage = stage_open(save().last_stage) ? int(save().last_stage) : 0;
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

                if(stage_open(stage))
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
                    sc.text(0, 37, stage == 1 ? "CLEAR GLOAM MEADOW" : "COMING SOON", false, true);
                }

                bn::string<32> who("L/R: ");
                who.append(character_name(character));
                sc.text(-66, 54, who);

                if(! wizard)
                {
                    wizard = bn::sprite_items::wizard.create_sprite(-84, 50, 0);
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

                sc.text(0, 68, "A: START  B: BACK", false, true);
            }

            bn::core::update();

            if(bn::keypad::b_pressed())
            {
                return false;
            }

            if(debug_menu && bn::keypad::select_pressed())
            {
                save().unlock_bits |= unlock_stage2;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }

            if(bn::keypad::left_pressed() && stage > 0)
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
                art::sfx_card.play(bn::fixed(0.6));
                bn::core::update();   // consume A
                return true;
            }
        }
    }
}

unsigned title_screen()
{
    test_screen = 1;
    // Title art (art/LÉEME.md "Bloque 2"): 8 bpp illustration + HEXBOUND logo in 6 sprite cells.
    bn::regular_bg_ptr title_bg = art::title_bg.create_bg(8, 48);
    bn::sprite_text_generator small(common::variable_8x8_sprite_font);
    small.set_center_alignment();
    bn::vector<bn::sprite_ptr, 6> logo;

    for(int k = 0; k < 6; ++k)
    {
        logo.push_back(art::logo.create_sprite(-64 + 64 * (k % 3), -60 + 32 * (k / 3), k));
    }

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

    art::sfx_card.play(bn::fixed(0.6));
    bn::core::update();
    return frames;
}

bool main_menu(run_setup& setup)
{
    bn::optional<bn::regular_bg_ptr> title_bg;
    bn::vector<bn::sprite_ptr, 6> logo;
    constexpr const char* items[3] = { "PLAY", "SHOP", "RECORDS" };
    int cursor = 0;

    while(true)
    {
        test_screen = 2;
        play_music(art::music_title);

        if(! title_bg)
        {
            title_bg = art::title_bg.create_bg(8, 48);

            for(int k = 0; k < 6; ++k)
            {
                logo.push_back(art::logo.create_sprite(-64 + 64 * (k % 3), -60 + 32 * (k / 3), k));
            }
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

                for(int i = 0; i < 3; ++i)
                {
                    bn::string<16> line(i == cursor ? "> " : "");
                    line.append(items[i]);
                    sc.text(0, 24 + i * 14, line, i == cursor, true);
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
            else if(bn::keypad::down_pressed() && cursor < 2)
            {
                ++cursor;
                play_sfx(bn::sound_items::sfx_menu_move);
                redraw = true;
            }
            else if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
            {
                chosen = cursor;
                art::sfx_card.play(bn::fixed(0.6));
            }
        }

        sc.clear();
        logo.clear();
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
        else
        {
            records_screen();
        }

        bn::core::update();
    }
}

}
