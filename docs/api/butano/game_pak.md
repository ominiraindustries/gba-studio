# Butano 21.10.1 — Game Pak (`game_pak`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_game_pak.h

### `BN_CFG_GAME_PAK_PREFETCH_ENABLED` (macro, bn_config_game_pak.h:29)

Specifies if Game Pak prefetch is enabled or not.

`#define BN_CFG_GAME_PAK_PREFETCH_ENABLED true`

When Game Pak prefetch is enabled, the GBA attempts to read opcodes from the Game Pak
when the CPU is not using the bus (if any).

### `BN_CFG_GAME_PAK_WAIT_STATE_FIRST` (macro, bn_config_game_pak.h:42)

Specifies Game Pak access time in clock cycles for non-sequential accesses.

`#define BN_CFG_GAME_PAK_WAIT_STATE_FIRST BN_GAME_PAK_WAIT_STATE_FIRST_AUTO`

Values not specified in BN_GAME_PAK_WAIT_STATE_FIRST_* macros are not allowed.

### `BN_CFG_GAME_PAK_WAIT_STATE_SECOND` (macro, bn_config_game_pak.h:55)

Specifies Game Pak access time in clock cycles for sequential accesses.

`#define BN_CFG_GAME_PAK_WAIT_STATE_SECOND BN_GAME_PAK_WAIT_STATE_SECOND_AUTO`

Values not specified in BN_GAME_PAK_WAIT_STATE_SECOND_* macros are not allowed.


## bn_game_pak_wait_state.h

### `BN_GAME_PAK_WAIT_STATE_FIRST_4` (macro, bn_game_pak_wait_state.h:25)

Game Pak access time is 4+1 clock cycles for non-sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_FIRST_4 0x0000`

### `BN_GAME_PAK_WAIT_STATE_FIRST_3` (macro, bn_game_pak_wait_state.h:36)

Game Pak access time is 3+1 clock cycles for non-sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_FIRST_3 0x0004`

This setting is not supported by slow flash carts like the SuperCard SD.

### `BN_GAME_PAK_WAIT_STATE_FIRST_2` (macro, bn_game_pak_wait_state.h:47)

Game Pak access time is 2+1 clock cycles for non-sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_FIRST_2 0x0008`

This setting is not supported by the Nintendo 3DS or slow flash carts like the SuperCard SD.

### `BN_GAME_PAK_WAIT_STATE_FIRST_8` (macro, bn_game_pak_wait_state.h:56)

Game Pak access time is 8+1 clock cycles for non-sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_FIRST_8 0x000C`

### `BN_GAME_PAK_WAIT_STATE_FIRST_AUTO` (macro, bn_game_pak_wait_state.h:66)

Game Pak access time is 4+1 clock cycles for non-sequential accesses if a slow flash cart like the SuperCard SD has been detected; 3+1 otherwise.

`#define BN_GAME_PAK_WAIT_STATE_FIRST_AUTO (-1)`

### `BN_GAME_PAK_WAIT_STATE_SECOND_2` (macro, bn_game_pak_wait_state.h:75)

Game Pak access time is 2+1 clock cycles for sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_SECOND_2 0x0000`

### `BN_GAME_PAK_WAIT_STATE_SECOND_1` (macro, bn_game_pak_wait_state.h:86)

Game Pak access time is 1+1 clock cycles for sequential accesses.

`#define BN_GAME_PAK_WAIT_STATE_SECOND_1 0x0010`

This setting is not supported by slow flash carts like the SuperCard SD.

### `BN_GAME_PAK_WAIT_STATE_SECOND_AUTO` (macro, bn_game_pak_wait_state.h:96)

Game Pak access time is 2+1 clock cycles for sequential accesses if a slow flash cart like the SuperCard SD has been detected; 1+1 otherwise.

`#define BN_GAME_PAK_WAIT_STATE_SECOND_AUTO (-1)`

