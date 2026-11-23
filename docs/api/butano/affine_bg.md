# Butano 21.10.1 — Affine backgrounds (`affine_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bg](bg.md)

## bn_affine_bg_attributes.h

### `bn::affine_bg_attributes` (class, bn_affine_bg_attributes.h:28)

Manages the attributes to commit to the GBA register of an affine background.

`class affine_bg_attributes`

- **affine_bg_attributes** `affine_bg_attributes(const affine_bg_map_ptr& map, int priority, bool wrapping_enabled, bool mosaic_enabled)` (L43): Constructor.
  `map` affine_bg_map_ptr of an affine background to copy.; `priority` Priority of an affine background relative to sprites and other backgrounds, in the range [0..3].; `wrapping_enabled` Indicates if an affine background wraps around at the edges or not.; `mosaic_enabled` Indicates if the mosaic effect is applied to an affine background or not.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **affine_bg_attributes** `affine_bg_attributes(const affine_bg_map_ptr& map, int priority, bool wrapping_enabled, bool mosaic_enabled, bn::green_swap_mode green_swap_mode)` (L57): Constructor.
  `map` affine_bg_map_ptr of an affine background to copy.; `priority` Priority of an affine background relative to sprites and other backgrounds, in the range [0..3].; `wrapping_enabled` Indicates if an affine background wraps around at the edges or not.; `mosaic_enabled` Indicates if the mosaic effect is applied to an affine background or not.; `green_swap_mode` Indicates how an affine background must be displayed when green swap is enabled.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **affine_bg_attributes** `affine_bg_attributes(affine_bg_map_ptr&& map, int priority, bool wrapping_enabled, bool mosaic_enabled)` (L71): Constructor.
  `map` affine_bg_map_ptr of an affine background to move.; `priority` Priority of an affine background relative to sprites and other backgrounds, in the range [0..3].; `wrapping_enabled` Indicates if an affine background wraps around at the edges or not.; `mosaic_enabled` Indicates if the mosaic effect is applied to an affine background or not.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **affine_bg_attributes** `affine_bg_attributes(affine_bg_map_ptr&& map, int priority, bool wrapping_enabled, bool mosaic_enabled, bn::green_swap_mode green_swap_mode)` (L85): Constructor.
  `map` affine_bg_map_ptr of an affine background to move.; `priority` Priority of an affine background relative to sprites and other backgrounds, in the range [0..3].; `wrapping_enabled` Indicates if an affine background wraps around at the edges or not.; `mosaic_enabled` Indicates if the mosaic effect is applied to an affine background or not.; `green_swap_mode` Indicates how an affine background must be displayed when green swap is enabled.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **map** `const affine_bg_map_ptr& map() const` (L91): Returns the affine_bg_map_ptr of an affine background.
- **set_map** `void set_map(const affine_bg_map_ptr& map)` (L100): Sets the affine_bg_map_ptr of an affine background.
  `map` affine_bg_map_ptr of an affine background to copy.
- **set_map** `void set_map(affine_bg_map_ptr&& map)` (L109): Sets the affine_bg_map_ptr of an affine background.
  `map` affine_bg_map_ptr of an affine background to move.
- **priority** `int priority() const` (L120): Returns the priority of an affine background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L133): Sets the priority of an affine background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **wrapping_enabled** `bool wrapping_enabled() const` (L138): Indicates if an affine background wraps around at the edges or not.
- **set_wrapping_enabled** `void set_wrapping_enabled(bool wrapping_enabled)` (L146): Sets if an affine background must wrap around at the edges or not.
- **mosaic_enabled** `bool mosaic_enabled() const` (L154): Indicates if the mosaic effect is applied to an affine background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L162): Sets if the mosaic effect must be applied to an affine background or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L170): Indicates how an affine background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L178): Sets how an affine background must be displayed when green swap is enabled.
- **operator==** `friend bool operator==(const affine_bg_attributes& a, const affine_bg_attributes& b) = default` (L186): Default equal operator.

## bn_affine_bg_builder.h

### `bn::affine_bg_builder` (class, bn_affine_bg_builder.h:32)

Creates affine_bg_ptr objects with custom attributes.

`class affine_bg_builder`

- **affine_bg_builder** `explicit affine_bg_builder(const affine_bg_item& item)` (L40): Constructor.
  `item` affine_bg_item containing the required information to generate affine backgrounds.
- **affine_bg_builder** `affine_bg_builder(const affine_bg_item& item, int map_index)` (L50): Constructor.
  `item` affine_bg_item containing the required information to generate affine backgrounds.; `map_index` Index of the map to reference in item.map_item() to generate affine backgrounds.
- **affine_bg_builder** `explicit affine_bg_builder(const affine_bg_map_ptr& map)` (L56): Constructor.
  `map` affine_bg_map_ptr to copy for generating affine backgrounds.
- **affine_bg_builder** `explicit affine_bg_builder(affine_bg_map_ptr&& map)` (L65): Constructor.
  `map` affine_bg_map_ptr to move for generating affine backgrounds.
- **item** `const optional<affine_bg_item>& item() const` (L74): Returns the affine_bg_item containing the required information to generate affine backgrounds if it has one; bn::nullopt otherwise.
- **map_index** `int map_index() const` (L82): Index of the map to reference in item.map_item() to generate affine backgrounds.
- **x** `fixed x() const` (L91): Returns the horizontal position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_x** `affine_bg_builder& set_x(fixed x)` (L102): Sets the horizontal position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the affine backgrounds to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L112): Returns the vertical position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_y** `affine_bg_builder& set_y(fixed y)` (L123): Sets the vertical position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `y` vertical position of the affine backgrounds to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L133): Returns the position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_position** `affine_bg_builder& set_position(fixed x, fixed y)` (L145): Sets the position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the affine backgrounds to generate.; `y` Vertical position of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_position** `affine_bg_builder& set_position(const fixed_point& position)` (L157): Sets the position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `position` Position of the affine backgrounds to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L167): Returns the horizontal top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `affine_bg_builder& set_top_left_x(fixed top_left_x)` (L175): Sets the horizontal top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the affine backgrounds to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L181): Returns the vertical top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `affine_bg_builder& set_top_left_y(fixed top_left_y)` (L189): Sets the vertical top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the affine backgrounds to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L195): Returns the top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `affine_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L204): Sets the top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the affine backgrounds to generate.; `top_left_y` Vertical top-left position of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_top_left_position** `affine_bg_builder& set_top_left_position(const fixed_point& top_left_position)` (L212): Sets the top-left position of the affine backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the affine backgrounds to generate.; devuelve: Reference to this.
- **rotation_angle** `fixed rotation_angle() const` (L217): Returns the rotation angle in degrees of the affine backgrounds to generate.
- **set_rotation_angle** `affine_bg_builder& set_rotation_angle(fixed rotation_angle)` (L227): Sets the rotation angle in degrees of the affine backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; devuelve: Reference to this.
- **set_rotation_angle_safe** `affine_bg_builder& set_rotation_angle_safe(fixed rotation_angle)` (L238): Sets the rotation angle in degrees of the affine backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in any range.; devuelve: Reference to this.
- **horizontal_scale** `fixed horizontal_scale() const` (L247): Returns the horizontal scale of the affine backgrounds to generate.
- **set_horizontal_scale** `affine_bg_builder& set_horizontal_scale(fixed horizontal_scale)` (L257): Sets the horizontal scale of the affine backgrounds to generate.
  `horizontal_scale` Horizontal scale of the affine backgrounds to generate.; devuelve: Reference to this.
- **vertical_scale** `fixed vertical_scale() const` (L266): Returns the vertical scale of the affine backgrounds to generate.
- **set_vertical_scale** `affine_bg_builder& set_vertical_scale(fixed vertical_scale)` (L276): Sets the vertical scale of the affine backgrounds to generate.
  `vertical_scale` Vertical scale of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `affine_bg_builder& set_scale(fixed scale)` (L287): Sets the scale of the affine backgrounds to generate.
  `scale` Scale of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `affine_bg_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)` (L299): Sets the scale of the affine backgrounds to generate.
  `horizontal_scale` Horizontal scale of the affine backgrounds to generate.; `vertical_scale` Vertical scale of the affine backgrounds to generate.; devuelve: Reference to this.
- **horizontal_shear** `fixed horizontal_shear() const` (L308): Returns the horizontal shear of the affine backgrounds to generate.
- **set_horizontal_shear** `affine_bg_builder& set_horizontal_shear(fixed horizontal_shear)` (L318): Sets the horizontal shear of the affine backgrounds to generate.
  `horizontal_shear` Horizontal shear of the affine backgrounds to generate.; devuelve: Reference to this.
- **vertical_shear** `fixed vertical_shear() const` (L327): Returns the vertical shear of the affine backgrounds to generate.
- **set_vertical_shear** `affine_bg_builder& set_vertical_shear(fixed vertical_shear)` (L337): Sets the vertical shear of the affine backgrounds to generate.
  `vertical_shear` Vertical shear of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `affine_bg_builder& set_shear(fixed shear)` (L348): Sets the shear of the affine backgrounds to generate.
  `shear` Shear of the affine backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `affine_bg_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)` (L360): Sets the shear of the affine backgrounds to generate.
  `horizontal_shear` Horizontal shear of the affine backgrounds to generate.; `vertical_shear` Vertical shear of the affine backgrounds to generate.; devuelve: Reference to this.
- **horizontal_flip** `bool horizontal_flip() const` (L369): Indicates if the affine backgrounds to generate are flipped in the horizontal axis or not.
- **set_horizontal_flip** `affine_bg_builder& set_horizontal_flip(bool horizontal_flip)` (L380): Sets if the affine backgrounds to generate must be flipped in the horizontal axis or not.
  `horizontal_flip` `true` if the affine backgrounds must be flipped in the horizontal axis; `false` otherwise.; devuelve: Reference to this.
- **vertical_flip** `bool vertical_flip() const` (L389): Indicates if the affine backgrounds to generate are flipped in the vertical axis or not.
- **set_vertical_flip** `affine_bg_builder& set_vertical_flip(bool vertical_flip)` (L400): Sets if the affine backgrounds to generate must be flipped in the vertical axis or not.
  `vertical_flip` `true` if the affine backgrounds must be flipped in the vertical axis; `false` otherwise.; devuelve: Reference to this.
- **pivot_x** `fixed pivot_x() const` (L410): Returns the horizontal position of the affine transformations pivot point of the affine backgrounds to generate.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L419): Sets the horizontal position of the affine transformations pivot point of the affine backgrounds to generate.
- **pivot_y** `fixed pivot_y() const` (L428): Returns the vertical position of the affine transformations pivot point of the affine backgrounds to generate.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L437): Sets the vertical position of the affine transformations pivot point of the affine backgrounds to generate.
- **pivot_position** `const fixed_point& pivot_position() const` (L446): Returns the position of the affine transformations pivot point of the affine backgrounds to generate.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L457): Sets the position of the affine transformations pivot point of the affine backgrounds to generate.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L468): Sets the position of the affine transformations pivot point of the affine backgrounds to generate.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_mat_attributes& mat_attributes() const` (L476): Returns the attributes of the affine transformation matrix of the affine backgrounds to generate.
- **set_affine_mat** `affine_bg_builder& set_affine_mat(const affine_mat_attributes& mat_attributes)` (L486): Sets the attributes of the affine transformation matrix of the affine backgrounds to generate.
  `mat_attributes` affine_mat_attributes of the affine backgrounds to generate.; devuelve: Reference to this.
- **priority** `int priority() const` (L498): Returns the priority of the affine backgrounds to generate relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `affine_bg_builder& set_priority(int priority)` (L512): Sets the priority of the affine backgrounds to generate relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].; devuelve: Reference to this.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **z_order** `int z_order() const` (L522): Returns the priority of the affine backgrounds to generate relative to other backgrounds, excluding sprites.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **set_z_order** `affine_bg_builder& set_z_order(int z_order)` (L538): Sets the priority of the affine backgrounds to generate relative to other backgrounds, excluding sprites.
  `z_order` Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].; devuelve: Reference to this.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **wrapping_enabled** `bool wrapping_enabled() const` (L543): Indicates if the affine backgrounds to generate wrap around at the edges or not.
- **set_wrapping_enabled** `affine_bg_builder& set_wrapping_enabled(bool wrapping_enabled)` (L553): Sets if the affine backgrounds to generate must wrap around at the edges or not.
  `wrapping_enabled` `true` if they must wrap around at the edges; `false` otherwise.; devuelve: Reference to this.
- **mosaic_enabled** `bool mosaic_enabled() const` (L562): Indicates if the mosaic effect must be applied to the affine backgrounds to generate or not.
- **set_mosaic_enabled** `affine_bg_builder& set_mosaic_enabled(bool mosaic_enabled)` (L572): Sets if the mosaic effect must be applied to the affine backgrounds to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L583): Indicates if blending must be applied to the affine backgrounds to generate or not.
  Blending is applied to affine backgrounds by making them part of the blending top layer.
- **set_blending_enabled** `affine_bg_builder& set_blending_enabled(bool blending_enabled)` (L595): Sets if blending must be applied to the affine backgrounds to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Blending is applied to affine backgrounds by making them part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L604): Indicates if the affine backgrounds to generate must be part of the blending top layer or not.
- **set_blending_top_enabled** `affine_bg_builder& set_blending_top_enabled(bool blending_top_enabled)` (L615): Sets if the affine backgrounds to generate must be part of the blending top layer or not.
  `blending_top_enabled` `true` if generated backgrounds must be part of the blending top layer; `false` otherwise.; devuelve: Reference to this.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L624): Indicates if the affine backgrounds to generate must be part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `affine_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)` (L635): Sets if the affine backgrounds to generate must be part of the blending bottom layer or not.
  `blending_bottom_enabled` `true` if generated backgrounds must be part of the blending bottom layer; `false` otherwise.; devuelve: Reference to this.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L644): Indicates how the affine backgrounds to generate must be displayed when green swap is enabled.
- **set_green_swap_mode** `affine_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L654): Sets how the affine backgrounds to generate must be displayed when green swap is enabled.
  `green_swap_mode` Green swap mode.; devuelve: Reference to this.
- **visible** `bool visible() const` (L663): Indicates if the affine backgrounds to generate must be committed to the GBA or not.
- **set_visible** `affine_bg_builder& set_visible(bool visible)` (L673): Sets if the affine backgrounds to generate must be committed to the GBA or not.
  `visible` `true` if the affine backgrounds must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L682): Returns the camera_ptr to attach to the affine backgrounds to generate (if any).
- **set_camera** `affine_bg_builder& set_camera(const camera_ptr& camera)` (L692): Sets the camera_ptr to attach to the affine backgrounds to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `affine_bg_builder& set_camera(camera_ptr&& camera)` (L703): Sets the camera_ptr to attach to the affine backgrounds to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `affine_bg_builder& set_camera(const optional<camera_ptr>& camera)` (L714): Sets or removes the camera_ptr to attach to the affine backgrounds to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `affine_bg_builder& set_camera(optional<camera_ptr>&& camera)` (L725): Sets or removes the camera_ptr to attach to the affine backgrounds to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `affine_bg_builder& remove_camera()` (L735): Removes the camera_ptr to attach to the affine backgrounds to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L744): Releases and returns the camera_ptr to attach to the affine backgrounds to generate (if any).
- **build** `affine_bg_ptr build() const` (L749): Generates and returns an affine_bg_ptr without releasing the acquired resources.
- **release_build** `affine_bg_ptr release_build()` (L756): Generates and returns an affine_bg_ptr releasing the acquired resources.
  affine_bg_ptr generation after calling this method may stop working.
- **build_optional** `optional<affine_bg_ptr> build_optional() const` (L762): Generates and returns an affine_bg_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<affine_bg_ptr> release_build_optional()` (L770): Generates and returns an affine_bg_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  affine_bg_ptr generation after calling this method may stop working.
- **map** `affine_bg_map_ptr map() const` (L775): Generates and returns an affine_bg_map_ptr without releasing the acquired resources.
- **map_optional** `optional<affine_bg_map_ptr> map_optional() const` (L781): Generates and returns an affine_bg_map_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_map** `affine_bg_map_ptr release_map()` (L788): Generates and returns an affine_bg_map_ptr releasing the acquired resources.
  affine_bg_ptr generation after calling this method may stop working.
- **release_map_optional** `optional<affine_bg_map_ptr> release_map_optional()` (L796): Generates and returns an affine_bg_map_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  affine_bg_ptr generation after calling this method may stop working.

## bn_affine_bg_ptr.h

### `bn::affine_bg_ptr` (class, bn_affine_bg_ptr.h:47)

std::shared_ptr like smart pointer that retains shared ownership of an affine background.

`class affine_bg_ptr`

Several affine_bg_ptr objects may own the same affine background.

The affine background is released when the last remaining affine_bg_ptr owning it is destroyed.

- **create** `static affine_bg_ptr create(const affine_bg_item& item)` (L56): Creates an affine_bg_ptr from the given affine_bg_item.
  `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(const affine_bg_item& item, int map_index)` (L64): Creates an affine_bg_ptr from the given affine_bg_item.
  `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(affine_bg_map_ptr map)` (L71): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(fixed x, fixed y, const affine_bg_item& item)` (L80): Creates an affine_bg_ptr from the given affine_bg_item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(fixed x, fixed y, const affine_bg_item& item, int map_index)` (L90): Creates an affine_bg_ptr from the given affine_bg_item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(const fixed_point& position, const affine_bg_item& item)` (L98): Creates an affine_bg_ptr from the given affine_bg_item.
  `position` Position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create( const fixed_point& position, const affine_bg_item& item, int map_index)` (L107): Creates an affine_bg_ptr from the given affine_bg_item.
  `position` Position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(fixed x, fixed y, affine_bg_map_ptr map)` (L117): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(const fixed_point& position, affine_bg_map_ptr map)` (L125): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `position` Position of the affine background.; `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(const affine_bg_builder& builder)` (L132): Creates an affine_bg_ptr from an affine_bg_builder reference.
  `builder` affine_bg_builder reference.; devuelve: The requested affine_bg_ptr.
- **create** `static affine_bg_ptr create(affine_bg_builder&& builder)` (L139): Creates an affine_bg_ptr from a moved affine_bg_builder.
  `builder` affine_bg_builder to move.; devuelve: The requested affine_bg_ptr.
- **create_optional** `static optional<affine_bg_ptr> create_optional(const affine_bg_item& item)` (L146): Creates an affine_bg_ptr from the given affine_bg_item.
  `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(const affine_bg_item& item, int map_index)` (L154): Creates an affine_bg_ptr from the given affine_bg_item.
  `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(affine_bg_map_ptr map)` (L161): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(fixed x, fixed y, const affine_bg_item& item)` (L170): Creates an affine_bg_ptr from the given affine_bg_item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional( fixed x, fixed y, const affine_bg_item& item, int map_index)` (L180): Creates an affine_bg_ptr from the given affine_bg_item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional( const fixed_point& position, const affine_bg_item& item)` (L189): Creates an affine_bg_ptr from the given affine_bg_item.
  `position` Position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional( const fixed_point& position, const affine_bg_item& item, int map_index)` (L199): Creates an affine_bg_ptr from the given affine_bg_item.
  `position` Position of the affine background.; `item` affine_bg_item containing the required information to generate the affine background.; `map_index` Index of the map to reference in item.map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(fixed x, fixed y, affine_bg_map_ptr map)` (L209): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional( const fixed_point& position, affine_bg_map_ptr map)` (L217): Creates an affine_bg_ptr from the given affine_bg_map_ptr.
  `position` Position of the affine background.; `map` affine_bg_map_ptr used by the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(const affine_bg_builder& builder)` (L225): Creates an affine_bg_ptr from an affine_bg_builder reference.
  `builder` affine_bg_builder reference.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<affine_bg_ptr> create_optional(affine_bg_builder&& builder)` (L232): Creates an affine_bg_ptr from a moved affine_bg_builder.
  `builder` affine_bg_builder to move.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **affine_bg_ptr** `affine_bg_ptr(const affine_bg_ptr& other)` (L238): Copy constructor.
  `other` affine_bg_ptr to copy.
- **operator=** `affine_bg_ptr& operator=(const affine_bg_ptr& other)` (L245): Copy assignment operator.
  `other` affine_bg_ptr to copy.; devuelve: Reference to this.
- **affine_bg_ptr** `affine_bg_ptr(affine_bg_ptr&& other) noexcept : affine_bg_ptr(other._handle)` (L251): Move constructor.
  `other` affine_bg_ptr to move.
- **operator=** `affine_bg_ptr& operator=(affine_bg_ptr&& other) noexcept` (L262): Move assignment operator.
  `other` affine_bg_ptr to move.; devuelve: Reference to this.
- **~affine_bg_ptr** `~affine_bg_ptr()` (L271): Releases the referenced affine background if no more affine_bg_ptr objects reference to it.
- **dimensions** `size dimensions() const` (L276): Returns the size in pixels of the affine background.
- **big** `bool big() const` (L283): Indicates if this affine background is big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **tiles** `const affine_bg_tiles_ptr& tiles() const` (L288): Returns the tiles used by this affine background.
- **set_tiles** `void set_tiles(const affine_bg_tiles_ptr& tiles)` (L296): Sets the tiles used by this affine background.
  `tiles` affine_bg_tiles_ptr to copy.
  It must be compatible with the current map of the affine background.
- **set_tiles** `void set_tiles(affine_bg_tiles_ptr&& tiles)` (L304): Sets the tiles used by this affine background.
  `tiles` affine_bg_tiles_ptr to move.
  It must be compatible with the current map of the affine background.
- **set_tiles** `void set_tiles(const affine_bg_tiles_item& tiles_item)` (L317): Replaces the tiles used by this affine background with a new tile set created with the given affine_bg_tiles_item.
  `tiles_item` It creates the new background tiles to use by this affine background.
  Before creating a new background tile set,
  the affine_bg_tiles_ptr used by this affine background is removed, so VRAM usage is reduced.
  
  The new background tiles must be compatible with the current map of the affine background.
- **palette** `const bg_palette_ptr& palette() const` (L322): Returns the color palette used by this affine background.
- **set_palette** `void set_palette(const bg_palette_ptr& palette)` (L328): Sets the color palette to use by this affine background.
  `palette` bg_palette_ptr to copy.
- **set_palette** `void set_palette(bg_palette_ptr&& palette)` (L334): Sets the color palette to use by this affine background.
  `palette` bg_palette_ptr to move.
- **set_palette** `void set_palette(const bg_palette_item& palette_item)` (L345): Replaces the color palette used by this affine background with a new one created with the given bg_palette_item.
  `palette_item` It creates the color palette to use by this affine background.
  Before creating a new color palette, the bg_palette_ptr used by this affine background is removed,
  so VRAM usage is reduced.
- **map** `const affine_bg_map_ptr& map() const` (L350): Returns the map used by this affine background.
- **set_map** `void set_map(const affine_bg_map_ptr& map)` (L356): Sets the map used by this affine background.
  `map` affine_bg_map_ptr to copy.
- **set_map** `void set_map(affine_bg_map_ptr&& map)` (L362): Sets the map used by this affine background.
  `map` affine_bg_map_ptr to move.
- **set_map** `void set_map(const affine_bg_map_item& map_item)` (L373): Replaces the map used by this affine background with a new map created with the given affine_bg_map_item.
  `map_item` It creates the new map to use by this affine background.
  Before creating a new map, the affine_bg_map_ptr used by this affine background is removed,
  so VRAM usage is reduced.
- **set_map** `void set_map(const affine_bg_map_item& map_item, int map_index)` (L385): Replaces the map used by this affine background with a new map created with the given affine_bg_map_item.
  `map_item` It creates the new map to use by this affine background.; `map_index` Index of the map to reference in map_item.
  Before creating a new map, the affine_bg_map_ptr used by this affine background is removed,
  so VRAM usage is reduced.
- **set_item** `void set_item(const affine_bg_item& item)` (L396): Replaces the tiles, the color palette and the map used by this affine background with the created with the given affine_bg_item.
  `item` It creates the resources to use by this affine background.
  Before creating new resources, the resources used by this affine background are removed,
  so VRAM usage is reduced.
- **set_item** `void set_item(const affine_bg_item& item, int map_index)` (L408): Replaces the tiles, the color palette and the map used by this affine background with the created with the given affine_bg_item.
  `item` It creates the resources to use by this affine background.; `map_index` Index of the map to reference in item.map_item().
  Before creating new resources, the resources used by this affine background are removed,
  so VRAM usage is reduced.
- **x** `fixed x() const` (L413): Returns the horizontal position of the affine background (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L418): Sets the horizontal position of the affine background (relative to its camera, if it has one).
- **y** `fixed y() const` (L423): Returns the vertical position of the affine background (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L428): Sets the vertical position of the affine background (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L433): Returns the position of the affine background (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L440): Sets the position of the affine background (relative to its camera, if it has one).
  `x` Horizontal position of the affine background (relative to its camera, if it has one).; `y` Vertical position of the affine background (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L445): Sets the position of the affine background (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L450): Returns the horizontal top-left position of the affine background (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L455): Sets the horizontal top-left position of the affine background (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L460): Returns the vertical top-left position of the affine background (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L465): Sets the vertical top-left position of the affine background (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L470): Returns the top-left position of the affine background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L477): Sets the top-left position of the affine background (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the affine background (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the affine background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L482): Sets the top-left position of the affine background (relative to its camera, if it has one).
- **rotation_angle** `fixed rotation_angle() const` (L487): Returns the rotation angle in degrees of the affine background.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L493): Sets the rotation angle in degrees of the affine background.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L499): Sets the rotation angle in degrees of the affine background.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `fixed horizontal_scale() const` (L504): Returns the horizontal scale of the affine background.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L509): Sets the horizontal scale of the affine background.
- **vertical_scale** `fixed vertical_scale() const` (L514): Returns the vertical scale of the affine background.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L519): Sets the vertical scale of the affine background.
- **set_scale** `void set_scale(fixed scale)` (L524): Sets the scale of the affine background.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L531): Sets the scale of the affine background.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **horizontal_shear** `fixed horizontal_shear() const` (L536): Returns the horizontal shear of the affine background.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L541): Sets the horizontal shear of the affine background.
- **vertical_shear** `fixed vertical_shear() const` (L546): Returns the vertical shear of the affine background.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L551): Sets the vertical shear of the affine background.
- **set_shear** `void set_shear(fixed shear)` (L556): Sets the shear of the affine background.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L563): Sets the shear of the affine background.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `bool horizontal_flip() const` (L568): Indicates if this affine background is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L573): Sets if this affine background must be flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L578): Indicates if this affine background must be flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L583): Sets if this affine background must be flipped in the vertical axis or not.
- **pivot_x** `fixed pivot_x() const` (L588): Returns the horizontal position of the affine transformations pivot point of this affine background.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L593): Sets the horizontal position of the affine transformations pivot point of this affine background.
- **pivot_y** `fixed pivot_y() const` (L598): Returns the vertical position of the affine transformations pivot point of this affine background.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L603): Sets the vertical position of the affine transformations pivot point of this affine background.
- **pivot_position** `const fixed_point& pivot_position() const` (L608): Returns the position of the affine transformations pivot point of this affine background.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L615): Sets the position of the affine transformations pivot point of this affine background.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L621): Sets the position of the affine transformations pivot point of this affine background.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_bg_mat_attributes& mat_attributes() const` (L626): Returns the attributes of the position and the transformation matrix of this affine background.
- **set_mat_attributes** `void set_mat_attributes(const affine_mat_attributes& mat_attributes)` (L631): Sets the attributes of the affine transformation matrix of this affine background.
- **priority** `int priority() const` (L639): Returns the priority of the affine background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L649): Sets the priority of the affine background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **z_order** `int z_order() const` (L658): Returns the priority of the affine background relative to other backgrounds, excluding sprites.
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **set_z_order** `void set_z_order(int z_order)` (L669): Sets the priority of the affine background relative to other backgrounds, excluding sprites.
  `z_order` Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
  Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
  
  Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
- **put_above** `void put_above()` (L674): Modify this affine background to be drawn above all of the other backgrounds with the same priority.
- **put_below** `void put_below()` (L679): Modify this affine background to be drawn below all of the other backgrounds with the same priority.
- **wrapping_enabled** `bool wrapping_enabled() const` (L684): Indicates if this affine background wraps around at the edges or not.
- **set_wrapping_enabled** `void set_wrapping_enabled(bool wrapping_enabled)` (L689): Sets if this affine background must wrap around at the edges or not.
- **mosaic_enabled** `bool mosaic_enabled() const` (L694): Indicates if the mosaic effect must be applied to this affine background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L699): Sets if the mosaic effect must be applied to this affine background or not.
- **blending_enabled** `bool blending_enabled() const` (L706): Indicates if blending must be applied to this affine background or not.
  Blending is applied to this affine background by making it part of the blending top layer.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L716): Sets if blending must be applied to this affine background or not.
  Blending is applied to this affine background by making it part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L724): Indicates if this affine background is part of the blending top layer or not.
- **set_blending_top_enabled** `void set_blending_top_enabled(bool blending_top_enabled)` (L729): Sets if this affine background is part of the blending top layer or not.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L734): Indicates if this affine background is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L739): Sets if this affine background is part of the blending bottom layer or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L744): Indicates how this affine background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L749): Sets how this affine background must be displayed when green swap is enabled.
- **visible** `bool visible() const` (L754): Indicates if this affine background must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L759): Sets if this affine background must be committed to the GBA or not.
- **visible_in_window** `bool visible_in_window(const window& window) const` (L764): Indicates if this affine background is visible in the given window or not.
- **set_visible_in_window** `void set_visible_in_window(bool visible, window& window)` (L769): Sets if this affine background must be visible in the given window or not.
- **camera** `const optional<camera_ptr>& camera() const` (L774): Returns the camera_ptr attached to this affine background (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L780): Sets the camera_ptr attached to this affine background.
  `camera` camera_ptr to copy to this affine background.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L786): Sets the camera_ptr attached to this affine background.
  `camera` camera_ptr to move to this affine background.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L792): Sets or removes the camera_ptr attached to this affine background.
  `camera` Optional camera_ptr to copy to this affine background.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L798): Sets or removes the camera_ptr attached to this affine background.
  `camera` Optional camera_ptr to move to this affine background.
- **remove_camera** `void remove_camera()` (L803): Removes the camera_ptr attached to this affine background (if any).
- **attributes** `affine_bg_attributes attributes() const` (L808): Returns the attributes to commit to the GBA for this affine background.
- **set_attributes** `void set_attributes(const affine_bg_attributes& attributes)` (L813): Sets the attributes to commit to the GBA for this affine background.
- **hw_id** `optional<int> hw_id() const` (L827): Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
  Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
  
  A background doesn't have an assigned hardware ID if it is not visible.
  
  Assigned hardware ID can change after calling this method
  if some properties of this background or others are updated.
  
  Call this method at your own risk.
- **handle** `const void* handle() const` (L832): Returns the internal handle.
- **swap** `void swap(affine_bg_ptr& other)` (L841): Exchanges the contents of this affine_bg_ptr with those of the other one.
  `other` affine_bg_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_ptr& a, affine_bg_ptr& b)` (L851): Exchanges the contents of an affine_bg_ptr with those of another one.
  `a` First affine_bg_ptr to exchange the contents with.; `b` Second affine_bg_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const affine_bg_ptr& a, const affine_bg_ptr& b) = default` (L859): Default equal operator.
