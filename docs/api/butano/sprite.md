# Butano 21.10.1 — Sprites (`sprite`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_config_sprite_tiles.h

### `BN_CFG_SPRITE_TILES_MAX_ITEMS` (macro, bn_config_sprite_tiles.h:27)

Specifies the maximum number of sprite tile sets that can be created with bn::sprite_tiles_ptr static constructors.

`#define BN_CFG_SPRITE_TILES_MAX_ITEMS 128`

### `BN_CFG_SPRITE_TILES_LOG_ENABLED` (macro, bn_config_sprite_tiles.h:40)

Specifies if the sprite tiles manager must be logged in detail or not.

`#define BN_CFG_SPRITE_TILES_LOG_ENABLED false`

Keep in mind that it is very slow, it is useful only for debugging.

### `BN_CFG_SPRITE_TILES_SANITY_CHECK_ENABLED` (macro, bn_config_sprite_tiles.h:53)

Specifies if the sprite tiles manager sanity check is enabled or not.

`#define BN_CFG_SPRITE_TILES_SANITY_CHECK_ENABLED false`

Sanity check asserts if the internal state of the manager is valid.


## bn_config_sprites.h

### `BN_CFG_SPRITES_MAX_ITEMS` (macro, bn_config_sprites.h:29)

Specifies the maximum number of sprite items that can be created with bn::sprite_ptr static constructors.

`#define BN_CFG_SPRITES_MAX_ITEMS 128`

A sprite item which is outside of the screen or hidden is not committed to the GBA,
so there can be more than 128 sprite items.

### `BN_CFG_SPRITES_MAX_SORT_LAYERS` (macro, bn_config_sprites.h:43)

Specifies the maximum number of used sprite sort layers.

`#define BN_CFG_SPRITES_MAX_SORT_LAYERS 16`

Sprites are grouped in layers depending of their background priority and z order,
so to reduce memory usage and improve performance, please use as few unique z orders as possible.

### `BN_CFG_SPRITES_USE_IWRAM` (macro, bn_config_sprites.h:54)

Indicates if the sprites manager should use IWRAM to improve performance or not.

`#define BN_CFG_SPRITES_USE_IWRAM true`


## bn_sprite_affine_second_attributes.h

### `bn::sprite_affine_second_attributes` (class, bn_sprite_affine_second_attributes.h:28)

Manages the attributes to commit to the second GBA register of an affine sprite (with an attached sprite_affine_mat_ptr).

`class sprite_affine_second_attributes`

- **sprite_affine_second_attributes** `sprite_affine_second_attributes(fixed x, const sprite_affine_mat_ptr& affine_mat)` (L37): Constructor.
  `x` Horizontal position of a sprite (relative to its camera, if it has one).; `affine_mat` Reference to the sprite_affine_mat_ptr attached to a sprite.
- **sprite_affine_second_attributes** `sprite_affine_second_attributes(fixed x, sprite_affine_mat_ptr&& affine_mat)` (L44): Constructor.
  `x` Horizontal position of a sprite (relative to its camera, if it has one).; `affine_mat` Moved reference to the sprite_affine_mat_ptr attached to a sprite.
- **x** `fixed x() const` (L49): Returns the horizontal position of a sprite (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L57): Sets the horizontal position of a sprite (relative to its camera, if it has one).
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L65): Returns the sprite_affine_mat_ptr attached to a sprite.
- **set_affine_mat** `void set_affine_mat(const sprite_affine_mat_ptr& affine_mat)` (L74): Sets the sprite_affine_mat_ptr attached to a sprite.
  `affine_mat` sprite_affine_mat_ptr to copy.
- **set_affine_mat** `void set_affine_mat(sprite_affine_mat_ptr&& affine_mat)` (L83): Sets the sprite_affine_mat_ptr attached to a sprite.
  `affine_mat` sprite_affine_mat_ptr to move.
- **operator==** `friend bool operator==(const sprite_affine_second_attributes& a, const sprite_affine_second_attributes& b) = default` (L91): Default equal operator.

## bn_sprite_builder.h

### `bn::sprite_builder` (class, bn_sprite_builder.h:37)

Creates sprite_ptr objects with custom attributes.

`class sprite_builder`

If some of the attributes of the sprite to create differ from the default ones,
using this class improves performance.

- **sprite_builder** `explicit sprite_builder(const sprite_item& item)` (L45): Constructor.
  `item` sprite_item containing the required information to generate sprites.
- **sprite_builder** `sprite_builder(const sprite_item& item, int graphics_index)` (L52): Constructor.
  `item` sprite_item containing the required information to generate sprites.; `graphics_index` Index of the tile set to reference in item.tiles_item() to generate sprites.
- **sprite_builder** `sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L60): Constructor.
  `shape_size` Shape and size of the sprites to generate.; `tiles` Smart pointer to the sprite tile set used to generate sprites.; `palette` Smart pointer to the sprite color palette used to generate sprites.
- **item** `const optional<sprite_item>& item() const` (L66): Returns the sprite_item containing the required information to generate sprites if it has one; bn::nullopt otherwise.
- **graphics_index** `int graphics_index() const` (L74): Index of the tile set to reference in item.tiles_item() to generate sprites.
- **shape_size** `const sprite_shape_size& shape_size() const` (L82): Returns the shape and size of the sprites to generate.
- **x** `fixed x() const` (L91): Returns the horizontal position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_x** `sprite_builder& set_x(fixed x)` (L102): Sets the horizontal position of the sprites to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the sprites to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L112): Returns the vertical position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_y** `sprite_builder& set_y(fixed y)` (L123): Sets the vertical position of the sprites to generate (relative to their camera, if they are going to have one).
  `y` Vertical position of the sprites to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L133): Returns the position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_position** `sprite_builder& set_position(fixed x, fixed y)` (L144): Sets the position of the sprites to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the sprites to generate.; `y` Vertical position of the sprites to generate.; devuelve: Reference to this.
- **set_position** `sprite_builder& set_position(const fixed_point& position)` (L155): Sets the position of the sprites to generate (relative to their camera, if they are going to have one).
  `position` Position of the sprites to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L165): Returns the horizontal top-left position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `sprite_builder& set_top_left_x(fixed top_left_x)` (L173): Sets the horizontal top-left position of the sprites to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the sprites to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L179): Returns the vertical top-left position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `sprite_builder& set_top_left_y(fixed top_left_y)` (L187): Sets the vertical top-left position of the sprites to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the sprites to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L193): Returns the top-left position of the sprites to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `sprite_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L202): Sets the top-left position of the sprites to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the sprites to generate.; `top_left_y` Vertical top-left position of the sprites to generate.; devuelve: Reference to this.
- **set_top_left_position** `sprite_builder& set_top_left_position(const fixed_point& top_left_position)` (L210): Sets the top-left position of the sprites to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the sprites to generate.; devuelve: Reference to this.
- **rotation_angle** `fixed rotation_angle() const` (L215): Returns the rotation angle in degrees of the sprites to generate.
- **set_rotation_angle** `sprite_builder& set_rotation_angle(fixed rotation_angle)` (L226): Sets the rotation angle in degrees of the sprites to generate.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; devuelve: Reference to this.
  If the rotation angle is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given rotation angle is attached to it.
- **set_rotation_angle_safe** `sprite_builder& set_rotation_angle_safe(fixed rotation_angle)` (L237): Sets the rotation angle in degrees of the sprites to generate.
  `rotation_angle` Rotation angle in degrees, in any range.; devuelve: Reference to this.
  If the rotation angle is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given rotation angle is attached to it.
- **horizontal_scale** `fixed horizontal_scale() const` (L242): Returns the horizontal scale of the sprites to generate.
- **set_horizontal_scale** `sprite_builder& set_horizontal_scale(fixed horizontal_scale)` (L253): Sets the horizontal scale of the sprites to generate.
  `horizontal_scale` Horizontal scale of the sprites to generate.; devuelve: Reference to this.
  If the horizontal scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given horizontal scale is attached to it.
- **vertical_scale** `fixed vertical_scale() const` (L258): Returns the vertical scale of the sprites to generate.
- **set_vertical_scale** `sprite_builder& set_vertical_scale(fixed vertical_scale)` (L269): Sets the vertical scale of the sprites to generate.
  `vertical_scale` Vertical scale of the sprites to generate.; devuelve: Reference to this.
  If the vertical scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical scale is attached to it.
- **set_scale** `sprite_builder& set_scale(fixed scale)` (L280): Sets the scale of the sprites to generate.
  `scale` Scale of the sprites to generate.; devuelve: Reference to this.
  If the scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical scale is attached to it.
- **set_scale** `sprite_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)` (L292): Sets the scale of the sprites to generate.
  `horizontal_scale` Horizontal scale of the sprites to generate.; `vertical_scale` Vertical scale of the sprites to generate.; devuelve: Reference to this.
  If the horizontal or the vertical scale is != 1 and the builder doesn't have
  an attached sprite_affine_mat_ptr, a new one with the given vertical scale is attached to it.
- **horizontal_shear** `fixed horizontal_shear() const` (L297): Returns the horizontal shear of the sprites to generate.
- **set_horizontal_shear** `sprite_builder& set_horizontal_shear(fixed horizontal_shear)` (L308): Sets the horizontal shear of the sprites to generate.
  `horizontal_shear` Horizontal shear of the sprites to generate.; devuelve: Reference to this.
  If the horizontal shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given horizontal shear is attached to it.
- **vertical_shear** `fixed vertical_shear() const` (L313): Returns the vertical shear of the sprites to generate.
- **set_vertical_shear** `sprite_builder& set_vertical_shear(fixed vertical_shear)` (L324): Sets the vertical shear of the sprites to generate.
  `vertical_shear` Vertical shear of the sprites to generate.; devuelve: Reference to this.
  If the vertical shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical shear is attached to it.
- **set_shear** `sprite_builder& set_shear(fixed shear)` (L335): Sets the shear of the sprites to generate.
  `shear` Shear of the sprites to generate.; devuelve: Reference to this.
  If the shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical shear is attached to it.
- **set_shear** `sprite_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)` (L347): Sets the shear of the sprites to generate.
  `horizontal_shear` Horizontal shear of the sprites to generate.; `vertical_shear` Vertical shear of the sprites to generate.; devuelve: Reference to this.
  If the horizontal or the vertical shear is != 0 and the builder doesn't have
  an attached sprite_affine_mat_ptr, a new one with the given vertical shear is attached to it.
- **bg_priority** `int bg_priority() const` (L355): Returns the priority of the sprites to generate relative to backgrounds.
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **set_bg_priority** `sprite_builder& set_bg_priority(int bg_priority)` (L369): Sets the priority of the sprites to generate relative to backgrounds.
  `bg_priority` Priority relative to backgrounds in the range [0..3].; devuelve: Reference to this.
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **z_order** `int z_order() const` (L379): Returns the priority of the sprites to generate relative to other ones.
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
  
  Sprites are grouped in layers depending of their background priority and z order,
  so to reduce memory usage and improve performance, please use as few unique z orders as possible.
- **set_z_order** `sprite_builder& set_z_order(int z_order)` (L395): Sets the priority of the sprites to generate relative to other ones.
  `z_order` Priority relative to other sprites in the range [-32767..32767].; devuelve: Reference to this.
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
  
  Sprites are grouped in layers depending of their background priority and z order,
  so to reduce memory usage and improve performance, please use as few unique z orders as possible.
- **horizontal_flip** `bool horizontal_flip() const` (L400): Indicates if the sprites to generate are flipped in the horizontal axis or not.
- **set_horizontal_flip** `sprite_builder& set_horizontal_flip(bool horizontal_flip)` (L408): Sets if the sprites to generate must be flipped in the horizontal axis or not.
  `horizontal_flip` `true` if the sprites must be flipped in the horizontal axis; `false` otherwise.; devuelve: Reference to this.
- **vertical_flip** `bool vertical_flip() const` (L413): Indicates if the sprites to generate are flipped in the vertical axis or not.
- **set_vertical_flip** `sprite_builder& set_vertical_flip(bool vertical_flip)` (L420): Sets if the sprites to generate must be flipped in the vertical axis or not.
  `vertical_flip` `true` if the sprites must be flipped in the vertical axis; `false` otherwise.; devuelve: Reference to this.
- **mosaic_enabled** `bool mosaic_enabled() const` (L425): Indicates if the mosaic effect must be applied to the sprites to generate or not.
- **set_mosaic_enabled** `sprite_builder& set_mosaic_enabled(bool mosaic_enabled)` (L435): Sets if the mosaic effect must be applied to the sprites to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L446): Indicates if blending must be applied to the sprites to generate or not.
  Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
- **set_blending_enabled** `sprite_builder& set_blending_enabled(bool blending_enabled)` (L459): Sets if blending must be applied to the sprites to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
- **window_enabled** `bool window_enabled() const` (L466): Indicates if the sprites to generate must be part of the silhouette of the sprite window or not.
  Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
- **set_window_enabled** `sprite_builder& set_window_enabled(bool window_enabled)` (L480): Sets if the sprites to generate must be part of the silhouette of the sprite window or not.
  `window_enabled` `true` if the sprites must be part of the silhouette of the sprite window; `false` otherwise.; devuelve: Reference to this.
  Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
- **double_size_mode** `sprite_double_size_mode double_size_mode() const` (L486): Returns the area the sprites to generate use to be drawn when they have an attached sprite_affine_mat_ptr.
- **set_double_size_mode** `sprite_builder& set_double_size_mode(sprite_double_size_mode double_size_mode)` (L497): Specifies the area the sprites to generate use to be drawn when they have an attached sprite_affine_mat_ptr.
  `double_size_mode` sprite_double_size_mode enum value.; devuelve: Reference to this.
- **visible** `bool visible() const` (L506): Indicates if the sprites to generate must be committed to the GBA or not.
- **set_visible** `sprite_builder& set_visible(bool visible)` (L516): Sets if the sprites to generate must be committed to the GBA or not.
  `visible` `true` if the sprites must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L525): Returns the camera_ptr to attach to the sprites to generate (if any).
- **set_camera** `sprite_builder& set_camera(const camera_ptr& camera)` (L535): Sets the camera_ptr to attach to the sprites to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `sprite_builder& set_camera(camera_ptr&& camera)` (L546): Sets the camera_ptr to attach to the sprites to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `sprite_builder& set_camera(const optional<camera_ptr>& camera)` (L557): Sets or removes the camera_ptr to attach to the sprites to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `sprite_builder& set_camera(optional<camera_ptr>&& camera)` (L568): Sets or removes the camera_ptr to attach to the sprites to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `sprite_builder& remove_camera()` (L578): Removes the camera_ptr to attach to the sprites to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L587): Releases and returns the camera_ptr to attach to the sprites to generate (if any).
- **build** `sprite_ptr build() const` (L592): Generates and returns a sprite_ptr without releasing the acquired resources.
- **release_build** `sprite_ptr release_build()` (L599): Generates and returns a sprite_ptr releasing the acquired resources.
  sprite_ptr generation after calling this method may stop working.
- **build_optional** `optional<sprite_ptr> build_optional() const` (L605): Generates and returns a sprite_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<sprite_ptr> release_build_optional()` (L613): Generates and returns a sprite_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  sprite_ptr generation after calling this method may stop working.
- **tiles** `sprite_tiles_ptr tiles() const` (L618): Generates and returns a sprite_tiles_ptr without releasing the acquired resources.
- **palette** `sprite_palette_ptr palette() const` (L623): Generates and returns a sprite_palette_ptr without releasing the acquired resources.
- **tiles_optional** `optional<sprite_tiles_ptr> tiles_optional() const` (L629): Generates and returns a sprite_tiles_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **palette_optional** `optional<sprite_palette_ptr> palette_optional() const` (L635): Generates and returns a sprite_palette_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_tiles** `sprite_tiles_ptr release_tiles()` (L642): Generates and returns a sprite_tiles_ptr releasing the acquired resources.
  sprite_ptr generation after calling this method may stop working.
- **release_palette** `sprite_palette_ptr release_palette()` (L649): Generates and returns a sprite_palette_ptr releasing the acquired resources.
  sprite_ptr generation after calling this method may stop working.
- **release_tiles_optional** `optional<sprite_tiles_ptr> release_tiles_optional()` (L657): Generates and returns a sprite_tiles_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  sprite_ptr generation after calling this method may stop working.
- **release_palette_optional** `optional<sprite_palette_ptr> release_palette_optional()` (L665): Generates and returns a sprite_palette_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  sprite_ptr generation after calling this method may stop working.
- **affine_mat** `const optional<sprite_affine_mat_ptr>& affine_mat() const` (L670): Returns the sprite_affine_mat_ptr to attach to the sprites to generate (if any).
- **set_affine_mat** `sprite_builder& set_affine_mat(const sprite_affine_mat_ptr& affine_mat)` (L683): Sets the sprite_affine_mat_ptr to attach to the sprites to generate.
  `affine_mat` sprite_affine_mat_ptr to copy to the builder.; devuelve: Reference to this.
  This method calls set_remove_affine_mat_when_not_needed(false).
- **set_affine_mat** `sprite_builder& set_affine_mat(sprite_affine_mat_ptr&& affine_mat)` (L693): Sets the sprite_affine_mat_ptr to attach to the sprites to generate.
  `affine_mat` sprite_affine_mat_ptr to move to the builder.; devuelve: Reference to this.
  This method calls set_remove_affine_mat_when_not_needed(false).
- **set_affine_mat** `sprite_builder& set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)` (L704): Sets or removes the sprite_affine_mat_ptr to attach to the sprites to generate.
  `affine_mat` Optional sprite_affine_mat_ptr to copy to the builder.; devuelve: Reference to this.
  This method calls set_remove_affine_mat_when_not_needed(false)
  if the given sprite_affine_mat_ptr is not null.
- **set_affine_mat** `sprite_builder& set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)` (L715): Sets or removes the sprite_affine_mat_ptr to attach to the sprites to generate.
  `affine_mat` Optional sprite_affine_mat_ptr to move to the builder.; devuelve: Reference to this.
  This method calls set_remove_affine_mat_when_not_needed(false)
  if the given sprite_affine_mat_ptr is not null.
- **remove_affine_mat** `sprite_builder& remove_affine_mat()` (L721): Removes the sprite_affine_mat_ptr to attach to the sprites to generate.
  devuelve: Reference to this.
- **release_affine_mat** `optional<sprite_affine_mat_ptr> release_affine_mat()` (L730): Releases and returns the sprite_affine_mat_ptr to attach to the sprites to generate (if any).
- **remove_affine_mat_when_not_needed** `bool remove_affine_mat_when_not_needed() const` (L738): Indicates if the sprite_affine_mat_ptr attached to the sprites to generate (if any) must be removed when it is not needed anymore.
  A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.
- **set_remove_affine_mat_when_not_needed** `sprite_builder& set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)` (L752): Sets if the sprite_affine_mat_ptr attached to the sprites to generate (if any) must be removed when it is not needed anymore.
  `remove_when_not_needed` `true` if the sprite_affine_mat_ptr must be removed when it is not needed anymore; `false` otherwise.; devuelve: Reference to this.
  A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.

## bn_sprite_double_size_mode.h

### `bn::sprite_double_size_mode` (enum, bn_sprite_double_size_mode.h:26)

Specifies the area a sprite uses to be drawn when it has an attached sprite_affine_mat_ptr.

`enum class sprite_double_size_mode : uint8_t { AUTO /* The sprite uses 2x the area specified by its shape and size only when its sprite_affine_mat_ptr requires it. */, ENABLED /* The sprite always uses 2x the area specified by its shape and size. */, DISABLED /* The sprite never uses more pixels than the area specified by its shape and size. */ }`


## bn_sprite_first_attributes.h

### `bn::sprite_first_attributes` (class, bn_sprite_first_attributes.h:27)

Manages the attributes to commit to the first GBA register of a sprite.

`class sprite_first_attributes`

- **sprite_first_attributes** `constexpr sprite_first_attributes(fixed y, bool mosaic_enabled, bool blending_enabled, bool window_enabled, bool visible)` (L44): Constructor.
  `y` Vertical position of a sprite (relative to its camera, if it has one).; `mosaic_enabled` Indicates if the mosaic effect is applied to a sprite or not.; `blending_enabled` Indicates if blending is applied to a sprite or not.; `window_enabled` Indicates if a sprite must be part of the silhouette of the sprite window or not.; `visible` Indicates if a sprite must be committed to the GBA or not.
  Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
- **y** `constexpr fixed y() const` (L59): Returns the vertical position of a sprite (relative to its camera, if it has one).
- **set_y** `void constexpr set_y(fixed y)` (L67): Sets the vertical position of a sprite (relative to its camera, if it has one).
- **mosaic_enabled** `constexpr bool mosaic_enabled() const` (L75): Indicates if the mosaic effect is applied to a sprite or not.
- **set_mosaic_enabled** `constexpr void set_mosaic_enabled(bool mosaic_enabled)` (L83): Sets if the mosaic effect is applied to a sprite or not.
- **blending_enabled** `constexpr bool blending_enabled() const` (L91): Indicates if blending is applied to a sprite or not.
- **set_blending_enabled** `constexpr void set_blending_enabled(bool blending_enabled)` (L101): Sets if blending is applied to a sprite or not.
  Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
- **window_enabled** `constexpr bool window_enabled() const` (L112): Indicates if a sprite must be part of the silhouette of the sprite window or not.
- **set_window_enabled** `constexpr void set_window_enabled(bool window_enabled)` (L122): Sets if a sprite must be part of the silhouette of the sprite window or not.
  Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
- **visible** `constexpr bool visible() const` (L133): Indicates if a sprite must be committed to the GBA or not.
- **set_visible** `constexpr void set_visible(bool visible)` (L141): Sets if a sprite must be committed to the GBA or not.
- **operator==** `constexpr friend bool operator==(const sprite_first_attributes& a, const sprite_first_attributes& b) = default` (L149): Default equal operator.

## bn_sprite_ptr.h

### `bn::sprite_ptr` (class, bn_sprite_ptr.h:47)

std::shared_ptr like smart pointer that retains shared ownership of a sprite.

`class sprite_ptr`

Several sprite_ptr objects may own the same sprite.

The sprite is released when the last remaining sprite_ptr owning it is destroyed.

- **create** `static sprite_ptr create(const sprite_item& item)` (L56): Creates a sprite_ptr from the given sprite_item.
  `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(const sprite_item& item, int graphics_index)` (L64): Creates a sprite_ptr from the given sprite_item.
  `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create( const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L73): Creates a sprite_ptr.
  `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(fixed x, fixed y, const sprite_item& item)` (L83): Creates a sprite_ptr from the given sprite_item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(fixed x, fixed y, const sprite_item& item, int graphics_index)` (L93): Creates a sprite_ptr from the given sprite_item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(const fixed_point& position, const sprite_item& item)` (L101): Creates a sprite_ptr from the given sprite_item.
  `position` Position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(const fixed_point& position, const sprite_item& item, int graphics_index)` (L110): Creates a sprite_ptr from the given sprite_item.
  `position` Position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(fixed x, fixed y, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L121): Creates a sprite_ptr.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L132): Creates a sprite_ptr.
  `position` Position of the sprite.; `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(const sprite_builder& builder)` (L140): Creates a sprite_ptr from a sprite_builder reference.
  `builder` sprite_builder reference.; devuelve: The requested sprite_ptr.
- **create** `static sprite_ptr create(sprite_builder&& builder)` (L147): Creates a sprite_ptr from a moved sprite_builder.
  `builder` sprite_builder to move.; devuelve: The requested sprite_ptr.
- **create_optional** `static optional<sprite_ptr> create_optional(const sprite_item& item)` (L154): Creates a sprite_ptr from the given sprite_item.
  `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(const sprite_item& item, int graphics_index)` (L162): Creates a sprite_ptr from the given sprite_item.
  `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional( const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L171): Creates a sprite_ptr.
  `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(fixed x, fixed y, const sprite_item& item)` (L181): Creates a sprite_ptr from the given sprite_item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(fixed x, fixed y, const sprite_item& item, int graphics_index)` (L191): Creates a sprite_ptr from the given sprite_item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(const fixed_point& position, const sprite_item& item)` (L200): Creates a sprite_ptr from the given sprite_item.
  `position` Position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(const fixed_point& position, const sprite_item& item, int graphics_index)` (L209): Creates a sprite_ptr from the given sprite_item.
  `position` Position of the sprite.; `item` sprite_item containing the required information to generate the sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional( fixed x, fixed y, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L221): Creates a sprite_ptr.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional( const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L232): Creates a sprite_ptr.
  `position` Position of the sprite.; `shape_size` Shape and size of the sprite.; `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(const sprite_builder& builder)` (L241): Creates a sprite_ptr from a sprite_builder reference.
  `builder` sprite_builder reference.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_ptr> create_optional(sprite_builder&& builder)` (L248): Creates a sprite_ptr from a moved sprite_builder.
  `builder` sprite_builder to move.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **sprite_ptr** `sprite_ptr(const sprite_ptr& other)` (L254): Copy constructor.
  `other` sprite_ptr to copy.
- **operator=** `sprite_ptr& operator=(const sprite_ptr& other)` (L261): Copy assignment operator.
  `other` sprite_ptr to copy.; devuelve: Reference to this.
- **sprite_ptr** `sprite_ptr(sprite_ptr&& other) noexcept : sprite_ptr(other._handle)` (L267): Move constructor.
  `other` sprite_ptr to move.
- **operator=** `sprite_ptr& operator=(sprite_ptr&& other) noexcept` (L278): Move assignment operator.
  `other` sprite_ptr to move.; devuelve: Reference to this.
- **~sprite_ptr** `~sprite_ptr()` (L287): Releases the referenced sprite if no more sprite_ptr objects reference to it.
- **shape_size** `sprite_shape_size shape_size() const` (L292): Returns the shape and size of the sprite.
- **dimensions** `size dimensions() const` (L297): Returns the size in pixels of the sprite.
- **tiles** `const sprite_tiles_ptr& tiles() const` (L302): Returns the tiles used by this sprite.
- **set_tiles** `void set_tiles(const sprite_tiles_ptr& tiles)` (L310): Sets the tiles used by this sprite.
  `tiles` sprite_tiles_ptr to copy.
  It must be compatible with the current color palette, shape and size of the sprite.
- **set_tiles** `void set_tiles(sprite_tiles_ptr&& tiles)` (L318): Sets the tiles used by this sprite.
  `tiles` sprite_tiles_ptr to move.
  It must be compatible with the current color palette, shape and size of the sprite.
- **set_tiles** `void set_tiles(const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles)` (L327): Sets the tiles, the shape and size of the sprite.
  `shape_size` Shape and size of the sprite.; `tiles` sprite_tiles_ptr to copy.
  It must be compatible with the current color palette of the sprite.
- **set_tiles** `void set_tiles(const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles)` (L336): Sets the tiles, the shape and size of the sprite.
  `shape_size` Shape and size of the sprite.; `tiles` sprite_tiles_ptr to move.
  It must be compatible with the current color palette of the sprite.
- **set_tiles** `void set_tiles(const sprite_tiles_item& tiles_item)` (L348): Replaces the tiles used by this sprite with a new tile set created with the given sprite_tiles_item.
  `tiles_item` It creates the new sprite tiles to use by this sprite.
  Before creating a new sprite tile set, the sprite_tiles_ptr used by this sprite is removed,
  so VRAM usage is reduced.
  
  The new sprite tiles must be compatible with the current color palette, shape and size of the sprite.
- **set_tiles** `void set_tiles(const sprite_tiles_item& tiles_item, int graphics_index)` (L361): Replaces the tiles used by this sprite with a new tile set created with the given sprite_tiles_item.
  `tiles_item` It creates the sprite tiles to use by this sprite.; `graphics_index` Index of the tile set to reference in tiles_item.
  Before creating a new sprite tile set, the sprite_tiles_ptr used by this sprite is removed,
  so VRAM usage is reduced.
  
  The new sprite tiles must be compatible with the current color palette, shape and size of the sprite.
- **set_tiles** `void set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size)` (L375): Replaces the tiles used by this sprite with a new tile set created with the given sprite_tiles_item, changing also the shape and size of the sprite.
  `tiles_item` It creates the sprite tiles to use by this sprite.; `shape_size` Shape and size of the sprite.
  Before creating a new sprite tile set, the sprite_tiles_ptr used by this sprite is removed,
  so VRAM usage is reduced.
  
  The new sprite tiles must be compatible with the current color palette of the sprite.
- **set_tiles** `void set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size, int graphics_index)` (L390): Replaces the tiles used by this sprite with a new tile set created with the given sprite_tiles_item, changing also the shape and size of the sprite.
  `tiles_item` It creates the sprite tiles to use by this sprite.; `shape_size` Shape and size of the sprite.; `graphics_index` Index of the tile set to reference in tiles_item.
  Before creating a new sprite tile set, the sprite_tiles_ptr used by this sprite is removed,
  so VRAM usage is reduced.
  
  The new sprite tiles must be compatible with the current color palette of the sprite.
- **palette** `const sprite_palette_ptr& palette() const` (L395): Returns the color palette used by this sprite.
- **set_palette** `void set_palette(const sprite_palette_ptr& palette)` (L403): Sets the color palette to use by this sprite.
  `palette` sprite_palette_ptr to copy.
  It must be compatible with the current tiles of the sprite.
- **set_palette** `void set_palette(sprite_palette_ptr&& palette)` (L411): Sets the color palette to use by this sprite.
  `palette` sprite_palette_ptr to move.
  It must be compatible with the current tiles of the sprite.
- **set_palette** `void set_palette(const sprite_palette_item& palette_item)` (L424): Replaces the color palette used by this sprite with a new one created with the given sprite_palette_item.
  `palette_item` It creates the color palette to use by this sprite.
  Before creating a new color palette, the sprite_palette_ptr used by this sprite is removed,
  so VRAM usage is reduced.
  
  The new color palette must be compatible with the current tiles of the sprite.
- **set_tiles_and_palette** `void set_tiles_and_palette(sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L434): Sets the tiles and the color palette to use by this sprite.
  `tiles` sprite_tiles_ptr to set.; `palette` sprite_palette_ptr to set.
  The given parameters must be compatible with the current shape and size of the sprite.
- **set_tiles_and_palette** `void set_tiles_and_palette(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette)` (L442): Sets the tiles, the color palette, the shape and size to use by this sprite.
  `shape_size` Shape and size of the sprite.; `tiles` sprite_tiles_ptr to set.; `palette` sprite_palette_ptr to set.
- **set_item** `void set_item(const sprite_item& item)` (L453): Replaces the tiles, the color palette, the shape and size used by this sprite with the created with the given sprite_item.
  `item` It creates the resources to use by this sprite.
  Before creating new resources, the resources used by this sprite are removed, so VRAM usage is reduced.
- **set_item** `void set_item(const sprite_item& item, int graphics_index)` (L464): Replaces the tiles, the color palette, the shape and size used by this sprite with the created with the given sprite_item.
  `item` It creates the resources to use by this sprite.; `graphics_index` Index of the tile set to reference in item.tiles_item().
  Before creating new resources, the resources used by this sprite are removed, so VRAM usage is reduced.
- **x** `fixed x() const` (L469): Returns the horizontal position of the sprite (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L474): Sets the horizontal position of the sprite (relative to its camera, if it has one).
- **y** `fixed y() const` (L479): Returns the vertical position of the sprite (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L484): Sets the vertical position of the sprite (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L489): Returns the position of the sprite (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L496): Sets the position of the sprite (relative to its camera, if it has one).
  `x` Horizontal position of the sprite (relative to its camera, if it has one).; `y` Vertical position of the sprite (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L501): Sets the position of the sprite (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L506): Returns the horizontal top-left position of the sprite (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L511): Sets the horizontal top-left position of the sprite (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L516): Returns the vertical top-left position of the sprite (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L521): Sets the vertical top-left position of the sprite (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L526): Returns the top-left position of the sprite (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L533): Sets the top-left position of the sprite (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the sprite (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the sprite (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L538): Sets the top-left position of the sprite (relative to its camera, if it has one).
- **rotation_angle** `fixed rotation_angle() const` (L543): Returns the rotation angle in degrees of the sprite.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L552): Sets the rotation angle in degrees of the sprite.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
  If the rotation angle is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given rotation angle is attached to it.
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L561): Sets the rotation angle in degrees of the sprite.
  `rotation_angle` Rotation angle in degrees, in any range.
  If the rotation angle is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given rotation angle is attached to it.
- **horizontal_scale** `fixed horizontal_scale() const` (L566): Returns the horizontal scale of the sprite.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L574): Sets the horizontal scale of the sprite.
  If the horizontal scale is != 1 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given horizontal scale is attached to it.
- **vertical_scale** `fixed vertical_scale() const` (L579): Returns the vertical scale of the sprite.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L587): Sets the vertical scale of the sprite.
  If the vertical scale is != 1 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical scale is attached to it.
- **set_scale** `void set_scale(fixed scale)` (L595): Sets the scale of the sprite.
  If the scale is != 1 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given scale is attached to it.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L605): Sets the scale of the sprite.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
  If the scale is != 1 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given scale is attached to it.
- **horizontal_shear** `fixed horizontal_shear() const` (L610): Returns the horizontal shear of the sprite.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L618): Sets the horizontal shear of the sprite.
  If the horizontal shear is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given horizontal shear is attached to it.
- **vertical_shear** `fixed vertical_shear() const` (L623): Returns the vertical shear of the sprite.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L631): Sets the vertical shear of the sprite.
  If the vertical shear is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given vertical shear is attached to it.
- **set_shear** `void set_shear(fixed shear)` (L639): Sets the shear of the sprite.
  If the shear is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given shear is attached to it.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L649): Sets the shear of the sprite.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
  If the shear is != 0 and the sprite doesn't have an attached sprite_affine_mat_ptr,
  a new one with the given shear is attached to it.
- **bg_priority** `int bg_priority() const` (L657): Returns the priority relative to backgrounds.
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **set_bg_priority** `void set_bg_priority(int bg_priority)` (L667): Sets the priority relative to backgrounds.
  `bg_priority` Priority relative to backgrounds in the range [0..3].
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **z_order** `int z_order() const` (L677): Returns the priority relative to other sprites.
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
  
  Sprites are grouped in layers depending of their background priority and z order,
  so to reduce memory usage and improve performance, please use as few unique z orders as possible.
- **set_z_order** `void set_z_order(int z_order)` (L689): Sets the priority relative to other sprites.
  `z_order` Priority relative to other sprites in the range [-32767..32767].
  Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
  
  Sprites are grouped in layers depending of their background priority and z order,
  so to reduce memory usage and improve performance, please use as few unique z orders as possible.
- **put_above** `void put_above()` (L694): Modify this sprite to be drawn above all of the other sprites with the same priority.
- **put_below** `void put_below()` (L699): Modify this sprite to be drawn below all of the other sprites with the same priority.
- **horizontal_flip** `bool horizontal_flip() const` (L704): Indicates if this sprite is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L709): Sets if this sprite is flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L714): Indicates if this sprite is flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L719): Sets if this sprite is flipped in the vertical axis or not.
- **mosaic_enabled** `bool mosaic_enabled() const` (L724): Indicates if the mosaic effect must be applied to this sprite or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L729): Sets if the mosaic effect must be applied to this sprite or not.
- **blending_enabled** `bool blending_enabled() const` (L736): Indicates if blending must be applied to this sprite or not.
  Keep in mind that blending and window attributes can't be enabled at the same time.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L743): Sets if blending must be applied to this sprite or not.
  Keep in mind that blending and window attributes can't be enabled at the same time.
- **window_enabled** `bool window_enabled() const` (L750): Indicates if this sprite must be part of the silhouette of the sprite window or not.
  Keep in mind that blending and window attributes can't be enabled at the same time.
- **set_window_enabled** `void set_window_enabled(bool window_enabled)` (L757): Sets if this sprite must be part of the silhouette of the sprite window or not.
  Keep in mind that blending and window attributes can't be enabled at the same time.
- **double_size** `bool double_size() const` (L763): Indicates if this sprite is being drawn using more pixels than the area specified by its shape and size or not.
- **double_size_mode** `sprite_double_size_mode double_size_mode() const` (L768): Returns the area this sprite uses to be drawn when it has an attached sprite_affine_mat_ptr.
- **set_double_size_mode** `void set_double_size_mode(sprite_double_size_mode double_size_mode)` (L773): Specifies the area this sprite uses to be drawn when it has an attached sprite_affine_mat_ptr.
- **visible** `bool visible() const` (L778): Indicates if this sprite must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L783): Sets if this sprite must be committed to the GBA or not.
- **camera** `const optional<camera_ptr>& camera() const` (L788): Returns the camera_ptr attached to this sprite (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L794): Sets the camera_ptr attached to this sprite.
  `camera` camera_ptr to copy to this sprite.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L800): Sets the camera_ptr attached to this sprite.
  `camera` camera_ptr to move to this sprite.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L806): Sets or removes the camera_ptr attached to this sprite.
  `camera` Optional camera_ptr to copy to this sprite.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L812): Sets or removes the camera_ptr attached to this sprite.
  `camera` Optional camera_ptr to move to this sprite.
- **remove_camera** `void remove_camera()` (L817): Removes the camera_ptr attached to this sprite (if any).
- **affine_mat** `const optional<sprite_affine_mat_ptr>& affine_mat() const` (L822): Returns the sprite_affine_mat_ptr attached to this sprite (if any).
- **set_affine_mat** `void set_affine_mat(const sprite_affine_mat_ptr& affine_mat)` (L831): Sets the sprite_affine_mat_ptr attached to this sprite.
  `affine_mat` sprite_affine_mat_ptr to copy to this sprite.
  This method calls set_remove_affine_mat_when_not_needed(false).
- **set_affine_mat** `void set_affine_mat(sprite_affine_mat_ptr&& affine_mat)` (L840): Sets the sprite_affine_mat_ptr attached to this sprite.
  `affine_mat` sprite_affine_mat_ptr to move to this sprite.
  This method calls set_remove_affine_mat_when_not_needed(false).
- **set_affine_mat** `void set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)` (L850): Sets or removes the sprite_affine_mat_ptr attached to this sprite.
  `affine_mat` Optional sprite_affine_mat_ptr to copy to this sprite.
  This method calls set_remove_affine_mat_when_not_needed(false)
  if the given sprite_affine_mat_ptr is not null.
- **set_affine_mat** `void set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)` (L860): Sets or removes the sprite_affine_mat_ptr attached to this sprite.
  `affine_mat` Optional sprite_affine_mat_ptr to move to this sprite.
  This method calls set_remove_affine_mat_when_not_needed(false)
  if the given sprite_affine_mat_ptr is not null.
- **remove_affine_mat** `void remove_affine_mat()` (L865): Removes the sprite_affine_mat_ptr attached to this sprite (if any).
- **remove_affine_mat_when_not_needed** `bool remove_affine_mat_when_not_needed() const` (L873): Indicates if the sprite_affine_mat_ptr attached to this sprite (if any) must be removed when it is not needed anymore.
  A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.
- **set_remove_affine_mat_when_not_needed** `void set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)` (L881): Sets if the sprite_affine_mat_ptr attached to this sprite (if any) must be removed when it is not needed anymore.
  A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.
- **first_attributes** `sprite_first_attributes first_attributes() const` (L886): Returns the attributes to commit to the first GBA register of this sprite.
- **set_first_attributes** `void set_first_attributes(const sprite_first_attributes& first_attributes)` (L891): Sets the attributes to commit to the first GBA register of this sprite.
- **regular_second_attributes** `sprite_regular_second_attributes regular_second_attributes() const` (L897): Returns the attributes to commit to the second GBA register of this sprite when it is regular (it does not have an attached sprite_affine_mat_ptr).
- **set_regular_second_attributes** `void set_regular_second_attributes(const sprite_regular_second_attributes& second_attributes)` (L903): Sets the attributes to commit to the second GBA register of this sprite when it is regular (it does not have an attached sprite_affine_mat_ptr).
- **affine_second_attributes** `sprite_affine_second_attributes affine_second_attributes() const` (L909): Returns the attributes to commit to the second GBA register of this sprite when it is affine (it hash an attached sprite_affine_mat_ptr).
- **set_affine_second_attributes** `void set_affine_second_attributes(const sprite_affine_second_attributes& second_attributes)` (L915): Sets the attributes to commit to the second GBA register of this sprite when it is affine (it hash an attached sprite_affine_mat_ptr).
- **third_attributes** `sprite_third_attributes third_attributes() const` (L920): Returns the attributes to commit to the third GBA register of this sprite.
- **set_third_attributes** `void set_third_attributes(const sprite_third_attributes& third_attributes)` (L925): Sets the attributes to commit to the third GBA register of this sprite.
- **handle** `const void* handle() const` (L930): Returns the internal handle.
- **swap** `void swap(sprite_ptr& other)` (L939): Exchanges the contents of this sprite_ptr with those of the other one.
  `other` sprite_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_ptr& a, sprite_ptr& b)` (L949): Exchanges the contents of a sprite_ptr with those of another one.
  `a` First sprite_ptr to exchange the contents with.; `b` Second sprite_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sprite_ptr& a, const sprite_ptr& b) = default` (L957): Default equal operator.

## bn_sprite_regular_second_attributes.h

### `bn::sprite_regular_second_attributes` (class, bn_sprite_regular_second_attributes.h:27)

Manages the attributes to commit to the second GBA register of a regular sprite (without an attached sprite_affine_mat_ptr).

`class sprite_regular_second_attributes`

- **sprite_regular_second_attributes** `constexpr sprite_regular_second_attributes() = default` (L34): Default constructor.
- **sprite_regular_second_attributes** `constexpr sprite_regular_second_attributes(fixed x, bool horizontal_flip, bool vertical_flip)` (L42): Constructor.
  `x` Horizontal position of a sprite (relative to its camera, if it has one).; `horizontal_flip` Indicates if the sprite is flipped in the horizontal axis or not.; `vertical_flip` Indicates if the sprite is flipped in the vertical axis or not.
- **x** `constexpr fixed x() const` (L52): Returns the horizontal position of a sprite (relative to its camera, if it has one).
- **set_x** `constexpr void set_x(fixed x)` (L60): Sets the horizontal position of a sprite (relative to its camera, if it has one).
- **horizontal_flip** `constexpr bool horizontal_flip() const` (L68): Indicates if the sprite is flipped in the horizontal axis or not.
- **set_horizontal_flip** `constexpr void set_horizontal_flip(bool horizontal_flip)` (L76): Sets if the sprite is flipped in the horizontal axis or not.
- **vertical_flip** `constexpr bool vertical_flip() const` (L84): Indicates if the sprite is flipped in the vertical axis or not.
- **set_vertical_flip** `constexpr void set_vertical_flip(bool vertical_flip)` (L92): Sets if the sprite is flipped in the vertical axis or not.
- **operator==** `constexpr friend bool operator==(const sprite_regular_second_attributes& a, const sprite_regular_second_attributes& b) = default` (L100): Default equal operator.

## bn_sprite_shape_size.h

### `bn::sprite_shape` (enum, bn_sprite_shape_size.h:27)

Valid sprite shapes enum.

`enum class sprite_shape : uint8_t { SQUARE, WIDE, TALL }`

### `bn::sprite_size` (enum, bn_sprite_shape_size.h:40)

Valid sprite sizes enum.

`enum class sprite_size : uint8_t { SMALL, NORMAL, BIG, HUGE }`

### `bn::sprite_shape_size` (class, bn_sprite_shape_size.h:62)

Calculates sprite dimensions from its shape and size.

`class sprite_shape_size`

<table>
<caption> Available sprite pixel sizes </caption>
<tr><th> shape / size    <th> small  <th> normal <th> big    <th> huge
<tr><th> square          <td> 8x8    <td> 16x16  <td> 32x32  <td> 64x64
<tr><th> wide            <td> 16x8   <td> 32x8   <td> 32x16  <td> 64x32
<tr><th> tall            <td> 8x16   <td> 8x32   <td> 16x32  <td> 32x64
</table>

- **sprite_shape_size** `constexpr sprite_shape_size(sprite_shape shape, sprite_size size)` (L71): Constructor.
  `shape` sprite_shape enum.; `size` sprite_size enum.
- **sprite_shape_size** `constexpr sprite_shape_size(int width, int height)` (L82): Constructor.
  `width` Width of the sprite in pixels.; `height` Height of the sprite in pixels.
- **shape** `constexpr sprite_shape shape() const` (L196): Returns the sprite_shape enum.
- **size** `constexpr sprite_size size() const` (L204): Returns the sprite_size enum.
- **width** `constexpr int width() const` (L212): Returns the width of the sprite in pixels.
- **height** `constexpr int height() const` (L289): Returns the height of the sprite in pixels.
- **pixels_count** `constexpr int pixels_count() const` (L366): Returns the area of the sprite in pixels.
- **tiles_count** `constexpr int tiles_count(bpp_mode bpp) const` (L376): Returns the area of the sprite in tiles.
  `bpp` Tiles' bits per pixel mode.; devuelve: Area of the sprite in tiles.
- **operator==** `constexpr friend bool operator==(const sprite_shape_size& a, const sprite_shape_size& b) = default` (L384): Default equal operator.

## bn_sprite_third_attributes.h

### `bn::sprite_third_attributes` (class, bn_sprite_third_attributes.h:27)

Manages the attributes to commit to the third GBA register of a sprite.

`class sprite_third_attributes`

- **sprite_third_attributes** `sprite_third_attributes(sprite_tiles_ptr tiles, sprite_palette_ptr palette, int bg_priority)` (L40): Constructor.
  `tiles` Smart pointer to a sprite tile set.; `palette` Smart pointer to a sprite color palette.; `bg_priority` Drawing priority relative to backgrounds in the range [0..3].
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **tiles** `const sprite_tiles_ptr& tiles() const` (L45): Returns the smart pointer to a sprite tile set.
- **set_tiles** `void set_tiles(const sprite_tiles_ptr& tiles)` (L54): Sets the smart pointer to a sprite tile set.
  `tiles` Smart pointer to copy.
- **set_tiles** `void set_tiles(sprite_tiles_ptr&& tiles)` (L63): Sets the smart pointer to a sprite tile set.
  `tiles` Smart pointer to move.
- **palette** `const sprite_palette_ptr& palette() const` (L71): Returns the smart pointer to a sprite color palette.
- **set_palette** `void set_palette(const sprite_palette_ptr& palette)` (L80): Sets the smart pointer to a sprite color palette.
  `palette` Smart pointer to copy.
- **set_palette** `void set_palette(sprite_palette_ptr&& palette)` (L89): Sets the smart pointer to a sprite color palette.
  `palette` Smart pointer to move.
- **bg_priority** `int bg_priority() const` (L100): Returns the priority relative to backgrounds.
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **set_bg_priority** `void set_bg_priority(int bg_priority)` (L113): Sets the priority relative to backgrounds.
  `bg_priority` Priority relative to backgrounds in the range [0..3].
  Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
  Sprites cover backgrounds of the same priority.
- **operator==** `friend bool operator==(const sprite_third_attributes& a, const sprite_third_attributes& b) = default` (L118): Default equal operator.

## bn_sprites.h

### `bn::sprites` (namespace, bn_sprites.h:23)

Sprites related functions.

- **used_items_count** `int used_items_count()` (L31): Returns the number of used sprite items created with sprite_ptr static constructors.
  A sprite item which is outside of the screen or hidden is not committed to the GBA,
  so there can be more than 128 sprite items.
- **available_items_count** `int available_items_count()` (L39): Returns the number of available sprite items that can be created with sprite_ptr static constructors.
  A sprite item which is outside of the screen or hidden is not committed to the GBA,
  so there can be more than 128 sprite items.
- **visible** `bool visible()` (L76): Indicates if the sprite layer is visible or not.
- **set_visible** `void set_visible(bool visible)` (L81): Sets if the sprite layer is visible or not.
- **bg_sorting_enabled** `bool bg_sorting_enabled()` (L88): Indicates if sprites are sorted according to their priority relative to backgrounds or not.
  Disabling background sorting allows to create sprite masks.
- **set_bg_sorting_enabled** `void set_bg_sorting_enabled(bool bg_sorting_enabled)` (L97): Sets if sprites are sorted according to their priority relative to backgrounds or not.
  Disabling background sorting allows to create sprite masks.
  
  Background sorting can't be toggled with active sprites.
- **blending_bottom_enabled** `bool blending_bottom_enabled()` (L102): Indicates if the sprite layer is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L107): Sets if the sprite layer must be part of the blending bottom layer or not.
- **reserved_handles_count** `int reserved_handles_count()` (L114): Returns the number of hardware sprite handles not used by Butano sprites manager.
  Normally you should not need to reserve handles, but it can be useful for messing with HDMA for example.
- **set_reserved_handles_count** `void set_reserved_handles_count(int reserved_handles_count)` (L123): Sets the number of hardware sprite handles not used by Butano sprites manager.
  Normally you should not need to reserve handles, but it can be useful for messing with HDMA for example.
  
  Changing reserved handles count reloads the internal attributes of all sprites (including the reserved ones).
- **reload** `void reload()` (L130): Reloads the internal attributes of all sprites (including the reserved ones).
  Normally you should not need to call this function, but it can be useful after messing with HDMA for example.
