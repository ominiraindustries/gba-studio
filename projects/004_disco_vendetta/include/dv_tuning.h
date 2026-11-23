// DISCO VENDETTA gameplay numbers and the greybox floor. Provisional until DISENO.md fixes them.

#ifndef DV_TUNING_H
#define DV_TUNING_H

#include "bn_fixed.h"

namespace dv
{
    constexpr int tile_size = 16;           // logical tile; drawn as 2x2 BG cells
    constexpr int map_columns = 32;         // logical tiles (512 px, the BG width)
    constexpr int map_rows = 19;

    constexpr bn::fixed player_speed = 1.5;
    constexpr int player_radius = 5;        // collision against walls and hits (generous: body is ~11 px)
    constexpr int aim_repeat_delay = 12;    // frames holding L/R before continuous rotation
    constexpr int aim_repeat_rate = 3;      // frames per 22.5 degree step while held

    constexpr int punch_range = 16;
    constexpr int punch_cooldown = 18;
    constexpr int pistol_ammo = 8;
    constexpr int pistol_cooldown = 10;
    constexpr bn::fixed player_bullet_speed = 6;
    constexpr int gunshot_noise_radius = 160;
    constexpr bn::fixed throw_speed = 4;
    constexpr int throw_frames = 30;
    constexpr int stun_frames = 60;

    constexpr bn::fixed thug_speed = 1.2;
    constexpr bn::fixed patrol_speed = 0.5;
    constexpr int vision_range = 112;
    constexpr int reaction_frames = 18;     // "!" shown before an enemy acts (rule 5)
    constexpr int thug_windup = 10;
    constexpr int thug_reach = 14;
    constexpr int gunman_fire_interval = 45;
    constexpr bn::fixed enemy_bullet_speed = 3;

    constexpr int kill_points = 100;
    constexpr int combo_window = 120;

    // Greybox floor 1. '#' wall, '.' floor, 'P' player, 'T' thug, 'G' gunman, 'g' pistol on the floor.
    constexpr const char* floor_1[map_rows] = {
        "################################",
        "#P.....#...........#...........#",
        "#......#...........#.....G.....#",
        "#......#....T......#...........#",
        "#..g...............#...........#",
        "#......#...........#...........#",
        "#......#...........#####..######",
        "####.###...........#...........#",
        "#......#####..######...........#",
        "#..........................T...#",
        "#......#####..######...........#",
        "#......#...........#...........#",
        "#..T...#.....G.....#####..######",
        "#......#...........#...........#",
        "####.####..........#.....G.....#",
        "#..................#...........#",
        "#..G.....................T.....#",
        "#..................#...........#",
        "################################",
    };

    constexpr int const_length(const char* text)
    {
        int length = 0;

        while(text[length])
        {
            ++length;
        }

        return length;
    }

    constexpr bool floor_rows_ok()
    {
        for(const char* row : floor_1)
        {
            if(const_length(row) != map_columns)
            {
                return false;
            }
        }

        return true;
    }

    static_assert(floor_rows_ok(), "every floor row must be map_columns wide");
}

#endif
