// HEXBOUND (007_survivors): boot, title, menus and the run loop. Menus live in hb_meta.cpp, the run in hb_game.cpp.

#include "bn_core.h"

#include "hb_game.h"
#include "hb_meta.h"
#include "hb_save.h"

int main()
{
    bn::core::init();
    hb::load_save();
    hb::fx_settings fx;
    unsigned seed = hb::title_screen();

    while(true)
    {
        hb::run_setup setup;
        hb::main_menu(setup);

        while(true)
        {
            hb::run_end end = hb::play_run(seed, fx, setup);
            ++seed;

            if(end == hb::run_end::title)
            {
                bn::core::update();
                break;
            }
        }
    }
}
