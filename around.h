#include <cstdio>
#include <cmath>

namespace ext {

    // around_detail
    //  - implementation details for 'around' function
    //
    namespace around_detail {

        // suggestion
        //  - internal helper, result of 'suggest' function
        //  - 'precision' is the determined/suggested precision for rounding
        //  - 'decimals' points to first decimal digit in the input string
        //
        struct suggestion {
            std::size_t precision = (std::size_t) -1;
            char *      decimals = nullptr;
        };

        // suggest
        //  - internal helper, suggests rounding precision for decimal number passed as string
        //  - this is the nutshell of the algorithm
        //
        inline suggestion suggest (char * string, std::size_t precision) {
            suggestion result;

            if (auto p = std::strchr (string, '.')) {
                ++p;
                result.decimals = p;

                char needle [2][16];
                if (precision > 15) {
                    precision = 15;
                }
                for (std::size_t i = 0; i != precision; ++i) {
                    needle [0][i] = '9';
                    needle [1][i] = '0';
                }
                needle [0][precision] = '\0';
                needle [1][precision] = '\0';

                auto p9 = std::strstr (p, needle [0]);
                auto p0 = std::strstr (p, needle [1]);

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
                result.decimals = std::strchr (string, '\0');
                result.precision = 0;
            }
            return result;
        }

        // suggest
        //  - internal helper, suggests rounding precision for the floating-point value
        //  - converts it to string and uses the decision above
        //
        inline suggestion suggest (double value, char * buffer, std::size_t length, std::size_t precision) {
            if (std::snprintf (buffer, length, "%.*f", (unsigned int) length, value)) {
                return suggest (buffer, precision);
            } else
                return suggestion {};
        }

        // round
        //  - internal helper, performs the actual rounding on the string
        //  - 'suggested.decimals' MUST point into valid string representation
        //    of the floating-point value PREFIXED with space character
        //
        inline char * round (suggestion suggested,
                             std::size_t minimum, std::size_t maximum) {
            auto n = suggested.precision;
            auto decimals = suggested.decimals;

            if (n > maximum) {
                n = maximum;
            }

            if (n != (std::size_t) -1) {
                switch (decimals [n]) {
                    case '0':
                        if (n < minimum) {
                            n = minimum;
                        }
                        if (n == 0) {
                            return &decimals [-1];
                        } else {
                            return &decimals [n];
                        }

                    case '9':
                        do {
                            --n;
                        } while (decimals [n] == '9');

                        if (decimals [n] == '.') {
                            char * endptr = nullptr;
                            if (minimum) {
                                std::size_t i = 1;
                                while (i != minimum + 1) {
                                    decimals [n + i++] = '0';
                                }
                                endptr = &decimals [n + i];
                            } else {
                                endptr = &decimals [n];
                            }

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
                            return endptr;
                        } else {
                            decimals [n++]++;
                            while (n < minimum) {
                                decimals [n++] = '0';
                            }
                            return &decimals [n];
                        }
                }
            }
            return &decimals [n];
        }
    }

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or (std::size_t) -1 
    //
    inline std::size_t around_suggest (double value, std::size_t precision) {
        char string [64];
        return around_detail::suggest (value, string, sizeof string, precision).precision;
    }

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or (std::size_t) -1 
    //
    inline std::size_t around_suggest (const char * value, std::size_t precision) {
        return around_detail::suggest ((char *) value, precision).precision;
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounds 'value' to 'around_suggest'ed precision and returns it
    //  - prefer string output versions of the call
    //
    inline double around (double value, std::size_t precision = 4,
                          std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1) {

        auto n = around_suggest (value, precision);
        if (n < minimum) n = minimum;
        if (n > maximum) n = maximum;

        auto m = std::pow (10, n);
        return std::round (value * m) / m;
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - 'value' must be in "-123.456" format, no extra spaces, simple minus, dot for decimal separator
    //  - rounded result is stored as string into 'buffer' (NOT NUL-terminated)
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //  - returns number of characters in the final string
    //
    inline std::size_t around (const char * value, char * buffer, std::size_t length, std::size_t precision = 4,
                               std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1) {

        using namespace around_detail;

        if (length > 1) {
#ifdef _CRT_SECURE_NO_WARNINGS
            std::strncpy (&buffer [1], value, length - 2);
#else
            strncpy_s (&buffer [1], length - 1, value, length - 2);
#endif
            buffer [0] = ' ';
            buffer [length - 1] = '\0';

            auto rv = round (suggest (buffer + 1, precision), minimum, maximum) - buffer;

            if (buffer [0] == ' ') {
                std::memmove (&buffer [0], &buffer [1], length - 1);
                --rv;
            }
            return rv;
        } else
            return 0;
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating-point number, like a human would do
    //  - rounded result is stored as string into 'buffer' (NOT NUL-terminated)
    //     - minimum/maximum can limit the number of actual decimals should the algorithm decide outside of the range
    //  - returns number of characters in the final string
    //
    inline std::size_t around (double value, char * buffer, std::size_t length, std::size_t precision = 4,
                               std::size_t minimum = 0, std::size_t maximum = (std::size_t) -1) {
        
        using namespace around_detail;

        if (length > 1) {
            buffer [0] = ' ';

            auto rv = round (suggest (value, buffer + 1, length - 1, precision), minimum, maximum) - buffer;

            if (buffer [0] == ' ') {
                std::memmove (&buffer [0], &buffer [1], length - 1);
                --rv;
            }
            return rv;
        } else
            return 0;
    }

}
