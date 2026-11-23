#include "dl_blit.h"

namespace dl::view
{
namespace
{
    // n2 halfwords from src to dst; src and dst have the same word parity
    inline void copy_run(uint16_t* dst, const uint16_t* src, int n2)
    {
        if(uintptr_t(dst) & 2)
        {
            *dst++ = *src++;
            --n2;
        }

        auto d32 = reinterpret_cast<uint32_t*>(dst);
        auto s32 = reinterpret_cast<const uint32_t*>(src);
        int words = n2 >> 1;

        for(; words >= 4; words -= 4)
        {
            uint32_t a = s32[0], b = s32[1], c = s32[2], d = s32[3];
            d32[0] = a;
            d32[1] = b;
            d32[2] = c;
            d32[3] = d;
            d32 += 4;
            s32 += 4;
        }

        for(; words; --words)
        {
            *d32++ = *s32++;
        }

        if(n2 & 1)
        {
            *reinterpret_cast<uint16_t*>(d32) = *reinterpret_cast<const uint16_t*>(s32);
        }
    }
}

int blit_stat_spans;
int blit_stat_halfwords;

void blit_spans_clip(uint16_t* page, const uint16_t* pixels, const dl::view_data::span* s, int count, int u0, int u1)
{
    blit_stat_spans += count;
    for(const dl::view_data::span* end = s + count; s != end; ++s)
    {
        int a = s->x2;
        int b = a + s->n2;
        int skip = 0;

        if(a < u0)
        {
            skip = u0 - a;
            a = u0;
        }

        if(b > u1)
        {
            b = u1;
        }

        if(a < b)
        {
            copy_run(page + s->y * 120 + a, pixels + s->off + skip, b - a);
            blit_stat_halfwords += b - a;
        }
    }
}

void blit_background(uint16_t* page, const uint16_t* image, const uint8_t* cover, int vp_y)
{
    for(int y = 0; y < 120; ++y)
    {
        int dist = 2 * y + 1 - 2 * vp_y;
        dist = dist < 0 ? -dist : dist;
        const uint16_t* src = image + y * 88;
        uint16_t* dst = page + y * 120;
        int s = 0;

        while(s < background_strips)
        {
            if(2 * cover[s] > dist)
            {
                ++s;
                continue;
            }

            int a = s;

            while(s < background_strips && 2 * cover[s] <= dist)
            {
                ++s;
            }

            copy_run(dst + a * 4, src + a * 4, (s - a) * 4);
        }
    }
}
}
