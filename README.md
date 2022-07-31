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

## Description

The function searches for N consecutive '0' or '9' characters in the decimal part of
a string representation of a double-precision floating-point value.

The precision, N, is 4 by default, and it's the first and only template parameter.

## Possible use cases

* adjusting after accumulating small floating point arithmetics errors
* improving legibility/scannability of numeric values on screen

## License

None. Public domain. WTF license.