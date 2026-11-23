# Butano 21.10.1 — Allocator (`allocator`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_best_fit_allocator.h

### `bn::best_fit_allocator` (class, bn_best_fit_allocator.h:28)

Manages a chunk of memory with a best fit allocation strategy.

`class best_fit_allocator`

- **best_fit_allocator** `best_fit_allocator() = default` (L37): Default constructor.
- **best_fit_allocator** `best_fit_allocator(void* start, size_type bytes)` (L44): Constructor.
  `start` Pointer to the first element of the memory to manage.; `bytes` Size in bytes of the memory to manage.
- **~best_fit_allocator** `~best_fit_allocator() noexcept` (L58): Destructor.
  It doesn't destroy its elements, they must be destroyed manually.
- **used_bytes** `size_type used_bytes() const` (L63): Returns the size in bytes of all allocated items.
- **available_bytes** `size_type available_bytes() const` (L71): Returns the number of bytes that still can be allocated.
- **empty** `bool empty() const` (L79): Indicates if it doesn't contain any item.
- **full** `bool full() const` (L87): Indicates if it can't contain any more items.
- **check_empty_on_destructor** `bool check_empty_on_destructor() const` (L95): Indicates if the allocator must check if it is empty or not on its destructor.
- **set_check_empty_on_destructor** `void set_check_empty_on_destructor(bool check_empty_on_destructor)` (L103): Sets if the allocator must check if it is empty or not on its destructor.
- **alloc** `void* alloc(size_type bytes)` (L116): Allocates uninitialized storage.
  `bytes` Bytes to allocate.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with free.
- **calloc** `void* calloc(size_type num, size_type bytes)` (L128): Allocates storage for an array of num objects of bytes size and initializes all bytes in it to zero.
  `num` Number of objects.; `bytes` Size in bytes of each object.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with free.
- **realloc** `void* realloc(void* ptr, size_type new_bytes)` (L147): Reallocates the given storage.
  `ptr` Pointer to the storage to reallocate.; `new_bytes` New size in bytes of the reallocated storage.; devuelve: On success, returns the pointer to the beginning of newly allocated storage. On failure, returns `nullptr`.
  Keep in mind that the existing area pointed to by ptr is never expanded or contracted.
  
  If ptr was not previously allocated by alloc, calloc or realloc, the behavior is undefined.
  
  On success, the original pointer ptr is invalidated and any access to it is undefined behavior
  (even if reallocation was in-place).
  
  To avoid a memory leak, the returned pointer must be deallocated with free.
- **free** `void free(void* ptr)` (L158): Deallocates the storage previously allocated by alloc, calloc or realloc.
  `ptr` Pointer to the storage to deallocate. It is invalidated and any access to it is undefined behavior.
  If ptr is `nullptr`, the function does nothing.
  
  If ptr was not previously allocated by alloc, calloc or realloc, the behavior is undefined.
- **create** `template<typename Type, typename... Args> Type& create(Args&&... args)` (L165): Constructs a value inside of the allocator.
  `args` Parameters of the value to construct.; devuelve: Reference to the new value.
- **destroy** `template<typename Type> void destroy(Type& value)` (L178): Destroys the given value, previously allocated with the create method.
- **reset** `void reset(void* start, size_type bytes)` (L190): Setups the allocator to manage a new chunk of memory.
  `start` Pointer to the first element of the memory to manage.; `bytes` Size in bytes of the memory to manage.
- **log_status** `void log_status() const` (L195): Logs the current status of the allocator.

## bn_config_best_fit_allocator.h

### `BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED` (macro, bn_config_best_fit_allocator.h:28)

Specifies if bn::best_fit_allocator sanity check is enabled or not.

`#define BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED false`

Sanity check asserts if the internal state of the allocator is valid.

### `BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED` (macro, bn_config_best_fit_allocator.h:39)

Specifies if bn::best_fit_allocator::free checks if the input pointer is valid or not.

`#define BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED false`

