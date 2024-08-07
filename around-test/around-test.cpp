#include "around.h"
#include <string>

auto tests = 0u;
auto passed = 0u;

void test (double value, double expected, std::size_t precision = 4, std::size_t minimum = 3, std::size_t maximum = 12) {
    ++tests;

    auto rounded = ext::around (value, precision);
    bool success = rounded == expected;

    char formatted [64];
    auto nfmtd = ext::around (value, formatted, sizeof formatted, precision, minimum, maximum);
    formatted [nfmtd] = '\0';

    if (std::strtod (formatted, nullptr) != expected) {
        success = false;
    }

    char stringtestIN [64];
    char stringtestOUT [64];
    std::snprintf (stringtestIN, 64, "%.64f", value);
    auto stringtestN = ext::around (stringtestIN, stringtestOUT, 64, precision, minimum, maximum);
    stringtestOUT [stringtestN] = '\0';

    if (std::strcmp (stringtestOUT, formatted) != 0) {
        success = false;
    }

    std::printf ("%.*f\t=> %f\t\"%s\"\t%s // %u => %u // '%s' (%u) '%s' (%u)\n",
                 32, value, 
                 rounded,
                 std::to_string (rounded).c_str (),
                 success ? "OK" : "FAILED",
                 (unsigned) precision,
                 (unsigned int) ext::around_suggest (value, precision),
                 formatted, (unsigned) nfmtd, 
                 stringtestOUT, (unsigned) stringtestN);

    if (success)
        ++passed;
}

int main () {
    test (0.0, 0.0);
    test (1.0, 1.0);
    test (23.299991, 23.3);
    test (-19.7299999, -19.73);
    test (-19.7200007, -19.72);

    test (1.0000456, 1.0);
    test (27.29999999, 27.3);
    test (27.23499999, 27.235);
    test (27.19939999, 27.1994, 3);
    test (27.19939999, 27.2, 2);

    test (-19.299999, -19.3);
    test (-20.799999, -20.8);
    test (147.000000, 147.0);


    test (7.2992, 7.2992);
    test (7.2002, 7.2002);

    test (7.299991, 7.3);
    test (7.200001, 7.2);
    test (9.999999, 10.0);
    test (999.9999, 1000.0);
    test (599.9999, 600.0);
    test (1099.9999, 1100.0);
    test (-999.9999, -1000.0);
    test (-599.9999, -600.0);
    test (-1099.9999, -1100.0);
    test (9.0999999, 9.1);
    test (-9.0999999, -9.1);

    test (7.299990006565, 7.3);
    test (12345678.299992, 12345678.3);
    test (12345678.9123456789, 12345678.9123456789);

    if (tests && (passed == tests)) {
        std::printf ("SUCCESS.\n");
    } else {
        std::printf ("FAILED! %u/%u tests passed\n", passed, tests);
    }
}
