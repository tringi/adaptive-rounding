#include <cstdio>
#include <cmath>

namespace ext {

    // around_detail
    //  - implementation details for 'around' function
    //
    namespace around_detail {

        // needle
        //  - generates static NUL-terminated N-character long string of C characters
        //  - example: needle<3,'a'>::string is static NUL-terminated string of three 'a' characters, i.e: "aaa"
        //
        template <std::size_t N, char C>
        struct needle {
            constexpr needle () {
                for (std::size_t i = 0; i != N; ++i) {
                    string [i] = C;
                }
                string [N] = '\0';
            }
            char string [N + 1];
        };
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating point number, like a human would do
    //
    template <std::size_t precision = 3>
    double around (double value) {

        char string [64];
        if (std::snprintf (string, sizeof string, "%.*f", (unsigned int) sizeof string, value)) {

            if (auto p = std::strchr (string, '.')) {
                ++p;
                auto p9 = std::strstr (p, around_detail::needle <precision, '9'> ().string);
                auto p0 = std::strstr (p, around_detail::needle <precision, '0'> ().string);

                std::ptrdiff_t n = 0;

                if (p0 && p9) {
                    n = std::min (p0 - p, p9 - p);
                } else
                if (p9) {
                    n = p9 - p;
                } else
                if (p0) {
                    n = p0 - p;
                }

                if (n) {
                    auto m = std::pow (10, n);
                    value = std::round (value * m) / m;
                }
            }
        }
        return value;
    }
}
