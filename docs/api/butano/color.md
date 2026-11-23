# Butano 21.10.1 — Colors (`color`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_bpp_mode.h

### `bn::bpp_mode` (enum, bn_bpp_mode.h:26)

Specifies the available bits per pixel modes.

`enum class bpp_mode : uint8_t { BPP_4 /* 4 bits per pixel (16 colors). */, BPP_8 /* 8 bits per pixel (256 colors). */ }`


## bn_color.h

### `bn::color` (class, bn_color.h:27)

Defines a color in the GBA format (15 bits BGR).

`class color`

- **color** `constexpr color() = default` (L34): Default constructor.
- **color** `constexpr explicit color(int data)` (L40): Constructor.
  `data` Color value in GBA format.
- **color** `constexpr color(int red, int green, int blue)` (L52): Constructor.
  `red` Red component, in the range [0..31].; `green` Green component, in the range [0..31].; `blue` Blue component, in the range [0..31].
- **data** `constexpr int data() const` (L63): Returns the representation of this color in GBA format.
- **set_data** `constexpr void set_data(int data)` (L71): Sets the representation of this color in GBA format.
- **set_components** `constexpr void set_components(int red, int green, int blue)` (L84): Sets all the components of this color.
  `red` Red component, in the range [0..31].; `green` Green component, in the range [0..31].; `blue` Blue component, in the range [0..31].
- **red** `constexpr int red() const` (L96): Returns the red component.
- **set_red** `constexpr void set_red(int red)` (L105): Sets the red component.
  `red` Red component, in the range [0..31].
- **green** `constexpr int green() const` (L115): Returns the green component.
- **set_green** `constexpr void set_green(int green)` (L124): Sets the green component.
  `green` Green component, in the range [0..31].
- **blue** `constexpr int blue() const` (L134): Returns the blue component.
- **set_blue** `constexpr void set_blue(int blue)` (L143): Sets the blue component.
  `blue` Blue component, in the range [0..31].
- **operator==** `constexpr friend bool operator==(color a, color b) = default` (L153): Default equal operator.

## bn_color_effect.h

### `bn::color_effect` (namespace, bn_color_effect.h:29)

Color effects related functions.

- **brightness** `void brightness(fixed brightness, span<color> colors_ref)` (L37): Increases the brightness of all referenced colors.
  `brightness` Brightness amount in the range [0..1].; `colors_ref` Colors reference.
- **brightness** `void brightness(const span<const color>& source_colors_ref, fixed brightness, span<color> destination_colors_ref)` (L46): Increases the brightness of all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `brightness` Brightness amount in the range [0..1].; `destination_colors_ref` Destination colors reference.
- **contrast** `void contrast(fixed contrast, span<color> colors_ref)` (L53): Increases the contrast of all referenced colors.
  `contrast` Contrast amount in the range [0..1].; `colors_ref` Colors reference.
- **contrast** `void contrast(const span<const color>& source_colors_ref, fixed contrast, span<color> destination_colors_ref)` (L62): Increases the contrast of all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `contrast` Contrast amount in the range [0..1].; `destination_colors_ref` Destination colors reference.
- **intensity** `void intensity(fixed intensity, span<color> colors_ref)` (L69): Increases the intensity of all referenced colors.
  `intensity` Intensity amount in the range [0..1].; `colors_ref` Colors reference.
- **intensity** `void intensity(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref)` (L78): Increases the intensity of all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `intensity` Intensity amount in the range [0..1].; `destination_colors_ref` Destination colors reference.
- **invert** `void invert(span<color> colors_ref)` (L84): Inverts all referenced colors.
  `colors_ref` Colors reference.
- **invert** `void invert(const span<const color>& source_colors_ref, span<color> destination_colors_ref)` (L91): Inverts all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `destination_colors_ref` Destination colors reference.
- **grayscale** `void grayscale(fixed intensity, span<color> colors_ref)` (L99): Applies a grayscale effect to all referenced colors.
  `intensity` Grayscale effect intensity in the range [0..1].; `colors_ref` Colors reference. It must be 4-byte aligned and have an even number of colors.
- **grayscale** `void grayscale(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref)` (L110): Applies a grayscale effect to all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference. It must be 4-byte aligned and have an even number of colors.; `intensity` Grayscale effect intensity in the range [0..1].; `destination_colors_ref` Destination colors reference. It must be 4-byte aligned and have an even number of colors.
- **hue_shift** `void hue_shift(fixed intensity, span<color> colors_ref)` (L117): Applies a hue shift effect to all referenced colors.
  `intensity` Hue shift effect intensity in the range [0..1].; `colors_ref` Colors reference.
- **hue_shift** `void hue_shift(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref)` (L126): Applies a hue shift effect to all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `intensity` Hue shift effect intensity in the range [0..1].; `destination_colors_ref` Destination colors reference.
- **blend** `void blend(const span<const color>& first_source_colors_ref, const span<const color>& second_source_colors_ref, fixed weight, span<color> destination_colors_ref)` (L139): Blends two color arrays referenced by first_source_colors_ref and second_source_colors_ref, storing the result in destination_colors_ref.
  `first_source_colors_ref` First source colors reference. It must be 4-byte aligned and have an even number of colors.; `second_source_colors_ref` Second source colors reference. It must be 4-byte aligned and have an even number of colors.; `weight` Blend weight in the range [0..1].; `destination_colors_ref` Destination colors reference. It must be 4-byte aligned and have an even number of colors.
- **fade** `void fade(color fade_color, fixed intensity, span<color> colors_ref)` (L149): Applies a fade effect to all referenced colors.
  `fade_color` Fade effect color.; `intensity` Fade effect intensity in the range [0..1].; `colors_ref` Colors reference. It must be 4-byte aligned and have an even number of colors.
- **fade** `void fade(const span<const color>& source_colors_ref, color fade_color, fixed intensity, span<color> destination_colors_ref)` (L161): Applies a fade effect to all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference. It must be 4-byte aligned and have an even number of colors.; `fade_color` Fade effect color.; `intensity` Fade effect intensity in the range [0..1].; `destination_colors_ref` Destination colors reference. It must be 4-byte aligned and have an even number of colors.
- **rotate** `void rotate(int rotate_count, span<color> colors_ref)` (L170): Rotates all referenced colors to the right.
  `rotate_count` Number of colors to rotate to the right in the range [1 - colors_count .. colors_count - 1].; `colors_ref` Colors reference.
- **rotate** `void rotate(const span<const color>& source_colors_ref, int rotate_count, span<color> destination_colors_ref)` (L183): Rotates all colors referenced by source_colors_ref to the right, storing the result in destination_colors_ref.
  `source_colors_ref` Source colors reference.; `rotate_count` Number of colors to rotate to the right in the range [1 - colors_count .. colors_count - 1].; `destination_colors_ref` Destination colors reference.
  If the source and destination tiles, colors or map cells overlap, the behavior is undefined.

## bn_colors.h

### `bn::colors` (namespace, bn_colors.h:23)

Some constant colors from TONC.

