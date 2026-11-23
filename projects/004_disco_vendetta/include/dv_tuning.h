// DISCO VENDETTA gameplay numbers (DISENO.md section 2) and floor 1 "LOBBY" (section 5).
// Angles are degrees in [0, 360), clockwise from east (screen y grows down).

#ifndef DV_TUNING_H
#define DV_TUNING_H

#include "bn_fixed.h"

namespace dv
{
    constexpr int tile_size = 16;           // logical tile; drawn as 2x2 BG cells
    constexpr int map_columns = 36;
    constexpr int map_rows = 22;
    constexpr int canvas_cells_x = 96;      // big BG canvas: 768x512 px (multiples of 256)
    constexpr int canvas_cells_y = 64;

    // Player.
    constexpr bn::fixed player_speed = 1.5;             // diagonal: x0.7071 = 1.06 per axis
    constexpr int player_wall_half = 5;                 // 10x10 wall box
    constexpr int player_hit_half = 4;                  // 8x8 damage box
    constexpr int corner_correction = 3;                // px slid around wall corners
    constexpr bn::fixed aim_tap_step = 5.625;           // first press turns at once
    constexpr bn::fixed aim_hold_speed = 6;             // degrees per frame while held (180 in 30 f)
    constexpr int aim_hold_delay = 6;                   // frames held before continuous turning
    constexpr int camera_lead_x = 40;
    constexpr int camera_lead_y = 24;

    // Weapons.
    constexpr int punch_reach = 20;                     // 14 px reach + half the 12 px enemy box
    constexpr int punch_half_arc = 45;
    constexpr int punch_cooldown = 20;
    constexpr int pistol_cooldown = 12;
    constexpr int pistol_max_ammo = 8;
    constexpr int floor_pistol_ammo = 4;                // pistol on the floor of room B
    constexpr int dropped_pistol_ammo = 4;              // a gunman's pistol
    constexpr bn::fixed player_bullet_speed = 6;
    constexpr bn::fixed enemy_bullet_speed = 4;
    constexpr int assist_half_angle = 22;               // +-22.5 degrees
    constexpr int assist_range = 120;
    constexpr bn::fixed throw_speed = 5;
    constexpr int throw_frames = 24;                    // 120 px
    constexpr int finish_range = 16;
    constexpr int finish_frames = 16;

    // Knockouts.
    constexpr int knock_punch = 180;
    constexpr int knock_throw = 90;
    constexpr int knock_door = 120;
    constexpr int knock_shake = 30;                     // last frames before getting up
    constexpr int knock_trail_interval = 20;            // a blood drop while knocked

    // Noise.
    constexpr int noise_pistol = 192;
    constexpr int noise_door = 64;

    // Enemies.
    constexpr bn::fixed goon_patrol_speed = 0.75;
    constexpr bn::fixed goon_chase_speed = 1.25;
    constexpr bn::fixed gunman_patrol_speed = 0.5;
    constexpr bn::fixed investigate_speed = 1;
    constexpr bn::fixed enemy_turn_speed = 8;           // drawn angle interpolation, degrees per frame
    constexpr int vision_range = 112;
    constexpr int vision_half_angle = 45;
    constexpr int vision_near = 24;                     // 360 degrees this close
    constexpr int gunman_aim_frames = 30;               // "!" while aiming
    constexpr int gunman_flash_frames = 10;             // muzzle flash in the last aim frames
    constexpr int gunman_shot_gap = 20;                 // + 30 aim = 50 f between shots
    constexpr int goon_alert_frames = 20;
    constexpr int goon_windup_distance = 20;
    constexpr int goon_windup_frames = 18;
    constexpr int goon_hit_reach = 20;                  // 16 px + half the 8 px player box
    constexpr int goon_recover_frames = 20;
    constexpr int offscreen_margin = 8;                 // enemies only fire inside the camera rect minus this
    constexpr int enemy_hit_half = 6;                   // 12x12 damage box
    constexpr int enemy_wall_half = 5;

    // Score (section 4).
    constexpr int goon_points = 300;
    constexpr int gunman_points = 500;
    constexpr int combo_window = 150;
    constexpr int combo_max = 8;
    constexpr int style_door = 200;
    constexpr int style_throw = 200;
    constexpr int style_finish = 100;
    constexpr int par_seconds = 60;
    constexpr int time_bonus_per_second = 50;
    constexpr int no_shot_bonus = 2000;

    // Flow (section 6).
    constexpr int card_frames = 45;
    constexpr int death_slowmo_frames = 30;
    constexpr int death_retry_frame = 20;
    constexpr int retry_fade_frames = 10;
    constexpr int pointer_delay = 600;                  // arrow to the nearest enemy after 10 s without a kill

    // Gore (greybox).
    constexpr int blood_particles = 24;
    constexpr int hitstop_frames = 3;
    constexpr int hit_flash_frames = 2;

    enum class patrol_axis { none, horizontal, vertical };

    struct enemy_spawn
    {
        char id;
        bool gunman;
        int facing;             // degrees
        patrol_axis axis;
        int from;               // patrol range in tiles (column for horizontal, row for vertical)
        int to;
    };

    constexpr enemy_spawn floor_1_enemies[] = {
        { '1', false, 0, patrol_axis::none, 0, 0 },             // room B, back to the door
        { '2', false, 0, patrol_axis::horizontal, 8, 16 },      // row 14, crosses the door of C
        { '3', true, 0, patrol_axis::none, 0, 0 },              // behind the bar in D
        { '4', false, 90, patrol_axis::none, 0, 0 },            // kitchen E
        { '5', false, 0, patrol_axis::horizontal, 19, 27 },     // office F, row 10
        { '6', false, 90, patrol_axis::vertical, 4, 11 },       // office F, column 32
        { '7', true, 90, patrol_axis::none, 0, 0 },             // office F, behind the desk
    };

    // '#' wall, '.' floor, 'D' door, 'w' window, '=' bar/table, 'X' exit, 'P' start, 'p' pistol, digits enemies,
    // '>' only marks the facing of enemy 1 in the design sketch (floor).
    constexpr const char* floor_1[map_rows] = {
        "####################################",
        "#......#.........#................X#",
        "#......#.........#............7....#",
        "#..P...D...1>....#..........=====..#",
        "#......#.........#.................#",
        "#......#......p..#..............6..#",
        "########.........#.................#",
        "############D#####.................#",
        "###########...####.................#",
        "###########...####.................#",
        "###########...####.5...............#",
        "###########...####.................#",
        "###########...####.................#",
        "###########wDw##########D###########",
        "#.......2..........#.........#######",
        "#.....=............#.4.......#######",
        "#.....=............#.........#######",
        "#...3.=............D.........#######",
        "#.....=............#.........#######",
        "#.....=............#.........#######",
        "#..................#.........#######",
        "####################################",
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
