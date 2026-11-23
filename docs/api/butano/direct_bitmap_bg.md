# Butano 21.10.1 — Direct bitmap backgrounds (`direct_bitmap_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bitmap_bg](bitmap_bg.md)

## bn_direct_bitmap_roi.h

### `bn::direct_bitmap_roi` (class, bn_direct_bitmap_roi.h:28)

Contains the required information to represent a region of interest (RoI) of a direct bitmap item.

`class direct_bitmap_roi`

The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.

- **direct_bitmap_roi** `constexpr direct_bitmap_roi(const direct_bitmap_item& item_ref, int x, int y, int width, int height) : direct_bitmap_roi(item_ref, point(x, y), size(width, height))` (L43): Constructor.
  `item_ref` Reference to the direct bitmap item.; `x` Horizontal position of the top-left corner of the region of interest.; `y` Vertical position of the top-left corner of the region of interest.; `width` Width in pixels of the region of interest.; `height` Height in pixels of the region of interest.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **direct_bitmap_roi** `constexpr direct_bitmap_roi(const direct_bitmap_item& item_ref, const point& position, const size& dimensions)` (L57): Constructor.
  `item_ref` Reference to the direct bitmap item.; `position` Position of the top-left corner of the region of interest.; `dimensions` Size in pixels of the region of interest.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **item_ref** `constexpr const direct_bitmap_item& item_ref() const` (L77): Returns the referenced direct bitmap item.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **x** `constexpr int x() const` (L85): Returns the horizontal position of the top-left corner of the region of interest.
- **y** `constexpr int y() const` (L93): Returns the vertical position of the top-left corner of the region of interest.
- **position** `constexpr const point& position() const` (L101): Returns the position of the top-left corner of the region of interest.
- **width** `constexpr int width() const` (L109): Returns the width in pixels of the region of interest.
- **height** `constexpr int height() const` (L117): Returns the height in pixels of the region of interest.
- **dimensions** `constexpr const size& dimensions() const` (L125): Returns the size in pixels of the region of interest.
- **colors_count** `constexpr int colors_count() const` (L133): Returns the number of referenced colors.
- **color_index** `constexpr int color_index(int x, int y) const` (L147): Returns the index of the referenced color in the specified pixel coordinates.
  `x` Horizontal position of the direct bitmap color [0..dimensions().width()).; `y` Vertical position of the direct bitmap color [0..dimensions().height()).; devuelve: The index of the referenced direct bitmap color.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **color_index** `constexpr int color_index(const point& position) const` (L166): Returns the index of the referenced color in the specified pixel coordinates.
  `position` Position of the direct bitmap color.; devuelve: The index of the referenced direct bitmap color.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **color** `constexpr bn::color color(int x, int y) const` (L180): Returns the referenced direct bitmap color in the specified pixel coordinates.
  `x` Horizontal position of the direct bitmap color [0..dimensions().width()).; `y` Vertical position of the direct bitmap color [0..dimensions().height()).; devuelve: The referenced direct bitmap color.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **color** `constexpr bn::color color(const point& position) const` (L193): Returns the referenced direct bitmap color in the specified pixel coordinates.
  `position` Position of the direct bitmap color.; devuelve: The referenced direct bitmap color.
  The item is not copied but referenced, so it should outlive the direct_bitmap_roi to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const direct_bitmap_roi& a, const direct_bitmap_roi& b) = default` (L201): Default equal operator.
