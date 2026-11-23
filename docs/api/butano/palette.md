# Butano 21.10.1 — Color palettes (`palette`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_bg_palette_ptr.h

### `bn::bg_palette_ptr` (class, bn_bg_palette_ptr.h:38)

std::shared_ptr like smart pointer that retains shared ownership of a background color palette.

`class bg_palette_ptr`

Several bg_palette_ptr objects may own the same background color palette.

The background color palette is released when the last remaining bg_palette_ptr owning it is destroyed.

- **find** `static optional<bg_palette_ptr> find(const bg_palette_item& palette_item)` (L48): Searches for a bg_palette_ptr which contains the given colors.
  `palette_item` bg_palette_item which references the colors to search.; devuelve: bg_palette_ptr which contains the referenced colors by palette_item if it has been found; bn::nullopt otherwise.
- **create** `static bg_palette_ptr create(const bg_palette_item& palette_item)` (L56): Searches for a bg_palette_ptr which contains the given colors. If it is not found, it creates a bg_palette_ptr which contains them.
  `palette_item` bg_palette_item which references the colors of the palette to search or create.; devuelve: The requested bg_palette_ptr.
- **create_new** `static bg_palette_ptr create_new(const bg_palette_item& palette_item)` (L63): Creates a bg_palette_ptr which contains the given colors.
  `palette_item` bg_palette_item which references the colors of the new palette.; devuelve: The requested bg_palette_ptr.
- **create_optional** `static optional<bg_palette_ptr> create_optional(const bg_palette_item& palette_item)` (L71): Searches for a bg_palette_ptr which contains the given colors. If it is not found, it creates a bg_palette_ptr which contains them.
  `palette_item` bg_palette_item which references the colors of the palette to search or create.; devuelve: The requested bg_palette_ptr if it could be allocated; bn::nullopt otherwise.
- **create_new_optional** `static optional<bg_palette_ptr> create_new_optional(const bg_palette_item& palette_item)` (L78): Creates a bg_palette_ptr which contains the given colors.
  `palette_item` bg_palette_item which references the colors of the new palette.; devuelve: The requested bg_palette_ptr if it could be allocated; bn::nullopt otherwise.
- **bg_palette_ptr** `bg_palette_ptr(const bg_palette_ptr& other)` (L84): Copy constructor.
  `other` bg_palette_ptr to copy.
- **operator=** `bg_palette_ptr& operator=(const bg_palette_ptr& other)` (L91): Copy assignment operator.
  `other` bg_palette_ptr to copy.; devuelve: Reference to this.
- **bg_palette_ptr** `bg_palette_ptr(bg_palette_ptr&& other) noexcept : _id(other._id)` (L97): Move constructor.
  `other` bg_palette_ptr to move.
- **operator=** `bg_palette_ptr& operator=(bg_palette_ptr&& other) noexcept` (L108): Move assignment operator.
  `other` bg_palette_ptr to move.; devuelve: Reference to this.
- **~bg_palette_ptr** `~bg_palette_ptr()` (L117): Releases the referenced background color palette if no more bg_palette_ptr objects reference to it.
- **id** `int id() const` (L122): Returns the internal id.
- **colors_count** `int colors_count() const` (L130): Returns the number of colors contained in this palette.
- **colors** `span<const color> colors() const` (L135): Returns the colors contained in this palette.
- **set_colors** `void set_colors(const span<const color>& colors)` (L141): Sets the colors contained in this palette.
  `colors` Colors to copy to this palette.
- **set_colors** `void set_colors(const bg_palette_item& palette_item)` (L147): Sets the colors contained in this palette.
  `palette_item` bg_palette_item which references the colors to copy to this palette.
- **set_color** `void set_color(int color_index, color color)` (L154): Sets a single color in this palette.
  `color_index` Index of the color to set in the palette.; `color` New color to set.
- **bpp** `bpp_mode bpp() const` (L159): Returns the bits per pixel of this palette.
- **inverted** `bool inverted() const` (L164): Indicates if the colors of this palette are inverted or not.
- **set_inverted** `void set_inverted(bool inverted)` (L169): Sets if the colors of this palette must be inverted or not.
- **grayscale_intensity** `fixed grayscale_intensity() const` (L174): Returns the intensity of the grayscale effect applied to the colors of this palette.
- **set_grayscale_intensity** `void set_grayscale_intensity(fixed intensity)` (L180): Sets the intensity of the grayscale effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **hue_shift_intensity** `fixed hue_shift_intensity() const` (L185): Returns the intensity of the hue shift effect applied to the colors of this palette.
- **set_hue_shift_intensity** `void set_hue_shift_intensity(fixed intensity)` (L191): Sets the intensity of the hue shift effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **fade_color** `color fade_color() const` (L196): Returns the color of the fade effect applied to the colors of this palette.
- **fade_intensity** `fixed fade_intensity() const` (L201): Returns the intensity of the fade effect applied to the colors of this palette.
- **set_fade_color** `void set_fade_color(color color)` (L206): Sets the color of the fade effect applied to the colors of this palette.
- **set_fade_intensity** `void set_fade_intensity(fixed intensity)` (L212): Sets the intensity of the fade effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **set_fade** `void set_fade(color color, fixed intensity)` (L219): Sets the color and the intensity of the fade effect applied to the colors of this palette.
  `color` New color.; `intensity` New intensity in the range [0..1].
- **rotate_count** `int rotate_count() const` (L224): Returns the number of colors to rotate to the right in this palette.
- **set_rotate_count** `void set_rotate_count(int count)` (L231): Sets the number of colors to rotate to the right in this palette.
  `count` Number of colors to rotate to the right in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **rotate_range_start** `int rotate_range_start() const` (L236): Returns the starting color index of the rotation range in this palette.
- **rotate_range_size** `int rotate_range_size() const` (L241): Returns the number of colors of the rotation range in this palette.
- **set_rotate_range** `void set_rotate_range(int start, int size)` (L248): Sets the rotation range of this palette.
  `start` Starting color index in the range [0 .. colors_count() - 2].; `size` Number of colors in the range [2 .. colors_count()].
- **swap** `void swap(bg_palette_ptr& other)` (L254): Exchanges the contents of this bg_palette_ptr with those of the other one.
  `other` bg_palette_ptr to exchange the contents with.
- **swap** `friend void swap(bg_palette_ptr& a, bg_palette_ptr& b)` (L264): Exchanges the contents of a bg_palette_ptr with those of another one.
  `a` First bg_palette_ptr to exchange the contents with.; `b` Second bg_palette_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const bg_palette_ptr& a, const bg_palette_ptr& b) = default` (L272): Default equal operator.

## bn_bg_palettes.h

### `bn::bg_palettes` (namespace, bn_bg_palettes.h:27)

Background color palettes related functions.

- **used_colors_count** `int used_colors_count()` (L32): Returns the number of used background colors.
- **available_colors_count** `int available_colors_count()` (L37): Returns the number of available background colors.
- **transparent_color** `const optional<color>& transparent_color()` (L42): Returns the overridden transparent color of the backgrounds if any, bn::nullopt otherwise.
- **set_transparent_color** `void set_transparent_color(color transparent_color)` (L47): Sets the overridden transparent color of the backgrounds.
- **set_transparent_color** `void set_transparent_color(const optional<color>& transparent_color)` (L52): Sets or removes the overridden transparent color of the backgrounds.
- **remove_transparent_color** `void remove_transparent_color()` (L57): Removes the overridden transparent color of the backgrounds, if any.
- **brightness** `fixed brightness()` (L62): Returns the brightness of all background color palettes.
- **set_brightness** `void set_brightness(fixed brightness)` (L68): Sets the brightness of all background color palettes.
  `brightness` New brightness in the range [0..1].
- **contrast** `fixed contrast()` (L73): Returns the contrast of all background color palettes.
- **set_contrast** `void set_contrast(fixed contrast)` (L79): Sets the contrast of all background color palettes.
  `contrast` New contrast in the range [0..1].
- **intensity** `fixed intensity()` (L84): Returns the intensity of all background color palettes.
- **set_intensity** `void set_intensity(fixed intensity)` (L90): Sets the intensity of all background color palettes.
  `intensity` New intensity in the range [0..1].
- **inverted** `bool inverted()` (L95): Indicates if the colors of all background color palettes are inverted or not.
- **set_inverted** `void set_inverted(bool inverted)` (L100): Sets if the colors of all background color palettes must be inverted or not.
- **grayscale_intensity** `fixed grayscale_intensity()` (L105): Returns the intensity of the grayscale effect applied to all background color palettes.
- **set_grayscale_intensity** `void set_grayscale_intensity(fixed intensity)` (L111): Sets the intensity of the grayscale effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
- **hue_shift_intensity** `fixed hue_shift_intensity()` (L116): Returns the intensity of the hue shift effect applied to all background color palettes.
- **set_hue_shift_intensity** `void set_hue_shift_intensity(fixed intensity)` (L122): Sets the intensity of the hue shift effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
- **fade_color** `color fade_color()` (L127): Returns the color of the fade effect applied to all background color palettes.
- **fade_intensity** `fixed fade_intensity()` (L132): Returns the intensity of the fade effect applied to all background color palettes.
- **set_fade_color** `void set_fade_color(color color)` (L137): Sets the color of the fade effect applied to all background color palettes.
- **set_fade_intensity** `void set_fade_intensity(fixed intensity)` (L143): Sets the intensity of the fade effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
- **set_fade** `void set_fade(color color, fixed intensity)` (L150): Sets the color and the intensity of the fade effect applied to all background color palettes.
  `color` New color.; `intensity` New intensity in the range [0..1].
- **custom_effect** `palette_effect_type custom_effect()` (L155): Returns the custom effect applied to all background color palettes.
- **set_custom_effect** `void set_custom_effect(palette_effect_type effect)` (L160): Sets the custom effect applied to all background color palettes.
- **reload_custom_effect** `void reload_custom_effect()` (L165): Reapplies the custom effect to all background color palettes.
- **log_status** `void log_status()` (L170): Logs the current status of the background color palettes manager.

## bn_sprite_palette_ptr.h

### `bn::sprite_palette_ptr` (class, bn_sprite_palette_ptr.h:38)

std::shared_ptr like smart pointer that retains shared ownership of a sprite color palette.

`class sprite_palette_ptr`

Several sprite_palette_ptr objects may own the same sprite color palette.

The sprite color palette is released when the last remaining sprite_palette_ptr owning it is destroyed.

- **find** `static optional<sprite_palette_ptr> find(const sprite_palette_item& palette_item)` (L48): Searches for a sprite_palette_ptr which contains the given colors.
  `palette_item` sprite_palette_item which references the colors to search.; devuelve: sprite_palette_ptr which contains the referenced colors by palette_item if it has been found; bn::nullopt otherwise.
- **create** `static sprite_palette_ptr create(const sprite_palette_item& palette_item)` (L56): Searches for a sprite_palette_ptr which contains the given colors. If it is not found, it creates a sprite_palette_ptr which contains them.
  `palette_item` sprite_palette_item which references the colors of the palette to search or create.; devuelve: The requested sprite_palette_ptr.
- **create_new** `static sprite_palette_ptr create_new(const sprite_palette_item& palette_item)` (L63): Creates a sprite_palette_ptr which contains the given colors.
  `palette_item` sprite_palette_item which references the colors of the new palette.; devuelve: The requested sprite_palette_ptr.
- **create_optional** `static optional<sprite_palette_ptr> create_optional(const sprite_palette_item& palette_item)` (L71): Searches for a sprite_palette_ptr which contains the given colors. If it is not found, it creates a sprite_palette_ptr which contains them.
  `palette_item` sprite_palette_item which references the colors of the palette to search or create.; devuelve: The requested sprite_palette_ptr if it could be allocated; bn::nullopt otherwise.
- **create_new_optional** `static optional<sprite_palette_ptr> create_new_optional(const sprite_palette_item& palette_item)` (L78): Creates a sprite_palette_ptr which contains the given colors.
  `palette_item` sprite_palette_item which references the colors of the new palette.; devuelve: The requested sprite_palette_ptr if it could be allocated; bn::nullopt otherwise.
- **sprite_palette_ptr** `sprite_palette_ptr(const sprite_palette_ptr& other)` (L84): Copy constructor.
  `other` sprite_palette_ptr to copy.
- **operator=** `sprite_palette_ptr& operator=(const sprite_palette_ptr& other)` (L91): Copy assignment operator.
  `other` sprite_palette_ptr to copy.; devuelve: Reference to this.
- **sprite_palette_ptr** `sprite_palette_ptr(sprite_palette_ptr&& other) noexcept : _id(other._id)` (L97): Move constructor.
  `other` sprite_palette_ptr to move.
- **operator=** `sprite_palette_ptr& operator=(sprite_palette_ptr&& other) noexcept` (L108): Move assignment operator.
  `other` sprite_palette_ptr to move.; devuelve: Reference to this.
- **~sprite_palette_ptr** `~sprite_palette_ptr()` (L117): Releases the referenced sprite color palette if no more sprite_palette_ptr objects reference to it.
- **id** `int id() const` (L122): Returns the internal id.
- **colors_count** `int colors_count() const` (L130): Returns the number of colors contained in this palette.
- **colors** `span<const color> colors() const` (L135): Returns the colors contained in this palette.
- **set_colors** `void set_colors(const span<const color>& colors)` (L141): Sets the colors contained in this palette.
  `colors` Colors to copy to this palette.
- **set_colors** `void set_colors(const sprite_palette_item& palette_item)` (L147): Sets the colors contained in this palette.
  `palette_item` sprite_palette_item which references the colors to copy to this palette.
- **set_color** `void set_color(int color_index, color color)` (L154): Sets a single color in this palette.
  `color_index` Index of the color to set in the palette.; `color` New color to set.
- **bpp** `bpp_mode bpp() const` (L159): Returns the bits per pixel of this palette.
- **inverted** `bool inverted() const` (L164): Indicates if the colors of this palette are inverted or not.
- **set_inverted** `void set_inverted(bool inverted)` (L169): Sets if the colors of this palette must be inverted or not.
- **grayscale_intensity** `fixed grayscale_intensity() const` (L174): Returns the intensity of the grayscale effect applied to the colors of this palette.
- **set_grayscale_intensity** `void set_grayscale_intensity(fixed intensity)` (L180): Sets the intensity of the grayscale effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **hue_shift_intensity** `fixed hue_shift_intensity() const` (L185): Returns the intensity of the hue shift effect applied to the colors of this palette.
- **set_hue_shift_intensity** `void set_hue_shift_intensity(fixed intensity)` (L191): Sets the intensity of the hue shift effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **fade_color** `color fade_color() const` (L196): Returns the color of the fade effect applied to the colors of this palette.
- **fade_intensity** `fixed fade_intensity() const` (L201): Returns the intensity of the fade effect applied to the colors of this palette.
- **set_fade_color** `void set_fade_color(color color)` (L206): Sets the color of the fade effect applied to the colors of this palette.
- **set_fade_intensity** `void set_fade_intensity(fixed intensity)` (L212): Sets the intensity of the fade effect applied to the colors of this palette.
  `intensity` New intensity in the range [0..1].
- **set_fade** `void set_fade(color color, fixed intensity)` (L219): Sets the color and the intensity of the fade effect applied to the colors of this palette.
  `color` New color.; `intensity` New intensity in the range [0..1].
- **rotate_count** `int rotate_count() const` (L224): Returns the number of colors to rotate to the right in this palette.
- **set_rotate_count** `void set_rotate_count(int count)` (L231): Sets the number of colors to rotate to the right in this palette.
  `count` Number of colors to rotate to the right in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **rotate_range_start** `int rotate_range_start() const` (L236): Returns the starting color index of the rotation range in this palette.
- **rotate_range_size** `int rotate_range_size() const` (L241): Returns the number of colors of the rotation range in this palette.
- **set_rotate_range** `void set_rotate_range(int start, int size)` (L248): Sets the rotation range of this palette.
  `start` Starting color index in the range [0 .. colors_count() - 2].; `size` Number of colors in the range [2 .. colors_count()].
- **swap** `void swap(sprite_palette_ptr& other)` (L254): Exchanges the contents of this sprite_palette_ptr with those of the other one.
  `other` sprite_palette_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_palette_ptr& a, sprite_palette_ptr& b)` (L264): Exchanges the contents of a sprite_palette_ptr with those of another one.
  `a` First sprite_palette_ptr to exchange the contents with.; `b` Second sprite_palette_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sprite_palette_ptr& a, const sprite_palette_ptr& b) = default` (L272): Default equal operator.

## bn_sprite_palettes.h

### `bn::sprite_palettes` (namespace, bn_sprite_palettes.h:26)

Sprite color palettes related functions.

- **used_colors_count** `int used_colors_count()` (L31): Returns the number of used sprite colors.
- **available_colors_count** `int available_colors_count()` (L36): Returns the number of available sprite colors.
- **brightness** `fixed brightness()` (L41): Returns the brightness of all sprite color palettes.
- **set_brightness** `void set_brightness(fixed brightness)` (L47): Sets the brightness of all sprite color palettes.
  `brightness` New brightness in the range [0..1].
- **contrast** `fixed contrast()` (L52): Returns the contrast of all sprite color palettes.
- **set_contrast** `void set_contrast(fixed contrast)` (L58): Sets the contrast of all sprite color palettes.
  `contrast` New contrast in the range [0..1].
- **intensity** `fixed intensity()` (L63): Returns the intensity of all sprite color palettes.
- **set_intensity** `void set_intensity(fixed intensity)` (L69): Sets the intensity of all sprite color palettes.
  `intensity` New intensity in the range [0..1].
- **inverted** `bool inverted()` (L74): Indicates if the colors of all sprite color palettes are inverted or not.
- **set_inverted** `void set_inverted(bool inverted)` (L79): Sets if the colors of all sprite color palettes must be inverted or not.
- **grayscale_intensity** `fixed grayscale_intensity()` (L84): Returns the intensity of the grayscale effect applied to all sprite color palettes.
- **set_grayscale_intensity** `void set_grayscale_intensity(fixed intensity)` (L90): Sets the intensity of the grayscale effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
- **hue_shift_intensity** `fixed hue_shift_intensity()` (L95): Returns the intensity of the hue shift effect applied to all sprite color palettes.
- **set_hue_shift_intensity** `void set_hue_shift_intensity(fixed intensity)` (L101): Sets the intensity of the hue shift effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
- **fade_color** `color fade_color()` (L106): Returns the color of the fade effect applied to all sprite color palettes.
- **fade_intensity** `fixed fade_intensity()` (L111): Returns the intensity of the fade effect applied to all sprite color palettes.
- **set_fade_color** `void set_fade_color(color color)` (L116): Sets the color of the fade effect applied to all sprite color palettes.
- **set_fade_intensity** `void set_fade_intensity(fixed intensity)` (L122): Sets the intensity of the fade effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
- **set_fade** `void set_fade(color color, fixed intensity)` (L129): Sets the color and the intensity of the fade effect applied to all sprite color palettes.
  `color` New color.; `intensity` New intensity in the range [0..1].
- **custom_effect** `palette_effect_type custom_effect()` (L134): Returns the custom effect applied to all sprite color palettes.
- **set_custom_effect** `void set_custom_effect(palette_effect_type effect)` (L139): Sets the custom effect applied to all sprite color palettes.
- **reload_custom_effect** `void reload_custom_effect()` (L144): Reapplies the custom effect to all sprite color palettes.
- **log_status** `void log_status()` (L149): Logs the current status of the sprite color palettes manager.
