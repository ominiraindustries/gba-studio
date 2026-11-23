// lab_delve_art: review bench for DELVE block-1 art (textures at 1x, ui_frame, monsters/props scaled by hardware).
// A: next scene, B: previous scene. Scenes 0-5 are pre-composed palette bitmaps (tools/make_scenes.py);
// scenes 5-7 add affine sprites at the depth scales of ESPEC_VISTA.md (1.0 / 0.55 / 0.35).
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

namespace
{
    constexpr int scenes_count = 9;

    struct placed
    {
        const bn::sprite_item* item;
        int frames;
        bn::sprite_ptr sprite;
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
        default: return bn::palette_bitmap_items::scene_6;
        }
    }

    void add(bn::vector<placed, 24>& out, const bn::sprite_item& item, int frames, int x, int y, bn::fixed scale,
             int z_order)
    {
        bn::sprite_ptr sprite = item.create_sprite(x, y);
        if(scale != 1)
        {
            sprite.set_scale(scale);
        }
        sprite.set_z_order(z_order);
        out.push_back(placed{ &item, frames, bn::move(sprite) });
    }

    void build_sprites(int scene, bn::vector<placed, 24>& out)
    {
        out.clear();
        if(scene == 5)
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
    bn::vector<placed, 24> sprites;
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
            painter.blit(0, 0, scene_bitmap(scene).pixels_item());
            painter.flip_page_now();
            painter.blit(0, 0, scene_bitmap(scene).pixels_item());
            build_sprites(scene, sprites);
            shown = scene;
            counter = 0;
        }

        // Cycle every frame of every sprite: idle 1-2, attack 3-4, hit 5 (30 frames each).
        if(counter % 30 == 0)
        {
            for(placed& p : sprites)
            {
                int frame = (counter / 30) % p.frames;
                p.sprite.set_tiles(p.item->tiles_item().create_tiles(frame));
            }
        }

        ++counter;
        bn::core::update();
    }
}
