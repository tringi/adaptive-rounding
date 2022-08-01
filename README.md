# `around` — adaptive rounding operation
*Attempts to perform nice rounding of a floating point number, like a human would do.*

## Usage: [around.h](around.h)

    #include "around.h"
    
    auto r0 = ext::around (1.0000456);      // 1.0
    auto r1 = ext::around (27.2999999);     // 27.3
    auto r2 = ext::around (27.23499999);    // 27.235
    auto r3 = ext::around<3> (27.19939999); // 27.1994
    auto r4 = ext::around<2> (27.19939999); // 27.2

    auto n1 = ext::around_suggest (1.0000456);      // 0
    auto n2 = ext::around_suggest (27.2999999);     // 1
    auto n2 = ext::around_suggest (27.23499999);    // 3
    auto n3 = ext::around_suggest<3> (27.19939999); // 4
    auto n4 = ext::around_suggest<2> (27.19939999); // 1

    char buffer [64];

    ext::around (1.0000456, buffer, sizeof buffer);      // "1"
    ext::around (27.2999999, buffer, sizeof buffer);     // "27.3"
    ext::around (27.23499999, buffer, sizeof buffer);    // "27.235"
    ext::around<3> (27.19939999, buffer, sizeof buffer); // "27.1994"
    ext::around<2> (27.19939999, buffer, sizeof buffer); // "27.2"

    ext::around ("1.0000456", buffer, sizeof buffer);      // "1"
    ext::around ("27.2999999", buffer, sizeof buffer);     // "27.3"
    ext::around ("27.23499999", buffer, sizeof buffer);    // "27.235"
    ext::around<3> ("27.19939999", buffer, sizeof buffer); // "27.1994"
    ext::around<2> ("27.19939999", buffer, sizeof buffer); // "27.2"

## Description

The function searches for N consecutive '0' or '9' characters in the decimal part of
a string representation of a double-precision floating-point value.

The precision, N, is 4 by default, and it's the first and only template parameter.

## Possible use cases

* adjusting after accumulating small floating point arithmetics errors
* improving legibility/scannability of numeric values on screen

## API details

### Common parameters

* `precision` - template argument, number of consecutive digits searched, see `N` above.
* `minimum`/`maximum` - optionally limits the number of actual decimals, should the algorithm decide outside of this range

### String input

* string `value` must be in `-123.456` format; no extra spaces, simple minus, dot `.` for decimal separator

### ext::around (string -> string buffer)

Rounded `value` is stored as string into the `buffer` of maximum `length` bytes.

**Note:** Provide at least 2 aditional bytes of space in `buffer` in addition to maximum expected length.

    template <std::size_t precision = 4>
    void around (const char * value,
                 char * buffer, std::size_t length,
                 std::size_t minimum = 0,
                 std::size_t maximum = ~0);

### ext::around (double -> string buffer)

Rounded string representation of `value` is stored as string into the `buffer` of maximum `length` bytes.

**Note:** Provide at least 2 aditional bytes of space in `buffer` in addition to maximum expected length.

    template <std::size_t precision = 4>
    void around (double value,
                 char * buffer, std::size_t length,
                 std::size_t minimum = 0, std::size_t maximum = ~0);

### ext::around (double -> double)

Rounds `value` to suggested precision and returns new number.

**Note:** By nature of floating points, new imprecisions can be introducted.

    template <std::size_t precision = 4>
    double around (double value,
                   std::size_t minimum = 0,
                   std::size_t maximum = ~0);

### ext::around_suggest

Suggests rounding precision for the number/string. Returns suggested precision or `~0`.

    template <std::size_t precision = 4>
    std::size_t around_suggest (double value);

    template <std::size_t precision = 4>
    std::size_t around_suggest (const char * value);

## License

None. Public domain. WTF license.
