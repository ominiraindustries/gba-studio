// DELVE view geometry: the single source of truth for the first-person view (ESPEC_VISTA.md).
//
// Walls live on cell EDGES (the generator ports dungeon_model.py v2: walls per side), so every piece is the face of
// one edge seen from the player's cell:
//   * front face (d, x): the FAR edge of the visible cell at depth d (0 = player's cell .. 3) and lateral offset x
//     (-2..+2). It is a rectangle.
//   * lateral face (d, l): the side edge of row d on the line X = l / 2 cells (l = -5, -3, -1, 1, 3, 5 half cells:
//     -2.5 .. +2.5). It is a trapezoid, kept at 2-px column granularity so every span starts on an even pixel.
//   * floor and ceiling: one static strip image (rows below / above the horizon) composed by tools/build_view.py.
//
// Camera: eye at mid-height of the walls, pulled back eye_back_z10 / 10 cells from the cell centre so the side walls
// of the player's own cell are visible (classic look). Depth z is measured in tenths of a cell (z10), lateral offsets
// in half cells (h). Screen offset from the vanishing point = proj_num * h / z10, rounded to an even pixel.
//
// tools/build_view.py parses every "constexpr int name = value;" line of this file, so keep them as plain integers.

#ifndef DL_VIEW_GEOMETRY_H
#define DL_VIEW_GEOMETRY_H

namespace dl::geo
{
    constexpr int view_x = 0;
    constexpr int view_y = 0;
    constexpr int view_w = 176;
    constexpr int view_h = 120;
    constexpr int vp_x = 88;                // vanishing point
    constexpr int vp_y = 60;
    constexpr int proj_num = 576;           // focal length 115.2 px per cell = proj_num / 5
    constexpr int eye_back_z10 = 7;         // eye 0.7 cells behind the cell centre
    constexpr int depths = 4;               // rows d = 0..3
    constexpr int max_lateral = 2;          // cells x = -2..+2
    constexpr int fog_z10 = 42;             // nothing is drawn farther than the far edge of row 3 (z = 4.2)
    constexpr int object_z10_offset = 3;    // objects stand 0.3 cells past the near edge of their cell
    constexpr int monster_size = 64;        // affine sprite base sizes (ESPEC_VISTA.md)
    constexpr int object_size = 32;

    // near / far edge of row d, in tenths of a cell
    [[nodiscard]] constexpr int near_z10(int d)
    {
        return 10 * d + 5 - eye_back_z10 + 4;   // d + 0.2 (= d - 0.5 + 0.7)
    }

    [[nodiscard]] constexpr int far_z10(int d)
    {
        return near_z10(d) + 10;                // d + 1.2
    }

    // Projected offset of a half-cell distance h at depth z10, rounded to an even pixel (symmetric for negative h).
    [[nodiscard]] constexpr int offset(int h, int z10)
    {
        int a = h < 0 ? -h : h;
        int half = (proj_num / 2 * a * 2 + z10) / (2 * z10);   // round(proj_num * a / (2 * z10))
        int result = half * 2;
        return h < 0 ? -result : result;
    }

    [[nodiscard]] constexpr int clamp(int v, int lo, int hi)
    {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    struct rect
    {
        int x0;     // inclusive
        int y0;
        int x1;     // exclusive
        int y1;

        [[nodiscard]] constexpr bool empty() const
        {
            return x1 <= x0 || y1 <= y0;
        }
    };

    // Front face (far edge) of cell (d, x), clipped to the view.
    [[nodiscard]] constexpr rect front_rect(int d, int x)
    {
        int z = far_z10(d);
        int half_h = offset(1, z);
        return rect{ clamp(vp_x + offset(2 * x - 1, z), 0, view_w), clamp(vp_y - half_h, 0, view_h),
                     clamp(vp_x + offset(2 * x + 1, z), 0, view_w), clamp(vp_y + half_h, 0, view_h) };
    }

    // Lateral face of row d on the line X = l / 2 (l odd, -5..5): horizontal extent clipped to the view (the
    // vertical extent changes per column; build_view.py computes it with the same formula). Empty when off screen.
    [[nodiscard]] constexpr rect lateral_rect(int d, int l)
    {
        int a = vp_x + offset(l, near_z10(d));
        int b = vp_x + offset(l, far_z10(d));
        int x0 = clamp(a < b ? a : b, 0, view_w);
        int x1 = clamp(a < b ? b : a, 0, view_w);
        int half_h = offset(1, near_z10(d));
        return rect{ x0, clamp(vp_y - half_h, 0, view_h), x1, clamp(vp_y + half_h, 0, view_h) };
    }

    // Affine sprite of an object standing in cell (d, x), d = 1..3. Scale is 1.0 at d = 1 (spec: ~1.0 / 0.55 / 0.35;
    // here 1.0 / 0.565 / 0.394). Positions are view pixels: centre x and the floor line (feet) y.
    [[nodiscard]] constexpr int object_z10(int d)
    {
        return near_z10(d) + object_z10_offset;
    }

    // scale numerator / denominator: scale = object_z10(1) / object_z10(d)
    [[nodiscard]] constexpr int object_scale_num()
    {
        return object_z10(1);
    }

    [[nodiscard]] constexpr int object_x(int d, int x)
    {
        return vp_x + offset(2 * x, object_z10(d));
    }

    [[nodiscard]] constexpr int object_feet_y(int d)
    {
        return vp_y + offset(1, object_z10(d));
    }

    static_assert(front_rect(0, 0).x1 - front_rect(0, 0).x0 == 96);
    static_assert(front_rect(3, 0).x1 - front_rect(3, 0).x0 == 28);
    static_assert(lateral_rect(0, 1).x0 == 136 && lateral_rect(0, 1).x1 == view_w);
}

#endif
