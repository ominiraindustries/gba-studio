#include "dl_transition.h"

#include "dl_view_geometry.h"

namespace dl::view
{
namespace
{
    namespace geo = dl::geo;

    constexpr int vw = geo::view_w;
    constexpr int vh = geo::view_h;
    constexpr int page_stride = 120;    // uint16_t per 240-px row

    BN_DATA_EWRAM alignas(4) uint16_t snaps[snapshot_count][vh * vw / 2];
}

void snapshot(const uint16_t* page, int slot)
{
    auto dst = reinterpret_cast<uint32_t*>(snaps[slot]);

    for(int y = 0; y < vh; ++y)
    {
        auto src = reinterpret_cast<const uint32_t*>(page + y * page_stride);

        for(int x = 0; x < vw / 4; ++x)
        {
            *dst++ = src[x];
        }
    }
}

void compose_zoom(uint16_t* page, int slot, int scale_q8, int dy)
{
    int inv = (1 << 24) / scale_q8;     // q16
    // one source pixel per 2-px pair (measured: per-pixel sampling took ~1 f per frame, this ~0.6 f); the
    // picture is only on screen for the few frames of the motion
    alignas(4) uint8_t xmap[vw / 2];

    for(int x = 0; x < vw / 2; ++x)
    {
        int sx = geo::vp_x + (((2 * (2 * x + 1 - geo::vp_x)) * inv) >> 17);
        xmap[x] = uint8_t(sx < 0 ? 0 : (sx >= vw ? vw - 1 : sx));
    }

    alignas(4) uint8_t line[vw];
    int line_y = -1;
    const uint32_t* prev_row = nullptr;

    for(int y = 0; y < vh; ++y)
    {
        int sy = geo::vp_y + (((2 * (y - dy - geo::vp_y) + 1) * inv) >> 17);
        sy = sy < 0 ? 0 : (sy >= vh ? vh - 1 : sy);
        auto dst = reinterpret_cast<uint32_t*>(page + y * page_stride);

        if(sy == line_y)
        {
            // same source row as the row above: copy the finished row
            for(int x = 0; x < vw / 4; ++x)
            {
                dst[x] = prev_row[x];
            }
        }
        else
        {
            // the source row goes to an IWRAM line first (word reads from EWRAM), then is sampled per pixel
            auto src_words = reinterpret_cast<const uint32_t*>(snaps[slot]) + sy * (vw / 4);
            auto line_words = reinterpret_cast<uint32_t*>(line);

            for(int x = 0; x < vw / 4; ++x)
            {
                line_words[x] = src_words[x];
            }

            const uint8_t* m = xmap;

            for(int x = 0; x < vw / 4; ++x, m += 2)
            {
                dst[x] = (uint32_t(line[m[0]]) | (uint32_t(line[m[1]]) << 16)) * 0x101u;
            }

            line_y = sy;
        }

        prev_row = dst;
    }
}

void compose_slide(uint16_t* page, int from, int to, int offset, bool toward_left)
{
    int o = offset >> 1;                // halfwords
    constexpr int row = vw / 2;

    for(int y = 0; y < vh; ++y)
    {
        const uint16_t* a = snaps[from] + y * row;
        const uint16_t* b = snaps[to] + y * row;
        uint16_t* dst = page + y * page_stride;

        if(toward_left)
        {
            for(int x = 0; x < row - o; ++x)
            {
                dst[x] = a[x + o];
            }

            for(int x = row - o; x < row; ++x)
            {
                dst[x] = b[x - (row - o)];
            }
        }
        else
        {
            for(int x = 0; x < o; ++x)
            {
                dst[x] = b[x + row - o];
            }

            for(int x = o; x < row; ++x)
            {
                dst[x] = a[x - o];
            }
        }
    }
}
}
