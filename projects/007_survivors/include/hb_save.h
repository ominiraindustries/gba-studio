#ifndef HB_SAVE_H
#define HB_SAVE_H

// HEXBOUND meta progress (DISENO_CONTENIDO.md §C4): gold, shop ranks, unlocks, records. Saved with bn::sram as one
// versioned struct; written at the end of a run, after buying / refunding and never in the middle of a run.

#include "bn_common.h"
#include "bn_fixed.h"

namespace hb
{
    enum shop_id : uint8_t
    {
        shop_might,
        shop_vitality,
        shop_haste,
        shop_magnet,
        shop_greed,
        shop_growth,
        shop_armor,
        shop_recovery,
        shop_reroll,
        shop_charged,
        shop_revival,
        shop_count
    };

    // unlock_bits (§C4 "Desbloqueos").
    enum unlock_bit : uint32_t
    {
        unlock_ember = 1u << 0,      // survive 5:00
        unlock_chain = 1u << 1,      // beat the Slime King
        unlock_lens = 1u << 2,       // 500 kills in total
        unlock_ward = 1u << 3,       // beat the Orc Warchief
        unlock_lance = 1u << 4,      // survive 10:00
        unlock_stage2 = 1u << 5,     // clear stage 1
        unlock_stage3 = 1u << 6,     // clear stage 2
        unlock_cinda = 1u << 7,      // bought in the shop (needs stage 1 cleared)
        unlock_morrow = 1u << 8,     // bought in the shop (needs stage 2 cleared)
        unlock_new_bits = 5          // the first 5 bits are the run unlocks shown as "NEW: ..." in the summary
    };

    enum character_id : uint8_t
    {
        char_wendel,
        char_cinda,
        char_morrow,
        char_count
    };

    struct stage_record
    {
        uint16_t best_time_s;
        uint16_t best_kills;
        uint8_t best_level;
        uint8_t best_hexes;
        uint16_t clears;
    };

    struct save_data
    {
        char magic[4];
        uint16_t version;
        uint16_t checksum;
        uint32_t gold;
        uint32_t gold_total;
        uint8_t shop_ranks[shop_count];
        uint8_t pad0;
        uint32_t unlock_bits;
        uint16_t relic_bits;
        uint16_t boss_bits;
        uint8_t stages_cleared;
        uint8_t pad1[3];
        stage_record records[3];
        uint32_t kills_total;
        uint16_t runs;
        uint16_t deaths;
        uint16_t fx_bits;        // OPTIONS: option_bit (0 = defaults, shake and flash ON)
        uint8_t music_vol;
        uint8_t sfx_vol;
        uint8_t character;
        uint8_t last_stage;
        uint8_t pad2[2];
    };

    constexpr int save_version = 1;

    enum option_bit : uint16_t
    {
        option_no_shake = 1u << 0,
        option_no_flash = 1u << 1
    };

    struct shop_def
    {
        const char* name;
        const char* effect;
        int ranks;
        int prices[5];
    };

    extern const shop_def shop_defs[shop_count];

    // Global meta state (one copy in EWRAM), loaded once at boot.
    save_data& save();

    // Reads SRAM; a bad magic / checksum or an unknown version starts a new save (never hangs).
    // Older versions keep their fields and get the new ones at 0. Returns false if a new save was started.
    bool load_save();

    void write_save();

    // OPTIONS "Erase save": a new save (defaults) written at once.
    void erase_save();

    // Music volume (0-1) for bn::music_item::play: the art level times the OPTIONS switch.
    [[nodiscard]] inline bn::fixed music_volume(int art_pct)
    {
        return bn::fixed(art_pct * int(save().music_vol) / 100) / 100;
    }

    [[nodiscard]] inline bool sfx_on()
    {
        return save().sfx_vol != 0;
    }

    // Gold spent on the shop ranks bought so far (REFUND gives it all back).
    [[nodiscard]] int shop_spent(const save_data& data);

    [[nodiscard]] inline bool unlocked(uint32_t bit)
    {
        return (save().unlock_bits & bit) != 0;
    }

    [[nodiscard]] const char* unlock_name(int bit_index);
    [[nodiscard]] const char* character_name(int character);
    [[nodiscard]] bool character_available(int character);
}

#endif
