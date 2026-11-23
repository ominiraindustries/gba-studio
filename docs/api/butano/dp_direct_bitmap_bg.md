# Butano 21.10.1 — Dual page direct bitmap backgrounds (`dp_direct_bitmap_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [direct_bitmap_bg](direct_bitmap_bg.md)

## bn_dp_direct_bitmap_bg_builder.h

### `bn::dp_direct_bitmap_bg_builder` (class, bn_dp_direct_bitmap_bg_builder.h:30)

Creates dp_direct_bitmap_bg_ptr objects with custom attributes.

`class dp_direct_bitmap_bg_builder`

- **x** `fixed x() const` (L38): Returns the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_x** `dp_direct_bitmap_bg_builder& set_x(fixed x)` (L49): Sets the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L59): Returns the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_y** `dp_direct_bitmap_bg_builder& set_y(fixed y)` (L70): Sets the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `y` vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L80): Returns the position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_position** `dp_direct_bitmap_bg_builder& set_position(fixed x, fixed y)` (L92): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; `y` Vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **set_position** `dp_direct_bitmap_bg_builder& set_position(const fixed_point& position)` (L104): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `position` Position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L114): Returns the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `dp_direct_bitmap_bg_builder& set_top_left_x(fixed top_left_x)` (L122): Sets the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L128): Returns the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `dp_direct_bitmap_bg_builder& set_top_left_y(fixed top_left_y)` (L136): Sets the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L142): Returns the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `dp_direct_bitmap_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L151): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **set_top_left_position** `dp_direct_bitmap_bg_builder& set_top_left_position(const fixed_point& top_left_position)` (L159): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **rotation_angle** `fixed rotation_angle() const` (L164): Returns the rotation angle in degrees of the backgrounds to generate.
- **set_rotation_angle** `dp_direct_bitmap_bg_builder& set_rotation_angle(fixed rotation_angle)` (L174): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; devuelve: Reference to this.
- **set_rotation_angle_safe** `dp_direct_bitmap_bg_builder& set_rotation_angle_safe(fixed rotation_angle)` (L185): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in any range.; devuelve: Reference to this.
- **horizontal_scale** `fixed horizontal_scale() const` (L194): Returns the horizontal scale of the backgrounds to generate.
- **set_horizontal_scale** `dp_direct_bitmap_bg_builder& set_horizontal_scale(fixed horizontal_scale)` (L204): Sets the horizontal scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_scale** `fixed vertical_scale() const` (L213): Returns the vertical scale of the backgrounds to generate.
- **set_vertical_scale** `dp_direct_bitmap_bg_builder& set_vertical_scale(fixed vertical_scale)` (L223): Sets the vertical scale of the backgrounds to generate.
  `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `dp_direct_bitmap_bg_builder& set_scale(fixed scale)` (L234): Sets the scale of the backgrounds to generate.
  `scale` Scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `dp_direct_bitmap_bg_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)` (L246): Sets the scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **expand** `dp_direct_bitmap_bg_builder& expand()` (L256): Sets the scale of the backgrounds to generate to fill the screen.
  devuelve: Reference to this.
- **horizontal_shear** `fixed horizontal_shear() const` (L261): Returns the horizontal shear of the backgrounds to generate.
- **set_horizontal_shear** `dp_direct_bitmap_bg_builder& set_horizontal_shear(fixed horizontal_shear)` (L271): Sets the horizontal shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_shear** `fixed vertical_shear() const` (L280): Returns the vertical shear of the backgrounds to generate.
- **set_vertical_shear** `dp_direct_bitmap_bg_builder& set_vertical_shear(fixed vertical_shear)` (L290): Sets the vertical shear of the backgrounds to generate.
  `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `dp_direct_bitmap_bg_builder& set_shear(fixed shear)` (L301): Sets the shear of the backgrounds to generate.
  `shear` Shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `dp_direct_bitmap_bg_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)` (L313): Sets the shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **horizontal_flip** `bool horizontal_flip() const` (L322): Indicates if the backgrounds to generate are flipped in the horizontal axis or not.
- **set_horizontal_flip** `dp_direct_bitmap_bg_builder& set_horizontal_flip(bool horizontal_flip)` (L332): Sets if the backgrounds to generate must be flipped in the horizontal axis or not.
  `horizontal_flip` `true` if the backgrounds must be flipped in the horizontal axis; `false` otherwise.; devuelve: Reference to this.
- **vertical_flip** `bool vertical_flip() const` (L341): Indicates if the backgrounds to generate are flipped in the vertical axis or not.
- **set_vertical_flip** `dp_direct_bitmap_bg_builder& set_vertical_flip(bool vertical_flip)` (L351): Sets if the backgrounds to generate must be flipped in the vertical axis or not.
  `vertical_flip` `true` if the backgrounds must be flipped in the vertical axis; `false` otherwise.; devuelve: Reference to this.
- **pivot_x** `fixed pivot_x() const` (L360): Returns the horizontal position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L368): Sets the horizontal position of the affine transformations pivot point of the backgrounds to generate.
- **pivot_y** `fixed pivot_y() const` (L376): Returns the vertical position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L384): Sets the vertical position of the affine transformations pivot point of the backgrounds to generate.
- **pivot_position** `const fixed_point& pivot_position() const` (L392): Returns the position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L402): Sets the position of the affine transformations pivot point of the backgrounds to generate.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L412): Sets the position of the affine transformations pivot point of the backgrounds to generate.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_mat_attributes& mat_attributes() const` (L420): Returns the attributes of the affine transformation matrix of the backgrounds to generate.
- **set_affine_mat** `dp_direct_bitmap_bg_builder& set_affine_mat(const affine_mat_attributes& mat_attributes)` (L430): Sets the attributes of the affine transformation matrix of the backgrounds to generate.
  `mat_attributes` affine_mat_attributes of the backgrounds to generate.; devuelve: Reference to this.
- **priority** `int priority() const` (L442): Returns the priority of the backgrounds to generate relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `dp_direct_bitmap_bg_builder& set_priority(int priority)` (L456): Sets the priority of the backgrounds to generate relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].; devuelve: Reference to this.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L461): Indicates if the mosaic effect must be applied to the backgrounds to generate or not.
- **set_mosaic_enabled** `dp_direct_bitmap_bg_builder& set_mosaic_enabled(bool mosaic_enabled)` (L471): Sets if the mosaic effect must be applied to the backgrounds to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L482): Indicates if blending must be applied to the backgrounds to generate or not.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **set_blending_enabled** `dp_direct_bitmap_bg_builder& set_blending_enabled(bool blending_enabled)` (L494): Sets if blending must be applied to the backgrounds to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L503): Indicates if the backgrounds to generate must be part of the blending top layer or not.
- **set_blending_top_enabled** `dp_direct_bitmap_bg_builder& set_blending_top_enabled(bool blending_top_enabled)` (L514): Sets if the backgrounds to generate must be part of the blending top layer or not.
  `blending_top_enabled` `true` if generated backgrounds must be part of the blending top layer; `false` otherwise.; devuelve: Reference to this.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L523): Indicates if the backgrounds to generate must be part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `dp_direct_bitmap_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)` (L534): Sets if the backgrounds to generate must be part of the blending bottom layer or not.
  `blending_bottom_enabled` `true` if generated backgrounds must be part of the blending bottom layer; `false` otherwise.; devuelve: Reference to this.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L543): Indicates how the backgrounds to generate must be displayed when green swap is enabled.
- **set_green_swap_mode** `dp_direct_bitmap_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L553): Sets how the backgrounds to generate must be displayed when green swap is enabled.
  `green_swap_mode` Green swap mode.; devuelve: Reference to this.
- **visible** `bool visible() const` (L562): Indicates if the backgrounds to generate must be committed to the GBA or not.
- **set_visible** `dp_direct_bitmap_bg_builder& set_visible(bool visible)` (L572): Sets if the backgrounds to generate must be committed to the GBA or not.
  `visible` `true` if the backgrounds must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L581): Returns the camera_ptr to attach to the backgrounds to generate (if any).
- **set_camera** `dp_direct_bitmap_bg_builder& set_camera(const camera_ptr& camera)` (L591): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `dp_direct_bitmap_bg_builder& set_camera(camera_ptr&& camera)` (L602): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `dp_direct_bitmap_bg_builder& set_camera(const optional<camera_ptr>& camera)` (L613): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `dp_direct_bitmap_bg_builder& set_camera(optional<camera_ptr>&& camera)` (L624): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `dp_direct_bitmap_bg_builder& remove_camera()` (L634): Removes the camera_ptr to attach to the backgrounds to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L643): Releases and returns the camera_ptr to attach to the backgrounds to generate (if any).
- **build** `dp_direct_bitmap_bg_ptr build() const` (L648): Generates and returns a dp_direct_bitmap_bg_ptr without releasing the acquired resources.
- **release_build** `dp_direct_bitmap_bg_ptr release_build()` (L655): Generates and returns a dp_direct_bitmap_bg_ptr releasing the acquired resources.
  dp_direct_bitmap_bg_ptr generation after calling this method may stop working.
- **build_optional** `optional<dp_direct_bitmap_bg_ptr> build_optional() const` (L661): Generates and returns a dp_direct_bitmap_bg_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<dp_direct_bitmap_bg_ptr> release_build_optional()` (L669): Generates and returns a dp_direct_bitmap_bg_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  dp_direct_bitmap_bg_ptr generation after calling this method may stop working.

## bn_dp_direct_bitmap_bg_painter.h

### `bn::dp_direct_bitmap_bg_painter` (class, bn_dp_direct_bitmap_bg_painter.h:33)

Performs low-level painting on dual page direct bitmap backgrounds.

`class dp_direct_bitmap_bg_painter`

Only one bitmap painter can be active at the same time.

- **dp_direct_bitmap_bg_painter** `explicit dp_direct_bitmap_bg_painter(const dp_direct_bitmap_bg_ptr& bg)` (L41): Class constructor.
  `bg` Dual page direct bitmap background to be modified.
- **dp_direct_bitmap_bg_painter** `explicit dp_direct_bitmap_bg_painter(dp_direct_bitmap_bg_ptr&& bg)` (L47): Class constructor.
  `bg` Dual page direct bitmap background to be modified.
- **dp_direct_bitmap_bg_painter** `dp_direct_bitmap_bg_painter(dp_direct_bitmap_bg_painter&& other) noexcept` (L57): Move constructor.
  `other` dp_direct_bitmap_bg_painter to move.
- **operator=** `dp_direct_bitmap_bg_painter& operator=(dp_direct_bitmap_bg_painter&& other) noexcept` (L64): Move assignment operator.
  `other` dp_direct_bitmap_bg_painter to move.; devuelve: Reference to this.
- **~dp_direct_bitmap_bg_painter** `~dp_direct_bitmap_bg_painter()` (L69): Class destructor.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L74): Returns the background modified by this painter.
- **page** `span<const color> page() const` (L82): Returns a span of the hidden frame buffer.
- **page** `span<color> page()` (L91): Returns a span of the hidden frame buffer.
- **flip_page_later** `void flip_page_later()` (L100): Swaps the hidden frame buffer in the next core::update call.
- **flip_page_now** `void flip_page_now()` (L110): Swaps the hidden frame buffer without waiting to the next core::update call.
  Expect lack of vsync issues like screen tearing.
- **fill** `inline void fill(color color)` (L118): Fills the current page with the given color.
- **unsafe_get** `inline color unsafe_get(int x, int y) const` (L129): Returns the referenced color without bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::dp_direct_width()).; `y` Vertical position in the current page [0..bitmap_bg::dp_direct_height()).; devuelve: The referenced color.
- **unsafe_get** `inline color unsafe_get(const point& position) const` (L139): Returns the referenced color without bounds checking.
  `position` Position in the current page.; devuelve: The referenced color.
- **get** `inline optional<color> get(int x, int y) const` (L150): Returns the referenced color with bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::dp_direct_width()).; `y` Vertical position in the current page [0..bitmap_bg::dp_direct_height()).; devuelve: The referenced color.
- **get** `inline optional<color> get(const point& position) const` (L167): Returns the referenced color with bounds checking.
  `position` Position in the current page.; devuelve: The referenced color.
- **unsafe_plot** `inline void unsafe_plot(int x, int y, color color)` (L178): Plots a pixel on the current page without bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **unsafe_plot** `inline void unsafe_plot(const point& position, color color)` (L188): Plots a pixel on the current page without bounds checking.
  `position` Position of the pixel to plot.; `color` Color to plot.
- **plot** `inline void plot(int x, int y, color color)` (L199): Plots a pixel on the current page with bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **plot** `inline void plot(const point& position, color color)` (L212): Plots a pixel on the current page with bounds checking.
  `position` Position of the pixel to plot.; `color` Color to plot.
- **unsafe_horizontal_line** `inline void unsafe_horizontal_line(int x1, int x2, int y, color color)` (L224): Draws an horizontal line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **horizontal_line** `inline void horizontal_line(int x1, int x2, int y, color color)` (L242): Draws an horizontal line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **unsafe_vertical_line** `inline void unsafe_vertical_line(int x, int y1, int y2, color color)` (L264): Draws a vertical line in the current page without bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **vertical_line** `inline void vertical_line(int x, int y1, int y2, color color)` (L288): Draws a vertical line in the current page with bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **unsafe_line** `inline void unsafe_line(int x1, int y1, int x2, int y2, color color)` (L311): Draws a line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **unsafe_line** `inline void unsafe_line(const point& first, const point& last, color color)` (L401): Draws a line in the current page without bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color` Color to plot.
- **line** `inline void line(int x1, int y1, int x2, int y2, color color)` (L414): Draws a line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **line** `inline void line(const point& first, const point& last, color color)` (L431): Draws a line in the current page with bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color` Color to plot.
- **unsafe_rectangle** `inline void unsafe_rectangle(int x1, int y1, int x2, int y2, color color)` (L444): Draws a filled rectangle in the current page without bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::dp_direct_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::dp_direct_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **unsafe_rectangle** `inline void unsafe_rectangle(const point& first, const point& last, color color)` (L475): Draws a filled rectangle in the current page without bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color` Color to plot.
- **rectangle** `inline void rectangle(int x1, int y1, int x2, int y2, color color)` (L488): Draws a filled rectangle in the current page with bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::dp_direct_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::dp_direct_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::dp_direct_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::dp_direct_height()).; `color` Color to plot.
- **rectangle** `inline void rectangle(const point& first, const point& last, color color)` (L510): Draws a filled rectangle in the current page with bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color` Color to plot.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const direct_bitmap_item& item)` (L522): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const direct_bitmap_item& item)` (L535): Copies the given item to the current page without bounds checking. Transparent pixels are also , so it should be faster than dp_direct_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const direct_bitmap_roi& roi)` (L547): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `roi` Region of interest to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const direct_bitmap_roi& roi)` (L561): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(int x, int y, const direct_bitmap_item& item)` (L573): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `item` Item to copy to the current page.
- **blit** `inline void blit(const point& position, const direct_bitmap_item& item)` (L594): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **blit** `inline void blit(int x, int y, const direct_bitmap_roi& roi)` (L606): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(const point& position, const direct_bitmap_roi& roi)` (L628): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than dp_direct_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)` (L641): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)` (L655): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)` (L668): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)` (L683): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)` (L696): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)` (L718): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)` (L731): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::dp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::dp_direct_height()).; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)` (L754): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than dp_direct_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.

## bn_dp_direct_bitmap_bg_ptr.h

### `bn::dp_direct_bitmap_bg_ptr` (class, bn_dp_direct_bitmap_bg_ptr.h:42)

std::shared_ptr like smart pointer that retains shared ownership of a dual page direct bitmap background.

`class dp_direct_bitmap_bg_ptr`

Several dp_direct_bitmap_bg_ptr objects may own the same background.

The background is released when the last remaining dp_direct_bitmap_bg_ptr owning it is destroyed.

- **dimensions** `static size dimensions()` (L49): Returns the size in pixels of the backgrounds.
- **create** `static dp_direct_bitmap_bg_ptr create()` (L54): Creates a dp_direct_bitmap_bg_ptr.
- **create** `static dp_direct_bitmap_bg_ptr create(const dp_direct_bitmap_bg_builder& builder)` (L61): Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
  `builder` dp_direct_bitmap_bg_builder reference.; devuelve: The requested dp_direct_bitmap_bg_ptr.
- **create** `static dp_direct_bitmap_bg_ptr create(dp_direct_bitmap_bg_builder&& builder)` (L68): Creates a dp_direct_bitmap_bg_ptr from a moved dp_direct_bitmap_bg_builder.
  `builder` dp_direct_bitmap_bg_builder to move.; devuelve: The requested dp_direct_bitmap_bg_ptr.
- **create_optional** `static optional<dp_direct_bitmap_bg_ptr> create_optional()` (L73): Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
- **create_optional** `static optional<dp_direct_bitmap_bg_ptr> create_optional(const dp_direct_bitmap_bg_builder& builder)` (L80): Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
  `builder` dp_direct_bitmap_bg_builder reference.; devuelve: The requested dp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<dp_direct_bitmap_bg_ptr> create_optional(dp_direct_bitmap_bg_builder&& builder)` (L87): Creates a dp_direct_bitmap_bg_ptr from a moved dp_direct_bitmap_bg_builder.
  `builder` dp_direct_bitmap_bg_builder to move.; devuelve: The requested dp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **dp_direct_bitmap_bg_ptr** `dp_direct_bitmap_bg_ptr(const dp_direct_bitmap_bg_ptr& other)` (L93): Copy constructor.
  `other` dp_direct_bitmap_bg_ptr to copy.
- **operator=** `dp_direct_bitmap_bg_ptr& operator=(const dp_direct_bitmap_bg_ptr& other)` (L100): Copy assignment operator.
  `other` dp_direct_bitmap_bg_ptr to copy.; devuelve: Reference to this.
- **dp_direct_bitmap_bg_ptr** `dp_direct_bitmap_bg_ptr(dp_direct_bitmap_bg_ptr&& other) noexcept : dp_direct_bitmap_bg_ptr(other._handle)` (L106): Move constructor.
  `other` dp_direct_bitmap_bg_ptr to move.
- **operator=** `dp_direct_bitmap_bg_ptr& operator=(dp_direct_bitmap_bg_ptr&& other) noexcept` (L117): Move assignment operator.
  `other` dp_direct_bitmap_bg_ptr to move.; devuelve: Reference to this.
- **~dp_direct_bitmap_bg_ptr** `~dp_direct_bitmap_bg_ptr()` (L126): Releases the referenced background if no more dp_direct_bitmap_bg_ptr objects reference to it.
- **page** `span<const color> page() const` (L131): Returns a span of the hidden frame buffer.
- **page** `span<color> page()` (L136): Returns a span of the hidden frame buffer.
- **flip_page_later** `void flip_page_later()` (L141): Swaps the hidden frame buffer in the next core::update call.
- **flip_page_now** `void flip_page_now()` (L148): Swaps the hidden frame buffer without waiting to the next core::update call.
  Expect lack of vsync issues like screen tearing.
- **x** `fixed x() const` (L153): Returns the horizontal position of the background (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L158): Sets the horizontal position of the background (relative to its camera, if it has one).
- **y** `fixed y() const` (L163): Returns the vertical position of the background (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L168): Sets the vertical position of the background (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L173): Returns the position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L180): Sets the position of the background (relative to its camera, if it has one).
  `x` Horizontal position of the background (relative to its camera, if it has one).; `y` Vertical position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L185): Sets the position of the background (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L190): Returns the horizontal top-left position of the background (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L195): Sets the horizontal top-left position of the background (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L200): Returns the vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L205): Sets the vertical top-left position of the background (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L210): Returns the top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L217): Sets the top-left position of the background (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the background (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L222): Sets the top-left position of the background (relative to its camera, if it has one).
- **rotation_angle** `fixed rotation_angle() const` (L227): Returns the rotation angle in degrees of the background.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L233): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L239): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `fixed horizontal_scale() const` (L244): Returns the horizontal scale of the background.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L249): Sets the horizontal scale of the background.
- **vertical_scale** `fixed vertical_scale() const` (L254): Returns the vertical scale of the background.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L259): Sets the vertical scale of the background.
- **set_scale** `void set_scale(fixed scale)` (L264): Sets the scale of the background.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L271): Sets the scale of the background.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **expand** `void expand()` (L276): Scales the background to fill the screen.
- **horizontal_shear** `fixed horizontal_shear() const` (L281): Returns the horizontal shear of the background.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L286): Sets the horizontal shear of the background.
- **vertical_shear** `fixed vertical_shear() const` (L291): Returns the vertical shear of the background.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L296): Sets the vertical shear of the background.
- **set_shear** `void set_shear(fixed shear)` (L301): Sets the shear of the background.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L308): Sets the shear of the background.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `bool horizontal_flip() const` (L313): Indicates if this background is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L318): Sets if this background must be flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L323): Indicates if this background must be flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L328): Sets if this background must be flipped in the vertical axis or not.
- **pivot_x** `fixed pivot_x() const` (L333): Returns the horizontal position of the affine transformations pivot point of this background.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L338): Sets the horizontal position of the affine transformations pivot point of this background.
- **pivot_y** `fixed pivot_y() const` (L343): Returns the vertical position of the affine transformations pivot point of this background.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L348): Sets the vertical position of the affine transformations pivot point of this background.
- **pivot_position** `const fixed_point& pivot_position() const` (L353): Returns the position of the affine transformations pivot point of this background.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L360): Sets the position of the affine transformations pivot point of this background.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L366): Sets the position of the affine transformations pivot point of this background.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_bg_mat_attributes& mat_attributes() const` (L371): Returns the attributes of the position and the transformation matrix of this background.
- **set_mat_attributes** `void set_mat_attributes(const affine_mat_attributes& mat_attributes)` (L376): Sets the attributes of the affine transformation matrix of this background.
- **priority** `int priority() const` (L384): Returns the priority of the background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L394): Sets the priority of the background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L399): Indicates if the mosaic effect must be applied to this background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L404): Sets if the mosaic effect must be applied to this background or not.
- **blending_enabled** `bool blending_enabled() const` (L411): Indicates if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L421): Sets if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L429): Indicates if this background is part of the blending top layer or not.
- **set_blending_top_enabled** `void set_blending_top_enabled(bool blending_top_enabled)` (L434): Sets if this background is part of the blending top layer or not.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L439): Indicates if this background is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L444): Sets if this background is part of the blending bottom layer or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L449): Indicates how this background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L454): Sets how this background must be displayed when green swap is enabled.
- **visible** `bool visible() const` (L459): Indicates if this background must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L464): Sets if this background must be committed to the GBA or not.
- **visible_in_window** `bool visible_in_window(const window& window) const` (L469): Indicates if this background is visible in the given window or not.
- **set_visible_in_window** `void set_visible_in_window(bool visible, window& window)` (L474): Sets if this background must be visible in the given window or not.
- **camera** `const optional<camera_ptr>& camera() const` (L479): Returns the camera_ptr attached to this background (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L485): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to copy to this background.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L491): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to move to this background.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L497): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to copy to this background.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L503): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to move to this background.
- **remove_camera** `void remove_camera()` (L508): Removes the camera_ptr attached to this background (if any).
- **attributes** `bitmap_bg_attributes attributes() const` (L513): Returns the attributes to commit to the GBA for this background.
- **set_attributes** `void set_attributes(const bitmap_bg_attributes& attributes)` (L518): Sets the attributes to commit to the GBA for this background.
- **hw_id** `optional<int> hw_id() const` (L532): Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
  Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
  
  A background doesn't have an assigned hardware ID if it is not visible.
  
  Assigned hardware ID can change after calling this method
  if some properties of this background or others are updated.
  
  Call this method at your own risk.
- **handle** `const void* handle() const` (L537): Returns the internal handle.
- **swap** `void swap(dp_direct_bitmap_bg_ptr& other)` (L546): Exchanges the contents of this dp_direct_bitmap_bg_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_ptr& a, dp_direct_bitmap_bg_ptr& b)` (L556): Exchanges the contents of an dp_direct_bitmap_bg_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const dp_direct_bitmap_bg_ptr& a, const dp_direct_bitmap_bg_ptr& b) = default` (L564): Default equal operator.
