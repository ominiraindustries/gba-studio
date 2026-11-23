# Butano 21.10.1 — Asserts (`assert`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_assert.h

### `BN_ASSERT_FILE` (macro, bn_assert.h:126)

Current input file name if available; otherwise an empty string.

`#define BN_ASSERT_FILE __FILE_NAME__`

### `bn::assert` (namespace, bn_assert.h:141)

Assert related functions.

- **callback** `callback_type callback()` (L146): Returns the user function called when an assert is triggered, before stopping execution.
- **set_callback** `void set_callback(callback_type callback)` (L151): Sets the user function called when an assert is triggered, before stopping execution.

## bn_config_assert.h

### `BN_CFG_ASSERT_ENABLED` (macro, bn_config_assert.h:26)

Specifies if asserts must be enabled or not.

`#define BN_CFG_ASSERT_ENABLED true`

### `BN_CFG_ASSERT_SHOW_GENERAL_INFO` (macro, bn_config_assert.h:39)

Specifies if general assert information must be shown or not.

`#define BN_CFG_ASSERT_SHOW_GENERAL_INFO true`

Disabling general information reduces ROM size.

### `BN_CFG_ASSERT_SHOW_DIAGNOSTIC` (macro, bn_config_assert.h:52)

Specifies if asserts diagnostic information must be shown or not.

`#define BN_CFG_ASSERT_SHOW_DIAGNOSTIC true`

Disabling diagnostic information reduces ROM size.

### `BN_CFG_ASSERT_TAG` (macro, bn_config_assert.h:63)

Specifies the default tag displayed in assert messages.

`#define BN_CFG_ASSERT_TAG BN_VERSION_STRING " " BN_TOOLCHAIN_TAG`

### `BN_CFG_ASSERT_BUFFER_SIZE` (macro, bn_config_assert.h:74)

Specifies the maximum size of the messages that can be displayed by an assert.

`#define BN_CFG_ASSERT_BUFFER_SIZE 512`

### `BN_CFG_ASSERT_STACKTRACE_DEMANGLE` (macro, bn_config_assert.h:89)

Indicates if the stack trace logged in error messages must be demangled or not.

`#define BN_CFG_ASSERT_STACKTRACE_DEMANGLE false`

Stack trace demangle requires to link standard system libraries (`DEFAULTLIBS` makefile variable).

Expect a crash after logging the stack trace.

