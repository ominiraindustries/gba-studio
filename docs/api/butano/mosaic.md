# Butano 21.10.1 — Mosaic (`mosaic`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_bgs_mosaic.h

### `bn::bgs_mosaic` (namespace, bn_bgs_mosaic.h:25)

Functions related with the mosaic applied to the backgrounds.

- **horizontal_stretch** `fixed horizontal_stretch()` (L30): Returns the horizontal stretch of the mosaic applied to the backgrounds.
- **set_horizontal_stretch** `void set_horizontal_stretch(fixed horizontal_stretch)` (L36): Sets the horizontal stretch of the mosaic applied to the backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
- **vertical_stretch** `fixed vertical_stretch()` (L41): Returns the vertical stretch of the mosaic applied to the backgrounds.
- **set_vertical_stretch** `void set_vertical_stretch(fixed vertical_stretch)` (L47): Sets the vertical stretch of the mosaic applied to the backgrounds.
  `vertical_stretch` Vertical stretch in the range [0..1].
- **set_stretch** `void set_stretch(fixed stretch)` (L53): Sets the stretch of the mosaic applied to the backgrounds.
  `stretch` Stretch in the range [0..1].
- **set_stretch** `void set_stretch(fixed horizontal_stretch, fixed vertical_stretch)` (L60): Sets the stretch of the mosaic applied to the backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].; `vertical_stretch` Vertical stretch in the range [0..1].

## bn_mosaic_attributes.h

### `bn::mosaic_attributes` (class, bn_mosaic_attributes.h:27)

Manages the attributes to commit to the GBA mosaic register.

`class mosaic_attributes`

- **mosaic_attributes** `constexpr mosaic_attributes() = default` (L34): Default constructor.
- **mosaic_attributes** `constexpr mosaic_attributes(fixed sprites_horizontal_stretch, fixed sprites_vertical_stretch, fixed bgs_horizontal_stretch, fixed bgs_vertical_stretch)` (L43): Constructor.
  `sprites_horizontal_stretch` Horizontal stretch of the mosaic applied to the sprites, in the range [0..1].; `sprites_vertical_stretch` Vertical stretch of the mosaic applied to the sprites, in the range [0..1].; `bgs_horizontal_stretch` Horizontal stretch of the mosaic applied to the backgrounds, in the range [0..1].; `bgs_vertical_stretch` Vertical stretch of the mosaic applied to the backgrounds, in the range [0..1].
- **sprites_horizontal_stretch** `constexpr fixed sprites_horizontal_stretch() const` (L63): Returns the horizontal stretch of the mosaic applied to the sprites.
- **set_sprites_horizontal_stretch** `constexpr void set_sprites_horizontal_stretch(fixed horizontal_stretch)` (L72): Sets the horizontal stretch of the mosaic applied to the sprites.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
- **sprites_vertical_stretch** `constexpr fixed sprites_vertical_stretch() const` (L83): Returns the vertical stretch of the mosaic applied to the sprites.
- **set_sprites_vertical_stretch** `constexpr void set_sprites_vertical_stretch(fixed vertical_stretch)` (L92): Sets the vertical stretch of the mosaic applied to the sprites.
  `vertical_stretch` Vertical stretch in the range [0..1].
- **set_sprites_stretch** `constexpr void set_sprites_stretch(fixed stretch)` (L103): Sets the stretch of the mosaic applied to the sprites.
  `stretch` Stretch in the range [0..1].
- **set_sprites_stretch** `constexpr void set_sprites_stretch(fixed horizontal_stretch, fixed vertical_stretch)` (L116): Sets the stretch of the mosaic applied to the sprites.
  `horizontal_stretch` Horizontal stretch in the range [0..1].; `vertical_stretch` Vertical stretch in the range [0..1].
- **bgs_horizontal_stretch** `constexpr fixed bgs_horizontal_stretch() const` (L129): Returns the horizontal stretch of the mosaic applied to the backgrounds.
- **set_bgs_horizontal_stretch** `constexpr void set_bgs_horizontal_stretch(fixed horizontal_stretch)` (L138): Sets the horizontal stretch of the mosaic applied to the backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
- **bgs_vertical_stretch** `constexpr fixed bgs_vertical_stretch() const` (L149): Returns the vertical stretch of the mosaic applied to the backgrounds.
- **set_bgs_vertical_stretch** `constexpr void set_bgs_vertical_stretch(fixed vertical_stretch)` (L158): Sets the vertical stretch of the mosaic applied to the backgrounds.
  `vertical_stretch` Vertical stretch in the range [0..1].
- **set_bgs_stretch** `constexpr void set_bgs_stretch(fixed stretch)` (L169): Sets the stretch of the mosaic applied to the backgrounds.
  `stretch` Stretch in the range [0..1].
- **set_bgs_stretch** `constexpr void set_bgs_stretch(fixed horizontal_stretch, fixed vertical_stretch)` (L182): Sets the stretch of the mosaic applied to the backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].; `vertical_stretch` Vertical stretch in the range [0..1].
- **set_horizontal_stretch** `constexpr void set_horizontal_stretch(fixed horizontal_stretch)` (L196): Sets the horizontal stretch of the mosaic applied to both sprites and backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
- **set_vertical_stretch** `constexpr void set_vertical_stretch(fixed vertical_stretch)` (L209): Sets the vertical stretch of the mosaic applied to both sprites and backgrounds.
  `vertical_stretch` Vertical stretch in the range [0..1].
- **set_stretch** `constexpr void set_stretch(fixed stretch)` (L221): Sets the stretch of the mosaic applied to both sprites and backgrounds.
  `stretch` Stretch in the range [0..1].
- **set_stretch** `constexpr void set_stretch(fixed horizontal_stretch, fixed vertical_stretch)` (L236): Sets the stretch of the mosaic applied to both sprites and backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].; `vertical_stretch` Vertical stretch in the range [0..1].
- **operator==** `constexpr friend bool operator==(const mosaic_attributes& a, const mosaic_attributes& b) = default` (L251): Default equal operator.

## bn_sprites_mosaic.h

### `bn::sprites_mosaic` (namespace, bn_sprites_mosaic.h:25)

Functions related with the mosaic applied to the sprites.

- **horizontal_stretch** `fixed horizontal_stretch()` (L30): Returns the horizontal stretch of the mosaic applied to the sprites.
- **set_horizontal_stretch** `void set_horizontal_stretch(fixed horizontal_stretch)` (L36): Sets the horizontal stretch of the mosaic applied to the sprites.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
- **vertical_stretch** `fixed vertical_stretch()` (L41): Returns the vertical stretch of the mosaic applied to the sprites.
- **set_vertical_stretch** `void set_vertical_stretch(fixed vertical_stretch)` (L47): Sets the vertical stretch of the mosaic applied to the sprites.
  `vertical_stretch` Vertical stretch in the range [0..1].
- **set_stretch** `void set_stretch(fixed stretch)` (L53): Sets the stretch of the mosaic applied to the sprites.
  `stretch` Stretch in the range [0..1].
- **set_stretch** `void set_stretch(fixed horizontal_stretch, fixed vertical_stretch)` (L60): Sets the stretch of the mosaic applied to the sprites.
  `horizontal_stretch` Horizontal stretch in the range [0..1].; `vertical_stretch` Vertical stretch in the range [0..1].
