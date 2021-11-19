#include "around.h"
#include <string>

auto tests = 0u;
auto passed = 0u;

void test (double value, double expected) {
    ++tests;

    auto rounded = ext::around (value);
    bool success = rounded == expected;

    std::printf ("%f\t=> %f\t\"%s\"\t%s\n",
                 value,
                 rounded,
                 std::to_string (rounded).c_str (),
                 success ? "OK" : "FAILED");

    if (success)
        ++passed;
}

int main () {
    test (0.0, 0.0);
    test (1.0, 1.0);
    test (23.299999, 23.3);
    test (-19.799999, -19.8);
    test (-19.70007, -19.7);

    test (-19.299999, -19.3);
    test (-20.799999, -20.8);
    test (147.000000, 147.0);


    test (7.2992, 7.2992);
    test (7.2002, 7.2002);

    test (7.29991, 7.3);
    test (7.20001, 7.2);

    test (7.299991, 7.3);
    test (7.200001, 7.2);

    test (7.29990006565, 7.3);
    test (12345678.29992, 12345678.3);

    if (tests && (passed == tests)) {
        std::printf ("SUCCESS.\n");
    } else {
        std::printf ("FAILED! %u/%u tests passed\n", passed, tests);
    }
}
