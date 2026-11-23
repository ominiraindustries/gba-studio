// 006_delve: Butano starter. A sprite moved with the d-pad over a background, text from Butano's
// sprite text generator and a sound effect on A.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_backdrop.h"
#include "bn_sprite_items_hero.h"

#include "common_variable_8x8_sprite_font.h"

// Mirrors of the game state for headless tests (tools/scripts/gbarun.py --check test_player_x.32>=...).
// Global, non-static and volatile so the symbol stays in the .elf and the stores are not optimised away.
volatile int test_player_x;
volatile int test_player_y;
volatile int test_sounds_played;

namespace
{
    constexpr int min_x = -112;
    constexpr int max_x = 112;
    constexpr int min_y = -72;
    constexpr int max_y = 72;

    [[nodiscard]] constexpr int clamp(int value, int low, int high)
    {
        return value < low ? low : (value > high ? high : value);
    }
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr backdrop = bn::regular_bg_items::backdrop.create_bg(8, 48);
    bn::sprite_ptr hero = bn::sprite_items::hero.create_sprite(0, 16);

    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    text_generator.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> text_sprites;
    text_generator.generate(0, -64, "DELVE", text_sprites);
    text_generator.generate(0, -48, "D-pad: move   A: sound", text_sprites);

    while(true)
    {
        int x = hero.x().right_shift_integer();
        int y = hero.y().right_shift_integer();

        if(bn::keypad::left_held())
        {
            --x;
        }
        else if(bn::keypad::right_held())
        {
            ++x;
        }

        if(bn::keypad::up_held())
        {
            --y;
        }
        else if(bn::keypad::down_held())
        {
            ++y;
        }

        x = clamp(x, min_x, max_x);
        y = clamp(y, min_y, max_y);
        hero.set_position(x, y);

        if(bn::keypad::a_pressed())
        {
            bn::sound_items::blip.play();
            test_sounds_played = test_sounds_played + 1;
        }

        test_player_x = x;
        test_player_y = y;
        bn::core::update();
    }
}
