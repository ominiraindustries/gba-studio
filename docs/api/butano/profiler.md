# Butano 21.10.1 — Profiler (`profiler`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_profiler.h

### `BN_CFG_PROFILER_ENABLED` (macro, bn_config_profiler.h:26)

Specifies if the profiler is enabled or not.

`#define BN_CFG_PROFILER_ENABLED false`

### `BN_CFG_PROFILER_LOG_ENGINE` (macro, bn_config_profiler.h:37)

Specifies if Butano subsystems must be profiled or not.

`#define BN_CFG_PROFILER_LOG_ENGINE false`

### `BN_CFG_PROFILER_LOG_ENGINE_DETAILED` (macro, bn_config_profiler.h:50)

Specifies if each Butano subsystem must be profiled separately or not.

`#define BN_CFG_PROFILER_LOG_ENGINE_DETAILED false`

**ref**: BN_CFG_PROFILER_LOG_ENGINE must be `true` to enable Butano subsystems profiling.

### `BN_CFG_PROFILER_MAX_ENTRIES` (macro, bn_config_profiler.h:61)

Specifies the maximum number of code blocks that can be profiled without too many performance issues.

`#define BN_CFG_PROFILER_MAX_ENTRIES 64`


## bn_profiler.h

### `bn::profiler` (namespace, bn_profiler.h:53)

Profiler related functions.

- **show** `[[noreturn]] void show()` (L58): Stops the execution and shows the profiling results on the screen.
