# Butano 21.10.1 — Backgrounds (`bg`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_bgs.h

### `bn::bgs` (namespace, bn_bgs.h:23)

Backgrounds related functions.

- **used_items_count** `int used_items_count()` (L30): Returns the number of used background items.
  A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.
- **available_items_count** `int available_items_count()` (L37): Returns the number of available background items.
  A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.

## bn_config_bg_blocks.h

### `BN_CFG_BG_BLOCKS_MAX_ITEMS` (macro, bn_config_bg_blocks.h:26)

Specifies the maximum number of background tile sets and maps that can be created.

`#define BN_CFG_BG_BLOCKS_MAX_ITEMS 16`

### `BN_CFG_BG_BLOCKS_MAX_OVERWRITE_TILES` (macro, bn_config_bg_blocks.h:38)

Specifies the maximum number of background tiles that can be overwritten with bn::regular_bg_tiles_ptr::overwrite_tile and bn::affine_bg_tiles_ptr::overwrite_tile.

`#define BN_CFG_BG_BLOCKS_MAX_OVERWRITE_TILES 32`

### `BN_CFG_BG_BLOCKS_LOG_ENABLED` (macro, bn_config_bg_blocks.h:51)

Specifies if the background blocks manager must be logged in detail or not.

`#define BN_CFG_BG_BLOCKS_LOG_ENABLED false`

Keep in mind that it is very slow, it is useful only for debugging.

### `BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED` (macro, bn_config_bg_blocks.h:64)

Specifies if the background blocks manager sanity check is enabled or not.

`#define BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED false`

Sanity check asserts if the internal state of the manager is valid.


## bn_config_bgs.h

### `BN_CFG_BGS_MAX_ITEMS` (macro, bn_config_bgs.h:28)

Specifies the maximum number of background items that can be created.

`#define BN_CFG_BGS_MAX_ITEMS 4`

A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.

