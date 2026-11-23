// Dungeon themes (G-37): which texture set and palette each floor uses, plus the debug override (FX menu).
// The view pieces of every theme share one geometry (dl_view_motion_data.h); a theme is a pixel bank + a palette
// (tools/build_view.py builds every art/themes/<name>/export it finds). Order and reasons: DISENO.md 6c.
#ifndef DL_THEMES_H
#define DL_THEMES_H

#include "bn_bg_palette_ptr.h"
#include "bn_color.h"

namespace dl::themes
{
    // floor 0, 1, 2 ... -> theme name; floors past the end keep the last one. Missing names fall back to theme 0.
    constexpr const char* floor_order[] = {
        "sewer", "timber_mine", "bone_crypt", "ice_cavern", "root_temple", "mirror_hall", "lava_forge"
    };
    constexpr int floor_order_count = sizeof(floor_order) / sizeof(floor_order[0]);

    // secret / rare floor theme (not placed yet: DISENO.md 6c); reachable from the FX menu
    constexpr const char* secret_theme = "flesh_warren";

    [[nodiscard]] int find(const char* name);       // theme index, -1 when not built
    [[nodiscard]] int for_floor(int floor);
    [[nodiscard]] int current();
    [[nodiscard]] const char* current_name();

    // switches the view pixel bank and the environment colours 1..191 (the UI colours 192..255 are left alone)
    void apply(bn::bg_palette_ptr palette, int theme);

    // forced theme (or the floor's one when not forced) for this floor
    void apply_for_floor(bn::bg_palette_ptr palette, int floor);

    // wall decoration sprite colours of the current theme: which 0 = palette A (torch, chains, lever), 1 = B (banner)
    [[nodiscard]] const bn::color* deco_colors(int which);

    // torch glow light colour of the current theme (5-bit channels, brightest = 31)
    [[nodiscard]] const uint8_t* glow_rgb();

    // debug override from the FX menu: -1 = by floor, else a theme index; changed() is true once after a change
    [[nodiscard]] int forced();
    void cycle_forced(int delta);
    [[nodiscard]] bool take_changed();
}

#endif
