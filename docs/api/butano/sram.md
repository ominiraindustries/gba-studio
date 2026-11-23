# Butano 21.10.1 — Game Pak SRAM (`sram`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [game_pak](game_pak.md)

## bn_config_sram.h

### `BN_CFG_SRAM_WAIT_STATE` (macro, bn_config_sram.h:28)

Specifies SRAM access time in clock cycles.

`#define BN_CFG_SRAM_WAIT_STATE BN_SRAM_WAIT_STATE_8`

Values not specified in BN_SRAM_WAIT_STATE_* macros are not allowed.


## bn_sram.h

### `bn::sram` (namespace, bn_sram.h:37)

SRAM related functions.

- **size** `constexpr int size()` (L42): Returns the total SRAM size in bytes.
- **read** `template<typename Type> void read(Type& destination)` (L51): Copies SRAM data into the given value.
  `destination` SRAM data is copied into this trivially copyable value.
- **read_offset** `template<typename Type> void read_offset(Type& destination, int offset)` (L65): Copies SRAM data into the given value.
  `destination` SRAM data is copied into this trivially copyable value.; `offset` Copying starts from SRAM start address + this offset.
- **read_span** `void read_span(span<uint8_t>& destination)` (L81): Copies SRAM data into the given span.
  `destination` SRAM data is copied into this span.
- **read_span_offset** `void read_span_offset(span<uint8_t>& destination, int offset)` (L88): Copies SRAM data into the given span.
  `destination` SRAM data is copied into this span.; `offset` Copying starts from SRAM start address + this offset.
- **write** `template<typename Type> void write(const Type& source)` (L94): Copies the given value into SRAM.
  `source` Value to copy. It must be trivially copyable.
- **write_offset** `template<typename Type> void write_offset(const Type& source, int offset)` (L108): Copies the given value into SRAM.
  `source` Value to copy. It must be trivially copyable.; `offset` The given value is copied into SRAM start address + this offset.
- **write_span** `void write_span(const span<const uint8_t>& source)` (L124): Copies the given span into SRAM.
  `source` Span to copy.
- **write_span_offset** `void write_span_offset(const span<const uint8_t>& source, int offset)` (L131): Copies the given span into SRAM.
  `source` Span to copy.; `offset` The given span is copied into SRAM start address + this offset.
- **clear** `void clear(int bytes)` (L137): Clears (fills with zero) SRAM.
  `bytes` Number of bytes to clear.
- **clear** `void clear(int bytes, int offset)` (L144): Clears (fills with zero) SRAM.
  `bytes` Number of bytes to clear.; `offset` Clear starts from SRAM start address + this offset.
- **set_bytes** `void set_bytes(uint8_t value, int bytes)` (L151): Fills SRAM with the given value.
  `value` Value to fill with.; `bytes` Number of bytes to fill.
- **set_bytes** `void set_bytes(uint8_t value, int bytes, int offset)` (L159): Fills SRAM with the given value.
  `value` Value to fill with.; `bytes` Number of bytes to fill.; `offset` Fill starts from SRAM start address + this offset.

## bn_sram_wait_state.h

### `BN_SRAM_WAIT_STATE_4` (macro, bn_sram_wait_state.h:25)

SRAM access time is 4+1 clock cycles.

`#define BN_SRAM_WAIT_STATE_4 0x0000`

### `BN_SRAM_WAIT_STATE_3` (macro, bn_sram_wait_state.h:34)

SRAM access time is 3+1 clock cycles.

`#define BN_SRAM_WAIT_STATE_3 0x0001`

### `BN_SRAM_WAIT_STATE_2` (macro, bn_sram_wait_state.h:43)

SRAM access time is 2+1 clock cycles.

`#define BN_SRAM_WAIT_STATE_2 0x0002`

### `BN_SRAM_WAIT_STATE_8` (macro, bn_sram_wait_state.h:52)

SRAM access time is 8+1 clock cycles.

`#define BN_SRAM_WAIT_STATE_8 0x0003`

