# Butano 21.10.1 — Background maps (`bg_map`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [bg](bg.md)

## bn_affine_bg_big_map_canvas_size.h

### `bn::affine_bg_big_map_canvas_size` (enum, bn_affine_bg_big_map_canvas_size.h:26)

Available affine background map canvas sizes enum.

`enum class affine_bg_big_map_canvas_size : uint8_t { NORMAL /* 256x256 pixels. */, BIG /* 512x512 pixels. */, HUGE /* 1024x1024 pixels. */ }`


## bn_affine_bg_map_cell_info.h

### `bn::affine_bg_map_cell_info` (class, bn_affine_bg_map_cell_info.h:26)

Manages the properties of an affine background map cell.

`class affine_bg_map_cell_info`

- **affine_bg_map_cell_info** `constexpr affine_bg_map_cell_info() = default` (L33): Default constructor.
- **affine_bg_map_cell_info** `constexpr explicit affine_bg_map_cell_info(affine_bg_map_cell cell)` (L39): Constructor.
  `cell` Managed affine_bg_map_cell.
- **cell** `constexpr affine_bg_map_cell cell() const` (L47): Returns the managed affine_bg_map_cell.
- **set_cell** `constexpr void set_cell(affine_bg_map_cell cell)` (L55): Sets the managed affine_bg_map_cell.
- **tile_index** `constexpr int tile_index() const` (L63): Returns the referenced tile index.
- **set_tile_index** `constexpr void set_tile_index(int tile_index)` (L71): Sets the referenced tile index.
- **operator==** `constexpr friend bool operator==(affine_bg_map_cell_info a, affine_bg_map_cell_info b) = default` (L81): Default equal operator.

## bn_affine_bg_map_ptr.h

### `bn::affine_bg_map_ptr` (class, bn_affine_bg_map_ptr.h:44)

std::shared_ptr like smart pointer that retains shared ownership of an affine background map.

`class affine_bg_map_ptr`

Several affine_bg_map_ptr objects may own the same affine background.

The affine background map is released when the last remaining affine_bg_map_ptr owning it is destroyed.

- **find** `static optional<affine_bg_map_ptr> find( const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)` (L56): Searches for an affine_bg_map_ptr which references the given information.
  `map_item` affine_bg_map_item which references the map cells to search.; `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; devuelve: affine_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<affine_bg_map_ptr> find( const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index)` (L68): Searches for an affine_bg_map_ptr which references the given information.
  `map_item` affine_bg_map_item which references the map cells to search.; `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; `map_index` Index of the referenced map to search.; devuelve: affine_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<affine_bg_map_ptr> find(const affine_bg_item& item)` (L78): Searches for an affine_bg_map_ptr which references the given information.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search.; devuelve: affine_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<affine_bg_map_ptr> find(const affine_bg_item& item, int map_index)` (L87): Searches for an affine_bg_map_ptr which references the given information.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search.; `map_index` Index of the referenced map to search.; devuelve: affine_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **create** `static affine_bg_map_ptr create( const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L102): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_item` affine_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create** `static affine_bg_map_ptr create( const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)` (L119): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_item` affine_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create** `static affine_bg_map_ptr create(const affine_bg_item& item)` (L134): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create** `static affine_bg_map_ptr create(const affine_bg_item& item, int map_index)` (L149): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **allocate** `static affine_bg_map_ptr allocate( const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L182): Creates an affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
  `dimensions` Size in map cells of the map to allocate.; `tiles` Referenced tiles of the map to allocate.; `palette` Referenced color palette of the map to allocate.; devuelve: affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
- **create_optional** `static optional<affine_bg_map_ptr> create_optional( const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L199): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_item` affine_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_map_ptr> create_optional( const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)` (L217): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_item` affine_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_map_ptr> create_optional(const affine_bg_item& item)` (L233): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_map_ptr> create_optional(const affine_bg_item& item, int map_index)` (L249): Searches for an affine_bg_map_ptr which references the given information. If it is not found, it creates an affine_bg_map_ptr which references it.
  `item` affine_bg_item which references the tiles, the color palette and the map cells to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the given information if it has been found; otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **allocate_optional** `static optional<affine_bg_map_ptr> allocate_optional( const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L283): Creates an affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
  `dimensions` Size in map cells of the map to allocate.; `tiles` Referenced tiles of the map to allocate.; `palette` Referenced color palette of the map to allocate.; devuelve: affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
- **affine_bg_map_ptr** `affine_bg_map_ptr(const affine_bg_map_ptr& other)` (L290): Copy constructor.
  `other` affine_bg_map_ptr to copy.
- **operator=** `affine_bg_map_ptr& operator=(const affine_bg_map_ptr& other)` (L297): Copy assignment operator.
  `other` affine_bg_map_ptr to copy.; devuelve: Reference to this.
- **affine_bg_map_ptr** `affine_bg_map_ptr(affine_bg_map_ptr&& other) noexcept : affine_bg_map_ptr(other._handle)` (L303): Move constructor.
  `other` affine_bg_map_ptr to move.
- **operator=** `affine_bg_map_ptr& operator=(affine_bg_map_ptr&& other) noexcept` (L314): Move assignment operator.
  `other` affine_bg_map_ptr to move.; devuelve: Reference to this.
- **~affine_bg_map_ptr** `~affine_bg_map_ptr()` (L323): Releases the referenced map cells if no more affine_bg_map_ptr objects reference to them.
- **id** `int id() const` (L328): Returns the internal id.
- **dimensions** `size dimensions() const` (L333): Returns the size in map cells of the referenced map.
- **big** `bool big() const` (L340): Indicates if this map is big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **big_canvas_size** `affine_bg_big_map_canvas_size big_canvas_size() const` (L345): Returns the size of the canvas used to render this big map.
- **tiles_offset** `int tiles_offset() const` (L350): Returns how many tiles to offset in the cells of this map before writing them in VRAM.
- **compression** `compression_type compression() const` (L355): Returns the compression of the referenced map cells.
- **cells_ref** `optional<span<const affine_bg_map_cell>> cells_ref() const` (L361): Returns the referenced map cells unless it was created with allocate or allocate_optional. In that case, it returns bn::nullopt.
- **set_cells_ref** `void set_cells_ref(const affine_bg_map_item& map_item)` (L373): Sets the map cells to handle.
  `map_item` affine_bg_map_item which references the map cells to set.
  The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
  
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **set_cells_ref** `void set_cells_ref(const affine_bg_map_item& map_item, int map_index)` (L386): Sets the map cells to handle.
  `map_item` affine_bg_map_item which references the map cells to set.; `map_index` Index of the referenced map to set.
  The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
  
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **reload_cells_ref** `void reload_cells_ref()` (L391): Uploads the referenced map cells to VRAM again to make visible the possible changes in them.
- **tiles** `const affine_bg_tiles_ptr& tiles() const` (L396): Returns the referenced tiles.
- **set_tiles** `void set_tiles(const affine_bg_tiles_ptr& tiles)` (L402): Sets the referenced tiles.
  `tiles` affine_bg_tiles_ptr to copy.
- **set_tiles** `void set_tiles(affine_bg_tiles_ptr&& tiles)` (L408): Sets the referenced tiles.
  `tiles` affine_bg_tiles_ptr to move.
- **set_tiles** `void set_tiles(const affine_bg_tiles_item& tiles_item)` (L421): Replaces the referenced tiles with a new tile set created with the given affine_bg_tiles_item.
  `tiles_item` It creates the new background tiles to reference.
  Before creating a new background tile set, the affine_bg_tiles_ptr referenced by this map is removed,
  so VRAM usage is reduced.
  
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if this map is dynamic (managed by yourself).
- **set_tiles** `void set_tiles(const affine_bg_tiles_item& tiles_item, bool allow_offset)` (L433): Replaces the referenced tiles with a new tile set created with the given affine_bg_tiles_item.
  `tiles_item` It creates the new background tiles to reference.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it this map is dynamic (managed by yourself).
  Before creating a new background tile set, the affine_bg_tiles_ptr referenced by this map is removed,
  so VRAM usage is reduced.
- **palette** `const bg_palette_ptr& palette() const` (L438): Returns the referenced color palette.
- **set_palette** `void set_palette(const bg_palette_ptr& palette)` (L444): Sets the referenced color palette.
  `palette` bg_palette_ptr to copy.
- **set_palette** `void set_palette(bg_palette_ptr&& palette)` (L450): Sets the referenced color palette.
  `palette` bg_palette_ptr to move.
- **set_palette** `void set_palette(const bg_palette_item& palette_item)` (L460): Replaces the referenced color palette with a new tile set created with the given bg_palette_item.
  `palette_item` It creates the new color palette to reference.
  Before creating a new color palette, the bg_palette_ptr referenced by this map is removed,
  so VRAM usage is reduced.
- **vram** `optional<span<const affine_bg_map_cell>> vram() const` (L466): Returns the allocated memory in VRAM if this affine_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
- **vram** `optional<span<affine_bg_map_cell>> vram()` (L472): Returns the allocated memory in VRAM if this affine_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
- **handle** `int handle() const` (L477): Returns the internal handle.
- **swap** `void swap(affine_bg_map_ptr& other)` (L486): Exchanges the contents of this affine_bg_map_ptr with those of the other one.
  `other` affine_bg_map_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_map_ptr& a, affine_bg_map_ptr& b)` (L496): Exchanges the contents of an affine_bg_map_ptr with those of another one.
  `a` First affine_bg_map_ptr to exchange the contents with.; `b` Second affine_bg_map_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const affine_bg_map_ptr& a, const affine_bg_map_ptr& b) = default` (L504): Default equal operator.

## bn_bg_maps.h

### `bn::bg_maps` (namespace, bn_bg_maps.h:28)

Background maps related functions.

- **used_cells_count** `int used_cells_count()` (L33): Returns the number of used background map cells.
- **available_cells_count** `int available_cells_count()` (L38): Returns the number of available background map cells.
- **used_blocks_count** `int used_blocks_count()` (L43): Returns the number of used background VRAM blocks by background maps.
- **available_blocks_count** `int available_blocks_count()` (L48): Returns the number of available background VRAM blocks.
- **new_affine_big_map_canvas_size** `affine_bg_big_map_canvas_size new_affine_big_map_canvas_size()` (L53): Returns the size of the canvas used to create big affine background maps.
- **set_new_affine_big_map_canvas_size** `void set_new_affine_big_map_canvas_size(affine_bg_big_map_canvas_size affine_big_map_canvas_size)` (L58): Sets the size of the canvas used to create big affine background maps.
- **log_status** `void log_status()` (L63): Logs the current status of the background blocks manager.

## bn_bg_palette_actions.h

### `bn::bg_palette_inverted_manager` (class, bn_bg_palette_actions.h:32)

Manages if the colors of a bg_palette_ptr must be inverted or not.

`class bg_palette_inverted_manager`

- **get** `static bool get(const bg_palette_ptr& palette)` (L39): Indicates if the colors of the given bg_palette_ptr are inverted or not.
- **set** `static void set(bool inverted, bg_palette_ptr& palette)` (L47): Sets if the colors of the given bg_palette_ptr must be inverted or not.
### `bn::bg_palette_inverted_toggle_action` (class, bn_bg_palette_actions.h:62)

Toggles if the colors of a bg_palette_ptr must be inverted or not when the action is updated a given number of times.

`class bg_palette_inverted_toggle_action : public bool_toggle_value_template_action<bg_palette_ptr, bg_palette_inverted_manager>`

- **bg_palette_inverted_toggle_action** `bg_palette_inverted_toggle_action(const bg_palette_ptr& palette, int duration_updates) : bool_toggle_value_template_action(palette, duration_updates)` (L73): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the colors of the given bg_palette_ptr must be inverted or not.
- **bg_palette_inverted_toggle_action** `bg_palette_inverted_toggle_action(bg_palette_ptr&& palette, int duration_updates) : bool_toggle_value_template_action(move(palette), duration_updates)` (L84): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the colors of the given bg_palette_ptr must be inverted or not.
- **palette** `const bg_palette_ptr& palette() const` (L92): Returns the bg_palette_ptr to modify.
### `bn::bg_palette_grayscale_manager` (class, bn_bg_palette_actions.h:108)

Manages the intensity of the grayscale effect applied to a bg_palette_ptr.

`class bg_palette_grayscale_manager`

- **get** `static fixed get(const bg_palette_ptr& palette)` (L115): Returns the intensity of the grayscale effect applied to the given bg_palette_ptr.
- **set** `static void set(fixed intensity, bg_palette_ptr& palette)` (L125): Sets the intensity of the grayscale effect applied to the given bg_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` bg_palette_ptr to modify.
### `bn::bg_palette_grayscale_to_action` (class, bn_bg_palette_actions.h:140)

Modifies the intensity of the grayscale effect applied to a bg_palette_ptr until it has a given state.

`class bg_palette_grayscale_to_action : public to_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>`

- **bg_palette_grayscale_to_action** `bg_palette_grayscale_to_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L154): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_grayscale_to_action** `bg_palette_grayscale_to_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L169): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L178): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L186): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_grayscale_loop_action` (class, bn_bg_palette_actions.h:202)

Modifies the intensity of the grayscale effect applied to a bg_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palette_grayscale_loop_action : public loop_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>`

- **bg_palette_grayscale_loop_action** `bg_palette_grayscale_loop_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L217): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **bg_palette_grayscale_loop_action** `bg_palette_grayscale_loop_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L233): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L242): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L251): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palette_grayscale_toggle_action` (class, bn_bg_palette_actions.h:266)

Changes the intensity of the grayscale effect applied to a bg_palette_ptr when the action is updated a given number of times.

`class bg_palette_grayscale_toggle_action : public toggle_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>`

- **bg_palette_grayscale_toggle_action** `bg_palette_grayscale_toggle_action(const bg_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L279): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_grayscale_toggle_action** `bg_palette_grayscale_toggle_action(bg_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L293): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L302): Returns the bg_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L310): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_hue_shift_manager` (class, bn_bg_palette_actions.h:326)

Manages the intensity of the hue shift effect applied to a bg_palette_ptr.

`class bg_palette_hue_shift_manager`

- **get** `static fixed get(const bg_palette_ptr& palette)` (L333): Returns the intensity of the hue shift effect applied to the given bg_palette_ptr.
- **set** `static void set(fixed intensity, bg_palette_ptr& palette)` (L343): Sets the intensity of the hue shift effect applied to the given bg_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` bg_palette_ptr to modify.
### `bn::bg_palette_hue_shift_to_action` (class, bn_bg_palette_actions.h:358)

Modifies the intensity of the hue shift effect applied to a bg_palette_ptr until it has a given state.

`class bg_palette_hue_shift_to_action : public to_value_template_action<bg_palette_ptr, fixed, bg_palette_hue_shift_manager>`

- **bg_palette_hue_shift_to_action** `bg_palette_hue_shift_to_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L372): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_hue_shift_to_action** `bg_palette_hue_shift_to_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L387): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L396): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L404): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_hue_shift_loop_action` (class, bn_bg_palette_actions.h:420)

Modifies the intensity of the hue shift effect applied to a bg_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palette_hue_shift_loop_action : public loop_value_template_action<bg_palette_ptr, fixed, bg_palette_hue_shift_manager>`

- **bg_palette_hue_shift_loop_action** `bg_palette_hue_shift_loop_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L435): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **bg_palette_hue_shift_loop_action** `bg_palette_hue_shift_loop_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L451): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L460): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L469): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palette_hue_shift_toggle_action` (class, bn_bg_palette_actions.h:484)

Changes the intensity of the hue shift effect applied to a bg_palette_ptr when the action is updated a given number of times.

`class bg_palette_hue_shift_toggle_action : public toggle_value_template_action<bg_palette_ptr, fixed, bg_palette_hue_shift_manager>`

- **bg_palette_hue_shift_toggle_action** `bg_palette_hue_shift_toggle_action(const bg_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L497): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_hue_shift_toggle_action** `bg_palette_hue_shift_toggle_action(bg_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L511): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L520): Returns the bg_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L528): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_fade_manager` (class, bn_bg_palette_actions.h:544)

Manages the intensity of the fade effect applied to a bg_palette_ptr.

`class bg_palette_fade_manager`

- **get** `static fixed get(const bg_palette_ptr& palette)` (L551): Returns the intensity of the fade effect applied to the given bg_palette_ptr.
- **set** `static void set(fixed intensity, bg_palette_ptr& palette)` (L561): Sets the intensity of the fade effect applied to the given bg_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` bg_palette_ptr to modify.
### `bn::bg_palette_fade_to_action` (class, bn_bg_palette_actions.h:576)

Modifies the intensity of the fade effect applied to a bg_palette_ptr until it has a given state.

`class bg_palette_fade_to_action : public to_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>`

- **bg_palette_fade_to_action** `bg_palette_fade_to_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L589): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_fade_to_action** `bg_palette_fade_to_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L604): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L613): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L621): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_fade_loop_action` (class, bn_bg_palette_actions.h:637)

Modifies the intensity of the fade effect applied to a bg_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palette_fade_loop_action : public loop_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>`

- **bg_palette_fade_loop_action** `bg_palette_fade_loop_action(const bg_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L652): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **bg_palette_fade_loop_action** `bg_palette_fade_loop_action(bg_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L668): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L677): Returns the bg_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L686): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palette_fade_toggle_action` (class, bn_bg_palette_actions.h:701)

Changes the intensity of the fade effect applied to a bg_palette_ptr when the action is updated a given number of times.

`class bg_palette_fade_toggle_action : public toggle_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>`

- **bg_palette_fade_toggle_action** `bg_palette_fade_toggle_action(const bg_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L714): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **bg_palette_fade_toggle_action** `bg_palette_fade_toggle_action(bg_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L728): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const bg_palette_ptr& palette() const` (L737): Returns the bg_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L745): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palette_rotate_manager` (class, bn_bg_palette_actions.h:761)

Manages the number of colors to rotate to the right in a bg_palette_ptr.

`class bg_palette_rotate_manager`

- **get** `static int get(const bg_palette_ptr& palette)` (L768): Returns the number of colors to rotate to the right in the given bg_palette_ptr.
- **set** `static void set(int count, bg_palette_ptr& palette)` (L779): Sets the number of colors to rotate to the right in the given bg_palette_ptr.
  `count` Number of colors to rotate to the right in the range [1 - rotate_range_size() .. rotate_range_size() - 1].; `palette` bg_palette_ptr to modify.
### `bn::bg_palette_rotate_by_action` (class, bn_bg_palette_actions.h:795)

Modifies the number of colors to rotate to the right in a bg_palette_ptr by delta_count when the action is updated a given number of times. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`class bg_palette_rotate_by_action : public cyclic_duration_by_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>`

- **bg_palette_rotate_by_action** `bg_palette_rotate_by_action(const bg_palette_ptr& palette, int duration_updates, int delta_count) : cyclic_duration_by_value_template_action(palette, duration_updates, delta_count, 0, 0)` (L808): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before updating the number of colors to rotate.; `delta_count` How many colors to add to the number of colors to rotate to the right when the action is updated duration_updates times.
- **bg_palette_rotate_by_action** `bg_palette_rotate_by_action(bg_palette_ptr&& palette, int duration_updates, int delta_count) : cyclic_duration_by_value_template_action(move(palette), duration_updates, delta_count, 0, 0)` (L826): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated before updating the number of colors to rotate.; `delta_count` How many colors to add to the number of colors to rotate to the right when the action is updated duration_updates times.
- **palette** `const bg_palette_ptr& palette() const` (L839): Returns the bg_palette_ptr to modify.
- **delta_count** `int delta_count() const` (L848): Returns how many colors to add to the number of colors to rotate to the right when the action is updated the given number of times.
### `bn::bg_palette_rotate_to_action` (class, bn_bg_palette_actions.h:862)

Modifies the number of colors to rotate to the right in a bg_palette_ptr until it has a given state.

`class bg_palette_rotate_to_action : public to_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>`

- **bg_palette_rotate_to_action** `bg_palette_rotate_to_action(const bg_palette_ptr& palette, int duration_updates, int final_count) : to_value_template_action(palette, duration_updates, final_count)` (L876): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the number of colors to rotate to the right is equal to final_count.; `final_count` Number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **bg_palette_rotate_to_action** `bg_palette_rotate_to_action(bg_palette_ptr&& palette, int duration_updates, int final_count) : to_value_template_action(move(palette), duration_updates, final_count)` (L893): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the number of colors to rotate to the right is equal to final_count.; `final_count` Number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const bg_palette_ptr& palette() const` (L903): Returns the bg_palette_ptr to modify.
- **final_count** `int final_count() const` (L912): Returns the number of colors to rotate to the right when the action is updated the given number of times.
### `bn::bg_palette_rotate_loop_action` (class, bn_bg_palette_actions.h:928)

Modifies the number of colors to rotate to the right in a bg_palette_ptr from a minimum to a maximum. When the number of colors to rotate to the right is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palette_rotate_loop_action : public loop_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>`

- **bg_palette_rotate_loop_action** `bg_palette_rotate_loop_action(const bg_palette_ptr& palette, int duration_updates, int final_count) : loop_value_template_action(palette, duration_updates, final_count)` (L943): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the number of colors to rotate delta.; `final_count` When the the number of colors to rotate to the right is equal to this parameter, it goes back to its initial state and vice versa.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **bg_palette_rotate_loop_action** `bg_palette_rotate_loop_action(bg_palette_ptr&& palette, int duration_updates, int final_count) : loop_value_template_action(move(palette), duration_updates, final_count)` (L960): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the number of colors to rotate delta.; `final_count` When the the number of colors to rotate to the right is equal to this parameter, it goes back to its initial state and vice versa.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const bg_palette_ptr& palette() const` (L970): Returns the bg_palette_ptr to modify.
- **final_count** `int final_count() const` (L979): When the number of colors to rotate to the right is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palette_rotate_toggle_action` (class, bn_bg_palette_actions.h:994)

Changes the number of colors to rotate to the right in a bg_palette_ptr when the action is updated a given number of times.

`class bg_palette_rotate_toggle_action : public toggle_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>`

- **bg_palette_rotate_toggle_action** `bg_palette_rotate_toggle_action(const bg_palette_ptr& palette, int duration_updates, int new_count) : toggle_value_template_action(palette, duration_updates, new_count)` (L1009): Constructor.
  `palette` bg_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the number of colors to rotate to the right.; `new_count` New number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **bg_palette_rotate_toggle_action** `bg_palette_rotate_toggle_action(bg_palette_ptr&& palette, int duration_updates, int new_count) : toggle_value_template_action(move(palette), duration_updates, new_count)` (L1026): Constructor.
  `palette` bg_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the number of colors to rotate to the right.; `new_count` New number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const bg_palette_ptr& palette() const` (L1036): Returns the bg_palette_ptr to modify.
- **new_count** `int new_count() const` (L1045): Returns the number of colors to rotate to the right when the action is updated the given number of times.

## bn_regular_bg_map_cell_info.h

### `bn::regular_bg_map_cell_info` (class, bn_regular_bg_map_cell_info.h:26)

Manages the properties of a regular background map cell.

`class regular_bg_map_cell_info`

- **regular_bg_map_cell_info** `constexpr regular_bg_map_cell_info() = default` (L33): Default constructor.
- **regular_bg_map_cell_info** `constexpr explicit regular_bg_map_cell_info(regular_bg_map_cell cell)` (L39): Constructor.
  `cell` Managed regular_bg_map_cell.
- **cell** `constexpr regular_bg_map_cell cell() const` (L47): Returns the managed regular_bg_map_cell.
- **set_cell** `constexpr void set_cell(regular_bg_map_cell cell)` (L55): Sets the managed regular_bg_map_cell.
- **tile_index** `constexpr int tile_index() const` (L63): Returns the referenced tile index.
- **set_tile_index** `constexpr void set_tile_index(int tile_index)` (L71): Sets the referenced tile index.
- **palette_id** `constexpr int palette_id() const` (L81): Returns the referenced palette id.
- **set_palette_id** `constexpr void set_palette_id(int palette_id)` (L89): Sets the referenced palette id.
- **horizontal_flip** `bool horizontal_flip() const` (L99): Indicates if the managed cell is flipped in the horizontal axis or not.
- **set_horizontal_flip** `void set_horizontal_flip(bool horizontal_flip)` (L107): Sets if the managed cell is flipped in the horizontal axis or not.
- **vertical_flip** `bool vertical_flip() const` (L115): Indicates if the managed cell is flipped in the vertical axis or not.
- **set_vertical_flip** `void set_vertical_flip(bool vertical_flip)` (L123): Sets if the managed cell is flipped in the vertical axis or not.
- **operator==** `friend bool operator==(regular_bg_map_cell_info a, regular_bg_map_cell_info b)` (L134): Equal operator.
  `a` First regular_bg_map_cell_info to compare.; `b` Second regular_bg_map_cell_info to compare.; devuelve: `true` if the first managed cell is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(regular_bg_map_cell_info a, regular_bg_map_cell_info b)` (L145): Not equal operator.
  `a` First regular_bg_map_cell_info to compare.; `b` Second regular_bg_map_cell_info to compare.; devuelve: `true` if the first managed cell is not equal to the second one, otherwise `false`.

## bn_regular_bg_map_ptr.h

### `bn::regular_bg_map_ptr` (class, bn_regular_bg_map_ptr.h:44)

std::shared_ptr like smart pointer that retains shared ownership of a regular background map.

`class regular_bg_map_ptr`

Several regular_bg_map_ptr objects may own the same regular background.

The regular background map is released when the last remaining regular_bg_map_ptr owning it is destroyed.

- **find** `static optional<regular_bg_map_ptr> find( const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)` (L56): Searches for a regular_bg_map_ptr which references the given information.
  `map_item` regular_bg_map_item which references the map cells to search.; `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; devuelve: regular_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<regular_bg_map_ptr> find( const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index)` (L68): Searches for a regular_bg_map_ptr which references the given information.
  `map_item` regular_bg_map_item which references the map cells to search.; `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; `map_index` Index of the referenced map to search.; devuelve: regular_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<regular_bg_map_ptr> find(const regular_bg_item& item)` (L78): Searches for a regular_bg_map_ptr which references the given information.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search.; devuelve: regular_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **find** `static optional<regular_bg_map_ptr> find(const regular_bg_item& item, int map_index)` (L87): Searches for a regular_bg_map_ptr which references the given information.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search.; `map_index` Index of the referenced map to search.; devuelve: regular_bg_map_ptr which references the given information if it has been found; bn::nullopt otherwise.
- **create** `static regular_bg_map_ptr create( const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L102): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_item` regular_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create** `static regular_bg_map_ptr create( const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)` (L119): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_item` regular_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create** `static regular_bg_map_ptr create(const regular_bg_item& item)` (L134): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create** `static regular_bg_map_ptr create(const regular_bg_item& item, int map_index)` (L149): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **allocate** `static regular_bg_map_ptr allocate( const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L182): Creates a regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
  `dimensions` Size in map cells of the map to allocate.; `tiles` Referenced tiles of the map to allocate.; `palette` Referenced color palette of the map to allocate.; devuelve: regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
- **create_optional** `static optional<regular_bg_map_ptr> create_optional( const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L199): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_item` regular_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<regular_bg_map_ptr> create_optional( const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)` (L217): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_item` regular_bg_map_item which references the map cells to search or handle.; `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<regular_bg_map_ptr> create_optional(const regular_bg_item& item)` (L233): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_optional** `static optional<regular_bg_map_ptr> create_optional(const regular_bg_item& item, int map_index)` (L249): Searches for a regular_bg_map_ptr which references the given information. If it is not found, it creates a regular_bg_map_ptr which references it.
  `item` regular_bg_item which references the tiles, the color palette and the map cells to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the given information if it has been found; otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **allocate_optional** `static optional<regular_bg_map_ptr> allocate_optional( const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L284): Creates a regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
  `dimensions` Size in map cells of the map to allocate.; `tiles` Referenced tiles of the map to allocate.; `palette` Referenced color palette of the map to allocate.; devuelve: regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
- **regular_bg_map_ptr** `regular_bg_map_ptr(const regular_bg_map_ptr& other)` (L291): Copy constructor.
  `other` regular_bg_map_ptr to copy.
- **operator=** `regular_bg_map_ptr& operator=(const regular_bg_map_ptr& other)` (L298): Copy assignment operator.
  `other` regular_bg_map_ptr to copy.; devuelve: Reference to this.
- **regular_bg_map_ptr** `regular_bg_map_ptr(regular_bg_map_ptr&& other) noexcept : regular_bg_map_ptr(other._handle)` (L304): Move constructor.
  `other` regular_bg_map_ptr to move.
- **operator=** `regular_bg_map_ptr& operator=(regular_bg_map_ptr&& other) noexcept` (L315): Move assignment operator.
  `other` regular_bg_map_ptr to move.; devuelve: Reference to this.
- **~regular_bg_map_ptr** `~regular_bg_map_ptr()` (L324): Releases the referenced map cells if no more regular_bg_map_ptr objects reference to them.
- **id** `int id() const` (L329): Returns the internal id.
- **dimensions** `size dimensions() const` (L334): Returns the size in map cells of the referenced map.
- **big** `bool big() const` (L341): Indicates if this map is big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **bpp** `bpp_mode bpp() const` (L346): Returns the bits per pixel of the referenced map.
- **tiles_offset** `int tiles_offset() const` (L351): Returns how many tiles to offset in the cells of this map before writing them in VRAM.
- **palette_banks_offset** `int palette_banks_offset() const` (L356): Returns how many palette banks to offset in the cells of this map before writing them in VRAM.
- **compression** `compression_type compression() const` (L361): Returns the compression of the referenced map cells.
- **cells_ref** `optional<span<const regular_bg_map_cell>> cells_ref() const` (L367): Returns the referenced map cells unless it was created with allocate or allocate_optional. In that case, it returns bn::nullopt.
- **set_cells_ref** `void set_cells_ref(const regular_bg_map_item& map_item)` (L379): Sets the map cells to handle.
  `map_item` regular_bg_map_item which references the map cells to set.
  The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
  
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **set_cells_ref** `void set_cells_ref(const regular_bg_map_item& map_item, int map_index)` (L392): Sets the map cells to handle.
  `map_item` regular_bg_map_item which references the map cells to set.; `map_index` Index of the referenced map to set.
  The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
  
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **reload_cells_ref** `void reload_cells_ref()` (L397): Uploads the referenced map cells to VRAM again to make visible the possible changes in them.
- **tiles** `const regular_bg_tiles_ptr& tiles() const` (L402): Returns the referenced tiles.
- **set_tiles** `void set_tiles(const regular_bg_tiles_ptr& tiles)` (L410): Sets the referenced tiles.
  `tiles` regular_bg_tiles_ptr to copy.
  It must be compatible with the referenced color palette.
- **set_tiles** `void set_tiles(regular_bg_tiles_ptr&& tiles)` (L418): Sets the referenced tiles.
  `tiles` regular_bg_tiles_ptr to move.
  It must be compatible with the referenced color palette.
- **set_tiles** `void set_tiles(const regular_bg_tiles_item& tiles_item)` (L433): Replaces the referenced tiles with a new tile set created with the given regular_bg_tiles_item.
  `tiles_item` It creates the new background tiles to reference.
  Before creating a new background tile set, the regular_bg_tiles_ptr referenced by this map is removed,
  so VRAM usage is reduced.
  
  The new background tiles must be compatible with the referenced color palette.
  
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if this map is dynamic (managed by yourself).
- **set_tiles** `void set_tiles(const regular_bg_tiles_item& tiles_item, bool allow_offset)` (L447): Replaces the referenced tiles with a new tile set created with the given regular_bg_tiles_item.
  `tiles_item` It creates the new background tiles to reference.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it this map is dynamic (managed by yourself).
  Before creating a new background tile set, the regular_bg_tiles_ptr referenced by this map is removed,
  so VRAM usage is reduced.
  
  The new background tiles must be compatible with the referenced color palette.
- **palette** `const bg_palette_ptr& palette() const` (L452): Returns the referenced color palette.
- **set_palette** `void set_palette(const bg_palette_ptr& palette)` (L460): Sets the referenced color palette.
  `palette` bg_palette_ptr to copy.
  It must be compatible with the referenced tiles.
- **set_palette** `void set_palette(bg_palette_ptr&& palette)` (L468): Sets the referenced color palette.
  `palette` bg_palette_ptr to move.
  It must be compatible with the referenced tiles.
- **set_palette** `void set_palette(const bg_palette_item& palette_item)` (L480): Replaces the referenced color palette with a new tile set created with the given bg_palette_item.
  `palette_item` It creates the new color palette to reference.
  Before creating a new color palette, the bg_palette_ptr referenced by this map is removed,
  so VRAM usage is reduced.
  
  The new color palette must be compatible with the referenced tiles.
- **set_tiles_and_palette** `void set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette)` (L487): Sets the tiles and the color palette to reference.
  `tiles` regular_bg_tiles_ptr to reference.; `palette` bg_palette_ptr to reference.
- **set_tiles_and_palette** `void set_tiles_and_palette(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item)` (L502): Replaces the referenced tiles and color palette with the created with the given regular_bg_tiles_item and bg_palette_item.
  `tiles_item` It creates the new background tiles to reference.; `palette_item` It creates the color palette to reference.
  Before creating new resources, the resources referenced by this map are removed,
  so VRAM usage is reduced.
  
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if this map is dynamic (managed by yourself).
- **set_tiles_and_palette** `void set_tiles_and_palette(const regular_bg_tiles_item& tiles_item, bool allow_tiles_offset, const bg_palette_item& palette_item)` (L516): Replaces the referenced tiles and color palette with the created with the given regular_bg_tiles_item and bg_palette_item.
  `tiles_item` It creates the new background tiles to reference.; `allow_tiles_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it this map is dynamic (managed by yourself).; `palette_item` It creates the color palette to reference.
  Before creating new resources, the resources referenced by this map are removed,
  so VRAM usage is reduced.
- **vram** `optional<span<const regular_bg_map_cell>> vram() const` (L523): Returns the allocated memory in VRAM if this regular_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
- **vram** `optional<span<regular_bg_map_cell>> vram()` (L529): Returns the allocated memory in VRAM if this regular_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
- **handle** `int handle() const` (L534): Returns the internal handle.
- **swap** `void swap(regular_bg_map_ptr& other)` (L543): Exchanges the contents of this regular_bg_map_ptr with those of the other one.
  `other` regular_bg_map_ptr to exchange the contents with.
- **swap** `friend void swap(regular_bg_map_ptr& a, regular_bg_map_ptr& b)` (L553): Exchanges the contents of a regular_bg_map_ptr with those of another one.
  `a` First regular_bg_map_ptr to exchange the contents with.; `b` Second regular_bg_map_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const regular_bg_map_ptr& a, const regular_bg_map_ptr& b) = default` (L561): Default equal operator.
