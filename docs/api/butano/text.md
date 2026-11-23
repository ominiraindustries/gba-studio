# Butano 21.10.1 — Text (`text`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_sprite_font.h

### `bn::sprite_font` (class, bn_sprite_font.h:36)

Contains the required information to generate text sprites.

`class sprite_font`

Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.

Also, UTF-8 characters are supported.

UTF-8 characters map and character widths are not copied but referenced,
so they should outlive the sprite_font to avoid dangling references.

- **sprite_font** `constexpr explicit sprite_font(const sprite_item& item) : sprite_font(item, utf8_characters_map_ref(), span<const int8_t>(), 0)` (L46): Constructor.
  `item` sprite_item used to generate text sprites.
- **sprite_font** `constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref) : sprite_font(item, utf8_characters_ref, span<const int8_t>(), 0)` (L62): Constructor.
  `item` sprite_item used to generate text sprites.; `utf8_characters_ref` Reference to the map of the supported UTF-8 characters and their position in the tile sets.
  UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
  
  UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
  to avoid dangling references.
- **sprite_font** `constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref, const span<const int8_t>& character_widths_ref) : sprite_font(item, utf8_characters_ref, character_widths_ref, 0)` (L83): Constructor.
  `item` sprite_item used to generate text sprites.; `utf8_characters_ref` Reference to the map of the supported UTF-8 characters and their position in the tile sets.; `character_widths_ref` Reference to the width in pixels of each supported character.
  UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
  
  UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
  to avoid dangling references.
  
  Character widths are not copied but referenced, so they should outlive the sprite_font
  to avoid dangling references.
- **sprite_font** `constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref, const span<const int8_t>& character_widths_ref, int space_between_characters)` (L107): Constructor.
  `item` sprite_item used to generate text sprites.; `utf8_characters_ref` Reference to the map of the supported UTF-8 characters and their position in the tile sets.; `character_widths_ref` Reference to the width in pixels of each supported character.; `space_between_characters` Space between two consecutive characters in pixels (it can be negative).
  UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
  
  UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
  to avoid dangling references.
  
  Character widths are not copied but referenced, so they should outlive the sprite_font
  to avoid dangling references.
- **item** `constexpr const sprite_item& item() const` (L130): Returns the sprite_item used to generate text sprites.
- **utf8_characters_ref** `constexpr const utf8_characters_map_ref& utf8_characters_ref() const` (L144): Returns the reference to the map of the supported UTF-8 characters and their position in the tile sets.
  UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
  
  UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
  to avoid dangling references.
- **character_widths_ref** `constexpr const span<const int8_t>& character_widths_ref() const` (L155): Returns the reference to the width in pixels of each supported character.
  Character widths are not copied but referenced, so they should outlive the sprite_font
  to avoid dangling references.
- **space_between_characters** `int space_between_characters() const` (L163): Returns the space between two consecutive characters in pixels (it can be negative).

## bn_sprite_text_generator.h

### `bn::sprite_text_generator` (class, bn_sprite_text_generator.h:39)

Generates sprites containing text from a given sprite_font.

`class sprite_text_generator`

Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.

Text can be printed in one sprite per character or multiple characters per sprite.

Also, UTF-8 characters are supported.

### `bn::sprite_text_generator::alignment_type` (enum, bn_sprite_text_generator.h:46)

Available horizontal alignment types.

`enum class alignment_type : uint8_t { LEFT /* Aligns with the left text edge. */, CENTER /* Aligns with the middle of the text. */, RIGHT /* Aligns with the right text edge. */ }`

- **sprite_text_generator** `explicit sprite_text_generator(const sprite_font& font)` (L57): Constructor.
  `font` Sprite font for drawing text.
- **sprite_text_generator** `sprite_text_generator(const sprite_font& font, const sprite_palette_item& palette_item)` (L65): Constructor.
  `font` Sprite font for drawing text.; `palette_item` 16 colors (4 bits per pixel) sprite_palette_item that generates the color palette used by the text sprites.
- **font** `const sprite_font& font() const` (L70): Returns the sprite font for drawing text.
- **palette_item** `const sprite_palette_item& palette_item() const` (L78): Returns the sprite_palette_item that generates the color palette used by the text sprites.
- **set_palette_item** `void set_palette_item(const sprite_palette_item& palette_item)` (L87): Sets the 16 colors (4 bits per pixel) sprite_palette_item that generates the color palette used by the text sprites.
- **alignment** `alignment_type alignment() const` (L92): Returns the horizontal alignment of the output text sprites.
- **set_alignment** `void set_alignment(alignment_type alignment)` (L100): Sets the horizontal alignment of the output text sprites.
- **set_left_alignment** `void set_left_alignment()` (L108): Sets the horizontal alignment of the output text sprites to the left.
- **set_center_alignment** `void set_center_alignment()` (L116): Sets the horizontal alignment of the output text sprites to the center.
- **set_right_alignment** `void set_right_alignment()` (L124): Sets the horizontal alignment of the output text sprites to the right.
- **bg_priority** `int bg_priority() const` (L135): Returns the priority of the output sprites priority relative to backgrounds.
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **set_bg_priority** `void set_bg_priority(int bg_priority)` (L148): Sets the priority of the output sprites relative to backgrounds.
  `bg_priority` Priority relative to backgrounds in the range [0..3].
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **z_order** `int z_order() const` (L155): Returns the priority of the output sprites relative to other sprites.
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
- **set_z_order** `void set_z_order(int z_order)` (L167): Sets the priority of the output sprites relative to other sprites.
  `z_order` Priority relative to other sprites in the range [-32767..32767].
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
- **mosaic_enabled** `bool mosaic_enabled() const` (L172): Indicates if the mosaic effect must be applied to the output sprites or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L180): Sets if the mosaic effect must be applied to the output sprites or not.
- **blending_enabled** `bool blending_enabled() const` (L188): Indicates if blending must be applied to the output sprites or not.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L196): Sets if blending must be applied to the output sprites or not.
- **visible** `bool visible() const` (L204): Indicates if the output sprites must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L212): Sets if the output sprites must be committed to the GBA or not.
- **camera** `const optional<camera_ptr>& camera() const` (L220): Returns the camera_ptr to attach to the output sprites (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L228): Sets the camera_ptr to attach to the output sprites.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L236): Sets the camera_ptr to attach to the output sprites.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L244): Sets or removes the camera_ptr to attach to the output sprites.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L252): Sets or removes the camera_ptr to attach to the output sprites.
- **remove_camera** `void remove_camera()` (L260): Removes the camera_ptr to attach to the output sprites.
- **release_camera** `optional<camera_ptr> release_camera()` (L268): Releases and returns the camera_ptr to attach to the output sprites (if any).
- **one_sprite_per_character** `bool one_sprite_per_character() const` (L274): Indicates if this sprite_text_generator must generate one sprite per character or print multiple characters in each output sprite, generating as few sprites as possible.
- **set_one_sprite_per_character** `void set_one_sprite_per_character(bool one_sprite_per_character)` (L283): Sets if this sprite_text_generator must generate one sprite per character or print multiple characters in each output sprite, generating as few sprites as possible.
- **width** `int width(const string_view& text) const` (L291): Returns the width in pixels of the given text.
- **generate** `template<int MaxSprites> vector<sprite_ptr, MaxSprites> generate(const string_view& text) const` (L299): Generates text sprites for the given single line of text.
  `MaxSprites` Maximum size of the returned sprite_ptr vector.; `text` Single line of text to print.; devuelve: sprite_ptr vector containing the generated text sprites.
- **generate** `template<int MaxSprites> vector<sprite_ptr, MaxSprites> generate(fixed x, fixed y, const string_view& text) const` (L315): Generates text sprites for the given single line of text.
  `MaxSprites` Maximum size of the returned sprite_ptr vector.; `x` Horizontal position of the first generated sprite, considering the current alignment.; `y` Vertical position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; devuelve: sprite_ptr vector containing the generated text sprites.
- **generate** `template<int MaxSprites> vector<sprite_ptr, MaxSprites> generate(const fixed_point& position, const string_view& text) const` (L330): Generates text sprites for the given single line of text.
  `MaxSprites` Maximum size of the returned sprite_ptr vector.; `position` Position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; devuelve: sprite_ptr vector containing the generated text sprites.
- **generate** `void generate(const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L345): Generates text sprites for the given single line of text.
  `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate** `void generate(fixed x, fixed y, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L356): Generates text sprites for the given single line of text.
  `x` Horizontal position of the first generated sprite, considering the current alignment.; `y` Vertical position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate** `void generate(const fixed_point& position, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L366): Generates text sprites for the given single line of text.
  `position` Position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate_top_left** `template<int MaxSprites> vector<sprite_ptr, MaxSprites> generate_top_left( fixed top_left_x, fixed top_left_y, const string_view& text) const` (L376): Generates text sprites for the given single line of text.
  `MaxSprites` Maximum size of the returned sprite_ptr vector.; `top_left_x` Horizontal top-left position of the first generated sprite, considering the current alignment.; `top_left_y` Vertical top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; devuelve: sprite_ptr vector containing the generated text sprites.
- **generate_top_left** `template<int MaxSprites> vector<sprite_ptr, MaxSprites> generate_top_left( const fixed_point& top_left_position, const string_view& text) const` (L392): Generates text sprites for the given single line of text.
  `MaxSprites` Maximum size of the returned sprite_ptr vector.; `top_left_position` Top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; devuelve: sprite_ptr vector containing the generated text sprites.
- **generate_top_left** `void generate_top_left(fixed top_left_x, fixed top_left_y, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L410): Generates text sprites for the given single line of text.
  `top_left_x` Horizontal top-left position of the first generated sprite, considering the current alignment.; `top_left_y` Vertical top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate_top_left** `void generate_top_left(const fixed_point& top_left_position, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L424): Generates text sprites for the given single line of text.
  `top_left_position` Top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate_optional** `bool generate_optional(const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L436): Generates text sprites for the given single line of text.
  `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.; devuelve: `true` if the text generation finished successfully, otherwise `false`.
  Keep in mind that this vector is not cleared before generating text.
- **generate_optional** `bool generate_optional(fixed x, fixed y, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L449): Generates text sprites for the given single line of text.
  `x` Horizontal position of the first generated sprite, considering the current alignment.; `y` Vertical position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.; devuelve: `true` if the text generation finished successfully, otherwise `false`.
  Keep in mind that this vector is not cleared before generating text.
- **generate_optional** `bool generate_optional(const fixed_point& position, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L462): Generates text sprites for the given single line of text.
  `position` Position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.; devuelve: `true` if the text generation finished successfully, otherwise `false`.
  Keep in mind that this vector is not cleared before generating text.
- **generate_top_left_optional** `bool generate_top_left_optional(fixed top_left_x, fixed top_left_y, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L474): Generates text sprites for the given single line of text.
  `top_left_x` Horizontal top-left position of the first generated sprite, considering the current alignment.; `top_left_y` Vertical top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.
  Keep in mind that this vector is not cleared before generating text.
- **generate_top_left_optional** `bool generate_top_left_optional(const fixed_point& top_left_position, const string_view& text, ivector<sprite_ptr>& output_sprites) const` (L490): Generates text sprites for the given single line of text.
  `top_left_position` Top-left position of the first generated sprite, considering the current alignment.; `text` Single line of text to print.; `output_sprites` Generated text sprites are stored in this vector.; devuelve: `true` if the text generation finished successfully, otherwise `false`.
  Keep in mind that this vector is not cleared before generating text.

## bn_utf8_character.h

### `bn::utf8_character` (class, bn_utf8_character.h:49)

Decodes a single UTF-8 character from a string.

`class utf8_character`

See utf8_decode_char() function in https://github.com/devkitPro/libtonc/blob/master/src/tte/tte_main.c.

- **utf8_character** `constexpr utf8_character(const char* text_ptr)` (L57): Constructor.
  `text_ptr` Non null pointer to the string to decode.
- **utf8_character** `constexpr utf8_character(const char& text_ref)` (L68): Constructor.
  `text_ref` Reference to the string to decode.
- **data** `constexpr int data() const` (L123): Returns the decoded UTF-8 character.
- **size** `constexpr int size() const` (L131): Returns the size in bytes of the decoded UTF-8 character.

## bn_utf8_characters_map.h

### `bn::utf8_characters_map` (class, bn_utf8_characters_map.h:29)

Maps a list of UTF-8 characters to their position in the list.

`template<const span<const utf8_character>& Utf8Characters> class utf8_characters_map`

- **utf8_characters_map** `constexpr utf8_characters_map()` (L37): Default constructor.
- **reference** `constexpr utf8_characters_map_ref reference() const` (L70): Returns a reference to this map without information about the UTF-8 characters input list.
  This map is not copied but referenced, so it should outlive the utf8_characters_map_ref
  to avoid dangling references.

## bn_utf8_characters_map_ref.h

### `bn::utf8_characters_map_ref` (class, bn_utf8_characters_map_ref.h:28)

Maps a list of UTF-8 characters to their position in the list.

`class utf8_characters_map_ref`

### `bn::utf8_characters_map_ref::item_type` (struct, bn_utf8_characters_map_ref.h:35)

Map item info.

`struct item_type`

- **utf8_characters_map_ref** `constexpr utf8_characters_map_ref() = default` (L45): Default constructor.
- **utf8_characters_map_ref** `constexpr utf8_characters_map_ref(const item_type& items_ref, int items_count, int characters_count)` (L57): Constructor.
  `items_ref` Reference to a list of map items.; `items_count` Referenced map items count.; `characters_count` Mapped UTF-8 characters count.
  Map items are not copied but referenced, so they should outlive the utf8_characters_map_ref
  to avoid dangling references.
- **size** `constexpr int size() const` (L70): Returns the mapped UTF-8 characters count.
- **empty** `constexpr bool empty() const` (L78): Indicates if it doesn't contain any mapped UTF-8 character.
- **index** `constexpr int index(const utf8_character& character) const` (L88): Searches for the position of a given UTF-8 character in the input list.
  `character` UTF-8 character to search for.; devuelve: Position of the UTF-8 character in the input list.
