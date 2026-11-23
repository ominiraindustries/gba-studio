// lab_pop_art: shows POP PANIC block-1 art (player frames, balls, harpoon, pop fx) over each city
// background. The background changes every 120 frames (or with L/R).

#include "bn_array.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_bg_london.h"
#include "bn_regular_bg_items_bg_madrid.h"
#include "bn_regular_bg_items_bg_paris.h"
#include "bn_sprite_items_ball_16.h"
#include "bn_sprite_items_ball_32.h"
#include "bn_sprite_items_ball_24.h"
#include "bn_sprite_items_item_double.h"
#include "bn_sprite_items_item_shield.h"
#include "bn_sprite_items_pigeon.h"
#include "bn_sprite_items_ball_8.h"
#include "bn_sprite_items_harpoon.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_pop_fx.h"

volatile int test_scene;

namespace
{
    // Ball palette swap test: same index layout (1-4 base light->dark, 5 outline, 6 highlight).
    constexpr bn::color blue_ball_colors[] = {
        bn::color(31, 0, 31), bn::color(18, 26, 31), bn::color(8, 18, 31), bn::color(4, 10, 26),
        bn::color(2, 4, 16), bn::color(1, 1, 6), bn::color(31, 31, 31), bn::color(0, 0, 0),
        bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0),
        bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0), bn::color(0, 0, 0)
    };
    constexpr bn::sprite_palette_item blue_ball_palette(blue_ball_colors, bn::bpp_mode::BPP_4);

    bn::regular_bg_ptr create_scene(int scene)
    {
        switch(scene)
        {
        case 0:
            return bn::regular_bg_items::bg_madrid.create_bg(8, 48);
        case 1:
            return bn::regular_bg_items::bg_paris.create_bg(8, 48);
        default:
            return bn::regular_bg_items::bg_london.create_bg(8, 48);
        }
    }
}

int main()
{
    bn::core::init();

    int scene = 0;
    bn::optional<bn::regular_bg_ptr> bg = create_scene(scene);
    bn::vector<bn::sprite_ptr, 48> sprites;

    // Player: all 9 frames along the floor (16x16, feet on screen row 151).
    for(int i = 0; i < 9; ++i)
    {
        sprites.push_back(bn::sprite_items::player.create_sprite(-112 + i * 20, 64, i));
    }

    // Items and pigeon frames.
    sprites.push_back(bn::sprite_items::item_double.create_sprite(76, 64));
    sprites.push_back(bn::sprite_items::item_shield.create_sprite(96, 64));
    for(int i = 0; i < 3; ++i)
    {
        sprites.push_back(bn::sprite_items::pigeon.create_sprite(-100 + i * 22, -24, i));
    }

    // Balls 32, 24 (in a 32x32 sprite), 16, 8 and a palette-swapped 32.
    sprites.push_back(bn::sprite_items::ball_32.create_sprite(-84, 16));
    sprites.push_back(bn::sprite_items::ball_24.create_sprite(-44, 16));
    sprites.push_back(bn::sprite_items::ball_16.create_sprite(-14, 16));
    sprites.push_back(bn::sprite_items::ball_8.create_sprite(6, 16));
    bn::sprite_ptr blue = bn::sprite_items::ball_32.create_sprite(40, 16);
    blue.set_palette(blue_ball_palette.create_new_palette());
    sprites.push_back(blue);

    // Pop fx: the 4 frames in a row.
    for(int i = 0; i < 4; ++i)
    {
        sprites.push_back(bn::sprite_items::pop_fx.create_sprite(-96 + i * 36, -58, i));
    }

    // Harpoon: tip plus repeated chain segments down to the floor.
    sprites.push_back(bn::sprite_items::harpoon.create_sprite(104, -64, 0));
    for(int y = -48; y <= 24; y += 16)
    {
        sprites.push_back(bn::sprite_items::harpoon.create_sprite(104, y, 1));
    }

    int timer = 0;
    while(true)
    {
        ++timer;
        bool next = timer >= 120 || bn::keypad::r_pressed();
        bool prev = bn::keypad::l_pressed();
        if(next || prev)
        {
            timer = 0;
            scene = (scene + (prev ? 2 : 1)) % 3;
            bg.reset();
            bg = create_scene(scene);
        }
        test_scene = scene;
        bn::core::update();
    }
}
