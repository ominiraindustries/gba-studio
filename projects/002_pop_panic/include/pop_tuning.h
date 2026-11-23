// Gameplay numbers and stage table for POP PANIC, from DISENO.md (sections 0-6). Change them there first.

#ifndef POP_TUNING_H
#define POP_TUNING_H

#include "bn_fixed.h"

namespace pop
{
    constexpr int screen_width = 240;
    constexpr int floor_y = 152;            // top of the thin floor strip (8 px, HUD on it); playfield 240x152

    // Balls: 0: 32 px, 1: 24 px, 2: 16 px, 3: 8 px. Same gravity for all; each size always rebounds to its height.
    constexpr int size_count = 4;
    constexpr int ball_diameter[size_count] = { 32, 24, 16, 8 };
    constexpr bn::fixed ball_gravity = 0.0625;
    constexpr bn::fixed ball_speed_x[size_count] = { 0.625, 0.75, 0.875, 1 };
    constexpr bn::fixed ball_bounce_speed[size_count] = { 3.39, 2.92, 2.35, 1.73 };   // 92, 68, 44, 24 px high
    constexpr int ball_apex_y[size_count] = { 44, 72, 100, 124 };                    // centre at the top of a bounce
    constexpr int ball_hurt_shrink[size_count] = { 2, 2, 1, 1 };                     // hurt circle = r - shrink
    constexpr bn::fixed ball_split_hop = 1.75;
    constexpr int ball_points[size_count] = { 20, 40, 80, 160 };
    constexpr int pop_hitstop[size_count] = { 3, 2, 2, 2 };
    constexpr int max_balls = 32;

    constexpr bn::fixed player_speed = 1.5;
    constexpr int player_hit_width = 8;     // generous to the player: smaller than the ~12x15 body
    constexpr int player_hit_height = 12;
    constexpr int player_start_x = 120;

    constexpr int harpoon_start_y = 137;
    constexpr bn::fixed harpoon_speed = 4;
    constexpr int harpoon_half_width = 3;   // 6 px hit width against the ball circle + 1 px

    constexpr int fire_buffer_frames = 6;   // a press up to 6 f early still fires (rule 2)

    constexpr int start_lives = 3;
    constexpr int max_lives = 5;
    constexpr int stage_bonus = 500;        // x stage number
    constexpr int respawn_invulnerable_frames = 60;

    constexpr int card_frames = 90;
    constexpr int card_frames_same_city = 45;
    constexpr int ready_frames = 60;
    constexpr int go_frames = 30;
    constexpr int death_hitstop_frames = 30;
    constexpr int death_fall_frames = 30;
    constexpr int clear_frames = 90;
    constexpr int game_over_frames = 90;

    // Items (block 1: DOUBLE and SHIELD only).
    enum item_type { item_none = -1, item_double = 0, item_shield = 1 };
    constexpr int item_drop_one_in = 6;
    constexpr int item_double_weight = 30;
    constexpr int item_shield_weight = 20;
    constexpr bn::fixed item_fall_speed = 1;
    constexpr int item_floor_frames = 300;
    constexpr int item_blink_frames = 90;
    constexpr int item_pick_margin = 4;

    // PIGEON: crosses high, cuts a harpoon whose rope it touches, dies to the harpoon tip.
    constexpr bn::fixed pigeon_speed = 1.25;
    constexpr int pigeon_warning_frames = 60;
    constexpr int pigeon_first_delay = 300;  // frames after GO (5 s minimum, section 6)
    constexpr int pigeon_repeat_delay = 600;
    constexpr int pigeon_points = 300;

    constexpr int last_stage_of_block = 5;  // block 1 ends here with "TO BE CONTINUED"

    struct ball_spawn
    {
        int size;
        int x;
        int dir;    // -1 left, 1 right
    };

    struct stage_info
    {
        int city;
        bn::fixed speed_factor;
        int player_x;
        int intro_item;     // item dropped by the first pop of the stage, or item_none
        bool pigeons;
        int count;
        ball_spawn balls[8];
    };

    constexpr const char* city_names[10] = {
        "MADRID", "PARIS", "LONDON", "ROME", "CAIRO", "MOSCOW", "NEW DELHI", "BEIJING", "TOKYO", "WASHINGTON D.C.",
    };

    constexpr int stage_count = 20;
    constexpr stage_info stages[stage_count] = {
        { 0, 1,    120, item_none,   false, 1, { { 1, 40, -1 } } },
        { 0, 1,    120, item_double, false, 1, { { 0, 192, 1 } } },
        { 1, 1.05, 120, item_shield, false, 2, { { 1, 40, -1 }, { 2, 216, 1 } } },
        { 1, 1.05, 120, item_none,   false, 2, { { 0, 40, -1 }, { 1, 200, 1 } } },
        { 2, 1.1,  120, item_none,   true,  3, { { 2, 24, -1 }, { 2, 56, -1 }, { 2, 216, 1 } } },
        { 2, 1.1,  120, item_none,   false, 2, { { 0, 192, 1 }, { 1, 40, -1 } } },
        { 3, 1.15, 120, item_none,   false, 2, { { 1, 40, -1 }, { 1, 200, 1 } } },
        { 3, 1.15, 120, item_none,   false, 2, { { 0, 32, -1 }, { 0, 208, 1 } } },
        { 4, 1.2,  120, item_none,   false, 1, { { 0, 200, 1 } } },
        { 4, 1.2,  40,  item_none,   false, 3, { { 0, 120, 1 }, { 1, 24, -1 }, { 1, 216, 1 } } },
        { 5, 1.2,  120, item_none,   false, 4, { { 1, 40, -1 }, { 1, 200, 1 }, { 2, 24, -1 }, { 2, 216, 1 } } },
        { 5, 1.2,  40,  item_none,   false, 3, { { 0, 32, -1 }, { 0, 208, 1 }, { 2, 120, 1 } } },
        { 6, 1.25, 120, item_none,   false, 2, { { 0, 40, -1 }, { 1, 200, 1 } } },
        { 6, 1.25, 40,  item_none,   false, 3, { { 0, 32, -1 }, { 0, 208, 1 }, { 1, 120, 1 } } },
        { 7, 1.25, 120, item_none,   false, 3, { { 1, 24, -1 }, { 1, 64, -1 }, { 1, 216, 1 } } },
        { 7, 1.25, 120, item_none,   true,  4, { { 0, 32, -1 }, { 0, 208, 1 }, { 1, 64, -1 }, { 1, 176, 1 } } },
        { 8, 1.3,  120, item_none,   false, 8, { { 2, 24, -1 }, { 2, 51, 1 }, { 2, 79, -1 }, { 2, 106, 1 },
                                                 { 2, 134, -1 }, { 2, 161, 1 }, { 2, 189, -1 }, { 2, 216, 1 } } },
        { 8, 1.3,  40,  item_none,   false, 3, { { 0, 32, -1 }, { 0, 120, 1 }, { 0, 208, 1 } } },
        { 9, 1.3,  120, item_none,   false, 2, { { 0, 40, -1 }, { 1, 200, 1 } } },
        { 9, 1.3,  120, item_none,   false, 2, { { 0, 40, -1 }, { 0, 200, 1 } } },   // boss placeholder
    };
}

#endif
