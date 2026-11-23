// Span copy of the view pieces, in IWRAM as ARM code (measured: see G-25 notes).
#ifndef DL_BLIT_H
#define DL_BLIT_H

#include "bn_common.h"
#include "dl_view_data.h"

namespace dl::view
{
    // Copies `count` spans (data at `pixels`, with the page's word parity) into the 240-px page (2 px per uint16_t),
    // every span clipped to the 2-px columns [u0, u1) (G-31: the columns nearer opaque pieces leave uncovered).
    BN_CODE_IWRAM void blit_spans_clip(uint16_t* page, const uint16_t* pixels, const dl::view_data::span* s, int count,
                                       int u0, int u1);

    // Floor / ceiling image (88 halfwords per row, 120 rows) into the view, skipping in each 8-px strip the rows a
    // wall covers: |y + 0.5 - vp_y| < cover[strip] px (G-31).
    constexpr int background_strips = 22;
    BN_CODE_IWRAM void blit_background(uint16_t* page, const uint16_t* image, const uint8_t* cover, int vp_y);

    // profiler counters (spans visited, halfwords copied), reset by the caller
    extern int blit_stat_spans;
    extern int blit_stat_halfwords;
}

#endif
