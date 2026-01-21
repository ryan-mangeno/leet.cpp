#include "submission.h"

// Define some test cases using the framework
TEST_CASE("Basic arithmetic") {
    REQUIRE(2 + 2 == 4);
    REQUIRE(10 - 5 == 5);
    REQUIRE(3 * 4 == 12);
}

TEST_CASE("String operations") {
    std::string s = "Hello";
    REQUIRE(s.length() == 5);
    REQUIRE(s + " World" == "Hello World");
}

TEST_CASE("This test should fail") {
    REQUIRE(1 + 1 == 3);  // Intentional failure
}

TEST_CASE("Vector operations") {
    std::vector<int> v = {1, 2, 3};
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
}

int main() {
    std::cout << "=== Test Framework API Tests ===\n\n";
    return runTests();
}
