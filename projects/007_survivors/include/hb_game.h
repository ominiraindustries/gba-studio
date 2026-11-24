#ifndef HB_GAME_H
#define HB_GAME_H

namespace hb
{
    // Effect toggles of DISENO.md §12 (SELECT menu). Defaults follow the table.
    struct fx_settings
    {
        bool hit_flash = true;
        bool knockback = true;
        bool damage_numbers = false;
        bool screen_shake = true;
        bool death_poof = true;
        bool gem_trail = true;
        bool level_flash = true;
        bool dusk = true;
        bool frost_translucent = true;
        bool boss_mosaic = true;
        bool shadows = true;

        // Debug (hb::debug_menu).
        bool god_mode = false;
    };

    enum class run_end
    {
        retry,
        title
    };

    // Chosen on the stage select screen (hb_meta.cpp).
    struct run_setup
    {
        int character = 0;   // hb::character_id
        int stage = 0;
    };

    // Plays one run (run -> summary). Reads the meta save (shop ranks, unlocks) and writes it at the end of the run.
    // Allocates the run state once on entry (EWRAM heap).
    [[nodiscard]] run_end play_run(unsigned seed, fx_settings& fx, const run_setup& setup);
}

#endif
