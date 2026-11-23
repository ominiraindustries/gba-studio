// DELVE visual effect switches: one place to turn each effect on or off (G-27 adds its effects here: player torch
// halo, step zoom, ...). The game reads them every frame, so a debug menu or a test build can flip them.

#ifndef DL_FX_H
#define DL_FX_H

namespace dl::fx
{
    struct switches
    {
        bool torch_flicker = true;      // torch flame animation (deco_torch_* 'flame')
        bool banner_wave = true;        // banner animation ('wave')
        bool torch_glow = true;         // blended halo behind every wall torch (deco_torch_glow)
        bool deco_depth_fade = true;    // wall decorations darken with distance like the walls
        bool lateral_squeeze = true;    // lateral decorations: horizontal scale follows the real foreshortening
        bool smooth_moves = true;       // G-31: real intermediate camera poses for steps, strafes and turns; off =
                                        // the old fakes below (step_zoom / turn_slide / head_bob)
        bool step_zoom = true;          // a step zooms the old view 1.0 -> 1.12 in 5 f (back: the new one 1.12 -> 1.0)
        bool turn_slide = true;         // a turn slides the old view out and the new one in, 6 f
        bool head_bob = true;           // 1-2 px dip during a step (USER DECIDES: on for the first test ROM)

        // G-34 monster readability (dl_monster_fx.h; hidden menu: hold SELECT + press L). USER DECIDES which stay.
        int monster_fade = 1;           // distance darkening: 0 none (art as drawn, the old look), 1 gentle
                                        // (d <= 1 100 %, d = 2 85 %, d = 3 65 %, further 50 %), 2 wall curve (60 / 36 %)
        bool monster_torch_tint = true; // d <= 1: warm lift, as if lit by the torch the player carries
        bool monster_accents = true;    // palette indices 13-15 (eyes, glow) never fade
        bool monster_appear_flash = true;   // short bright flash when a monster first comes into view
        bool monster_attack_flash = true;   // red double pulse before an attack (telegraph; combat calls it)
        int monster_halo = 2;           // 0 off, 1 dark outline, 2 additive backlight (+1 sprite, +1 affine matrix)
        bool monster_breathe = true;    // idle breathing: 3 % vertical scale, feet stay on the floor
    };

    inline switches enabled;
}

#endif
