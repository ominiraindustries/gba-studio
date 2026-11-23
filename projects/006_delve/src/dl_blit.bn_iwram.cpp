#include "dl_blit.h"

namespace dl::view
{
    void blit_spans(uint16_t* page, const dl::view_data::span* s, int count)
    {
        const uint16_t* pixels = dl::view_data::pixels;

        for(const dl::view_data::span* end = s + count; s != end; ++s)
        {
            uint16_t* dst = page + s->y * 120 + s->x2;
            const uint16_t* src = pixels + s->off;
            int n2 = s->n2;

            if(s->x2 & 1)
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
}
