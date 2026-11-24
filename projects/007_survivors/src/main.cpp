// HEXBOUND (007_survivors): title screen and run loop. The run itself lives in hb_game.cpp.

#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_16x16_sprite_font.h"

#include "hb_art.h"
#include "hb_config.h"
#include "hb_game.h"

extern volatile int test_mode;

namespace
{
    // Returns a seed taken from how long the player waited on the title (deterministic for the same input).
    unsigned title_screen()
    {
        bn::bg_palettes::set_transparent_color(bn::color(4, 6, 5));
        bn::sprite_text_generator big(common::variable_16x16_sprite_font);
        bn::sprite_text_generator small(common::variable_8x8_sprite_font);
        big.set_center_alignment();
        small.set_center_alignment();

        bn::vector<bn::sprite_ptr, 24> texts;
        big.generate(0, -50, hb::game_title, texts);
        bn::string<32> sub(hb::hero_name);
        sub.append(" vs the horde");
        small.generate(0, -30, sub, texts);
        small.generate(0, 48, hb::stage_name, texts);

        bn::sprite_ptr wizard = hb::art::wizard.create_sprite(0, 4, hb::art::wizard_idle.first);
        bn::sprite_ptr slime_l = hb::art::slime.item->create_sprite(-40, 12, 0);
        bn::sprite_ptr slime_r = hb::art::slime.item->create_sprite(40, 12, 0);
        slime_r.set_horizontal_flip(true);

        bn::vector<bn::sprite_ptr, 4> press;
        unsigned frames = 0;
        test_mode = 0;

        while(! bn::keypad::start_pressed() && ! bn::keypad::a_pressed())
        {
            ++frames;

            if(frames % 60 == 1)
            {
                press.clear();
                small.generate(0, 30, "PRESS START", press);
            }
            else if(frames % 60 == 40)
            {
                press.clear();
            }

            int hop = (frames >> 3) & 3;
            wizard.set_tiles(hb::art::wizard.tiles_item(), hb::art::wizard_idle.frame(int(frames), true));
            slime_l.set_tiles(hb::art::slime.item->tiles_item(), hop);
            slime_r.set_tiles(hb::art::slime.item->tiles_item(), (hop + 2) & 3);
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
