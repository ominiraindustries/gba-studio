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
    };

    inline switches enabled;
}

#endif
