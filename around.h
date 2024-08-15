#include <cstddef>

namespace ext {

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or (std::size_t) -1 
    //
    std::size_t around_suggest (double value, std::size_t precision);

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or (std::size_t) -1 
    //
    std::size_t around_suggest (const char * value, std::size_t precision);

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounds 'value' to 'around_suggest'ed precision and returns it
    //  - prefer string output versions of the call
    //
    double around (double value, std::size_t precision = 4,
                   std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1);

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - 'value' must be in "-123.456" format, no extra spaces, simple minus, dot for decimal separator
    //  - rounded result is stored as string into 'buffer' (NOT NUL-terminated)
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //  - returns number of characters in the final string
    //
    std::size_t around (const char * value, char * buffer, std::size_t length, std::size_t precision = 4,
                        std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1);

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounded result is stored as string into 'buffer' (NOT NUL-terminated)
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //  - returns number of characters in the final string
    //
    std::size_t around (double value, char * buffer, std::size_t length, std::size_t precision = 4,
                        std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1);

}
