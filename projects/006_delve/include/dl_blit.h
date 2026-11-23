// Span copy of the view pieces, in IWRAM as ARM code (measured: see G-25 notes).
#ifndef DL_BLIT_H
#define DL_BLIT_H

#include "bn_common.h"
#include "dl_view_data.h"

namespace dl::view
{
    // Copies `count` spans into the 240-px page (2 px per uint16_t). Span data has the page's word parity.
    BN_CODE_IWRAM void blit_spans(uint16_t* page, const dl::view_data::span* s, int count);
}

#endif
