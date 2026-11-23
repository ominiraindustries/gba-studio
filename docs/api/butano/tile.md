# Butano 21.10.1 — Tiles (`tile`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_affine_bg_tiles_ptr.h

### `bn::affine_bg_tiles_ptr` (class, bn_affine_bg_tiles_ptr.h:40)

std::shared_ptr like smart pointer that retains shared ownership of the tiles of an affine background.

`class affine_bg_tiles_ptr`

Several affine_bg_tiles_ptr objects may own the same background tiles.

The background tiles are released when the last remaining affine_bg_tiles_ptr owning them is destroyed.

The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
to avoid dangling references.

- **find** `static optional<affine_bg_tiles_ptr> find(const affine_bg_tiles_item& tiles_item)` (L50): Searches for an affine_bg_tiles_ptr which references the given tiles.
  `tiles_item` affine_bg_tiles_item which references the tiles to search.; devuelve: affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; bn::nullopt otherwise.
- **create** `static affine_bg_tiles_ptr create(const affine_bg_tiles_item& tiles_item)` (L66): Searches for an affine_bg_tiles_ptr which references the given tiles. If it is not found, it creates an affine_bg_tiles_ptr which references them.
  `tiles_item` affine_bg_tiles_item which references the tiles to search or handle.; devuelve: affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns an affine_bg_tiles_ptr which references them.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **create** `static affine_bg_tiles_ptr create(const affine_bg_tiles_item& tiles_item, bool allow_offset)` (L81): Searches for an affine_bg_tiles_ptr which references the given tiles. If it is not found, it creates an affine_bg_tiles_ptr which references them.
  `tiles_item` affine_bg_tiles_item which references the tiles to search or handle.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns an affine_bg_tiles_ptr which references them.
  The tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **allocate** `static affine_bg_tiles_ptr allocate(int tiles_count)` (L102): Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; devuelve: affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
- **allocate** `static affine_bg_tiles_ptr allocate(int tiles_count, bool allow_offset)` (L111): Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
- **create_optional** `static optional<affine_bg_tiles_ptr> create_optional(const affine_bg_tiles_item& tiles_item)` (L128): Searches for an affine_bg_tiles_ptr which references the given tiles. If it is not found, it creates an affine_bg_tiles_ptr which references them.
  `tiles_item` affine_bg_tiles_item which references the tiles to search or handle.; devuelve: affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns an affine_bg_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_tiles_ptr> create_optional( const affine_bg_tiles_item& tiles_item, bool allow_offset)` (L144): Searches for an affine_bg_tiles_ptr which references the given tiles. If it is not found, it creates an affine_bg_tiles_ptr which references them.
  `tiles_item` affine_bg_tiles_item which references the tiles to search or handle.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns an affine_bg_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **allocate_optional** `static optional<affine_bg_tiles_ptr> allocate_optional(int tiles_count)` (L167): Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; devuelve: affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
- **allocate_optional** `static optional<affine_bg_tiles_ptr> allocate_optional(int tiles_count, bool allow_offset)` (L177): Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen if it could be allocated; bn::nullopt otherwise.
- **affine_bg_tiles_ptr** `affine_bg_tiles_ptr(const affine_bg_tiles_ptr& other)` (L183): Copy constructor.
  `other` affine_bg_tiles_ptr to copy.
- **operator=** `affine_bg_tiles_ptr& operator=(const affine_bg_tiles_ptr& other)` (L190): Copy assignment operator.
  `other` affine_bg_tiles_ptr to copy.; devuelve: Reference to this.
- **affine_bg_tiles_ptr** `affine_bg_tiles_ptr(affine_bg_tiles_ptr&& other) noexcept : affine_bg_tiles_ptr(other._handle)` (L196): Move constructor.
  `other` affine_bg_tiles_ptr to move.
- **operator=** `affine_bg_tiles_ptr& operator=(affine_bg_tiles_ptr&& other) noexcept` (L207): Move assignment operator.
  `other` affine_bg_tiles_ptr to move.; devuelve: Reference to this.
- **~affine_bg_tiles_ptr** `~affine_bg_tiles_ptr()` (L216): Releases the referenced background tiles if no more affine_bg_tiles_ptr objects reference to them.
- **id** `int id() const` (L221): Returns the internal id.
- **cbb** `int cbb() const` (L226): Returns the character base block of the referenced tiles.
- **tiles_count** `int tiles_count() const` (L231): Returns the referenced number of tiles.
- **offset** `int offset() const` (L236): Returns how many tiles to offset in the cells of a map using these tiles before writing them in VRAM.
- **compression** `compression_type compression() const` (L241): Returns the compression of the referenced tiles.
- **tiles_ref** `optional<span<const tile>> tiles_ref() const` (L247): Returns the referenced tiles unless it was created with allocate or allocate_optional. In that case, it returns bn::nullopt.
- **set_tiles_ref** `void set_tiles_ref(const affine_bg_tiles_item& tiles_item)` (L260): Sets the tiles to handle.
  `tiles_item` affine_bg_tiles_item which references the tiles to handle.
  Remember that the background tiles system does not support multiple affine_bg_tiles_ptr items
  referencing to the same tiles.
  
  Remember also that the tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **reload_tiles_ref** `void reload_tiles_ref()` (L265): Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
- **overwrite_tile** `void overwrite_tile(int tile_index, const tile& tiles_ref)` (L277): Overwrites a single tile.
  `tile_index` Index of the tile to overwrite.; `tiles_ref` Reference to the new tile data. It must point to two tiles, because an affine tile counts as two bn::tile objects.
  Remember that the tiles are not copied but referenced,
  so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
- **vram** `optional<span<const tile>> vram() const` (L283): Returns the allocated memory in VRAM if this affine_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **vram** `optional<span<tile>> vram()` (L289): Returns the allocated memory in VRAM if this affine_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **handle** `int handle() const` (L294): Returns the internal handle.
- **swap** `void swap(affine_bg_tiles_ptr& other)` (L303): Exchanges the contents of this affine_bg_tiles_ptr with those of the other one.
  `other` affine_bg_tiles_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_tiles_ptr& a, affine_bg_tiles_ptr& b)` (L313): Exchanges the contents of an affine_bg_tiles_ptr with those of another one.
  `a` First affine_bg_tiles_ptr to exchange the contents with.; `b` Second affine_bg_tiles_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const affine_bg_tiles_ptr& a, const affine_bg_tiles_ptr& b) = default` (L321): Default equal operator.

## bn_bg_tiles.h

### `bn::bg_tiles` (namespace, bn_bg_tiles.h:25)

Background tiles related functions.

- **used_tiles_count** `int used_tiles_count()` (L30): Returns the number of used background tiles.
- **available_tiles_count** `int available_tiles_count()` (L35): Returns the number of available background tiles.
- **used_blocks_count** `int used_blocks_count()` (L40): Returns the number of used background VRAM blocks by background tiles.
- **available_blocks_count** `int available_blocks_count()` (L45): Returns the number of available background VRAM blocks.
- **allow_offset** `bool allow_offset()` (L51): Specifies if tiles offset is allowed to improve VRAM usage when creating regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
- **set_allow_offset** `void set_allow_offset(bool allow_offset)` (L57): Sets if tiles offset is allowed to improve VRAM usage when creating regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
- **log_status** `void log_status()` (L62): Logs the current status of the background blocks manager.

## bn_regular_bg_tiles_ptr.h

### `bn::regular_bg_tiles_ptr` (class, bn_regular_bg_tiles_ptr.h:41)

std::shared_ptr like smart pointer that retains shared ownership of the tiles of a regular background.

`class regular_bg_tiles_ptr`

Several regular_bg_tiles_ptr objects may own the same background tiles.

The background tiles are released when the last remaining regular_bg_tiles_ptr owning them is destroyed.

The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
to avoid dangling references.

- **find** `static optional<regular_bg_tiles_ptr> find(const regular_bg_tiles_item& tiles_item)` (L51): Searches for a regular_bg_tiles_ptr which references the given tiles.
  `tiles_item` regular_bg_tiles_item which references the tiles to search.; devuelve: regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; bn::nullopt otherwise.
- **create** `static regular_bg_tiles_ptr create(const regular_bg_tiles_item& tiles_item)` (L67): Searches for a regular_bg_tiles_ptr which references the given tiles. If it is not found, it creates a regular_bg_tiles_ptr which references them.
  `tiles_item` regular_bg_tiles_item which references the tiles to search or handle.; devuelve: regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a regular_bg_tiles_ptr which references them.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **create** `static regular_bg_tiles_ptr create(const regular_bg_tiles_item& tiles_item, bool allow_offset)` (L82): Searches for a regular_bg_tiles_ptr which references the given tiles. If it is not found, it creates a regular_bg_tiles_ptr which references them.
  `tiles_item` regular_bg_tiles_item which references the tiles to search or handle.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a regular_bg_tiles_ptr which references them.
  The tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **allocate** `static regular_bg_tiles_ptr allocate(int tiles_count, bpp_mode bpp)` (L104): Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; devuelve: regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
- **allocate** `static regular_bg_tiles_ptr allocate(int tiles_count, bpp_mode bpp, bool allow_offset)` (L114): Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
- **create_optional** `static optional<regular_bg_tiles_ptr> create_optional(const regular_bg_tiles_item& tiles_item)` (L131): Searches for a regular_bg_tiles_ptr which references the given tiles. If it is not found, it creates a regular_bg_tiles_ptr which references them.
  `tiles_item` regular_bg_tiles_item which references the tiles to search or handle.; devuelve: regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a regular_bg_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
  
  The tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **create_optional** `static optional<regular_bg_tiles_ptr> create_optional( const regular_bg_tiles_item& tiles_item, bool allow_offset)` (L147): Searches for a regular_bg_tiles_ptr which references the given tiles. If it is not found, it creates a regular_bg_tiles_ptr which references them.
  `tiles_item` regular_bg_tiles_item which references the tiles to search or handle.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a regular_bg_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **allocate_optional** `static optional<regular_bg_tiles_ptr> allocate_optional(int tiles_count, bpp_mode bpp)` (L171): Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; devuelve: regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen if it could be allocated; bn::nullopt otherwise.
  Tiles offset is allowed to improve VRAM usage if bg_tiles::allow_offset says so.
  You should probably disable it if a dynamic map is going to use these tiles.
- **allocate_optional** `static optional<regular_bg_tiles_ptr> allocate_optional( int tiles_count, bpp_mode bpp, bool allow_offset)` (L182): Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; `allow_offset` Indicates if tiles offset is allowed to improve VRAM usage. You should probably disable it if a dynamic map is going to use these tiles.; devuelve: regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen if it could be allocated; bn::nullopt otherwise.
- **regular_bg_tiles_ptr** `regular_bg_tiles_ptr(const regular_bg_tiles_ptr& other)` (L189): Copy constructor.
  `other` regular_bg_tiles_ptr to copy.
- **operator=** `regular_bg_tiles_ptr& operator=(const regular_bg_tiles_ptr& other)` (L196): Copy assignment operator.
  `other` regular_bg_tiles_ptr to copy.; devuelve: Reference to this.
- **regular_bg_tiles_ptr** `regular_bg_tiles_ptr(regular_bg_tiles_ptr&& other) noexcept : regular_bg_tiles_ptr(other._handle)` (L202): Move constructor.
  `other` regular_bg_tiles_ptr to move.
- **operator=** `regular_bg_tiles_ptr& operator=(regular_bg_tiles_ptr&& other) noexcept` (L213): Move assignment operator.
  `other` regular_bg_tiles_ptr to move.; devuelve: Reference to this.
- **~regular_bg_tiles_ptr** `~regular_bg_tiles_ptr()` (L222): Releases the referenced background tiles if no more regular_bg_tiles_ptr objects reference to them.
- **id** `int id() const` (L227): Returns the internal id.
- **cbb** `int cbb() const` (L232): Returns the character base block of the referenced tiles.
- **tiles_count** `int tiles_count() const` (L237): Returns the referenced number of tiles.
- **bpp** `bpp_mode bpp() const` (L242): Returns the bits per pixel of the referenced tiles.
- **offset** `int offset() const` (L247): Returns how many tiles to offset in the cells of a map using these tiles before writing them in VRAM.
- **compression** `compression_type compression() const` (L252): Returns the compression of the referenced tiles.
- **tiles_ref** `optional<span<const tile>> tiles_ref() const` (L258): Returns the referenced tiles unless it was created with allocate or allocate_optional. In that case, it returns bn::nullopt.
- **set_tiles_ref** `void set_tiles_ref(const regular_bg_tiles_item& tiles_item)` (L271): Sets the tiles to handle.
  `tiles_item` regular_bg_tiles_item which references the tiles to handle.
  Remember that the background tiles system does not support multiple regular_bg_tiles_ptr items
  referencing to the same tiles.
  
  Remember also that the tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **reload_tiles_ref** `void reload_tiles_ref()` (L276): Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
- **overwrite_tile** `void overwrite_tile(int tile_index, const tile& tiles_ref)` (L288): Overwrites a single tile.
  `tile_index` Index of the tile to overwrite.; `tiles_ref` Reference to the new tile data. If bpp() == bpp_mode::BPP_8, it must point to two tiles, because a bpp_mode::BPP_8 tile counts as two bn::tile objects.
  Remember that the tiles are not copied but referenced,
  so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
- **vram** `optional<span<const tile>> vram() const` (L294): Returns the allocated memory in VRAM if this regular_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **vram** `optional<span<tile>> vram()` (L300): Returns the allocated memory in VRAM if this regular_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **handle** `int handle() const` (L305): Returns the internal handle.
- **swap** `void swap(regular_bg_tiles_ptr& other)` (L314): Exchanges the contents of this regular_bg_tiles_ptr with those of the other one.
  `other` regular_bg_tiles_ptr to exchange the contents with.
- **swap** `friend void swap(regular_bg_tiles_ptr& a, regular_bg_tiles_ptr& b)` (L324): Exchanges the contents of a regular_bg_tiles_ptr with those of another one.
  `a` First regular_bg_tiles_ptr to exchange the contents with.; `b` Second regular_bg_tiles_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const regular_bg_tiles_ptr& a, const regular_bg_tiles_ptr& b) = default` (L332): Default equal operator.

## bn_sprite_tiles.h

### `bn::sprite_tiles` (namespace, bn_sprite_tiles.h:25)

Sprite tiles related functions.

- **used_tiles_count** `int used_tiles_count()` (L30): Returns the number of used sprite tiles managed with sprite_tiles_ptr objects.
- **available_tiles_count** `int available_tiles_count()` (L35): Returns the number of available sprite tiles that can be managed with sprite_tiles_ptr objects.
- **used_items_count** `int used_items_count()` (L40): Returns the number of used sprite tile sets created with sprite_tiles_ptr static constructors.
- **available_items_count** `int available_items_count()` (L46): Returns the number of available sprite tile sets that can be created with sprite_tiles_ptr static constructors.
- **log_status** `void log_status()` (L51): Logs the current status of the sprite tiles manager.

## bn_sprite_tiles_ptr.h

### `bn::sprite_tiles_ptr` (class, bn_sprite_tiles_ptr.h:40)

std::shared_ptr like smart pointer that retains shared ownership of the tiles of a sprite.

`class sprite_tiles_ptr`

Several sprite_tiles_ptr objects may own the same sprite tiles.

The sprite tiles are released when the last remaining sprite_tiles_ptr owning them is destroyed.

The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr to avoid dangling references.

- **find** `static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item)` (L50): Searches for a sprite_tiles_ptr which references the given tiles.
  `tiles_item` sprite_tiles_item which references the tiles to search.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; bn::nullopt otherwise.
- **find** `static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item, int graphics_index)` (L59): Searches for a sprite_tiles_ptr which references the given tiles.
  `tiles_item` sprite_tiles_item which references the tiles to search.; `graphics_index` Index of the tile set to search in sprite_tiles_item.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found; bn::nullopt otherwise.
- **create** `static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item)` (L72): Searches for a sprite_tiles_ptr which references the given tiles. If it is not found, it creates a sprite_tiles_ptr which references them.
  `tiles_item` sprite_tiles_item which references the tiles to search or handle.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a sprite_tiles_ptr which references them.
  The tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **create** `static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item, int graphics_index)` (L86): Searches for a sprite_tiles_ptr which references the given tiles. If it is not found, it creates a sprite_tiles_ptr which references them.
  `tiles_item` sprite_tiles_item which references the tiles to search or handle.; `graphics_index` Index of the tile set to search in sprite_tiles_item.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found; otherwise it returns a sprite_tiles_ptr which references them.
  The tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **allocate** `static sprite_tiles_ptr allocate(int tiles_count, bpp_mode bpp)` (L110): Creates a sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; devuelve: sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
- **create_optional** `static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item)` (L123): Searches for a sprite_tiles_ptr which references the given tiles. If it is not found, it creates a sprite_tiles_ptr which references them.
  `tiles_item` sprite_tiles_item which references the tiles to search or handle.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found; otherwise it returns a sprite_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item, int graphics_index)` (L137): Searches for a sprite_tiles_ptr which references the given tiles. If it is not found, it creates a sprite_tiles_ptr which references them.
  `tiles_item` sprite_tiles_item which references the tiles to search or handle.; `graphics_index` Index of the tile set to search in sprite_tiles_item.; devuelve: sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found; otherwise it returns a sprite_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
  The tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **allocate_optional** `static optional<sprite_tiles_ptr> allocate_optional(int tiles_count, bpp_mode bpp)` (L164): Creates a sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
  `tiles_count` Number of tiles to allocate.; `bpp` Bits per pixel of the tiles to allocate.; devuelve: sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen if it could be allocated; bn::nullopt otherwise.
- **sprite_tiles_ptr** `sprite_tiles_ptr(const sprite_tiles_ptr& other)` (L170): Copy constructor.
  `other` sprite_tiles_ptr to copy.
- **operator=** `sprite_tiles_ptr& operator=(const sprite_tiles_ptr& other)` (L177): Copy assignment operator.
  `other` sprite_tiles_ptr to copy.; devuelve: Reference to this.
- **sprite_tiles_ptr** `sprite_tiles_ptr(sprite_tiles_ptr&& other) noexcept : sprite_tiles_ptr(other._handle)` (L183): Move constructor.
  `other` sprite_tiles_ptr to move.
- **operator=** `sprite_tiles_ptr& operator=(sprite_tiles_ptr&& other) noexcept` (L194): Move assignment operator.
  `other` sprite_tiles_ptr to move.; devuelve: Reference to this.
- **~sprite_tiles_ptr** `~sprite_tiles_ptr()` (L203): Releases the referenced sprite tiles if no more sprite_tiles_ptr objects reference to them.
- **id** `int id() const` (L208): Returns the internal id.
- **tiles_count** `int tiles_count() const` (L213): Returns the referenced number of tiles.
- **compression** `compression_type compression() const` (L218): Returns the compression of the referenced tiles.
- **tiles_ref** `optional<span<const tile>> tiles_ref() const` (L224): Returns the referenced tiles unless it was created with allocate or allocate_optional. In that case, it returns bn::nullopt.
- **set_tiles_ref** `void set_tiles_ref(const sprite_tiles_item& tiles_item)` (L237): Sets the tiles to handle.
  `tiles_item` sprite_tiles_item which references the tiles to handle.
  Remember that the sprite tiles system does not support multiple sprite_tiles_ptr items
  referencing to the same tiles.
  
  Remember also that the tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **set_tiles_ref** `void set_tiles_ref(const sprite_tiles_item& tiles_item, int graphics_index)` (L251): Sets the tiles to handle.
  `tiles_item` sprite_tiles_item which references the tiles to handle.; `graphics_index` Index of the tile set to reference in sprite_tiles_item.
  Remember that the sprite tiles system does not support multiple sprite_tiles_ptr items
  referencing to the same tiles.
  
  Remember also that the tiles are not copied but referenced,
  so they should outlive the sprite_tiles_ptr to avoid dangling references.
- **reload_tiles_ref** `void reload_tiles_ref()` (L256): Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
- **vram** `optional<span<const tile>> vram() const` (L262): Returns the allocated memory in VRAM if this sprite_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **vram** `optional<span<tile>> vram()` (L268): Returns the allocated memory in VRAM if this sprite_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
- **handle** `int handle() const` (L273): Returns the internal handle.
- **swap** `void swap(sprite_tiles_ptr& other)` (L282): Exchanges the contents of this sprite_tiles_ptr with those of the other one.
  `other` sprite_tiles_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_tiles_ptr& a, sprite_tiles_ptr& b)` (L292): Exchanges the contents of a sprite_tiles_ptr with those of another one.
  `a` First sprite_tiles_ptr to exchange the contents with.; `b` Second sprite_tiles_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b) = default` (L300): Default equal operator.

## bn_tile.h

### `bn::tile` (class, bn_tile.h:26)

Represents a 8x8 block of 16 color pixels.

`class tile`

