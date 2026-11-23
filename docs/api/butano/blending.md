# Butano 21.10.1 — Blending (`blending`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_blending.h

### `bn::blending` (namespace, bn_blending.h:29)

Blending related functions.

### `bn::blending::fade_color_type` (enum, bn_blending.h:34)

Available fade colors.

`enum class fade_color_type : uint8_t { BLACK /* Black fade color. */, WHITE /* White fade color. */ }`

- **transparency_alpha** `fixed transparency_alpha()` (L44): Returns the weight of the transparency blending, with 0 being fully transparent and 1 being fully visible.
- **set_transparency_alpha** `void set_transparency_alpha(fixed transparency_alpha)` (L54): Sets the weight of the transparency blending, with 0 being fully transparent and 1 being fully visible.
  `transparency_alpha` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **intensity_alpha** `fixed intensity_alpha()` (L59): Returns the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
- **set_intensity_alpha** `void set_intensity_alpha(fixed intensity_alpha)` (L68): Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
  `intensity_alpha` Intensity weight in the range [0..1].
  Keep in mind that intensity and fade blendings can't be enabled at the same time.
- **set_transparency_and_intensity_alpha** `void set_transparency_and_intensity_alpha(fixed transparency_alpha, fixed intensity_alpha)` (L79): Sets the weights of the transparency and the intensity blendings.
  `transparency_alpha` Transparency weight in the range [0..1], with 0 being fully transparent and 1 being fully visible.; `intensity_alpha` Intensity weight in the range [0..1], with 0 being disabled and 1 being fully applied.
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **transparency_top_weight** `fixed transparency_top_weight()` (L85): Returns the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
- **set_transparency_top_weight** `void set_transparency_top_weight(fixed transparency_top_weight)` (L95): Sets the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_top_weight` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **transparency_bottom_weight** `fixed transparency_bottom_weight()` (L101): Returns the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
- **set_transparency_bottom_weight** `void set_transparency_bottom_weight(fixed transparency_bottom_weight)` (L111): Sets the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_bottom_weight` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **set_transparency_weights** `void set_transparency_weights(fixed transparency_top_weight, fixed transparency_bottom_weight)` (L122): Sets the weights of the transparency blending, with 0 being fully transparent and 1 being fully visible.
  `transparency_top_weight` Transparency weight in the range [0..1].; `transparency_bottom_weight` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **transparency_attributes** `blending_transparency_attributes transparency_attributes()` (L127): Returns the attributes to commit to the GBA transparency and intensity blending register.
- **set_transparency_attributes** `void set_transparency_attributes(const blending_transparency_attributes& transparency_attributes)` (L132): Sets the attributes to commit to the GBA transparency and intensity blending register.
- **fade_color** `fade_color_type fade_color()` (L137): Returns the color used for fade blending.
- **set_fade_color** `void set_fade_color(fade_color_type fade_color)` (L142): Sets the color used for fade blending.
- **set_black_fade_color** `void set_black_fade_color()` (L147): Sets black as the color used for fade blending.
- **set_white_fade_color** `void set_white_fade_color()` (L152): Sets white as the color used for fade blending.
- **fade_alpha** `fixed fade_alpha()` (L157): Returns the weight of the fade blending, with 0 being disabled and 1 being fully applied.
- **set_fade_alpha** `void set_fade_alpha(fixed fade_alpha)` (L166): Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
  `fade_alpha` Fade weight in the range [0..1].
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
- **set_fade_alpha** `void set_fade_alpha(blending_fade_alpha fade_alpha)` (L175): Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
  `fade_alpha` blending_fade_alpha to set.
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
- **restore** `void restore()` (L180): Restores blending to its initial state.

## bn_blending_fade_alpha.h

### `bn::blending_fade_alpha` (class, bn_blending_fade_alpha.h:27)

Manages the attributes to commit to the GBA fade blending register.

`class blending_fade_alpha`

- **blending_fade_alpha** `constexpr blending_fade_alpha() = default` (L34): Default constructor.
- **blending_fade_alpha** `constexpr explicit blending_fade_alpha(fixed value)` (L40): Constructor.
  `value` Fade weight in the range [0..1], with 0 being disabled and 1 being fully applied.
- **value** `constexpr fixed value() const` (L49): Returns the weight of the fade blending, with 0 being disabled and 1 being fully applied.
- **set_value** `constexpr void set_value(fixed value)` (L58): Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
  `value` Fade weight in the range [0..1].
- **operator==** `constexpr friend bool operator==( const blending_fade_alpha& a, const blending_fade_alpha& b) = default` (L68): Default equal operator.

## bn_blending_transparency_attributes.h

### `bn::blending_transparency_attributes` (class, bn_blending_transparency_attributes.h:27)

Manages the attributes to commit to the GBA transparency and intensity blendings register.

`class blending_transparency_attributes`

- **blending_transparency_attributes** `constexpr blending_transparency_attributes() = default` (L34): Default constructor.
- **blending_transparency_attributes** `constexpr blending_transparency_attributes(fixed transparency_top_weight, fixed transparency_bottom_weight)` (L43): Constructor.
  `transparency_top_weight` Transparency weight in the range [0..1] for the top layer, with 0 being fully transparent and 1 being fully visible.; `transparency_bottom_weight` Transparency weight in the range [0..1] for the bottom layer, with 0 being fully transparent and 1 being fully visible.
- **transparency_top_weight** `constexpr fixed transparency_top_weight() const` (L57): Returns the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
- **set_transparency_top_weight** `constexpr void set_transparency_top_weight(fixed transparency_top_weight)` (L67): Sets the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_top_weight` Transparency weight in the range [0..1].
- **transparency_bottom_weight** `constexpr fixed transparency_bottom_weight() const` (L79): Returns the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
- **set_transparency_bottom_weight** `constexpr void set_transparency_bottom_weight(fixed transparency_bottom_weight)` (L89): Sets the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_bottom_weight` Transparency weight in the range [0..1].
- **set_transparency_alpha** `constexpr void set_transparency_alpha(fixed transparency_alpha)` (L102): Sets the weight of the transparency blending, with 0 being fully transparent and 1 being fully visible.
  `transparency_alpha` Transparency weight in the range [0..1].
- **set_intensity_alpha** `constexpr void set_intensity_alpha(fixed intensity_alpha)` (L115): Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
  `intensity_alpha` Intensity weight in the range [0..1].
- **set_transparency_and_intensity_alphas** `constexpr void set_transparency_and_intensity_alphas(fixed transparency_alpha, fixed intensity_alpha)` (L131): Sets the weights of the transparency and the the intensity blendings.
  `transparency_alpha` Transparency weight in the range [0..1], with 0 being fully transparent and 1 being fully visible.; `intensity_alpha` Intensity weight in the range [0..1], with 0 being disabled and 1 being fully applied.
- **operator==** `constexpr friend bool operator==(const blending_transparency_attributes& a, const blending_transparency_attributes& b) = default` (L145): Default equal operator.
