# `around` — adaptive rounding operation
*Attempts to perform nice rounding of a floating point number, like a human would do.*

## Usage: [around.h](around.h)

    #include "around.h"
    
    auto r0 = around (1.000456);      // 1.0
    auto r1 = around (27.2999999);    // 27.3
    auto r2 = around (27.2349999);    // 27.235
    auto r3 = around<3> (27.1993999); // 27.1994
    auto r4 = around<2> (27.1993999); // 27.2

## Description

The function searches for N consecutive '0' or '9' characters in the decimal part of
a string representation of a double-precision floating-point value.

The precision, N, is 3 by default, and it's the first and only template parameter.

## Possible use cases

* adjusting after accumulating small floating point arithmetics errors
* improving legibility/scannability of numeric values on screen

## License

None. Public domain. WTF license.