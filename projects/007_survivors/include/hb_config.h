#ifndef HB_CONFIG_H
#define HB_CONFIG_H

// Single place for names shown on screen and for the tuning numbers of DISENO.md (block 1).

#include "bn_fixed.h"

namespace hb
{
    // PROVISIONAL (DISENO.md §18.1, recommended answer): change the on-screen title and hero name here.
    // The ROM header title is ROMTITLE in the Makefile.
    constexpr const char* game_title = "HEXBOUND";
    constexpr const char* hero_name = "Wendel";
    constexpr const char* stage_name = "Mossy Meadow";

    // Debug rows (GOD MODE, SKIP TO 4:50, STRESS 56, LEVEL UP) in the SELECT menu. Set to false for release.
    constexpr bool debug_menu = true;

    constexpr int fps = 60;

    // Player (§2)
    constexpr bn::fixed player_speed = 1;
    constexpr bn::fixed player_accel = bn::fixed(1) / 4;   // 0 -> 1.0 in 4 frames
    constexpr bn::fixed player_decel = bn::fixed(1) / 3;   // 1.0 -> 0 in 3 frames
    constexpr int player_hp = 100;
    constexpr int player_invulnerable_frames = 40;   // 30 -> 40 (DISENO §19.1, G-45.2)
    constexpr int player_half_box = 4;                     // 8x8 hurt box
    constexpr int pickup_radius = 32;                      // §19.4 (was 24)
    constexpr bn::fixed gem_fly_speed = 3;

    // Enemies (§2, §13)
    constexpr int max_enemies = 56;
    constexpr int enemy_half_box = 7;                      // 14x14 box in a 16x16 sprite
    constexpr int spawn_min_distance = 136;
    constexpr int spawn_max_distance = 168;
    constexpr int recycle_distance = 260;
    constexpr int knockback_frames = 6;
    constexpr bn::fixed knockback_speed = bn::fixed(4) / 6; // 4 px in 6 frames
    constexpr int hit_flash_frames = 4;
    constexpr int separation_distance = 12;                 // neighbours closer than this (px, per axis) push apart
    constexpr bn::fixed separation_push = bn::fixed(0.3);  // px per axis and neighbour (max 3) per frame

    // Pools (§13)
    constexpr int max_bolts = 12;
    constexpr int max_runes = 4;
    constexpr int frost_shards = 8;
    constexpr int max_gems = 20;
    constexpr int max_particles = 6;
    constexpr int max_damage_numbers = 6;

    // Timeline (§6, §14)
    constexpr int boss_time = 5 * 60 * fps;
    constexpr int pre_boss_pause = 10 * fps;
    constexpr int shrine_period = 90 * fps;
    constexpr int blessing_frames = 60 * fps;

    // XP (§10)
    [[nodiscard]] constexpr int xp_needed(int level)
    {
        if(level == 1)
        {
            return 3;   // §19.4: first level-up sooner (was 5)
        }

        return level <= 20 ? 5 + 3 * (level - 1) : 5 + 3 * 19 + 6 * (level - 20);
    }
}

#endif
