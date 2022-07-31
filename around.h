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

        // suggest
        //  - internal helper, suggests rounding precision for decimal number passed as string
        //  - this is the nutshell of the algorithm
        //
        template <std::size_t precision = 4>
        std::size_t suggest (char * string, char ** decimal = nullptr) {
            if (auto p = std::strchr (string, '.')) {

                ++p;
                if (decimal) {
                    *decimal = p;
                }

                auto p9 = std::strstr (p, around_detail::needle <precision, '9'> ().string);
                auto p0 = std::strstr (p, around_detail::needle <precision, '0'> ().string);

                if (p0 && p9) {
                    return std::min (p0 - p, p9 - p);
                } else
                if (p9) {
                    return p9 - p;
                } else
                if (p0) {
                    return p0 - p;
                }
            } else
                return 0;
        }

        // suggest
        //  - internal helper, suggests rounding precision for the floating point value
        //  - converts it to string and uses the decision above
        //
        template <std::size_t precision = 4>
        std::size_t suggest (double value, char * buffer, std::size_t length, char ** decimal = nullptr) {
            if (std::snprintf (buffer, length, "%.*f", (unsigned int) length, value)) {
                return suggest <precision> (buffer, decimal);
            } else
                return ~0;
        }
    }

    // around_suggest
    //  - suggests rounding precision
    //  - returns: suggested precision or ~0 
    //
    template <std::size_t precision = 4>
    std::size_t around_suggest  (double value) {
        char string [64];
        return around_detail::suggest <precision> (value, string, sizeof string);
    }

    // around
    //  - adaptive rounding operation; attempts nice rounding of a floating point number, like a human would do
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
    //  - adaptive rounding operation; attempts nice rounding of a floating point number, like a human would do
    //  - 
    //
    template <std::size_t precision = 4>
    void around (double value, char * buffer, std::size_t length,
                 std::size_t minimum = 0, std::size_t maximum = ~0) {
        
        if (length > 1) {
            buffer [0] = ' ';

            char * decimals = nullptr;
            auto n = around_detail::suggest <precision> (value, buffer + 1, length - 1, &decimals);
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
            if (buffer [0] == ' ') {
                std::memmove (&buffer [0], &buffer [1], length - 1);
            }
        }
    }

}
