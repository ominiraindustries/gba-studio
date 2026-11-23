// DELVE step / turn transitions composed in software on the view rectangle only (BG2 is affine in mode 4, but the
// panel and the bottom strip live in the same bitmap, so a hardware zoom or scroll would move them too).
// Snapshots of the 176x120 view are kept in EWRAM; composition runs from IWRAM (measured in G-25).

#ifndef DL_TRANSITION_H
#define DL_TRANSITION_H

#include <stdint.h>

#include "bn_common.h"

namespace dl::view
{
    constexpr int snapshot_count = 2;

    // Mode 4 has two pages 0xA000 bytes apart: the one on screen is the one that is not the back page.
    [[nodiscard]] inline const uint16_t* front_page(const uint16_t* back_page)
    {
        constexpr uintptr_t base = 0x06000000;
        return reinterpret_cast<const uint16_t*>(uintptr_t(back_page) == base ? base + 0xA000 : base);
    }

    // copies the view rectangle of `page` to snapshot `slot`
    BN_CODE_IWRAM void snapshot(const uint16_t* page, int slot);

    // snapshot `slot` scaled by scale_q8 / 256 around the vanishing point and moved down `dy` px, into `page`
    BN_CODE_IWRAM void compose_zoom(uint16_t* page, int slot, int scale_q8, int dy);

    // turn: `from` slides out and `to` slides in by `offset` px (even, 0..176); toward_left: the picture moves left
    // (turning right)
    BN_CODE_IWRAM void compose_slide(uint16_t* page, int from, int to, int offset, bool toward_left);
}

#endif
