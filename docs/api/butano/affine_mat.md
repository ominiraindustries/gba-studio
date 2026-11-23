# Butano 21.10.1 — Affine transformation matrices (`affine_mat`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_affine_bg_mat_attributes.h

### `bn::affine_bg_mat_attributes` (class, bn_affine_bg_mat_attributes.h:31)

Contains the attributes which define the position and the transformation matrix of an affine background.

`class affine_bg_mat_attributes`

- **affine_bg_mat_attributes** `constexpr affine_bg_mat_attributes() = default` (L38): Default constructor.
- **affine_bg_mat_attributes** `constexpr affine_bg_mat_attributes( const fixed_point& position, const fixed_size& half_dimensions, const fixed_point& pivot_position, const affine_mat_attributes& mat_attributes)` (L47): Constructor.
  `position` Affine background absolute position (ignoring its camera).; `half_dimensions` Half of the affine background size.; `pivot_position` Position of the affine transformations pivot point.; `mat_attributes` Attributes which define the base affine transformation matrix.
- **x** `constexpr fixed x() const` (L61): Returns the horizontal absolute position of the affine background (ignoring its camera).
- **set_x** `constexpr void set_x(fixed x)` (L69): Sets the horizontal absolute position of the affine background (ignoring its camera).
- **y** `constexpr fixed y() const` (L78): Returns the absolute vertical position of the affine background (ignoring its camera).
- **set_y** `constexpr void set_y(fixed y)` (L86): Sets the absolute vertical position of the affine background (ignoring its camera).
- **position** `constexpr const fixed_point& position() const` (L95): Returns the absolute position of the affine background (ignoring its camera).
- **set_position** `constexpr void set_position(fixed x, fixed y)` (L105): Sets the absolute position of the affine background (ignoring its camera).
  `x` Horizontal absolute position of the affine background.; `y` Vertical absolute position of the affine background.
- **set_position** `constexpr void set_position(const fixed_point& position)` (L116): Sets the absolute position of the affine background (ignoring its camera).
  `position` Absolute position of the affine background.
- **top_left_x** `constexpr fixed top_left_x() const` (L125): Returns the horizontal absolute top-left position of the affine background (ignoring its camera).
- **set_top_left_x** `constexpr void set_top_left_x(fixed top_left_x)` (L133): Sets the horizontal absolute top-left position of the affine background (ignoring its camera).
- **top_left_y** `constexpr fixed top_left_y() const` (L141): Returns the absolute vertical top-left position of the affine background (ignoring its camera).
- **set_top_left_y** `constexpr void set_top_left_y(fixed top_left_y)` (L149): Sets the absolute vertical top-left position of the affine background (ignoring its camera).
- **top_left_position** `constexpr fixed_point top_left_position() const` (L157): Returns the absolute top-left position of the affine background (ignoring its camera).
- **set_top_left_position** `constexpr void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L167): Sets the absolute top-left position of the affine background (ignoring its camera).
  `top_left_x` Horizontal absolute top-left position of the affine background.; `top_left_y` Vertical absolute top-left position of the affine background.
- **set_top_left_position** `constexpr void set_top_left_position(const fixed_point& top_left_position)` (L176): Sets the absolute top-left position of the affine background (ignoring its camera).
  `top_left_position` Absolute top-left position of the affine background.
- **half_dimensions** `constexpr const fixed_size& half_dimensions() const` (L184): Returns the half of the affine background size.
- **set_half_dimensions** `constexpr void set_half_dimensions(const fixed_size& half_dimensions)` (L192): Sets the half of the affine background size.
- **pivot_x** `constexpr fixed pivot_x() const` (L201): Returns the horizontal position of the affine transformations pivot point.
- **set_pivot_x** `constexpr void set_pivot_x(fixed pivot_x)` (L209): Sets the horizontal position of the affine transformations pivot point.
- **pivot_y** `constexpr fixed pivot_y() const` (L218): Returns the vertical position of the affine transformations pivot point.
- **set_pivot_y** `constexpr void set_pivot_y(fixed pivot_y)` (L226): Sets the vertical position of the affine transformations pivot point.
- **pivot_position** `constexpr const fixed_point& pivot_position() const` (L235): Returns the position of the affine transformations pivot point.
- **set_pivot_position** `constexpr void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L245): Sets the position of the affine transformations pivot point.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `constexpr void set_pivot_position(const fixed_point& pivot_position)` (L256): Sets the position of the affine transformations pivot point.
  `pivot_position` Position of the pivot point.
- **rotation_angle** `constexpr fixed rotation_angle() const` (L265): Returns the rotation angle in degrees.
- **set_rotation_angle** `constexpr void set_rotation_angle(fixed rotation_angle)` (L274): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `constexpr void set_rotation_angle_safe(fixed rotation_angle)` (L284): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `constexpr fixed horizontal_scale() const` (L293): Returns the horizontal scale.
- **set_horizontal_scale** `constexpr void set_horizontal_scale(fixed horizontal_scale)` (L301): Sets the horizontal scale.
- **vertical_scale** `constexpr fixed vertical_scale() const` (L310): Returns the vertical scale.
- **set_vertical_scale** `constexpr void set_vertical_scale(fixed vertical_scale)` (L318): Sets the vertical scale.
- **set_scale** `constexpr void set_scale(fixed scale)` (L327): Sets the scale.
- **set_scale** `constexpr void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L338): Sets the scale.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **horizontal_shear** `constexpr fixed horizontal_shear() const` (L347): Returns the horizontal shear.
- **set_horizontal_shear** `constexpr void set_horizontal_shear(fixed horizontal_shear)` (L355): Sets the horizontal shear.
- **vertical_shear** `constexpr fixed vertical_shear() const` (L364): Returns the vertical shear.
- **set_vertical_shear** `constexpr void set_vertical_shear(fixed vertical_shear)` (L372): Sets the vertical shear.
- **set_shear** `constexpr void set_shear(fixed shear)` (L381): Sets the shear.
- **set_shear** `constexpr void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L392): Sets the shear.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `constexpr bool horizontal_flip() const` (L401): Indicates if this matrix is flipped in the horizontal axis or not.
- **set_horizontal_flip** `constexpr void set_horizontal_flip(bool horizontal_flip)` (L409): Sets if this matrix is flipped in the horizontal axis or not.
- **vertical_flip** `constexpr bool vertical_flip() const` (L418): Indicates if this matrix is flipped in the vertical axis or not.
- **set_vertical_flip** `constexpr void set_vertical_flip(bool vertical_flip)` (L426): Sets if this matrix is flipped in the vertical axis or not.
- **mat_attributes** `constexpr const affine_mat_attributes& mat_attributes() const` (L435): Returns the attributes which define the base affine transformation matrix.
- **set_mat_attributes** `constexpr void set_mat_attributes(const affine_mat_attributes& mat_attributes)` (L443): Sets the attributes which define the base affine transformation matrix.
- **pa_register_value** `constexpr int pa_register_value() const` (L453): Returns the value to commit to the first GBA register of an affine background transformation matrix with these attributes.
- **pb_register_value** `constexpr int pb_register_value() const` (L462): Returns the value to commit to the second GBA register of an affine background transformation matrix with these attributes.
- **pc_register_value** `constexpr int pc_register_value() const` (L471): Returns the value to commit to the third GBA register of an affine background transformation matrix with these attributes.
- **pd_register_value** `constexpr int pd_register_value() const` (L480): Returns the value to commit to the fourth GBA register of an affine background transformation matrix with these attributes.
- **dx_register_value** `constexpr int dx_register_value() const` (L489): Returns the value to commit to the fifth GBA register of an affine background transformation matrix with these attributes.
- **dy_register_value** `constexpr int dy_register_value() const` (L498): Returns the value to commit to the fifth GBA register of an affine background transformation matrix with these attributes.
- **operator==** `constexpr friend bool operator==(const affine_bg_mat_attributes& a, const affine_bg_mat_attributes& b)` (L509): Equal operator.
  `a` First affine_bg_mat_attributes to compare.; `b` Second affine_bg_mat_attributes to compare.; devuelve: `true` if the first affine_bg_mat_attributes is equal to the second one, otherwise `false`.

## bn_affine_mat_attributes.h

### `bn::affine_mat_attributes` (class, bn_affine_mat_attributes.h:26)

Contains the attributes which define an affine transformation matrix.

`class affine_mat_attributes`

- **affine_mat_attributes** `constexpr affine_mat_attributes() = default` (L33): Identity constructor.
- **affine_mat_attributes** `constexpr affine_mat_attributes(fixed rotation_angle, fixed horizontal_scale, fixed vertical_scale, bool horizontal_flip, bool vertical_flip) : affine_mat_attributes(rotation_angle, horizontal_scale, vertical_scale, 0, 0, horizontal_flip, vertical_flip)` (L43): Constructor.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.; `horizontal_flip` Indicates if this matrix is flipped in the horizontal axis or not.; `vertical_flip` Indicates if this matrix is flipped in the vertical axis or not.
- **affine_mat_attributes** `constexpr affine_mat_attributes(fixed rotation_angle, fixed horizontal_scale, fixed vertical_scale, fixed horizontal_shear, fixed vertical_shear, bool horizontal_flip, bool vertical_flip)` (L59): Constructor.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.; `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.; `horizontal_flip` Indicates if this matrix is flipped in the horizontal axis or not.; `vertical_flip` Indicates if this matrix is flipped in the vertical axis or not.
- **rotation_angle** `constexpr fixed rotation_angle() const` (L85): Returns the rotation angle in degrees.
- **set_rotation_angle** `constexpr void set_rotation_angle(fixed rotation_angle)` (L94): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `constexpr void set_rotation_angle_safe(fixed rotation_angle)` (L108): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `constexpr fixed horizontal_scale() const` (L116): Returns the horizontal scale.
- **set_horizontal_scale** `constexpr void set_horizontal_scale(fixed horizontal_scale)` (L124): Sets the horizontal scale.
- **vertical_scale** `constexpr fixed vertical_scale() const` (L137): Returns the vertical scale.
- **set_vertical_scale** `constexpr void set_vertical_scale(fixed vertical_scale)` (L145): Sets the vertical scale.
- **set_scale** `constexpr void set_scale(fixed scale)` (L158): Sets the scale.
- **set_scale** `constexpr void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L177): Sets the scale.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **horizontal_shear** `constexpr fixed horizontal_shear() const` (L195): Returns the horizontal shear.
- **set_horizontal_shear** `constexpr void set_horizontal_shear(fixed horizontal_shear)` (L203): Sets the horizontal shear.
- **vertical_shear** `constexpr fixed vertical_shear() const` (L212): Returns the vertical shear.
- **set_vertical_shear** `constexpr void set_vertical_shear(fixed vertical_shear)` (L220): Sets the vertical shear.
- **set_shear** `constexpr void set_shear(fixed shear)` (L229): Sets the shear.
- **set_shear** `constexpr void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L240): Sets the shear.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `constexpr bool horizontal_flip() const` (L249): Indicates if this matrix is flipped in the horizontal axis or not.
- **set_horizontal_flip** `constexpr void set_horizontal_flip(bool horizontal_flip)` (L257): Sets if this matrix is flipped in the horizontal axis or not.
- **vertical_flip** `constexpr bool vertical_flip() const` (L267): Indicates if this matrix is flipped in the vertical axis or not.
- **set_vertical_flip** `constexpr void set_vertical_flip(bool vertical_flip)` (L275): Sets if this matrix is flipped in the vertical axis or not.
- **identity** `constexpr bool identity() const` (L285): Indicates if this matrix is equal to the identity matrix or not.
- **flipped_identity** `constexpr bool flipped_identity() const` (L295): Indicates if this matrix is equal to the identity matrix or not, ignoring flip attributes.
- **pa_register_value** `constexpr int pa_register_value() const` (L306): Returns the value to commit to the first GBA register of an affine transformation matrix with these attributes.
- **pb_register_value** `constexpr int pb_register_value() const` (L315): Returns the value to commit to the second GBA register of an affine transformation matrix with these attributes.
- **pc_register_value** `constexpr int pc_register_value() const` (L324): Returns the value to commit to the third GBA register of an affine transformation matrix with these attributes.
- **pd_register_value** `constexpr int pd_register_value() const` (L333): Returns the value to commit to the fourth GBA register of an affine transformation matrix with these attributes.
- **unsafe_set_register_values** `constexpr void unsafe_set_register_values(int pa, int pb, int pc, int pd)` (L344): UNSAFE: sets the values to commit to the GBA registers of an affine transformation matrix bypassing the rest of the parameters.
  The rest of the parameters are not updated, so the affine transformation matrix ends in an inconsistent state.
- **operator==** `constexpr friend bool operator==(const affine_mat_attributes& a, const affine_mat_attributes& b)` (L358): Equal operator.
  `a` First affine_mat_attributes to compare.; `b` Second affine_mat_attributes to compare.; devuelve: `true` if the first affine_mat_attributes is equal to the second one, otherwise `false`.

## bn_sprite_affine_mat_ptr.h

### `bn::sprite_affine_mat_ptr` (class, bn_sprite_affine_mat_ptr.h:36)

std::shared_ptr like smart pointer that retains shared ownership of a sprite affine transformation matrix.

`class sprite_affine_mat_ptr`

Several sprite_affine_mat_ptr objects may own the same sprite affine transformation matrix.

The sprite affine transformation matrix is released
when the last remaining sprite_affine_mat_ptr owning it is destroyed.

- **create** `static sprite_affine_mat_ptr create()` (L44): Creates an identity affine transformation matrix.
  devuelve: The requested sprite_affine_mat_ptr.
- **create** `static sprite_affine_mat_ptr create(const affine_mat_attributes& attributes)` (L51): Creates an affine transformation matrix with the specified attributes.
  `attributes` affine_mat_attributes of the output matrix.; devuelve: The requested sprite_affine_mat_ptr.
- **create_optional** `static optional<sprite_affine_mat_ptr> create_optional()` (L57): Creates an identity affine transformation matrix.
  devuelve: The requested sprite_affine_mat_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sprite_affine_mat_ptr> create_optional(const affine_mat_attributes& attributes)` (L64): Creates an affine transformation matrix with the specified attributes.
  `attributes` affine_mat_attributes of the output matrix.; devuelve: The requested sprite_affine_mat_ptr if it could be allocated; bn::nullopt otherwise.
- **sprite_affine_mat_ptr** `sprite_affine_mat_ptr(const sprite_affine_mat_ptr& other)` (L70): Copy constructor.
  `other` sprite_affine_mat_ptr to copy.
- **operator=** `sprite_affine_mat_ptr& operator=(const sprite_affine_mat_ptr& other)` (L77): Copy assignment operator.
  `other` sprite_affine_mat_ptr to copy.; devuelve: Reference to this.
- **sprite_affine_mat_ptr** `sprite_affine_mat_ptr(sprite_affine_mat_ptr&& other) noexcept : sprite_affine_mat_ptr(other._id)` (L83): Move constructor.
  `other` sprite_affine_mat_ptr to move.
- **operator=** `sprite_affine_mat_ptr& operator=(sprite_affine_mat_ptr&& other) noexcept` (L94): Move assignment operator.
  `other` sprite_affine_mat_ptr to move.; devuelve: Reference to this.
- **~sprite_affine_mat_ptr** `~sprite_affine_mat_ptr()` (L104): Releases the referenced sprite affine transformation matrix if no more sprite_affine_mat_ptr objects reference to it.
- **id** `int id() const` (L109): Returns the internal id.
- **rotation_angle** `fixed rotation_angle() const` (L117): Returns the rotation angle in degrees.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L123): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L129): Sets the rotation angle in degrees.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `fixed horizontal_scale() const` (L134): Returns the horizontal scale.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L139): Sets the horizontal scale.
- **vertical_scale** `fixed vertical_scale() const` (L144): Returns the vertical scale.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L149): Sets the vertical scale.
- **set_scale** `void set_scale(fixed scale)` (L154): Sets the scale.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L161): Sets the scale.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **horizontal_shear** `fixed horizontal_shear() const` (L166): Returns the horizontal shear.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L171): Sets the horizontal shear.
- **vertical_shear** `fixed vertical_shear() const` (L176): Returns the vertical shear.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L181): Sets the vertical shear.
- **set_shear** `void set_shear(fixed shear)` (L186): Sets the shear.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L193): Sets the shear.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `bool horizontal_flip() const` (L198): Indicates if this matrix is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L203): Sets if this matrix is flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L208): Indicates if this matrix is flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L213): Sets if this matrix is flipped in the vertical axis or not.
- **attributes** `const affine_mat_attributes& attributes() const` (L218): Returns the attributes of this matrix.
- **set_attributes** `void set_attributes(const affine_mat_attributes& attributes)` (L223): Sets the attributes of this matrix.
- **identity** `bool identity() const` (L228): Indicates if this matrix is equal to the identity matrix or not.
- **flipped_identity** `bool flipped_identity() const` (L233): Indicates if this matrix is equal to the identity matrix or not, ignoring flip attributes.
- **swap** `void swap(sprite_affine_mat_ptr& other)` (L239): Exchanges the contents of this sprite_affine_mat_ptr with those of the other one.
  `other` sprite_affine_mat_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_ptr& a, sprite_affine_mat_ptr& b)` (L249): Exchanges the contents of a sprite_affine_mat_ptr with those of another one.
  `a` First sprite_affine_mat_ptr to exchange the contents with.; `b` Second sprite_affine_mat_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sprite_affine_mat_ptr& a, const sprite_affine_mat_ptr& b) = default` (L257): Default equal operator.

## bn_sprite_affine_mats.h

### `bn::sprite_affine_mats` (namespace, bn_sprite_affine_mats.h:25)

Sprite affine transformation matrices related functions.

- **used_count** `int used_count()` (L31): Returns the number of used sprite affine transformation matrices managed with sprite_affine_mat_ptr objects.
- **available_count** `int available_count()` (L37): Returns the number of available sprite affine transformation matrices that can be managed with sprite_affine_mat_ptr objects.
