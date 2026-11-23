# Butano 21.10.1 — Strings (`string`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_format.h

- **format** `template<int MaxSize, class... Args> string<MaxSize> format(const string_view& format, const Args&... args)` (L134): Format the given arguments according to the given format string, and return the result as a string.
  `MaxSize` Maximum number of characters that can be stored in the output string.; `Args` Types of the arguments to be formatted.; `format` string_view representing the format string.; `args` Arguments to be formatted. They are used in order when processing the format string.; devuelve: A string holding the formatted result.
  The format string consists of:
  * Ordinary characters (except `{` and `}`), which are copied unchanged to the output.
  * Escape sequences `{{` and `}}`, which are replaced with `{` and `}` respectively in the output.
  * Replacement fields, with the following format: `{}`.
- **format_ref** `template<class... Args> void format_ref(istring_base& string, const string_view& format, const Args&... args)` (L157): Format the given arguments according to the given format string, storing the result in the given string.
  `string` The result of the formatting is stored in this string.; `format` string_view representing the format string.; `args` Arguments to be formatted. They are used in order when processing the format string.
  The format string consists of:
  * Ordinary characters (except `{` and `}`), which are copied unchanged to the output.
  * Escape sequences `{{` and `}}`, which are replaced with `{` and `}` respectively in the output.
  * Replacement fields, with the following format: `{}`.

## bn_istring_base.h

### `bn::istring_base` (class, bn_istring_base.h:30)

Base class of bn::istring.

`class istring_base`

It avoids circular references when including bn::istring header.

- **istring_base** `template<int MaxSize> constexpr istring_base(char (&char_array_ref)[MaxSize])` (L54): Constructor.
  `char_array_ref` Non empty const characters array.
  The characters are not conserved: the first character of the array is set to null and the output size is 0.
  If you want to conserve the characters, you should use istring::from_char_array instead.
  
  The characters are not copied but referenced, so they should outlive the istring_base
  to avoid dangling references.
- **size** `constexpr size_type size() const` (L67): Returns the number of characters in the istring_base.
- **length** `constexpr size_type length() const` (L75): Returns the number of characters in the istring_base.
- **max_size** `constexpr size_type max_size() const` (L83): Returns the maximum possible number of characters in the istring_base.
- **available** `constexpr size_type available() const` (L91): Returns the remaining characters capacity.
- **empty** `constexpr bool empty() const` (L99): Indicates if it doesn't contain any character.
- **full** `constexpr bool full() const` (L107): Indicates if it can't contain any more characters.
- **data** `constexpr const_pointer data() const` (L115): Returns a const pointer to the beginning of the characters data.
- **data** `constexpr pointer data()` (L123): Returns a const pointer to the beginning of the characters data.
- **c_str** `constexpr const_pointer c_str() const` (L131): Returns a const pointer to the beginning of the characters data (it is null-terminated).
- **begin** `constexpr const_iterator begin() const` (L139): Returns a const iterator to the beginning of the characters data.
- **begin** `constexpr iterator begin()` (L147): Returns an iterator to the beginning of the characters data.
- **end** `constexpr iterator end()` (L155): Returns a const iterator to the end of the characters data.
- **end** `constexpr const_iterator end() const` (L163): Returns an iterator to the end of the characters data.
- **cbegin** `constexpr const_iterator cbegin() const` (L171): Returns a const iterator to the beginning of the characters data.
- **cend** `constexpr const_iterator cend() const` (L179): Returns a const iterator to the end of the characters data.
- **clear** `constexpr void clear()` (L187): Removes all characters.
- **operator==** `constexpr friend bool operator==(const istring_base& a, const istring_base& b)` (L199): Equal operator.
  `a` First istring_base to compare.; `b` Second istring_base to compare.; devuelve: `true` if the first istring_base is equal to the second one, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const istring_base& a, const istring_base& b)` (L226): Less than operator.
  `a` First istring_base to compare.; `b` Second istring_base to compare.; devuelve: `true` if the first istring_base is lexicographically less than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const istring_base& a, const istring_base& b)` (L238): Greater than operator.
  `a` First istring_base to compare.; `b` Second istring_base to compare.; devuelve: `true` if the first istring_base is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const istring_base& a, const istring_base& b)` (L250): Less than or equal operator.
  `a` First istring_base to compare.; `b` Second istring_base to compare.; devuelve: `true` if the first istring_base is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const istring_base& a, const istring_base& b)` (L262): Greater than or equal operator.
  `a` First istring_base to compare.; `b` Second istring_base to compare.; devuelve: `true` if the first istring_base is lexicographically greater than or equal to the second one, otherwise `false`.

## bn_sstream.h

### `bn::ostringstream` (class, bn_sstream.h:31)

Implements output operations on string based streams.

`class ostringstream`

It effectively stores an instance of istring_base and performs output operations to it.

- **ostringstream** `explicit ostringstream(istring_base& string)` (L39): Constructor.
  `string` string in which to append to.
- **str** `const istring& str() const` (L44): Returns a const reference to the managed string.
- **str** `istring& str()` (L52): Returns a reference to the managed string.
- **rdbuf** `const istring* rdbuf() const` (L60): Returns a const pointer to the managed string.
- **rdbuf** `istring* rdbuf()` (L68): Returns a pointer to the managed string.
- **rdbuf** `istring* rdbuf(istring_base* sb)` (L78): Sets the managed string.
  `sb` Pointer to new string to manage.; devuelve: Pointer to new string to manage.
- **set_rdbuf** `void set_rdbuf(istring_base& sb)` (L84): Sets the managed string.
  `sb` Reference to new string to manage.
- **precision** `int precision() const` (L89): Returns the decimal precision of fixed point operations.
- **precision** `int precision(int new_precision)` (L99): Sets the decimal precision of fixed point operations.
  `new_precision` New decimal precision.; devuelve: New decimal precision.
- **set_precision** `void set_precision(int precision)` (L105): Sets the decimal precision of fixed point operations.
  `precision` New decimal precision.
- **view** `string_view view() const` (L110): Returns a view over the contents of the managed string.
- **size** `int size() const` (L115): Returns the number of characters in the managed string.
- **length** `int length() const` (L120): Returns the number of characters in the managed string.
- **max_size** `int max_size() const` (L125): Returns the maximum possible number of characters in the managed string.
- **available** `int available() const` (L130): Returns the remaining characters capacity in the managed string.
- **empty** `bool empty() const` (L135): Indicates if the managed string doesn't contain any character.
- **full** `bool full() const` (L140): Indicates if the managed string can't contain any more characters.
- **append** `void append(char character)` (L146): Appends an additional character to the managed string.
  `character` Character to append.
- **put** `ostringstream& put(char character)` (L152): Appends an additional character to the managed string.
  `character` Character to append.
- **append** `void append(const string_view& view)` (L162): Appends additional characters to the managed string.
  `view` string_view to append.
- **append** `void append(const istring_base& string)` (L168): Appends additional characters to the managed string.
  `string` string to append.
- **append** `void append(const char* char_array_ptr)` (L174): Appends additional characters to the managed string.
  `char_array_ptr` Pointer to null-terminated characters array.
- **append** `void append(const char* char_array_ptr, int char_array_size)` (L181): Appends additional characters to the managed string.
  `char_array_ptr` Pointer to characters array.; `char_array_size` Characters count of the characters array.
- **write** `ostringstream& write(const char* char_array_ptr, int char_array_size)` (L189): Appends additional characters to the managed string.
  `char_array_ptr` Pointer to characters array.; `char_array_size` Characters count of the characters array.; devuelve: Reference to this.
- **append** `template<int MaxSize> void append(char(&char_array)[MaxSize])` (L199): Appends additional characters to the managed string.
  `char_array` Non empty const characters array.
- **append** `void append(bool value)` (L208): Appends the character representation of the given bool value to the managed string.
- **append** `void append(int value)` (L216): Appends the character representation of the given int value to the managed string.
- **append** `void append(long value)` (L221): Appends the character representation of the given long value to the managed string.
- **append** `void append(int64_t value)` (L226): Appends the character representation of the given int64_t value to the managed string.
- **append** `void append(unsigned value)` (L231): Appends the character representation of the given unsigned value to the managed string.
- **append** `void append(unsigned long value)` (L236): Appends the character representation of the given unsigned long value to the managed string.
- **append** `void append(uint64_t value)` (L241): Appends the character representation of the given uint64_t value to the managed string.
- **append** `void append(const void* ptr)` (L246): Appends the character representation of the given pointer to the managed string.
- **append** `void append([[maybe_unused]] const nullptr_t& null_ptr)` (L251): Appends the character representation of the given null pointer to the managed string.
- **append** `template<int Precision> void append(fixed_t<Precision> value)` (L259): Appends the character representation of the given fixed point value to the managed string.
- **append_args** `template<typename Type, typename... Args> void append_args(const Type& value, const Args&... args)` (L300): Appends the character representation of the given parameters to the managed string.
- **append_args** `template<typename Type> void append_args(const Type& value)` (L310): Appends the character representation of the given value to the managed string.
- **swap** `void swap(ostringstream& other)` (L328): Exchanges the contents of this ostringstream with those of the other one.
  `other` ostringstream to exchange the contents with.
- **swap** `friend void swap(ostringstream& a, ostringstream& b)` (L335): Exchanges the contents of a ostringstream with those of another one.
  `a` First ostringstream to exchange the contents with.; `b` Second ostringstream to exchange the contents with.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, char character)` (L359): Appends an additional character to the given ostringstream.
  `stream` ostringstream in which to append to.; `character` Character to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const string_view& view)` (L373): Appends additional characters to the given ostringstream.
  `stream` ostringstream in which to append to.; `view` string_view to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const istring_base& string)` (L387): Appends additional characters to the given ostringstream.
  `stream` ostringstream in which to append to.; `string` string to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const char* char_array_ptr)` (L401): Appends additional characters to the given ostringstream.
  `stream` ostringstream in which to append to.; `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Reference to the ostringstream.
- **operator<<** `template<int MaxSize> inline ostringstream& operator<<(ostringstream& stream, char(&char_array)[MaxSize])` (L415): Appends additional characters to the given ostringstream.
  `stream` ostringstream in which to append to.; `char_array` Non empty const characters array.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, bool value)` (L430): Appends the character representation of the given bool value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` bool value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, int value)` (L444): Appends the character representation of the given int value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` int value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, long value)` (L458): Appends the character representation of the given long value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` long value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, int64_t value)` (L472): Appends the character representation of the given int64_t value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` int64_t value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, unsigned value)` (L486): Appends the character representation of the given unsigned value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` unsigned value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, unsigned long value)` (L500): Appends the character representation of the given unsigned long value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` unsigned long value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, uint64_t value)` (L514): Appends the character representation of the given uint64_t value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` uint64_t value to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const void* ptr)` (L528): Appends the character representation of the given pointer to the given ostringstream.
  `stream` ostringstream in which to append to.; `ptr` pointer to append.; devuelve: Reference to the ostringstream.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const nullptr_t& null_ptr)` (L542): Appends the character representation of the given null pointer to the given ostringstream.
  `stream` ostringstream in which to append to.; `null_ptr` Null pointer to append.; devuelve: Reference to the ostringstream.
- **operator<<** `template<int Precision> ostringstream& operator<<(ostringstream& stream, fixed_t<Precision> value)` (L556): Appends the character representation of the given fixed point value to the given ostringstream.
  `stream` ostringstream in which to append to.; `value` Fixed point value to append.; devuelve: Reference to the ostringstream.

## bn_string.h

### `bn::istring` (class, bn_string.h:29)

Base class of bn::string.

`class istring : public istring_base`

Can be used as a reference type for all bn::string containers.

- **from_char_array** `template<int MaxSize> static constexpr istring from_char_array(char (&char_array_ref)[MaxSize])` (L44): Returns an istring pointing to the given null-terminated characters array.
  The characters are not copied but referenced, so they should outlive the istring
  to avoid dangling references.
- **operator=** `constexpr istring& operator=(const istring_base& other)` (L68): Copy assignment operator.
  `other` istring_base to copy.; devuelve: Reference to this.
- **operator=** `constexpr istring& operator=(const string_view& view)` (L79): Copy assignment operator.
  `view` string_view to copy.; devuelve: Reference to this.
- **operator=** `constexpr istring& operator=(const_pointer char_array_ptr)` (L90): Copy assignment operator.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Reference to this.
- **rbegin** `constexpr const_reverse_iterator rbegin() const` (L99): Returns a const reverse iterator to the end of the characters data.
- **rbegin** `constexpr reverse_iterator rbegin()` (L107): Returns a reverse iterator to the end of the characters data.
- **rend** `constexpr const_reverse_iterator rend() const` (L115): Returns a const reverse iterator to the beginning of the characters data.
- **rend** `constexpr reverse_iterator rend()` (L123): Returns a reverse iterator to the beginning of the characters data.
- **crbegin** `constexpr const_reverse_iterator crbegin() const` (L131): Returns a const reverse iterator to the end of the characters data.
- **crend** `constexpr const_reverse_iterator crend() const` (L139): Returns a const reverse iterator to the beginning of the characters data.
- **operator[]** `constexpr const_reference operator[](size_type index) const` (L147): Returns a const reference to the character stored at the specified index.
- **operator[]** `constexpr reference operator[](size_type index)` (L157): Returns a reference to the character stored at the specified index.
- **at** `constexpr const_reference at(size_type index) const` (L167): Returns a const reference to the character stored at the specified index.
- **at** `constexpr reference at(size_type index)` (L177): Returns a reference to the character stored at the specified index.
- **front** `constexpr const_reference front() const` (L187): Returns a const reference to the first character.
- **front** `constexpr reference front()` (L197): Returns a reference to the first character.
- **back** `constexpr const_reference back() const` (L207): Returns a const reference to the last character.
- **back** `constexpr reference back()` (L217): Returns a reference to the last character.
- **starts_with** `constexpr bool starts_with(value_type value) const` (L229): Checks if the referenced string begins with the given prefix.
  `value` Single character.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **starts_with** `constexpr bool starts_with(const string_view& view) const` (L239): Checks if the referenced string begins with the given prefix.
  `view` string_view to check.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **starts_with** `constexpr bool starts_with(const_pointer char_array_ptr) const` (L264): Checks if the referenced string begins with the given prefix.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(value_type value) const` (L301): Checks if the referenced string ends with the given prefix.
  `value` Single character.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(const string_view& view) const` (L311): Checks if the referenced string ends with the given prefix.
  `view` string_view to check.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(const_pointer char_array_ptr) const` (L329): Checks if the referenced string ends with the given prefix.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **contains** `constexpr bool contains(value_type value) const` (L341): Checks if the referenced string contains the given character.
  `value` Single character.; devuelve: `true` if the referenced string contains the given character; `false` otherwise.
- **contains** `constexpr bool contains(const string_view& view) const` (L351): Checks if the referenced string contains the given substring.
  `view` string_view to check.; devuelve: `true` if the referenced string contains the given substring; `false` otherwise.
- **contains** `constexpr bool contains(const_pointer char_array_ptr) const` (L361): Checks if the referenced string contains the given substring.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string contains the given substring; `false` otherwise.
- **find** `constexpr size_type find(value_type value) const` (L373): Finds the first substring equal to the given character.
  `value` Single character.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **find** `constexpr size_type find(const string_view& view) const` (L391): Finds the first substring equal to the given character sequence.
  `view` string_view to search.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **find** `constexpr size_type find(const_pointer char_array_ptr) const` (L427): Finds the first substring equal to the given character sequence.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **assign** `constexpr istring& assign(const istring_base& other)` (L464): Replaces the contents of the istring.
  `other` istring_base replacement.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const istring_base& other, size_type position)` (L476): Replaces the contents of the istring.
  `other` istring_base replacement.; `position` Starting character index.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const istring_base& other, size_type position, size_type count)` (L492): Replaces the contents of the istring.
  `other` istring_base replacement.; `position` Starting character index.; `count` Number of characters to assign.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(value_type value)` (L506): Replaces the contents of the istring.
  `value` Character replacement.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const string_view& view)` (L520): Replaces the contents of the istring.
  `view` string_view replacement.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const string_view& view, size_type position)` (L532): Replaces the contents of the istring.
  `view` string_view replacement.; `position` Starting character index.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const string_view& view, size_type position, size_type count)` (L548): Replaces the contents of the istring.
  `view` string_view replacement.; `position` Starting character index.; `count` Number of characters to assign.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const_pointer char_array_ptr)` (L562): Replaces the contents of the istring.
  `char_array_ptr` Pointer to null-terminated characters array replacement.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const_pointer char_array_ptr, size_type char_array_size)` (L573): Replaces the contents of the istring.
  `char_array_ptr` Pointer to null-terminated characters array replacement.; `char_array_size` Characters count of the characters array.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(size_type count, value_type value)` (L584): Replaces the contents of the istring with count copies of character value.
  `count` New size.; `value` Character replacement.; devuelve: Reference to this.
- **assign** `constexpr istring& assign(const_iterator first, const_iterator last)` (L602): Replaces the contents of the istring with the characters in the range [first, last).
  `first` First element of the range.; `last` Last element of the range.; devuelve: Reference to this.
- **push_back** `constexpr void push_back(value_type value)` (L611): Inserts a character at the end of the istring.
  `value` Character to insert.
- **pop_back** `constexpr void pop_back()` (L625): Removes the last character of the istring.
- **append** `constexpr istring& append(const istring_base& other)` (L640): Appends additional characters to the istring.
  `other` istring_base to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(const istring_base& other, size_type position)` (L652): Appends additional characters to the istring.
  `other` istring_base to append.; `position` Starting character index.; devuelve: Reference to this.
- **append** `constexpr istring& append(const istring_base& other, size_type position, size_type count)` (L668): Appends additional characters to the istring.
  `other` istring_base to append.; `position` Starting character index.; `count` Number of characters to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(value_type value)` (L682): Appends an additional character to the istring.
  `value` Character to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(const string_view& view)` (L693): Appends additional characters to the istring.
  `view` string_view to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(const string_view& view, size_type position)` (L705): Appends additional characters to the istring.
  `view` string_view to append.; `position` Starting character index.; devuelve: Reference to this.
- **append** `constexpr istring& append(const string_view& view, size_type position, size_type count)` (L721): Appends additional characters to the istring.
  `view` string_view to append.; `position` Starting character index.; `count` Number of characters to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(const_pointer char_array_ptr)` (L735): Appends additional characters to the istring.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Reference to this.
- **append** `constexpr istring& append(const_pointer char_array_ptr, size_type char_array_size)` (L747): Appends additional characters to the istring.
  `char_array_ptr` Pointer to characters array.; `char_array_size` Characters count of the characters array.; devuelve: Reference to this.
- **append** `constexpr istring& append(size_type count, value_type value)` (L759): Appends additional characters to the istring.
  `count` Number of characters to append.; `value` Character to append.; devuelve: Reference to this.
- **append** `constexpr istring& append(const_iterator first, const_iterator last)` (L777): Appends additional characters to the istring.
  `first` First element of the range to append.; `last` Last element of the range to append.; devuelve: Reference to this.
- **erase** `constexpr iterator erase(const_iterator position)` (L787): Erases a character.
  `position` Iterator to the character to erase.; devuelve: Iterator following the erased character.
- **erase** `constexpr iterator erase(const_iterator first, const_iterator last)` (L808): Erases a range of characters.
  `first` Iterator to the first character to erase.; `last` Iterator to the last character to erase.; devuelve: Iterator following the last erased character.
  The range includes all the characters between first and last, including the
  character pointed by first, but not the one pointed by last.
- **resize** `constexpr void resize(size_type count)` (L826): Resizes the istring.
  `count` New size.
- **resize** `constexpr void resize(size_type count, value_type value)` (L836): Resizes the istring.
  `count` New size.; `value` Character to fill new elements with.
- **unsafe_resize** `constexpr void unsafe_resize(size_type count)` (L856): Resizes the istring without filling new characters or replacing the old null character.
  `count` New size.
- **shrink** `constexpr void shrink(size_type count)` (L868): Resizes the istring to a size less or equal than the previous one.
  `count` New size.
- **swap** `constexpr void swap(istring& other)` (L880): Exchanges the contents of this istring with those of the other one.
  `other` istring to exchange the contents with.
- **swap** `constexpr friend void swap(istring& a, istring& b)` (L928): Exchanges the contents of a istring with those of another one.
  `a` First istring to exchange the contents with.; `b` Second istring to exchange the contents with.
- **operator+=** `constexpr istring& operator+=(const istring_base& other)` (L938): Appends additional characters to the istring.
  `other` istring_base to append.; devuelve: Reference to this.
- **operator+=** `constexpr istring& operator+=(value_type value)` (L949): Appends an additional character to the istring.
  `value` Character to append.; devuelve: Reference to this.
- **operator+=** `constexpr istring& operator+=(const string_view& view)` (L960): Appends additional characters to the istring.
  `view` string_view to append.; devuelve: Reference to this.
- **operator+=** `constexpr istring& operator+=(const_pointer char_array_ptr)` (L971): Appends additional characters to the istring.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Reference to this.
- **operator==** `constexpr friend bool operator==(const istring_base& a, const_pointer b)` (L983): Equal operator.
  `a` istring_base to compare.; `b` char array to compare.; devuelve: `true` if the istring_base is equal to the char array, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const istring_base& a, const_pointer b)` (L1004): Less than operator.
  `a` istring_base to compare.; `b` char array to compare.; devuelve: `true` if the istring_base is lexicographically less than the char array, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const istring_base& a, const_pointer b)` (L1016): Greater than operator.
  `a` istring_base to compare.; `b` char array to compare.; devuelve: `true` if the istring_base is lexicographically greater than the char array, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const istring_base& a, const_pointer b)` (L1028): Less than or equal operator.
  `a` istring_base to compare.; `b` char array to compare.; devuelve: `true` if the istring_base is lexicographically less than or equal to the char array, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const istring_base& a, const_pointer b)` (L1040): Greater than or equal operator.
  `a` istring_base to compare.; `b` char array to compare.; devuelve: `true` if the istring_base is lexicographically greater than or equal to the char array, otherwise `false`.
- **string** `string() : istring(_buffer, 0, MaxSize)` (L1097): Default constructor.
- **string** `string(const string& other) : string()` (L1107): Copy constructor.
  `other` string to copy.
- **string** `string(const istring_base& other) : string()` (L1117): Copy constructor.
  `other` istring_base to copy.
- **string** `string(const istring_base& other, size_type position) : string()` (L1128): Copy constructor.
  `other` istring_base to copy.; `position` Starting character index.
- **string** `string(const istring_base& other, size_type position, size_type count) : string()` (L1140): Copy constructor.
  `other` istring_base to copy.; `position` Starting character index.; `count` Number of characters to copy.
- **string** `string(const string_view& view) : string()` (L1150): Copy constructor.
  `view` string_view to copy.
- **string** `string(const_pointer char_array_ptr) : string()` (L1160): Copy constructor.
  `char_array_ptr` Pointer to null-terminated characters array.
- **string** `string(const_pointer char_array_ptr, size_type char_array_size) : string()` (L1171): Copy constructor.
  `char_array_ptr` Pointer to characters array.; `char_array_size` Characters count of the characters array.
- **string** `string(size_type count, value_type value) : string()` (L1182): Constructs a string with count copies of character value.
  `count` Number of characters to assign.; `value` Character to assign.
- **string** `string(const_iterator first, const_iterator last) : string()` (L1193): Constructs a string with the characters in the range [first, last).
  `first` First element of the range.; `last` Last element of the range.
- **operator=** `string& operator=(const string& other)` (L1206): Copy assignment operator.
  `other` string to copy.; devuelve: Reference to this.
- **operator=** `string& operator=(const istring_base& other)` (L1217): Copy assignment operator.
  `other` istring_base to copy.; devuelve: Reference to this.
- **operator=** `string& operator=(const string_view& view)` (L1228): Copy assignment operator.
  `view` string_view to copy.; devuelve: Reference to this.
- **operator=** `string& operator=(const_pointer char_array_ptr)` (L1239): Copy assignment operator.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Reference to this.
- **substr** `string substr() const` (L1250): Returns a copy of this.
- **substr** `string substr(size_type position) const` (L1258): Returns the substring [position, size() - position).
- **substr** `string substr(size_type position, size_type count) const` (L1276): Returns the substring [position, position + count).
- **operator+** `friend string operator+(const string& a, const istring_base& b)` (L1298): Concatenates a string and a istring_base.
  `a` First string to concatenate.; `b` Second istring_base to concatenate.; devuelve: string containing characters from a followed by the characters from b.
- **operator+** `friend string operator+(const string& a, value_type b)` (L1311): Concatenates a string and a character.
  `a` First string to concatenate.; `b` Second character to concatenate.; devuelve: string containing characters from a followed by b.
- **operator+** `friend string operator+(const string& a, const string_view& b)` (L1324): Concatenates two strings.
  `a` First string to concatenate.; `b` Second string_view to concatenate.; devuelve: string containing characters from a followed by the characters from b.
- **operator+** `friend string operator+(const string& a, const_pointer b)` (L1337): Concatenates two strings.
  `a` First string to concatenate.; `b` Second pointer to null-terminated characters array to concatenate.; devuelve: string containing characters from a followed by the characters from b.
- **make_string** `template<int MaxSize> string<MaxSize - 1> make_string(const char (&char_array)[MaxSize])` (L1356): Creates a string deducing its size from the size of the argument.
  `char_array` Non empty const characters array.; devuelve: string containing the given char array.
- **to_string** `template<int MaxSize, typename Type> string<MaxSize> to_string(const Type& value)` (L1371): Converts the given value to a string.
  `MaxSize` Maximum number of characters that can be stored in the output string.; `value` Value to print in the string.; devuelve: string containing the representation of the given value.
- **erase** `constexpr istring::size_type erase(istring& string, char value)` (L1425): Erases all characters from a istring that are equal to the specified one.
  `string` istring from which to erase.; `value` Character to erase.; devuelve: Number of erased characters.
- **erase_if** `template<class Pred> constexpr istring::size_type erase_if(istring& string, const Pred& pred)` (L1438): Erases all characters from a istring that satisfy the specified predicate.
  `string` istring from which to erase.; `pred` Unary predicate which returns `true` if the character should be erased.; devuelve: Number of erased characters.

## bn_string_fwd.h

### `bn::string` (class, bn_string_fwd.h:33)

`std::string` like container with a fixed size buffer.

`template<int MaxSize> class string`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.


## bn_string_view.h

### `bn::string_view` (class, bn_string_view.h:33)

Refers to a constant contiguous sequence of char values.

`class string_view`

It doesn't store the char data like a string does.

- **string_view** `constexpr string_view() = default` (L54): Default constructor.
- **string_view** `constexpr string_view(const istring_base& string)` (L60): Constructor.
  `string` Base string to reference.
- **string_view** `constexpr string_view(const_pointer char_array_ptr)` (L70): Constructor.
  `char_array_ptr` Pointer to null-terminated characters array.
- **string_view** `constexpr string_view(const_pointer char_array_ptr, size_type char_array_size)` (L88): Constructor.
  `char_array_ptr` Pointer to characters array.; `char_array_size` Characters count of the characters array.
- **front** `constexpr const_reference front() const` (L102): Returns a const reference to the first character.
- **back** `constexpr const_reference back() const` (L112): Returns a const reference to the last character.
- **data** `constexpr const_pointer data() const` (L122): Returns a const pointer to the first character of the internal storage.
- **begin** `constexpr const_iterator begin() const` (L130): Returns a const iterator to the beginning of the string_view.
- **cbegin** `constexpr const_iterator cbegin() const` (L138): Returns a const iterator to the beginning of the string_view.
- **end** `constexpr const_iterator end() const` (L146): Returns a const iterator to the end of the string_view.
- **cend** `constexpr const_iterator cend() const` (L154): Returns a const iterator to the end of the string_view.
- **rbegin** `constexpr const_reverse_iterator rbegin() const` (L162): Returns a const reverse iterator to the end of the string_view.
- **crbegin** `constexpr const_reverse_iterator crbegin() const` (L170): Returns a const reverse iterator to the end of the string_view.
- **rend** `constexpr const_reverse_iterator rend() const` (L178): Returns a const reverse iterator to the beginning of the string_view.
- **crend** `constexpr const_reverse_iterator crend() const` (L186): Returns a const reverse iterator to the beginning of the string_view.
- **empty** `constexpr bool empty() const` (L194): Indicates if it doesn't contain any character.
- **size** `constexpr size_type size() const` (L202): Returns the number of characters in the string_view.
- **length** `constexpr size_type length() const` (L210): Returns the number of characters in the string_view.
- **operator[]** `constexpr const_reference operator[](size_type position) const` (L220): Returns a const reference to the character at the specified location.
  `position` Character's position index.; devuelve: Const reference to the character at the specified location.
- **at** `constexpr const_reference at(size_type position) const` (L232): Returns a const reference to the character at the specified location.
  `position` Character's position index.; devuelve: Const reference to the character at the specified location.
- **substr** `constexpr string_view substr() const` (L242): Returns a copy of this.
- **substr** `constexpr string_view substr(size_type position) const` (L250): Returns a string_view of the substring [position, size() - position).
- **substr** `constexpr string_view substr(size_type position, size_type count) const` (L268): Returns a string_view of the substring [position, position + count).
- **remove_prefix** `constexpr void remove_prefix(size_type count)` (L287): Shrinks the string_view by moving its start forward by count characters.
- **remove_suffix** `constexpr void remove_suffix(size_type count)` (L297): Shrinks the string_view by moving its end backward by count characters.
- **starts_with** `constexpr bool starts_with(value_type value) const` (L309): Checks if the referenced string begins with the given prefix.
  `value` Single character.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **starts_with** `constexpr bool starts_with(const string_view& other) const` (L319): Checks if the referenced string begins with the given prefix.
  `other` Another string_view.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **starts_with** `constexpr bool starts_with(const_pointer char_array_ptr) const` (L344): Checks if the referenced string begins with the given prefix.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string begins with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(value_type value) const` (L383): Checks if the referenced string ends with the given prefix.
  `value` Single character.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(const string_view& other) const` (L393): Checks if the referenced string ends with the given prefix.
  `other` Another string_view.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **ends_with** `constexpr bool ends_with(const_pointer char_array_ptr) const` (L411): Checks if the referenced string ends with the given prefix.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string ends with the given prefix; `false` otherwise.
- **contains** `constexpr bool contains(value_type value) const` (L423): Checks if the referenced string contains the given character.
  `value` Single character.; devuelve: `true` if the referenced string contains the given character; `false` otherwise.
- **contains** `constexpr bool contains(const string_view& other) const` (L433): Checks if the referenced string contains the given substring.
  `other` Another string_view.; devuelve: `true` if the referenced string contains the given substring; `false` otherwise.
- **contains** `constexpr bool contains(const_pointer char_array_ptr) const` (L443): Checks if the referenced string contains the given substring.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: `true` if the referenced string contains the given substring; `false` otherwise.
- **find** `constexpr size_type find(value_type value) const` (L455): Finds the first substring equal to the given character.
  `value` Single character.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **find** `constexpr size_type find(const string_view& other) const` (L473): Finds the first substring equal to the given character sequence.
  `other` Another string_view.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **find** `constexpr size_type find(const_pointer char_array_ptr) const` (L509): Finds the first substring equal to the given character sequence.
  `char_array_ptr` Pointer to null-terminated characters array.; devuelve: Position of the first character of the found substring, or npos if no such substring is found.
- **swap** `constexpr void swap(string_view& other)` (L520): Exchanges the contents of this string_view with those of the other one.
  `other` string_view to exchange the contents with.
- **swap** `constexpr friend void swap(string_view& a, string_view& b)` (L531): Exchanges the contents of a string_view with those of another one.
  `a` First string_view to exchange the contents with.; `b` Second string_view to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const string_view& a, const string_view& b)` (L542): Equal operator.
  `a` First string_view to compare.; `b` Second string_view to compare.; devuelve: `true` if the first string_view is equal to the second one, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const string_view& a, const string_view& b)` (L569): Less than operator.
  `a` First string_view to compare.; `b` Second string_view to compare.; devuelve: `true` if the first string_view is lexicographically less than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const string_view& a, const string_view& b)` (L581): Greater than operator.
  `a` First string_view to compare.; `b` Second string_view to compare.; devuelve: `true` if the first string_view is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const string_view& a, const string_view& b)` (L593): Less than or equal operator.
  `a` First string_view to compare.; `b` Second string_view to compare.; devuelve: `true` if the first string_view is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const string_view& a, const string_view& b)` (L605): Greater than or equal operator.
  `a` First string_view to compare.; `b` Second string_view to compare.; devuelve: `true` if the first string_view is lexicographically greater than or equal to the second one, otherwise `false`.
