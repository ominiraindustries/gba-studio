#ifndef HB_META_H
#define HB_META_H

#include "hb_game.h"

namespace hb
{
    // PRESS START screen; returns how many frames the player waited (seed material).
    [[nodiscard]] unsigned title_screen();

    // Main menu (PLAY / SHOP / RECORDS) until a stage and character are chosen.
    bool main_menu(run_setup& setup);

    // E5.4 step 4: 3 ending screens over ending_bg, then the credits (START skips).
    void ending_sequence();
}

#endif
