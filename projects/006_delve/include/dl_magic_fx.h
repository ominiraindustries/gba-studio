// DELVE G-40 M1: what the spells look like in the view (MAGIA_Y_TESOROS.md 1.3-1.4). The magic logic lives in
// dl::combat (dl_magic.cpp); this only places sprites from its state: the Magic Missile dart (fx_missile, scaled with
// the distance like the objects), Heal sparkles rising at the view edges (fx_heal) and the Light flash
// (fx_light_burst, additive like the torch glow). Light's brighter view is a palette change (light_palette).
#ifndef DL_MAGIC_FX_H
#define DL_MAGIC_FX_H

#include "bn_bg_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace dl
{
    struct magic_state;

    class magic_fx
    {
    public:
        // cell / dir: the player's resting cell and facing
        void update(const magic_state& m, int cell, int dir);

        void clear();
        void set_visible(bool visible);

        [[nodiscard]] int sprites_count() const;
        [[nodiscard]] bool missile_shown() const { return ! _missile.empty() && _missile.front().visible(); }

        // view colours 1..191 lifted (~ +12 % and a little more in the dark tones); call after the theme palette
        // is applied. PROVISIONAL: one curve for every depth (MAGIA 1.4 asks halo 100/100/90 % per band).
        static void light_palette(bn::bg_palette_ptr palette);

    private:
        bn::vector<bn::sprite_ptr, 1> _missile;
        bn::vector<bn::sprite_ptr, 4> _sparks;
        bn::vector<bn::sprite_ptr, 1> _flash;
        int _frame = 0;
        bool _visible = true;
    };
}

#endif
