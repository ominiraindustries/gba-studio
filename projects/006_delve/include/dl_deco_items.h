// DELVE wall decoration art: the ONLY place that names the deco_* sprite items (art/LÉEME.md "Adornos de pared").
// Every sprite is 32x64, sprite row v = texture row v (eye at row 32); anchors are the sprite pixel touching the
// wall: centre = wall point - (anchor - (16, 32)) * scale.

#ifndef DL_DECO_ITEMS_H
#define DL_DECO_ITEMS_H

#include "bn_sprite_items_deco_banner_front.h"
#include "bn_sprite_items_deco_banner_left.h"
#include "bn_sprite_items_deco_banner_right.h"
#include "bn_sprite_items_deco_chains_front.h"
#include "bn_sprite_items_deco_chains_left.h"
#include "bn_sprite_items_deco_chains_right.h"
#include "bn_sprite_items_deco_lever_front.h"
#include "bn_sprite_items_deco_lever_left.h"
#include "bn_sprite_items_deco_lever_right.h"
#include "bn_sprite_items_deco_torch_front.h"
#include "bn_sprite_items_deco_torch_glow.h"
#include "bn_sprite_items_deco_torch_left.h"
#include "bn_sprite_items_deco_torch_right.h"

#include "dl_view.h"

namespace dl::deco
{
    enum view_id : int
    {
        VIEW_FRONT,
        VIEW_LEFT,      // wall on the left of the view
        VIEW_RIGHT
    };

    struct art
    {
        const bn::sprite_item* item[3];     // front, left, right
        int8_t anchor_x[3];
        int8_t anchor_y;
        int8_t frames;                      // animation frames (lever: 0 up, 1 down)
        uint8_t frame_ticks[4];             // animation frame durations in game frames
        bool palette_b;                     // banner palette (B); the rest share palette A
    };

    // index = dl::view::deco_kind
    inline constexpr art arts[dl::view::DECO_KIND_COUNT] = {
        { { nullptr, nullptr, nullptr }, { 0, 0, 0 }, 0, 0, { 0, 0, 0, 0 }, false },
        { { &bn::sprite_items::deco_torch_front, &bn::sprite_items::deco_torch_left,
            &bn::sprite_items::deco_torch_right }, { 16, 6, 26 }, 43, 4, { 7, 4, 8, 5 }, false },    // 110/70/140/90 ms
        { { &bn::sprite_items::deco_banner_front, &bn::sprite_items::deco_banner_left,
            &bn::sprite_items::deco_banner_right }, { 16, 9, 23 }, 5, 2, { 30, 30, 0, 0 }, true },
        { { &bn::sprite_items::deco_chains_front, &bn::sprite_items::deco_chains_left,
            &bn::sprite_items::deco_chains_right }, { 16, 9, 23 }, 10, 1, { 0, 0, 0, 0 }, false },
        { { &bn::sprite_items::deco_lever_front, &bn::sprite_items::deco_lever_left,
            &bn::sprite_items::deco_lever_right }, { 16, 9, 23 }, 32, 1, { 0, 0, 0, 0 }, false },     // frame 0
        { { &bn::sprite_items::deco_lever_front, &bn::sprite_items::deco_lever_left,
            &bn::sprite_items::deco_lever_right }, { 16, 9, 23 }, 32, 1, { 0, 0, 0, 0 }, false },     // frame 1
    };

    // torch halo (64x64): its centre goes on this torch sprite pixel; drawn behind the torch, blended
    inline constexpr const bn::sprite_item& torch_glow = bn::sprite_items::deco_torch_glow;
    inline constexpr int8_t glow_x[3] = { 16, 16, 15 };
    constexpr int glow_y = 12;

    // lateral art draws what is parallel to the wall squeezed to 0.32 (art/LÉEME.md)
    constexpr int lateral_squeeze_x100 = 32;

    // palette A colours that are light (flame and its warm glint): they do not fade with distance
    constexpr bool lit_index_a(int i)
    {
        return i == 8 || i >= 12;
    }
}

#endif
