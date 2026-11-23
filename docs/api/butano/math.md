# Butano 21.10.1 — Math (`math`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_clip_line.h

- **clip_line** `template<typename Type> constexpr optional<array<Type, 4>> clip_line( Type x1, Type y1, Type x2, Type y2, Type clip_x, Type clip_y, Type clip_width, Type clip_height)` (L39): Clips a line to a rectangular area.
  `x1` Horizontal position of the start point of the line.; `y1` Vertical position of the start point of the line.; `x2` Horizontal position of the end point of the line.; `y2` Vertical position of the end point of the line.; `clip_x` Horizontal position of the top-left point of the clipping rectangle.; `clip_y` Vertical position of the top-left point of the clipping rectangle.; `clip_width` Width of the clipping rectangle (> 0).; `clip_height` Height of the clipping rectangle (> 0).; devuelve: An array with the clipped line coordinates in the form { x1, y1, x2, y2 } if at least part of the line is inside the rectangular clipping area; bn::nullopt otherwise.
  Implementation provided by https://github.com/scivision/lineclipping-julia

## bn_fixed.h

- **from_data** `static constexpr fixed_t from_data(int data)` (L35): Returns a fixed_t with the given internal data.
  If you want to convert an integer value to fixed_t, you should use its constructor.
- **precision** `static constexpr int precision()` (L45): Returns the number of bits used for the fractional part.
- **scale** `static constexpr int scale()` (L53): Returns the internal data of fixed_t<Precision>(1);
- **half_scale** `static constexpr int half_scale()` (L61): Returns the internal data of fixed_t<Precision / 2>(1);
- **fixed_t** `constexpr fixed_t() = default` (L71): Default constructor.
- **fixed_t** `constexpr fixed_t(int value)` (L77): Constructor.
  `value` Integer value.
- **fixed_t** `constexpr fixed_t(unsigned value)` (L86): Constructor.
  `value` Unsigned integer value.
- **fixed_t** `constexpr fixed_t(float value)` (L95): Constructor.
  `value` Single precision floating point value.
- **fixed_t** `constexpr fixed_t(double value)` (L104): Constructor.
  `value` Double precision floating point value.
- **fixed_t** `template<int OtherPrecision> constexpr fixed_t(fixed_t<OtherPrecision> other)` (L113): Constructor.
  `other` Fixed point value.
- **data** `constexpr int data() const` (L124): Returns the internal data.
- **integer** `constexpr int integer() const` (L132): Returns the integer part using a division.
- **operator int** `constexpr explicit operator int() const` (L140): Returns the integer part using a division.
- **shift_integer** `constexpr int shift_integer() const` (L148): Returns the integer part using a right shift.
- **right_shift_integer** `constexpr int right_shift_integer() const` (L156): Returns the integer part using a right shift.
- **unsigned_integer** `constexpr unsigned unsigned_integer() const` (L164): Returns the integer part, assuming that it is >= 0.
- **round_integer** `constexpr int round_integer() const` (L172): Returns the nearest integer value.
- **floor_integer** `constexpr int floor_integer() const` (L189): Returns the largest integer value not greater than this value.
- **ceil_integer** `constexpr int ceil_integer() const` (L205): Returns the smallest integer value not less than this value.
- **fraction** `constexpr int fraction() const` (L221): Returns the fractional part.
- **to_float** `constexpr float to_float() const` (L229): Returns the nearest single precision floating point value.
- **operator float** `constexpr explicit operator float() const` (L237): Returns the nearest single precision floating point value.
- **to_double** `constexpr double to_double() const` (L245): Returns the nearest double precision floating point value.
- **operator double** `constexpr explicit operator double() const` (L253): Returns the nearest double precision floating point value.
- **multiplication** `constexpr fixed_t multiplication(int value) const` (L261): Returns the multiplication of this value by the given integer value.
- **multiplication** `constexpr fixed_t multiplication(fixed_t other) const` (L270): Returns the multiplication of this value by the given fixed point value, using half precision to try to avoid overflow.
- **shift_multiplication** `constexpr fixed_t shift_multiplication(int value) const` (L285): Returns the multiplication of this value by the given integer value.
- **shift_multiplication** `constexpr fixed_t shift_multiplication(fixed_t other) const` (L294): Returns the multiplication of this value by the given fixed point value, using half precision to try to avoid overflow and right shifts instead of divisions for the precision handling.
- **safe_multiplication** `constexpr fixed_t safe_multiplication(int value) const` (L309): Returns the multiplication of this value by the given integer value.
- **safe_multiplication** `constexpr fixed_t safe_multiplication(fixed_t other) const` (L318): Returns the multiplication of this value by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_multiplication** `constexpr fixed_t unsafe_multiplication(int value) const` (L326): Returns the multiplication of this value by the given integer value.
- **unsafe_multiplication** `constexpr fixed_t unsafe_multiplication(fixed_t other) const` (L335): Returns the multiplication of this value by the given fixed point value without trying to avoid overflow.
- **unsafe_shift_multiplication** `constexpr fixed_t unsafe_shift_multiplication(int value) const` (L343): Returns the multiplication of this value by the given integer value.
- **unsafe_shift_multiplication** `constexpr fixed_t unsafe_shift_multiplication(fixed_t other) const` (L352): Returns the multiplication of this value by the given fixed point value without trying to avoid overflow, using a right shift instead of a division for the precision handling.
- **division** `constexpr fixed_t division(int value) const` (L360): Returns the division of this value by the given integer value.
- **division** `constexpr fixed_t division(fixed_t other) const` (L369): Returns the division of this value by the given fixed point value, using half precision to try to avoid overflow.
- **shift_division** `constexpr fixed_t shift_division(int value) const` (L384): Returns the division of this value by the given integer value.
- **shift_division** `constexpr fixed_t shift_division(fixed_t other) const` (L393): Returns the division of this value by the given fixed point value, using half precision to try to avoid overflow and shifts for the precision handling.
- **safe_division** `constexpr fixed_t safe_division(int value) const` (L408): Returns the division of this value by the given integer value.
- **safe_division** `constexpr fixed_t safe_division(fixed_t other) const` (L417): Returns the division of this value by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_division** `constexpr fixed_t unsafe_division(int value) const` (L425): Returns the division of this value by the given integer value.
- **unsafe_division** `constexpr fixed_t unsafe_division(fixed_t other) const` (L433): Returns the division of this value by the given fixed point value without trying to avoid overflow.
- **unsafe_shift_division** `constexpr fixed_t unsafe_shift_division(int value) const` (L441): Returns the division of this value by the given integer value.
- **unsafe_shift_division** `constexpr fixed_t unsafe_shift_division(fixed_t other) const` (L450): Returns the division of this value by the given fixed point value without trying to avoid overflow, using a left shift instead of a multiplication for the precision handling.
- **modulo** `constexpr fixed_t modulo(int value) const` (L458): Returns the remainder of the division of this value by the given integer value.
- **left_shift** `constexpr fixed_t left_shift(int bits) const` (L467): Returns this value shifted to the left by the given bits.
- **right_shift** `constexpr fixed_t right_shift(int bits) const` (L475): Returns this value shifted to the right by the given bits.
- **operator-** `constexpr fixed_t operator-() const` (L483): Returns a fixed_t that is formed by changing the sign of this one.
- **operator+=** `constexpr fixed_t& operator+=(fixed_t other)` (L493): Adds the given fixed_t to this one.
  `other` fixed_t to add.; devuelve: Reference to this.
- **operator-=** `constexpr fixed_t& operator-=(fixed_t other)` (L504): Subtracts the given fixed_t to this one.
  `other` fixed_t to subtract.; devuelve: Reference to this.
- **operator*=** `constexpr fixed_t& operator*=(int value)` (L515): Multiplies this fixed_t by the given factor.
  `value` Integer multiplication factor.; devuelve: Reference to this.
- **operator*=** `constexpr fixed_t& operator*=(fixed_t other)` (L526): Multiplies this fixed_t by the given factor.
  `other` Fixed point multiplication factor.; devuelve: Reference to this.
- **operator/=** `constexpr fixed_t& operator/=(int value)` (L537): Divides this fixed_t by the given divisor.
  `value` Valid integer divisor (!= 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_t& operator/=(fixed_t other)` (L548): Divides this fixed_t by the given divisor.
  `other` Valid fixed point divisor (!= 0).; devuelve: Reference to this.
- **operator%=** `constexpr fixed_t& operator%=(int other)` (L559): Assigns the remainder of the division of this fixed_t by the given divisor.
  `other` Valid integer divisor (!= 0).; devuelve: Reference to this.
- **operator<<=** `constexpr fixed_t& operator<<=(int bits)` (L570): Shifts to the left this fixed_t by the given bits.
  `bits` Number of bits to shift to the left.; devuelve: Reference to this.
- **operator>>=** `constexpr fixed_t& operator>>=(int bits)` (L581): Shifts to the right this fixed_t by the given bits.
  `bits` Number of bits to shift to the right.; devuelve: Reference to this.
- **operator+** `constexpr friend fixed_t operator+(fixed_t a, fixed_t b)` (L590): Returns the sum of a and b.
- **max** `template<int OtherPrecision> constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator+( fixed_t a, fixed_t<OtherPrecision> b)` (L598): Returns the sum of a and b.
- **operator-** `constexpr friend fixed_t operator-(fixed_t a, fixed_t b)` (L610): Returns the subtraction of a and b.
- **max** `template<int OtherPrecision> constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator-( fixed_t a, fixed_t<OtherPrecision> b)` (L618): Returns the subtraction of a and b.
- **operator*** `constexpr friend fixed_t operator*(fixed_t a, int b)` (L630): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_t operator*(int a, fixed_t b)` (L638): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_t operator*(fixed_t a, fixed_t b)` (L646): Returns a multiplied by b.
- **max** `template<int OtherPrecision> constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator*( fixed_t a, fixed_t<OtherPrecision> b)` (L654): Returns a multiplied by b.
- **operator/** `constexpr friend fixed_t operator/(fixed_t a, int b)` (L666): Returns a divided by b.
- **operator/** `constexpr friend fixed_t operator/(fixed_t a, fixed_t b)` (L674): Returns a divided by b.
- **max** `template<int OtherPrecision> constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator/( fixed_t a, fixed_t<OtherPrecision> b)` (L682): Returns a divided by b.
- **operator%** `constexpr friend fixed_t operator%(fixed_t a, int b)` (L694): Returns the remainder of the division of a by b.
- **operator<<** `constexpr friend fixed_t operator<<(fixed_t a, int b)` (L702): Returns a shifted to the left by b.
- **operator>>** `constexpr friend fixed_t operator>>(fixed_t a, int b)` (L710): Returns a shifted to the right by b.
- **operator<=>** `constexpr friend auto operator<=>(fixed_t a, fixed_t b) = default` (L718): Default three-way comparison operator.

## bn_fixed_fwd.h

### `bn::fixed_t` (class, bn_fixed_fwd.h:30)

Poor man's float.

`template<int Precision> class fixed_t`

It fakes numbers with decimal points with integers.


## bn_fixed_point.h

- **fixed_point_t** `constexpr fixed_point_t() = default` (L31): Default constructor.
- **fixed_point_t** `constexpr fixed_point_t(fixed_t<Precision> x, fixed_t<Precision> y)` (L38): Constructor.
  `x` Horizontal coordinate.; `y` Vertical coordinate.
- **fixed_point_t** `constexpr fixed_point_t(const point& point)` (L48): Constructor.
  `point` Integer point.
- **x** `constexpr fixed_t<Precision> x() const` (L57): Returns the horizontal coordinate.
- **set_x** `constexpr void set_x(fixed_t<Precision> x)` (L65): Sets the horizontal coordinate.
- **y** `constexpr fixed_t<Precision> y() const` (L73): Returns the vertical coordinate.
- **set_y** `constexpr void set_y(fixed_t<Precision> y)` (L81): Sets the vertical coordinate.
- **multiplication** `constexpr fixed_point_t multiplication(int value) const` (L89): Returns the multiplication of this point by the given integer value.
- **multiplication** `constexpr fixed_point_t multiplication(fixed_t<Precision> value) const` (L98): Returns the multiplication of this point by the given fixed point value, using half precision to try to avoid overflow.
- **safe_multiplication** `constexpr fixed_point_t safe_multiplication(int value) const` (L106): Returns the multiplication of this point by the given integer value.
- **safe_multiplication** `constexpr fixed_point_t safe_multiplication(fixed_t<Precision> value) const` (L115): Returns the multiplication of this point by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_multiplication** `constexpr fixed_point_t unsafe_multiplication(int value) const` (L123): Returns the multiplication of this point by the given integer value.
- **unsafe_multiplication** `constexpr fixed_point_t unsafe_multiplication(fixed_t<Precision> value) const` (L132): Returns the multiplication of this point by the given fixed point value without trying to avoid overflow.
- **division** `constexpr fixed_point_t division(int value) const` (L140): Returns the division of this point by the given integer value.
- **division** `constexpr fixed_point_t division(fixed_t<Precision> value) const` (L149): Returns the division of this point by the given fixed point value, using half precision to try to avoid overflow.
- **safe_division** `constexpr fixed_point_t safe_division(int value) const` (L157): Returns the division of this value by the given integer value.
- **safe_division** `constexpr fixed_point_t safe_division(fixed_t<Precision> value) const` (L166): Returns the division of this point by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_division** `constexpr fixed_point_t unsafe_division(int value) const` (L174): Returns the division of this value by the given integer value.
- **unsafe_division** `constexpr fixed_point_t unsafe_division(fixed_t<Precision> value) const` (L183): Returns the division of this point by the given fixed point value without trying to avoid overflow.
- **operator-** `constexpr fixed_point_t operator-() const` (L191): Returns a fixed_point_t that is formed by changing the sign of both coordinates.
- **operator+=** `constexpr fixed_point_t& operator+=(const fixed_point_t& other)` (L201): Adds the given fixed_point_t to this one.
  `other` fixed_point_t to add.; devuelve: Reference to this.
- **operator-=** `constexpr fixed_point_t& operator-=(const fixed_point_t& other)` (L213): Subtracts the given fixed_point_t to this one.
  `other` fixed_point_t to subtract.; devuelve: Reference to this.
- **operator*=** `constexpr fixed_point_t& operator*=(int value)` (L225): Multiplies both coordinates by the given factor.
  `value` Integer multiplication factor.; devuelve: Reference to this.
- **operator*=** `constexpr fixed_point_t& operator*=(fixed_t<Precision> value)` (L237): Multiplies both coordinates by the given factor.
  `value` Fixed point multiplication factor.; devuelve: Reference to this.
- **operator/=** `constexpr fixed_point_t& operator/=(int value)` (L249): Divides both coordinates by the given divisor.
  `value` Valid integer divisor (!= 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_point_t& operator/=(fixed_t<Precision> value)` (L261): Divides both coordinates by the given divisor.
  `value` Valid fixed point divisor (!= 0).; devuelve: Reference to this.
- **operator+** `constexpr friend fixed_point_t operator+(const fixed_point_t& a, const fixed_point_t& b)` (L271): Returns the sum of a and b.
- **operator-** `constexpr friend fixed_point_t operator-(const fixed_point_t& a, const fixed_point_t& b)` (L279): Returns b subtracted from a.
- **operator*** `constexpr friend fixed_point_t operator*(const fixed_point_t& a, int b)` (L287): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_point_t operator*(int a, const fixed_point_t& b)` (L295): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_point_t operator*(const fixed_point_t& a, fixed_t<Precision> b)` (L303): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_point_t operator*(fixed_t<Precision> a, const fixed_point_t& b)` (L311): Returns a multiplied by b.
- **operator/** `constexpr friend fixed_point_t operator/(const fixed_point_t& a, int b)` (L319): Returns a divided by b.
- **operator/** `constexpr friend fixed_point_t operator/(const fixed_point_t& a, fixed_t<Precision> b)` (L327): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const fixed_point_t& a, const fixed_point_t& b) = default` (L335): Default equal operator.

## bn_fixed_point_fwd.h

### `bn::fixed_point_t` (class, bn_fixed_point_fwd.h:26)

Defines a two-dimensional point using fixed point precision.

`template<int Precision> class fixed_point_t`


## bn_fixed_rect.h

- **fixed_rect_t** `constexpr fixed_rect_t() = default` (L32): Default constructor.
- **fixed_rect_t** `constexpr fixed_rect_t(const fixed_point_t<Precision>& position, const fixed_size_t<Precision>& dimensions)` (L39): Constructor.
  `position` Position of the center point of the rectangle.; `dimensions` Size of the rectangle.
- **fixed_rect_t** `constexpr fixed_rect_t( fixed_t<Precision> x, fixed_t<Precision> y, fixed_t<Precision> width, fixed_t<Precision> height)` (L52): Constructor.
  `x` Horizontal position of the center point of the rectangle.; `y` Vertical position of the center point of the rectangle.; `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **fixed_rect_t** `constexpr fixed_rect_t(const rect& rect)` (L63): Constructor.
  `rect` Integer rectangle.
- **position** `constexpr const fixed_point_t<Precision>& position() const` (L72): Returns the position of the center point of the rectangle.
- **set_position** `constexpr void set_position(const fixed_point_t<Precision>& position)` (L80): Sets the position of the center point of the rectangle.
- **set_position** `constexpr void set_position(fixed_t<Precision> x, fixed_t<Precision> y)` (L90): Sets the position of the center point of the rectangle.
  `x` Horizontal position of the center point of the rectangle.; `y` Vertical position of the center point of the rectangle.
- **dimensions** `constexpr const fixed_size_t<Precision>& dimensions() const` (L98): Returns the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(const fixed_size_t<Precision>& dimensions)` (L106): Sets the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(fixed_t<Precision> width, fixed_t<Precision> height)` (L116): Sets the size of the rectangle.
  `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **x** `constexpr fixed_t<Precision> x() const` (L124): Returns the horizontal position of the center point of the rectangle.
- **set_x** `constexpr void set_x(fixed_t<Precision> x)` (L132): Sets the horizontal position of the center point of the rectangle.
- **y** `constexpr fixed_t<Precision> y() const` (L140): Returns the vertical position of the center point of the rectangle.
- **set_y** `constexpr void set_y(fixed_t<Precision> y)` (L148): Sets the vertical position of the center point of the rectangle.
- **width** `constexpr fixed_t<Precision> width() const` (L156): Returns the width of the rectangle.
- **set_width** `constexpr void set_width(fixed_t<Precision> width)` (L165): Sets the width of the rectangle.
  `width` Valid width (>= 0).
- **height** `constexpr fixed_t<Precision> height() const` (L173): Returns the height of the rectangle.
- **set_height** `constexpr void set_height(fixed_t<Precision> height)` (L182): Sets the height of the rectangle.
  `height` Valid height (>= 0).
- **top** `constexpr fixed_t<Precision> top() const` (L190): Returns the position of the top boundary of the rectangle.
- **left** `constexpr fixed_t<Precision> left() const` (L198): Returns the position of the left boundary of the rectangle.
- **bottom** `constexpr fixed_t<Precision> bottom() const` (L206): Returns the position of the bottom boundary of the rectangle.
- **right** `constexpr fixed_t<Precision> right() const` (L214): Returns the position of the right boundary of the rectangle.
- **top_left** `constexpr fixed_point_t<Precision> top_left() const` (L222): Returns the position of the top-left corner of the rectangle.
- **top_right** `constexpr fixed_point_t<Precision> top_right() const` (L230): Returns the position of the top-right corner of the rectangle.
- **bottom_left** `constexpr fixed_point_t<Precision> bottom_left() const` (L238): Returns the position of the bottom-left corner of the rectangle.
- **bottom_right** `constexpr fixed_point_t<Precision> bottom_right() const` (L246): Returns the position of the bottom-right corner of the rectangle.
- **center_x** `constexpr fixed_t<Precision> center_x() const` (L254): Returns the horizontal position of the center point of the rectangle.
- **center_y** `constexpr fixed_t<Precision> center_y() const` (L262): Returns the vertical position of the center point of the rectangle.
- **center** `constexpr fixed_point_t<Precision> center() const` (L270): Returns the position of the center point of the rectangle.
- **contains** `constexpr bool contains(const fixed_point_t<Precision>& point) const` (L280): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const fixed_point_t<Precision>& point) const` (L298): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `true`.
- **contains** `constexpr bool contains(const fixed_rect_t& other) const` (L316): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const fixed_rect_t& other) const` (L327): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `true`.
- **intersects** `constexpr bool intersects(const fixed_rect_t& other) const` (L339): Indicates if this rectangle intersects with the given one or not.
  Two rectangles intersect if there is at least one point that is within both rectangles,
  excluding their edges.
- **touches** `constexpr bool touches(const fixed_rect_t& other) const` (L360): Indicates if this rectangle touches the given one or not.
  Two rectangles touch each other if there is at least one point that is within both rectangles,
  including their edges.
- **multiplication** `constexpr fixed_rect_t multiplication(int value) const` (L378): Returns the multiplication of this rectangle by the given integer value.
- **multiplication** `constexpr fixed_rect_t multiplication(fixed_t<Precision> value) const` (L387): Returns the multiplication of this rectangle by the given fixed point value, using half precision to try to avoid overflow.
- **safe_multiplication** `constexpr fixed_rect_t safe_multiplication(int value) const` (L395): Returns the multiplication of this rectangle by the given integer value.
- **safe_multiplication** `constexpr fixed_rect_t safe_multiplication(fixed_t<Precision> value) const` (L404): Returns the multiplication of this rectangle by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_multiplication** `constexpr fixed_rect_t unsafe_multiplication(int value) const` (L412): Returns the multiplication of this rectangle by the given integer value.
- **unsafe_multiplication** `constexpr fixed_rect_t unsafe_multiplication(fixed_t<Precision> value) const` (L421): Returns the multiplication of this rectangle by the given fixed point value without trying to avoid overflow.
- **division** `constexpr fixed_rect_t division(int value) const` (L429): Returns the division of this rectangle by the given integer value.
- **division** `constexpr fixed_rect_t division(fixed_t<Precision> value) const` (L438): Returns the division of this rectangle by the given fixed point value, using half precision to try to avoid overflow.
- **safe_division** `constexpr fixed_rect_t safe_division(int value) const` (L446): Returns the division of this rectangle by the given integer value.
- **safe_division** `constexpr fixed_rect_t safe_division(fixed_t<Precision> value) const` (L455): Returns the division of this rectangle by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_division** `constexpr fixed_rect_t unsafe_division(int value) const` (L463): Returns the division of this rectangle by the given integer value.
- **unsafe_division** `constexpr fixed_rect_t unsafe_division(fixed_t<Precision> value) const` (L472): Returns the division of this rectangle by the given fixed point value without trying to avoid overflow.
- **operator*=** `constexpr fixed_rect_t& operator*=(int value)` (L482): Multiplies both width and height of the rectangle by the given factor.
  `value` Valid integer multiplication factor (>= 0).; devuelve: Reference to this.
- **operator*=** `constexpr fixed_rect_t& operator*=(fixed_t<Precision> value)` (L493): Multiplies both width and height of the rectangle by the given factor.
  `value` Valid fixed point multiplication factor (>= 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_rect_t& operator/=(int value)` (L504): Divides both width and height of the rectangle by the given divisor.
  `value` Valid integer divisor (> 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_rect_t& operator/=(fixed_t<Precision> value)` (L515): Divides both width and height of the rectangle by the given divisor.
  `value` Valid fixed point divisor (> 0).; devuelve: Reference to this.
- **operator*** `constexpr friend fixed_rect_t operator*(const fixed_rect_t& a, int b)` (L524): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_rect_t operator*(int a, const fixed_rect_t& b)` (L532): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_rect_t operator*(const fixed_rect_t& a, fixed_t<Precision> b)` (L540): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_rect_t operator*(fixed_t<Precision> a, const fixed_rect_t& b)` (L548): Returns a multiplied by b.
- **operator/** `constexpr friend fixed_rect_t operator/(const fixed_rect_t& a, int b)` (L556): Returns a divided by b.
- **operator/** `constexpr friend fixed_rect_t operator/(const fixed_rect_t& a, fixed_t<Precision> b)` (L564): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const fixed_rect_t& a, const fixed_rect_t& b) = default` (L572): Default equal operator.

## bn_fixed_rect_fwd.h

### `bn::fixed_rect_t` (class, bn_fixed_rect_fwd.h:26)

Defines a two-dimensional rectangle using fixed point precision and a center point as position.

`template<int Precision> class fixed_rect_t`


## bn_fixed_size.h

- **fixed_size_t** `constexpr fixed_size_t() = default` (L31): Default constructor.
- **fixed_size_t** `constexpr fixed_size_t(fixed_t<Precision> width, fixed_t<Precision> height)` (L38): Constructor.
  `width` Valid width (>= 0).; `height` Valid height (>= 0).
- **fixed_size_t** `constexpr fixed_size_t(const size& size)` (L50): Constructor.
  `size` Integer size.
- **width** `constexpr fixed_t<Precision> width() const` (L61): Returns the width.
- **set_width** `constexpr void set_width(fixed_t<Precision> width)` (L70): Sets the width.
  `width` Valid width (>= 0).
- **height** `constexpr fixed_t<Precision> height() const` (L80): Returns the height.
- **set_height** `constexpr void set_height(fixed_t<Precision> height)` (L89): Sets the height.
  `height` Valid height (>= 0).
- **multiplication** `constexpr fixed_size_t multiplication(int value) const` (L99): Returns the multiplication of this size by the given integer value.
- **multiplication** `constexpr fixed_size_t multiplication(fixed_t<Precision> value) const` (L108): Returns the multiplication of this size by the given fixed point value, using half precision to try to avoid overflow.
- **safe_multiplication** `constexpr fixed_size_t safe_multiplication(int value) const` (L116): Returns the multiplication of this size by the given integer value.
- **safe_multiplication** `constexpr fixed_size_t safe_multiplication(fixed_t<Precision> value) const` (L125): Returns the multiplication of this size by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_multiplication** `constexpr fixed_size_t unsafe_multiplication(int value) const` (L133): Returns the multiplication of this size by the given integer value.
- **unsafe_multiplication** `constexpr fixed_size_t unsafe_multiplication(fixed_t<Precision> value) const` (L142): Returns the multiplication of this size by the given fixed point value without trying to avoid overflow.
- **division** `constexpr fixed_size_t division(int value) const` (L150): Returns the division of this size by the given integer value.
- **division** `constexpr fixed_size_t division(fixed_t<Precision> value) const` (L159): Returns the division of this size by the given fixed point value, using half precision to try to avoid overflow.
- **safe_division** `constexpr fixed_size_t safe_division(int value) const` (L167): Returns the division of this value by the given integer value.
- **safe_division** `constexpr fixed_size_t safe_division(fixed_t<Precision> value) const` (L176): Returns the division of this size by the given fixed point value, casting them to int64_t to try to avoid overflow.
- **unsafe_division** `constexpr fixed_size_t unsafe_division(int value) const` (L184): Returns the division of this value by the given integer value.
- **unsafe_division** `constexpr fixed_size_t unsafe_division(fixed_t<Precision> value) const` (L193): Returns the division of this size by the given fixed point value without trying to avoid overflow.
- **operator+=** `constexpr fixed_size_t& operator+=(const fixed_size_t& other)` (L203): Adds the given fixed_size_t to this one.
  `other` fixed_size_t to add.; devuelve: Reference to this.
- **operator-=** `constexpr fixed_size_t& operator-=(const fixed_size_t& other)` (L215): Subtracts the given fixed_size_t to this one.
  `other` fixed_size_t to subtract.; devuelve: Reference to this.
- **operator*=** `constexpr fixed_size_t& operator*=(int value)` (L227): Multiplies both width and height by the given factor.
  `value` Valid integer multiplication factor (>= 0).; devuelve: Reference to this.
- **operator*=** `constexpr fixed_size_t& operator*=(fixed_t<Precision> value)` (L239): Multiplies both width and height by the given factor.
  `value` Valid fixed point multiplication factor (>= 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_size_t& operator/=(int value)` (L251): Divides both width and height by the given divisor.
  `value` Valid integer divisor (> 0).; devuelve: Reference to this.
- **operator/=** `constexpr fixed_size_t& operator/=(fixed_t<Precision> value)` (L263): Divides both width and height by the given divisor.
  `value` Valid fixed point divisor (> 0).; devuelve: Reference to this.
- **operator+** `constexpr friend fixed_size_t operator+(const fixed_size_t& a, const fixed_size_t& b)` (L273): Returns the sum of a and b.
- **operator-** `constexpr friend fixed_size_t operator-(const fixed_size_t& a, const fixed_size_t& b)` (L281): Returns b subtracted from a.
- **operator*** `constexpr friend fixed_size_t operator*(const fixed_size_t& a, int b)` (L289): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_size_t operator*(int a, const fixed_size_t& b)` (L297): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_size_t operator*(const fixed_size_t& a, fixed_t<Precision> b)` (L305): Returns a multiplied by b.
- **operator*** `constexpr friend fixed_size_t operator*(fixed_t<Precision> a, const fixed_size_t& b)` (L313): Returns a multiplied by b.
- **operator/** `constexpr friend fixed_size_t operator/(const fixed_size_t& a, int b)` (L321): Returns a divided by b.
- **operator/** `constexpr friend fixed_size_t operator/(const fixed_size_t& a, fixed_t<Precision> b)` (L329): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const fixed_size_t& a, const fixed_size_t& b) = default` (L337): Default equal operator.

## bn_fixed_size_fwd.h

### `bn::fixed_size_t` (class, bn_fixed_size_fwd.h:26)

Defines the size of a two-dimensional object using fixed point precision.

`template<int Precision> class fixed_size_t`


## bn_math.h

- **abs** `template<typename Type> constexpr Type abs(Type value)` (L39): Returns the absolute value of the given value.
- **abs** `template<typename Type> constexpr Type abs(Type value) requires(is_unsigned_v<Type>)` (L50): Returns the absolute value of the given value.
- **sqrt** `BN_CONST constexpr int sqrt(int value)` (L62): Returns the integer square root of the given value.
- **unsafe_sqrt** `template<int Precision> BN_CONST constexpr fixed_t<Precision> unsafe_sqrt(fixed_t<Precision> value)` (L119): Returns the fixed point square root of the given value without trying to avoid overflow.
  See https://github.com/JoaoBaptMG/gba-modern/blob/master/source/math/fixedmath.hpp
- **safe_degrees_angle** `constexpr fixed safe_degrees_angle(fixed degrees_angle)` (L132): Returns an angle in degrees in the range [0..360) from the given angle in degrees in any range.
- **sin** `constexpr fixed sin(fixed_t<16> angle)` (L152): Calculates the sine value of an angle.
  `angle` Angle (2π = 1).; devuelve: Sine value in the range [-1..1].
- **degrees_sin** `constexpr fixed degrees_sin(fixed degrees_angle)` (L165): Calculates the sine value of an angle in degrees.
  `degrees_angle` Angle in degrees.; devuelve: Sine value in the range [-1..1].
- **lut_sin** `constexpr fixed lut_sin(int lut_angle)` (L179): Calculates the sine value of an angle using a LUT.
  `lut_angle` Angle in the range [0..2048].; devuelve: Sine value in the range [-1..1].
- **degrees_lut_sin** `constexpr fixed degrees_lut_sin(fixed degrees_angle)` (L202): Calculates the sine value of an angle in degrees using a LUT.
  `degrees_angle` Angle in degrees in the range [0..360].; devuelve: Sine value in the range [-1..1].
- **degrees_lut_sin_safe** `constexpr fixed degrees_lut_sin_safe(fixed degrees_angle)` (L229): Calculates the sine value of an angle in degrees in any range using a LUT.
  `degrees_angle` Angle in degrees in any range.; devuelve: Sine value in the range [-1..1].
- **cos** `constexpr fixed cos(fixed_t<16> angle)` (L255): Calculates the cosine value of an angle.
  `angle` Angle (2π = 1).; devuelve: Cosine value in the range [-1..1].
- **degrees_cos** `constexpr fixed degrees_cos(fixed degrees_angle)` (L268): Calculates the cosine value of an angle in degrees.
  `degrees_angle` Angle in degrees.; devuelve: Cosine value in the range [-1..1].
- **lut_cos** `constexpr fixed lut_cos(int lut_angle)` (L282): Calculates the cosine value of an angle using a LUT.
  `lut_angle` Angle in the range [0..2048].; devuelve: Cosine value in the range [-1..1].
- **degrees_lut_cos** `constexpr fixed degrees_lut_cos(fixed degrees_angle)` (L307): Calculates the cosine value of an angle in degrees using a LUT.
  `degrees_angle` Angle in degrees in the range [0..360].; devuelve: Cosine value in the range [-1..1].
- **degrees_lut_cos_safe** `constexpr fixed degrees_lut_cos_safe(fixed degrees_angle)` (L335): Calculates the cosine value of an angle in degrees in any range using a LUT.
  `degrees_angle` Angle in degrees in any range.; devuelve: Cosine value in the range [-1..1].
- **sin_and_cos** `constexpr pair<fixed, fixed> sin_and_cos(fixed_t<16> angle)` (L362): Calculates the sine and the cosine values of an angle.
  `angle` Angle (2π = 1).; devuelve: Sine and cosine values in the range [-1..1].
- **degrees_sin_and_cos** `constexpr pair<fixed, fixed> degrees_sin_and_cos(fixed degrees_angle)` (L377): Calculates the sine and the cosine values of an angle in degrees.
  `degrees_angle` Angle in degrees.; devuelve: Sine and cosine values in the range [-1..1].
- **lut_sin_and_cos** `constexpr pair<fixed, fixed> lut_sin_and_cos(int lut_angle)` (L393): Calculates the sine and the cosine values value of an angle using a LUT.
  `lut_angle` Angle in the range [0..2048].; devuelve: Sine and cosine values in the range [-1..1].
- **degrees_lut_sin_and_cos** `constexpr pair<fixed, fixed> degrees_lut_sin_and_cos(fixed degrees_angle)` (L424): Calculates the sine and the cosine values of an angle in degrees using a LUT.
  `degrees_angle` Angle in degrees in the range [0..360].; devuelve: Sine and cosine values in the range [-1..1].
- **degrees_lut_sin_and_cos_safe** `constexpr pair<fixed, fixed> degrees_lut_sin_and_cos_safe(fixed degrees_angle)` (L457): Calculates the sine and the cosine values of an angle in degrees in any range using a LUT.
  `degrees_angle` Angle in degrees in any range.; devuelve: Sine and cosine values in the range [-1..1].
- **atan2** `BN_CONST fixed_t<16> atan2(int y, int x)` (L490): Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
  `y` Vertical value in the range [-32767..32767].; `x` Horizontal value in the range [-32767..32767].; devuelve: Arc tangent of y/x in the range [-0.5..0.5] (2π = 1).
- **degrees_atan2** `BN_CONST inline fixed degrees_atan2(int y, int x)` (L500): Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
  `y` Vertical value in the range [-32767..32767].; `x` Horizontal value in the range [-32767..32767].; devuelve: Arc tangent of y/x in degrees in the range [-180..180].
- **diamond_angle** `constexpr fixed_t<16> diamond_angle(int y, int x)` (L517): Computes an approximation of the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
  `y` Vertical value in the range [-131072..131071].; `x` Horizontal value in the range [-131072..131071].; devuelve: Arc tangent of y/x in the range [-0.5..0.5] (2π = 1).
  https://www.freesteel.co.uk/wpblog/2009/06/05/encoding-2d-angles-without-trigonometry/
- **degrees_diamond_angle** `constexpr fixed degrees_diamond_angle(int y, int x)` (L567): Computes an approximation of the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
  `y` Vertical value in the range [-131072..131071].; `x` Horizontal value in the range [-131072..131071].; devuelve: Arc tangent of y/x in degrees in the range [-180..180].
  https://www.freesteel.co.uk/wpblog/2009/06/05/encoding-2d-angles-without-trigonometry/
- **lut_reciprocal** `constexpr fixed_t<20> lut_reciprocal(int lut_value)` (L579): Calculates the reciprocal of a value using a LUT.
  `lut_value` Value in the range [1..1024].; devuelve: Reciprocal of the given value (1 / value).

## bn_point.h

### `bn::point` (class, bn_point.h:27)

Defines a two-dimensional point using integer precision.

`class point`

- **point** `constexpr point() = default` (L34): Default constructor.
- **point** `constexpr point(int x, int y)` (L41): Constructor.
  `x` Horizontal coordinate.; `y` Vertical coordinate.
- **x** `constexpr int x() const` (L50): Returns the horizontal coordinate.
- **set_x** `constexpr void set_x(int x)` (L58): Sets the horizontal coordinate.
- **y** `constexpr int y() const` (L66): Returns the vertical coordinate.
- **set_y** `constexpr void set_y(int y)` (L74): Sets the vertical coordinate.
- **operator-** `constexpr point operator-() const` (L82): Returns a point that is formed by changing the sign of both coordinates.
- **operator+=** `constexpr point& operator+=(const point& other)` (L92): Adds the given point to this one.
  `other` point to add.; devuelve: Reference to this.
- **operator-=** `constexpr point& operator-=(const point& other)` (L104): Subtracts the given point to this one.
  `other` point to subtract.; devuelve: Reference to this.
- **operator*=** `constexpr point& operator*=(int value)` (L116): Multiplies both coordinates by the given factor.
  `value` Integer multiplication factor.; devuelve: Reference to this.
- **operator/=** `constexpr point& operator/=(int value)` (L128): Divides both coordinates by the given divisor.
  `value` Valid integer divisor (!= 0).; devuelve: Reference to this.
- **operator+** `constexpr friend point operator+(const point& a, const point& b)` (L140): Returns the sum of a and b.
- **operator-** `constexpr friend point operator-(const point& a, const point& b)` (L148): Returns b subtracted from a.
- **operator*** `constexpr friend point operator*(const point& a, int b)` (L156): Returns a multiplied by b.
- **operator/** `constexpr friend point operator/(const point& a, int b)` (L164): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const point& a, const point& b) = default` (L174): Default equal operator.

## bn_power_of_two.h

- **power_of_two** `template<typename Type> constexpr bool power_of_two(Type value)` (L26): Indicates if the given value is a power of two or not.

## bn_reciprocal_lut.h

- **calculate_reciprocal_lut_value** `template<int Precision = 20> constexpr fixed_t<Precision> calculate_reciprocal_lut_value(int lut_value)` (L31): Calculates the value to store in a reciprocal LUT for the given value.
  `Precision` Number of bits used for the fractional part of the reciprocal value.; `lut_value` Value in the range (>= 1).; devuelve: Reciprocal of the given value (1 / value).
- **reciprocal_lut_size** `constexpr int reciprocal_lut_size = 1025` (L44): Reciprocal 20bit LUT size.
- **reciprocal_lut** `extern const array<fixed_t<20>, reciprocal_lut_size>& reciprocal_lut` (L51): Reciprocal 20bit LUT.
- **reciprocal_16_lut_size** `constexpr int reciprocal_16_lut_size = 1025` (L58): Reciprocal 16bit LUT size.
- **alignas** `alignas(int) extern const array<uint16_t, reciprocal_16_lut_size>& reciprocal_16_lut` (L65): Reciprocal 16bit LUT.

## bn_rect.h

### `bn::rect` (class, bn_rect.h:31)

Defines a two-dimensional rectangle using integer precision and a center point as position.

`class rect`

Since it takes a center point as position instead of a top-left point, it has problems with odd dimensions.

If you need to handle odd dimensions, use bn::top_left_rect instead.

- **rect** `constexpr rect() = default` (L38): Default constructor.
- **rect** `constexpr rect(const point& position, const size& dimensions)` (L45): Constructor.
  `position` Position of the center point of the rectangle.; `dimensions` Size of the rectangle.
- **rect** `constexpr rect(int x, int y, int width, int height)` (L58): Constructor.
  `x` Horizontal position of the center point of the rectangle.; `y` Vertical position of the center point of the rectangle.; `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **position** `constexpr const point& position() const` (L67): Returns the position of the center point of the rectangle.
- **set_position** `constexpr void set_position(const point& position)` (L75): Sets the position of the center point of the rectangle.
- **set_position** `constexpr void set_position(int x, int y)` (L85): Sets the position of the center point of the rectangle.
  `x` Horizontal position of the center point of the rectangle.; `y` Vertical position of the center point of the rectangle.
- **dimensions** `constexpr const size& dimensions() const` (L93): Returns the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(const size& dimensions)` (L101): Sets the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(int width, int height)` (L111): Sets the size of the rectangle.
  `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **x** `constexpr int x() const` (L119): Returns the horizontal position of the center point of the rectangle.
- **set_x** `constexpr void set_x(int x)` (L127): Sets the horizontal position of the center point of the rectangle.
- **y** `constexpr int y() const` (L135): Returns the vertical position of the center point of the rectangle.
- **set_y** `constexpr void set_y(int y)` (L143): Sets the vertical position of the center point of the rectangle.
- **width** `constexpr int width() const` (L151): Returns the width of the rectangle.
- **set_width** `constexpr void set_width(int width)` (L160): Sets the width of the rectangle.
  `width` Valid width (>= 0).
- **height** `constexpr int height() const` (L168): Returns the height of the rectangle.
- **set_height** `constexpr void set_height(int height)` (L177): Sets the height of the rectangle.
  `height` Valid height (>= 0).
- **top** `constexpr int top() const` (L185): Returns the position of the top boundary of the rectangle.
- **left** `constexpr int left() const` (L193): Returns the position of the left boundary of the rectangle.
- **bottom** `constexpr int bottom() const` (L201): Returns the position of the bottom boundary of the rectangle.
- **right** `constexpr int right() const` (L209): Returns the position of the right boundary of the rectangle.
- **top_left** `constexpr point top_left() const` (L217): Returns the position of the top-left corner of the rectangle.
- **top_right** `constexpr point top_right() const` (L225): Returns the position of the top-right corner of the rectangle.
- **bottom_left** `constexpr point bottom_left() const` (L233): Returns the position of the bottom-left corner of the rectangle.
- **bottom_right** `constexpr point bottom_right() const` (L241): Returns the position of the bottom-right corner of the rectangle.
- **center_x** `constexpr int center_x() const` (L249): Returns the horizontal position of the center point of the rectangle.
- **center_y** `constexpr int center_y() const` (L257): Returns the vertical position of the center point of the rectangle.
- **center** `constexpr point center() const` (L265): Returns the position of the center point of the rectangle.
- **contains** `constexpr bool contains(const point& point) const` (L275): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const point& point) const` (L293): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `true`.
- **contains** `constexpr bool contains(const rect& other) const` (L311): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const rect& other) const` (L322): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `true`.
- **intersects** `constexpr bool intersects(const rect& other) const` (L334): Indicates if this rectangle intersects with the given one or not.
  Two rectangles intersect if there is at least one point that is within both rectangles,
  excluding their edges.
- **touches** `constexpr bool touches(const rect& other) const` (L355): Indicates if this rectangle touches the given one or not.
  Two rectangles touch each other if there is at least one point that is within both rectangles,
  including their edges.
- **operator*=** `constexpr rect& operator*=(int value)` (L375): Multiplies both width and height of the rectangle by the given factor.
  `value` Integer multiplication factor.; devuelve: Reference to this.
- **operator/=** `constexpr rect& operator/=(int value)` (L386): Divides both width and height of the rectangle by the given divisor.
  `value` Valid integer divisor (> 0).; devuelve: Reference to this.
- **operator*** `constexpr friend rect operator*(const rect& a, int b)` (L395): Returns a multiplied by b.
- **operator/** `constexpr friend rect operator/(const rect& a, int b)` (L403): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const rect& a, const rect& b) = default` (L411): Default equal operator.

## bn_rule_of_three_approximation.h

### `bn::rule_of_three_approximation` (class, bn_rule_of_three_approximation.h:31)

Approximates a rule of three cross-multiplication (x = (value * multiplier) / divisor).

`class rule_of_three_approximation`

rule_of_three_approximation objects should be created at compile time to improve performance.

See https://en.wikipedia.org/wiki/Cross-multiplication#Rule_of_Three

- **rule_of_three_approximation** `constexpr rule_of_three_approximation(int divisor, int multiplier)` (L40): Constructor.
  `divisor` Valid divisor (> 0).; `multiplier` Valid multiplier (> 0).
- **calculate** `constexpr unsigned calculate(unsigned value) const` (L50): Returns an approximation of (value * multiplier) / divisor, casting the value to uint64_t to try to avoid integer overflow.
- **unsafe_calculate** `constexpr unsigned unsafe_calculate(unsigned value) const` (L59): Returns an approximation of (value * multiplier) / divisor, without trying to avoid integer overflow.
- **calculate** `constexpr int calculate(int value) const` (L68): Returns an approximation of (value * multiplier) / divisor, casting the value to int64_t to try to avoid integer overflow.
- **unsafe_calculate** `constexpr int unsafe_calculate(int value) const` (L77): Returns an approximation of (value * multiplier) / divisor, without trying to avoid integer overflow.
- **calculate** `template<int Precision> constexpr fixed_t<Precision> calculate(fixed_t<Precision> value) const` (L86): Returns an approximation of (value * multiplier) / divisor, casting the value to int64_t to try to avoid integer overflow.
- **unsafe_calculate** `template<int Precision> constexpr fixed_t<Precision> unsafe_calculate(fixed_t<Precision> value) const` (L96): Returns an approximation of (value * multiplier) / divisor, without trying to avoid integer overflow.

## bn_sin_lut.h

- **calculate_sin_lut_value** `constexpr int calculate_sin_lut_value(int lut_angle)` (L50): Calculates the value to store in sin_lut for the given angle.
  `lut_angle` sin_lut angle (360 degrees = 65536).; devuelve: Sine value in the range [-4096, 4096].
- **sin_lut_size** `constexpr int sin_lut_size = 2049` (L100): Size of the sine LUT of 16bit values in 4.12 format.
- **sin_lut** `extern const array<int16_t, sin_lut_size>& sin_lut` (L107): Sine LUT of 16bit values in 4.12 format.

## bn_size.h

### `bn::size` (class, bn_size.h:27)

Defines the size of a two-dimensional object using integer precision.

`class size`

- **size** `constexpr size() = default` (L34): Default constructor.
- **size** `constexpr size(int width, int height)` (L41): Constructor.
  `width` Valid width (>= 0).; `height` Valid height (>= 0).
- **width** `constexpr int width() const` (L52): Returns the width.
- **set_width** `constexpr void set_width(int width)` (L61): Sets the width.
  `width` Valid width (>= 0).
- **height** `constexpr int height() const` (L71): Returns the height.
- **set_height** `constexpr void set_height(int height)` (L80): Sets the height.
  `height` Valid height (>= 0).
- **operator+=** `constexpr size& operator+=(const size& other)` (L92): Adds the given size to this one.
  `other` size to add.; devuelve: Reference to this.
- **operator-=** `constexpr size& operator-=(const size& other)` (L104): Subtracts the given size to this one.
  `other` size to subtract.; devuelve: Reference to this.
- **operator*=** `constexpr size& operator*=(int value)` (L120): Multiplies both width and height by the given factor.
  `value` Valid integer multiplication factor (>= 0).; devuelve: Reference to this.
- **operator/=** `constexpr size& operator/=(int value)` (L134): Divides both width and height by the given divisor.
  `value` Valid integer divisor (> 0).; devuelve: Reference to this.
- **operator+** `constexpr friend size operator+(const size& a, const size& b)` (L146): Returns the sum of a and b.
- **operator-** `constexpr friend size operator-(const size& a, const size& b)` (L154): Returns b subtracted from a.
- **operator*** `constexpr friend size operator*(const size& a, int b)` (L162): Returns a multiplied by b.
- **operator/** `constexpr friend size operator/(const size& a, int b)` (L172): Returns a divided by b.
- **operator==** `constexpr friend bool operator==(const size& a, const size& b) = default` (L182): Default equal operator.

## bn_top_left_fixed_rect.h

- **top_left_fixed_rect_t** `constexpr top_left_fixed_rect_t() = default` (L32): Default constructor.
- **top_left_fixed_rect_t** `constexpr top_left_fixed_rect_t( const fixed_point_t<Precision>& position, const fixed_size_t<Precision>& dimensions)` (L39): Constructor.
  `position` Position of the top-left point of the rectangle.; `dimensions` Size of the rectangle.
- **top_left_fixed_rect_t** `constexpr top_left_fixed_rect_t( fixed_t<Precision> x, fixed_t<Precision> y, fixed_t<Precision> width, fixed_t<Precision> height)` (L53): Constructor.
  `x` Horizontal position of the top-left point of the rectangle.; `y` Vertical position of the top-left point of the rectangle.; `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **top_left_fixed_rect_t** `constexpr top_left_fixed_rect_t(const top_left_rect& rect)` (L64): Constructor.
  `rect` Integer rectangle.
- **position** `constexpr const fixed_point_t<Precision>& position() const` (L73): Returns the position of the top-left point of the rectangle.
- **set_position** `constexpr void set_position(const fixed_point_t<Precision>& position)` (L81): Sets the position of the top-left point of the rectangle.
- **set_position** `constexpr void set_position(fixed_t<Precision> x, fixed_t<Precision> y)` (L91): Sets the position of the top-left point of the rectangle.
  `x` Horizontal position of the top-left point of the rectangle.; `y` Vertical position of the top-left point of the rectangle.
- **dimensions** `constexpr const fixed_size_t<Precision>& dimensions() const` (L99): Returns the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(const fixed_size_t<Precision>& dimensions)` (L107): Sets the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(fixed_t<Precision> width, fixed_t<Precision> height)` (L117): Sets the size of the rectangle.
  `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **x** `constexpr fixed_t<Precision> x() const` (L125): Returns the horizontal position of the top-left point of the rectangle.
- **set_x** `constexpr void set_x(fixed_t<Precision> x)` (L133): Sets the horizontal position of the top-left point of the rectangle.
- **y** `constexpr fixed_t<Precision> y() const` (L141): Returns the vertical position of the top-left point of the rectangle.
- **set_y** `constexpr void set_y(fixed_t<Precision> y)` (L149): Sets the vertical position of the top-left point of the rectangle.
- **width** `constexpr fixed_t<Precision> width() const` (L157): Returns the width of the rectangle.
- **set_width** `constexpr void set_width(fixed_t<Precision> width)` (L166): Sets the width of the rectangle.
  `width` Valid width (>= 0).
- **height** `constexpr fixed_t<Precision> height() const` (L174): Returns the height of the rectangle.
- **set_height** `constexpr void set_height(fixed_t<Precision> height)` (L183): Sets the height of the rectangle.
  `height` Valid height (>= 0).
- **top** `constexpr fixed_t<Precision> top() const` (L191): Returns the position of the top boundary of the rectangle.
- **left** `constexpr fixed_t<Precision> left() const` (L199): Returns the position of the left boundary of the rectangle.
- **bottom** `constexpr fixed_t<Precision> bottom() const` (L207): Returns the position of the bottom boundary of the rectangle.
- **right** `constexpr fixed_t<Precision> right() const` (L215): Returns the position of the right boundary of the rectangle.
- **top_left** `constexpr fixed_point_t<Precision> top_left() const` (L223): Returns the position of the top-left corner of the rectangle.
- **top_right** `constexpr fixed_point_t<Precision> top_right() const` (L231): Returns the position of the top-right corner of the rectangle.
- **bottom_left** `constexpr fixed_point_t<Precision> bottom_left() const` (L239): Returns the position of the bottom-left corner of the rectangle.
- **bottom_right** `constexpr fixed_point_t<Precision> bottom_right() const` (L247): Returns the position of the bottom-right corner of the rectangle.
- **center_x** `constexpr fixed_t<Precision> center_x() const` (L255): Returns the horizontal position of the center point of the rectangle.
- **center_y** `constexpr fixed_t<Precision> center_y() const` (L263): Returns the vertical position of the center point of the rectangle.
- **center** `constexpr fixed_point_t<Precision> center() const` (L271): Returns the position of the center point of the rectangle.
- **contains** `constexpr bool contains(const fixed_point_t<Precision>& point) const` (L281): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const fixed_point_t<Precision>& point) const` (L299): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `true`.
- **contains** `constexpr bool contains(const top_left_fixed_rect_t& other) const` (L317): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const top_left_fixed_rect_t& other) const` (L328): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `true`.
- **intersects** `constexpr bool intersects(const top_left_fixed_rect_t& other) const` (L340): Indicates if this rectangle intersects with the given one or not.
  Two rectangles intersect if there is at least one point that is within both rectangles,
  excluding their edges.
- **touches** `constexpr bool touches(const top_left_fixed_rect_t& other) const` (L361): Indicates if this rectangle touches the given one or not.
  Two rectangles touch each other if there is at least one point that is within both rectangles,
  including their edges.
- **operator==** `constexpr friend bool operator==( const top_left_fixed_rect_t& a, const top_left_fixed_rect_t& b) = default` (L379): Default equal operator.

## bn_top_left_fixed_rect_fwd.h

### `bn::top_left_fixed_rect_t` (class, bn_top_left_fixed_rect_fwd.h:26)

Defines a two-dimensional rectangle using fixed point precision and a top-left point as position.

`template<int Precision> class top_left_fixed_rect_t`


## bn_top_left_rect.h

### `bn::top_left_rect` (class, bn_top_left_rect.h:27)

Defines a two-dimensional rectangle using integer precision and a top-left point as position.

`class top_left_rect`

- **top_left_rect** `constexpr top_left_rect() = default` (L34): Default constructor.
- **top_left_rect** `constexpr top_left_rect(const point& position, const size& dimensions)` (L41): Constructor.
  `position` Position of the top-left point of the rectangle.; `dimensions` Size of the rectangle.
- **top_left_rect** `constexpr top_left_rect(int x, int y, int width, int height)` (L54): Constructor.
  `x` Horizontal position of the top-left point of the rectangle.; `y` Vertical position of the top-left point of the rectangle.; `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **position** `constexpr const point& position() const` (L63): Returns the position of the top-left point of the rectangle.
- **set_position** `constexpr void set_position(const point& position)` (L71): Sets the position of the top-left point of the rectangle.
- **set_position** `constexpr void set_position(int x, int y)` (L81): Sets the position of the top-left point of the rectangle.
  `x` Horizontal position of the top-left point of the rectangle.; `y` Vertical position of the top-left point of the rectangle.
- **dimensions** `constexpr const size& dimensions() const` (L89): Returns the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(const size& dimensions)` (L97): Sets the size of the rectangle.
- **set_dimensions** `constexpr void set_dimensions(int width, int height)` (L107): Sets the size of the rectangle.
  `width` Valid width of the rectangle (>= 0).; `height` Valid height of the rectangle (>= 0).
- **x** `constexpr int x() const` (L115): Returns the horizontal position of the top-left point of the rectangle.
- **set_x** `constexpr void set_x(int x)` (L123): Sets the horizontal position of the top-left point of the rectangle.
- **y** `constexpr int y() const` (L131): Returns the vertical position of the top-left point of the rectangle.
- **set_y** `constexpr void set_y(int y)` (L139): Sets the vertical position of the top-left point of the rectangle.
- **width** `constexpr int width() const` (L147): Returns the width of the rectangle.
- **set_width** `constexpr void set_width(int width)` (L156): Sets the width of the rectangle.
  `width` Valid width (>= 0).
- **height** `constexpr int height() const` (L164): Returns the height of the rectangle.
- **set_height** `constexpr void set_height(int height)` (L173): Sets the height of the rectangle.
  `height` Valid height (>= 0).
- **top** `constexpr int top() const` (L181): Returns the position of the top boundary of the rectangle.
- **left** `constexpr int left() const` (L189): Returns the position of the left boundary of the rectangle.
- **bottom** `constexpr int bottom() const` (L197): Returns the position of the bottom boundary of the rectangle.
- **right** `constexpr int right() const` (L205): Returns the position of the right boundary of the rectangle.
- **top_left** `constexpr point top_left() const` (L213): Returns the position of the top-left corner of the rectangle.
- **top_right** `constexpr point top_right() const` (L221): Returns the position of the top-right corner of the rectangle.
- **bottom_left** `constexpr point bottom_left() const` (L229): Returns the position of the bottom-left corner of the rectangle.
- **bottom_right** `constexpr point bottom_right() const` (L237): Returns the position of the bottom-right corner of the rectangle.
- **center_x** `constexpr int center_x() const` (L245): Returns the horizontal position of the center point of the rectangle.
- **center_y** `constexpr int center_y() const` (L253): Returns the vertical position of the center point of the rectangle.
- **center** `constexpr point center() const` (L261): Returns the position of the center point of the rectangle.
- **contains** `constexpr bool contains(const point& point) const` (L271): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const point& point) const` (L289): Indicates if the given point is inside the rectangle or not.
  If the point is on the edge of the rectangle, it returns `true`.
- **contains** `constexpr bool contains(const top_left_rect& other) const` (L307): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `false`.
- **edge_contains** `constexpr bool edge_contains(const top_left_rect& other) const` (L318): Indicates if the given rectangle is inside this one or not.
  If the given rectangle is on the edge of this one, it returns `true`.
- **intersects** `constexpr bool intersects(const top_left_rect& other) const` (L330): Indicates if this rectangle intersects with the given one or not.
  Two rectangles intersect if there is at least one point that is within both rectangles,
  excluding their edges.
- **touches** `constexpr bool touches(const top_left_rect& other) const` (L351): Indicates if this rectangle touches the given one or not.
  Two rectangles touch each other if there is at least one point that is within both rectangles,
  including their edges.
- **operator==** `constexpr friend bool operator==(const top_left_rect& a, const top_left_rect& b) = default` (L369): Default equal operator.
