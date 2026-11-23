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
#include "poker_tables_pushfold.h"

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

    enum class scene { title, level_select, which_hand, push_fold, math_outs, math_pot_odds, results };

    enum class level_kind { which_hand, kickers, push_sb, call_bb, math_outs, math_pot_odds };

    struct level_info
    {
        const char* code;
        const char* name;
        level_kind kind;
        int hands;
        int pass;           // correct answers needed for one star
        bool available;     // false until its tables exist
    };

    constexpr int level_count = 6;
    constexpr level_info levels[level_count] = {
        { "0-1", "WHICH HAND WINS?", level_kind::which_hand, 20, 17, true },
        { "0-2", "KICKERS AND TIES", level_kind::kickers, 20, 17, true },
        { "1-1", "SB SHOVE OR FOLD 10BB", level_kind::push_sb, 20, 17, true },
        { "1-2", "BB CALL OR FOLD 10BB", level_kind::call_bb, 20, 17, true },
        { "MD-1", "OUTS: RULE OF 4", level_kind::math_outs, 10, 8, true },
        { "MD-2", "POT ODDS", level_kind::math_pot_odds, 10, 8, true },
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
        t.small.generate(0, -24, "ONE DECISION AT A TIME", sprites);
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
                    switch(levels[selected_level].kind)
                    {

                    case level_kind::math_outs:
                        return scene::math_outs;

                    case level_kind::math_pot_odds:
                        return scene::math_pot_odds;

                    case level_kind::push_sb:
                    case level_kind::call_bb:
                        return scene::push_fold;

                    default:
                        return scene::which_hand;
                    }
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

                int left_category = category_of(evaluate(left, 7));
                int right_category = category_of(evaluate(right, 7));
                int gap = left_category > right_category ? left_category - right_category :
                                                           right_category - left_category;
                bool fits = level.kind == level_kind::kickers ? gap == 0 : gap >= (hand < 3 ? 2 : 1);

                if(fits)
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
            t.small.generate(116, -60, counter, sprites);
            t.small.set_center_alignment();
            t.small.generate(-77, 4, "LEFT", sprites);
            t.small.generate(77, 4, "RIGHT", sprites);

            for(int i = 0; i < 3; ++i)
            {
                t.small.generate(button_x[i], 66, button_names[i], sprites);
            }

            // Choose.
            feedback.clear();

            if(hand < 3)
            {
                t.small.generate(0, 77, "LEFT/RIGHT: CHOOSE   A: OK", feedback);
            }

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

            feedback.clear();   // drop the controls hint
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

    // ---------------------------------------------------------------------------------------------------------
    // Question widgets shared by the MATH DOJO levels.

    void wait_for_a()
    {
        while(! bn::keypad::a_pressed())
        {
            wait_frame();
        }

        wait_frame();
    }

    // Up to 4 option buttons on the bottom row; returns the chosen index.
    // Controls hint shown under the buttons in the first decisions of a level (rule 6).
    bool show_control_hint = false;

    int ask_options(texts& t, const char* const* labels, int count)
    {
        bn::vector<bn::sprite_ptr, 32> sprites;

        if(show_control_hint)
        {
            t.small.generate(0, 77, "LEFT/RIGHT: CHOOSE   A: OK", sprites);
        }

        int spacing = 240 / count;
        int xs[4] = {};

        for(int i = 0; i < count; ++i)
        {
            xs[i] = -120 + spacing / 2 + i * spacing;
            t.small.generate(xs[i], 66, labels[i], sprites);
        }

        bn::sprite_ptr cursor = bn::sprite_items::gb_cursor.create_sprite(xs[0], 66);
        int choice = 0;

        while(true)
        {
            if(bn::keypad::left_pressed() && choice > 0)
            {
                --choice;
            }
            else if(bn::keypad::right_pressed() && choice < count - 1)
            {
                ++choice;
            }

            cursor.set_position(xs[choice], 66);

            if(bn::keypad::a_pressed())
            {
                break;
            }

            wait_frame();
        }

        wait_frame();
        return choice;
    }

    // Number picker: up/down +-1, left/right +-10, A confirms.
    int ask_number(texts& t, const char* suffix, int start, int low, int high)
    {
        bn::vector<bn::sprite_ptr, 8> value_sprites;
        bn::vector<bn::sprite_ptr, 16> help;
        t.small.generate(0, 72, "UP/DOWN 1   LEFT/RIGHT 10   A: OK", help);
        int value = start;
        int shown = -1;

        while(true)
        {
            if(bn::keypad::up_pressed())
            {
                ++value;
            }
            else if(bn::keypad::down_pressed())
            {
                --value;
            }
            else if(bn::keypad::right_pressed())
            {
                value += 10;
            }
            else if(bn::keypad::left_pressed())
            {
                value -= 10;
            }

            value = value < low ? low : (value > high ? high : value);

            if(value != shown)
            {
                shown = value;
                value_sprites.clear();
                bn::string<16> text("< ");
                text += bn::to_string<6>(value);
                text += suffix;
                text += " >";
                t.big.generate(0, 54, text, value_sprites);
            }

            if(bn::keypad::a_pressed())
            {
                break;
            }

            wait_frame();
        }

        wait_frame();
        return value;
    }

    void draw_header(texts& t, bn::ivector<bn::sprite_ptr>& sprites, const level_info& level, int question)
    {
        t.small.set_left_alignment();
        t.small.generate(-116, -72, bn::string<48>(level.code) + "  " + level.name, sprites);
        t.small.set_right_alignment();
        bn::string<16> counter("QUESTION ");
        counter += bn::to_string<4>(question + 1);
        counter += "/";
        counter += bn::to_string<4>(level.hands);
        t.small.generate(116, -60, counter, sprites);
        t.small.set_center_alignment();
    }

    void show_verdict(texts& t, bn::ivector<bn::sprite_ptr>& sprites, bool ok, bool half = false)
    {
        t.big.set_palette_item(ok ? gold_palette : ink_palettes[1]);
        t.big.generate(0, 30, ok ? "CORRECT!" : (half ? "HALF RIGHT" : "NOT QUITE"), sprites);
        t.big.set_palette_item(common::variable_8x16_sprite_font.item().palette_item());
    }

    drill_result finish(const level_info& level, int correct)
    {
        drill_result r;
        r.level = selected_level;
        r.correct = correct;
        r.hands = level.hands;
        r.stars = stars_for(level, correct);
        return r;
    }

    // ---------------------------------------------------------------------------------------------------------
    // MD-1: count outs on the flop, then estimate the equity to the river (rule of 4 accepted).

    enum draw_kind { draw_flush, draw_oesd, draw_gutshot, draw_combo, draw_gutshot_flush, draw_kind_count };
    constexpr const char* draw_names[draw_kind_count] = {
        "FLUSH DRAW", "OPEN-ENDED STRAIGHT DRAW", "GUTSHOT", "FLUSH + STRAIGHT DRAW", "FLUSH DRAW + GUTSHOT",
    };
    constexpr int draw_outs[draw_kind_count] = { 9, 8, 4, 15, 12 };

    // Turn cards that give a straight or better (hole cards always take part: the board has only 4 cards).
    int count_outs(const card* five)
    {
        bool used[52] = {};

        for(int i = 0; i < 5; ++i)
        {
            used[five[i].id()] = true;
        }

        card six[6];

        for(int i = 0; i < 5; ++i)
        {
            six[i] = five[i];
        }

        int outs = 0;

        for(int id = 0; id < 52; ++id)
        {
            if(! used[id])
            {
                six[5] = card::from_id(id);
                outs += category_of(evaluate(six, 6)) >= straight;
            }
        }

        return outs;
    }

    // Builds hero (0-1) + flop (2-4) for a draw template; ranks are distinct, suits as the template needs.
    void build_draw(draw_kind kind, card* five)
    {
        int s = random.get_unbiased_int(4);
        int other = (s + 1 + random.get_unbiased_int(3)) % 4;
        int ranks[5];

        if(kind == draw_flush)
        {
            int pool[13];

            for(int i = 0; i < 13; ++i)
            {
                pool[i] = i;
            }

            for(int i = 0; i < 5; ++i)
            {
                int j = i + random.get_unbiased_int(13 - i);
                int tmp = pool[i];
                pool[i] = pool[j];
                pool[j] = tmp;
                ranks[i] = pool[i];
            }
        }
        else
        {
            bool gut = kind == draw_gutshot || kind == draw_gutshot_flush;
            int r = gut ? random.get_unbiased_int(9) : 1 + random.get_unbiased_int(8);
            ranks[0] = r;
            ranks[1] = r + 1;
            ranks[2] = gut ? r + 3 : r + 2;
            ranks[3] = gut ? r + 4 : r + 3;

            // A far card that does not touch the run.
            do
            {
                ranks[4] = random.get_unbiased_int(13);
            }
            while(ranks[4] >= r - 2 && ranks[4] <= r + 6);
        }

        bool flush = kind == draw_flush || kind == draw_combo || kind == draw_gutshot_flush;

        for(int i = 0; i < 5; ++i)
        {
            int suit;

            if(flush)
            {
                suit = i == 3 ? other : s;   // hero 2 + two flop cards of the suit, one off-suit flop card
            }
            else
            {
                suit = (s + i) % 4;          // rainbow-ish: never four of a suit
            }

            five[i] = card{ ranks[i], suit };
        }
    }

    scene math_outs_scene(texts& t)
    {
        const level_info& level = levels[selected_level];
        card_view flop_views[3];
        card_view hero_views[2];
        bn::vector<bn::sprite_ptr, 48> sprites;
        int correct = 0;

        for(int q = 0; q < level.hands; ++q)
        {
            draw_kind kind = draw_kind(random.get_unbiased_int(draw_kind_count));
            card five[5];

            for(int attempt = 0; attempt < 50; ++attempt)
            {
                build_draw(kind, five);

                if(category_of(evaluate(five, 5)) < straight && count_outs(five) == draw_outs[kind])
                {
                    break;
                }
            }

            int outs = count_outs(five);
            int exact = 100 - ((47 - outs) * (46 - outs) * 100 + (47 * 46) / 2) / (47 * 46);
            test_answer = outs;
            test_hand = q;

            for(int i = 0; i < 3; ++i)
            {
                flop_views[i].show(t, five[2 + i], -28 + i * 28, -36);
            }

            hero_views[0].show(t, five[0], -14, 8);
            hero_views[1].show(t, five[1], 14, 8);

            sprites.clear();
            draw_header(t, sprites, level, q);
            t.small.generate(0, -58, "FLOP", sprites);
            t.small.generate(0, 30, "OUTS TO A STRAIGHT OR BETTER?", sprites);

            // Options: the right count and three others from the usual draw sizes.
            constexpr int sizes[5] = { 4, 8, 9, 12, 15 };
            int option_values[4];
            int n = 0;
            int skip = random.get_unbiased_int(4);

            for(int i = 0, k = 0; i < 5 && n < 4; ++i)
            {
                if(sizes[i] == outs)
                {
                    option_values[n++] = sizes[i];
                }
                else if(k++ != skip)
                {
                    option_values[n++] = sizes[i];
                }
            }

            bn::string<4> labels_storage[4];
            const char* labels[4];

            for(int i = 0; i < 4; ++i)
            {
                labels_storage[i] = bn::to_string<4>(option_values[i]);
                labels[i] = labels_storage[i].c_str();
            }

            show_control_hint = q < 2;
            int outs_answer = option_values[ask_options(t, labels, 4)];
            bool outs_ok = outs_answer == outs;

            sprites.clear();
            draw_header(t, sprites, level, q);
            t.small.generate(0, 30, "CHANCE TO HIT BY THE RIVER? (%)", sprites);
            int answer = ask_number(t, "%", 20, 0, 100);
            int diff = answer > exact ? answer - exact : exact - answer;
            bool equity_ok = diff <= 4 || answer == outs * 4;

            bool ok = outs_ok && equity_ok;
            correct += ok;
            test_correct = correct;
            bn::sound_items::blip.play();

            sprites.clear();
            draw_header(t, sprites, level, q);
            show_verdict(t, sprites, ok, outs_ok || equity_ok);
            bn::string<48> line(bn::to_string<4>(outs));
            line += " OUTS: ";
            line += draw_names[kind];
            t.small.generate(0, 48, line, sprites);
            line = "EXACT ";
            line += bn::to_string<4>(exact);
            line += "%   RULE OF 4: ";
            line += bn::to_string<4>(outs);
            line += " x 4 = ";
            line += bn::to_string<4>(outs * 4);
            line += "%";
            t.small.generate(0, 60, line, sprites);
            line = "YOU: ";
            line += bn::to_string<4>(outs_answer);
            line += outs_ok ? " OUTS OK   " : " OUTS X   ";
            line += bn::to_string<4>(answer);
            line += equity_ok ? "% OK   A: NEXT" : "% X   A: NEXT";
            t.small.generate(0, 72, line, sprites);
            wait_for_a();
        }

        last_result = finish(level, correct);
        return scene::results;
    }

    // ---------------------------------------------------------------------------------------------------------
    // MD-2: pot odds. Equity needed = bet / (pot + 2 * bet); then call or fold with a given equity.

    scene math_pot_odds_scene(texts& t)
    {
        const level_info& level = levels[selected_level];
        bn::vector<bn::sprite_ptr, 48> sprites;
        constexpr int bet_percents[7] = { 25, 33, 50, 66, 75, 100, 150 };
        int correct = 0;

        for(int q = 0; q < level.hands; ++q)
        {
            int pot = 4 + random.get_unbiased_int(27);
            int percent = bet_percents[random.get_unbiased_int(7)];
            int bet = (pot * percent + 50) / 100;
            bet = bet < 1 ? 1 : bet;
            percent = (bet * 100 + pot / 2) / pot;   // show the real size after rounding the bet
            int total = pot + 2 * bet;
            int need = (bet * 100 + total / 2) / total;
            test_answer = need;
            test_hand = q;

            sprites.clear();
            draw_header(t, sprites, level, q);
            bn::string<48> line("POT ");
            line += bn::to_string<4>(pot);
            line += " BB";
            t.big.generate(0, -40, line, sprites);
            line = "VILLAIN BETS ";
            line += bn::to_string<4>(bet);
            line += " BB (";
            line += bn::to_string<4>(percent);
            line += "% POT)";
            t.small.generate(0, -18, line, sprites);
            bn::string<48> bet_line = line;
            t.small.generate(0, 30, "EQUITY YOU NEED TO CALL? (%)", sprites);

            bool need_ok;
            int need_answer = 0;

            if(q < 4)
            {
                // Scaffolding: first questions with options, the rest with the number picker.
                int option_values[4];
                int offsets[4] = { -10, -5, 5, 10 };
                int right = random.get_unbiased_int(4);

                // Three distractors around the answer, then sorted so the buttons read low to high.
                for(int i = 0, k = 0; i < 4; ++i)
                {
                    option_values[i] = i == right ? need : need + offsets[k++];
                }

                for(int i = 1; i < 4; ++i)
                {
                    for(int j = i; j > 0 && option_values[j - 1] > option_values[j]; --j)
                    {
                        int tmp = option_values[j];
                        option_values[j] = option_values[j - 1];
                        option_values[j - 1] = tmp;
                    }
                }

                bn::string<6> storage[4];
                const char* labels[4];

                for(int i = 0; i < 4; ++i)
                {
                    storage[i] = bn::to_string<4>(option_values[i]);
                    storage[i] += "%";
                    labels[i] = storage[i].c_str();
                }

                show_control_hint = q < 2;
                need_answer = option_values[ask_options(t, labels, 4)];
                need_ok = need_answer == need;
            }
            else
            {
                need_answer = ask_number(t, "%", 30, 0, 100);
                int diff = need_answer > need ? need_answer - need : need - need_answer;
                need_ok = diff <= 2;
            }

            // Call or fold with a given equity near the threshold (EDGE spots later in the level).
            int gap = q < 5 ? 6 + random.get_unbiased_int(6) : 2 + random.get_unbiased_int(3);
            int equity = random.get_bool() ? need + gap : need - gap;
            equity = equity < 1 ? 1 : equity;
            sprites.clear();
            draw_header(t, sprites, level, q);
            line = "POT ";
            line += bn::to_string<4>(pot);
            line += " BB";
            t.big.generate(0, -40, line, sprites);
            t.small.generate(0, -18, bet_line, sprites);
            line = "YOUR EQUITY: ";
            line += bn::to_string<4>(equity);
            line += "%";
            t.big.generate(0, 0, line, sprites);
            t.small.generate(0, 30, "CALL OR FOLD?", sprites);
            constexpr const char* call_fold[2] = { "FOLD", "CALL" };
            bool call = ask_options(t, call_fold, 2) == 1;
            bool decision_ok = call == (equity >= need);

            bool ok = need_ok && decision_ok;
            correct += ok;
            test_correct = correct;
            bn::sound_items::blip.play();

            sprites.clear();
            draw_header(t, sprites, level, q);
            show_verdict(t, sprites, ok, need_ok || decision_ok);
            line = "NEED = ";
            line += bn::to_string<4>(bet);
            line += " / (";
            line += bn::to_string<4>(pot);
            line += " + ";
            line += bn::to_string<4>(bet);
            line += " + ";
            line += bn::to_string<4>(bet);
            line += ") = ";
            line += bn::to_string<4>(need);
            line += "%";
            t.small.generate(0, 48, line, sprites);
            line = equity >= need ? "EQUITY ABOVE NEED: CALL" : "EQUITY BELOW NEED: FOLD";
            t.small.generate(0, 60, line, sprites);
            line = "YOU: ";
            line += bn::to_string<4>(need_answer);
            line += need_ok ? "% OK   " : "% X   ";
            line += call ? "CALL" : "FOLD";
            line += decision_ok ? " OK   A: NEXT" : " X   A: NEXT";
            t.small.generate(0, 72, line, sprites);
            wait_for_a();
        }

        last_result = finish(level, correct);
        return scene::results;
    }

    // ---------------------------------------------------------------------------------------------------------
    // World 1: heads-up push/fold against the exact Nash tables (source E).

    // "+0.35" style text from hundredths of a big blind.
    bn::string<12> bb_text(int centi, bool sign)
    {
        bn::string<12> text;
        int a = centi < 0 ? -centi : centi;

        if(centi < 0)
        {
            text += '-';
        }
        else if(sign)
        {
            text += '+';
        }

        text += bn::to_string<6>(a / 100);
        text += '.';
        int frac = a % 100;

        if(frac < 10)
        {
            text += '0';
        }

        text += bn::to_string<4>(frac);
        return text;
    }

    // Difficulty bucket of a hand by |EV difference| (DISENO.md section 2): 0 CLEAR, 1 MEDIUM, 2 EDGE.
    constexpr int ev_bucket(int centi)
    {
        int a = centi < 0 ? -centi : centi;
        return a >= 100 ? 0 : (a >= 25 ? 1 : 2);
    }

    constexpr int pushfold_close_centi = 5;   // |EV| under 0.05 bb or a mixed strategy: both actions are fine

    scene push_fold_scene(texts& t)
    {
        const level_info& level = levels[selected_level];
        bool sb = level.kind == level_kind::push_sb;
        constexpr int stack_bb = 10;
        constexpr int variant = 0;
        int stack_row = pushfold_stack_index(stack_bb * 2);

        card_view hero_views[2];
        card_view villain_views[2];
        bn::vector<bn::sprite_ptr, 48> sprites;
        bn::vector<bn::sprite_ptr, 32> feedback;
        int correct = 0;
        int total_loss = 0;

        for(int hand = 0; hand < level.hands; ++hand)
        {
            // Scaffolding: hands 1-5 CLEAR, 6-14 MEDIUM, 15-20 EDGE.
            int wanted = hand < 5 ? 0 : (hand < 14 ? 1 : 2);
            card a;
            card b;
            int cls = 0;
            int ev = 0;
            int freq = 0;

            for(int attempt = 0; attempt < 200; ++attempt)
            {
                deck d;
                d.shuffle(random);
                a = d.draw();
                b = d.draw();
                cls = hand_class(a, b);
                ev = sb ? sb_push_ev[variant][stack_row][cls] : bb_call_ev[variant][stack_row][cls];
                freq = sb ? sb_push_freq[variant][stack_row][cls] : bb_call_freq[variant][stack_row][cls];

                if(ev_bucket(ev) == wanted)
                {
                    break;
                }
            }

            bool close = (ev < pushfold_close_centi && ev > -pushfold_close_centi) || (freq > 12 && freq < 243);
            test_answer = ev > 0 ? 1 : 0;
            test_hand = hand;

            villain_views[0].show_back(-14, -44);
            villain_views[1].show_back(14, -44);
            hero_views[0].show(t, a, -14, 18);
            hero_views[1].show(t, b, 14, 18);

            sprites.clear();
            t.small.set_left_alignment();
            t.small.generate(-116, -72, bn::string<48>(level.code) + "  " + level.name, sprites);
            t.small.set_right_alignment();
            bn::string<16> counter("HAND ");
            counter += bn::to_string<4>(hand + 1);
            counter += "/";
            counter += bn::to_string<4>(level.hands);
            t.small.generate(116, -60, counter, sprites);
            t.small.set_center_alignment();

            bn::string<32> villain_line(sb ? "BB  10 BB" : "SB  ALL-IN 10 BB");
            bn::string<32> hero_line(sb ? "YOU (SB)  10 BB" : "YOU (BB)  10 BB");
            t.small.generate(64, -44, villain_line, sprites);
            t.small.generate(-70, 18, hero_line, sprites);
            t.small.generate(64, -12, sb ? "POT 1.5 BB" : "POT 11 BB", sprites);
            t.small.generate(70, 18, class_names[cls], sprites);

            constexpr const char* push_labels[2] = { "FOLD", "ALL-IN" };
            constexpr const char* call_labels[2] = { "FOLD", "CALL" };
            show_control_hint = hand < 3;
            bool aggressive = ask_options(t, sb ? push_labels : call_labels, 2) == 1;

            bool right_action = ev > 0;
            bool ok = aggressive == right_action || close;
            int loss = aggressive == right_action ? 0 : (ev < 0 ? -ev : ev);
            correct += ok;
            total_loss += close ? 0 : loss;
            test_correct = correct;
            bn::sound_items::blip.play();

            // Feedback: verdict, EV of the aggressive action against folding, the equilibrium frequency.
            sprites.clear();
            t.small.set_left_alignment();
            t.small.generate(-116, -72, bn::string<48>(level.code) + "  " + level.name, sprites);
            t.small.set_center_alignment();
            t.small.generate(64, -44, villain_line, sprites);
            t.small.generate(-70, 18, hero_line, sprites);
            t.small.generate(70, 18, class_names[cls], sprites);
            feedback.clear();

            const char* verdict = close ? "CLOSE: BOTH OK" : (ok ? "CORRECT!" : "MISTAKE");
            t.big.set_palette_item(ok ? gold_palette : ink_palettes[1]);
            t.big.generate(0, -12, verdict, feedback);
            t.big.set_palette_item(common::variable_8x16_sprite_font.item().palette_item());

            bn::string<48> line(sb ? "ALL-IN EV vs FOLD: " : "CALL EV vs FOLD: ");
            line += bb_text(ev, true);
            line += " BB";
            t.small.generate(0, 44, line, feedback);
            line = "NASH: ";
            line += sb ? "SHOVE " : "CALL ";
            line += bn::to_string<4>((freq * 100 + 127) / 255);
            line += "%";

            if(! ok)
            {
                line += "   YOU LOSE ";
                line += bb_text(loss, false);
                line += " BB";
            }

            t.small.generate(0, 56, line, feedback);
            t.small.generate(0, 70, "A: NEXT", feedback);
            wait_for_a();
            feedback.clear();
        }

        // Pass needs 17/20 and an average loss under 0.10 bb per hand (DISENO.md section 2).
        last_result = finish(level, correct);

        if(total_loss > 10 * level.hands)
        {
            last_result.stars = 0;
        }

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

        case scene::push_fold:
            current = push_fold_scene(t);
            break;

        case scene::math_outs:
            current = math_outs_scene(t);
            break;

        case scene::math_pot_odds:
            current = math_pot_odds_scene(t);
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
