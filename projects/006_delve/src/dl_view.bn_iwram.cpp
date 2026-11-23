// DELVE view renderer (see dl_view.h), compiled as ARM in IWRAM (*.bn_iwram.cpp; measured in G-25). Pieces and geometry come from dl_view_data (generated) and
// dl_view_geometry.h; this file only decides which edge face goes where and copies the spans.

#include "dl_view.h"

#include "dl_blit.h"
#include "dl_view_data.h"
#include "dl_view_geometry.h"

namespace dl::view
{
namespace
{
    using namespace dl::gen;
    namespace vd = dl::view_data;

    constexpr int page_stride = 120;    // uint16_t per 240-px row

    void blit_piece(uint16_t* page, const vd::piece& p)
    {
        blit_spans(page, vd::spans + p.first, p.count);
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
            if(lever_of_edge(lv, idx) >= 0)
            {
                return es.open[idx] ? vd::set_wall_lever_down : vd::set_wall_lever_up;
            }

            static constexpr int8_t set_of_decor[DECOR_COUNT] = {
                vd::set_wall, vd::set_wall_torch, vd::set_wall_banner, vd::set_wall_chains, vd::set_wall_skulls,
                vd::set_wall_crack, vd::set_wall
            };
            int decor = floor_tables[lv.floor_table].decor_of_var[var];
            return set_of_decor[decor < DECOR_COUNT ? decor : 0];
        }

        case K_SECRET:
            return vd::set_wall_crack;

        case K_DOOR:
        case K_LOCKED:      // PROVISIONAL: no lock texture yet
            return es.open[idx] ? vd::set_door_open : vd::set_door_closed;

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
        return sight_kind[k] || (k == K_BARS && es.open[idx]);
    }

    alignas(4) uint8_t map_buffer[64 * 64];
}

int relative_cell(int c, int dir, int d, int x)
{
    int cx = (c & (W - 1)) + d * dx[dir] + x * dx[(dir + 1) & 3];
    int cy = (c >> 5) + d * dy[dir] + x * dy[(dir + 1) & 3];
    return (cx >= 0 && cx < W && cy >= 0 && cy < H) ? cy * W + cx : -1;
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

    vis.cell[0][2] = int16_t(cell);

    for(int d = 0; d < rows; ++d)
    {
        static constexpr int order[cols] = { 0, -1, 1, -2, 2 };

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
                int behind = vis.cell[d - 1][x + 2];
                v = behind >= 0 && sight_passes(lv, es, edge_index(behind, dir));
            }

            if(! v && x != 0)
            {
                int inner = vis.cell[d][x + 2 + (x < 0 ? 1 : -1)];
                v = inner >= 0 && sight_passes(lv, es, edge_index(inner, x < 0 ? left : right));
            }

            if(v)
            {
                vis.cell[d][x + 2] = int16_t(c);
            }
        }
    }

    blit_piece(page, vd::background);

    for(int d = rows - 1; d >= 0; --d)
    {
        static constexpr int far_order[cols] = { -2, 2, -1, 1, 0 };

        for(int k = 0; k < cols; ++k)
        {
            int x = far_order[k];
            int c = vis.cell[d][x + 2];

            if(c >= 0)
            {
                int set = edge_set(lv, es, edge_index(c, dir));

                if(set >= 0)
                {
                    blit_piece(page, vd::front[set][d][x + 2]);
                }
            }
        }

        // lateral faces, outer lines first: line l = 2x - 1 (left edge of x <= 0) or 2x + 1 (right edge of x >= 0)
        static constexpr int lat_x[6] = { -2, 2, -1, 1, 0, 0 };
        static constexpr int lat_l[6] = { -5, 5, -3, 3, -1, 1 };

        for(int k = 0; k < 6; ++k)
        {
            int c = vis.cell[d][lat_x[k] + 2];

            if(c >= 0)
            {
                int l = lat_l[k];
                int set = edge_set(lv, es, edge_index(c, l < 0 ? left : right));

                if(set >= 0)
                {
                    blit_piece(page, vd::lateral[set][d][(l + 5) >> 1]);
                }
            }
        }
    }
}

void draw_minimap(uint16_t* page, int px, int py, const level& lv, const edge_state& es, const uint8_t* seen,
                  int cell, int dir)
{
    constexpr uint8_t c_floor = 240, c_wall = 241, c_player = 242, c_door = 243, c_bars = 244, c_stairs = 245,
            c_lever = 246, c_dark = 247;
    constexpr int size = 60;
    constexpr int stride = 64;      // 64x64 buffer: the south / east lines of the last row never overflow
    uint8_t* buf = map_buffer;
    auto words = reinterpret_cast<uint32_t*>(buf);

    for(int i = 0; i < stride * stride / 4; ++i)
    {
        words[i] = c_dark * 0x01010101u;
    }

    int ox = (cell & (W - 1)) - 7;
    int oy = (cell >> 5) - 7;

    for(int j = 0; j < 15; ++j)
    {
        int cy = oy + j;

        if(cy < 0 || cy >= H)
        {
            continue;
        }

        for(int i = 0; i < 15; ++i)
        {
            int cx = ox + i;

            if(cx < 0 || cx >= W)
            {
                continue;
            }

            int c = cy * W + cx;

            if(! seen[c])
            {
                continue;
            }

            uint8_t* p = buf + (j * 4) * stride + i * 4;

            for(int yy = 1; yy < 4; ++yy)
            {
                p[yy * stride + 1] = c_floor;
                p[yy * stride + 2] = c_floor;
                p[yy * stride + 3] = c_floor;
            }

            for(int d = 0; d < 4; ++d)
            {
                int idx = edge_index(c, d);
                int k = idx >= 0 ? (lv.ed[idx] & 15) : K_WALL;

                if(k == K_OPEN || k == K_ARCH)
                {
                    continue;
                }

                uint8_t col = k == K_DOOR || k == K_LOCKED ? c_door : (k == K_STAIRS ? c_stairs : c_wall);
                uint8_t alt = col;

                if(k == K_BARS)
                {
                    col = es.open[idx] ? c_floor : c_bars;
                    alt = c_floor;
                }

                uint8_t mid = (k == K_WALL && idx >= 0 && lever_of_edge(lv, idx) >= 0) ? c_lever : alt;
                int step = (d & 1) ? stride : 1;
                uint8_t* q = p + (d == 1 ? 4 : 0) + (d == 2 ? 4 * stride : 0);
                q[0] = col;
                q[step] = alt;
                q[2 * step] = mid;
                q[3 * step] = alt;
                q[4 * step] = col;
            }
        }
    }

    // player: 2x2 in the centre cell plus a heading pixel
    uint8_t* pc = buf + (7 * 4 + 1) * stride + 7 * 4 + 1;
    pc[stride + 1] = c_player;
    pc[stride + 2] = c_player;
    pc[2 * stride + 1] = c_player;
    pc[2 * stride + 2] = c_player;
    static constexpr int hx[4] = { 1, 3, 1, 0 };
    static constexpr int hy[4] = { 0, 1, 3, 1 };
    pc[hy[dir] * stride + hx[dir]] = c_player;
    pc[(dir == 0 || dir == 2 ? hy[dir] : hy[dir] + 1) * stride + (dir == 0 || dir == 2 ? hx[dir] + 1 : hx[dir])] =
            c_player;

    for(int y = 0; y < size; ++y)
    {
        uint16_t* dst = page + (py + y) * page_stride + (px >> 1);
        const uint16_t* src = reinterpret_cast<const uint16_t*>(buf + y * stride);

        for(int x = 0; x < size / 2; ++x)
        {
            dst[x] = src[x];
        }
    }
}
}
