#ifndef HB_ART_H
#define HB_ART_H

// Every graphic and sound the game uses is chosen here (frame tables from art/LÉEME.md). To swap art, change the
// include + reference and, if the layout differs, the frame constants next to it. Nothing else names an item.
// Items named tmp_* are PROVISIONAL placeholders from tests/gen_tmp_gfx.py / gen_tmp_sfx.py (no final art yet).

#include "bn_regular_bg_items_ground.h"
#include "bn_regular_bg_tiles_items_props.h"
#include "bn_bg_palette_items_props_palette.h"
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_slime.h"
#include "bn_sprite_items_bat.h"
#include "bn_sprite_items_skeleton.h"
#include "bn_sprite_items_slime_king.h"
#include "bn_sprite_items_gems.h"
#include "bn_sprite_items_coin.h"
#include "bn_sprite_items_chest.h"
#include "bn_sprite_items_heart.h"
#include "bn_sprite_items_icons.h"
#include "bn_sprite_items_shrine.h"
#include "bn_sprite_items_fx_spark.h"
#include "bn_sprite_items_fx_rune.h"
#include "bn_sprite_items_fx_frost.h"
#include "bn_sprite_items_fx_impact.h"
#include "bn_sprite_items_fx_poof.h"
#include "bn_sprite_items_fx_aura.h"
#include "bn_sprite_items_shadow_small.h"
#include "bn_sprite_items_shadow_big.h"
#include "bn_sprite_items_tmp_fx.h"
#include "bn_sprite_items_tmp_card.h"
#include "bn_sprite_items_tmp_wave.h"
#include "bn_sprite_items_tmp_xpbar.h"
#include "bn_sprite_items_tmp_hpbar.h"
#include "bn_sprite_items_tmp_bossbar.h"
#include "bn_sound_items.h"

namespace hb::art
{
    // A run of frames inside a sprite item.
    struct anim
    {
        int first;
        int count;
        int frame_time;   // frames per animation frame

        [[nodiscard]] constexpr int frame(int ticks, bool loop) const
        {
            int i = ticks / frame_time;
            return first + (loop ? i % count : (i < count ? i : count - 1));
        }
    };

    // ---- Map: BG0 ground (512x512, seamless) + BG1 props written chunk by chunk.
    inline constexpr const bn::regular_bg_item& ground = bn::regular_bg_items::ground;
    inline constexpr const bn::regular_bg_tiles_item& props_tiles = bn::regular_bg_tiles_items::props;
    inline constexpr const bn::bg_palette_item& props_palette = bn::bg_palette_items::props_palette;
    constexpr int props_tiles_columns = 256 / 8;

    // Rects from art/props_layout.json (pixels) + solid footprint in cells (sx, sy, sw, sh; sw = 0: walk through).
    struct prop_def
    {
        int x, y, w, h;
        int sx, sy, sw, sh;
    };

    enum prop_id
    {
        prop_tree, prop_pine, prop_arch, prop_pillar, prop_ruin_wall, prop_rock, prop_bush, prop_bush_flowers,
        prop_stump, prop_mushrooms, prop_pillar_stump, prop_rune_stone, prop_tall_grass, prop_flowers, prop_pond,
        prop_crystal, prop_log, prop_rocks_small, prop_count
    };

    constexpr prop_def props[prop_count] = {
        { 0, 0, 56, 64, 2, 6, 3, 2 },       // tree: trunk
        { 56, 0, 48, 64, 2, 6, 2, 2 },      // pine: trunk
        { 104, 0, 64, 64, 0, 4, 2, 4 },     // arch: left leg (right leg added in code, middle is walkable)
        { 168, 0, 32, 64, 1, 5, 2, 3 },     // pillar
        { 0, 64, 64, 40, 0, 2, 8, 3 },      // ruin_wall
        { 64, 64, 32, 32, 0, 1, 4, 3 },     // rock
        { 96, 64, 32, 32, 0, 0, 0, 0 },     // bush
        { 128, 64, 32, 32, 0, 0, 0, 0 },    // bush_flowers
        { 160, 64, 32, 32, 1, 2, 2, 2 },    // stump
        { 192, 64, 32, 32, 0, 0, 0, 0 },    // mushrooms
        { 224, 64, 32, 32, 1, 1, 2, 3 },    // pillar_stump
        { 0, 104, 24, 32, 0, 1, 3, 3 },     // rune_stone
        { 24, 104, 32, 32, 0, 0, 0, 0 },    // tall_grass
        { 56, 104, 32, 32, 0, 0, 0, 0 },    // flowers
        { 88, 104, 32, 32, 0, 0, 0, 0 },    // pond (small, grassy: walk through)
        { 120, 104, 32, 32, 1, 1, 2, 3 },   // crystal (thorns)
        { 152, 104, 32, 32, 0, 1, 4, 2 },   // log
        { 184, 104, 32, 24, 0, 0, 0, 0 },   // rocks_small
    };

    // ---- Wizard 32x32 (feet 1 px above the bottom row).
    inline constexpr const bn::sprite_item& wizard = bn::sprite_items::wizard;
    constexpr anim wizard_idle = { 0, 4, 12 };
    constexpr anim wizard_idle_side = { 4, 4, 12 };
    constexpr anim wizard_walk_down = { 8, 6, 6 };
    constexpr anim wizard_walk_side = { 14, 6, 6 };   // faces right; left = horizontal flip
    constexpr anim wizard_walk_up = { 20, 6, 6 };
    constexpr anim wizard_idle_up = { 20, 1, 12 };    // no idle_up tag: first walk_up frame
    constexpr anim wizard_hurt = { 26, 2, 5 };
    constexpr anim wizard_death = { 28, 7, 8 };
    constexpr int wizard_y_offset = -10;              // sprite centre relative to the 8x8 hurt box centre

    // ---- Monsters. y offset = sprite centre relative to the 14x14 body box centre.
    struct monster_art
    {
        const bn::sprite_item* item;
        anim move;
        anim death;
        int y_offset;
    };

    constexpr monster_art slime = { &bn::sprite_items::slime, { 0, 4, 8 }, { 4, 4, 4 }, 0 };
    constexpr monster_art bat = { &bn::sprite_items::bat, { 0, 4, 6 }, { 4, 3, 5 }, 0 };
    constexpr monster_art skeleton = { &bn::sprite_items::skeleton, { 0, 4, 10 }, { 4, 4, 5 }, -8 };

    inline constexpr const bn::sprite_item& slime_king = bn::sprite_items::slime_king;
    constexpr anim king_idle = { 0, 4, 10 };
    constexpr int king_crouch = 4;
    constexpr anim king_air = { 5, 2, 8 };
    constexpr anim king_land = { 7, 2, 10 };

    // ---- Pickups.
    inline constexpr const bn::sprite_item& gems = bn::sprite_items::gems;
    constexpr int gem_blue = 0;      // 2 frames each
    constexpr int gem_green = 2;
    constexpr int gem_red = 4;
    inline constexpr const bn::sprite_item& coin = bn::sprite_items::coin;
    inline constexpr const bn::sprite_item& chest = bn::sprite_items::chest;
    inline constexpr const bn::sprite_item& heart = bn::sprite_items::heart;
    inline constexpr const bn::sprite_item& shrine = bn::sprite_items::shrine;   // 16x32

    // ---- Icons 16x16 for cards.
    inline constexpr const bn::sprite_item& icons = bn::sprite_items::icons;
    constexpr int icon_spark_bolt = 0;
    constexpr int icon_rune_orbit = 1;
    constexpr int icon_frost_ring = 2;
    constexpr int icon_swift_boots = 3;
    constexpr int icon_heart_amulet = 4;
    constexpr int icon_magnet = 5;
    constexpr int icon_spell_tome = 6;
    constexpr int icon_star = 7;

    // ---- Effects.
    inline constexpr const bn::sprite_item& fx_spark = bn::sprite_items::fx_spark;     // 8x8, 2 frames
    inline constexpr const bn::sprite_item& fx_rune = bn::sprite_items::fx_rune;       // 8x8, 2 frames
    inline constexpr const bn::sprite_item& fx_frost = bn::sprite_items::fx_frost;     // 16x16, grow 0-2
    inline constexpr const bn::sprite_item& fx_impact = bn::sprite_items::fx_impact;   // 8x8, 0-2
    inline constexpr const bn::sprite_item& fx_poof = bn::sprite_items::fx_poof;       // 8x8, 0-3
    inline constexpr const bn::sprite_item& fx_aura = bn::sprite_items::fx_aura;       // 16x16, 2 frames
    inline constexpr const bn::sprite_item& shadow_small = bn::sprite_items::shadow_small;   // 16x8
    inline constexpr const bn::sprite_item& shadow_big = bn::sprite_items::shadow_big;       // 32x16

    // PROVISIONAL (no final art yet): edge arrow, "!" warning, gem trail spark, card, shockwave, bars.
    inline constexpr const bn::sprite_item& tmp_fx = bn::sprite_items::tmp_fx;
    constexpr int tmp_trail = 5;
    constexpr int tmp_arrow_right = 6;
    constexpr int tmp_arrow_down = 7;
    constexpr int tmp_arrow_down_right = 8;
    constexpr int tmp_warning = 9;
    inline constexpr const bn::sprite_item& card = bn::sprite_items::tmp_card;   // 64x64, 0 normal, 1 selected
    inline constexpr const bn::sprite_item& wave = bn::sprite_items::tmp_wave;   // 64x64, 2 frames
    inline constexpr const bn::sprite_item& xp_bar = bn::sprite_items::tmp_xpbar;   // 32x8, frame N = N px filled
    inline constexpr const bn::sprite_item& hp_bar = bn::sprite_items::tmp_hpbar;
    inline constexpr const bn::sprite_item& boss_bar = bn::sprite_items::tmp_bossbar;
    constexpr int bar_width = 32;

    // ---- Sounds (PROVISIONAL tmp_* from tests/gen_tmp_sfx.py).
    inline constexpr const bn::sound_item& sfx_hit = bn::sound_items::tmp_hit;
    inline constexpr const bn::sound_item& sfx_gem = bn::sound_items::tmp_gem;
    inline constexpr const bn::sound_item& sfx_level = bn::sound_items::tmp_level;
    inline constexpr const bn::sound_item& sfx_hurt = bn::sound_items::tmp_hurt;
    inline constexpr const bn::sound_item& sfx_boss = bn::sound_items::tmp_boss;
    inline constexpr const bn::sound_item& sfx_poof = bn::sound_items::tmp_poof;
    inline constexpr const bn::sound_item& sfx_menu = bn::sound_items::blip;
}

#endif
