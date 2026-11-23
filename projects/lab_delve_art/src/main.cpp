// lab_delve_art: review bench for DELVE art (textures at 1x, ui_frame, monsters/props scaled by hardware).
// A: next scene, B: previous scene. Scenes 0-5 are pre-composed palette bitmaps (tools/make_scenes.py);
// scenes 5-7 add affine sprites at the depth scales of ESPEC_VISTA.md (1.0 / 0.55 / 0.35).
// Scenes 9-11: wall decoration sprites (deco_*): front views, then lateral views, at 1.0 / 0.55 / 0.3 over the stone
// wall, and a corridor mock, with the animated flame and the torch glow blended on top.
#include "bn_blending.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_palette_bitmap_item.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_vector.h"

#include "bn_palette_bitmap_items_scene_0.h"
#include "bn_palette_bitmap_items_scene_1.h"
#include "bn_palette_bitmap_items_scene_2.h"
#include "bn_palette_bitmap_items_scene_3.h"
#include "bn_palette_bitmap_items_scene_4.h"
#include "bn_palette_bitmap_items_scene_5.h"
#include "bn_palette_bitmap_items_scene_6.h"
#include "bn_palette_bitmap_items_scene_7.h"
#include "bn_palette_bitmap_items_scene_8.h"
#include "bn_sprite_items_mon_rat.h"
#include "bn_sprite_items_mon_skeleton.h"
#include "bn_sprite_items_mon_slime.h"
#include "bn_sprite_items_prop_barrel.h"
#include "bn_sprite_items_prop_bones.h"
#include "bn_sprite_items_prop_spikes.h"
#include "bn_sprite_items_prop_crate.h"
#include "bn_sprite_items_prop_statue.h"
#include "bn_sprite_items_prop_table.h"
#include "bn_sprite_items_prop_chest.h"
#include "bn_sprite_items_prop_plate.h"
#include "bn_sprite_items_prop_pit.h"
#include "bn_sprite_items_deco_torch_front.h"
#include "bn_sprite_items_deco_torch_left.h"
#include "bn_sprite_items_deco_torch_right.h"
#include "bn_sprite_items_deco_torch_glow.h"
#include "bn_sprite_items_deco_banner_front.h"
#include "bn_sprite_items_deco_banner_left.h"
#include "bn_sprite_items_deco_banner_right.h"
#include "bn_sprite_items_deco_chains_front.h"
#include "bn_sprite_items_deco_chains_left.h"
#include "bn_sprite_items_deco_chains_right.h"
#include "bn_sprite_items_deco_lever_front.h"
#include "bn_sprite_items_deco_lever_left.h"
#include "bn_sprite_items_deco_lever_right.h"

namespace
{
    constexpr int scenes_count = 12;
    constexpr int max_sprites = 40;

    // Flame: 4 frames with an irregular rhythm (110 / 70 / 140 / 90 ms, as tagged in the .aseprite).
    constexpr int flame_ticks[] = { 7, 4, 8, 5 };
    constexpr int flame_cycle = 24;

    struct placed
    {
        const bn::sprite_item* item;
        int frames;
        bn::sprite_ptr sprite;
        int phase;
    };

    const bn::palette_bitmap_item& scene_bitmap(int scene)
    {
        switch(scene)
        {
        case 0: return bn::palette_bitmap_items::scene_0;
        case 1: return bn::palette_bitmap_items::scene_1;
        case 2: return bn::palette_bitmap_items::scene_2;
        case 3: return bn::palette_bitmap_items::scene_3;
        case 4: return bn::palette_bitmap_items::scene_4;
        case 5: return bn::palette_bitmap_items::scene_5;
        case 9: case 10: return bn::palette_bitmap_items::scene_7;
        case 11: return bn::palette_bitmap_items::scene_8;
        default: return bn::palette_bitmap_items::scene_6;
        }
    }

    void add(bn::vector<placed, max_sprites>& out, const bn::sprite_item& item, int frames, bn::fixed x, bn::fixed y,
             bn::fixed scale, int z_order, int phase = 0)
    {
        bn::sprite_ptr sprite = item.create_sprite(x, y);
        if(scale != 1)
        {
            sprite.set_scale(scale);
        }
        sprite.set_z_order(z_order);
        out.push_back(placed{ &item, frames, bn::move(sprite), phase });
    }

    // Halo over a torch whose sprite centre is (x, y): the flame centre is sprite pixel (fx, 12) of the torch.
    void add_glow(bn::vector<placed, max_sprites>& out, bn::fixed x, bn::fixed y, bn::fixed scale, int fx, int z_order)
    {
        bn::fixed gx = x + (fx - 16) * scale;
        bn::fixed gy = y + (12 - 32) * scale;
        bn::sprite_ptr glow = bn::sprite_items::deco_torch_glow.create_sprite(gx, gy);
        glow.set_scale(scale);
        glow.set_blending_enabled(true);
        glow.set_z_order(z_order);   // behind its torch: OBJ-OBJ blending does not exist, the top OBJ pixel wins
        out.push_back(placed{ &bn::sprite_items::deco_torch_glow, 1, bn::move(glow), 0 });
    }

    // Deco on a wall face: (wx, wy) = screen point of the wall contact, (ax, ay) = that pixel in the sprite.
    void add_deco(bn::vector<placed, max_sprites>& out, const bn::sprite_item& item, int frames, bn::fixed wx,
                  bn::fixed wy, int ax, int ay, bn::fixed scale, int z_order, int phase)
    {
        add(out, item, frames, wx - (ax - 16) * scale, wy - (ay - 32) * scale, scale, z_order, phase);
    }

    void build_decos(int scene, bn::vector<placed, max_sprites>& out)
    {
        if(scene == 9 || scene == 10)
        {
            // Rows: scale 1.0, 0.55, 0.3. Scene 9: front views; scene 10: left views then right views.
            const bn::sprite_item* front[] = { &bn::sprite_items::deco_torch_front, &bn::sprite_items::deco_banner_front,
                                               &bn::sprite_items::deco_chains_front, &bn::sprite_items::deco_lever_front };
            const bn::sprite_item* sides[] = { &bn::sprite_items::deco_torch_left, &bn::sprite_items::deco_banner_left,
                                               &bn::sprite_items::deco_chains_left, &bn::sprite_items::deco_lever_left,
                                               &bn::sprite_items::deco_torch_right, &bn::sprite_items::deco_banner_right,
                                               &bn::sprite_items::deco_chains_right, &bn::sprite_items::deco_lever_right };
            const int frames[] = { 4, 2, 1, 2 };
            const bn::fixed scales[] = { 1, bn::fixed(0.55), bn::fixed(0.3) };
            const int ys[] = { -44, 14, 56 };
            int count = scene == 9 ? 4 : 8;
            for(int row = 0; row < 3; ++row)
            {
                for(int i = 0; i < count; ++i)
                {
                    bn::fixed x = scene == 9 ? -84 + i * 56 : -105 + i * 30;
                    const bn::sprite_item& item = scene == 9 ? *front[i] : *sides[i];
                    add(out, item, frames[i % 4], x, ys[row], scales[row], 0, 0);   // same frame: shared tiles
                    if(i % 4 == 0)
                    {
                        add_glow(out, x, ys[row], scales[row], scene == 9 ? 16 : (i == 0 ? 16 : 15), 1);
                    }
                }
            }
        }
        else if(scene == 11)
        {
            // Corridor mock (f = 88, eye at mid wall, view centre (88, 60)), lateral faces at X = -+0.5 cells:
            // wall texel row v at depth z is at x = 88 -+ 44 / z, y = 60 + 88 * (v / 64 - 0.5) / z.
            // Sprite coordinates are relative to the screen centre (120, 80).
            struct wall_deco { const bn::sprite_item* item; int frames; int side; bn::fixed z; int ax; int ay; };
            const wall_deco decos[] = {
                { &bn::sprite_items::deco_lever_left, 2, -1, bn::fixed(1.2), 9, 32 },
                { &bn::sprite_items::deco_torch_left, 4, -1, bn::fixed(2.2), 6, 43 },
                { &bn::sprite_items::deco_chains_right, 1, 1, bn::fixed(1.3), 23, 10 },
                { &bn::sprite_items::deco_torch_right, 4, 1, bn::fixed(2.2), 26, 43 },
            };
            int phase = 0;
            for(const wall_deco& d : decos)
            {
                bn::fixed scale = bn::fixed(88) / d.z / 64;
                bn::fixed wx = bn::fixed(88 - 120) + d.side * bn::fixed(44) / d.z;
                bn::fixed wy = bn::fixed(60 - 80) + bn::fixed(88) * (bn::fixed(d.ay) / 64 - bn::fixed(0.5)) / d.z;
                add_deco(out, *d.item, d.frames, wx, wy, d.ax, d.ay, scale, d.z > 2 ? 2 : 1, phase++);
                if(d.frames == 4)
                {
                    add_glow(out, wx - (d.ax - 16) * scale, wy - (d.ay - 32) * scale, scale, d.side < 0 ? 16 : 15, 3);
                }
            }
            // Front wall at z = 2.5 (scale 0.55): banner centred.
            add(out, bn::sprite_items::deco_banner_front, 2, 88 - 120, 60 - 80, bn::fixed(88) / bn::fixed(2.5) / 64, 3);
        }
    }

    void build_sprites(int scene, bn::vector<placed, max_sprites>& out)
    {
        out.clear();
        if(scene >= 9)
        {
            build_decos(scene, out);
        }
        else if(scene == 5)
        {
            // Room mock: positions from the raycaster (f = 88, eye at half height), feet on the cell centre.
            add(out, bn::sprite_items::prop_spikes, 2, -32, -7, bn::fixed(0.55), 3);
            add(out, bn::sprite_items::prop_barrel, 1, -61, -11, bn::fixed(0.35), 4);
            add(out, bn::sprite_items::prop_bones, 1, 12, -7, bn::fixed(0.55), 3);
            add(out, bn::sprite_items::mon_slime, 5, -3, -16, bn::fixed(0.35), 2);
            add(out, bn::sprite_items::mon_skeleton, 5, -76, -16, bn::fixed(0.55), 1);
            add(out, bn::sprite_items::mon_rat, 5, -32, -8, 1, 0);
        }
        else if(scene == 6)
        {
            const bn::sprite_item* items[] = { &bn::sprite_items::mon_rat, &bn::sprite_items::mon_skeleton,
                                               &bn::sprite_items::mon_slime };
            for(int i = 0; i < 3; ++i)
            {
                int x = -80 + i * 80;
                add(out, *items[i], 5, x, -46, 1, 0);
                add(out, *items[i], 5, x, 16, bn::fixed(0.55), 0);
                add(out, *items[i], 5, x, 60, bn::fixed(0.35), 0);
            }
        }
        else if(scene == 7 || scene == 8)
        {
            const bn::sprite_item* items7[] = { &bn::sprite_items::prop_barrel, &bn::sprite_items::prop_bones,
                                                &bn::sprite_items::prop_spikes, &bn::sprite_items::prop_crate,
                                                &bn::sprite_items::prop_statue };
            const bn::sprite_item* items8[] = { &bn::sprite_items::prop_table, &bn::sprite_items::prop_chest,
                                                &bn::sprite_items::prop_plate, &bn::sprite_items::prop_pit };
            int frames7[] = { 1, 1, 2, 1, 1 };
            int frames8[] = { 1, 2, 2, 1 };
            int count = scene == 7 ? 5 : 4;
            for(int i = 0; i < count; ++i)
            {
                const bn::sprite_item& item = scene == 7 ? *items7[i] : *items8[i];
                int frames = scene == 7 ? frames7[i] : frames8[i];
                int x = -96 + i * 48;
                add(out, item, frames, x, -50, 1, 0);
                add(out, item, frames, x, 0, bn::fixed(0.55), 0);
                add(out, item, frames, x, 40, bn::fixed(0.35), 0);
            }
        }
    }
}

int main()
{
    bn::core::init();

    bn::palette_bitmap_bg_ptr bg = bn::palette_bitmap_bg_ptr::create(
            bn::palette_bitmap_items::scene_0.palette_item());
    bn::palette_bitmap_bg_painter painter(bg);
    bn::vector<placed, max_sprites> sprites;
    bn::blending::set_transparency_alpha(bn::fixed(0.4));
    int scene = 0;
    int shown = -1;
    int counter = 0;

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            scene = (scene + 1) % scenes_count;
        }
        else if(bn::keypad::b_pressed())
        {
            scene = (scene + scenes_count - 1) % scenes_count;
        }

        if(scene != shown)
        {
            sprites.clear();
            painter.blit(0, 0, scene_bitmap(scene).pixels_item());
            painter.flip_page_now();
            painter.blit(0, 0, scene_bitmap(scene).pixels_item());
            build_sprites(scene, sprites);
            shown = scene;
            counter = 0;
        }

        for(placed& p : sprites)
        {
            if(p.frames == 4)
            {
                // Torch flame: irregular rhythm, offset per torch so they do not flicker in sync.
                int t = (counter + p.phase * 11) % flame_cycle;
                int frame = 0;
                while(t >= flame_ticks[frame])
                {
                    t -= flame_ticks[frame];
                    ++frame;
                }
                if(t == 0 || counter == 0)
                {
                    p.sprite.set_tiles(p.item->tiles_item().create_tiles(frame));
                }
            }
            else if(counter % 30 == 0)
            {
                // Other animations: one frame each 30 ticks (idle, attack, hit; banner wave; lever up / down).
                int frame = (counter / 30) % p.frames;
                p.sprite.set_tiles(p.item->tiles_item().create_tiles(frame));
            }
        }

        ++counter;
        bn::core::update();
    }
}
