# Butano 21.10.1 — Timers (`timer`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_timer.h

### `BN_CFG_TIMER_FREQUENCY` (macro, bn_config_timer.h:28)

Specifies timer frequency in clock cycles.

`#define BN_CFG_TIMER_FREQUENCY 64`

Allowed values are 1, 64, 256 and 1024.


## bn_timer.h

### `bn::timer` (class, bn_timer.h:26)

Provides a way to calculate elapsed times.

`class timer`

- **timer** `timer()` (L33): Constructor.
- **elapsed_ticks** `int elapsed_ticks() const` (L38): Returns the number of ticks since this timer was built or restarted.
- **restart** `void restart()` (L48): Restarts the timer's internal state.
  Equivalent to:
  ```cpp
  *this = timer();
  ```
- **elapsed_ticks_with_restart** `int elapsed_ticks_with_restart()` (L53): Returns the number of ticks since this timer was built or restarted, restarting its internal state.

## bn_timers.h

### `bn::timers` (namespace, bn_timers.h:23)

Timing related functions.

- **ticks_per_frame** `constexpr int ticks_per_frame()` (L28): Returns the number of ticks elapsed in a screen refresh (V-Draw + V-Blank).
- **ticks_per_second** `constexpr int ticks_per_second()` (L36): Returns the number of ticks elapsed in a second.
- **ticks_per_vblank** `constexpr int ticks_per_vblank()` (L44): Returns the number of ticks elapsed in a V-Blank period.
- **cpu_clocks_per_tick** `constexpr int cpu_clocks_per_tick()` (L52): Returns the number of CPU clocks per timer tick.
