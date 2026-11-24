// HEXBOUND (007_survivors): title screen and run loop. The run itself lives in hb_game.cpp.

#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_regular_bg_ptr.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "common_variable_8x8_sprite_font.h"

#include "hb_art.h"
#include "hb_config.h"
#include "hb_game.h"

extern volatile int test_mode;

namespace
{
    // Returns a seed taken from how long the player waited on the title (deterministic for the same input).
    unsigned title_screen()
    {
        // Title art (art/LÉEME.md "Bloque 2"): 8 bpp illustration + HEXBOUND logo in 6 sprite cells.
        // The logo is art: renaming the game means repainting art/logo.aseprite (hb::game_title is the text name).
        bn::regular_bg_ptr title_bg = hb::art::title_bg.create_bg(8, 48);
        bn::sprite_text_generator small(common::variable_8x8_sprite_font);
        small.set_center_alignment();

        bn::vector<bn::sprite_ptr, 6> logo;

        for(int k = 0; k < 6; ++k)
        {
            logo.push_back(hb::art::logo.create_sprite(-64 + 64 * (k % 3), -60 + 32 * (k / 3), k));
        }

        bn::vector<bn::sprite_ptr, 4> press;
        unsigned frames = 0;
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

        hb::art::sfx_level.play(bn::fixed(0.6));
        bn::core::update();   // consume the START press so the run does not open the pause menu
        return frames;
    }
}

int main()
{
    bn::core::init();
    hb::fx_settings fx;
    unsigned seed = title_screen();

    while(true)
    {
        hb::run_end end = hb::play_run(seed, fx);
        ++seed;

        if(end == hb::run_end::title)
        {
            bn::core::update();
            seed += title_screen();
        }
    }
}
