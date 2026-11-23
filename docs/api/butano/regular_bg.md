# Butano 21.10.1 — Regular backgrounds (`regular_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bg](bg.md)

## bn_regular_bg_attributes.h

### `bn::regular_bg_attributes` (class, bn_regular_bg_attributes.h:28)

Manages the attributes to commit to the GBA register of a regular background.

`class regular_bg_attributes`

- **regular_bg_attributes** `regular_bg_attributes(const regular_bg_map_ptr& map, int priority, bool mosaic_enabled)` (L42): Constructor.
  `map` regular_bg_map_ptr of a regular background to copy.; `priority` Priority of a regular background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to a regular background or not.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **regular_bg_attributes** `regular_bg_attributes(const regular_bg_map_ptr& map, int priority, bool mosaic_enabled, bn::green_swap_mode green_swap_mode)` (L55): Constructor.
  `map` regular_bg_map_ptr of a regular background to copy.; `priority` Priority of a regular background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to a regular background or not.; `green_swap_mode` Indicates how a regular background must be displayed when green swap is enabled.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **regular_bg_attributes** `regular_bg_attributes(regular_bg_map_ptr&& map, int priority, bool mosaic_enabled)` (L68): Constructor.
  `map` regular_bg_map_ptr of a regular background to move.; `priority` Priority of a regular background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to a regular background or not.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **regular_bg_attributes** `regular_bg_attributes(regular_bg_map_ptr&& map, int priority, bool mosaic_enabled, bn::green_swap_mode green_swap_mode)` (L81): Constructor.
  `map` regular_bg_map_ptr of a regular background to move.; `priority` Priority of a regular background relative to sprites and other backgrounds, in the range [0..3].; `mosaic_enabled` Indicates if the mosaic effect is applied to a regular background or not.; `green_swap_mode` Indicates how a regular background must be displayed when green swap is enabled.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **map** `const regular_bg_map_ptr& map() const` (L87): Returns the regular_bg_map_ptr of a regular background.
- **set_map** `void set_map(const regular_bg_map_ptr& map)` (L96): Sets the regular_bg_map_ptr of a regular background.
  `map` regular_bg_map_ptr of a regular background to copy.
- **set_map** `void set_map(regular_bg_map_ptr&& map)` (L105): Sets the regular_bg_map_ptr of a regular background.
  `map` regular_bg_map_ptr of a regular background to move.
- **priority** `int priority() const` (L116): Returns the priority of a regular background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L129): Sets the priority of a regular background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L134): Indicates if the mosaic effect is applied to a regular background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L142): Sets if the mosaic effect must be applied to a regular background or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L150): Indicates how a regular background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L158): Sets how a regular background must be displayed when green swap is enabled.
- **operator==** `friend bool operator==(const regular_bg_attributes& a, const regular_bg_attributes& b) = default` (L166): Default equal operator.

## bn_regular_bg_builder.h

### `bn::regular_bg_builder` (class, bn_regular_bg_builder.h:31)

Creates regular_bg_ptr objects with custom attributes.

`class regular_bg_builder`

- **regular_bg_builder** `explicit regular_bg_builder(const regular_bg_item& item)` (L39): Constructor.
  `item` regular_bg_item containing the required information to generate regular backgrounds.
- **regular_bg_builder** `regular_bg_builder(const regular_bg_item& item, int map_index)` (L49): Constructor.
  `item` regular_bg_item containing the required information to generate regular backgrounds.; `map_index` Index of the map to reference in item.map_item() to generate regular backgrounds.
- **regular_bg_builder** `explicit regular_bg_builder(const regular_bg_map_ptr& map)` (L55): Constructor.
  `map` regular_bg_map_ptr to copy for generating regular backgrounds.
- **regular_bg_builder** `explicit regular_bg_builder(regular_bg_map_ptr&& map)` (L64): Constructor.
  `map` regular_bg_map_ptr to move for generating regular backgrounds.
- **item** `const optional<regular_bg_item>& item() const` (L73): Returns the regular_bg_item containing the required information to generate regular backgrounds if it has one; bn::nullopt otherwise.
- **map_index** `int map_index() const` (L81): Index of the map to reference in item.map_item() to generate regular backgrounds.
- **x** `fixed x() const` (L90): Returns the horizontal position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_x** `regular_bg_builder& set_x(fixed x)` (L101): Sets the horizontal position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the regular backgrounds to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L111): Returns the vertical position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_y** `regular_bg_builder& set_y(fixed y)` (L122): Sets the vertical position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `y` vertical position of the regular backgrounds to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L132): Returns the position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_position** `regular_bg_builder& set_position(fixed x, fixed y)` (L144): Sets the position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the regular backgrounds to generate.; `y` Vertical position of the regular backgrounds to generate.; devuelve: Reference to this.
- **set_position** `regular_bg_builder& set_position(const fixed_point& position)` (L156): Sets the position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `position` Position of the regular backgrounds to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L166): Returns the horizontal top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `regular_bg_builder& set_top_left_x(fixed top_left_x)` (L174): Sets the horizontal top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the regular backgrounds to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L180): Returns the vertical top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `regular_bg_builder& set_top_left_y(fixed top_left_y)` (L188): Sets the vertical top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the regular backgrounds to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L194): Returns the top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `regular_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L203): Sets the top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the regular backgrounds to generate.; `top_left_y` Vertical top-left position of the regular backgrounds to generate.; devuelve: Reference to this.
- **set_top_left_position** `regular_bg_builder& set_top_left_position(const fixed_point& top_left_position)` (L211): Sets the top-left position of the regular backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the regular backgrounds to generate.; devuelve: Reference to this.
- **priority** `int priority() const` (L219): Returns the priority of the regular backgrounds to generate relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `regular_bg_builder& set_priority(int priority)` (L233): Sets the priority of the regular backgrounds to generate relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].; devuelve: Reference to this.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **z_order** `int z_order() const` (L243): Returns the priority of the regular backgrounds to generate relative to other backgrounds, excluding sprites.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **set_z_order** `regular_bg_builder& set_z_order(int z_order)` (L259): Sets the priority of the regular backgrounds to generate relative to other backgrounds, excluding sprites.
  `z_order` Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].; devuelve: Reference to this.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **mosaic_enabled** `bool mosaic_enabled() const` (L264): Indicates if the mosaic effect must be applied to the regular backgrounds to generate or not.
- **set_mosaic_enabled** `regular_bg_builder& set_mosaic_enabled(bool mosaic_enabled)` (L274): Sets if the mosaic effect must be applied to the regular backgrounds to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L285): Indicates if blending must be applied to the regular backgrounds to generate or not.
  Blending is applied to regular backgrounds by making them part of the blending top layer.
- **set_blending_enabled** `regular_bg_builder& set_blending_enabled(bool blending_enabled)` (L297): Sets if blending must be applied to the regular backgrounds to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Blending is applied to regular backgrounds by making them part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L306): Indicates if the regular backgrounds to generate must be part of the blending top layer or not.
- **set_blending_top_enabled** `regular_bg_builder& set_blending_top_enabled(bool blending_top_enabled)` (L317): Sets if the regular backgrounds to generate must be part of the blending top layer or not.
  `blending_top_enabled` `true` if generated backgrounds must be part of the blending top layer; `false` otherwise.; devuelve: Reference to this.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L326): Indicates if the regular backgrounds to generate must be part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `regular_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)` (L337): Sets if the regular backgrounds to generate must be part of the blending bottom layer or not.
  `blending_bottom_enabled` `true` if generated backgrounds must be part of the blending bottom layer; `false` otherwise.; devuelve: Reference to this.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L346): Indicates how the regular backgrounds to generate must be displayed when green swap is enabled.
- **set_green_swap_mode** `regular_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L356): Sets how the regular backgrounds to generate must be displayed when green swap is enabled.
  `green_swap_mode` Green swap mode.; devuelve: Reference to this.
- **visible** `bool visible() const` (L365): Indicates if the regular backgrounds to generate must be committed to the GBA or not.
- **set_visible** `regular_bg_builder& set_visible(bool visible)` (L375): Sets if the regular backgrounds to generate must be committed to the GBA or not.
  `visible` `true` if the regular backgrounds must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L384): Returns the camera_ptr to attach to the regular backgrounds to generate (if any).
- **set_camera** `regular_bg_builder& set_camera(const camera_ptr& camera)` (L394): Sets the camera_ptr to attach to the regular backgrounds to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `regular_bg_builder& set_camera(camera_ptr&& camera)` (L405): Sets the camera_ptr to attach to the regular backgrounds to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `regular_bg_builder& set_camera(const optional<camera_ptr>& camera)` (L416): Sets or removes the camera_ptr to attach to the regular backgrounds to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `regular_bg_builder& set_camera(optional<camera_ptr>&& camera)` (L427): Sets or removes the camera_ptr to attach to the regular backgrounds to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `regular_bg_builder& remove_camera()` (L437): Removes the camera_ptr to attach to the regular backgrounds to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L446): Releases and returns the camera_ptr to attach to the regular backgrounds to generate (if any).
- **build** `regular_bg_ptr build() const` (L451): Generates and returns a regular_bg_ptr without releasing the acquired resources.
- **release_build** `regular_bg_ptr release_build()` (L458): Generates and returns a regular_bg_ptr releasing the acquired resources.
  regular_bg_ptr generation after calling this method may stop working.
- **build_optional** `optional<regular_bg_ptr> build_optional() const` (L464): Generates and returns a regular_bg_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<regular_bg_ptr> release_build_optional()` (L472): Generates and returns a regular_bg_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  regular_bg_ptr generation after calling this method may stop working.
- **map** `regular_bg_map_ptr map() const` (L477): Generates and returns a regular_bg_map_ptr without releasing the acquired resources.
- **map_optional** `optional<regular_bg_map_ptr> map_optional() const` (L483): Generates and returns a regular_bg_map_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_map** `regular_bg_map_ptr release_map()` (L490): Generates and returns a regular_bg_map_ptr releasing the acquired resources.
  regular_bg_ptr generation after calling this method may stop working.
- **release_map_optional** `optional<regular_bg_map_ptr> release_map_optional()` (L498): Generates and returns a regular_bg_map_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  regular_bg_ptr generation after calling this method may stop working.

## bn_regular_bg_ptr.h

### `bn::regular_bg_ptr` (class, bn_regular_bg_ptr.h:45)

std::shared_ptr like smart pointer that retains shared ownership of a regular background.

`class regular_bg_ptr`

Several regular_bg_ptr objects may own the same regular background.

The regular background is released when the last remaining regular_bg_ptr owning it is destroyed.

- **create** `static regular_bg_ptr create(const regular_bg_item& item)` (L54): Creates a regular_bg_ptr from the given regular_bg_item.
  `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(const regular_bg_item& item, int map_index)` (L62): Creates a regular_bg_ptr from the given regular_bg_item.
  `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(regular_bg_map_ptr map)` (L69): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(fixed x, fixed y, const regular_bg_item& item)` (L78): Creates a regular_bg_ptr from the given regular_bg_item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(fixed x, fixed y, const regular_bg_item& item, int map_index)` (L88): Creates a regular_bg_ptr from the given regular_bg_item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(const fixed_point& position, const regular_bg_item& item)` (L96): Creates a regular_bg_ptr from the given regular_bg_item.
  `position` Position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create( const fixed_point& position, const regular_bg_item& item, int map_index)` (L105): Creates a regular_bg_ptr from the given regular_bg_item.
  `position` Position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(fixed x, fixed y, regular_bg_map_ptr map)` (L115): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(const fixed_point& position, regular_bg_map_ptr map)` (L123): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `position` Position of the regular background.; `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(const regular_bg_builder& builder)` (L130): Creates a regular_bg_ptr from a regular_bg_builder reference.
  `builder` regular_bg_builder reference.; devuelve: The requested regular_bg_ptr.
- **create** `static regular_bg_ptr create(regular_bg_builder&& builder)` (L137): Creates a regular_bg_ptr from a moved regular_bg_builder.
  `builder` regular_bg_builder to move.; devuelve: The requested regular_bg_ptr.
- **create_optional** `static optional<regular_bg_ptr> create_optional(const regular_bg_item& item)` (L144): Creates a regular_bg_ptr from the given regular_bg_item.
  `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(const regular_bg_item& item, int map_index)` (L152): Creates a regular_bg_ptr from the given regular_bg_item.
  `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(regular_bg_map_ptr map)` (L159): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(fixed x, fixed y, const regular_bg_item& item)` (L168): Creates a regular_bg_ptr from the given regular_bg_item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional( fixed x, fixed y, const regular_bg_item& item, int map_index)` (L178): Creates a regular_bg_ptr from the given regular_bg_item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional( const fixed_point& position, const regular_bg_item& item)` (L187): Creates a regular_bg_ptr from the given regular_bg_item.
  `position` Position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional( const fixed_point& position, const regular_bg_item& item, int map_index)` (L197): Creates a regular_bg_ptr from the given regular_bg_item.
  `position` Position of the regular background.; `item` regular_bg_item containing the required information to generate the regular background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(fixed x, fixed y, regular_bg_map_ptr map)` (L207): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional( const fixed_point& position, regular_bg_map_ptr map)` (L215): Creates a regular_bg_ptr from the given regular_bg_map_ptr.
  `position` Position of the regular background.; `map` regular_bg_map_ptr used by the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(const regular_bg_builder& builder)` (L223): Creates a regular_bg_ptr from a regular_bg_builder reference.
  `builder` regular_bg_builder reference.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<regular_bg_ptr> create_optional(regular_bg_builder&& builder)` (L230): Creates a regular_bg_ptr from a moved regular_bg_builder.
  `builder` regular_bg_builder to move.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **regular_bg_ptr** `regular_bg_ptr(const regular_bg_ptr& other)` (L236): Copy constructor.
  `other` regular_bg_ptr to copy.
- **operator=** `regular_bg_ptr& operator=(const regular_bg_ptr& other)` (L243): Copy assignment operator.
  `other` regular_bg_ptr to copy.; devuelve: Reference to this.
- **regular_bg_ptr** `regular_bg_ptr(regular_bg_ptr&& other) noexcept : regular_bg_ptr(other._handle)` (L249): Move constructor.
  `other` regular_bg_ptr to move.
- **operator=** `regular_bg_ptr& operator=(regular_bg_ptr&& other) noexcept` (L260): Move assignment operator.
  `other` regular_bg_ptr to move.; devuelve: Reference to this.
- **~regular_bg_ptr** `~regular_bg_ptr()` (L269): Releases the referenced regular background if no more regular_bg_ptr objects reference to it.
- **dimensions** `size dimensions() const` (L274): Returns the size in pixels of the regular background.
- **big** `bool big() const` (L281): Indicates if this regular background is big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **tiles** `const regular_bg_tiles_ptr& tiles() const` (L286): Returns the tiles used by this regular background.
- **set_tiles** `void set_tiles(const regular_bg_tiles_ptr& tiles)` (L294): Sets the tiles used by this regular background.
  `tiles` regular_bg_tiles_ptr to copy.
  It must be compatible with the current map of the regular background.
- **set_tiles** `void set_tiles(regular_bg_tiles_ptr&& tiles)` (L302): Sets the tiles used by this regular background.
  `tiles` regular_bg_tiles_ptr to move.
  It must be compatible with the current map of the regular background.
- **set_tiles** `void set_tiles(const regular_bg_tiles_item& tiles_item)` (L315): Replaces the tiles used by this regular background with a new tile set created with the given regular_bg_tiles_item.
  `tiles_item` It creates the new background tiles to use by this regular background.
  Before creating a new background tile set,
  the regular_bg_tiles_ptr used by this regular background is removed, so VRAM usage is reduced.
  
  The new background tiles must be compatible with the current map of the regular background.
- **palette** `const bg_palette_ptr& palette() const` (L320): Returns the color palette used by this regular background.
- **set_palette** `void set_palette(const bg_palette_ptr& palette)` (L328): Sets the color palette to use by this regular background.
  `palette` bg_palette_ptr to copy.
  It must be compatible with the current map of the regular background.
- **set_palette** `void set_palette(bg_palette_ptr&& palette)` (L336): Sets the color palette to use by this regular background.
  `palette` bg_palette_ptr to move.
  It must be compatible with the current map of the regular background.
- **set_palette** `void set_palette(const bg_palette_item& palette_item)` (L349): Replaces the color palette used by this regular background with a new one created with the given bg_palette_item.
  `palette_item` It creates the color palette to use by this regular background.
  Before creating a new color palette, the bg_palette_ptr used by this regular background is removed,
  so VRAM usage is reduced.
  
  The new color palette must be compatible with the current map of the regular background.
- **set_tiles_and_palette** `void set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L359): Sets the tiles and the color palette to use by this regular background.
  `tiles` regular_bg_tiles_ptr to set.; `palette` bg_palette_ptr to set.
  The given parameters must be compatible with the current map of the regular background.
- **set_tiles_and_palette** `void set_tiles_and_palette(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item)` (L371): Replaces the tiles and the color palette used by this regular background with the created with the given regular_bg_tiles_item and bg_palette_item.
  `tiles_item` It creates the new background tiles to use by this regular background.; `palette_item` It creates the color palette to use by this regular background.
  Before creating new resources, the resources used by this regular background are removed,
  so VRAM usage is reduced.
- **map** `const regular_bg_map_ptr& map() const` (L376): Returns the map used by this regular background.
- **set_map** `void set_map(const regular_bg_map_ptr& map)` (L382): Sets the map used by this regular background.
  `map` regular_bg_map_ptr to copy.
- **set_map** `void set_map(regular_bg_map_ptr&& map)` (L388): Sets the map used by this regular background.
  `map` regular_bg_map_ptr to move.
- **set_map** `void set_map(const regular_bg_map_item& map_item)` (L399): Replaces the map used by this regular background with a new map created with the given regular_bg_map_item.
  `map_item` It creates the new map to use by this regular background.
  Before creating a new map, the regular_bg_map_ptr used by this regular background is removed,
  so VRAM usage is reduced.
- **set_map** `void set_map(const regular_bg_map_item& map_item, int map_index)` (L411): Replaces the map used by this regular background with a new map created with the given regular_bg_map_item.
  `map_item` It creates the new map to use by this regular background.; `map_index` Index of the map to reference in map_item.
  Before creating a new map, the regular_bg_map_ptr used by this regular background is removed,
  so VRAM usage is reduced.
- **set_item** `void set_item(const regular_bg_item& item)` (L422): Replaces the tiles, the color palette and the map used by this regular background with the created with the given regular_bg_item.
  `item` It creates the resources to use by this regular background.
  Before creating new resources, the resources used by this regular background are removed,
  so VRAM usage is reduced.
- **set_item** `void set_item(const regular_bg_item& item, int map_index)` (L434): Replaces the tiles, the color palette and the map used by this regular background with the created with the given regular_bg_item.
  `item` It creates the resources to use by this regular background.; `map_index` Index of the map to reference in item.map_item().
  Before creating new resources, the resources used by this regular background are removed,
  so VRAM usage is reduced.
- **x** `fixed x() const` (L439): Returns the horizontal position of the regular background (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L444): Sets the horizontal position of the regular background (relative to its camera, if it has one).
- **y** `fixed y() const` (L449): Returns the vertical position of the regular background (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L454): Sets the vertical position of the regular background (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L459): Returns the position of the regular background (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L466): Sets the position of the regular background (relative to its camera, if it has one).
  `x` Horizontal position of the regular background (relative to its camera, if it has one).; `y` Vertical position of the regular background (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L471): Sets the position of the regular background (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L476): Returns the horizontal top-left position of the regular background (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L481): Sets the horizontal top-left position of the regular background (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L486): Returns the vertical top-left position of the regular background (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L491): Sets the vertical top-left position of the regular background (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L496): Returns the top-left position of the regular background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L503): Sets the top-left position of the regular background (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the regular background (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the regular background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L508): Sets the top-left position of the regular background (relative to its camera, if it has one).
- **priority** `int priority() const` (L516): Returns the priority of the regular background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L526): Sets the priority of the regular background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **z_order** `int z_order() const` (L535): Returns the priority of the regular background relative to other backgrounds, excluding sprites.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **set_z_order** `void set_z_order(int z_order)` (L546): Sets the priority of the regular background relative to other backgrounds, excluding sprites.
  `z_order` Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **put_above** `void put_above()` (L551): Modify this regular background to be drawn above all of the other backgrounds with the same priority.
- **put_below** `void put_below()` (L556): Modify this regular background to be drawn below all of the other backgrounds with the same priority.
- **mosaic_enabled** `bool mosaic_enabled() const` (L561): Indicates if the mosaic effect must be applied to this regular background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L566): Sets if the mosaic effect must be applied to this regular background or not.
- **blending_enabled** `bool blending_enabled() const` (L573): Indicates if blending must be applied to this regular background or not.
  Blending is applied to this regular background by making it part of the blending top layer.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L583): Sets if blending must be applied to this regular background or not.
  Blending is applied to this regular background by making it part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L591): Indicates if this regular background is part of the blending top layer or not.
- **set_blending_top_enabled** `void set_blending_top_enabled(bool blending_top_enabled)` (L596): Sets if this regular background is part of the blending top layer or not.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L601): Indicates if this regular background is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L606): Sets if this regular background is part of the blending bottom layer or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L611): Indicates how this regular background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L616): Sets how this regular background must be displayed when green swap is enabled.
- **visible** `bool visible() const` (L621): Indicates if this regular background must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L626): Sets if this regular background must be committed to the GBA or not.
- **visible_in_window** `bool visible_in_window(const window& window) const` (L631): Indicates if this regular background is visible in the given window or not.
- **set_visible_in_window** `void set_visible_in_window(bool visible, window& window)` (L636): Sets if this regular background must be visible in the given window or not.
- **camera** `const optional<camera_ptr>& camera() const` (L641): Returns the camera_ptr attached to this regular background (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L647): Sets the camera_ptr attached to this regular background.
  `camera` camera_ptr to copy to this regular background.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L653): Sets the camera_ptr attached to this regular background.
  `camera` camera_ptr to move to this regular background.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L659): Sets or removes the camera_ptr attached to this regular background.
  `camera` Optional camera_ptr to copy to this regular background.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L665): Sets or removes the camera_ptr attached to this regular background.
  `camera` Optional camera_ptr to move to this regular background.
- **remove_camera** `void remove_camera()` (L670): Removes the camera_ptr attached to this regular background (if any).
- **attributes** `regular_bg_attributes attributes() const` (L675): Returns the attributes to commit to the GBA for this regular background.
- **set_attributes** `void set_attributes(const regular_bg_attributes& attributes)` (L680): Sets the attributes to commit to the GBA for this regular background.
- **hw_id** `optional<int> hw_id() const` (L694): Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
  Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
  
  A background doesn't have an assigned hardware ID if it is not visible.
  
  Assigned hardware ID can change after calling this method
  if some properties of this background or others are updated.
  
  Call this method at your own risk.
- **handle** `const void* handle() const` (L699): Returns the internal handle.
- **swap** `void swap(regular_bg_ptr& other)` (L708): Exchanges the contents of this regular_bg_ptr with those of the other one.
  `other` regular_bg_ptr to exchange the contents with.
- **swap** `friend void swap(regular_bg_ptr& a, regular_bg_ptr& b)` (L718): Exchanges the contents of a regular_bg_ptr with those of another one.
  `a` First regular_bg_ptr to exchange the contents with.; `b` Second regular_bg_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const regular_bg_ptr& a, const regular_bg_ptr& b) = default` (L726): Default equal operator.
