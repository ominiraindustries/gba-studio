# Butano 21.10.1 — H-Blank direct memory access (`hdma`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_config_hdma.h

### `BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED` (macro, bn_config_hdma.h:29)

Specifies if the high priority HDMA IRQ is enabled or not.

`#define BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED 0`


## bn_hdma.h

### `bn::hdma` (namespace, bn_hdma.h:23)

H-Blank direct memory access related functions.

- **running** `bool running()` (L30): Indicates if HDMA is active or not.
- **start** `void start(const bn::span<const uint16_t>& source_ref, uint16_t& destination_ref)` (L45): Start copying each frame the given amount of elements from the memory location referenced by source_ref to the memory location referenced by destination_ref.
  `source_ref` Const reference to the memory location to copy from. It must refer to a multiple of bn::display::height() values.; `destination_ref` Reference to the memory location to copy to.
  The elements are not copied but referenced,
  so they should be alive while HDMA is running to avoid dangling references.
  
  If the elements overlap, the behavior is undefined.
- **stop** `void stop()` (L57): Stops copying elements each frame.
- **high_priority_running** `bool high_priority_running()` (L64): Indicates if high priority HDMA is active or not.
  High priority HDMA can cause issues with audio, so avoid it unless necessary.
- **high_priority_start** `void high_priority_start(const bn::span<const uint16_t>& source_ref, uint16_t& destination_ref)` (L81): Start copying each frame with high priority the given amount of elements from the memory location referenced by source_ref to the memory location referenced by destination_ref.
  `source_ref` Const reference to the memory location to copy from. It must refer to a multiple of bn::display::height() values.; `destination_ref` Reference to the memory location to copy to.
  The elements are not copied but referenced,
  so they should be alive while HDMA is running to avoid dangling references.
  
  If the elements overlap, the behavior is undefined.
  
  High priority HDMA can cause issues with audio, so avoid it unless necessary.
- **high_priority_stop** `void high_priority_stop()` (L95): Stops copying elements each frame with high priority.
  High priority HDMA can cause issues with audio, so avoid it unless necessary.
- **high_priority_interrupt_handler** `interrupt_handler_type high_priority_interrupt_handler()` (L100): Returns the handler of the high priority HDMA interrupt.
- **set_high_priority_interrupt_handler** `void set_high_priority_interrupt_handler(interrupt_handler_type interrupt_handler)` (L107): Sets the handler of the high priority HDMA interrupt.
  **ref**: BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED must be active for the handler to be called.
