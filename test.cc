#include "cppunit.h"

// Test examples.
class Cppunit_tests: public Cppunit {
    void single_test() {
        // Integral type match check.
        CHECK(2 + 2, 4);

        // Boolean type value check.
        CHECKT(2 + 2 == 4);

        // String match check.
        CHECKS("a" "b", "ab");

        // // Stdin override example.
        // test_cin("2\n2");
        // CHECK((new test_class)->calculate(), 4);
    }
};

int main(int argc, char *argv[]) {
    // Test invocation example.
    return (new Cppunit_tests)->run();
}
