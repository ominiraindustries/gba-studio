#include "hb_save.h"

#include "bn_memory.h"
#include "bn_sram.h"

namespace hb
{

namespace
{
    save_data g_save;

    // Bytes covered by each version (fields are only ever appended; index = version).
    constexpr int version_size[save_version + 1] = { 0, int(sizeof(save_data)) };

    [[nodiscard]] uint16_t checksum_of(const save_data& data, int size)
    {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&data);
        unsigned sum = 0;

        // The header (magic, version, checksum) is the first 8 bytes; the checksum covers the rest.
        for(int i = 8; i < size; ++i)
        {
            sum += unsigned(bytes[i]) * unsigned((i & 7) + 1);
        }

        return uint16_t(sum ^ (sum >> 16) ^ 0x5A5A);
    }

    void reset(save_data& data)
    {
        bn::memory::clear(1, data);
        data.magic[0] = 'H';
        data.magic[1] = 'X';
        data.magic[2] = 'B';
        data.magic[3] = '1';
        data.version = save_version;
        data.music_vol = 100;
        data.sfx_vol = 100;
    }
}

// §C4 shop table.
const shop_def shop_defs[shop_count] = {
    { "Might", "+5% damage", 5, { 30, 60, 120, 200, 300 } },
    { "Vitality", "+10 max HP", 5, { 30, 60, 120, 200, 300 } },
    { "Haste", "+4% speed", 5, { 30, 60, 120, 200, 300 } },
    { "Magnet", "+15% pickup", 5, { 30, 60, 120, 200, 300 } },
    { "Greed", "+10% gold", 5, { 30, 60, 120, 200, 300 } },
    { "Growth", "+4% XP", 5, { 40, 80, 160, 250, 350 } },
    { "Armor", "-5% damage taken", 3, { 100, 250, 500, 0, 0 } },
    { "Recovery", "+1 HP every 10 s", 3, { 80, 200, 400, 0, 0 } },
    { "Reroll", "+1 card reroll", 3, { 100, 250, 450, 0, 0 } },
    { "Charged", "Nova starts at 50%", 1, { 150, 0, 0, 0, 0 } },
    { "Revival", "Revive once, 50% HP", 1, { 600, 0, 0, 0, 0 } },
};

save_data& save()
{
    return g_save;
}

bool load_save()
{
    save_data data;
    bn::sram::read(data);
    bool magic_ok = data.magic[0] == 'H' && data.magic[1] == 'X' && data.magic[2] == 'B' && data.magic[3] == '1';
    int version = data.version;

    if(magic_ok && version >= 1 && version <= save_version &&
       data.checksum == checksum_of(data, version_size[version]))
    {
        // Older version: keep its fields, zero the ones it did not have.
        uint8_t* bytes = reinterpret_cast<uint8_t*>(&data);

        for(int i = version_size[version]; i < int(sizeof(save_data)); ++i)
        {
            bytes[i] = 0;
        }

        data.version = save_version;

        for(int s = 0; s < shop_count; ++s)
        {
            if(data.shop_ranks[s] > shop_defs[s].ranks)
            {
                data.shop_ranks[s] = uint8_t(shop_defs[s].ranks);
            }
        }

        if(data.character >= char_count)
        {
            data.character = char_wendel;
        }

        g_save = data;
        return true;
    }

    reset(g_save);
    write_save();
    return false;
}

void write_save()
{
    g_save.version = save_version;
    g_save.checksum = checksum_of(g_save, int(sizeof(save_data)));
    bn::sram::write(g_save);
}

int shop_spent(const save_data& data)
{
    int spent = 0;

    for(int s = 0; s < shop_count; ++s)
    {
        for(int r = 0; r < data.shop_ranks[s]; ++r)
        {
            spent += shop_defs[s].prices[r];
        }
    }

    return spent;
}

const char* unlock_name(int bit_index)
{
    constexpr const char* names[] = { "EMBER TOSS", "STORM CHAIN", "LENS OF REACH", "WARD STONE", "MOON LANCE",
                                      "GRIM GRAVEYARD", "ASHEN KEEP" };
    return bit_index >= 0 && bit_index < 7 ? names[bit_index] : "";
}

const char* character_name(int character)
{
    constexpr const char* names[char_count] = { "Wendel", "Cinda", "Morrow" };
    return names[character];
}

bool character_available(int character)
{
    return character == char_wendel || (character == char_cinda && unlocked(unlock_cinda)) ||
           (character == char_morrow && unlocked(unlock_morrow));
}

}
