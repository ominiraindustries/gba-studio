// DELVE view geometry: the single source of truth for the first-person view (ESPEC_VISTA.md).
//
// Model (G-31, Dungeon Hack look, user correction of G-29): every cell keeps a walkable floor of full width 1 with
// its wall faces at +-0.5 from its centre; the wall thickness t lives BETWEEN cells, so the cell pitch is 1 + t.
// The band between two cells is solid stone on a wall edge (seen from both sides, its ends are block faces) and
// floor / ceiling on an open edge (a short connector of length t at full corridor width). Openings (door, arch,
// bars, stairs) have jambs t deep with the leaf in the middle of the band. The t x t square where four bands meet
// is a solid block when any of its four edges is a wall, open floor otherwise (rooms have no pillars).
//
// Camera: eye at mid-height of the walls, eye_back_z10 / 10 cells behind the cell centre, so the side walls of the
// player's own cell are visible (classic look); focal 96 px per cell (proj_num / 5). tools/build_view.py ray-traces
// every piece from these numbers for the resting view and for the intermediate camera poses of smooth moves.
// Coordinates of the base frame (dl_view.h project): u = 1/320 cell, X from the centre of the player's column, Z from
// the resting eye. Cell (d, x) has its centre at X = x * pitch_u, Z = d * pitch_u + eye_back_u.
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
    constexpr int vp_y = 48;                // horizon at 40 % of the view: camera looking slightly down
    constexpr int proj_num = 480;           // focal length 96 px per cell = proj_num / 5 (wider FOV)
    constexpr int eye_back_z10 = 7;         // eye 0.7 cells behind the cell centre
    constexpr int depths = 5;               // rows d = 0..4 of the resting view
    constexpr int max_lateral = 3;          // cells x = -3..+3 of the resting view
    constexpr int wall_thick_64 = 24;       // wall band thickness t in texels of a 64-texel cell: t = 0.375 (G-31)
    constexpr int cell_pitch_64 = 88;       // 64 + wall_thick_64: cell pitch 1 + t = 1.375 cells
    constexpr int fog_z10 = 67;             // nothing farther than the front face of row 4 (z = 4 * 1.375 + 1.2)
    constexpr int fade_x1000 = 300;         // attenuation exp(-0.300 (z - 1)): the row-4 front keeps its old 17 %
    constexpr int monster_size = 64;        // affine sprite base sizes (ESPEC_VISTA.md)
    constexpr int object_size = 32;
    constexpr int deco_ref_z10 = 15;        // wall decoration sprites: 1 sprite px = 1 texel at z = 1.5
    constexpr int deco_max_scale_x100 = 200; // lateral decorations at d = 0 would need > 2x (double-size limit)
    constexpr int object_ref_z10 = 15;      // objects: scale 1.0 at z = 1.5 (they shrink as 1 / z)

    constexpr int cell_u = 320;
    constexpr int pitch_u = cell_u * cell_pitch_64 / 64;     // 440
    constexpr int eye_back_u = cell_u * eye_back_z10 / 10;   // 224
    constexpr int object_front_u = 64;      // objects stand 0.2 cells in front of their cell centre (to the viewer)
    constexpr int deco_ref_u = deco_ref_z10 * 32;
    constexpr int object_ref_u = object_ref_z10 * 32;

    [[nodiscard]] constexpr int clamp(int v, int lo, int hi)
    {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    // centre of base-relative cell (d, x) in the base frame (u)
    [[nodiscard]] constexpr int cell_x_u(int x)
    {
        return x * pitch_u;
    }

    [[nodiscard]] constexpr int cell_z_u(int d)
    {
        return d * pitch_u + eye_back_u;
    }

    // depth band of a camera depth (u), 0 = player's row: used for sprite z order and the depth fade of sprites
    [[nodiscard]] constexpr int depth_band(int z_u)
    {
        return z_u < 160 ? 0 : (z_u - 160) / pitch_u;
    }

    // view y of the floor line at camera depth z_u (feet of objects)
    [[nodiscard]] constexpr int floor_y(int z_u)
    {
        return vp_y + (proj_num / 10 * cell_u) / z_u;     // 0.5 cell below the eye
    }

    static_assert(pitch_u == 440 && depth_band(cell_z_u(1) - object_front_u) == 1);
}

#endif
