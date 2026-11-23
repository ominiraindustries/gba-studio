// DELVE combat feedback (G-35): floating numbers (damage, MISS, crits), the red view edge when the player is hit and
// the gold flash of a level up. Text only with bn::sprite_text_generator (the game's common_variable_8x8).
#ifndef DL_COMBAT_FX_H
#define DL_COMBAT_FX_H

#include "bn_fixed.h"
#include "bn_sprite_ptr.h"
#include "bn_string.h"
#include "bn_vector.h"

namespace bn
{
    class sprite_text_generator;
}

namespace dl
{
    class combat_fx
    {
    public:
        explicit combat_fx(bn::sprite_text_generator& text);

        // text centred at sprite coordinates (x, y), rising for 30 f. Created on the next light frame (text
        // generation costs a few hundred ticks; a view redraw frame is already at ~0.99 f).
        void number(const char* text, bn::fixed x, bn::fixed y);
        void red_edge(int frames = 6);
        void gold_flash();
        void clear();

        // heavy: the view was redrawn or composed this frame
        void update(bool heavy);

        [[nodiscard]] int last_ticks() const { return _last_ticks; }
        [[nodiscard]] int max_ticks() const { return _max_ticks; }

    private:
        struct popup
        {
            bn::vector<bn::sprite_ptr, 3> sprites;
            int t = 0;
        };

        struct pending
        {
            bn::string<12> text;
            bn::fixed x, y;
        };

        bn::sprite_text_generator& _text;
        bn::vector<popup, 4> _popups;
        bn::vector<pending, 4> _pending;
        bn::vector<bn::sprite_ptr, 20> _edge;
        int _edge_t = 0;
        int _flash_t = 0;
        int _last_ticks = 0;
        int _max_ticks = 0;
    };
}

#endif
