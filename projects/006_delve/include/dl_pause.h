// DELVE G-40 M1: the START pause with the BAG and SPELLS tabs (MAGIA_Y_TESOROS.md 5.1). Each tab is one mode 4
// palette_bitmap (pause_bag / pause_spells, UI colours only) blitted on open or on a tab change; icons, cursor and
// text are sprites (bn::sprite_text_generator + common_variable_8x8). The bottom strip of the HUD stays as it is and
// its message line describes the item under the cursor. MAP comes with M2 (grey tab).
#ifndef DL_PAUSE_H
#define DL_PAUSE_H

#include "bn_palette_bitmap_bg_painter.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"

namespace dl
{
    class combat;
    class hud;

    class pause_screen
    {
    public:
        [[nodiscard]] bool is_open() const { return _open; }

        void open(bn::palette_bitmap_bg_painter& painter, bn::sprite_text_generator& text, combat& cb, hud& hd);

        // false: closed this frame (the game repaints the dungeon)
        bool update(bn::palette_bitmap_bg_painter& painter, bn::sprite_text_generator& text, combat& cb, hud& hd);

        [[nodiscard]] int sprites_count() const { return _sprites.size() + _cursor_sprite.size(); }

    private:
        struct bag_entry
        {
            int8_t kind;    // 0 potion, 1 key, 2 scroll, 3 ring
            int8_t index;   // scroll: spell_id
            int8_t count;
        };

        bn::vector<bn::sprite_ptr, 56> _sprites;
        bn::vector<bn::sprite_ptr, 1> _cursor_sprite;
        bn::vector<bag_entry, 12> _bag;
        int _tab = 0;           // 0 BAG, 1 SPELLS
        int _cursor = 0;        // BAG: 0..11 grid, 12..15 equipment (weapon, armour, ring, amulet); SPELLS: 0..9
        bool _open = false;

        void _blit(bn::palette_bitmap_bg_painter& painter);
        void _build_bag(combat& cb);
        void _draw(bn::sprite_text_generator& text, combat& cb);
        void _place_cursor();
        void _describe(combat& cb, hud& hd);
    };
}

#endif
