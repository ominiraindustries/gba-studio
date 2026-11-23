# Butano 21.10.1 — Logging (`log`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_log.h

### `BN_CFG_LOG_ENABLED` (macro, bn_config_log.h:26)

Specifies if logging is enabled or not.

`#define BN_CFG_LOG_ENABLED true`

### `BN_CFG_LOG_BACKEND` (macro, bn_config_log.h:39)

Specifies the log backend to use.

`#define BN_CFG_LOG_BACKEND BN_LOG_BACKEND_MGBA`

Values not specified in BN_LOG_BACKEND_* macros are not allowed.

### `BN_CFG_LOG_MAX_SIZE` (macro, bn_config_log.h:50)

Specifies the maximum number of characters that can be logged with one BN_LOG call.

`#define BN_CFG_LOG_MAX_SIZE 0x100`


## bn_log.h

### `bn` (namespace, bn_log.h:74)

Prints in one line of text the representation of the given parameters with the specified log level.

Example:

```cpp
BN_LOG_LEVEL(bn::log_level::ERROR, "Invalid integer: ", integer);
```
Custom parameter types are supported by overloading bn::ostringstream::operator<<.

Example:

```cpp
bn::ostringstream& operator<<(bn::ostringstream& stream, const custom_type& custom_value)
{
    stream.append("custom_type: ");
    stream.append(custom_value.data);
    return stream;
}
```

- **log** `void log(const istring_base& message)` (L110): Prints in one line of text the given message with the default log level (bn::log_level::WARN).
- **log** `void log(log_level level, const istring_base& message)` (L119): Prints in one line of text the given message.
  `level` Message level.; `message` Message to print.

## bn_log_backend.h

### `BN_LOG_BACKEND_MGBA` (macro, bn_log_backend.h:28)

<a href="https://mgba.io">mGBA</a> log backend.

`#define BN_LOG_BACKEND_MGBA 0`

It also works with <a href="https://github.com/nba-emu/NanoBoyAdvance">NanoBoyAdvance</a>
and <a href="https://github.com/SourMesen/Mesen2">Mesen</a>.

### `BN_LOG_BACKEND_NOCASHGBA` (macro, bn_log_backend.h:37)

<a href="https://problemkaputt.de/gba.htm">No$gba</a> log backend.

`#define BN_LOG_BACKEND_NOCASHGBA 1`

### `BN_LOG_BACKEND_VBA` (macro, bn_log_backend.h:48)

<a href="https://en.wikipedia.org/wiki/VisualBoyAdvance">VisualBoyAdvance</a> log backend.

`#define BN_LOG_BACKEND_VBA 2`

It crashes on hardware and on other emulators!


## bn_log_level.h

### `bn::log_level` (enum, bn_log_level.h:28)

Specifies the level of a log message.

`enum class log_level : uint8_t { FATAL /* Fatal. A message with this level stops the execution. */, ERROR /* Error. */, WARN /* Warn (default). */, INFO /* Info. */, DEBUG /* Debug. */ }`

Most emulators ignore this log level.

