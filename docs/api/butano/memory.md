# Butano 21.10.1 — Memory (`memory`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_alignment.h

- **aligned** `template<int Bytes> bool aligned(const void* pointer)` (L25): Indicates if the given `void` pointer is aligned to the given Bytes.
- **aligned** `template<int Bytes, typename Type> bool aligned(const Type* pointer)` (L38): Indicates if the given Type pointer is aligned to the given Bytes.

## bn_memory.h

### `bn::memory` (namespace, bn_memory.h:59)

Memory related functions.

- **ewram_alloc** `void* ewram_alloc(int bytes)` (L69): Allocates uninitialized storage in EWRAM.
  `bytes` Bytes to allocate.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
- **ewram_calloc** `void* ewram_calloc(int num, int bytes)` (L81): Allocates storage in EWRAM for an array of num objects of bytes size and initializes all bytes in it to zero.
  `num` Number of objects.; `bytes` Size in bytes of each object.; devuelve: On success, returns the pointer to the beginning of newly allocated memory. On failure, returns `nullptr`.
  To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
- **ewram_realloc** `void* ewram_realloc(void* ptr, int new_bytes)` (L99): Reallocates the given storage in the EWRAM.
  `ptr` Pointer to the storage to reallocate.; `new_bytes` New size in bytes of the reallocated storage.; devuelve: On success, returns the pointer to the beginning of newly allocated storage. On failure, returns `nullptr`.
  If ptr was not previously allocated by bn::memory::ewram_alloc, bn::memory::ewram_calloc or
  bn::memory::ewram_realloc, the behavior is undefined.
  
  On success, the original pointer ptr is invalidated and any access to it is undefined behavior
  (even if reallocation was in-place).
  
  To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
- **ewram_free** `void ewram_free(void* ptr)` (L112): Deallocates the storage previously allocated by bn::memory::ewram_alloc, bn::memory::ewram_calloc or bn::memory::ewram_realloc.
  `ptr` Pointer to the storage to deallocate. It is invalidated and any access to it is undefined behavior.
  If ptr is `nullptr`, the function does nothing.
  
  If ptr was not previously allocated by bn::memory::ewram_alloc, bn::memory::ewram_calloc or
  bn::memory::ewram_realloc, the behavior is undefined.
- **used_alloc_ewram** `int used_alloc_ewram()` (L118): Returns the size in bytes of all allocated items in EWRAM with bn::memory::ewram_alloc, bn::memory::ewram_calloc and bn::memory::ewram_realloc.
- **available_alloc_ewram** `int available_alloc_ewram()` (L124): Returns the number of bytes that still can be allocated in EWRAM with bn::memory::ewram_alloc, bn::memory::ewram_calloc and bn::memory::ewram_realloc.
- **log_alloc_ewram_status** `void log_alloc_ewram_status()` (L129): Logs the current status of the EWRAM allocator.
- **used_stack_iwram** `int used_stack_iwram()` (L134): Returns the number of bytes of IWRAM used by the stack.
- **used_static_iwram** `int used_static_iwram()` (L139): Returns the size in bytes of all static objects in IWRAM.
- **used_static_ewram** `int used_static_ewram()` (L144): Returns the size in bytes of all static objects in EWRAM.
- **used_rom** `int used_rom()` (L149): Returns the size of the ROM in bytes.
- **fast_ewram** `bool fast_ewram()` (L154): Indicates if EWRAM access time is 1+1 clock cycles for sequential accesses or not.
- **copy** `template<typename Type> void copy(const Type& source_ref, int elements, Type& destination_ref)` (L166): Copies the given amount of elements from the object referenced by source_ref to the object referenced by destination_ref.
  `source_ref` Const reference to the memory location to copy from.; `elements` Number of elements to copy (not bytes).; `destination_ref` Reference to the memory location to copy to.
  If the source and destination objects overlap, the behavior is undefined.
- **copy** `template<typename Type, int SourceSize> void copy(const Type (&source_array)[SourceSize], Type& destination_ref)` (L215): Copies the memory of the given array to the object referenced by destination_ref.
  `source_array` Array to copy from.; `destination_ref` Reference to the memory location to copy to.
  If the source and destination objects overlap, the behavior is undefined.
- **clear** `template<typename Type> void clear(int elements, Type& destination_ref)` (L227): Clears (fills with zero) the memory of the given amount of elements from the object referenced by destination_ref.
  `elements` Number of elements to clear (not bytes).; `destination_ref` Reference to the memory location to clear.
- **vram_safe_copy** `void vram_safe_copy(const void* source_ptr, int bytes, void* destination_ptr)` (L280): Copies the given amount of bytes from the object pointed by source_ptr to the object pointed by destination_ptr, ensuring that the copy works if the destination is in VRAM.
  `source_ptr` Const pointer to the memory location to copy from.; `bytes` Number of bytes to copy.; `destination_ptr` Pointer to the memory location to copy to.
  If the source and destination objects overlap, the behavior is undefined.
- **clear** `template<typename Type, int Size> void clear(Type (&array)[Size])` (L285): Clears (fills with zero) the memory of the given array.
- **set_bytes** `void set_bytes(uint8_t value, int bytes, void* destination_ptr)` (L301): Copies the given value into each of the first bytes of the object pointed to by destination_ptr.
  `value` Value to fill with.; `bytes` Number of bytes to fill.; `destination_ptr` Pointer to the object to fill.
  If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
  the behavior is undefined.
- **set_half_words** `void set_half_words(uint16_t value, int half_words, void* destination_ptr)` (L313): Copies the given value into each of the first half_words of the object pointed to by destination_ptr.
  `value` Value to fill with.; `half_words` Number of half words to fill.; `destination_ptr` Pointer to the object to fill.
  If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
  the behavior is undefined.
- **set_words** `void set_words(unsigned value, int words, void* destination_ptr)` (L325): Copies the given value into each of the first words of the object pointed to by destination_ptr.
  `value` Value to fill with.; `words` Number of words to fill.; `destination_ptr` Pointer to the object to fill.
  If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
  the behavior is undefined.
- **decompress** `void decompress(compression_type compression, const void* source_ptr, int bytes, void* destination_ptr)` (L338): Decompresses the compressed data pointed to by source_ptr in the first bytes of the object pointed to by destination_ptr.
  `compression` Compression type.; `source_ptr` Compressed data to decompress.; `bytes` Number of decompressed bytes.; `destination_ptr` Destination of the decompressed data.
  If the source and destination data overlap, the behavior is undefined.
- **dma_enabled** `bool dma_enabled()` (L343): Indicates if Butano can use DMA for memory copies when it's safe to do so.
- **set_dma_enabled** `void set_dma_enabled(bool dma_enabled)` (L348): Sets if Butano can use DMA for memory copies when it's safe to do so.

## bn_unique_ptr.h

- **unique_ptr** `constexpr unique_ptr(nullptr_t) : unique_ptr()` (L41): Nullptr constructor.
- **unique_ptr** `constexpr explicit unique_ptr(pointer ptr)` (L50): Constructor.
  `ptr` Pointer to the object to manage.
- **unique_ptr** `constexpr unique_ptr(pointer ptr, const Deleter& deleter)` (L60): Constructor.
  `ptr` Pointer to the object to manage.; `deleter` Object which disposes the object to manage when the unique_ptr goes out of scope.
- **unique_ptr** `constexpr unique_ptr(pointer ptr, Deleter&& deleter)` (L71): Constructor.
  `ptr` Pointer to the object to manage.; `deleter` Object which disposes the object to manage when the unique_ptr goes out of scope.
- **unique_ptr** `constexpr unique_ptr(unique_ptr&& other) noexcept : _ptr(other.release()), _deleter(move(other._deleter))` (L83): Move constructor.
  `other` unique_ptr object to move.
- **unique_ptr** `template<typename OtherType, typename OtherDeleter> constexpr unique_ptr(unique_ptr<OtherType, OtherDeleter>&& other)` (L93): Move constructor.
  `other` unique_ptr object to move with different type and deleter.
- **operator=** `constexpr unique_ptr& operator=(unique_ptr&& other) noexcept` (L107): Move assignment operator.
  `other` unique_ptr object to move.; devuelve: Reference to this.
- **operator=** `template<typename OtherType, typename OtherDeleter> constexpr unique_ptr& operator=(unique_ptr<OtherType, OtherDeleter>&& other)` (L119): Move assignment operator.
  `other` unique_ptr object to move with different type and deleter.; devuelve: Reference to this.
- **operator=** `constexpr unique_ptr& operator=(nullptr_t) noexcept` (L130): Effectively the same as calling reset().
- **~unique_ptr** `constexpr ~unique_ptr()` (L138): Class destructor, which disposes the managed object.
- **operator bool** `constexpr operator bool() const` (L146): Indicates if it contains a managed object or not.
- **get** `constexpr const Type* get() const` (L154): Returns a const pointer to the managed object.
- **get** `constexpr pointer get()` (L162): Returns a pointer to the managed object.
- **get_deleter** `constexpr const Deleter& get_deleter() const` (L171): Returns a const reference to the object which disposes the managed object when the unique_ptr goes out of scope.
- **get_deleter** `constexpr Deleter& get_deleter()` (L180): Returns a reference to the object which disposes the managed object when the unique_ptr goes out of scope.
- **operator*** `constexpr const Type& operator*() const` (L188): Returns a const reference to the managed object.
- **operator*** `constexpr reference operator*()` (L198): Returns a reference to the managed object.
- **operator->** `constexpr const Type* operator->() const` (L208): Returns a const pointer to the managed object.
- **operator->** `constexpr pointer operator->()` (L218): Returns a pointer to the managed object.
- **release** `constexpr pointer release()` (L229): Releases the ownership of the managed object.
  devuelve: Pointer to the released object.
- **reset** `constexpr void reset()` (L239): Disposes the managed object.
- **reset** `constexpr void reset(pointer ptr)` (L252): Disposes the managed object and replaces it with the given one.
  `ptr` Pointer to the new object to manage.
- **reset** `template<typename OtherType> constexpr void reset(OtherType* ptr)` (L265): Disposes the managed object and replaces it with the given one.
  `ptr` Pointer to the new object to manage with different type.
- **reset** `constexpr void reset(nullptr_t)` (L278): Disposes the managed object.
- **swap** `constexpr void swap(unique_ptr& other)` (L287): Exchanges the contents of this unique_ptr with those of the other one.
  `other` unique_ptr to exchange the contents with.
- **swap** `constexpr friend void swap(unique_ptr& a, unique_ptr& b)` (L298): Exchanges the contents of a unique_ptr with those of another one.
  `a` First unique_ptr to exchange the contents with.; `b` Second unique_ptr to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const unique_ptr& a, const unique_ptr& b)` (L309): Equal operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is equal to the second one, otherwise `false`.
- **operator!=** `constexpr friend bool operator!=(const unique_ptr& a, const unique_ptr& b)` (L320): Not equal operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is not equal to the second one, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const unique_ptr& a, const unique_ptr& b)` (L332): Less than operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is lexicographically less than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const unique_ptr& a, const unique_ptr& b)` (L344): Greater than operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const unique_ptr& a, const unique_ptr& b)` (L356): Less than or equal operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const unique_ptr& a, const unique_ptr& b)` (L368): Greater than or equal operator.
  `a` First unique_ptr to compare.; `b` Second unique_ptr to compare.; devuelve: `true` if the first managed object is lexicographically greater than or equal to the second one, otherwise `false`.
- **operator==** `constexpr friend bool operator==(const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L379): Equal operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `true` if the unique_ptr does not have a managed object, otherwise `false`.
- **operator!=** `constexpr friend bool operator!=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L390): Not equal operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `true` if the unique_ptr has a managed object, otherwise `false`.
- **operator<** `constexpr friend bool operator<([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L401): Less than operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `false`.
- **operator>** `constexpr friend bool operator>(const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L412): Greater than operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `true` if the unique_ptr has a managed object, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L423): Less than or equal operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `true` if the unique_ptr does not have a managed object, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)` (L434): Greater than or equal than operator.
  `a` unique_ptr to compare.; `b` Null pointer to compare.; devuelve: `true`.
- **make_unique** `template<typename Type, class... Args> constexpr unique_ptr<Type> make_unique(Args&&... args)` (L456): Constructs an object and wraps it in a unique_ptr.
  `Type` Type of the object to construct.; `Args` Type of the arguments of the object to construct.; `args` Parameters of the object to construct.; devuelve: A unique_ptr managing the new object.
- **operator<<** `template<typename Type, typename Deleter> ostringstream& operator<<(ostringstream& stream, const unique_ptr<Type, Deleter>& value)` (L491): Appends the character representation of the given unique_ptr to the given ostringstream.
  `Type` Type of the managed object.; `Deleter` Type of the object which disposes the managed object when the unique_ptr goes out of scope.
