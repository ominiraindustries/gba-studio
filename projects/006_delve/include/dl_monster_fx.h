// DELVE monster readability effects (G-34): per-monster palette (distance curve, torch tint, unfaded accents),
// appear / attack flashes (palette fade), backlight halo sprite and idle breathing. Every effect has a switch in
// dl_fx.h; fx_menu is the hidden menu that flips them (hold SELECT + press L).

#ifndef DL_MONSTER_FX_H
#define DL_MONSTER_FX_H

#include "bn_optional.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

namespace bn
{
    class sprite_item;
    class sprite_text_generator;
}

namespace dl
{
    class monster_fx
    {
    public:
        static constexpr int max_slots = 3;     // monsters with their own palette (sprite palettes are scarce)

        // once per frame: begin, apply for every monster sprite on screen, end
        // heavy: the view was redrawn or composed this frame (~0.9-0.99 f): no new slots (~400 ticks each, they
        // wait one frame with the plain palette) and only the cheap per-sprite work
        void begin_frame(bool heavy);
        void apply(int ent, const bn::sprite_item& item, bn::sprite_ptr& sprite, int band);
        void end_frame();

        void telegraph(int ent);    // red double pulse before this monster attacks
        void telegraph_all();
        void clear();               // floor change: drops every slot
        void refresh();             // a switch changed: rebuild palettes and halos next apply

        [[nodiscard]] int slots_used() const;

    private:
        struct slot
        {
            int ent = -1;
            int idle = 0;           // frames without apply (released after a short grace)
            int key = -1;           // palette build key
            int flash = 0;          // appear flash frames left
            int tele = 0;           // telegraph frames left
            int fade_q = -1;        // last fade level written (avoids palette updates)
            int halo_mode = 0;
            bool touched = false;
            bn::fixed base_y, set_y, base_vs, set_vs;
            bool breathing = false;
            bn::optional<bn::sprite_palette_ptr> palette;
            bn::optional<bn::sprite_ptr> halo;
        };

        slot _slots[max_slots];
        bn::optional<bn::sprite_palette_ptr> _halo_palette;
        int _halo_palette_mode = 0;
        int _frame = 0;
        bool _heavy = false;

        void _release(slot& s);
        void _update_palette(slot& s, const bn::sprite_item& item, int band);
        void _update_fade(slot& s);
        void _update_breath(slot& s, bn::sprite_ptr& sprite, int band);
        void _update_halo(slot& s, const bn::sprite_item& item, const bn::sprite_ptr& sprite);
    };

    class fx_menu
    {
    public:
        // true while the menu is open (the game skips its own input that frame)
        bool update(bn::sprite_text_generator& text, monster_fx& mfx);

        [[nodiscard]] bool open() const
        {
            return _open;
        }

    private:
        bn::vector<bn::sprite_ptr, 56> _sprites;
        int _cursor = 0;
        bool _open = false;

        void _draw(bn::sprite_text_generator& text);
    };
}

#endif
