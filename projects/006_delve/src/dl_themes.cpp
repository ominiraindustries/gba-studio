#include "dl_themes.h"

#include "dl_view_data.h"
#include "dl_view_motion_data.h"

// debug build: make USERFLAGS=-DDL_FORCE_THEME=3 starts with theme 3 forced (captures); the FX menu changes it
#ifndef DL_FORCE_THEME
    #define DL_FORCE_THEME -1
#endif

namespace dl::themes
{
    namespace
    {
        int current_theme = 0;
        // zero-initialised on purpose: an initialised IWRAM global read 0 at run time in a test build (G-37)
        int forced_delta = 0;
        bool changed_flag = false;

        bool same(const char* a, const char* b)
        {
            while(*a && *a == *b)
            {
                ++a;
                ++b;
            }

            return *a == *b;
        }
    }

    int find(const char* name)
    {
        for(int t = 0; t < dl::view_data::theme_count; ++t)
        {
            if(same(dl::view_data::theme_names[t], name))
            {
                return t;
            }
        }

        return -1;
    }

    int for_floor(int floor)
    {
        int i = floor < 0 ? 0 : floor >= floor_order_count ? floor_order_count - 1 : floor;
        int t = find(floor_order[i]);
        return t < 0 ? 0 : t;
    }

    int current()
    {
        return current_theme;
    }

    const char* current_name()
    {
        return dl::view_data::theme_names[current_theme];
    }

    void apply(bn::bg_palette_ptr palette, int theme)
    {
        if(theme < 0 || theme >= dl::view_data::theme_count)
        {
            theme = 0;
        }

        current_theme = theme;
        dl::view_motion_data::current_pixels = dl::view_motion_data::theme_pixels[theme];

        const bn::color* colors = dl::view_data::theme_palette_colors[theme];

        for(int i = 1; i < dl::view_data::ui_first_color; ++i)
        {
            palette.set_color(i, colors[i]);
        }
    }

    void apply_for_floor(bn::bg_palette_ptr palette, int floor)
    {
        int f = forced();
        apply(palette, f >= 0 ? f : for_floor(floor));
    }

    const bn::color* deco_colors(int which)
    {
        return dl::view_data::theme_deco_colors[current_theme][which ? 1 : 0];
    }

    const uint8_t* glow_rgb()
    {
        return dl::view_data::theme_glow_rgb[current_theme];
    }

    int forced()
    {
        int n = dl::view_data::theme_count + 1;     // -1 (by floor), 0 .. theme_count - 1
        return ((DL_FORCE_THEME + 1 + forced_delta) % n + n) % n - 1;
    }

    void cycle_forced(int delta)
    {
        forced_delta += delta;
        changed_flag = true;
    }

    bool take_changed()
    {
        bool c = changed_flag;
        changed_flag = false;
        return c;
    }
}
