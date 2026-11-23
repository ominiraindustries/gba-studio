# Butano 21.10.1 — Palette bitmap backgrounds (`palette_bitmap_bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bitmap_bg](bitmap_bg.md)

## bn_palette_bitmap_bg_builder.h

### `bn::palette_bitmap_bg_builder` (class, bn_palette_bitmap_bg_builder.h:32)

Creates palette_bitmap_bg_ptr objects with custom attributes.

`class palette_bitmap_bg_builder`

- **palette_bitmap_bg_builder** `explicit palette_bitmap_bg_builder(const bg_palette_item& palette_item)` (L41): Constructor.
  `palette_item` bg_palette_item containing the required information to generate the palette used by the generated backgrounds.
- **palette_bitmap_bg_builder** `explicit palette_bitmap_bg_builder(const bg_palette_ptr& palette)` (L47): Constructor.
  `palette` bg_palette_ptr to copy for the generated backgrounds.
- **palette_bitmap_bg_builder** `explicit palette_bitmap_bg_builder(bg_palette_ptr&& palette)` (L53): Constructor.
  `palette` bg_palette_ptr to move for the generated backgrounds.
- **palette_item** `const optional<bg_palette_item>& palette_item() const` (L59): Returns the bg_palette_item containing the required information to generate the palette used by the generated backgrounds if it has one; bn::nullopt otherwise.
- **x** `fixed x() const` (L68): Returns the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_x** `palette_bitmap_bg_builder& set_x(fixed x)` (L79): Sets the horizontal position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; devuelve: Reference to this.
- **y** `fixed y() const` (L89): Returns the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_y** `palette_bitmap_bg_builder& set_y(fixed y)` (L100): Sets the vertical position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `y` vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **position** `const fixed_point& position() const` (L110): Returns the position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_position** `palette_bitmap_bg_builder& set_position(fixed x, fixed y)` (L122): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `x` Horizontal position of the backgrounds to generate.; `y` Vertical position of the backgrounds to generate.; devuelve: Reference to this.
- **set_position** `palette_bitmap_bg_builder& set_position(const fixed_point& position)` (L134): Sets the position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `position` Position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_x** `fixed top_left_x() const` (L144): Returns the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_x** `palette_bitmap_bg_builder& set_top_left_x(fixed top_left_x)` (L152): Sets the horizontal top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_y** `fixed top_left_y() const` (L158): Returns the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_y** `palette_bitmap_bg_builder& set_top_left_y(fixed top_left_y)` (L166): Sets the vertical top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **top_left_position** `fixed_point top_left_position() const` (L172): Returns the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
- **set_top_left_position** `palette_bitmap_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y)` (L181): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_x` Horizontal top-left position of the backgrounds to generate.; `top_left_y` Vertical top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **set_top_left_position** `palette_bitmap_bg_builder& set_top_left_position(const fixed_point& top_left_position)` (L189): Sets the top-left position of the backgrounds to generate (relative to their camera, if they are going to have one).
  `top_left_position` Top-left position of the backgrounds to generate.; devuelve: Reference to this.
- **rotation_angle** `fixed rotation_angle() const` (L194): Returns the rotation angle in degrees of the backgrounds to generate.
- **set_rotation_angle** `palette_bitmap_bg_builder& set_rotation_angle(fixed rotation_angle)` (L204): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; devuelve: Reference to this.
- **set_rotation_angle_safe** `palette_bitmap_bg_builder& set_rotation_angle_safe(fixed rotation_angle)` (L215): Sets the rotation angle in degrees of the backgrounds to generate.
  `rotation_angle` Rotation angle in degrees, in any range.; devuelve: Reference to this.
- **horizontal_scale** `fixed horizontal_scale() const` (L224): Returns the horizontal scale of the backgrounds to generate.
- **set_horizontal_scale** `palette_bitmap_bg_builder& set_horizontal_scale(fixed horizontal_scale)` (L234): Sets the horizontal scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_scale** `fixed vertical_scale() const` (L243): Returns the vertical scale of the backgrounds to generate.
- **set_vertical_scale** `palette_bitmap_bg_builder& set_vertical_scale(fixed vertical_scale)` (L253): Sets the vertical scale of the backgrounds to generate.
  `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `palette_bitmap_bg_builder& set_scale(fixed scale)` (L264): Sets the scale of the backgrounds to generate.
  `scale` Scale of the backgrounds to generate.; devuelve: Reference to this.
- **set_scale** `palette_bitmap_bg_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)` (L276): Sets the scale of the backgrounds to generate.
  `horizontal_scale` Horizontal scale of the backgrounds to generate.; `vertical_scale` Vertical scale of the backgrounds to generate.; devuelve: Reference to this.
- **horizontal_shear** `fixed horizontal_shear() const` (L285): Returns the horizontal shear of the backgrounds to generate.
- **set_horizontal_shear** `palette_bitmap_bg_builder& set_horizontal_shear(fixed horizontal_shear)` (L295): Sets the horizontal shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; devuelve: Reference to this.
- **vertical_shear** `fixed vertical_shear() const` (L304): Returns the vertical shear of the backgrounds to generate.
- **set_vertical_shear** `palette_bitmap_bg_builder& set_vertical_shear(fixed vertical_shear)` (L314): Sets the vertical shear of the backgrounds to generate.
  `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `palette_bitmap_bg_builder& set_shear(fixed shear)` (L325): Sets the shear of the backgrounds to generate.
  `shear` Shear of the backgrounds to generate.; devuelve: Reference to this.
- **set_shear** `palette_bitmap_bg_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)` (L337): Sets the shear of the backgrounds to generate.
  `horizontal_shear` Horizontal shear of the backgrounds to generate.; `vertical_shear` Vertical shear of the backgrounds to generate.; devuelve: Reference to this.
- **horizontal_flip** `bool horizontal_flip() const` (L346): Indicates if the backgrounds to generate are flipped in the horizontal axis or not.
- **set_horizontal_flip** `palette_bitmap_bg_builder& set_horizontal_flip(bool horizontal_flip)` (L356): Sets if the backgrounds to generate must be flipped in the horizontal axis or not.
  `horizontal_flip` `true` if the backgrounds must be flipped in the horizontal axis; `false` otherwise.; devuelve: Reference to this.
- **vertical_flip** `bool vertical_flip() const` (L365): Indicates if the backgrounds to generate are flipped in the vertical axis or not.
- **set_vertical_flip** `palette_bitmap_bg_builder& set_vertical_flip(bool vertical_flip)` (L375): Sets if the backgrounds to generate must be flipped in the vertical axis or not.
  `vertical_flip` `true` if the backgrounds must be flipped in the vertical axis; `false` otherwise.; devuelve: Reference to this.
- **pivot_x** `fixed pivot_x() const` (L384): Returns the horizontal position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L392): Sets the horizontal position of the affine transformations pivot point of the backgrounds to generate.
- **pivot_y** `fixed pivot_y() const` (L400): Returns the vertical position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L408): Sets the vertical position of the affine transformations pivot point of the backgrounds to generate.
- **pivot_position** `const fixed_point& pivot_position() const` (L416): Returns the position of the affine transformations pivot point of the backgrounds to generate.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L426): Sets the position of the affine transformations pivot point of the backgrounds to generate.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L436): Sets the position of the affine transformations pivot point of the backgrounds to generate.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_mat_attributes& mat_attributes() const` (L444): Returns the attributes of the affine transformation matrix of the backgrounds to generate.
- **set_affine_mat** `palette_bitmap_bg_builder& set_affine_mat(const affine_mat_attributes& mat_attributes)` (L454): Sets the attributes of the affine transformation matrix of the backgrounds to generate.
  `mat_attributes` affine_mat_attributes of the backgrounds to generate.; devuelve: Reference to this.
- **priority** `int priority() const` (L466): Returns the priority of the backgrounds to generate relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `palette_bitmap_bg_builder& set_priority(int priority)` (L480): Sets the priority of the backgrounds to generate relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].; devuelve: Reference to this.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L485): Indicates if the mosaic effect must be applied to the backgrounds to generate or not.
- **set_mosaic_enabled** `palette_bitmap_bg_builder& set_mosaic_enabled(bool mosaic_enabled)` (L495): Sets if the mosaic effect must be applied to the backgrounds to generate or not.
  `mosaic_enabled` `true` if the mosaic effect must be applied; `false` otherwise.; devuelve: Reference to this.
- **blending_enabled** `bool blending_enabled() const` (L506): Indicates if blending must be applied to the backgrounds to generate or not.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **set_blending_enabled** `palette_bitmap_bg_builder& set_blending_enabled(bool blending_enabled)` (L518): Sets if blending must be applied to the backgrounds to generate or not.
  `blending_enabled` `true` if blending must be applied; `false` otherwise.; devuelve: Reference to this.
  Blending is applied to backgrounds by making them part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L527): Indicates if the backgrounds to generate must be part of the blending top layer or not.
- **set_blending_top_enabled** `palette_bitmap_bg_builder& set_blending_top_enabled(bool blending_top_enabled)` (L538): Sets if the backgrounds to generate must be part of the blending top layer or not.
  `blending_top_enabled` `true` if generated backgrounds must be part of the blending top layer; `false` otherwise.; devuelve: Reference to this.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L547): Indicates if the backgrounds to generate must be part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `palette_bitmap_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)` (L558): Sets if the backgrounds to generate must be part of the blending bottom layer or not.
  `blending_bottom_enabled` `true` if generated backgrounds must be part of the blending bottom layer; `false` otherwise.; devuelve: Reference to this.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L567): Indicates how the backgrounds to generate must be displayed when green swap is enabled.
- **set_green_swap_mode** `palette_bitmap_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L577): Sets how the backgrounds to generate must be displayed when green swap is enabled.
  `green_swap_mode` Green swap mode.; devuelve: Reference to this.
- **visible** `bool visible() const` (L586): Indicates if the backgrounds to generate must be committed to the GBA or not.
- **set_visible** `palette_bitmap_bg_builder& set_visible(bool visible)` (L596): Sets if the backgrounds to generate must be committed to the GBA or not.
  `visible` `true` if the backgrounds must be committed to the GBA; `false` otherwise.; devuelve: Reference to this.
- **camera** `const optional<camera_ptr>& camera() const` (L605): Returns the camera_ptr to attach to the backgrounds to generate (if any).
- **set_camera** `palette_bitmap_bg_builder& set_camera(const camera_ptr& camera)` (L615): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `palette_bitmap_bg_builder& set_camera(camera_ptr&& camera)` (L626): Sets the camera_ptr to attach to the backgrounds to generate.
  `camera` camera_ptr to move to the builder.; devuelve: Reference to this.
- **set_camera** `palette_bitmap_bg_builder& set_camera(const optional<camera_ptr>& camera)` (L637): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to copy to the builder.; devuelve: Reference to this.
- **set_camera** `palette_bitmap_bg_builder& set_camera(optional<camera_ptr>&& camera)` (L648): Sets or removes the camera_ptr to attach to the backgrounds to generate.
  `camera` Optional camera_ptr to move to the builder.; devuelve: Reference to this.
- **remove_camera** `palette_bitmap_bg_builder& remove_camera()` (L658): Removes the camera_ptr to attach to the backgrounds to generate.
  devuelve: Reference to this.
- **release_camera** `optional<camera_ptr> release_camera()` (L667): Releases and returns the camera_ptr to attach to the backgrounds to generate (if any).
- **build** `palette_bitmap_bg_ptr build() const` (L672): Generates and returns a palette_bitmap_bg_ptr without releasing the acquired resources.
- **release_build** `palette_bitmap_bg_ptr release_build()` (L679): Generates and returns a palette_bitmap_bg_ptr releasing the acquired resources.
  palette_bitmap_bg_ptr generation after calling this method may stop working.
- **build_optional** `optional<palette_bitmap_bg_ptr> build_optional() const` (L685): Generates and returns a palette_bitmap_bg_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_build_optional** `optional<palette_bitmap_bg_ptr> release_build_optional()` (L693): Generates and returns a palette_bitmap_bg_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  palette_bitmap_bg_ptr generation after calling this method may stop working.
- **palette** `bg_palette_ptr palette() const` (L698): Generates and returns a bg_palette_ptr without releasing the acquired resources.
- **palette_optional** `optional<bg_palette_ptr> palette_optional() const` (L704): Generates and returns a bg_palette_ptr without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
- **release_palette** `bg_palette_ptr release_palette()` (L711): Generates and returns a bg_palette_ptr releasing the acquired resources.
  palette_bitmap_bg_ptr generation after calling this method may stop working.
- **release_palette_optional** `optional<bg_palette_ptr> release_palette_optional()` (L719): Generates and returns a bg_palette_ptr releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
  palette_bitmap_bg_ptr generation after calling this method may stop working.

## bn_palette_bitmap_bg_painter.h

### `bn::palette_bitmap_bg_painter` (class, bn_palette_bitmap_bg_painter.h:34)

Performs low-level painting on palette bitmap backgrounds.

`class palette_bitmap_bg_painter`

Only one bitmap painter can be active at the same time.

- **palette_bitmap_bg_painter** `explicit palette_bitmap_bg_painter(const palette_bitmap_bg_ptr& bg)` (L42): Class constructor.
  `bg` Palette bitmap background to be modified.
- **palette_bitmap_bg_painter** `explicit palette_bitmap_bg_painter(palette_bitmap_bg_ptr&& bg)` (L48): Class constructor.
  `bg` Palette bitmap background to be modified.
- **palette_bitmap_bg_painter** `palette_bitmap_bg_painter(palette_bitmap_bg_painter&& other) noexcept` (L58): Move constructor.
  `other` palette_bitmap_bg_painter to move.
- **operator=** `palette_bitmap_bg_painter& operator=(palette_bitmap_bg_painter&& other) noexcept` (L65): Move assignment operator.
  `other` palette_bitmap_bg_painter to move.; devuelve: Reference to this.
- **~palette_bitmap_bg_painter** `~palette_bitmap_bg_painter()` (L70): Class destructor.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L75): Returns the background modified by this painter.
- **page** `span<const uint16_t> page() const` (L83): Returns a span of the hidden frame buffer.
- **page** `span<uint16_t> page()` (L91): Returns a span of the hidden frame buffer.
- **flip_page_later** `void flip_page_later()` (L99): Swaps the hidden frame buffer in the next core::update call.
- **flip_page_now** `void flip_page_now()` (L109): Swaps the hidden frame buffer without waiting to the next core::update call.
  Expect lack of vsync issues like screen tearing.
- **clear** `inline void clear()` (L117): Fills the current page with the transparent color.
- **fill** `inline void fill(int color_index)` (L126): Fills the current page with the given color.
  `color_index` Palette index of the color to fill the current page.
- **unsafe_get** `inline int unsafe_get(int x, int y) const` (L137): Returns the referenced color palette index without bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::palette_width()).; `y` Vertical position in the current page [0..bitmap_bg::palette_height()).; devuelve: The referenced color palette index.
- **unsafe_get** `inline int unsafe_get(const point& position) const` (L148): Returns the referenced color palette index without bounds checking.
  `position` Position in the current page.; devuelve: The referenced color palette index.
- **get** `inline optional<int> get(int x, int y) const` (L159): Returns the referenced color palette index with bounds checking.
  `x` Horizontal position in the current page [0..bitmap_bg::palette_width()).; `y` Vertical position in the current page [0..bitmap_bg::palette_height()).; devuelve: The referenced color palette index.
- **get** `inline optional<int> get(const point& position) const` (L176): Returns the referenced color palette index with bounds checking.
  `position` Position in the current page.; devuelve: The referenced color palette index.
- **unsafe_plot** `inline void unsafe_plot(int x, int y, int color_index)` (L187): Plots a pixel on the current page without bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::palette_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::palette_height()).; `color_index` Palette index of the color to plot.
- **unsafe_plot** `inline void unsafe_plot(const point& position, int color_index)` (L206): Plots a pixel on the current page without bounds checking.
  `position` Position of the pixel to plot.; `color_index` Palette index of the color to plot.
- **plot** `inline void plot(int x, int y, int color_index)` (L217): Plots a pixel on the current page with bounds checking.
  `x` Horizontal position of the pixel to plot [0..bitmap_bg::palette_width()).; `y` Vertical position of the pixel to plot [0..bitmap_bg::palette_height()).; `color_index` Palette index of the color to plot.
- **plot** `inline void plot(const point& position, int color_index)` (L230): Plots a pixel on the current page with bounds checking.
  `position` Position of the pixel to plot.; `color_index` Palette index of the color to plot.
- **unsafe_horizontal_line** `inline void unsafe_horizontal_line(int x1, int x2, int y, int color_index)` (L242): Draws an horizontal line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::palette_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::palette_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **horizontal_line** `inline void horizontal_line(int x1, int x2, int y, int color_index)` (L289): Draws an horizontal line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::palette_width()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::palette_width()).; `y` Vertical position of the horizontal line [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **unsafe_vertical_line** `inline void unsafe_vertical_line(int x, int y1, int y2, int color_index)` (L311): Draws a vertical line in the current page without bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::palette_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **vertical_line** `inline void vertical_line(int x, int y1, int y2, int color_index)` (L355): Draws a vertical line in the current page with bounds checking.
  `x` Horizontal position of the vertical line [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::palette_height()).; `y2` Vertical position of the last pixel [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **unsafe_line** `inline void unsafe_line(int x1, int y1, int x2, int y2, int color_index)` (L378): Draws a line in the current page without bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::palette_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::palette_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **unsafe_line** `inline void unsafe_line(const point& first, const point& last, int color_index)` (L481): Draws a line in the current page without bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color_index` Color palette index.
- **line** `inline void line(int x1, int y1, int x2, int y2, int color_index)` (L494): Draws a line in the current page with bounds checking.
  `x1` Horizontal position of the first pixel [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first pixel [0..bitmap_bg::palette_height()).; `x2` Horizontal position of the last pixel [0..bitmap_bg::palette_width()).; `y2` Vertical position of the last pixel [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **line** `inline void line(const point& first, const point& last, int color_index)` (L511): Draws a line in the current page with bounds checking.
  `first` Position of the first pixel.; `last` Position of the last pixel.; `color_index` Color palette index.
- **unsafe_rectangle** `inline void unsafe_rectangle(int x1, int y1, int x2, int y2, int color_index)` (L524): Draws a filled rectangle in the current page without bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::palette_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::palette_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **unsafe_rectangle** `inline void unsafe_rectangle(const point& first, const point& last, int color_index)` (L591): Draws a filled rectangle in the current page without bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color_index` Color palette index.
- **rectangle** `inline void rectangle(int x1, int y1, int x2, int y2, int color_index)` (L604): Draws a filled rectangle in the current page with bounds checking.
  `x1` Horizontal position of the first corner [0..bitmap_bg::palette_width()).; `y1` Vertical position of the first corner [0..bitmap_bg::palette_height()).; `x2` Horizontal position of the last corner [0..bitmap_bg::palette_width()).; `y2` Vertical position of the last corner [0..bitmap_bg::palette_height()).; `color_index` Color palette index.
- **rectangle** `inline void rectangle(const point& first, const point& last, int color_index)` (L626): Draws a filled rectangle in the current page with bounds checking.
  `first` Position of the first corner.; `last` Position of the last corner.; `color_index` Color palette index.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const palette_bitmap_pixels_item& item)` (L638): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const palette_bitmap_pixels_item& item)` (L651): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const palette_bitmap_item& item)` (L663): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const palette_bitmap_item& item)` (L675): Copies the given item to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(int x, int y, const palette_bitmap_roi& roi)` (L687): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `roi` Region of interest to copy to the current page.
- **unsafe_blit** `inline void unsafe_blit(const point& position, const palette_bitmap_roi& roi)` (L701): Copies the given region of interest to the current page without bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::unsafe_draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(int x, int y, const palette_bitmap_pixels_item& item)` (L713): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **blit** `inline void blit(const point& position, const palette_bitmap_pixels_item& item)` (L734): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **blit** `inline void blit(int x, int y, const palette_bitmap_item& item)` (L746): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **blit** `inline void blit(const point& position, const palette_bitmap_item& item)` (L758): Copies the given item to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **blit** `inline void blit(int x, int y, const palette_bitmap_roi& roi)` (L770): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `roi` Region of interest to copy to the current page.
- **blit** `inline void blit(const point& position, const palette_bitmap_roi& roi)` (L792): Copies the given region of interest to the current page with bounds checking. Transparent pixels are also copied, so it should be faster than palette_bitmap_bg_painter::draw.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const palette_bitmap_pixels_item& item)` (L804): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const palette_bitmap_pixels_item& item)` (L817): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const palette_bitmap_item& item)` (L829): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const palette_bitmap_item& item)` (L841): Copies the given item to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(int x, int y, const palette_bitmap_roi& roi)` (L853): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `roi` Region of interest to copy to the current page.
- **unsafe_draw** `inline void unsafe_draw(const point& position, const palette_bitmap_roi& roi)` (L867): Copies the given region of interest to the current page without bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::unsafe_blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.
- **draw** `inline void draw(int x, int y, const palette_bitmap_pixels_item& item)` (L879): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **draw** `inline void draw(const point& position, const palette_bitmap_pixels_item& item)` (L900): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **draw** `inline void draw(int x, int y, const palette_bitmap_item& item)` (L912): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `item` Item to copy to the current page.
- **draw** `inline void draw(const point& position, const palette_bitmap_item& item)` (L924): Copies the given item to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `item` Item to copy to the current page.
- **draw** `inline void draw(int x, int y, const palette_bitmap_roi& roi)` (L936): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `x` Horizontal position of the current page top-left corner [0..bitmap_bg::palette_width()).; `y` Vertical position of the current page top-left corner [0..bitmap_bg::palette_height()).; `roi` Region of interest to copy to the current page.
- **draw** `inline void draw(const point& position, const palette_bitmap_roi& roi)` (L958): Copies the given region of interest to the current page with bounds checking. Transparent pixels are skipped, so it should be slower than palette_bitmap_bg_painter::blit.
  `position` Position of the current page top-left corner.; `roi` Region of interest to copy to the current page.

## bn_palette_bitmap_bg_ptr.h

### `bn::palette_bitmap_bg_ptr` (class, bn_palette_bitmap_bg_ptr.h:43)

std::shared_ptr like smart pointer that retains shared ownership of a palette bitmap background.

`class palette_bitmap_bg_ptr`

Several palette_bitmap_bg_ptr objects may own the same background.

The background is released when the last remaining palette_bitmap_bg_ptr owning it is destroyed.

- **dimensions** `static size dimensions()` (L50): Returns the size in pixels of the backgrounds.
- **create** `static palette_bitmap_bg_ptr create(const bg_palette_item& palette_item)` (L58): Creates a palette_bitmap_bg_ptr from the given bg_palette_item.
  `palette_item` bg_palette_item containing the required information to generate the palette used by the background.; devuelve: The requested palette_bitmap_bg_ptr.
- **create** `static palette_bitmap_bg_ptr create(const bg_palette_ptr& palette)` (L65): Creates a palette_bitmap_bg_ptr from the given bg_palette_ptr.
  `palette` bg_palette_ptr reference.; devuelve: The requested palette_bitmap_bg_ptr.
- **create** `static palette_bitmap_bg_ptr create(bg_palette_ptr&& palette)` (L72): Creates a palette_bitmap_bg_ptr from a moved bg_palette_ptr.
  `palette` bg_palette_ptr to move.; devuelve: The requested palette_bitmap_bg_ptr.
- **create** `static palette_bitmap_bg_ptr create(const palette_bitmap_bg_builder& builder)` (L79): Creates a palette_bitmap_bg_ptr from a palette_bitmap_bg_builder reference.
  `builder` palette_bitmap_bg_builder reference.; devuelve: The requested palette_bitmap_bg_ptr.
- **create** `static palette_bitmap_bg_ptr create(palette_bitmap_bg_builder&& builder)` (L86): Creates a palette_bitmap_bg_ptr from a moved palette_bitmap_bg_builder.
  `builder` palette_bitmap_bg_builder to move.; devuelve: The requested palette_bitmap_bg_ptr.
- **create_optional** `static optional<palette_bitmap_bg_ptr> create_optional(const bg_palette_item& palette_item)` (L94): Creates a palette_bitmap_bg_ptr from the given bg_palette_item.
  `palette_item` bg_palette_item containing the required information to generate the palette used by the background.; devuelve: The requested palette_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<palette_bitmap_bg_ptr> create_optional(const bg_palette_ptr& palette)` (L101): Creates a palette_bitmap_bg_ptr from the given bg_palette_ptr.
  `palette` bg_palette_ptr reference.; devuelve: The requested palette_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<palette_bitmap_bg_ptr> create_optional(bg_palette_ptr&& palette)` (L108): Creates a palette_bitmap_bg_ptr from a moved bg_palette_ptr.
  `palette` bg_palette_ptr to move.; devuelve: The requested palette_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<palette_bitmap_bg_ptr> create_optional(const palette_bitmap_bg_builder& builder)` (L115): Creates a palette_bitmap_bg_ptr from a palette_bitmap_bg_builder reference.
  `builder` palette_bitmap_bg_builder reference.; devuelve: The requested palette_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<palette_bitmap_bg_ptr> create_optional(palette_bitmap_bg_builder&& builder)` (L122): Creates a palette_bitmap_bg_ptr from a moved palette_bitmap_bg_builder.
  `builder` palette_bitmap_bg_builder to move.; devuelve: The requested palette_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **palette_bitmap_bg_ptr** `palette_bitmap_bg_ptr(const palette_bitmap_bg_ptr& other)` (L128): Copy constructor.
  `other` palette_bitmap_bg_ptr to copy.
- **operator=** `palette_bitmap_bg_ptr& operator=(const palette_bitmap_bg_ptr& other)` (L135): Copy assignment operator.
  `other` palette_bitmap_bg_ptr to copy.; devuelve: Reference to this.
- **palette_bitmap_bg_ptr** `palette_bitmap_bg_ptr(palette_bitmap_bg_ptr&& other) noexcept : palette_bitmap_bg_ptr(other._handle)` (L141): Move constructor.
  `other` palette_bitmap_bg_ptr to move.
- **operator=** `palette_bitmap_bg_ptr& operator=(palette_bitmap_bg_ptr&& other) noexcept` (L152): Move assignment operator.
  `other` palette_bitmap_bg_ptr to move.; devuelve: Reference to this.
- **~palette_bitmap_bg_ptr** `~palette_bitmap_bg_ptr()` (L161): Releases the referenced background if no more palette_bitmap_bg_ptr objects reference to it.
- **page** `span<const uint16_t> page() const` (L166): Returns a span of the hidden frame buffer.
- **page** `span<uint16_t> page()` (L171): Returns a span of the hidden frame buffer.
- **flip_page_later** `void flip_page_later()` (L176): Swaps the hidden frame buffer in the next core::update call.
- **flip_page_now** `void flip_page_now()` (L183): Swaps the hidden frame buffer without waiting to the next core::update call.
  Expect lack of vsync issues like screen tearing.
- **palette** `const bg_palette_ptr& palette() const` (L188): Returns the color palette used by this background.
- **set_palette** `void set_palette(const bg_palette_ptr& palette)` (L194): Sets the color palette to use by this background.
  `palette` bg_palette_ptr to copy.
- **set_palette** `void set_palette(bg_palette_ptr&& palette)` (L200): Sets the color palette to use by this background.
  `palette` bg_palette_ptr to move.
- **set_palette** `void set_palette(const bg_palette_item& palette_item)` (L210): Replaces the color palette used by this background with a new one created with the given bg_palette_item.
  `palette_item` It creates the color palette to use by this background.
  Before creating a new color palette, the bg_palette_ptr used by this background is removed,
  so VRAM usage is reduced.
- **x** `fixed x() const` (L215): Returns the horizontal position of the background (relative to its camera, if it has one).
- **set_x** `void set_x(fixed x)` (L220): Sets the horizontal position of the background (relative to its camera, if it has one).
- **y** `fixed y() const` (L225): Returns the vertical position of the background (relative to its camera, if it has one).
- **set_y** `void set_y(fixed y)` (L230): Sets the vertical position of the background (relative to its camera, if it has one).
- **position** `const fixed_point& position() const` (L235): Returns the position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(fixed x, fixed y)` (L242): Sets the position of the background (relative to its camera, if it has one).
  `x` Horizontal position of the background (relative to its camera, if it has one).; `y` Vertical position of the background (relative to its camera, if it has one).
- **set_position** `void set_position(const fixed_point& position)` (L247): Sets the position of the background (relative to its camera, if it has one).
- **top_left_x** `fixed top_left_x() const` (L252): Returns the horizontal top-left position of the background (relative to its camera, if it has one).
- **set_top_left_x** `void set_top_left_x(fixed top_left_x)` (L257): Sets the horizontal top-left position of the background (relative to its camera, if it has one).
- **top_left_y** `fixed top_left_y() const` (L262): Returns the vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_y** `void set_top_left_y(fixed top_left_y)` (L267): Sets the vertical top-left position of the background (relative to its camera, if it has one).
- **top_left_position** `fixed_point top_left_position() const` (L272): Returns the top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(fixed top_left_x, fixed top_left_y)` (L279): Sets the top-left position of the background (relative to its camera, if it has one).
  `top_left_x` Horizontal top-left position of the background (relative to its camera, if it has one).; `top_left_y` Vertical top-left position of the background (relative to its camera, if it has one).
- **set_top_left_position** `void set_top_left_position(const fixed_point& top_left_position)` (L284): Sets the top-left position of the background (relative to its camera, if it has one).
- **rotation_angle** `fixed rotation_angle() const` (L289): Returns the rotation angle in degrees of the background.
- **set_rotation_angle** `void set_rotation_angle(fixed rotation_angle)` (L295): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].
- **set_rotation_angle_safe** `void set_rotation_angle_safe(fixed rotation_angle)` (L301): Sets the rotation angle in degrees of the background.
  `rotation_angle` Rotation angle in degrees, in any range.
- **horizontal_scale** `fixed horizontal_scale() const` (L306): Returns the horizontal scale of the background.
- **set_horizontal_scale** `void set_horizontal_scale(fixed horizontal_scale)` (L311): Sets the horizontal scale of the background.
- **vertical_scale** `fixed vertical_scale() const` (L316): Returns the vertical scale of the background.
- **set_vertical_scale** `void set_vertical_scale(fixed vertical_scale)` (L321): Sets the vertical scale of the background.
- **set_scale** `void set_scale(fixed scale)` (L326): Sets the scale of the background.
- **set_scale** `void set_scale(fixed horizontal_scale, fixed vertical_scale)` (L333): Sets the scale of the background.
  `horizontal_scale` Horizontal scale.; `vertical_scale` Vertical scale.
- **horizontal_shear** `fixed horizontal_shear() const` (L338): Returns the horizontal shear of the background.
- **set_horizontal_shear** `void set_horizontal_shear(fixed horizontal_shear)` (L343): Sets the horizontal shear of the background.
- **vertical_shear** `fixed vertical_shear() const` (L348): Returns the vertical shear of the background.
- **set_vertical_shear** `void set_vertical_shear(fixed vertical_shear)` (L353): Sets the vertical shear of the background.
- **set_shear** `void set_shear(fixed shear)` (L358): Sets the shear of the background.
- **set_shear** `void set_shear(fixed horizontal_shear, fixed vertical_shear)` (L365): Sets the shear of the background.
  `horizontal_shear` Horizontal shear.; `vertical_shear` Vertical shear.
- **horizontal_flip** `bool horizontal_flip() const` (L370): Indicates if this background is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L375): Sets if this background must be flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L380): Indicates if this background must be flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L385): Sets if this background must be flipped in the vertical axis or not.
- **pivot_x** `fixed pivot_x() const` (L390): Returns the horizontal position of the affine transformations pivot point of this background.
- **set_pivot_x** `void set_pivot_x(fixed pivot_x)` (L395): Sets the horizontal position of the affine transformations pivot point of this background.
- **pivot_y** `fixed pivot_y() const` (L400): Returns the vertical position of the affine transformations pivot point of this background.
- **set_pivot_y** `void set_pivot_y(fixed pivot_y)` (L405): Sets the vertical position of the affine transformations pivot point of this background.
- **pivot_position** `const fixed_point& pivot_position() const` (L410): Returns the position of the affine transformations pivot point of this background.
- **set_pivot_position** `void set_pivot_position(fixed pivot_x, fixed pivot_y)` (L417): Sets the position of the affine transformations pivot point of this background.
  `pivot_x` Horizontal position of the pivot point.; `pivot_y` Vertical position of the pivot point.
- **set_pivot_position** `void set_pivot_position(const fixed_point& pivot_position)` (L423): Sets the position of the affine transformations pivot point of this background.
  `pivot_position` Position of the pivot point.
- **mat_attributes** `const affine_bg_mat_attributes& mat_attributes() const` (L428): Returns the attributes of the position and the transformation matrix of this background.
- **set_mat_attributes** `void set_mat_attributes(const affine_mat_attributes& mat_attributes)` (L433): Sets the attributes of the affine transformation matrix of this background.
- **priority** `int priority() const` (L441): Returns the priority of the background relative to sprites and other backgrounds.
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **set_priority** `void set_priority(int priority)` (L451): Sets the priority of the background relative to sprites and other backgrounds.
  `priority` Priority in the range [0..3].
  Backgrounds with higher priority are drawn first
  (and therefore can be covered by later sprites and backgrounds).
- **mosaic_enabled** `bool mosaic_enabled() const` (L456): Indicates if the mosaic effect must be applied to this background or not.
- **set_mosaic_enabled** `void set_mosaic_enabled(bool mosaic_enabled)` (L461): Sets if the mosaic effect must be applied to this background or not.
- **blending_enabled** `bool blending_enabled() const` (L468): Indicates if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **set_blending_enabled** `void set_blending_enabled(bool blending_enabled)` (L478): Sets if blending must be applied to this background or not.
  Blending is applied to this background by making it part of the blending top layer.
- **blending_top_enabled** `bool blending_top_enabled() const` (L486): Indicates if this background is part of the blending top layer or not.
- **set_blending_top_enabled** `void set_blending_top_enabled(bool blending_top_enabled)` (L491): Sets if this background is part of the blending top layer or not.
- **blending_bottom_enabled** `bool blending_bottom_enabled() const` (L496): Indicates if this background is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L501): Sets if this background is part of the blending bottom layer or not.
- **green_swap_mode** `bn::green_swap_mode green_swap_mode() const` (L506): Indicates how this background must be displayed when green swap is enabled.
- **set_green_swap_mode** `void set_green_swap_mode(bn::green_swap_mode green_swap_mode)` (L511): Sets how this background must be displayed when green swap is enabled.
- **visible** `bool visible() const` (L516): Indicates if this background must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L521): Sets if this background must be committed to the GBA or not.
- **visible_in_window** `bool visible_in_window(const window& window) const` (L526): Indicates if this background is visible in the given window or not.
- **set_visible_in_window** `void set_visible_in_window(bool visible, window& window)` (L531): Sets if this background must be visible in the given window or not.
- **camera** `const optional<camera_ptr>& camera() const` (L536): Returns the camera_ptr attached to this background (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L542): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to copy to this background.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L548): Sets the camera_ptr attached to this background.
  `camera` camera_ptr to move to this background.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L554): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to copy to this background.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L560): Sets or removes the camera_ptr attached to this background.
  `camera` Optional camera_ptr to move to this background.
- **remove_camera** `void remove_camera()` (L565): Removes the camera_ptr attached to this background (if any).
- **attributes** `bitmap_bg_attributes attributes() const` (L570): Returns the attributes to commit to the GBA for this background.
- **set_attributes** `void set_attributes(const bitmap_bg_attributes& attributes)` (L575): Sets the attributes to commit to the GBA for this background.
- **hw_id** `optional<int> hw_id() const` (L589): Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
  Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
  
  A background doesn't have an assigned hardware ID if it is not visible.
  
  Assigned hardware ID can change after calling this method
  if some properties of this background or others are updated.
  
  Call this method at your own risk.
- **handle** `const void* handle() const` (L594): Returns the internal handle.
- **swap** `void swap(palette_bitmap_bg_ptr& other)` (L603): Exchanges the contents of this palette_bitmap_bg_ptr with those of the other one.
  `other` palette_bitmap_bg_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_ptr& a, palette_bitmap_bg_ptr& b)` (L613): Exchanges the contents of an palette_bitmap_bg_ptr with those of another one.
  `a` First palette_bitmap_bg_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const palette_bitmap_bg_ptr& a, const palette_bitmap_bg_ptr& b) = default` (L621): Default equal operator.

## bn_palette_bitmap_roi.h

### `bn::palette_bitmap_roi` (class, bn_palette_bitmap_roi.h:28)

Contains the required information to represent a region of interest (RoI) of a palette bitmap pixels item.

`class palette_bitmap_roi`

The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.

- **palette_bitmap_roi** `constexpr palette_bitmap_roi( const palette_bitmap_pixels_item& item_ref, int x, int y, int width, int height) : palette_bitmap_roi(item_ref, point(x, y), size(width, height))` (L43): Constructor.
  `item_ref` Reference to the palette bitmap pixels item.; `x` Horizontal position of the top-left corner of the region of interest.; `y` Vertical position of the top-left corner of the region of interest.; `width` Width in pixels of the region of interest.; `height` Height in pixels of the region of interest.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **palette_bitmap_roi** `constexpr palette_bitmap_roi( const palette_bitmap_pixels_item& item_ref, const point& position, const size& dimensions)` (L58): Constructor.
  `item_ref` Reference to the palette bitmap pixels item.; `position` Position of the top-left corner of the region of interest.; `dimensions` Size in pixels of the region of interest.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **item_ref** `constexpr const palette_bitmap_pixels_item& item_ref() const` (L79): Returns the referenced palette bitmap pixels item.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **x** `constexpr int x() const` (L87): Returns the horizontal position of the top-left corner of the region of interest.
- **y** `constexpr int y() const` (L95): Returns the vertical position of the top-left corner of the region of interest.
- **position** `constexpr const point& position() const` (L103): Returns the position of the top-left corner of the region of interest.
- **width** `constexpr int width() const` (L111): Returns the width in pixels of the region of interest.
- **height** `constexpr int height() const` (L119): Returns the height in pixels of the region of interest.
- **dimensions** `constexpr const size& dimensions() const` (L127): Returns the size in pixels of the region of interest.
- **pixels_count** `constexpr int pixels_count() const` (L135): Returns the number of referenced pixels.
- **pixel_index** `constexpr int pixel_index(int x, int y) const` (L149): Returns the index of the referenced pixel in the specified pixel coordinates.
  `x` Horizontal position of the palette bitmap pixel [0..dimensions().width()).; `y` Vertical position of the palette bitmap pixel [0..dimensions().height()).; devuelve: The index of the referenced palette bitmap pixel.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **pixel_index** `constexpr int pixel_index(const point& position) const` (L168): Returns the index of the referenced pixel in the specified pixel coordinates.
  `position` Position of the palette bitmap pixel.; devuelve: The index of the referenced palette bitmap pixel.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **pixel** `constexpr uint8_t pixel(int x, int y) const` (L182): Returns the referenced palette bitmap pixel in the specified pixel coordinates.
  `x` Horizontal position of the palette bitmap pixel [0..dimensions().width()).; `y` Vertical position of the palette bitmap pixel [0..dimensions().height()).; devuelve: The referenced palette bitmap pixel.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **pixel** `constexpr uint8_t pixel(const point& position) const` (L195): Returns the referenced palette bitmap pixel in the specified pixel coordinates.
  `position` Position of the palette bitmap pixel.; devuelve: The referenced palette bitmap pixel.
  The item is not copied but referenced, so it should outlive the palette_bitmap_roi to avoid dangling references.
- **operator==** `constexpr friend bool operator==( const palette_bitmap_roi& a, const palette_bitmap_roi& b) = default` (L203): Default equal operator.
