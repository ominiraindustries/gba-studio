# Butano 21.10.1 — Single page direct bitmap backgrounds (`sp_direct_bitmap_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [direct_bitmap_bg](direct_bitmap_bg.md)

## bn_sp_direct_bitmap_bg_builder.h

### `bn::sp_direct_bitmap_bg_builder` (class, bn_sp_direct_bitmap_bg_builder.h:30)

Creates sp_direct_bitmap_bg_ptr objects with custom attributes.

`class sp_direct_bitmap_bg_builder`

- **x** `fixed x() const` (L38): Returns the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_x** `sp_direct_bitmap_bg_builder& set_x(fixed x)` (L49): Sets the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L59): Returns the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_y** `sp_direct_bitmap_bg_builder& set_y(fixed y)` (L70): Sets the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `y` vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L80): Returns the position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_position** `sp_direct_bitmap_bg_builder& set_position(fixed x, fixed y)` (L92): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; `y` Vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **set_position** `sp_direct_bitmap_bg_builder& set_position(const fixed_point& position)` (L104): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `position` Position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L114): Returns the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `sp_direct_bitmap_bg_builder& set_top_left_x(fixed top_left_x)` (L122): Sets the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L128): Returns the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `sp_direct_bitmap_bg_builder& set_top_left_y(fixed top_left_y)` (L136): Sets the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L142): Returns the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `sp_direct_bitmap_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L151): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **set_top_left_position** `sp_direct_bitmap_bg_builder& set_top_left_position(const fixed_point& top_left_position)` (L159): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **rotation_angle** `fixed rotation_angle() const` (L164): Returns the rotation angle in degrees of the backgrounds to generate.
- **set_rotation_angle** `sp_direct_bitmap_bg_builder& set_rotation_angle(fixed rotation_angle)` (L174): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; devuelve: Reference to this.
- **set_rotation_angle_safe** `sp_direct_bitmap_bg_builder& set_rotation_angle_safe(fixed rotation_angle)` (L185): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in any range.; devuelve: Reference to this.
- **horizontal_scale** `fixed horizontal_scale() const` (L194): Returns the horizontal scale of the backgrounds to generate.
- **set_horizontal_scale** `sp_direct_bitmap_bg_builder& set_horizontal_scale(fixed horizontal_scale)` (L204): Sets the horizontal scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_scale** `fixed vertical_scale() const` (L213): Returns the vertical scale of the backgrounds to generate.
- **set_vertical_scale** `sp_direct_bitmap_bg_builder& set_vertical_scale(fixed vertical_scale)` (L223): Sets the vertical scale of the backgrounds to generate.
  `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `sp_direct_bitmap_bg_builder& set_scale(fixed scale)` (L234): Sets the scale of the backgrounds to generate.
  `scale` Scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `sp_direct_bitmap_bg_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)` (L246): Sets the scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **expand** `sp_direct_bitmap_bg_builder& expand()` (L256): Sets the scale of the backgrounds to generate to fill the screen.
  devuelve: Reference to this.
- **horizontal_shear** `fixed horizontal_shear() const` (L261): Returns the horizontal shear of the backgrounds to generate.
- **set_horizontal_shear** `sp_direct_bitmap_bg_builder& set_horizontal_shear(fixed horizontal_shear)` (L271): Sets the horizontal shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_shear** `fixed vertical_shear() const` (L280): Returns the vertical shear of the backgrounds to generate.
- **set_vertical_shear** `sp_direct_bitmap_bg_builder& set_vertical_shear(fixed vertical_shear)` (L290): Sets the vertical shear of the backgrounds to generate.
  `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `sp_direct_bitmap_bg_builder& set_shear(fixed shear)` (L301): Sets the shear of the backgrounds to generate.
  `shear` Shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `sp_direct_bitmap_bg_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)` (L313): Sets the shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **horizontal_flip** `bool horizontal_flip() const` (L322): Indicates if the backgrounds to generate are flipped in the horizontal axis or not.
- **set_horizontal_flip** `sp_direct_bitmap_bg_builder& set_horizontal_flip(bool horizontal_flip)` (L332): Sets if the backgrounds to generate must be flipped in the horizontal axis or not.
  `horizontal_flip` `true` if the backgrounds must be flipped in the horizontal axis; `false` otherwise.; devuelve: Reference to this.
- **vertical_flip** `bool vertical_flip() const` (L341): Indicates if the backgrounds to generate are flipped in the vertical axis or not.
- **set_vertical_flip** `sp_direct_bitmap_bg_builder& set_vertical_flip(bool vertical_flip)` (L351): Sets if the backgrounds to generate must be flipped in the vertical axis or not.
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
- **set_affine_mat** `sp_direct_bitmap_bg_builder& set_affine_mat(const affine_mat_attributes& mat_attributes)` (L430): Sets the attributes of the affine transformation matrix of the backgrounds to generate.
  `mat_attributes` affine_mat_attributes of the backgrounds to generate.; devuelve: Reference to this.
- **priority** `int priority() const` (L442): Returns the priority of the backgrounds to generate relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `sp_direct_bitmap_bg_builder& set_priority(int priority)` (L456): Sets the priority of the backgrounds to generate relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].; devuelve: Reference to this.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L461): Indicates if the mosaic effect must be applied to the backgrounds to generate or not.
- **set_mosaic_enabled** `sp_direct_bitmap_bg_builder& set_mosaic_enabled(bool mosaic_enabled)` (L471): Sets if the mosaic effect must be applied to the backgrounds to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L482): Indicates if blending must be applied to the backgrounds to generate or not.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **set_blending_enabled** `sp_direct_bitmap_bg_builder& set_blending_enabled(bool blending_enabled)` (L494): Sets if blending must be applied to the backgrounds to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L503): Indicates if the backgrounds to generate must be part of the blending top layer or not.
- **set_blending_top_enabled** `sp_direct_bitmap_bg_builder& set_blending_top_enabled(bool blending_top_enabled)` (L514): Sets if the backgrounds to generate must be part of the blending top layer or not.
  `blending_top_enabled` `true` if generated backgrounds must be part of the blending top layer; `false` otherwise.; devuelve: Reference to this.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L523): Indicates if the backgrounds to generate must be part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `sp_direct_bitmap_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)` (L534): Sets if the backgrounds to generate must be part of the blending bottom layer or not.
  `blending_bottom_enabled` `true` if generated backgrounds must be part of the blending bottom layer; `false` otherwise.; devuelve: Reference to this.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L543): Indicates how the backgrounds to generate must be displayed when green swap is enabled.
- **set_green_swap_mode** `sp_direct_bitmap_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L553): Sets how the backgrounds to generate must be displayed when green swap is enabled.
  `green_swap_mode` Green swap mode.; devuelve: Reference to this.
- **visible** `bool visible() const` (L562): Indicates if the backgrounds to generate must be committed to the GBA or not.
- **set_visible** `sp_direct_bitmap_bg_builder& set_visible(bool visible)` (L572): Sets if the backgrounds to generate must be committed to the GBA or not.
  `visible` `true` if the backgrounds must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L581): Returns the camera_ptr to attach to the backgrounds to generate (if any).
- **set_camera** `sp_direct_bitmap_bg_builder& set_camera(const camera_ptr& camera)` (L591): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `sp_direct_bitmap_bg_builder& set_camera(camera_ptr&& camera)` (L602): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `sp_direct_bitmap_bg_builder& set_camera(const optional<camera_ptr>& camera)` (L613): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `sp_direct_bitmap_bg_builder& set_camera(optional<camera_ptr>&& camera)` (L624): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `sp_direct_bitmap_bg_builder& remove_camera()` (L634): Removes the camera_ptr to attach to the backgrounds to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L643): Releases and returns the camera_ptr to attach to the backgrounds to generate (if any).
- **build** `sp_direct_bitmap_bg_ptr build() const` (L648): Generates and returns a sp_direct_bitmap_bg_ptr without releasing the acquired resources.
- **release_build** `sp_direct_bitmap_bg_ptr release_build()` (L655): Generates and returns a sp_direct_bitmap_bg_ptr releasing the acquired resources.
  sp_direct_bitmap_bg_ptr generation after calling this method may stop working.
- **build_optional** `optional<sp_direct_bitmap_bg_ptr> build_optional() const` (L661): Generates and returns a sp_direct_bitmap_bg_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<sp_direct_bitmap_bg_ptr> release_build_optional()` (L669): Generates and returns a sp_direct_bitmap_bg_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  sp_direct_bitmap_bg_ptr generation after calling this method may stop working.

## bn_sp_direct_bitmap_bg_painter.h

### `bn::sp_direct_bitmap_bg_painter` (class, bn_sp_direct_bitmap_bg_painter.h:30)

Performs low-level painting on single page direct bitmap backgrounds.

`class sp_direct_bitmap_bg_painter`

- **sp_direct_bitmap_bg_painter** `explicit sp_direct_bitmap_bg_painter(const sp_direct_bitmap_bg_ptr& bg)` (L38): Class constructor.
  `bg` Single page direct bitmap background to be modified.
- **sp_direct_bitmap_bg_painter** `explicit sp_direct_bitmap_bg_painter(sp_direct_bitmap_bg_ptr&& bg)` (L47): Class constructor.
  `bg` Single page direct bitmap background to be modified.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L55): Returns the background modified by this painter.
- **page** `span<const color> page() const` (L63): Returns a span of the hidden frame buffer.
- **page** `span<color> page()` (L72): Returns a span of the hidden frame buffer.
- **fill** `inline void fill(color color)` (L81): Fills the current page with the given color.
- **unsafe_get** `inline color unsafe_get(int x, int y) const` (L92): Returns the referenced color without bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::sp_direct_width()).; `y` Vertical position in the current page [0..bitmap_bg::sp_direct_height()).; devuelve: The referenced color.
- **unsafe_get** `inline color unsafe_get(const point& position) const` (L102): Returns the referenced color without bounds checking.
  `position` Position in the current page.; devuelve: The referenced color.
- **get** `inline optional<color> get(int x, int y) const` (L113): Returns the referenced color with bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::sp_direct_width()).; `y` Vertical position in the current page [0..bitmap_bg::sp_direct_height()).; devuelve: The referenced color.
- **get** `inline optional<color> get(const point& position) const` (L130): Returns the referenced color with bounds checking.
  `position` Position in the current page.; devuelve: The referenced color.
- **unsafe_plot** `inline void unsafe_plot(int x, int y, color color)` (L141): Plots a pixel on the current page without bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **unsafe_plot** `inline void unsafe_plot(const point& position, color color)` (L151): Plots a pixel on the current page without bounds checking.
  `position` Position of the pixel to plot.; `color` Color to plot.
- **plot** `inline void plot(int x, int y, color color)` (L162): Plots a pixel on the current page with bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **plot** `inline void plot(const point& position, color color)` (L175): Plots a pixel on the current page with bounds checking.
  `position` Position of the pixel to plot.; `color` Color to plot.
- **unsafe_horizontal_line** `inline void unsafe_horizontal_line(int x1, int x2, int y, color color)` (L187): Draws an horizontal line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **horizontal_line** `inline void horizontal_line(int x1, int x2, int y, color color)` (L205): Draws an horizontal line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **unsafe_vertical_line** `inline void unsafe_vertical_line(int x, int y1, int y2, color color)` (L227): Draws a vertical line in the current page without bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **vertical_line** `inline void vertical_line(int x, int y1, int y2, color color)` (L251): Draws a vertical line in the current page with bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **unsafe_line** `inline void unsafe_line(int x1, int y1, int x2, int y2, color color)` (L274): Draws a line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **unsafe_line** `inline void unsafe_line(const point& first, const point& last, color color)` (L364): Draws a line in the current page without bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color` Color to plot.
- **line** `inline void line(int x1, int y1, int x2, int y2, color color)` (L377): Draws a line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **line** `inline void line(const point& first, const point& last, color color)` (L394): Draws a line in the current page with bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color` Color to plot.
- **unsafe_rectangle** `inline void unsafe_rectangle(int x1, int y1, int x2, int y2, color color)` (L407): Draws a filled rectangle in the current page without bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::sp_direct_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::sp_direct_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **unsafe_rectangle** `inline void unsafe_rectangle(const point& first, const point& last, color color)` (L438): Draws a filled rectangle in the current page without bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color` Color to plot.
- **rectangle** `inline void rectangle(int x1, int y1, int x2, int y2, color color)` (L451): Draws a filled rectangle in the current page with bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::sp_direct_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::sp_direct_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::sp_direct_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::sp_direct_height()).; `color` Color to plot.
- **rectangle** `inline void rectangle(const point& first, const point& last, color color)` (L473): Draws a filled rectangle in the current page with bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color` Color to plot.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const direct_bitmap_item& item)` (L485): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const direct_bitmap_item& item)` (L498): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const direct_bitmap_roi& roi)` (L510): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `roi` Region of interest to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const direct_bitmap_roi& roi)` (L524): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(int x, int y, const direct_bitmap_item& item)` (L536): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `item` Item to copy to the current page.
- **blit** `inline void blit(const point& position, const direct_bitmap_item& item)` (L557): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **blit** `inline void blit(int x, int y, const direct_bitmap_roi& roi)` (L569): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(const point& position, const direct_bitmap_roi& roi)` (L591): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)` (L604): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)` (L618): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)` (L631): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)` (L646): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)` (L659): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)` (L681): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)` (L694): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.
- **draw** `inline void draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)` (L717): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.; `transparent_color` Color of the transparent pixels.

## bn_sp_direct_bitmap_bg_ptr.h

### `bn::sp_direct_bitmap_bg_ptr` (class, bn_sp_direct_bitmap_bg_ptr.h:42)

std::shared_ptr like smart pointer that retains shared ownership of a single page direct bitmap background.

`class sp_direct_bitmap_bg_ptr`

Several sp_direct_bitmap_bg_ptr objects may own the same background.

The background is released when the last remaining sp_direct_bitmap_bg_ptr owning it is destroyed.

- **dimensions** `static size dimensions()` (L49): Returns the size in pixels of the backgrounds.
- **create** `static sp_direct_bitmap_bg_ptr create()` (L54): Creates a sp_direct_bitmap_bg_ptr.
- **create** `static sp_direct_bitmap_bg_ptr create(const sp_direct_bitmap_bg_builder& builder)` (L61): Creates a sp_direct_bitmap_bg_ptr from a sp_direct_bitmap_bg_builder reference.
  `builder` sp_direct_bitmap_bg_builder reference.; devuelve: The requested sp_direct_bitmap_bg_ptr.
- **create** `static sp_direct_bitmap_bg_ptr create(sp_direct_bitmap_bg_builder&& builder)` (L68): Creates a sp_direct_bitmap_bg_ptr from a moved sp_direct_bitmap_bg_builder.
  `builder` sp_direct_bitmap_bg_builder to move.; devuelve: The requested sp_direct_bitmap_bg_ptr.
- **create_optional** `static optional<sp_direct_bitmap_bg_ptr> create_optional()` (L73): Creates a sp_direct_bitmap_bg_ptr from a sp_direct_bitmap_bg_builder reference.
- **create_optional** `static optional<sp_direct_bitmap_bg_ptr> create_optional(const sp_direct_bitmap_bg_builder& builder)` (L80): Creates a sp_direct_bitmap_bg_ptr from a sp_direct_bitmap_bg_builder reference.
  `builder` sp_direct_bitmap_bg_builder reference.; devuelve: The requested sp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<sp_direct_bitmap_bg_ptr> create_optional(sp_direct_bitmap_bg_builder&& builder)` (L87): Creates a sp_direct_bitmap_bg_ptr from a moved sp_direct_bitmap_bg_builder.
  `builder` sp_direct_bitmap_bg_builder to move.; devuelve: The requested sp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **sp_direct_bitmap_bg_ptr** `sp_direct_bitmap_bg_ptr(const sp_direct_bitmap_bg_ptr& other)` (L93): Copy constructor.
  `other` sp_direct_bitmap_bg_ptr to copy.
- **operator=** `sp_direct_bitmap_bg_ptr& operator=(const sp_direct_bitmap_bg_ptr& other)` (L100): Copy assignment operator.
  `other` sp_direct_bitmap_bg_ptr to copy.; devuelve: Reference to this.
- **sp_direct_bitmap_bg_ptr** `sp_direct_bitmap_bg_ptr(sp_direct_bitmap_bg_ptr&& other) noexcept : sp_direct_bitmap_bg_ptr(other._handle)` (L106): Move constructor.
  `other` sp_direct_bitmap_bg_ptr to move.
- **operator=** `sp_direct_bitmap_bg_ptr& operator=(sp_direct_bitmap_bg_ptr&& other) noexcept` (L117): Move assignment operator.
  `other` sp_direct_bitmap_bg_ptr to move.; devuelve: Reference to this.
- **~sp_direct_bitmap_bg_ptr** `~sp_direct_bitmap_bg_ptr()` (L126): Releases the referenced background if no more sp_direct_bitmap_bg_ptr objects reference to it.
- **page** `span<const color> page() const` (L131): Returns a span of the hidden frame buffer.
- **page** `span<color> page()` (L136): Returns a span of the hidden frame buffer.
- **x** `fixed x() const` (L141): Returns the horizontal position of the background (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L146): Sets the horizontal position of the background (relative to its camera, if it has one).
- **y** `fixed y() const` (L151): Returns the vertical position of the background (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L156): Sets the vertical position of the background (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L161): Returns the position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L168): Sets the position of the background (relative to its camera, if it has one).
  `x` Horizontal position of the background (relative to its camera, if it has one).; `y` Vertical position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L173): Sets the position of the background (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L178): Returns the horizontal top-left position of the background (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L183): Sets the horizontal top-left position of the background (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L188): Returns the vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L193): Sets the vertical top-left position of the background (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L198): Returns the top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L205): Sets the top-left position of the background (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the background (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L210): Sets the top-left position of the background (relative to its camera, if it has one).
- **rotation_angle** `fixed rotation_angle() const` (L215): Returns the rotation angle in degrees of the background.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L221): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L227): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `fixed horizontal_scale() const` (L232): Returns the horizontal scale of the background.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L237): Sets the horizontal scale of the background.
- **vertical_scale** `fixed vertical_scale() const` (L242): Returns the vertical scale of the background.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L247): Sets the vertical scale of the background.
- **set_scale** `void set_scale(fixed scale)` (L252): Sets the scale of the background.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L259): Sets the scale of the background.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **expand** `void expand()` (L264): Scales the background to fill the screen.
- **horizontal_shear** `fixed horizontal_shear() const` (L269): Returns the horizontal shear of the background.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L274): Sets the horizontal shear of the background.
- **vertical_shear** `fixed vertical_shear() const` (L279): Returns the vertical shear of the background.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L284): Sets the vertical shear of the background.
- **set_shear** `void set_shear(fixed shear)` (L289): Sets the shear of the background.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L296): Sets the shear of the background.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `bool horizontal_flip() const` (L301): Indicates if this background is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L306): Sets if this background must be flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L311): Indicates if this background must be flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L316): Sets if this background must be flipped in the vertical axis or not.
- **pivot_x** `fixed pivot_x() const` (L321): Returns the horizontal position of the affine transformations pivot point of this background.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L326): Sets the horizontal position of the affine transformations pivot point of this background.
- **pivot_y** `fixed pivot_y() const` (L331): Returns the vertical position of the affine transformations pivot point of this background.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L336): Sets the vertical position of the affine transformations pivot point of this background.
- **pivot_position** `const fixed_point& pivot_position() const` (L341): Returns the position of the affine transformations pivot point of this background.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L348): Sets the position of the affine transformations pivot point of this background.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L354): Sets the position of the affine transformations pivot point of this background.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_bg_mat_attributes& mat_attributes() const` (L359): Returns the attributes of the position and the transformation matrix of this background.
- **set_mat_attributes** `void set_mat_attributes(const affine_mat_attributes& mat_attributes)` (L364): Sets the attributes of the affine transformation matrix of this background.
- **priority** `int priority() const` (L372): Returns the priority of the background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L382): Sets the priority of the background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L387): Indicates if the mosaic effect must be applied to this background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L392): Sets if the mosaic effect must be applied to this background or not.
- **blending_enabled** `bool blending_enabled() const` (L399): Indicates if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L409): Sets if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L417): Indicates if this background is part of the blending top layer or not.
- **set_blending_top_enabled** `void set_blending_top_enabled(bool blending_top_enabled)` (L422): Sets if this background is part of the blending top layer or not.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L427): Indicates if this background is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L432): Sets if this background is part of the blending bottom layer or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L437): Indicates how this background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L442): Sets how this background must be displayed when green swap is enabled.
- **visible** `bool visible() const` (L447): Indicates if this background must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L452): Sets if this background must be committed to the GBA or not.
- **visible_in_window** `bool visible_in_window(const window& window) const` (L457): Indicates if this background is visible in the given window or not.
- **set_visible_in_window** `void set_visible_in_window(bool visible, window& window)` (L462): Sets if this background must be visible in the given window or not.
- **camera** `const optional<camera_ptr>& camera() const` (L467): Returns the camera_ptr attached to this background (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L473): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to copy to this background.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L479): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to move to this background.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L485): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to copy to this background.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L491): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to move to this background.
- **remove_camera** `void remove_camera()` (L496): Removes the camera_ptr attached to this background (if any).
- **attributes** `bitmap_bg_attributes attributes() const` (L501): Returns the attributes to commit to the GBA for this background.
- **set_attributes** `void set_attributes(const bitmap_bg_attributes& attributes)` (L506): Sets the attributes to commit to the GBA for this background.
- **hw_id** `optional<int> hw_id() const` (L520): Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
  Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
  
  A background doesn't have an assigned hardware ID if it is not visible.
  
  Assigned hardware ID can change after calling this method
  if some properties of this background or others are updated.
  
  Call this method at your own risk.
- **handle** `const void* handle() const` (L525): Returns the internal handle.
- **swap** `void swap(sp_direct_bitmap_bg_ptr& other)` (L534): Exchanges the contents of this sp_direct_bitmap_bg_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_ptr& a, sp_direct_bitmap_bg_ptr& b)` (L544): Exchanges the contents of an sp_direct_bitmap_bg_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sp_direct_bitmap_bg_ptr& a, const sp_direct_bitmap_bg_ptr& b) = default` (L552): Default equal operator.
