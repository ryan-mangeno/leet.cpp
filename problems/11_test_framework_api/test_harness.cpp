#include "submission.h"

TEST_CASE("Numerical Boundaries & Floating Point") {
    REQUIRE(std::numeric_limits<int>::max() > 0);
    REQUIRE(std::numeric_limits<int>::min() < 0);
    
    // Floating point precision edge case
    double a = 0.1 + 0.2;
    REQUIRE(std::abs(a - 0.3) < 0.00001); 
}

TEST_CASE("String & Object Equality") {
    std::string s1 = "LeetCode";
    std::string s2 = "Leet" + std::string("Code");
    REQUIRE(s1 == s2);
    REQUIRE(s1.length() == 8);
    REQUIRE(!s1.empty());
}

TEST_CASE("Pointer & Nullptr Logic") {
    int val = 100;
    int* ptr = &val;
    int* empty = nullptr;
    
    REQUIRE(ptr != nullptr);
    REQUIRE(empty == nullptr);
    REQUIRE(*ptr == 100);
}

TEST_CASE("Container Integrity") {
    std::vector<int> v = {1, 2, 3, 4, 5};
    REQUIRE(v.size() == 5);
    REQUIRE(v.front() == 1);
    REQUIRE(v.back() == 5);
    
    v.push_back(6);
    REQUIRE(v.size() == 6);
}

TEST_CASE("Intentional Failure (To Test Reporter)") {
    // This is to ensure the framework correctly catches and reports a fail
    REQUIRE(1 + 1 == 3); 
}

TEST_CASE("Boolean Short-Circuiting") {
    int side_effect = 0;
    auto check = [&]() { side_effect++; return true; };
    
    REQUIRE(check());
    REQUIRE(side_effect == 1); // Ensures REQUIRE executes expression exactly once
}

TEST_CASE("Large Scale Assertion Stress") {
    for(int i = 0; i < 100; ++i) {
        REQUIRE(i * 2 == i + i);
    }
}


TEST_CASE("Failure") {
    REQUIRE(2 + 2 == 5);
}


// Runner function
static int runTests() {
    auto results = TestRegistry::instance().runAll();
    
    int passed = 0, failed = 0;
    for (const auto& result : results) {
        if (result.passed) {
            std::cout << "✅ "  << result.name << "\n";
            ++passed;
        } else {
            std::cout << "❌ " << result.name << "\n";
            std::cout << "  " << result.error << "\n";
            ++failed;
        }
    }
    
    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 2 ? true : false;
}


int main() {
    std::cout << "=== Starting Custom Test Framework API ===\n\n";
    if (runTests()) {
        std::cout << "\n🏆 ALL TESTS PASSED! 🏆\n";
    } else {
        std::cout << "\n❌ SOME TESTS FAILED! ❌\n";
    }
    return 0;
}