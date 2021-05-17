#include "color.h"
#include "test_util.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

void test_hsl_to_rgb() {
    hsl_color_t hsl_color = {25.0, 1.0, 0.75};
    rgb_color_t rgb = color_convert_hsl_to_rgb(hsl_color);
    assert(isclose(rgb.r, 1.0));
    assert(isclose(rgb.g, 0.70833333));
    assert(isclose(rgb.b, 0.5));
}

int main(int argc, char *argv[]) {
    // Run all tests? True if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_hsl_to_rgb)

    puts("hsl_to_rgb PASS");
}
