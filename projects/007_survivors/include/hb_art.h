#ifndef HB_ART_H
#define HB_ART_H

// Every graphic and sound the game uses is chosen here (frame tables from art/LÉEME.md). To swap art, change the
// include + reference and, if the layout differs, the frame constants next to it. Nothing else names an item.
// Items named tmp_* are PROVISIONAL placeholders from tests/gen_tmp_gfx.py / gen_tmp_sfx.py (no final art yet).

#include "bn_regular_bg_items_ground.h"
#include "bn_regular_bg_tiles_items_props.h"
#include "bn_bg_palette_items_props_palette.h"
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_wizard_ghost.h"
#include "bn_sprite_items_slime.h"
#include "bn_sprite_items_bat.h"
#include "bn_sprite_items_skeleton.h"
#include "bn_sprite_items_slime_king.h"
#include "bn_sprite_items_goblin.h"
#include "bn_sprite_items_orc.h"
#include "bn_sprite_items_orc_warchief.h"
#include "bn_sprite_items_treant.h"
#include "bn_sprite_items_fx_root.h"
#include "bn_sprite_items_fx_thorn.h"
#include "bn_sprite_items_fx_seed.h"
#include "bn_sprite_items_wolf.h"
#include "bn_sprite_items_big_slime.h"
#include "bn_sprite_items_fx_ember.h"
#include "bn_sprite_items_fx_explosion.h"
#include "bn_sprite_items_fx_fire.h"
#include "bn_sprite_items_fx_chain.h"
#include "bn_sprite_items_fx_dust.h"
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
#include "bn_sprite_items_fx_nova.h"
#include "bn_sprite_items_fx_nova_spark.h"
#include "bn_sprite_items_fx_blink.h"
#include "bn_sprite_items_hud_nova.h"
#include "bn_sprite_items_tmp_fx.h"
#include "bn_sprite_items_tmp_wave.h"
#include "bn_sprite_items_hud_bossbar.h"
#include "bn_sprite_items_hud_xp.h"
#include "bn_sprite_items_hud_xp_cap.h"
#include "bn_sprite_items_hud_hp.h"
#include "bn_sprite_items_hud_plaque.h"
#include "bn_sprite_items_hud_slots.h"
#include "bn_sprite_items_hud_arrow.h"
#include "bn_sprite_items_hud_alert.h"
#include "bn_sprite_items_fx_levelup.h"
#include "bn_sprite_items_card.h"
#include "bn_sprite_items_card_base.h"
#include "bn_sprite_items_logo.h"
#include "bn_regular_bg_items_title_bg.h"
#include "bn_sprite_palette_items_text_ink.h"
#include "bn_sprite_palette_items_text_gold.h"
#include "bn_sprite_palette_items_text_red.h"
#include "bn_sprite_items_ghost.h"
#include "bn_sprite_items_skeleton_big.h"
#include "bn_sprite_items_bone_colossus.h"
#include "bn_sprite_items_banshee.h"
#include "bn_sprite_items_lich.h"
#include "bn_sprite_items_fx_arrow.h"
#include "bn_sprite_items_fx_bone.h"
#include "bn_sprite_items_fx_skull.h"
#include "bn_sprite_items_fx_grave.h"
#include "bn_sprite_palette_items_rotten_palette.h"
#include "bn_sprite_palette_items_crow_palette.h"
#include "bn_sprite_palette_items_grey_flesh_palette.h"
#include "bn_sprite_palette_items_red_bone_palette.h"
#include "bn_sprite_palette_items_violet_palette.h"
#include "bn_bg_palette_items_ground_graveyard_palette.h"
#include "bn_bg_palette_items_props_graveyard_palette.h"
#include "bn_sound_items.h"
#include "bn_music_items.h"

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

    // ---- Facing: 8 directions, index 0 S, 1 SE, 2 E, 3 NE, 4 N, 5 NW, 6 W, 7 SW. Only S..N (0-4) are drawn;
    // 5-7 are 3-1 mirrored (art faces right).
    constexpr int dir_count = 8;
    constexpr int drawn_dirs = 5;

    [[nodiscard]] constexpr int drawn_dir(int dir)
    {
        return dir <= 4 ? dir : 8 - dir;
    }

    [[nodiscard]] constexpr bool dir_flipped(int dir)
    {
        return dir > 4;
    }

    // ---- Wizard 32x32 (feet 1 px above the bottom row). wizard_ghost has the same 59-frame layout.
    inline constexpr const bn::sprite_item& wizard = bn::sprite_items::wizard;
    inline constexpr const bn::sprite_item& wizard_ghost = bn::sprite_items::wizard_ghost;   // Blink trail
    constexpr anim wizard_idle[drawn_dirs] = {
        { 0, 4, 12 }, { 35, 4, 12 }, { 4, 4, 12 }, { 39, 4, 12 }, { 43, 4, 12 }   // idle_s/se/e/ne/n
    };
    constexpr anim wizard_walk[drawn_dirs] = {
        { 8, 6, 6 }, { 47, 6, 6 }, { 14, 6, 6 }, { 53, 6, 6 }, { 20, 6, 6 }       // walk_s/se/e/ne/n
    };
    constexpr anim wizard_hurt = { 26, 2, 5 };
    constexpr anim wizard_death = { 28, 7, 8 };
    constexpr int wizard_y_offset = -10;              // sprite centre relative to the 8x8 hurt box centre
    constexpr int wizard_turn_frames = 3;             // one 45 degree step every N frames (never a 180 snap)

    // ---- Monsters. y offset = sprite centre relative to the 14x14 body box centre.
    // walk_dirs = 1: one move cycle facing right (left = flip). walk_dirs = 5: move[] = walk_s/se/e/ne/n and the
    // monster turns towards the wizard in 8 directions (a goblin/orc with those tags only needs a new row here).
    struct monster_art
    {
        const bn::sprite_item* item;
        anim move[drawn_dirs];
        int walk_dirs;
        anim death;
        int y_offset;
    };

    constexpr monster_art slime = { &bn::sprite_items::slime, { { 0, 4, 8 } }, 1, { 4, 4, 4 }, 0 };
    constexpr monster_art bat = { &bn::sprite_items::bat, { { 0, 4, 6 } }, 1, { 4, 3, 5 }, 0 };
    constexpr monster_art skeleton = { &bn::sprite_items::skeleton,
                                       { { 8, 4, 10 }, { 12, 4, 10 }, { 0, 4, 10 }, { 16, 4, 10 }, { 20, 4, 10 } },
                                       5, { 4, 4, 5 }, -8 };
    // goblin / orc 32x32 (art/LÉEME.md): walk_e 0-3, death 4-7, walk_s 8-11, walk_se 12-15, walk_ne 16-19, walk_n 20-23.
    constexpr monster_art goblin = { &bn::sprite_items::goblin,
                                     { { 8, 4, 7 }, { 12, 4, 7 }, { 0, 4, 7 }, { 16, 4, 7 }, { 20, 4, 7 } },
                                     5, { 4, 4, 5 }, -8 };
    constexpr monster_art orc = { &bn::sprite_items::orc,
                                  { { 8, 4, 10 }, { 12, 4, 10 }, { 0, 4, 10 }, { 16, 4, 10 }, { 20, 4, 10 } },
                                  5, { 4, 4, 6 }, -8 };
    // big_slime 32x32: hop 0-3, death 4-7.
    constexpr monster_art big_slime = { &bn::sprite_items::big_slime, { { 0, 4, 8 } }, 1, { 4, 4, 4 }, -6 };
    // wolf 32x16 (faces right): run 0-3, crouch 4 (red eyes: warning), dash 5, death 6-8.
    constexpr monster_art wolf = { &bn::sprite_items::wolf, { { 0, 4, 5 } }, 1, { 6, 3, 5 }, -2 };
    constexpr int wolf_crouch = 4;
    constexpr int wolf_dash = 5;

    inline constexpr const bn::sprite_item& slime_king = bn::sprite_items::slime_king;
    constexpr anim king_idle = { 0, 4, 10 };
    constexpr int king_crouch = 4;
    constexpr anim king_air = { 5, 2, 8 };
    constexpr anim king_land = { 7, 2, 10 };

    // Orc Warchief 64x64: walk 0-5, attack 6-14, stomp 15-16, horn 17-18.
    inline constexpr const bn::sprite_item& warchief = bn::sprite_items::orc_warchief;
    constexpr anim warchief_walk = { 0, 6, 8 };
    constexpr anim warchief_stomp = { 15, 2, 6 };
    constexpr anim warchief_charge = { 12, 2, 4 };
    constexpr anim warchief_horn = { 17, 2, 8 };
    constexpr int warchief_stunned = 14;

    // Thornwood Treant 64x64 (block 3): idle 0-3, walk 4-7, slam 8-10, hurt 11, death 12-15.
    inline constexpr const bn::sprite_item& treant = bn::sprite_items::treant;
    constexpr anim treant_idle = { 0, 4, 10 };
    constexpr anim treant_walk = { 4, 4, 10 };
    constexpr anim treant_slam = { 8, 3, 10 };
    constexpr int treant_hurt = 11;
    constexpr anim treant_death = { 12, 4, 8 };
    inline constexpr const bn::sprite_item& fx_root = bn::sprite_items::fx_root;     // 16x16, crack 0-1, spike 2-4
    constexpr anim root_crack = { 0, 2, 8 };
    constexpr anim root_spike = { 2, 3, 7 };
    inline constexpr const bn::sprite_item& fx_thorn = bn::sprite_items::fx_thorn;   // 16x16, idle 0-1
    inline constexpr const bn::sprite_item& fx_seed = bn::sprite_items::fx_seed;     // 8x8, fly 0-1

    // ---- Pickups.
    // Stage 2 Grim Graveyard (DISENO_RUTA block 5, art/LÉEME.md "Bloque 5"): horde arts reused with palettes.
    constexpr monster_art ghost = { &bn::sprite_items::ghost, { { 0, 4, 8 } }, 1, { 6, 3, 5 }, -2 };
    constexpr int ghost_fade = 4;
    constexpr monster_art skeleton_big = { &bn::sprite_items::skeleton_big, { { 0, 6, 8 } }, 1, { 6, 7, 4 }, -6 };

    inline constexpr const bn::sprite_item& bone_colossus = bn::sprite_items::bone_colossus;   // faces right
    constexpr anim colossus_walk = { 0, 6, 8 };
    constexpr anim colossus_throw = { 6, 2, 15 };
    constexpr anim colossus_collapse = { 8, 3, 8 };
    constexpr anim colossus_death = { 11, 5, 8 };
    inline constexpr const bn::sprite_item& banshee = bn::sprite_items::banshee;
    constexpr anim banshee_float = { 0, 4, 10 };
    constexpr anim banshee_wail = { 4, 2, 8 };
    constexpr anim banshee_vanish = { 6, 2, 8 };
    constexpr anim banshee_death = { 8, 4, 8 };
    inline constexpr const bn::sprite_item& lich = bn::sprite_items::lich;
    constexpr anim lich_idle = { 0, 4, 10 };
    constexpr anim lich_cast = { 4, 3, 10 };
    constexpr anim lich_teleport = { 7, 3, 8 };
    constexpr anim lich_death = { 10, 5, 8 };
    inline constexpr const bn::sprite_item& fx_arrow = bn::sprite_items::fx_arrow;   // 8x8, e 0, ne 1
    inline constexpr const bn::sprite_item& fx_bone = bn::sprite_items::fx_bone;     // 8x8, spin 0-3
    inline constexpr const bn::sprite_item& fx_skull = bn::sprite_items::fx_skull;   // 8x8, fly 0-1 (faces right)
    inline constexpr const bn::sprite_item& fx_grave = bn::sprite_items::fx_grave;   // 16x16, crack 0-2
    inline constexpr const bn::sprite_palette_item& rotten_palette = bn::sprite_palette_items::rotten_palette;
    inline constexpr const bn::sprite_palette_item& crow_palette = bn::sprite_palette_items::crow_palette;
    inline constexpr const bn::sprite_palette_item& grey_flesh_palette = bn::sprite_palette_items::grey_flesh_palette;
    inline constexpr const bn::sprite_palette_item& red_bone_palette = bn::sprite_palette_items::red_bone_palette;
    inline constexpr const bn::sprite_palette_item& violet_palette = bn::sprite_palette_items::violet_palette;
    inline constexpr const bn::bg_palette_item& ground_graveyard_palette = bn::bg_palette_items::ground_graveyard_palette;
    inline constexpr const bn::bg_palette_item& props_graveyard_palette = bn::bg_palette_items::props_graveyard_palette;

    inline constexpr const bn::sprite_item& gems = bn::sprite_items::gems;
    constexpr int gem_blue = 0;      // 2 frames each
    constexpr int gem_green = 2;
    constexpr int gem_red = 4;
    inline constexpr const bn::sprite_item& coin = bn::sprite_items::coin;
    inline constexpr const bn::sprite_item& chest = bn::sprite_items::chest;
    constexpr int chest_cursed_closed = 2;
    constexpr int chest_cursed_open = 3;
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
    constexpr int icon_ember_toss = 8;
    constexpr int icon_storm_chain = 9;

    // ---- Effects.
    inline constexpr const bn::sprite_item& fx_spark = bn::sprite_items::fx_spark;     // 8x8, 2 frames
    inline constexpr const bn::sprite_item& fx_rune = bn::sprite_items::fx_rune;       // 8x8, 2 frames
    inline constexpr const bn::sprite_item& fx_frost = bn::sprite_items::fx_frost;     // 16x16, grow 0-2
    inline constexpr const bn::sprite_item& fx_impact = bn::sprite_items::fx_impact;   // 8x8, 0-2
    inline constexpr const bn::sprite_item& fx_poof = bn::sprite_items::fx_poof;       // 8x8, 0-3
    inline constexpr const bn::sprite_item& fx_aura = bn::sprite_items::fx_aura;       // 16x16, 2 frames
    inline constexpr const bn::sprite_item& nova_ring = bn::sprite_items::fx_nova;        // 64x64, burst 0-7
    constexpr anim nova_ring_anim = { 0, 8, 3 };
    inline constexpr const bn::sprite_item& nova_spark = bn::sprite_items::fx_nova_spark; // 8x8, twinkle 0-3
    constexpr anim nova_spark_anim = { 0, 4, 4 };
    inline constexpr const bn::sprite_item& blink = bn::sprite_items::fx_blink;          // 32x32
    constexpr anim blink_out = { 0, 3, 3 };     // implosion where the wizard leaves
    constexpr anim blink_in = { 3, 3, 3 };      // flash where he lands
    inline constexpr const bn::sprite_item& fx_ember = bn::sprite_items::fx_ember;          // 8x8, fly 0-1
    inline constexpr const bn::sprite_item& fx_explosion = bn::sprite_items::fx_explosion;  // 32x32, burst 0-4
    constexpr anim explosion_anim = { 0, 5, 4 };
    inline constexpr const bn::sprite_item& fx_fire = bn::sprite_items::fx_fire;            // 16x16, burn 0-3
    constexpr anim fire_anim = { 0, 4, 5 };
    inline constexpr const bn::sprite_item& fx_chain = bn::sprite_items::fx_chain;          // 8x8, zap 0-2
    inline constexpr const bn::sprite_item& fx_dust = bn::sprite_items::fx_dust;            // 8x8, puff 0-2
    constexpr anim dust_anim = { 0, 3, 5 };
    inline constexpr const bn::sprite_item& shadow_small = bn::sprite_items::shadow_small;   // 16x8
    inline constexpr const bn::sprite_item& shadow_big = bn::sprite_items::shadow_big;       // 32x16

    // ---- UI (art/LÉEME.md "Bloque 2").
    inline constexpr const bn::sprite_item& xp_bar = bn::sprite_items::hud_xp;       // 32x8, frame k = 2k px (0-16)
    inline constexpr const bn::sprite_item& xp_cap = bn::sprite_items::hud_xp_cap;   // 8x8, left 0, right 1
    constexpr int xp_pieces = 7;
    inline constexpr const bn::sprite_item& hp_bar = bn::sprite_items::hud_hp;       // 32x8, fill 0-9
    constexpr int hp_bar_frames = 9;
    inline constexpr const bn::sprite_item& plaque = bn::sprite_items::hud_plaque;   // 32x16, left 0, right 1
    inline constexpr const bn::sprite_item& slots = bn::sprite_items::hud_slots;     // 16x16, weapon 1-8, passive 10-17
    constexpr int slot_weapon_first = 1;
    constexpr int slot_passive_first = 10;
    inline constexpr const bn::sprite_item& edge_arrow = bn::sprite_items::hud_arrow; // gold E/NE/N 0-2, red 3-5
    constexpr int arrow_east = 0;
    constexpr int arrow_north_east = 1;
    constexpr int arrow_north = 2;
    inline constexpr const bn::sprite_item& nova_meter = bn::sprite_items::hud_nova; // 16x16, charge 0-4, full 5-6
    constexpr int nova_meter_charge_frames = 5;
    constexpr anim nova_meter_full = { 5, 2, 9 };
    inline constexpr const bn::sprite_item& alert = bn::sprite_items::hud_alert;     // 16x16, blink 0-1
    inline constexpr const bn::sprite_item& level_burst = bn::sprite_items::fx_levelup;   // 64x64, 0-2 grow, 2<->3 loop
    inline constexpr const bn::sprite_item& card = bn::sprite_items::card;           // 64x64 top, 0 normal 1 selected
    inline constexpr const bn::sprite_item& card_base = bn::sprite_items::card_base; // 64x32 at y + 48
    inline constexpr const bn::sprite_item& logo = bn::sprite_items::logo;           // 6 cells of 64x32
    inline constexpr const bn::regular_bg_item& title_bg = bn::regular_bg_items::title_bg;
    inline constexpr const bn::sprite_palette_item& text_ink = bn::sprite_palette_items::text_ink;
    inline constexpr const bn::sprite_palette_item& text_gold = bn::sprite_palette_items::text_gold;
    inline constexpr const bn::sprite_palette_item& text_red = bn::sprite_palette_items::text_red;

    // PROVISIONAL (no final art yet): gem trail spark, shockwave ring.
    inline constexpr const bn::sprite_item& tmp_fx = bn::sprite_items::tmp_fx;
    constexpr int tmp_trail = 5;
    inline constexpr const bn::sprite_item& wave = bn::sprite_items::tmp_wave;          // 64x64, 2 frames
    inline constexpr const bn::sprite_item& boss_bar = bn::sprite_items::hud_bossbar;   // 32x8, frame k = 2k px
    constexpr int bar_width = 32;
    constexpr int boss_bar_notch = 17;

    // ---- Sounds (audio/LEEME.md, G-48). Pitch never above x1.24 (G-46).
    inline constexpr const bn::sound_item& sfx_spark = bn::sound_items::sfx_spark;   // one per burst, vol 0.3
    inline constexpr const bn::sound_item& sfx_hit = bn::sound_items::sfx_hit;
    inline constexpr const bn::sound_item& sfx_gem = bn::sound_items::sfx_gem;
    inline constexpr const bn::sound_item& sfx_level = bn::sound_items::sfx_levelup;
    inline constexpr const bn::sound_item& sfx_hurt = bn::sound_items::sfx_hurt;
    inline constexpr const bn::sound_item& sfx_boss = bn::sound_items::sfx_boss;
    inline constexpr const bn::sound_item& sfx_poof = bn::sound_items::sfx_poof;
    inline constexpr const bn::sound_item& sfx_menu = bn::sound_items::sfx_card;
    inline constexpr const bn::sound_item& sfx_card = bn::sound_items::sfx_card;
    inline constexpr const bn::sound_item& sfx_blink = bn::sound_items::sfx_blink;
    inline constexpr const bn::sound_item& sfx_nova = bn::sound_items::sfx_nova;
    inline constexpr const bn::sound_item& sfx_wave = bn::sound_items::sfx_wave;
    inline constexpr const bn::sound_item& sfx_howl = bn::sound_items::sfx_howl;
    inline constexpr const bn::sound_item& sfx_horn = bn::sound_items::sfx_horn;
    inline constexpr const bn::sound_item& sfx_stomp = bn::sound_items::sfx_stomp;
    inline constexpr const bn::sound_item& sfx_explosion = bn::sound_items::sfx_explosion;
    inline constexpr const bn::sound_item& sfx_zap = bn::sound_items::sfx_zap;
    inline constexpr const bn::sound_item& sfx_chest = bn::sound_items::sfx_chest;
    inline constexpr const bn::sound_item& sfx_coin_burst = bn::sound_items::sfx_coin_burst;
    inline constexpr const bn::sound_item& sfx_root_crack = bn::sound_items::sfx_root_crack;
    inline constexpr const bn::sound_item& sfx_slam = bn::sound_items::sfx_slam;
    inline constexpr const bn::sound_item& sfx_thorn = bn::sound_items::sfx_thorn;
    inline constexpr const bn::sound_item& sfx_curse_sting = bn::sound_items::sfx_curse_sting;
    // Jingles (music stopped first so they are heard).
    inline constexpr const bn::sound_item& sfx_wail = bn::sound_items::sfx_wail;
    inline constexpr const bn::sound_item& sfx_arrow = bn::sound_items::sfx_arrow;
    inline constexpr const bn::sound_item& sfx_rise = bn::sound_items::sfx_rise;
    inline constexpr const bn::sound_item& sfx_skull = bn::sound_items::sfx_skull;
    inline constexpr const bn::sound_item& sfx_teleport = bn::sound_items::sfx_teleport;
    inline constexpr const bn::sound_item& sfx_bone = bn::sound_items::sfx_bone;
    inline constexpr const bn::sound_item& jingle_victory = bn::sound_items::jingle_victory;
    inline constexpr const bn::sound_item& jingle_defeat = bn::sound_items::jingle_defeat;
    inline constexpr const bn::sound_item& jingle_boss_down = bn::sound_items::jingle_boss_down;

    // ---- Music (looping .it modules), volume 0.5-0.6 so the effects stay audible.
    inline constexpr const bn::music_item& music_title = bn::music_items::music_title;
    inline constexpr const bn::music_item& music_meadow = bn::music_items::music_meadow;
    inline constexpr const bn::music_item& music_graveyard = bn::music_items::music_graveyard;
    inline constexpr const bn::music_item& music_boss = bn::music_items::music_boss;
    constexpr int music_volume_pct = 55;
}

#endif
