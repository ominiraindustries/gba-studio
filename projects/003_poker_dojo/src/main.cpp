// POKET GTO: a poker trainer by levels. Each drill is one decision with instant feedback.
// Block 1 scaffold: title, level select, "which hand wins?" drill, results with stars saved to SRAM.
// Push/fold drills plug in when the solver tables (include/poker_tables_pushfold.h) land.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_color.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_seed_random.h"
#include "bn_sound_items.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sram.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_gb_table.h"
#include "bn_sprite_items_gb_card.h"
#include "bn_sprite_items_gb_card_back.h"
#include "bn_sprite_items_gb_cursor.h"
#include "bn_sprite_items_gb_suits.h"
#include "bn_sprite_items_gb_suits_big.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "poker_cards.h"

// Mirrors of the game state for headless tests (gbarun.py --check test_x.32...).
volatile int test_scene;
volatile int test_level;
volatile int test_hand;
volatile int test_correct;
volatile int test_answer;      // expected answer index of the current drill
volatile int test_stars;

namespace
{
    using namespace poker;

    enum class scene { title, level_select, which_hand, results };

    enum class level_kind { which_hand, kickers, push_sb, call_bb };

    struct level_info
    {
        const char* code;
        const char* name;
        level_kind kind;
        int hands;
        int pass;           // correct answers needed for one star
        bool available;     // false until its tables exist
    };

    constexpr int level_count = 4;
    constexpr level_info levels[level_count] = {
        { "0-1", "WHICH HAND WINS?", level_kind::which_hand, 20, 17, true },
        { "0-2", "KICKERS AND TIES", level_kind::kickers, 20, 17, true },
        { "1-1", "SB SHOVE OR FOLD 10BB", level_kind::push_sb, 20, 17, false },
        { "1-2", "BB CALL OR FOLD 10BB", level_kind::call_bb, 20, 17, false },
    };

    // ---------------------------------------------------------------------------------------------------------
    // Save data.

    struct save_data
    {
        char magic[8];
        unsigned char stars[16];
    };

    constexpr char save_magic[8] = { 'P', 'K', 'T', 'G', 'T', 'O', '0', '1' };
    save_data save;

    void load_save()
    {
        bn::sram::read(save);
        bool valid = true;

        for(int i = 0; i < 8; ++i)
        {
            valid = valid && save.magic[i] == save_magic[i];
        }

        if(! valid)
        {
            save = save_data();

            for(int i = 0; i < 8; ++i)
            {
                save.magic[i] = save_magic[i];
            }

            bn::sram::write(save);
        }
    }

    // ---------------------------------------------------------------------------------------------------------
    // Text and cards.

    constexpr bn::color rgb(int r, int g, int b)
    {
        return bn::color(r / 8, g / 8, b / 8);
    }

    // Font uses index 14 for the fill and 12 for the outline: dark ink on the white card.
    constexpr bn::color card_white = rgb(250, 250, 245);
    constexpr bn::color ink_colors[4][16] = {
        { {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, card_white, {}, rgb(20, 20, 30), {} },
        { {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, card_white, {}, rgb(210, 30, 40), {} },
        { {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, card_white, {}, rgb(30, 90, 220), {} },
        { {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, card_white, {}, rgb(20, 150, 60), {} },
    };
    constexpr bn::sprite_palette_item ink_palettes[4] = {
        bn::sprite_palette_item(ink_colors[0], bn::bpp_mode::BPP_4),
        bn::sprite_palette_item(ink_colors[1], bn::bpp_mode::BPP_4),
        bn::sprite_palette_item(ink_colors[2], bn::bpp_mode::BPP_4),
        bn::sprite_palette_item(ink_colors[3], bn::bpp_mode::BPP_4),
    };

    // Yellow text for highlights and results.
    constexpr bn::color gold_colors[16] = {
        {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, rgb(60, 40, 0), {}, rgb(255, 215, 60), {}
    };
    constexpr bn::sprite_palette_item gold_palette(gold_colors, bn::bpp_mode::BPP_4);

    struct texts
    {
        bn::sprite_text_generator small;
        bn::sprite_text_generator big;
        bn::sprite_text_generator ink;

        texts() :
            small(common::variable_8x8_sprite_font),
            big(common::variable_8x16_sprite_font),
            ink(common::variable_8x8_sprite_font)
        {
            small.set_center_alignment();
            big.set_center_alignment();
            ink.set_left_alignment();
        }
    };

    // One card on screen: body, rank text, small and big suit. Card body is 24x32 centred on (x, y).
    class card_view
    {

    public:
        void show(texts& t, card c, int x, int y)
        {
            clear();
            _body = bn::sprite_items::gb_card.create_sprite(x, y);
            t.ink.set_palette_item(ink_palettes[c.suit]);
            t.ink.generate(x - 10, y - 11, rank_names[c.rank], _rank);
            _small_suit = bn::sprite_items::gb_suits.create_sprite(x - 7, y - 2, c.suit);
            _big_suit = bn::sprite_items::gb_suits_big.create_sprite(x + 3, y + 7, c.suit);
        }

        void show_back(int x, int y)
        {
            clear();
            _body = bn::sprite_items::gb_card_back.create_sprite(x, y);
        }

        void clear()
        {
            _body.reset();
            _small_suit.reset();
            _big_suit.reset();
            _rank.clear();
        }

    private:
        bn::optional<bn::sprite_ptr> _body;
        bn::optional<bn::sprite_ptr> _small_suit;
        bn::optional<bn::sprite_ptr> _big_suit;
        bn::vector<bn::sprite_ptr, 2> _rank;
    };

    // Shared result of a drill, read by the results scene.
    struct drill_result
    {
        int level = 0;
        int correct = 0;
        int hands = 0;
        int stars = 0;
    };

    drill_result last_result;
    bn::seed_random random;
    unsigned frame_counter = 0;

    void wait_frame()
    {
        ++frame_counter;
        bn::core::update();
    }

    int stars_for(const level_info& level, int correct)
    {
        if(correct >= level.hands)
        {
            return 3;
        }

        if(correct >= level.pass + (level.hands - level.pass) / 2)
        {
            return 2;
        }

        return correct >= level.pass ? 1 : 0;
    }

    // ---------------------------------------------------------------------------------------------------------
    // Scenes. Each one runs its own loop and returns the next scene.

    scene title_scene(texts& t)
    {
        bn::vector<bn::sprite_ptr, 32> sprites;
        t.big.generate(0, -44, "POKET GTO", sprites);
        t.small.generate(0, -24, "LEARN POKER, ONE DECISION AT A TIME", sprites);
        bn::vector<bn::sprite_ptr, 8> blink;
        int timer = 0;

        while(true)
        {
            blink.clear();

            if((timer / 30) % 2 == 0)
            {
                t.small.generate(0, 40, "PRESS START", blink);
            }

            if(bn::keypad::start_pressed() || bn::keypad::a_pressed())
            {
                random.set_seed(frame_counter * 2654435761u + 7);
                return scene::level_select;
            }

            ++timer;
            wait_frame();
        }
    }

    int selected_level = 0;

    scene level_select_scene(texts& t)
    {
        bn::vector<bn::sprite_ptr, 48> sprites;
        bn::vector<bn::sprite_ptr, 16> info;
        bool redraw = true;

        while(true)
        {
            if(bn::keypad::up_pressed() && selected_level > 0)
            {
                --selected_level;
                redraw = true;
            }
            else if(bn::keypad::down_pressed() && selected_level < level_count - 1)
            {
                ++selected_level;
                redraw = true;
            }

            if(redraw)
            {
                redraw = false;
                sprites.clear();
                t.big.generate(0, -66, "CHOOSE A LEVEL", sprites);

                for(int i = 0; i < level_count; ++i)
                {
                    const level_info& level = levels[i];
                    bn::string<48> line(level.code);
                    line += "  ";
                    line += level.name;
                    t.small.set_left_alignment();
                    t.small.generate(-104, -36 + i * 18, line, sprites);

                    if(i == selected_level)
                    {
                        t.small.generate(-114, -36 + i * 18, ">", sprites);
                    }
                    t.small.set_right_alignment();

                    bn::string<8> stars;

                    for(int s = 0; s < 3; ++s)
                    {
                        stars += s < save.stars[i] ? '*' : '-';
                    }

                    t.small.generate(104, -36 + i * 18, level.available ? stars : bn::string<8>("SOON"), sprites);
                }

                t.small.set_center_alignment();
                t.small.generate(0, 68, "A: PLAY   B: BACK", sprites);
                info.clear();
            }

            if(bn::keypad::a_pressed())
            {
                if(levels[selected_level].available)
                {
                    return scene::which_hand;
                }

                info.clear();
                t.small.generate(0, 40, "COMING SOON: SOLVER TABLES IN PROGRESS", info);
            }
            else if(bn::keypad::b_pressed())
            {
                return scene::title;
            }

            test_level = selected_level;
            wait_frame();
        }
    }

    // "Which hand wins?": two hands and a full board; answer LEFT, SPLIT or RIGHT.
    scene which_hand_scene(texts& t)
    {
        const level_info& level = levels[selected_level];
        card_view board_views[5];
        card_view left_views[2];
        card_view right_views[2];
        bn::vector<bn::sprite_ptr, 48> sprites;
        bn::vector<bn::sprite_ptr, 32> feedback;
        bn::sprite_ptr cursor = bn::sprite_items::gb_cursor.create_sprite(0, 66);
        constexpr const char* button_names[3] = { "LEFT", "TIE", "RIGHT" };
        constexpr int button_x[3] = { -76, 0, 76 };

        int correct = 0;
        int choice = 1;

        for(int hand = 0; hand < level.hands; ++hand)
        {
            // 0-2 only deals spots where both hands share a category (kickers, best 5 of 7, ties).
            card left[7];
            card right[7];

            for(int attempt = 0; attempt < 400; ++attempt)
            {
                deck d;
                d.shuffle(random);

                for(int i = 0; i < 2; ++i)
                {
                    left[i] = d.draw();
                    right[i] = d.draw();
                }

                for(int i = 0; i < 5; ++i)
                {
                    card c = d.draw();
                    left[2 + i] = c;
                    right[2 + i] = c;
                }

                if(level.kind != level_kind::kickers ||
                        category_of(evaluate(left, 7)) == category_of(evaluate(right, 7)))
                {
                    break;
                }
            }

            for(int i = 0; i < 5; ++i)
            {
                board_views[i].show(t, left[2 + i], -56 + i * 28, -28);
            }

            left_views[0].show(t, left[0], -90, 26);
            left_views[1].show(t, left[1], -64, 26);
            right_views[0].show(t, right[0], 64, 26);
            right_views[1].show(t, right[1], 90, 26);

            int left_score = evaluate(left, 7);
            int right_score = evaluate(right, 7);
            int answer = left_score > right_score ? 0 : (left_score < right_score ? 2 : 1);
            test_answer = answer;
            test_hand = hand;

            sprites.clear();
            t.small.set_left_alignment();
            t.small.generate(-116, -72, bn::string<48>(level.code) + "  " + level.name, sprites);
            t.small.set_right_alignment();
            bn::string<16> counter("HAND ");
            counter += bn::to_string<4>(hand + 1);
            counter += "/";
            counter += bn::to_string<4>(level.hands);
            t.small.generate(116, -72, counter, sprites);
            t.small.set_center_alignment();
            t.small.generate(-77, 4, "LEFT", sprites);
            t.small.generate(77, 4, "RIGHT", sprites);

            for(int i = 0; i < 3; ++i)
            {
                t.small.generate(button_x[i], 66, button_names[i], sprites);
            }

            // Choose.
            feedback.clear();

            while(true)
            {
                if(bn::keypad::left_pressed() && choice > 0)
                {
                    --choice;
                }
                else if(bn::keypad::right_pressed() && choice < 2)
                {
                    ++choice;
                }

                cursor.set_position(button_x[choice], 66);
                cursor.set_visible(true);

                if(bn::keypad::a_pressed())
                {
                    break;
                }

                wait_frame();
            }

            wait_frame();

            // Feedback: verdict and both hand names, then A for the next hand.
            bool ok = choice == answer;
            correct += ok;
            test_correct = correct;
            bn::sound_items::blip.play();
            cursor.set_visible(false);

            sprites.clear();
            t.small.set_left_alignment();
            t.small.generate(-116, -72, bn::string<48>(level.code) + "  " + level.name, sprites);
            t.small.set_center_alignment();
            t.small.generate(-66, 48, category_names[category_of(left_score)], sprites);
            t.small.generate(66, 48, category_names[category_of(right_score)], sprites);

            t.big.set_palette_item(ok ? gold_palette : ink_palettes[1]);
            t.big.generate(0, 4, ok ? "CORRECT!" : "WRONG", feedback);
            t.big.set_palette_item(common::variable_8x16_sprite_font.item().palette_item());

            bn::string<32> line(answer == 1 ? "TIE: SPLIT POT" : (answer == 0 ? "LEFT WINS" : "RIGHT WINS"));
            t.small.generate(0, 66, line + "  -  A: NEXT", feedback);

            while(! bn::keypad::a_pressed())
            {
                wait_frame();
            }

            wait_frame();
        }

        last_result.level = selected_level;
        last_result.correct = correct;
        last_result.hands = level.hands;
        last_result.stars = stars_for(level, correct);
        return scene::results;
    }

    scene results_scene(texts& t)
    {
        const drill_result& r = last_result;
        bn::vector<bn::sprite_ptr, 32> sprites;
        bool record = r.stars > save.stars[r.level];

        if(record)
        {
            save.stars[r.level] = r.stars;
            bn::sram::write(save);
        }

        test_stars = r.stars;
        t.big.generate(0, -40, r.stars ? "LEVEL PASSED!" : "TRY AGAIN", sprites);
        bn::string<32> line(bn::to_string<4>(r.correct));
        line += " / ";
        line += bn::to_string<4>(r.hands);
        line += " CORRECT";
        t.small.generate(0, -12, line, sprites);

        bn::string<8> stars;

        for(int s = 0; s < 3; ++s)
        {
            stars += s < r.stars ? '*' : '-';
        }

        t.big.generate(0, 8, stars, sprites);

        if(record)
        {
            t.small.generate(0, 28, "NEW BEST!", sprites);
        }

        t.small.generate(0, 60, "A: CONTINUE", sprites);

        while(! bn::keypad::a_pressed())
        {
            wait_frame();
        }

        wait_frame();
        return scene::level_select;
    }
}

int main()
{
    bn::core::init();
    load_save();

    bn::regular_bg_ptr table = bn::regular_bg_items::gb_table.create_bg(8, 48);
    texts t;
    scene current = scene::title;

    while(true)
    {
        test_scene = int(current);

        switch(current)
        {

        case scene::title:
            current = title_scene(t);
            break;

        case scene::level_select:
            current = level_select_scene(t);
            break;

        case scene::which_hand:
            current = which_hand_scene(t);
            break;

        case scene::results:
            current = results_scene(t);
            break;

        default:
            current = scene::title;
            break;
        }

        // Let the A press that closed a scene end before the next scene reads it.
        wait_frame();
    }
}
