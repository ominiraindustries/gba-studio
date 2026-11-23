// DELVE view renderer (see dl_view.h), compiled as ARM in IWRAM (*.bn_iwram.cpp; measured in G-25). Pieces and
// geometry come from dl_view_data (generated) and dl_view_geometry.h; this file only decides which edge face goes
// where and copies the spans.

#include "dl_view.h"

#include "bn_common.h"

#include "dl_blit.h"
#include "dl_view_data.h"
#include "dl_view_geometry.h"

namespace dl::view
{
namespace
{
    using namespace dl::gen;
    namespace vd = dl::view_data;
    namespace geo = dl::geo;

    constexpr int page_stride = 120;    // uint16_t per 240-px row
    constexpr int owner_cols = geo::view_w / 2;

    // Which face is the last opaque thing drawn on each 2-px column. Walls are full height and nearer walls are
    // taller, so this is the face seen in that column: a decoration is shown only where its own face is seen.
    uint8_t owner[owner_cols];

    constexpr uint8_t front_id(int d, int xi)
    {
        return uint8_t(1 + d * cols + xi);
    }

    constexpr uint8_t lateral_id(int d, int li)
    {
        return uint8_t(64 + d * vd::lines + li);
    }

    // vertex block of vertex row dv (Z = near(0) for dv = 0, far(dv - 1) otherwise) on lateral line index li
    constexpr uint8_t vertex_id(int dv, int li)
    {
        return uint8_t(128 + dv * vd::lines + li);
    }

    void blit_piece(uint16_t* page, const vd::piece& p)
    {
        blit_spans(page, vd::spans + p.first, p.count);
    }

    void own(const vd::piece& p, uint8_t id)
    {
        for(int c = p.c0; c < p.c1 && c < owner_cols; ++c)
        {
            owner[c] = id;
        }
    }

    bool owns(int screen_x, uint8_t id)
    {
        int c = screen_x >> 1;

        for(int k = c - 1; k <= c + 1; ++k)
        {
            if(k < 0 || k >= owner_cols || owner[k] != id)
            {
                return false;
            }
        }

        return true;
    }

    int lever_of_edge(const level& lv, int idx)
    {
        for(int i = 0; i < lv.nhooks; ++i)
        {
            if(lv.hooks[i].type == H_LEVER && lv.hooks[i].lever_edge == idx)
            {
                return i;
            }
        }

        return -1;
    }

    // piece set of an edge, or -1 when nothing is drawn
    int edge_set(const level& lv, const edge_state& es, int idx)
    {
        if(idx < 0)
        {
            return vd::set_wall;
        }

        int e = lv.ed[idx];
        int var = e >> 4;

        switch(e & 15)
        {
        case K_WALL:
        {
            // torch, banner, chains and levers are sprites (edge_deco): the wall behind them is plain
            static constexpr int8_t set_of_decor[DECOR_COUNT] = {
                vd::set_wall, vd::set_wall, vd::set_wall, vd::set_wall, vd::set_wall_skulls, vd::set_wall_crack,
                vd::set_wall
            };
            int decor = floor_tables[lv.floor_table].decor_of_var[var];
            int set = set_of_decor[decor < DECOR_COUNT ? decor : 0];

            if(set == vd::set_wall)
            {
                // one of the wall variants, fixed per edge: walls keep their look and every step shifts them
                set += int((unsigned(idx) * 2654435761u) >> 30);
            }

            return set;
        }

        case K_SECRET:
            return vd::set_wall_crack;

        case K_DOOR:
            return es.open[idx] ? vd::set_door_open : vd::set_door_closed;

        case K_LOCKED:
            return es.open[idx] ? vd::set_door_open : vd::set_door_locked;

        case K_ARCH:
            return vd::set_arch;

        case K_BARS:
            return es.open[idx] ? -1 : vd::set_bars;

        case K_STAIRS:
            return vd::set_stairs;

        default:
            return -1;
        }
    }

    bool sight_passes(const level& lv, const edge_state& es, int idx)
    {
        if(idx < 0)
        {
            return false;
        }

        int k = lv.ed[idx] & 15;
        return sight_kind[k] || ((k == K_BARS || k == K_DOOR || k == K_LOCKED) && es.open[idx]);
    }

    // the edge is part of a wall slab (anything but a plain opening); outside the map counts as wall
    bool slab(const level& lv, int idx)
    {
        return idx < 0 || (lv.ed[idx] & 15) != K_OPEN;
    }

    void add_deco(visibility& vis, int kind, int d, int slot, bool lateral)
    {
        if(kind != DECO_NONE && vis.ndecos < max_decos)
        {
            vis.decos[vis.ndecos++] = deco_place{ uint8_t(kind), uint8_t(d), int8_t(slot), uint8_t(lateral) };
        }
    }

    bool visible_at(const visibility& vis, int d, int x)
    {
        return d >= 0 && d < rows && x >= -half && x <= half && vis.cell[d][x + half] >= 0;
    }

    // Slabs are centred on their edges (G-29), so the four slabs meeting at a vertex share a block there: drawn
    // when any of its edges is a slab and one of its four cells is visible (front face + the side face towards
    // the player; the painter's order hides what is behind nearer faces).
    void draw_vertices(uint16_t* page, const level& lv, int cell, int dir, const visibility& vis, int dv)
    {
        static constexpr int lat_l[vd::lines] = { -7, 7, -5, 5, -3, 3, -1, 1 };
        const int right = (dir + 1) & 3;
        const int ra = dv - 1;      // row in front of the vertex (-1: behind the player's row)

        for(int k = 0; k < vd::lines; ++k)
        {
            int l = lat_l[k];
            int li = (l + vd::lines - 1) >> 1;
            const vd::piece& pf = vd::vertex_front[dv][li];
            const vd::piece& ps = vd::vertex_side[dv][li];

            if(! pf.count && ! ps.count)
            {
                continue;
            }

            int xa = (l - 1) / 2;   // cells left and right of the line
            int xb = xa + 1;

            if(! visible_at(vis, ra, xa) && ! visible_at(vis, ra, xb) && ! visible_at(vis, dv, xa) &&
                    ! visible_at(vis, dv, xb))
            {
                continue;
            }

            int caa = relative_cell(cell, dir, ra, xa);
            int cab = relative_cell(cell, dir, ra, xb);
            int cba = relative_cell(cell, dir, dv, xa);
            bool front_a = caa < 0 || slab(lv, edge_index(caa, dir));
            bool front_b = cab < 0 || slab(lv, edge_index(cab, dir));
            bool lat_near = caa < 0 || slab(lv, edge_index(caa, right));     // line l, row in front of the vertex
            bool lat_far = cba < 0 || slab(lv, edge_index(cba, right));

            if(! (front_a || front_b || lat_near || lat_far))
            {
                continue;
            }

            // measured (G-29, tour max): both faces of every block 1.22 f -> 1.10 f with this culling. The side face is hidden behind the front wall of the inner cell in front of the vertex; the front
            // face behind the lateral wall in front of it (beside the player's row it is behind the eye).
            bool inner_front = l < 0 ? front_b : front_a;

            if(ps.count && ! inner_front)
            {
                blit_piece(page, ps);
                own(ps, vertex_id(dv, li));
            }

            if(pf.count && dv > 0 && ! lat_near)
            {
                blit_piece(page, pf);
                own(pf, vertex_id(dv, li));
            }
        }
    }

    // minimap
    alignas(4) uint16_t map_cache[2][15 * 15];
}

int relative_cell(int c, int dir, int d, int x)
{
    int cx = (c & (W - 1)) + d * dx[dir] + x * dx[(dir + 1) & 3];
    int cy = (c >> 5) + d * dy[dir] + x * dy[(dir + 1) & 3];
    return (cx >= 0 && cx < W && cy >= 0 && cy < H) ? cy * W + cx : -1;
}

int edge_deco(const level& lv, const edge_state& es, int idx)
{
    if(idx < 0 || (lv.ed[idx] & 15) != K_WALL)
    {
        return DECO_NONE;
    }

    if(lever_of_edge(lv, idx) >= 0)
    {
        return es.open[idx] ? DECO_LEVER_DOWN : DECO_LEVER_UP;
    }

    switch(floor_tables[lv.floor_table].decor_of_var[lv.ed[idx] >> 4])
    {
    case DECOR_TORCH:
        return DECO_TORCH;
    case DECOR_BANNER:
        return DECO_BANNER;
    case DECOR_CHAINS:
        return DECO_CHAINS;
    default:
        return DECO_NONE;
    }
}

void draw(uint16_t* page, const level& lv, const edge_state& es, int cell, int dir, visibility& vis)
{
    const int left = (dir + 3) & 3;
    const int right = (dir + 1) & 3;

    // visibility flood: from behind through the far edge, or sideways from the inner neighbour
    for(int d = 0; d < rows; ++d)
    {
        for(int x = 0; x < cols; ++x)
        {
            vis.cell[d][x] = -1;
        }
    }

    static_assert(rows == vd::depths && cols == vd::cols);
    vis.cell[0][half] = int16_t(cell);
    vis.ndecos = 0;

    for(int d = 0; d < rows; ++d)
    {
        static constexpr int order[cols] = { 0, -1, 1, -2, 2, -3, 3 };

        for(int k = 0; k < cols; ++k)
        {
            int x = order[k];

            if(d == 0 && x == 0)
            {
                continue;
            }

            int c = relative_cell(cell, dir, d, x);

            if(c < 0)
            {
                continue;
            }

            bool v = false;

            if(d > 0)
            {
                int behind = vis.cell[d - 1][x + half];
                v = behind >= 0 && sight_passes(lv, es, edge_index(behind, dir));
            }

            if(! v && x != 0)
            {
                int inner = vis.cell[d][x + half + (x < 0 ? 1 : -1)];
                v = inner >= 0 && sight_passes(lv, es, edge_index(inner, x < 0 ? left : right));
            }

            if(v)
            {
                vis.cell[d][x + half] = int16_t(c);
            }
        }
    }

    for(int c = 0; c < owner_cols; ++c)
    {
        owner[c] = 0;
    }

    blit_piece(page, vd::background[((cell & (W - 1)) + (cell >> 5)) & 1]);

    for(int d = rows - 1; d >= 0; --d)
    {
        // vertex blocks on the far edge of row d (Z = far(d)), outer lines first: behind the faces of row d
        draw_vertices(page, lv, cell, dir, vis, d + 1);


        static constexpr int far_order[cols] = { -3, 3, -2, 2, -1, 1, 0 };

        for(int k = 0; k < cols; ++k)
        {
            int x = far_order[k];
            int c = vis.cell[d][x + half];

            if(c >= 0)
            {
                int idx = edge_index(c, dir);
                int set = edge_set(lv, es, idx);

                if(set >= 0)
                {
                    const vd::piece& p = vd::front[set][d][x + half];
                    blit_piece(page, p);

                    if(vd::set_opaque[set])
                    {
                        own(p, front_id(d, x + half));
                    }

                    add_deco(vis, edge_deco(lv, es, idx), d, x, false);
                }
            }
        }

        // lateral faces, outer lines first: line l = 2x - 1 (left edge of x <= 0) or 2x + 1 (right edge of x >= 0)
        static constexpr int lat_x[vd::lines] = { -3, 3, -2, 2, -1, 1, 0, 0 };
        static constexpr int lat_l[vd::lines] = { -7, 7, -5, 5, -3, 3, -1, 1 };


        for(int k = 0; k < vd::lines; ++k)
        {
            int c = vis.cell[d][lat_x[k] + half];

            if(c >= 0)
            {
                int l = lat_l[k];
                int idx = edge_index(c, l < 0 ? left : right);
                int set = edge_set(lv, es, idx);

                if(set >= 0)
                {
                    int li = (l + vd::lines - 1) >> 1;
                    const vd::piece& p = vd::lateral[set][d][li];
                    blit_piece(page, p);

                    if(vd::set_opaque[set])
                    {
                        own(p, lateral_id(d, li));
                    }

                    add_deco(vis, edge_deco(lv, es, idx), d, l, true);
                }
            }
        }
    }

    draw_vertices(page, lv, cell, dir, vis, 0);     // the blocks beside the player's own row

    // keep only the decorations whose face is what is seen around their anchor
    int n = 0;

    for(int i = 0; i < vis.ndecos; ++i)
    {
        const deco_place& p = vis.decos[i];
        int sx = p.lateral ? geo::deco_lateral_x(p.d, p.slot) : geo::deco_front_x(p.d, p.slot);
        uint8_t id = p.lateral ? lateral_id(p.d, (p.slot + vd::lines - 1) >> 1) : front_id(p.d, p.slot + half);

        if(sx >= 0 && sx < geo::view_w && owns(sx, id))
        {
            vis.decos[n++] = p;
        }
    }

    vis.ndecos = n;
}

bool occluded(int x0, int x1, int z_u)
{
    int c0 = geo::clamp(x0 >> 1, 0, owner_cols);
    int c1 = geo::clamp((x1 + 1) >> 1, 0, owner_cols);

    for(int c = c0; c < c1; ++c)
    {
        int id = owner[c];
        int z;

        if(id == 0)
        {
            continue;
        }
        else if(id < 64)
        {
            z = geo::front_face_u((id - 1) / cols);
        }
        else if(id < 128)
        {
            // lateral face of row d on line l: depth at this column from offset = 96 px * |X| / Z
            int d = (id - 64) / vd::lines;
            int l = 2 * ((id - 64) % vd::lines) - (vd::lines - 1);
            int dist = 2 * c + 1 - geo::vp_x;
            dist = dist < 0 ? -dist : dist;
            z = dist ? geo::proj_num / 5 * geo::lateral_face_abs_u(l) / dist : geo::far_u(d);
            z = geo::clamp(z, geo::near_u(d) + geo::wall_half_u, geo::far_u(d) - geo::wall_half_u);
        }
        else
        {
            int dv = (id - 128) / vd::lines;
            z = (dv ? geo::far_u(dv - 1) : geo::near_u(0)) - geo::wall_half_u;
        }

        if(z < z_u)
        {
            return true;
        }
    }

    return false;
}

void invalidate_minimap()
{
    for(int p = 0; p < 2; ++p)
    {
        for(int i = 0; i < 15 * 15; ++i)
        {
            map_cache[p][i] = 0xFFFF;
        }
    }
}

namespace
{
    // Minimap block keys per world cell, padded one cell east / south for the map's own S / E border lines.
    // A block is the 4x4 square of a cell: N line on its top row, W line on its left column, floor inside.
    // key = seen | N code << 1 | W code << 4 | corner << 7 (player << 8 is added at draw time)
    constexpr int kw = W + 1;
    constexpr int kh = H + 1;
    BN_DATA_EWRAM uint16_t world_keys[kw * kh];
    BN_DATA_EWRAM uint8_t key_dirty[kw * kh];
    BN_DATA_EWRAM uint16_t dirty_list[kw * kh];
    int dirty_count;

    bool seen_at(const uint8_t* seen, int x, int y)
    {
        return x >= 0 && x < W && y >= 0 && y < H && seen[y * W + x];
    }

    // edge code of side d (0 N, 3 W) of cell (x, y), which may lie just outside the map:
    // 0 open / arch (dark line), 1 wall, 2 door, 3 stairs, 4 bars, 5 raised bars, 6 lever wall
    int code_at(const level& lv, const edge_state& es, int x, int y, int d)
    {
        int idx;

        if(x >= 0 && x < W && y >= 0 && y < H)
        {
            idx = edge_index(y * W + x, d);
        }
        else
        {
            int nx = x + dx[d], ny = y + dy[d];

            if(nx < 0 || nx >= W || ny < 0 || ny >= H)
            {
                return 0;
            }

            idx = edge_index(ny * W + nx, d ^ 2);
        }

        if(idx < 0)
        {
            return 1;
        }

        switch(lv.ed[idx] & 15)
        {
        case K_OPEN:
        case K_ARCH:
            return 0;
        case K_DOOR:
        case K_LOCKED:
            return 2;
        case K_STAIRS:
            return 3;
        case K_BARS:
            return es.open[idx] ? 5 : 4;
        default:
            return lever_of_edge(lv, idx) >= 0 ? 6 : 1;
        }
    }

    uint16_t block_key(const level& lv, const edge_state& es, const uint8_t* seen, int x, int y)
    {
        bool s = seen_at(seen, x, y);
        bool sn = seen_at(seen, x, y - 1);
        bool sw = seen_at(seen, x - 1, y);
        int n = (s || sn) ? code_at(lv, es, x, y, 0) : 0;
        int w = (s || sw) ? code_at(lv, es, x, y, 3) : 0;
        bool corner = n || w;

        if(! corner)
        {
            bool snw = seen_at(seen, x - 1, y - 1);
            corner = ((sn || snw) && code_at(lv, es, x, y - 1, 3)) || ((sw || snw) && code_at(lv, es, x - 1, y, 0));
        }

        return uint16_t(int(s) | (n << 1) | (w << 4) | (int(corner) << 7));
    }
}

void minimap_reset(const level& lv, const edge_state& es, const uint8_t* seen)
{
    for(int y = 0; y < kh; ++y)
    {
        for(int x = 0; x < kw; ++x)
        {
            world_keys[y * kw + x] = block_key(lv, es, seen, x, y);
            key_dirty[y * kw + x] = 0;
        }
    }

    dirty_count = 0;
    invalidate_minimap();
}

void minimap_touch(int c)
{
    int cx = c & (W - 1);
    int cy = c >> 5;

    for(int y = cy; y <= cy + 1; ++y)
    {
        for(int x = cx; x <= cx + 1; ++x)
        {
            int k = y * kw + x;

            if(! key_dirty[k])
            {
                key_dirty[k] = 1;
                dirty_list[dirty_count++] = uint16_t(k);
            }
        }
    }
}

void minimap_update(const level& lv, const edge_state& es, const uint8_t* seen)
{
    for(int i = 0; i < dirty_count; ++i)
    {
        int k = dirty_list[i];
        key_dirty[k] = 0;
        world_keys[k] = block_key(lv, es, seen, k % kw, k / kw);
    }

    dirty_count = 0;
}

void draw_minimap(uint16_t* page, int page_index, int px, int py, int cell, int dir)
{
    // palette 240..247 (tools/build_view.py MAP_COLORS)
    constexpr uint8_t c_floor = 240, c_wall = 241, c_player = 242, c_door = 243, c_bars = 244, c_stairs = 245,
            c_lever = 246, c_dark = 247;
    static constexpr uint8_t code_a[8] = { c_dark, c_wall, c_door, c_stairs, c_bars, c_floor, c_wall, c_dark };
    static constexpr uint8_t code_b[8] = { c_dark, c_wall, c_door, c_stairs, c_floor, c_floor, c_lever, c_dark };

    int ox = (cell & (W - 1)) - 7;
    int oy = (cell >> 5) - 7;
    uint16_t* cache = map_cache[page_index & 1];

    for(int j = 0; j < 15; ++j)
    {
        int cy = oy + j;

        for(int i = 0; i < 15; ++i)
        {
            int cx = ox + i;
            int key = (cx >= 0 && cx < kw && cy >= 0 && cy < kh) ? world_keys[cy * kw + cx] : 0;

            if(i == 7 && j == 7)
            {
                key |= (dir + 1) << 8;
            }

            uint16_t& cached = cache[j * 15 + i];

            if(cached == key)
            {
                continue;
            }

            cached = uint16_t(key);
            int n = (key >> 1) & 7;
            int w = (key >> 4) & 7;
            uint8_t f = (key & 1) ? c_floor : c_dark;
            uint8_t b[4][4];
            b[0][0] = (key & 0x80) ? c_wall : c_dark;
            b[0][1] = code_a[n];
            b[0][2] = code_b[n];
            b[0][3] = code_a[n];

            for(int r = 1; r < 4; ++r)
            {
                b[r][0] = r == 2 ? code_b[w] : code_a[w];
                b[r][1] = f;
                b[r][2] = f;
                b[r][3] = f;
            }

            if(key >> 8)
            {
                // 2x2 body plus a heading pixel pair (N: top row, E: right column, S: bottom row, W: left column)
                b[1][1] = b[1][2] = b[2][1] = b[2][2] = c_player;
                static constexpr int8_t hx[4][2] = { { 1, 2 }, { 3, 3 }, { 1, 2 }, { 0, 0 } };
                static constexpr int8_t hy[4][2] = { { 0, 0 }, { 1, 2 }, { 3, 3 }, { 1, 2 } };
                b[hy[dir][0]][hx[dir][0]] = c_player;
                b[hy[dir][1]][hx[dir][1]] = c_player;
            }

            uint16_t* dst = page + (py + j * 4) * page_stride + (px >> 1) + i * 2;

            for(int r = 0; r < 4; ++r)
            {
                dst[0] = uint16_t(b[r][0] | (b[r][1] << 8));
                dst[1] = uint16_t(b[r][2] | (b[r][3] << 8));
                dst += page_stride;
            }
        }
    }
}
}
