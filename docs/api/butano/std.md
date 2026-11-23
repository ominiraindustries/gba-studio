# Butano 21.10.1 — Standard library (`std`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_cstdlib.h

- **malloc** `void* malloc(int bytes)` (L30): Allocates uninitialized storage in EWRAM.
  `bytes` Bytes to allocate.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with bn::free.
- **calloc** `void* calloc(int num, int bytes)` (L44): Allocates storage in EWRAM for an array of num objects of bytes size and initializes all bytes in it to zero.
  `num` Number of objects.; `bytes` Size in bytes of each object.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with bn::free.
- **realloc** `void* realloc(void* ptr, int new_bytes)` (L63): Reallocates the given storage in the EWRAM.
  `ptr` Pointer to the storage to reallocate.; `new_bytes` New size in bytes of the reallocated storage.; devuelve: On success, returns the pointer to the beginning of newly allocated storage. On failure, returns `nullptr`.
  If ptr was not previously allocated by bn::malloc, bn::calloc or bn::realloc, the behavior is undefined.
  
  On success, the original pointer ptr is invalidated and any access to it is undefined behavior
  (even if reallocation was in-place).
  
  To avoid a memory leak, the returned pointer must be deallocated with bn::free.
- **free** `void free(void* ptr)` (L76): Deallocates the storage previously allocated by bn::malloc, bn::calloc or bn::realloc.
  `ptr` Pointer to the storage to deallocate. It is invalidated and any access to it is undefined behavior.
  If ptr is `nullptr`, the function does nothing.
  
  If ptr was not previously allocated by bn::malloc, bn::calloc or bn::realloc, the behavior is undefined.

## bn_cstring.h

- **memcpy** `void memcpy(void* destination, const void* source, int bytes)` (L28): Copies the given bytes from the object pointed to by source to the object pointed to by destination.
  `destination` Pointer to the memory location to copy to.; `source` Pointer to the memory location to copy from.; `bytes` Number of bytes to copy.
- **memset** `void memset(void* destination, uint8_t value, int bytes)` (L38): Copies the given value into each of the first given bytes of the object pointed to by destination.
  `destination` Pointer to the object to fill.; `value` Fill byte.; `bytes` Number of bytes to fill.
- **memclear** `void memclear(void* destination, int bytes)` (L47): Copies the value `0` into each of the first given bytes of the object pointed to by destination.
  `destination` Pointer to the object to clear.; `bytes` Number of bytes to clear.
