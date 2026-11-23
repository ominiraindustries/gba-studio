# Butano 21.10.1 — Bitmap backgrounds (`bitmap_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bg](bg.md)

## bn_bitmap_bg.h

### `bn::bitmap_bg` (namespace, bn_bitmap_bg.h:24)

Bitmap background related functions.

- **palette_width** `constexpr int palette_width()` (L29): Returns the width of palette bitmap backgrounds in pixels (240).
- **palette_height** `constexpr int palette_height()` (L37): Returns the height of palette bitmap backgrounds in pixels (160).
- **palette_size** `constexpr size palette_size()` (L45): Returns the size of palette bitmap backgrounds in pixels (240x160).
- **sp_direct_width** `constexpr int sp_direct_width()` (L53): Returns the width of single page direct bitmap backgrounds in pixels (240).
- **sp_direct_height** `constexpr int sp_direct_height()` (L61): Returns the height of single page direct bitmap backgrounds in pixels (160).
- **sp_direct_size** `constexpr size sp_direct_size()` (L69): Returns the size of single page direct bitmap backgrounds in pixels (240x160).
- **dp_direct_width** `constexpr int dp_direct_width()` (L77): Returns the width of dual page direct bitmap backgrounds in pixels (160).
- **dp_direct_height** `constexpr int dp_direct_height()` (L85): Returns the height of dual page direct bitmap backgrounds in pixels (128).
- **dp_direct_size** `constexpr size dp_direct_size()` (L93): Returns the size of dual page direct bitmap backgrounds in pixels (160x128).

## bn_bitmap_bg_attributes.h

### `bn::bitmap_bg_attributes` (class, bn_bitmap_bg_attributes.h:28)

Manages the attributes to commit to the GBA register of a bitmap background.

`class bitmap_bg_attributes`

- **bitmap_bg_attributes** `bitmap_bg_attributes(int priority, bool mosaic_enabled)` (L41): Constructor.
  `priority` Priority of a background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to an affine background or not.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **bitmap_bg_attributes** `bitmap_bg_attributes(int priority, bool mosaic_enabled, bn::green_swap_mode green_swap_mode)` (L53): Constructor.
  `priority` Priority of a background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to a background or not.; `green_swap_mode` Indicates how a background must be displayed when green swap is enabled.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **priority** `int priority() const` (L61): Returns the priority of a background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L74): Sets the priority of a background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L79): Indicates if the mosaic effect is applied to a background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L87): Sets if the mosaic effect must be applied to a background or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L95): Indicates how a background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L103): Sets how a background must be displayed when green swap is enabled.
- **operator==** `friend bool operator==(const bitmap_bg_attributes& a, const bitmap_bg_attributes& b) = default` (L111): Default equal operator.
