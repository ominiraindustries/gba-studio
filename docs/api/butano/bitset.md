# Butano 21.10.1 — Bitset (`bitset`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_bitset.h

### `bn::ibitset` (class, bn_bitset.h:57)

Base class of bn::bitset.

`class ibitset`

Can be used as a reference type for all bn::bitset containers.

### `bn::ibitset::reference` (class, bn_bitset.h:66)

Proxy to allow to interact with individual bits of a ibitset.

`class reference`

- **operator bool** `constexpr operator bool() const` (L73): Returns the value of the referenced bit.
- **operator~** `constexpr bool operator~() const` (L81): Returns the inverse of the referenced bit.
- **operator=** `constexpr reference& operator=(bool value)` (L91): Assigns a value to the referenced bit.
  `value` Value to assign.; devuelve: Reference to this.
- **operator=** `constexpr reference& operator=(const reference& other)` (L102): Assigns a value to the referenced bit.
  `other` Reference to the value to assign.; devuelve: Reference to this.
- **flip** `constexpr reference& flip()` (L112): Flips the referenced bit.
  devuelve: Reference to this.
- **operator=** `constexpr ibitset& operator=(const ibitset& other)` (L138): Copy assignment operator.
  `other` ibitset to copy.; devuelve: Reference to this.
- **data** `constexpr const element_t* data() const` (L154): Returns a const pointer to the beginning of the internal data.
- **size** `constexpr int size() const` (L162): Returns the bits count.
- **elements_size** `constexpr int elements_size() const` (L170): Returns the elements count.
- **count** `constexpr int count() const` (L178): Returns the number of bits that are set to `true`.
- **test** `constexpr bool test(int index) const` (L193): Returns the value stored at the specified index.
- **set** `constexpr ibitset& set()` (L204): Sets all bits to `true`.
  devuelve: Reference to this.
- **set** `constexpr ibitset& set(int index)` (L215): Sets the specified bit to `true`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **set** `constexpr ibitset& set(int index, bool value)` (L229): Sets the specified bit to the specified value.
  `index` Index of the bit to set.; `value` Value to set.; devuelve: Reference to this.
- **reset** `constexpr ibitset& reset()` (L252): Sets all bits to `false`.
  devuelve: Reference to this.
- **reset** `constexpr ibitset& reset(int index)` (L263): Sets the specified bit to `false`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **flip** `constexpr ibitset& flip()` (L275): Flips all values (changes `true` values to `false` and vice versa).
  devuelve: Reference to this.
- **flip** `constexpr ibitset& flip(int index)` (L292): Flips the specified bit value (changes `true` value to `false` and vice versa).
  `index` Index of the bit to flip.; devuelve: Reference to this.
- **operator[]** `constexpr bool operator[](int index) const` (L303): Returns the value stored at the specified index.
- **operator[]** `constexpr reference operator[](int index)` (L311): Returns a reference to the value stored at the specified index.
- **at** `constexpr bool at(int index) const` (L321): Returns the value stored at the specified index.
- **at** `constexpr reference at(int index)` (L329): Returns a reference to the value stored at the specified index.
- **all** `constexpr bool all() const` (L339): Indicates if all values are `true`.
- **any** `constexpr bool any() const` (L357): Indicates if any value is `true`.
- **none** `constexpr bool none() const` (L365): Indicates if all values are `false`.
- **operator&=** `constexpr ibitset& operator&=(const ibitset& other)` (L385): Sets the bits to the result of binary AND of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator|=** `constexpr ibitset& operator|=(const ibitset& other)` (L405): Sets the bits to the result of binary OR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator^=** `constexpr ibitset& operator^=(const ibitset& other)` (L425): Sets the bits to the result of binary XOR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **swap** `constexpr void swap(ibitset& other)` (L444): Exchanges the contents of this ibitset with those of the other one.
  `other` ibitset to exchange the contents with.
- **swap** `constexpr friend void swap(ibitset& a, ibitset& b)` (L458): Exchanges the contents of an ibitset with those of another one.
  `a` First ibitset to exchange the contents with.; `b` Second ibitset to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const ibitset& a, const ibitset& b)` (L469): Equal operator.
  `a` First ibitset to compare.; `b` Second ibitset to compare.; devuelve: `true` if the first ibitset is equal to the second one, otherwise `false`.
- **bitset** `bitset() : ibitset(_num_elements, _storage_buffer)` (L545): Default constructor.
- **bitset** `bitset(const bitset& other) : ibitset(_num_elements, _storage_buffer)` (L554): Copy constructor.
  `other` bitset to copy.
- **bitset** `bitset(const ibitset& other) : ibitset(_num_elements, _storage_buffer)` (L564): Copy constructor.
  `other` ibitset to copy.
- **operator=** `bitset& operator=(const bitset& other)` (L575): Copy assignment operator.
  `other` bitset to copy.; devuelve: Reference to this.
- **operator=** `bitset& operator=(const ibitset& other)` (L590): Copy assignment operator.
  `other` ibitset to copy.; devuelve: Reference to this.
- **set** `bitset& set()` (L600): Sets all bits to `true`.
  devuelve: Reference to this.
- **set** `bitset& set(int index)` (L611): Sets the specified bit to `true`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **set** `bitset& set(int index, bool value)` (L623): Sets the specified bit to the specified value.
  `index` Index of the bit to set.; `value` Value to set.; devuelve: Reference to this.
- **reset** `bitset& reset()` (L633): Sets all bits to `false`.
  devuelve: Reference to this.
- **reset** `bitset& reset(int index)` (L644): Sets the specified bit to `false`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **flip** `bitset& flip()` (L654): Flips all values (changes `true` values to `false` and vice versa).
  devuelve: Reference to this.
- **flip** `bitset& flip(int index)` (L665): Flips the specified bit value (changes `true` value to `false` and vice versa).
  `index` Index of the bit to flip.; devuelve: Reference to this.
- **operator&=** `bitset& operator&=(const ibitset& other)` (L676): Sets the bits to the result of binary AND of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator|=** `bitset& operator|=(const ibitset& other)` (L687): Sets the bits to the result of binary OR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator^=** `bitset& operator^=(const ibitset& other)` (L698): Sets the bits to the result of binary XOR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator~** `bitset operator~() const` (L707): Returns this bitset with all bits flipped (binary NOT).
- **operator&** `friend bitset operator&(const bitset& a, const bitset& b)` (L717): Returns a bitset containing the result of binary AND of the given bitsets.
- **operator|** `friend bitset operator|(const bitset& a, const bitset& b)` (L727): Returns a bitset containing the result of binary OR of the given bitsets.
- **operator^** `friend bitset operator^(const bitset& a, const bitset& b)` (L737): Returns a bitset containing the result of binary XOR of the given bitsets.
### `bn::bitset_ref` (class, bn_bitset.h:754)

ibitset implementation which manages an external elements array.

`class bitset_ref : public ibitset`

- **bitset_ref** `template<int ElementSize> constexpr bitset_ref(element_t (&elements_array_ref)[ElementSize]) : ibitset(ElementSize, elements_array_ref)` (L765): Constructor.
  `elements_array_ref` Elements array reference.
  The elements are not copied but referenced, so they should outlive the bitset_ref
  to avoid dangling references.
- **bitset_ref** `constexpr bitset_ref(span<element_t> elements_ref) : ibitset(elements_ref.size(), elements_ref.data())` (L779): Constructor.
  `elements_ref` Elements reference.
  The elements are not copied but referenced, so they should outlive the bitset_ref
  to avoid dangling references.
- **bitset_ref** `constexpr bitset_ref(bitset_ref&& other) noexcept : ibitset(other.elements_size(), other.data())` (L789): Move constructor.
  `other` bitset_ref to move.
- **operator=** `constexpr bitset_ref& operator=(const ibitset& other)` (L799): Copy assignment operator.
  `other` ibitset to copy.; devuelve: Reference to this.
- **set** `constexpr bitset_ref& set()` (L809): Sets all bits to `true`.
  devuelve: Reference to this.
- **set** `constexpr bitset_ref& set(int index)` (L820): Sets the specified bit to `true`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **set** `constexpr bitset_ref& set(int index, bool value)` (L832): Sets the specified bit to the specified value.
  `index` Index of the bit to set.; `value` Value to set.; devuelve: Reference to this.
- **reset** `constexpr bitset_ref& reset()` (L842): Sets all bits to `false`.
  devuelve: Reference to this.
- **reset** `constexpr bitset_ref& reset(int index)` (L853): Sets the specified bit to `false`.
  `index` Index of the bit to set.; devuelve: Reference to this.
- **flip** `constexpr bitset_ref& flip()` (L863): Flips all values (changes `true` values to `false` and vice versa).
  devuelve: Reference to this.
- **flip** `constexpr bitset_ref& flip(int index)` (L874): Flips the specified bit value (changes `true` value to `false` and vice versa).
  `index` Index of the bit to flip.; devuelve: Reference to this.
- **operator&=** `constexpr bitset_ref& operator&=(const ibitset& other)` (L885): Sets the bits to the result of binary AND of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator|=** `constexpr bitset_ref& operator|=(const ibitset& other)` (L896): Sets the bits to the result of binary OR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator^=** `constexpr bitset_ref& operator^=(const ibitset& other)` (L907): Sets the bits to the result of binary XOR of this ibitset and the given one.
  `other` Another ibitset.; devuelve: Reference to this.
- **operator<<** `inline ostringstream& operator<<(ostringstream& stream, const ibitset& value)` (L945): Appends the character representation of the given ibitset to the given ostringstream.

## bn_bitset_fwd.h

### `bn::bitset` (class, bn_bitset_fwd.h:31)

`std::bitset` like container.

`template<int Size> class bitset`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

