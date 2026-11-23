// HUD option A (HUD.md, G-32): right panel (tape compass, FLOOR, minimap well, keys) and bottom strip (weapon +
// reload, HP bar + number, quick item, message line), plus damage-direction arrows at the view edges.
//
// Where each part lives (cheapest per frame):
//  - static frame, icons, troughs: ui_frame bitmap (blitted by the game with the rest of the page);
//  - bars, key icon, weapon glow: painted into the bitmap only when they change, on BOTH pages (the view only
//    flips on a step, so a value drawn on one page would blink back on the next flip);
//  - text and damage arrows: sprites (bn::sprite_text_generator + common_variable_8x8, ui_arrow).
#ifndef DL_HUD_H
#define DL_HUD_H

#include "bn_palette_bitmap_bg_painter.h"
#include "bn_bg_palette_ptr.h"
#include "bn_color.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

namespace dl
{
    // What the HUD shows. Filled by the game every frame from dl::combat (G-35).
    struct hud_state
    {
        int hp = 30;
        int hp_max = 30;
        int potions = 0;
        int keys = 0;
        int reload = 0;         // 0..reload_max; full = the weapon can strike
        int reload_max = 30;
        int floor = 0;          // 0-based
        int dir = 0;            // 0 N, 1 E, 2 S, 3 W
        // G-40 magic (MAGIA_Y_TESOROS.md 5.2)
        int mp = 0;
        int mp_max = 10;
        bool mp_on = false;     // a spell is known: before that the bar is empty and MP grey
        int belt = 0;           // selected belt entry: 0 potion, 1 + spell_id, -1 empty
        int belt_cost = 0;      // spell MP cost
        bool belt_ok = true;    // enough MP (or a free scroll)
        int belt_sel = 0;
        int belt_mask = 1;      // bit per non-empty belt entry
        bool casting = false;
        int light = 0;          // Light frames left (status icon)
    };

    class hud
    {
    public:
        // Panel geometry the game needs (the minimap is drawn by dl::view into this well).
        static constexpr int minimap_x = 178;
        static constexpr int minimap_y = 31;

        // bg: the game's mode 4 background; its palette gets the UI colours 192..239 and 248..255 of ui_frame.
        explicit hud(bn::palette_bitmap_bg_ptr& bg);

        hud_state state;

        // Call once per frame after the game updated `state` (and after any view redraw of this frame).
        // heavy_frame: the view was redrawn / composed this frame; text changes wait (up to 8 f) for a light one.
        void update(bn::palette_bitmap_bg_painter& painter, bool heavy_frame);

        // The game repainted the whole frame (ui_frame blit): repaint every dynamic part on the next update.
        void invalidate();

        // Hide / show every HUD sprite (e.g. the DESCENDING screen).
        void set_visible(bool visible);

        // G-40 pause: the right panel (compass, floor, keys, status icons) is covered by the pause bitmap
        void set_panel_visible(bool visible);

        // One line, up to 28 characters; frames <= 0 keeps it until the next message.
        void message(const char* text, int frames = 180);
        void clear_message();

        // Damage-direction arrow at the left (side < 0) or right (side > 0) edge of the view (ESPEC_VISTA).
        void show_damage_arrow(int side, int frames = 48);

        // Measured cost of the last update() / worst so far (timer ticks; bn::timers::ticks_per_frame() = 1 f).
        int last_ticks() const { return _last_ticks; }
        int max_ticks() const { return _max_ticks; }
        int max_paint_ticks() const { return _max_paint_ticks; }  // bitmap part (bars, belt, dots)
        int max_pre_ticks() const { return _max_pre_ticks; }      // text + sprites part

        // Budget report: HUD sprites (OAM) and sprite VRAM tiles (32 bytes each) in use right now.
        int sprites_count() const;
        int sprite_tiles_count() const;

    private:
        bn::sprite_text_generator _text;
        bn::sprite_palette_ptr _grey_palette;
        bn::sprite_palette_ptr _gold_palette;
        bn::sprite_palette_ptr _white_palette;     // G-40: every HUD text palette is held from the start, so
        bn::sprite_palette_ptr _red_palette;       // world sprites made first can never leave the HUD without one
        bn::sprite_palette_ptr _blue_palette;
        bn::vector<bn::sprite_ptr, 4> _compass_sprites;     // [N, E, S, W]
        int _compass_dx[4] = {};
        bn::vector<bn::sprite_ptr, 2> _floor_sprites;
        bn::vector<bn::sprite_ptr, 1> _hp_label_sprites;
        bn::vector<bn::sprite_ptr, 2> _hp_sprites;
        bn::vector<bn::sprite_ptr, 1> _key_sprites;
        bn::vector<bn::sprite_ptr, 2> _potion_sprites;
        bn::vector<bn::sprite_ptr, 12> _message_sprites;
        bn::vector<bn::sprite_ptr, 2> _arrow_sprites;
        bn::vector<bn::sprite_ptr, 1> _mp_label_sprites;
        bn::vector<bn::sprite_ptr, 2> _mp_sprites;
        bn::vector<bn::sprite_ptr, 1> _belt_icon;
        bn::vector<bn::sprite_ptr, 1> _status_icon;
        bn::bg_palette_ptr _bg_palette;
        bn::color _ring_color;
        int _drawn_mp_w = -1;
        int _mp_anim_w = 0;         // shown MP bar width (fills in ~30 f when the first spell is learned)
        int _drawn_belt = -2;
        int _drawn_dots = -1;
        int _drawn_ring = -1;
        int _drawn_label_on = -1;
        bool _panel_visible = true;
        int _arrow_timer[2] = { 0, 0 };

        hud_state _shown;           // what the text sprites show
        int _drawn_hp_w = -1;       // what the bitmap shows (-1 = repaint)
        int _drawn_hp_blink = -1;
        int _drawn_reload_w = -1;
        int _drawn_keys = -1;
        int _glow_timer = 0;
        bool _glow_drawn = false;
        bool _text_dirty = true;
        bool _visible = true;
        int _compass_anim = 0;      // tape slide after a turn, frames left
        int _compass_from = 0;      // +1 turned right, -1 turned left
        int _message_timer = 0;
        int _frame = 0;
        int _last_ticks = 0;
        int _max_ticks = 0;
        int _max_paint_ticks = 0;
        bool _text_job = false;     // a text job ran this frame
        bool _heavy_now = false;
        int _belt_delay = 0;
        int _paint_delay = 0;
        int _message_pos = 0;      // next character of _pending_message to generate (chunked)
        int _message_x = 0;
        int _max_pre_ticks = 0;
        int _text_delay = 0;
        bn::string<32> _pending_message;
        int _pending_frames = 0;
        bool _has_pending_message = false;

        void _update_compass();
        void _rebuild_text();
        void _flush_message();
        void _place_compass();
        void _paint_bars(bn::palette_bitmap_bg_painter& painter);
        static void _sync(bn::palette_bitmap_bg_painter& painter, int x0, int y0, int x1, int y1);
    };
}

#endif
