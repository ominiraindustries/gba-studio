// DELVE G-40 M1: spell sprites in the view. See dl_magic_fx.h.

#include "dl_magic_fx.h"

#include "bn_color.h"
#include "bn_fixed.h"
#include "bn_math.h"
#include "bn_optional.h"

#include "bn_sprite_items_fx_heal.h"
#include "bn_sprite_items_fx_light_burst.h"
#include "bn_sprite_items_fx_missile.h"

#include "dl_combat.h"
#include "dl_view.h"
#include "dl_view_geometry.h"

namespace dl
{
    namespace
    {
        // relative (forward, right) cells of `cell` seen from `from` facing `dir`
        void relative(int from, int dir, int cell, int& fwd, int& right)
        {
            int ddx = (cell & (gen::W - 1)) - (from & (gen::W - 1));
            int ddy = (cell >> 5) - (from >> 5);
            fwd = ddx * gen::dx[dir] + ddy * gen::dy[dir];
            right = ddx * gen::dx[(dir + 1) & 3] + ddy * gen::dy[(dir + 1) & 3];
        }

        template<int N>
        bool ensure(bn::vector<bn::sprite_ptr, N>& v, const bn::sprite_item& item, int count)
        {
            while(v.size() < count)
            {
                bn::optional<bn::sprite_ptr> s = item.create_sprite_optional(0, 0);

                if(! s)
                {
                    return false;   // 16 sprite palettes full: skip the effect, never assert
                }

                s->set_bg_priority(0);
                s->set_z_order(-20);
                v.push_back(bn::move(*s));
            }

            return true;
        }
    }

    void magic_fx::clear()
    {
        _missile.clear();
        _sparks.clear();
        _flash.clear();
    }

    void magic_fx::set_visible(bool visible)
    {
        _visible = visible;

        if(! visible)
        {
            clear();
        }
    }

    int magic_fx::sprites_count() const
    {
        return _missile.size() + _sparks.size() + _flash.size();
    }

    void magic_fx::update(const magic_state& m, int cell, int dir)
    {
        namespace geo = dl::geo;
        ++_frame;

        if(! _visible)
        {
            return;
        }

        // ---- Magic Missile: 16x16 dart, 1 cell / 4 f, scaled like the objects (it grows as it leaves)
        bool shown = false;

        if(m.missile_cell >= 0)
        {
            int fwd, right;
            relative(cell, dir, m.missile_cell, fwd, right);
            int mf, mr;     // missile direction relative to the player
            int ahead = gen::neighbour(m.missile_cell, m.missile_dir);
            relative(cell, dir, ahead >= 0 ? ahead : m.missile_cell, mf, mr);
            mf -= fwd;
            mr -= right;
            int t = m.missile_hit > 0 ? 0 : m.missile_t;
            int zu = geo::cell_z_u(0) + fwd * geo::pitch_u + mf * geo::pitch_u * t / missile_cell_f;
            int xu = geo::cell_x_u(right) + mr * geo::pitch_u * t / missile_cell_f;

            if(m.missile_hit > 0)
            {
                zu -= geo::object_front_u * 2;      // the spark sits on the near face of the cell it hit
            }

            int vx, z;

            if(zu > geo::cell_u / 2 && dl::view::project(0, 0, xu, zu, vx, z) && z >= geo::cell_u / 2 &&
               ensure(_missile, bn::sprite_items::fx_missile, 1))
            {
                bn::fixed scale = bn::min(bn::fixed(geo::object_ref_u) / z * 2, bn::fixed(2));
                bn::sprite_ptr& s = _missile.front();
                int frame = m.missile_hit > 0 ? 3 - ((m.missile_hit >> 2) & 1) : (_frame >> 2) & 1;
                s.set_tiles(bn::sprite_items::fx_missile.tiles_item(), frame);
                s.set_scale(scale);
                s.set_position(vx - 120, bn::fixed(geo::floor_y(z)) - scale * 24 - 80);
                s.set_visible(true);
                shown = true;
            }
        }

        if(! shown)
        {
            _missile.clear();
        }

        // ---- Heal: 4 sparkles rising along the side edges of the view for 20 f
        if(m.heal_fx > 0)
        {
            if(ensure(_sparks, bn::sprite_items::fx_heal, 4))
            {
                int age = 20 - m.heal_fx;

                for(int i = 0; i < 4; ++i)
                {
                    int side = i & 1 ? 1 : -1;
                    int x = side * (geo::view_w / 2 - 12 - (i >> 1) * 10);
                    int y = 40 - age * 4 - (i >> 1) * 14;
                    _sparks[i].set_position(x + (geo::view_w / 2 - 120), y - 80 + geo::view_h / 2);
                    _sparks[i].set_tiles(bn::sprite_items::fx_heal.tiles_item(), ((age >> 2) + i) & 3);
                }
            }
        }
        else if(! _sparks.empty())
        {
            _sparks.clear();
        }

        // ---- Light: warm white flash in the middle of the view, 8 f, additive like the torch glow
        if(m.light_fx > 0)
        {
            if(ensure(_flash, bn::sprite_items::fx_light_burst, 1))
            {
                bn::sprite_ptr& s = _flash.front();
                s.set_position(geo::view_w / 2 - 120, geo::view_h / 2 - 80);
                s.set_blending_enabled(true);
                s.set_scale(2);
                s.set_tiles(bn::sprite_items::fx_light_burst.tiles_item(), bn::min((8 - m.light_fx) / 3, 2));
            }
        }
        else if(! _flash.empty())
        {
            _flash.clear();
        }
    }

    void magic_fx::light_palette(bn::bg_palette_ptr palette)
    {
        bn::span<const bn::color> colors = palette.colors();

        for(int i = 1; i < 192 && i < colors.size(); ++i)
        {
            bn::color c = colors[i];
            auto lift = [](int v) { return bn::min(31, v + ((v + 4) >> 3)); };
            palette.set_color(i, bn::color(lift(c.red()), lift(c.green()), lift(c.blue())));
        }
    }
}
