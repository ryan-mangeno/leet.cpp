#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <cmath>


namespace test_framework {


class ExpectedException : public std::exception {
    public:
        ExpectedException(const std::string& msg) : m_Msg(msg) {}
        const char* what() const noexcept override {
            return m_Msg.c_str();
        }
    private:
        std::string m_Msg;

};

class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }

    void add_test(const std::string& name, std::function<void()> test_fn) {
        tests_.push_back({name, test_fn});
    }

    void run_all() {
        std::cout << "Running " << tests_.size() << " tests..." << std::endl;
        
        for (auto& test : tests_) {
            current_test_name_ = test.first;
            try {
                test.second();
                passed_++;
                std::cout << "✅ [PASS] " << test.first << std::endl;
            }
            catch (const ExpectedException& e) {
                passed_++;
                std::cout << "✅ [PASS] " << test.first 
                         << ": " << e.what() << std::endl;
            }
            catch (const std::exception& e) {
                failed_++;
                std::cout << "❌ [FAIL] " << test.first 
                         << ": " << e.what() << std::endl;
            }
        }
        
        print_summary();
    }

    void assert_true(bool condition, const std::string& msg = "") {
        if (!condition) {
            throw std::runtime_error(msg.empty() ? "Assertion failed" : msg);
        }
    }

    void assert_equal(int actual, int expected) {
        if (actual != expected) {
            throw std::runtime_error("Expected " + std::to_string(expected) + 
                                   " but got " + std::to_string(actual));
        }
    }

    void assert_equal(double actual, double expected, double epsilon = 1e-6) {
        if (std::abs(actual - expected) > epsilon) {
            throw std::runtime_error("Expected " + std::to_string(expected) + 
                                   " but got " + std::to_string(actual));
        }
    }

    void assert_equal(const std::string& actual, const std::string& expected) {
        if (actual != expected) {
            throw std::runtime_error("Expected '" + expected + "' but got '" + actual + "'");
        }
    }

    void assert_throws(std::function<void()> fn, const std::string& msg = "") {
        try {
            fn();
            throw std::runtime_error(msg.empty() ? "Expected exception but none thrown" : msg);
        }
        catch( const ExpectedException& e) {
            // expected, return
            return;
        } 
        catch (const std::exception& e) {
            throw e;
        }
    }

    void section(const std::string& name) {
        std::cout << "\n" << "[" << name << "]" << std::endl;
    }

private:
    TestRunner() = default;
    
    void print_summary() {
        std::cout << "\n================================================" << std::endl;
        std::cout << "TEST SUMMARY" << std::endl;
        std::cout << "================================================" << std::endl;
        std::cout << "Total:  " << (passed_ + failed_) << std::endl;
        std::cout << "Passed: " << passed_ << std::endl;
        std::cout << "Failed: " << failed_ << std::endl;
        
        if (failed_ == 0) {
            std::cout << "\n🏆 " << "ALL TESTS PASSED!" << " 🏆\n" << std::endl;
        } else {
            exit(1);
        }
    }

    std::vector<std::pair<std::string, std::function<void()>>> tests_;
    std::string current_test_name_;
    int passed_ = 0;
    int failed_ = 0;
};

// Macros for convenience
#define TEST(name) \
    void test_##name(); \
    struct TestRegistrar_##name { \
        TestRegistrar_##name() { \
            test_framework::TestRunner::instance().add_test(#name, test_##name); \
        } \
    } registrar_##name; \
    void test_##name()

#define ASSERT_TRUE(cond) \
    test_framework::TestRunner::instance().assert_true(cond, "Failed: " #cond)

#define ASSERT_EQ(actual, expected) \
    test_framework::TestRunner::instance().assert_equal(actual, expected)

#define ASSERT_THROWS(fn) \
    test_framework::TestRunner::instance().assert_throws(fn)

#define SECTION(name) \
    test_framework::TestRunner::instance().section(name)

#define RUN_ALL_TESTS() \
    test_framework::TestRunner::instance().run_all()

} // namespace test_framework

#endif
