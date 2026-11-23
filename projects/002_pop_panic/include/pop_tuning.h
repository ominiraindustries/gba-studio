// Gameplay numbers and stage table for POP PANIC. Provisional values until DISENO.md fixes them.

#ifndef POP_TUNING_H
#define POP_TUNING_H

#include "bn_fixed.h"

namespace pop
{
    constexpr int screen_width = 240;
    constexpr int floor_y = 144;            // top of the floor strip; the playfield is 240x144

    constexpr int size_count = 4;           // 0: 48 px, 1: 32 px, 2: 16 px, 3: 8 px
    constexpr int ball_diameter[size_count] = { 48, 32, 16, 8 };
    constexpr bn::fixed ball_gravity = 0.1;
    constexpr bn::fixed ball_speed_x = 1;
    // Fixed rebound speed per size, so each size always bounces to the same height: v = sqrt(2 * g * h).
    constexpr bn::fixed ball_bounce_speed[size_count] = { 4.47, 4.0, 3.35, 2.68 };   // h ~ 100, 80, 56, 36 px
    constexpr bn::fixed ball_split_hop = 2.5;
    constexpr int ball_points[size_count] = { 50, 100, 150, 200 };

    constexpr bn::fixed player_speed = 1.5;
    constexpr int player_hit_width = 10;    // generous to the player: smaller than the 16x24 body
    constexpr int player_hit_height = 18;
    constexpr bn::fixed harpoon_speed = 3;
    constexpr int start_lives = 3;

    constexpr int ready_frames = 100;
    constexpr int dying_frames = 70;
    constexpr int clear_frames = 100;

    struct ball_spawn
    {
        int size;
        int x;
        int y;
        int dir;    // -1 left, 1 right
    };

    struct stage_info
    {
        int city;
        int count;
        ball_spawn balls[4];
    };

    constexpr const char* city_names[10] = {
        "MADRID", "PARIS", "LONDON", "ROME", "CAIRO", "MOSCOW", "NEW DELHI", "BEIJING", "TOKYO", "WASHINGTON D.C.",
    };

    constexpr int stage_count = 20;
    constexpr stage_info stages[stage_count] = {
        { 0, 1, { { 1, 60, 50, 1 } } },
        { 0, 1, { { 0, 70, 50, 1 } } },
        { 1, 2, { { 1, 50, 50, 1 }, { 1, 190, 50, -1 } } },
        { 1, 2, { { 0, 60, 50, 1 }, { 2, 190, 60, -1 } } },
        { 2, 2, { { 0, 50, 50, 1 }, { 1, 190, 50, -1 } } },
        { 2, 2, { { 0, 50, 50, 1 }, { 0, 190, 50, -1 } } },
        { 3, 3, { { 1, 40, 40, 1 }, { 1, 120, 60, -1 }, { 1, 200, 40, -1 } } },
        { 3, 2, { { 0, 60, 40, 1 }, { 1, 180, 60, -1 } } },
        { 4, 3, { { 0, 50, 50, 1 }, { 2, 120, 40, 1 }, { 1, 190, 50, -1 } } },
        { 4, 2, { { 0, 50, 50, 1 }, { 0, 190, 50, -1 } } },
        { 5, 4, { { 1, 30, 40, 1 }, { 1, 90, 60, 1 }, { 1, 150, 60, -1 }, { 1, 210, 40, -1 } } },
        { 5, 3, { { 0, 50, 50, 1 }, { 1, 120, 40, -1 }, { 0, 190, 50, -1 } } },
        { 6, 3, { { 0, 40, 50, 1 }, { 0, 120, 40, -1 }, { 1, 200, 60, -1 } } },
        { 6, 3, { { 0, 50, 50, 1 }, { 0, 190, 50, -1 }, { 1, 120, 30, 1 } } },
        { 7, 4, { { 0, 40, 50, 1 }, { 1, 100, 40, 1 }, { 1, 140, 40, -1 }, { 0, 200, 50, -1 } } },
        { 7, 3, { { 0, 40, 50, 1 }, { 0, 120, 40, 1 }, { 0, 200, 50, -1 } } },
        { 8, 4, { { 0, 40, 50, 1 }, { 0, 100, 40, -1 }, { 1, 160, 40, 1 }, { 1, 210, 60, -1 } } },
        { 8, 4, { { 0, 40, 50, 1 }, { 0, 100, 40, 1 }, { 0, 140, 40, -1 }, { 1, 200, 60, -1 } } },
        { 9, 4, { { 0, 30, 50, 1 }, { 0, 90, 40, 1 }, { 0, 150, 40, -1 }, { 0, 210, 50, -1 } } },
        { 9, 4, { { 0, 30, 40, 1 }, { 0, 90, 60, -1 }, { 0, 150, 60, 1 }, { 0, 210, 40, -1 } } },
    };
}

#endif
