# Butano 21.10.1 — Tools (`tool`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_affine_bg_item.h

### `bn::affine_bg_item` (class, bn_affine_bg_item.h:39)

Contains the required information to generate affine backgrounds and their maps.

`class affine_bg_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `affine_bg` type.

Tiles, colors and map cells are not copied but referenced,
so they should outlive the affine_bg_item to avoid dangling references.

- **affine_bg_item** `constexpr affine_bg_item(const span<const tile>& tiles_ref, const span<const color>& colors_ref, const affine_bg_map_cell& map_cells_ref, const size& map_dimensions)` (L62): Constructor.
  `tiles_ref` Reference to affine background tiles.; `colors_ref` Reference to an array of multiples of 16 colors.; `map_cells_ref` affine background map cells to reference.; `map_dimensions` Size in map cells of the referenced map cells.
  The tiles are not copied but referenced,
  so they should outlive the affine_bg_item to avoid dangling references.
  
  The colors are not copied but referenced,
  so they should outlive the affine_bg_item to avoid dangling references.
  
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_item to avoid dangling references.
- **affine_bg_item** `constexpr affine_bg_item(const affine_bg_tiles_item& tiles_item, const bg_palette_item& palette_item, const affine_bg_map_item& map_item)` (L76): Constructor.
  `tiles_item` It creates the tiles of the output affine backgrounds.; `palette_item` It creates the color palette of the output affine backgrounds.; `map_item` It creates the map of the output affine backgrounds.
- **tiles_item** `constexpr const affine_bg_tiles_item& tiles_item() const` (L90): Returns the item used to create the tiles of the output affine backgrounds.
- **palette_item** `constexpr const bg_palette_item& palette_item() const` (L98): Returns the item used to create the color palette of the output affine backgrounds.
- **map_item** `constexpr const affine_bg_map_item& map_item() const` (L106): Returns the item used to create the map of the output affine backgrounds.
- **big** `constexpr bool big() const` (L116): Indicates if backgrounds generated with this item are big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **decompress** `affine_bg_item decompress( span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref, span<affine_bg_map_cell> decompressed_cells_ref) const` (L132): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref, the colors referenced by decompressed_colors_ref and the map cells referenced by decompressed_cells_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; `decompressed_colors_ref` Destination of the decompressed colors.; `decompressed_cells_ref` Destination of the decompressed map cells.; devuelve: An affine_bg_item pointing to the decompressed tiles, colors and map cells.
  If the source and destination tiles, colors or map cells overlap, the behavior is undefined.
- **create_bg** `affine_bg_ptr create_bg() const` (L149): Creates an affine_bg_ptr using the information contained in this item.
  devuelve: The requested affine_bg_ptr.
- **create_bg** `affine_bg_ptr create_bg(int map_index) const` (L156): Creates an affine_bg_ptr using the information contained in this item.
  `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr.
- **create_bg** `affine_bg_ptr create_bg(fixed x, fixed y) const` (L164): Creates an affine_bg_ptr using the information contained in this item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; devuelve: The requested affine_bg_ptr.
- **create_bg** `affine_bg_ptr create_bg(fixed x, fixed y, int map_index) const` (L173): Creates an affine_bg_ptr using the information contained in this item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr.
- **create_bg** `affine_bg_ptr create_bg(const fixed_point& position) const` (L180): Creates an affine_bg_ptr using the information contained in this item.
  `position` Position of the affine background.; devuelve: The requested affine_bg_ptr.
- **create_bg** `affine_bg_ptr create_bg(const fixed_point& position, int map_index) const` (L188): Creates an affine_bg_ptr using the information contained in this item.
  `position` Position of the affine background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional() const` (L194): Creates an affine_bg_ptr using the information contained in this item.
  devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional(int map_index) const` (L201): Creates an affine_bg_ptr using the information contained in this item.
  `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional(fixed x, fixed y) const` (L209): Creates an affine_bg_ptr using the information contained in this item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional(fixed x, fixed y, int map_index) const` (L218): Creates an affine_bg_ptr using the information contained in this item.
  `x` Horizontal position of the affine background.; `y` Vertical position of the affine background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional(const fixed_point& position) const` (L225): Creates an affine_bg_ptr using the information contained in this item.
  `position` Position of the affine background.; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<affine_bg_ptr> create_bg_optional(const fixed_point& position, int map_index) const` (L233): Creates an affine_bg_ptr using the information contained in this item.
  `position` Position of the affine background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **find_map** `optional<affine_bg_map_ptr> find_map() const` (L240): Searches for an affine_bg_map_ptr which references the information provided by this item.
  devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **find_map** `optional<affine_bg_map_ptr> find_map(int map_index) const` (L248): Searches for an affine_bg_map_ptr which references the information provided by this item.
  `map_index` Index of the referenced map to search.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **create_map** `affine_bg_map_ptr create_map() const` (L260): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map** `affine_bg_map_ptr create_map(int map_index) const` (L273): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<affine_bg_map_ptr> create_map_optional() const` (L297): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<affine_bg_map_ptr> create_map_optional(int map_index) const` (L311): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const affine_bg_item& a, const affine_bg_item& b) = default` (L326): Default equal operator.

## bn_affine_bg_map_item.h

### `bn::affine_bg_map_item` (class, bn_affine_bg_map_item.h:45)

Contains the required information to generate affine background maps.

`class affine_bg_map_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `affine_bg` type.

The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
to avoid dangling references.

- **affine_bg_map_item** `constexpr affine_bg_map_item(const affine_bg_map_cell& cells_ref, const size& dimensions) : affine_bg_map_item(cells_ref, dimensions, compression_type::NONE)` (L58): Constructor.
  `cells_ref` Reference to one or more affine background map cells.; `dimensions` Size in map cells of the referenced map cells.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **affine_bg_map_item** `constexpr affine_bg_map_item(const affine_bg_map_cell& cells_ref, const size& dimensions, int maps_count) : affine_bg_map_item(cells_ref, dimensions, compression_type::NONE, maps_count)` (L73): Constructor.
  `cells_ref` Reference to one or more affine background map cells.; `dimensions` Size in map cells of each referenced map.; `maps_count` Number of maps contained in cells_ref.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **affine_bg_map_item** `constexpr affine_bg_map_item( const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression)` (L88): Constructor.
  `cells_ref` Reference to one or more affine background map cells.; `dimensions` Size in map cells of the referenced map cells.; `compression` Compression type.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **affine_bg_map_item** `constexpr affine_bg_map_item( const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression, int maps_count)` (L115): Constructor.
  `cells_ref` Reference to one or more affine background map cells.; `dimensions` Size in map cells of each referenced map.; `compression` Compression type.; `maps_count` Number of maps contained in cells_ref.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **affine_bg_map_item** `constexpr affine_bg_map_item( const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression, int maps_count, bool big)` (L147): Constructor.
  `cells_ref` Reference to one or more affine background map cells.; `dimensions` Size in map cells of each referenced map.; `compression` Compression type.; `maps_count` Number of maps contained in cells_ref.; `big` Indicates if maps generated with this item are big or not.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
  
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **cells_ptr** `constexpr const affine_bg_map_cell* cells_ptr() const` (L170): Returns a pointer to the referenced map cells of the first map.
- **cells_ptr** `constexpr const affine_bg_map_cell* cells_ptr(int map_index) const` (L178): Returns a pointer to the referenced map cells of the map indicated by map_index.
- **cells_ref** `constexpr const affine_bg_map_cell& cells_ref() const` (L190): Returns the referenced map cells of the first map.
- **cells_ref** `constexpr const affine_bg_map_cell& cells_ref(int map_index) const` (L198): Returns the referenced map cells of the map indicated by map_index.
- **dimensions** `constexpr const size& dimensions() const` (L206): Returns the size in map cells of the referenced map cells.
- **big** `constexpr bool big() const` (L216): Indicates if maps generated with this item are big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **maps_count** `constexpr int maps_count() const` (L224): Returns the number of referenced maps.
- **cells_count** `constexpr int cells_count() const` (L232): Returns the number of referenced map cells.
- **compression** `constexpr compression_type compression() const` (L240): Returns the compression type.
- **cell_index** `constexpr int cell_index(int map_x, int map_y) const` (L255): Returns the index of the referenced map cell in the specified map coordinates.
  `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The index of the referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **cell_index** `constexpr int cell_index(const point& map_position) const` (L274): Returns the index of the referenced map cell in the specified map coordinates.
  `map_position` Position of the map cell.; devuelve: The index of the referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **cell** `constexpr affine_bg_map_cell cell(int map_x, int map_y) const` (L289): Returns the referenced map cell in the specified map coordinates.
  `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **cell** `constexpr affine_bg_map_cell cell(int map_index, int map_x, int map_y) const` (L305): Returns the referenced map cell in the specified map coordinates.
  `map_index` Referenced map index.; `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **cell** `constexpr affine_bg_map_cell cell(const point& map_position) const` (L322): Returns the referenced map cell in the specified map coordinates.
  `map_position` Position of the map cell.; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **cell** `constexpr affine_bg_map_cell cell(int map_index, const point& map_position) const` (L337): Returns the referenced map cell in the specified map coordinates.
  `map_index` Referenced map index.; `map_position` Position of the map cell.; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
  to avoid dangling references.
- **decompress** `affine_bg_map_item decompress(span<affine_bg_map_cell> decompressed_cells_ref) const` (L350): Decompresses the stored data in the map cells referenced by decompressed_cells_ref.
  `decompressed_cells_ref` Destination of the decompressed map cells.; devuelve: An affine_bg_map_item pointing to the decompressed map cells.
  If the source and destination map cells overlap, the behavior is undefined.
- **find_map** `optional<affine_bg_map_ptr> find_map( const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const` (L367): Searches for an affine_bg_map_ptr which references the information provided by this item.
  `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **find_map** `optional<affine_bg_map_ptr> find_map( const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index) const` (L378): Searches for an affine_bg_map_ptr which references the information provided by this item.
  `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; `map_index` Index of the referenced map to search.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **create_map** `affine_bg_map_ptr create_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const` (L393): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map** `affine_bg_map_ptr create_map( affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const` (L408): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<affine_bg_map_ptr> create_map_optional( affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const` (L435): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<affine_bg_map_ptr> create_map_optional( affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const` (L452): Searches for an affine_bg_map_ptr which references the information provided by this item. If it is not found, it creates an affine_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: affine_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns an affine_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the affine_bg_map_ptr to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const affine_bg_map_item& a, const affine_bg_map_item& b) = default` (L470): Default equal operator.

## bn_affine_bg_tiles_item.h

### `bn::affine_bg_tiles_item` (class, bn_affine_bg_tiles_item.h:41)

Contains the required information to generate affine background tiles.

`class affine_bg_tiles_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `affine_bg` type.

The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
to avoid dangling references.

- **valid_tiles_count** `constexpr static bool valid_tiles_count(int tiles_count)` (L48): Indicates if the specified tiles count are valid or not.
- **affine_bg_tiles_item** `constexpr explicit affine_bg_tiles_item(const span<const tile>& tiles_ref) : affine_bg_tiles_item(tiles_ref, compression_type::NONE)` (L60): Constructor.
  `tiles_ref` Reference to one or more background tiles.
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
  to avoid dangling references.
- **affine_bg_tiles_item** `constexpr affine_bg_tiles_item(const span<const tile>& tiles_ref, compression_type compression)` (L74): Constructor.
  `tiles_ref` Reference to one or more background tiles.; `compression` Compression type.
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
  to avoid dangling references.
- **compression** `constexpr compression_type compression() const` (L84): Returns the compression type.
- **decompress** `affine_bg_tiles_item decompress(span<tile> decompressed_tiles_ref) const` (L97): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; devuelve: An affine_bg_tiles_item pointing to the decompressed tiles.
  If the source and destination tiles overlap, the behavior is undefined.
- **tiles_ref** `constexpr const span<const tile>& tiles_ref() const` (L105): Returns the reference to one or more background tiles.
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
  to avoid dangling references.
- **find_tiles** `optional<affine_bg_tiles_ptr> find_tiles() const` (L115): Searches for an affine_bg_tiles_ptr which reference the background tiles.
  devuelve: affine_bg_tiles_ptr which reference the background tiles if they have been found; bn::nullopt otherwise.
- **create_tiles** `affine_bg_tiles_ptr create_tiles() const` (L130): Searches for an affine_bg_tiles_ptr which reference the background tiles. If they are not found, it creates an affine_bg_tiles_ptr which reference them.
  devuelve: affine_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns an affine_bg_tiles_ptr which reference them.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles** `affine_bg_tiles_ptr create_tiles(bool allow_offset) const` (L144): Searches for an affine_bg_tiles_ptr which reference the background tiles. If they are not found, it creates an affine_bg_tiles_ptr which reference them.
  `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns an affine_bg_tiles_ptr which reference them.
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<affine_bg_tiles_ptr> create_tiles_optional() const` (L167): Searches for an affine_bg_tiles_ptr which reference the background tiles. If they are not found, it creates an affine_bg_tiles_ptr which reference them.
  devuelve: affine_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a new affine_bg_tiles_ptr which reference them if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<affine_bg_tiles_ptr> create_tiles_optional(bool allow_offset) const` (L182): Searches for an affine_bg_tiles_ptr which reference the background tiles. If they are not found, it creates an affine_bg_tiles_ptr which reference them.
  `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a new affine_bg_tiles_ptr which reference them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
  to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const affine_bg_tiles_item& a, const affine_bg_tiles_item& b)` (L197): Equal operator.
  `a` First affine_bg_tiles_item to compare.; `b` Second affine_bg_tiles_item to compare.; devuelve: `true` if the first affine_bg_tiles_item is equal to the second one, otherwise `false`.

## bn_bg_palette_item.h

### `bn::bg_palette_item` (class, bn_bg_palette_item.h:42)

Contains the required information to generate background color palettes.

`class bg_palette_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `regular_bg` or `affine_bg` types.

The colors are not copied but referenced, so they should outlive the bg_palette_item
to avoid dangling references.

- **bg_palette_item** `constexpr bg_palette_item(const span<const color>& colors_ref, bpp_mode bpp) : bg_palette_item(colors_ref, bpp, compression_type::NONE)` (L55): Constructor.
  `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of the color palettes to create.
  The colors are not copied but referenced, so they should outlive the bg_palette_item
  to avoid dangling references.
- **bg_palette_item** `constexpr bg_palette_item(const span<const color>& colors_ref, bpp_mode bpp, compression_type compression)` (L70): Constructor.
  `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of the color palettes to create.; `compression` Compression type.
  The colors are not copied but referenced, so they should outlive the bg_palette_item
  to avoid dangling references.
- **colors_ref** `constexpr const span<const color>& colors_ref() const` (L85): Returns the referenced array of multiples of 16 colors.
  The colors are not copied but referenced, so they should outlive the bg_palette_item
  to avoid dangling references.
- **bpp** `constexpr bpp_mode bpp() const` (L93): Returns the bits per pixel of the color palettes to create.
- **compression** `constexpr compression_type compression() const` (L101): Returns the compression type.
- **decompress** `bg_palette_item decompress(span<color> decompressed_colors_ref) const` (L114): Decompresses the stored data in the colors referenced by decompressed_colors_ref.
  `decompressed_colors_ref` Destination of the decompressed colors.; devuelve: A bg_palette_item pointing to the decompressed colors.
  If the source and destination colors overlap, the behavior is undefined.
- **find_palette** `optional<bg_palette_ptr> find_palette() const` (L121): Searches for a bg_palette_ptr which contains the colors referenced by this item.
  devuelve: bg_palette_ptr which contains the colors referenced by this item if it has been found; bn::nullopt otherwise.
- **create_palette** `bg_palette_ptr create_palette() const` (L129): Searches for a bg_palette_ptr which contains the colors referenced by this item. If it is not found, it creates a bg_palette_ptr which contains them.
  devuelve: bg_palette_ptr which contains the colors referenced by this item if it has been found; otherwise it returns a new bg_palette_ptr which contains them.
- **create_new_palette** `bg_palette_ptr create_new_palette() const` (L135): Creates a new bg_palette_ptr which contains the colors referenced by this item.
  devuelve: bg_palette_ptr which contains the colors referenced by this item.
- **create_palette_optional** `optional<bg_palette_ptr> create_palette_optional() const` (L144): Searches for a bg_palette_ptr which contains the colors referenced by this item. If it is not found, it creates a bg_palette_ptr which contains them.
  devuelve: bg_palette_ptr which contains the colors referenced by this item if it has been found; otherwise it returns a new bg_palette_ptr which contains them it if it could be allocated; bn::nullopt otherwise.
- **create_new_palette_optional** `optional<bg_palette_ptr> create_new_palette_optional() const` (L151): Creates a new bg_palette_ptr which contains the colors referenced by this item.
  devuelve: bg_palette_ptr which contains the colors referenced by this item it if it could be allocated; bn::nullopt otherwise.
- **operator==** `constexpr friend bool operator==(const bg_palette_item& a, const bg_palette_item& b) = default` (L156): Default equal operator.

## bn_compression_type.h

### `bn::compression_type` (enum, bn_compression_type.h:26)

Specifies the available compression types.

`enum class compression_type : uint8_t { NONE /* Uncompressed data. */, LZ77 /* LZ77 compressed data. */, RUN_LENGTH /* Run-length compressed data. */, HUFFMAN /* Huffman compressed data. */ }`


## bn_direct_bitmap_item.h

### `bn::direct_bitmap_item` (class, bn_direct_bitmap_item.h:38)

Contains the required information to generate direct bitmaps.

`class direct_bitmap_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `direct_bitmap` type.

The colors are not copied but referenced, so they should outlive the direct_bitmap_item
to avoid dangling references.

- **direct_bitmap_item** `constexpr direct_bitmap_item(const bn::color& colors_ref, const size& dimensions) : direct_bitmap_item(colors_ref, dimensions, compression_type::NONE)` (L51): Constructor.
  `colors_ref` Reference to the bitmap colors.; `dimensions` Size in pixels of the referenced bitmap.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **direct_bitmap_item** `constexpr direct_bitmap_item( const bn::color& colors_ref, const size& dimensions, compression_type compression)` (L66): Constructor.
  `colors_ref` Reference to the bitmap colors.; `dimensions` Size in pixels of the referenced bitmap.; `compression` Compression type.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **colors_ptr** `constexpr const bn::color* colors_ptr() const` (L82): Returns a pointer to the referenced bitmap colors.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **colors_ref** `constexpr const bn::color& colors_ref() const` (L93): Returns the referenced bitmap colors.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **dimensions** `constexpr const size& dimensions() const` (L104): Returns the size in pixels of the referenced bitmap.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **colors_count** `constexpr int colors_count() const` (L115): Returns the number of referenced colors.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **color_index** `constexpr int color_index(int x, int y) const` (L130): Returns the index of the referenced color in the specified pixel coordinates.
  `x` Horizontal position of the bitmap color [0..dimensions().width()).; `y` Vertical position of the bitmap color [0..dimensions().height()).; devuelve: The index of the referenced bitmap color.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **color_index** `constexpr int color_index(const point& position) const` (L149): Returns the index of the referenced color in the specified pixel coordinates.
  `position` Position of the bitmap color.; devuelve: The index of the referenced bitmap color.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **color** `constexpr bn::color color(int x, int y) const` (L164): Returns the referenced bitmap color in the specified pixel coordinates.
  `x` Horizontal position of the bitmap color [0..dimensions().width()).; `y` Vertical position of the bitmap color [0..dimensions().height()).; devuelve: The referenced bitmap color.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **color** `constexpr bn::color color(const point& position) const` (L178): Returns the referenced bitmap color in the specified pixel coordinates.
  `position` Position of the bitmap color.; devuelve: The referenced bitmap color.
  The colors are not copied but referenced, so they should outlive the direct_bitmap_item
  to avoid dangling references.
- **compression** `constexpr compression_type compression() const` (L186): Returns the compression type.
- **decompress** `direct_bitmap_item decompress(span<bn::color> decompressed_colors_ref) const` (L199): Decompresses the stored data in the colors referenced by decompressed_colors_ref.
  `decompressed_colors_ref` Destination of the decompressed colors.; devuelve: An direct_bitmap_item pointing to the decompressed colors.
  If the source and destination colors overlap, the behavior is undefined.
- **operator==** `constexpr friend bool operator==( const direct_bitmap_item& a, const direct_bitmap_item& b) = default` (L204): Default equal operator.

## bn_dmg_music_item.h

### `bn::dmg_music_item` (class, bn_dmg_music_item.h:33)

Contains the required information to play DMG music.

`class dmg_music_item`

The assets conversion tools generate an object of this type in the build folder for each module file.

Module files with `*.mod` and `*.s3m` extensions are supported.

- **dmg_music_item** `constexpr explicit dmg_music_item(const uint8_t& data_ref) : dmg_music_item(data_ref, dmg_music_type::GBT_PLAYER)` (L44): Constructor.
  `data_ref` Reference to the song data.
  Song data is not copied but referenced, so it should outlive the dmg_music_item
  to avoid dangling references.
- **dmg_music_item** `constexpr explicit dmg_music_item(const uint8_t& data_ref, dmg_music_type type)` (L57): Constructor.
  `data_ref` Reference to the song data.; `type` Referenced song type.
  Song data is not copied but referenced, so it should outlive the dmg_music_item
  to avoid dangling references.
- **data_ptr** `constexpr const uint8_t* data_ptr() const` (L66): Returns a pointer to the referenced song data.
- **data_ref** `constexpr const uint8_t& data_ref() const` (L74): Returns the referenced song data.
- **type** `constexpr dmg_music_type type() const` (L82): Returns the referenced song type.
- **play** `void play() const` (L92): Plays the DMG music specified by this item with default settings.
  Default settings are speed = 1 and loop enabled.
- **play** `void play(int speed) const` (L100): Plays the DMG music specified by this item.
  `speed` Playback speed, in the range [1..256].
  VGM player only supports the default playback speed (1).
- **play** `void play(int speed, bool loop) const` (L110): Plays the DMG music specified by this item.
  `speed` Playback speed, in the range [1..256].; `loop` Indicates if it must be played until it is stopped manually or until end.
  VGM player only supports the default playback speed (1).
- **operator==** `constexpr friend bool operator==(const dmg_music_item& a, const dmg_music_item& b) = default` (L115): Default equal operator.

## bn_music_item.h

### `bn::music_item` (class, bn_music_item.h:32)

Contains the required information to play Direct Sound music.

`class music_item`

The assets conversion tools generate an object of this type in the build folder for each module file.

Module files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions are supported.

- **music_item** `constexpr explicit music_item(int id)` (L40): Constructor.
  `id` Internal ID.
- **id** `constexpr int id() const` (L48): Returns the internal ID.
- **play** `void play() const` (L58): Plays the Direct Sound music specified by this item with default settings.
  Default settings are volume = 1 and loop enabled.
- **play** `void play(fixed volume) const` (L64): Plays the Direct Sound music specified by this item.
  `volume` Volume level, in the range [0..1].
- **play** `void play(fixed volume, bool loop) const` (L71): Plays the Direct Sound music specified by this item.
  `volume` Volume level, in the range [0..1].; `loop` Indicates if it must be played until it is stopped manually or until end.
- **play_jingle** `void play_jingle() const` (L76): Plays the Direct Sound jingle specified by this item with volume = 1.
- **play_jingle** `void play_jingle(fixed volume) const` (L82): Plays the Direct Sound jingle specified by this item.
  `volume` Volume level, in the range [0..1].
- **operator==** `constexpr friend bool operator==(music_item a, music_item b) = default` (L87): Default equal operator.

## bn_palette_bitmap_item.h

### `bn::palette_bitmap_item` (class, bn_palette_bitmap_item.h:35)

Contains the required information to generate palette bitmaps.

`class palette_bitmap_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `palette_bitmap` type.

Pixels and colors are not copied but referenced, so they should outlive the palette_bitmap_item
to avoid dangling references.

- **palette_bitmap_item** `constexpr palette_bitmap_item( const uint8_t& pixels_ref, const size& dimensions, const span<const color>& colors_ref)` (L52): Constructor.
  `pixels_ref` Reference to the palette bitmap pixels.; `dimensions` Size in pixels of the referenced palette bitmap pixels.; `colors_ref` Reference to an array of multiples of 16 colors.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_item
  to avoid dangling references.
  
  The colors are not copied but referenced, so they should outlive the palette_bitmap_item
  to avoid dangling references.
- **palette_bitmap_item** `constexpr palette_bitmap_item( const palette_bitmap_pixels_item& pixels_item, const bg_palette_item& palette_item)` (L64): Constructor.
  `pixels_item` It references the pixels of the bitmap.; `palette_item` It references the colors of the bitmap.
- **pixels_item** `constexpr const palette_bitmap_pixels_item& pixels_item() const` (L75): Returns the item that references the pixels of the bitmap.
- **palette_item** `constexpr const bg_palette_item& palette_item() const` (L83): Returns the item that references the colors of the bitmap.
- **decompress** `palette_bitmap_item decompress( span<uint8_t> decompressed_pixels_ref, span<color> decompressed_colors_ref) const` (L98): Decompresses the stored data in the pixels referenced by decompressed_pixels_ref and the colors referenced by decompressed_colors_ref.
  `decompressed_pixels_ref` Destination of the decompressed pixels.; `decompressed_colors_ref` Destination of the decompressed colors.; devuelve: A sprite_item pointing to the decompressed pixels and colors.
  If the source and destination pixels or colors overlap, the behavior is undefined.
- **operator==** `constexpr friend bool operator==( const palette_bitmap_item& a, const palette_bitmap_item& b) = default` (L104): Default equal operator.

## bn_palette_bitmap_pixels_item.h

### `bn::palette_bitmap_pixels_item` (class, bn_palette_bitmap_pixels_item.h:37)

Contains the required information to generate palette bitmap pixels.

`class palette_bitmap_pixels_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `palette_bitmap_pixels` type.

The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
to avoid dangling references.

- **palette_bitmap_pixels_item** `constexpr palette_bitmap_pixels_item(const uint8_t& pixels_ref, const size& dimensions) : palette_bitmap_pixels_item(pixels_ref, dimensions, compression_type::NONE)` (L50): Constructor.
  `pixels_ref` Reference to the palette bitmap pixels.; `dimensions` Size in pixels of the referenced palette bitmap pixels.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **palette_bitmap_pixels_item** `constexpr palette_bitmap_pixels_item( const uint8_t& pixels_ref, const size& dimensions, compression_type compression)` (L65): Constructor.
  `pixels_ref` Reference to the palette bitmap pixels.; `dimensions` Size in pixels of the referenced palette bitmap pixels.; `compression` Compression type.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixels_ptr** `constexpr const uint8_t* pixels_ptr() const` (L81): Returns a pointer to the referenced palette bitmap pixels.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixels_ref** `constexpr const uint8_t& pixels_ref() const` (L92): Returns the referenced palette bitmap pixels.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **dimensions** `constexpr const size& dimensions() const` (L103): Returns the size in pixels of the referenced palette bitmap pixels.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixels_count** `constexpr int pixels_count() const` (L114): Returns the number of referenced pixels.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixel_index** `constexpr int pixel_index(int x, int y) const` (L129): Returns the index of the referenced pixel in the specified pixel coordinates.
  `x` Horizontal position of the palette bitmap pixel [0..dimensions().width()).; `y` Vertical position of the palette bitmap pixel [0..dimensions().height()).; devuelve: The index of the referenced palette bitmap pixel.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixel_index** `constexpr int pixel_index(const point& position) const` (L148): Returns the index of the referenced pixel in the specified pixel coordinates.
  `position` Position of the palette bitmap pixel.; devuelve: The index of the referenced palette bitmap pixel.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixel** `constexpr uint8_t pixel(int x, int y) const` (L163): Returns the referenced palette bitmap pixel in the specified pixel coordinates.
  `x` Horizontal position of the palette bitmap pixel [0..dimensions().width()).; `y` Vertical position of the palette bitmap pixel [0..dimensions().height()).; devuelve: The referenced palette bitmap pixel.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **pixel** `constexpr uint8_t pixel(const point& position) const` (L177): Returns the referenced palette bitmap pixel in the specified pixel coordinates.
  `position` Position of the palette bitmap pixel.; devuelve: The referenced palette bitmap pixel.
  The pixels are not copied but referenced, so they should outlive the palette_bitmap_pixels_item
  to avoid dangling references.
- **compression** `constexpr compression_type compression() const` (L185): Returns the compression type.
- **decompress** `palette_bitmap_pixels_item decompress(span<uint8_t> decompressed_pixels_ref) const` (L198): Decompresses the stored data in the pixels referenced by decompressed_pixels_ref.
  `decompressed_pixels_ref` Destination of the decompressed pixels.; devuelve: An palette_bitmap_pixels_item pointing to the decompressed pixels.
  If the source and destination pixels overlap, the behavior is undefined.
- **operator==** `constexpr friend bool operator==( const palette_bitmap_pixels_item& a, const palette_bitmap_pixels_item& b) = default` (L203): Default equal operator.

## bn_regular_bg_item.h

### `bn::regular_bg_item` (class, bn_regular_bg_item.h:39)

Contains the required information to generate regular backgrounds and their maps.

`class regular_bg_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `regular_bg` type.

Tiles, colors and map cells are not copied but referenced,
so they should outlive the regular_bg_item to avoid dangling references.

- **regular_bg_item** `constexpr regular_bg_item(const span<const tile>& tiles_ref, const span<const color>& colors_ref, bpp_mode bpp, const regular_bg_map_cell& map_cells_ref, const size& map_dimensions) : regular_bg_item(regular_bg_tiles_item(tiles_ref, bpp), bg_palette_item(colors_ref, bpp), regular_bg_map_item(map_cells_ref, map_dimensions))` (L63): Constructor.
  `tiles_ref` Reference to regular background tiles.; `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of referenced colors.; `map_cells_ref` Regular background map cells to reference.; `map_dimensions` Size in map cells of the referenced map cells.
  The tiles are not copied but referenced,
  so they should outlive the regular_bg_item to avoid dangling references.
  
  The colors are not copied but referenced,
  so they should outlive the regular_bg_item to avoid dangling references.
  
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_item to avoid dangling references.
- **regular_bg_item** `constexpr regular_bg_item(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item, const regular_bg_map_item& map_item)` (L76): Constructor.
  `tiles_item` It creates the tiles of the output regular backgrounds.; `palette_item` It creates the color palette of the output regular backgrounds.; `map_item` It creates the map of the output regular backgrounds.
- **tiles_item** `constexpr const regular_bg_tiles_item& tiles_item() const` (L90): Returns the item used to create the tiles of the output regular backgrounds.
- **palette_item** `constexpr const bg_palette_item& palette_item() const` (L98): Returns the item used to create the color palette of the output regular backgrounds.
- **map_item** `constexpr const regular_bg_map_item& map_item() const` (L106): Returns the item used to create the map of the output regular backgrounds.
- **big** `constexpr bool big() const` (L116): Indicates if backgrounds generated with this item are big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **decompress** `regular_bg_item decompress( span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref, span<regular_bg_map_cell> decompressed_cells_ref) const` (L132): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref, the colors referenced by decompressed_colors_ref and the map cells referenced by decompressed_cells_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; `decompressed_colors_ref` Destination of the decompressed colors.; `decompressed_cells_ref` Destination of the decompressed map cells.; devuelve: A regular_bg_item pointing to the decompressed tiles, colors and map cells.
  If the source and destination tiles, colors or map cells overlap, the behavior is undefined.
- **create_bg** `regular_bg_ptr create_bg() const` (L149): Creates a regular_bg_ptr using the information contained in this item.
  devuelve: The requested regular_bg_ptr.
- **create_bg** `regular_bg_ptr create_bg(int map_index) const` (L156): Creates a regular_bg_ptr using the information contained in this item.
  `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr.
- **create_bg** `regular_bg_ptr create_bg(fixed x, fixed y) const` (L164): Creates a regular_bg_ptr using the information contained in this item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; devuelve: The requested regular_bg_ptr.
- **create_bg** `regular_bg_ptr create_bg(fixed x, fixed y, int map_index) const` (L173): Creates a regular_bg_ptr using the information contained in this item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr.
- **create_bg** `regular_bg_ptr create_bg(const fixed_point& position) const` (L180): Creates a regular_bg_ptr using the information contained in this item.
  `position` Position of the regular background.; devuelve: The requested regular_bg_ptr.
- **create_bg** `regular_bg_ptr create_bg(const fixed_point& position, int map_index) const` (L188): Creates a regular_bg_ptr using the information contained in this item.
  `position` Position of the regular background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional() const` (L194): Creates a regular_bg_ptr using the information contained in this item.
  devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional(int map_index) const` (L201): Creates a regular_bg_ptr using the information contained in this item.
  `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional(fixed x, fixed y) const` (L209): Creates a regular_bg_ptr using the information contained in this item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional(fixed x, fixed y, int map_index) const` (L218): Creates a regular_bg_ptr using the information contained in this item.
  `x` Horizontal position of the regular background.; `y` Vertical position of the regular background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional(const fixed_point& position) const` (L225): Creates a regular_bg_ptr using the information contained in this item.
  `position` Position of the regular background.; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **create_bg_optional** `optional<regular_bg_ptr> create_bg_optional(const fixed_point& position, int map_index) const` (L233): Creates a regular_bg_ptr using the information contained in this item.
  `position` Position of the regular background.; `map_index` Index of the map to reference in map_item().; devuelve: The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
- **find_map** `optional<regular_bg_map_ptr> find_map() const` (L240): Searches for a regular_bg_map_ptr which references the information provided by this item.
  devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **find_map** `optional<regular_bg_map_ptr> find_map(int map_index) const` (L248): Searches for a regular_bg_map_ptr which references the information provided by this item.
  `map_index` Index of the referenced map to search.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **create_map** `regular_bg_map_ptr create_map() const` (L260): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map** `regular_bg_map_ptr create_map(int map_index) const` (L273): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<regular_bg_map_ptr> create_map_optional() const` (L296): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<regular_bg_map_ptr> create_map_optional(int map_index) const` (L310): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const regular_bg_item& a, const regular_bg_item& b) = default` (L325): Default equal operator.

## bn_regular_bg_map_item.h

### `bn::regular_bg_map_item` (class, bn_regular_bg_map_item.h:45)

Contains the required information to generate regular background maps.

`class regular_bg_map_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `regular_bg` type.

The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
to avoid dangling references.

- **regular_bg_map_item** `constexpr regular_bg_map_item(const regular_bg_map_cell& cells_ref, const size& dimensions) : regular_bg_map_item(cells_ref, dimensions, compression_type::NONE)` (L58): Constructor.
  `cells_ref` Reference to one or more regular background map cells.; `dimensions` Size in map cells of the referenced map cells.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **regular_bg_map_item** `constexpr regular_bg_map_item(const regular_bg_map_cell& cells_ref, const size& dimensions, int maps_count) : regular_bg_map_item(cells_ref, dimensions, compression_type::NONE, maps_count)` (L73): Constructor.
  `cells_ref` Reference to one or more regular background map cells.; `dimensions` Size in map cells of each referenced map.; `maps_count` Number of maps contained in cells_ref.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **regular_bg_map_item** `constexpr regular_bg_map_item( const regular_bg_map_cell& cells_ref, const size& dimensions, compression_type compression)` (L88): Constructor.
  `cells_ref` Reference to one or more regular background map cells.; `dimensions` Size in map cells of the referenced map cells.; `compression` Compression type.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **regular_bg_map_item** `constexpr regular_bg_map_item( const regular_bg_map_cell& cells_ref, const size& dimensions, compression_type compression, int maps_count)` (L113): Constructor.
  `cells_ref` Reference to one or more regular background map cells.; `dimensions` Size in map cells of each referenced map.; `compression` Compression type.; `maps_count` Number of maps contained in cells_ref.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **regular_bg_map_item** `constexpr regular_bg_map_item( const regular_bg_map_cell& cells_ref, const size& dimensions, compression_type compression, int maps_count, bool big)` (L143): Constructor.
  `cells_ref` Reference to one or more regular background map cells.; `dimensions` Size in map cells of each referenced map.; `compression` Compression type.; `maps_count` Number of maps contained in cells_ref.; `big` Indicates if maps generated with this item are big or not.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
  
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **cells_ptr** `constexpr const regular_bg_map_cell* cells_ptr() const` (L164): Returns a pointer to the referenced map cells of the first map.
- **cells_ptr** `constexpr const regular_bg_map_cell* cells_ptr(int map_index) const` (L172): Returns a pointer to the referenced map cells of the map indicated by map_index.
- **cells_ref** `constexpr const regular_bg_map_cell& cells_ref() const` (L184): Returns the referenced map cells of the first map.
- **cells_ref** `constexpr const regular_bg_map_cell& cells_ref(int map_index) const` (L192): Returns the referenced map cells of the map indicated by map_index.
- **dimensions** `constexpr const size& dimensions() const` (L200): Returns the size in map cells of each referenced map.
- **big** `constexpr bool big() const` (L210): Indicates if maps generated with this item are big or not.
  Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
- **maps_count** `constexpr int maps_count() const` (L218): Returns the number of referenced maps.
- **cells_count** `constexpr int cells_count() const` (L226): Returns the number of referenced map cells.
- **compression** `constexpr compression_type compression() const` (L234): Returns the compression type.
- **flat_layout** `constexpr bool flat_layout() const` (L242): Indicates if the referenced map has a flat layout or not.
- **cell_index** `constexpr int cell_index(int map_x, int map_y) const` (L257): Returns the index of the referenced map cell in the specified map coordinates.
  `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The index of the referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **cell_index** `constexpr int cell_index(const point& map_position) const` (L285): Returns the index of the referenced map cell in the specified map coordinates.
  `map_position` Position of the map cell.; devuelve: The index of the referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **cell** `constexpr regular_bg_map_cell cell(int map_x, int map_y) const` (L300): Returns the referenced map cell in the specified map coordinates.
  `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **cell** `constexpr regular_bg_map_cell cell(int map_index, int map_x, int map_y) const` (L316): Returns the referenced map cell in the specified map coordinates.
  `map_index` Referenced map index.; `map_x` Horizontal position of the map cell [0..dimensions().width()).; `map_y` Vertical position of the map cell [0..dimensions().height()).; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **cell** `constexpr regular_bg_map_cell cell(const point& map_position) const` (L333): Returns the referenced map cell in the specified map coordinates.
  `map_position` Position of the map cell.; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **cell** `constexpr regular_bg_map_cell cell(int map_index, const point& map_position) const` (L348): Returns the referenced map cell in the specified map coordinates.
  `map_index` Referenced map index.; `map_position` Position of the map cell.; devuelve: The referenced map cell.
  The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
  to avoid dangling references.
- **decompress** `regular_bg_map_item decompress(span<regular_bg_map_cell> decompressed_cells_ref) const` (L361): Decompresses the stored data in the map cells referenced by decompressed_cells_ref.
  `decompressed_cells_ref` Destination of the decompressed map cells.; devuelve: A regular_bg_map_item pointing to the decompressed map cells.
  If the source and destination map cells overlap, the behavior is undefined.
- **find_map** `optional<regular_bg_map_ptr> find_map( const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const` (L378): Searches for a regular_bg_map_ptr which references the information provided by this item.
  `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **find_map** `optional<regular_bg_map_ptr> find_map( const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index) const` (L389): Searches for a regular_bg_map_ptr which references the information provided by this item.
  `tiles` Referenced tiles of the map to search.; `palette` Referenced color palette of the map to search.; `map_index` Index of the referenced map to search.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; bn::nullopt otherwise.
- **create_map** `regular_bg_map_ptr create_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const` (L404): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map** `regular_bg_map_ptr create_map( regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const` (L419): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<regular_bg_map_ptr> create_map_optional( regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const` (L446): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **create_map_optional** `optional<regular_bg_map_ptr> create_map_optional( regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const` (L463): Searches for a regular_bg_map_ptr which references the information provided by this item. If it is not found, it creates a regular_bg_map_ptr which references it.
  `tiles` Referenced tiles of the map to search or handle.; `palette` Referenced color palette of the map to search or handle.; `map_index` Index of the referenced map to search or handle.; devuelve: regular_bg_map_ptr which references the information provided by this item if it has been found; otherwise it returns a regular_bg_map_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The map cells are not copied but referenced,
  so they should outlive the regular_bg_map_ptr to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const regular_bg_map_item& a, const regular_bg_map_item& b) = default` (L481): Default equal operator.

## bn_regular_bg_tiles_item.h

### `bn::regular_bg_tiles_item` (class, bn_regular_bg_tiles_item.h:42)

Contains the required information to generate regular background tiles.

`class regular_bg_tiles_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `regular_bg` type.

The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
to avoid dangling references.

- **valid_tiles_count** `constexpr static bool valid_tiles_count(int tiles_count, bpp_mode bpp)` (L49): Indicates if the specified tiles count are valid for the specified bits per pixel or not.
- **regular_bg_tiles_item** `constexpr regular_bg_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp) : regular_bg_tiles_item(tiles_ref, bpp, compression_type::NONE)` (L68): Constructor.
  `tiles_ref` Reference to one or more background tiles.; `bpp` tiles_ref bits per pixel.
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
  to avoid dangling references.
- **regular_bg_tiles_item** `constexpr regular_bg_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp, compression_type compression)` (L83): Constructor.
  `tiles_ref` Reference to one or more background tiles.; `bpp` tiles_ref bits per pixel.; `compression` Compression type.
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
  to avoid dangling references.
- **tiles_ref** `constexpr const span<const tile>& tiles_ref() const` (L98): Returns the reference to one or more background tiles.
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
  to avoid dangling references.
- **bpp** `constexpr bpp_mode bpp() const` (L106): Returns the bits per pixel of the referenced tiles.
- **compression** `constexpr compression_type compression() const` (L114): Returns the compression type.
- **decompress** `regular_bg_tiles_item decompress(span<tile> decompressed_tiles_ref) const` (L127): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; devuelve: A regular_bg_tiles_item pointing to the decompressed tiles.
  If the source and destination tiles overlap, the behavior is undefined.
- **find_tiles** `optional<regular_bg_tiles_ptr> find_tiles() const` (L134): Searches for a regular_bg_tiles_ptr which reference the background tiles.
  devuelve: regular_bg_tiles_ptr which reference the background tiles if they have been found; bn::nullopt otherwise.
- **create_tiles** `regular_bg_tiles_ptr create_tiles() const` (L149): Searches for a regular_bg_tiles_ptr which reference the background tiles. If they are not found, it creates a regular_bg_tiles_ptr which reference them.
  devuelve: regular_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a regular_bg_tiles_ptr which reference them.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles** `regular_bg_tiles_ptr create_tiles(bool allow_offset) const` (L163): Searches for a regular_bg_tiles_ptr which reference the background tiles. If they are not found, it creates a regular_bg_tiles_ptr which reference them.
  `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a regular_bg_tiles_ptr which reference them.
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<regular_bg_tiles_ptr> create_tiles_optional() const` (L186): Searches for a regular_bg_tiles_ptr which reference the background tiles. If they are not found, it creates a regular_bg_tiles_ptr which reference them.
  devuelve: regular_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a new regular_bg_tiles_ptr which reference them if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<regular_bg_tiles_ptr> create_tiles_optional(bool allow_offset) const` (L201): Searches for a regular_bg_tiles_ptr which reference the background tiles. If they are not found, it creates a regular_bg_tiles_ptr which reference them.
  `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which reference the background tiles if they have been found; otherwise it returns a new regular_bg_tiles_ptr which reference them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
  to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const regular_bg_tiles_item& a, const regular_bg_tiles_item& b)` (L216): Equal operator.
  `a` First regular_bg_tiles_item to compare.; `b` Second regular_bg_tiles_item to compare.; devuelve: `true` if the first regular_bg_tiles_item is equal to the second one, otherwise `false`.

## bn_sound_item.h

### `bn::sound_item` (class, bn_sound_item.h:31)

Contains the required information to play sound effects.

`class sound_item`

The assets conversion tools generate an object of this type in the build folder for each *.wav file.

- **sound_item** `constexpr explicit sound_item(int id)` (L39): Constructor.
  `id` Internal ID.
- **id** `constexpr int id() const` (L47): Returns the internal ID.
- **play** `sound_handle play() const` (L58): Plays the sound effect specified by this item with default settings.
  devuelve: Sound effect handle.
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play** `sound_handle play(fixed volume) const` (L65): Plays the sound effect specified by this item.
  `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle.
- **play** `sound_handle play(fixed volume, fixed speed, fixed panning) const` (L74): Plays the sound effect specified by this item.
  `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle.
- **play_optional** `optional<sound_handle> play_optional() const` (L82): Plays the sound effect specified by this item with default settings.
  devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_optional** `optional<sound_handle> play_optional(fixed volume) const` (L89): Plays the sound effect specified by this item.
  `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
- **play_optional** `optional<sound_handle> play_optional(fixed volume, fixed speed, fixed panning) const` (L98): Plays the sound effect specified by this item.
  `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
- **play_with_priority** `sound_handle play_with_priority(int priority) const` (L111): Plays the sound effect specified by this item with default settings and the given priority.
  `priority` Priority relative to backgrounds in the range [-32767..32767].; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
  
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_with_priority** `sound_handle play_with_priority(int priority, fixed volume) const` (L123): Plays the sound effect specified by this item with the given priority.
  `priority` Priority relative to backgrounds in the range [-32767..32767].; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_with_priority** `sound_handle play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const` (L137): Plays the sound effect specified by this item with the given priority.
  `priority` Priority relative to backgrounds in the range [-32767..32767].; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional(int priority) const` (L150): Plays the sound effect specified by this item with default settings and the given priority.
  `priority` Priority in the range [-32767..32767].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
  
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional(int priority, fixed volume) const` (L162): Plays the sound effect specified by this item with the given priority.
  `priority` Priority in the range [-32767..32767].; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional(int priority, fixed volume, fixed speed, fixed panning) const` (L176): Plays the sound effect specified by this item with the given priority.
  `priority` Priority in the range [-32767..32767].; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **operator==** `constexpr friend bool operator==(sound_item a, sound_item b) = default` (L181): Default equal operator.

## bn_sprite_item.h

### `bn::sprite_item` (class, bn_sprite_item.h:38)

Contains the required information to generate sprites.

`class sprite_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `sprite` type.

Tiles and colors are not copied but referenced, so they should outlive the sprite_item to avoid dangling references.

- **sprite_item** `constexpr sprite_item(const sprite_shape_size& shape_size, const span<const tile>& tiles_ref, const span<const color>& colors_ref, bpp_mode bpp, int graphics_count) : sprite_item(shape_size, sprite_tiles_item(tiles_ref, bpp, graphics_count), sprite_palette_item(colors_ref, bpp))` (L56): Constructor.
  `shape_size` Shape and size of the output sprites.; `tiles_ref` Reference to one or more sprite tile sets.; `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of the output sprites.; `graphics_count` Number of sprite tile sets contained in tiles_ref.
  The tiles are not copied but referenced, so they should outlive the sprite_item to avoid dangling references.
  
  The colors are not copied but referenced, so they should outlive the sprite_item to avoid dangling references.
- **sprite_item** `constexpr sprite_item(const sprite_shape_size& shape_size, const sprite_tiles_item& tiles_item, const sprite_palette_item& palette_item)` (L69): Constructor.
  `shape_size` Shape and size of the output sprites.; `tiles_item` It creates the tiles of the output sprites.; `palette_item` It creates the color palette of the output sprites.
- **shape_size** `constexpr const sprite_shape_size& shape_size() const` (L84): Returns the shape and size of the output sprites.
- **tiles_item** `constexpr const sprite_tiles_item& tiles_item() const` (L92): Returns the item used to create the tiles of the output sprites.
- **palette_item** `constexpr const sprite_palette_item& palette_item() const` (L100): Returns the item used to create the color palette of the output sprites.
- **decompress** `sprite_item decompress( span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref) const` (L115): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref and the colors referenced by decompressed_colors_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; `decompressed_colors_ref` Destination of the decompressed colors.; devuelve: A sprite_item pointing to the decompressed tiles and colors.
  If the source and destination tiles or colors overlap, the behavior is undefined.
- **create_sprite** `sprite_ptr create_sprite() const` (L122): Creates a sprite_ptr using the information contained in this item.
  devuelve: The requested sprite_ptr.
- **create_sprite** `sprite_ptr create_sprite(int graphics_index) const` (L129): Creates a sprite_ptr using the information contained in this item.
  `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr.
- **create_sprite** `sprite_ptr create_sprite(fixed x, fixed y) const` (L137): Creates a sprite_ptr using the information contained in this item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; devuelve: The requested sprite_ptr.
- **create_sprite** `sprite_ptr create_sprite(fixed x, fixed y, int graphics_index) const` (L146): Creates a sprite_ptr using the information contained in this item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr.
- **create_sprite** `sprite_ptr create_sprite(const fixed_point& position) const` (L153): Creates a sprite_ptr using the information contained in this item.
  `position` Position of the sprite.; devuelve: The requested sprite_ptr.
- **create_sprite** `sprite_ptr create_sprite(const fixed_point& position, int graphics_index) const` (L161): Creates a sprite_ptr using the information contained in this item.
  `position` Position of the sprite.; `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional() const` (L167): Creates a sprite_ptr using the information contained in this item.
  devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional(int graphics_index) const` (L174): Creates a sprite_ptr using the information contained in this item.
  `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional(fixed x, fixed y) const` (L182): Creates a sprite_ptr using the information contained in this item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional(fixed x, fixed y, int graphics_index) const` (L191): Creates a sprite_ptr using the information contained in this item.
  `x` Horizontal position of the sprite.; `y` Vertical position of the sprite.; `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional(const fixed_point& position) const` (L198): Creates a sprite_ptr using the information contained in this item.
  `position` Position of the sprite.; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **create_sprite_optional** `optional<sprite_ptr> create_sprite_optional(const fixed_point& position, int graphics_index) const` (L206): Creates a sprite_ptr using the information contained in this item.
  `position` Position of the sprite.; `graphics_index` Index of the tile set to reference in tiles_item().; devuelve: The requested sprite_ptr if it could be allocated; bn::nullopt otherwise.
- **operator==** `constexpr friend bool operator==(const sprite_item& a, const sprite_item& b) = default` (L211): Default equal operator.

## bn_sprite_palette_item.h

### `bn::sprite_palette_item` (class, bn_sprite_palette_item.h:42)

Contains the required information to generate sprite color palettes.

`class sprite_palette_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `sprite` type.

The colors are not copied but referenced, so they should outlive the sprite_palette_item
to avoid dangling references.

- **sprite_palette_item** `constexpr sprite_palette_item(const span<const color>& colors_ref, bpp_mode bpp) : sprite_palette_item(colors_ref, bpp, compression_type::NONE)` (L55): Constructor.
  `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of the color palettes to create.
  The colors are not copied but referenced, so they should outlive the sprite_palette_item
  to avoid dangling references.
- **sprite_palette_item** `constexpr sprite_palette_item(const span<const color>& colors_ref, bpp_mode bpp, compression_type compression)` (L70): Constructor.
  `colors_ref` Reference to an array of multiples of 16 colors.; `bpp` Bits per pixel of the color palettes to create.; `compression` Compression type.
  The colors are not copied but referenced, so they should outlive the sprite_palette_item
  to avoid dangling references.
- **colors_ref** `constexpr const span<const color>& colors_ref() const` (L87): Returns the referenced array of multiples of 16 colors.
  The colors are not copied but referenced, so they should outlive the sprite_palette_item
  to avoid dangling references.
- **bpp** `constexpr bpp_mode bpp() const` (L95): Returns the bits per pixel of the color palettes to create.
- **compression** `constexpr compression_type compression() const` (L103): Returns the compression type.
- **decompress** `sprite_palette_item decompress(span<color> decompressed_colors_ref) const` (L116): Decompresses the stored data in the colors referenced by decompressed_colors_ref.
  `decompressed_colors_ref` Destination of the decompressed colors.; devuelve: A sprite_palette_item pointing to the decompressed colors.
  If the source and destination colors overlap, the behavior is undefined.
- **find_palette** `optional<sprite_palette_ptr> find_palette() const` (L123): Searches for a sprite_palette_ptr which contains the colors referenced by this item.
  devuelve: sprite_palette_ptr which contains the colors referenced by this item if it has been found; bn::nullopt otherwise.
- **create_palette** `sprite_palette_ptr create_palette() const` (L131): Searches for a sprite_palette_ptr which contains the colors referenced by this item. If it is not found, it creates a sprite_palette_ptr which contains them.
  devuelve: sprite_palette_ptr which contains the colors referenced by this item if it has been found; otherwise it returns a new sprite_palette_ptr which contains them.
- **create_new_palette** `sprite_palette_ptr create_new_palette() const` (L137): Creates a new sprite_palette_ptr which contains the colors referenced by this item.
  devuelve: sprite_palette_ptr which contains the colors referenced by this item.
- **create_palette_optional** `optional<sprite_palette_ptr> create_palette_optional() const` (L146): Searches for a sprite_palette_ptr which contains the colors referenced by this item. If it is not found, it creates a sprite_palette_ptr which contains them.
  devuelve: sprite_palette_ptr which contains the colors referenced by this item if it has been found; otherwise it returns a new sprite_palette_ptr which contains them it if it could be allocated; bn::nullopt otherwise.
- **create_new_palette_optional** `optional<sprite_palette_ptr> create_new_palette_optional() const` (L153): Creates a new sprite_palette_ptr which contains the colors referenced by this item.
  devuelve: sprite_palette_ptr which contains the colors referenced by this item it if it could be allocated; bn::nullopt otherwise.
- **operator==** `constexpr friend bool operator==(const sprite_palette_item& a, const sprite_palette_item& b) = default` (L158): Default equal operator.

## bn_sprite_tiles_item.h

### `bn::sprite_tiles_item` (class, bn_sprite_tiles_item.h:41)

Contains the required information to generate sprite tiles.

`class sprite_tiles_item`

The assets conversion tools generate an object of this type in the build folder for each *.bmp file
with `sprite` type.

The tiles are not copied but referenced, so they should outlive the sprite_tiles_item to avoid dangling references.

- **valid_tiles_count** `constexpr static bool valid_tiles_count(int tiles_count, bpp_mode bpp)` (L48): Indicates if the specified tiles count are valid for the specified bits per pixel or not.
- **sprite_tiles_item** `constexpr sprite_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp) : sprite_tiles_item(tiles_ref, bpp, compression_type::NONE)` (L69): Constructor.
  `tiles_ref` Reference to one or more sprite tile sets.; `bpp` tiles_ref bits per pixel.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
  to avoid dangling references.
- **sprite_tiles_item** `constexpr sprite_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp, compression_type compression)` (L84): Constructor.
  `tiles_ref` Reference to one or more sprite tile sets.; `bpp` tiles_ref bits per pixel.; `compression` Compression type.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
  to avoid dangling references.
- **sprite_tiles_item** `constexpr sprite_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp, int graphics_count) : sprite_tiles_item(tiles_ref, bpp, compression_type::NONE, graphics_count)` (L105): Constructor.
  `tiles_ref` Reference to one or more sprite tile sets.; `bpp` tiles_ref bits per pixel.; `graphics_count` Number of sprite tile sets contained in tiles_ref.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
  to avoid dangling references.
- **sprite_tiles_item** `constexpr sprite_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp, compression_type compression, int graphics_count)` (L121): Constructor.
  `tiles_ref` Reference to one or more sprite tile sets.; `bpp` tiles_ref bits per pixel.; `graphics_count` Number of sprite tile sets contained in tiles_ref.; `compression` Compression type.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
  to avoid dangling references.
- **tiles_ref** `constexpr const span<const tile>& tiles_ref() const` (L146): Returns the reference to one or more sprite tile sets.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
  to avoid dangling references.
- **bpp** `constexpr bpp_mode bpp() const` (L154): Returns the bits per pixel of the referenced tiles.
- **graphics_count** `constexpr int graphics_count() const` (L162): Returns the number of sprite tile sets contained in tiles_ref.
- **tiles_count_per_graphic** `constexpr int tiles_count_per_graphic() const` (L170): Returns the number of sprite tiles contained in each sprite tile set.
- **graphics_tiles_ref** `constexpr span<const tile> graphics_tiles_ref() const` (L178): Returns the reference to the first sprite tile set.
- **graphics_tiles_ref** `constexpr span<const tile> graphics_tiles_ref(int graphics_index) const` (L189): Returns the reference to the sprite tile set indicated by graphics_index.
- **compression** `constexpr compression_type compression() const` (L203): Returns the compression type.
- **decompress** `sprite_tiles_item decompress(span<tile> decompressed_tiles_ref) const` (L216): Decompresses the stored data in the tiles referenced by decompressed_tiles_ref.
  `decompressed_tiles_ref` Destination of the decompressed tiles.; devuelve: A sprite_tiles_item pointing to the decompressed tiles.
  If the source and destination tiles overlap, the behavior is undefined.
- **find_tiles** `optional<sprite_tiles_ptr> find_tiles() const` (L223): Searches for a sprite_tiles_ptr which references the first sprite tile set.
  devuelve: sprite_tiles_ptr which references the first sprite tile set if it has been found; bn::nullopt otherwise.
- **find_tiles** `optional<sprite_tiles_ptr> find_tiles(int graphics_index) const` (L231): Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
  `graphics_index` Index of the tile set to search.; devuelve: sprite_tiles_ptr which references the indicated sprite tile set if it has been found; bn::nullopt otherwise.
- **create_tiles** `sprite_tiles_ptr create_tiles() const` (L243): Searches for a sprite_tiles_ptr which references the first sprite tile set. If it is not found, it creates a sprite_tiles_ptr which references it.
  devuelve: sprite_tiles_ptr which references the first sprite tile set if it has been found; otherwise it returns a sprite_tiles_ptr which references it.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
  to avoid dangling references.
- **create_tiles** `sprite_tiles_ptr create_tiles(int graphics_index) const` (L256): Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index. If it is not found, it creates a sprite_tiles_ptr which references it.
  `graphics_index` Index of the tile set to search or handle.; devuelve: sprite_tiles_ptr which references the sprite tile set indicated by graphics_index if it has been found; otherwise it returns a sprite_tiles_ptr which references it.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<sprite_tiles_ptr> create_tiles_optional() const` (L279): Searches for a sprite_tiles_ptr which references the first sprite tile set. If it is not found, it creates a sprite_tiles_ptr which references it.
  devuelve: sprite_tiles_ptr which references the first sprite tile set if it has been found; otherwise it returns a new sprite_tiles_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
  to avoid dangling references.
- **create_tiles_optional** `optional<sprite_tiles_ptr> create_tiles_optional(int graphics_index) const` (L293): Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index. If it is not found, it creates a sprite_tiles_ptr which references it.
  `graphics_index` Index of the tile set to search or handle.; devuelve: sprite_tiles_ptr which references the sprite tile set indicated by graphics_index if it has been found; otherwise it returns a new sprite_tiles_ptr which references it if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
  to avoid dangling references.
- **operator==** `constexpr friend bool operator==(const sprite_tiles_item& a, const sprite_tiles_item& b)` (L311): Equal operator.
  `a` First sprite_tiles_item to compare.; `b` Second sprite_tiles_item to compare.; devuelve: `true` if the first sprite_tiles_item is equal to the second one, otherwise `false`.
