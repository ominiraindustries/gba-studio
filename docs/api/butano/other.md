# Butano 21.10.1 — Other (`other`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_common.h

### `BN_CONST` (macro, bn_common.h:27)

The return value of a function with this attribute is solely a function of the arguments.

`#define BN_CONST __attribute__((const))`


## bn_type_id.h

### `bn::type_id_t` (class, bn_type_id.h:48)

Unique type ID without RTTI.

`class type_id_t`

See https://github.com/Manu343726/ctti/blob/master/include/ctti/type_id.hpp

- **type_id_t** `constexpr type_id_t() = default` (L60): Default constructor.
- **internal_id** `constexpr id_type* internal_id() const` (L65): Returns the internal id.
- **operator==** `constexpr friend bool operator==(type_id_t a, type_id_t b) = default` (L73): Default equal operator.
- **type_id** `template<typename Type> constexpr type_id_t type_id()` (L88): Returns the type_id_t of the specified type.

## bn_version.h

### `BN_VERSION_MAJOR` (macro, bn_version.h:26)

Specifies the current major version of Butano.

`#define BN_VERSION_MAJOR 21`

### `BN_VERSION_MINOR` (macro, bn_version.h:37)

Specifies the current minor version of Butano.

`#define BN_VERSION_MINOR 10`

### `BN_VERSION_PATCH` (macro, bn_version.h:48)

Specifies the current patch version of Butano.

`#define BN_VERSION_PATCH 1`

### `BN_VERSION_STRING` (macro, bn_version.h:59)

Specifies the current version of Butano in string format.

`#define BN_VERSION_STRING "21.10.1"`

### `bn::version` (namespace, bn_version.h:67)

Current version of Butano.

- **major** `constexpr int major()` (L72): Returns the current major version of Butano.
- **minor** `constexpr int minor()` (L80): Returns the current minor version of Butano.
- **patch** `constexpr int patch()` (L88): Returns the current patch version of Butano.
