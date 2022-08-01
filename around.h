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

        // suggestion
        //  - 
        //
        struct suggestion {
            std::size_t precision = (std::size_t) -1;
            char *      decimals = nullptr;
        };

        // suggest
        //  - internal helper, suggests rounding precision for decimal number passed as string
        //  - this is the nutshell of the algorithm
        //
        template <std::size_t precision = 4>
        suggestion suggest (char * string) {
            suggestion result;

            if (auto p = std::strchr (string, '.')) {
                ++p;
                result.decimals = p;

                auto p9 = std::strstr (p, around_detail::needle <precision, '9'> ().string);
                auto p0 = std::strstr (p, around_detail::needle <precision, '0'> ().string);

                if (p0 && p9) {
                    result.precision = std::min (p0 - p, p9 - p);
                } else
                if (p9) {
                    result.precision = p9 - p;
                } else
                if (p0) {
                    result.precision = p0 - p;
                }
            } else {
                result.precision = 0;
            }
            return result;
        }

        // suggest
        //  - internal helper, suggests rounding precision for the floating-point value
        //  - converts it to string and uses the decision above
        //
        template <std::size_t precision = 4>
        suggestion suggest (double value, char * buffer, std::size_t length) {
            if (std::snprintf (buffer, length, "%.*f", (unsigned int) length, value)) {
                return suggest <precision> (buffer);
            } else
                return suggestion {};
        }

        // round
        //  - internal helper, performs the actual rounding on the string
        //  - 'suggested.decimals' MUST point into valid string representation
        //    of the floating-point value PREFIXED with space character
        //
        inline void round (suggestion suggested,
                           std::size_t minimum, std::size_t maximum) {
            auto n = suggested.precision;
            auto decimals = suggested.decimals;

            if (n != ~0) {
                if (n < minimum) n = minimum;
                if (n > maximum) n = maximum;

                switch (decimals [n]) {
                    case '0':
                        if (n == 0) {
                            decimals [-1] = '\0';
                        } else {
                            decimals [n] = '\0';
                        }
                        break;

                    case '9':
                        do {
                            --n;
                        } while (decimals [n] == '9');

                        if (decimals [n] == '.') {
                            decimals [n] = '\0';

                            --n;
                            while (decimals [n] == '9') {
                                decimals [n] = '0';
                                --n;
                            }

                            switch (decimals [n]) {
                                case '-':
                                    decimals [n - 1] = '-';
                                    [[ fallthrough ]];
                                case ' ':
                                    decimals [n] = '1';
                                    break;
                                default:
                                    ++decimals [n];
                            }
                        } else {
                            decimals [n + 0]++;
                            decimals [n + 1] = '\0';
                        }
                        break;
                }
            }
        }
    }

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or ~0 
    //
    template <std::size_t precision = 4>
    std::size_t around_suggest (double value) {
        char string [64];
        return around_detail::suggest <precision> (value, string, sizeof string).precision;
    }

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or ~0 
    //
    template <std::size_t precision = 4>
    std::size_t around_suggest (const char * value) {
        return around_detail::suggest <precision> (value).precision;
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounds 'value' to 'around_suggest'ed precision and returns it
    //  - prefer string output versions of the call
    //
    template <std::size_t precision = 4>
    double around (double value,
                   std::size_t minimum = 0, std::size_t maximum = ~0) {

        auto n = around_suggest <precision> (value);
        if (n < minimum) n = minimum;
        if (n > maximum) n = maximum;

        auto m = std::pow (10, n);
        return std::round (value * m) / m;
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - 'value' must be in "-123.456" format, no extra spaces, simple minus, dot for decimal separator
    //  - rounded result is stored as string into 'buffer'
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //
    template <std::size_t precision = 4>
    void around (const char * value, char * buffer, std::size_t length,
                 std::size_t minimum = 0, std::size_t maximum = ~0) {

        using namespace around_detail;

        if (length > 1) {
#ifdef _CRT_SECURE_NO_WARNINGS
            std::strncpy (&buffer [1], value, length - 2);
#else
            strncpy_s (&buffer [1], length - 1, value, length - 2);
#endif
            buffer [0] = ' ';
            buffer [length - 1] = '\0';

            round (suggest <precision> (buffer + 1), minimum, maximum);

            if (buffer [0] == ' ') {
                std::memmove (&buffer [0], &buffer [1], length - 1);
            }
        }
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounded result is stored as string into 'buffer'
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //
    template <std::size_t precision = 4>
    void around (double value, char * buffer, std::size_t length,
                 std::size_t minimum = 0, std::size_t maximum = ~0) {
        
        using namespace around_detail;

        if (length > 1) {
            buffer [0] = ' ';

            round (suggest <precision> (value, buffer + 1, length - 1), minimum, maximum);

            if (buffer [0] == ' ') {
                std::memmove (&buffer [0], &buffer [1], length - 1);
            }
        }
    }

}
