#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <sstream>

// Test result structure
struct TestResult {
    std::string name;
    bool passed;
    std::string error;
};

// Test registry (singleton pattern)
class TestRegistry {
public:
    static TestRegistry& instance() {
        static TestRegistry registry;
        return registry;
    }
    
    void registerTest(const std::string& name, std::function<void()> test) {
        // TODO: implement
    }
    
    std::vector<TestResult> runAll() {
        // TODO: implement - run all tests and collect results
    }

private:
    // TODO: add members to store registered tests
};

// Assertion exception
class AssertionFailure : public std::exception {
public:
    AssertionFailure(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};

// REQUIRE macro
#define REQUIRE(condition) \
    if (!(condition)) { \
        std::ostringstream oss; \
        oss << "Assertion failed: " << #condition \
            << " at " << __FILE__ << ":" << __LINE__; \
        throw AssertionFailure(oss.str()); \
    }

// TEST_CASE macro - creates a function and auto-registers it
// TODO: Implement this macro to:
// 1. Create a unique function for the test
// 2. Auto-register it with TestRegistry
// Hint: use __COUNTER__ for unique names

#define TEST_CASE(name) \
    /* TODO: implement */

// Runner function
inline int runTests() {
    auto results = TestRegistry::instance().runAll();
    
    int passed = 0, failed = 0;
    for (const auto& result : results) {
        if (result.passed) {
            std::cout << "✓ " << result.name << "\n";
            ++passed;
        } else {
            std::cout << "✗ " << result.name << "\n";
            std::cout << "  " << result.error << "\n";
            ++failed;
        }
    }
    
    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
