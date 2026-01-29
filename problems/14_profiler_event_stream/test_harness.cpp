#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "submission.h"

using namespace leet_cpp;

int g_passed = 0;
int g_failed = 0;

void log_result(const char* name, bool passed, const char* msg = "") {
    if (passed) {
        std::cout << "✅ [PASS] " << name << std::endl;
        g_passed++;
    } else {
        std::cout << "❌ [FAIL] " << name << ": " << msg << std::endl;
        g_failed++;
        exit(1);
    }
}

bool compare_event(const Event& a, const Event& b) {
    return a.Type == b.Type && 
           std::abs(a.Elapsed - b.Elapsed) < 0.01 && 
           a.Method == b.Method;
}

void Test_SingleSample() {
    Samples input = {{2.4, {"main", "foo", "bar"}}};
    
    Events expected = {
        {Kind::Begin, 2.4, "main"},
        {Kind::Begin, 2.4, "foo"},
        {Kind::Begin, 2.4, "bar"},
        {Kind::End, 2.4, "bar"},
        {Kind::End, 2.4, "foo"},
        {Kind::End, 2.4, "main"}
    };
    
    Events result = GenerateEvents(input);
    
    if (result.size() != expected.size()) {
        log_result("SingleSample", false, "Wrong event count");
        return;
    }
    
    for (size_t i = 0; i < result.size(); i++) {
        if (!compare_event(result[i], expected[i])) {
            std::cout << "Expected: " << (expected[i].Type == Kind::Begin ? "Begin" : "End") 
                      << " " << expected[i].Elapsed << " " << expected[i].Method << std::endl;
            std::cout << "Got: " << (result[i].Type == Kind::Begin ? "Begin" : "End")
                      << " " << result[i].Elapsed << " " << result[i].Method << std::endl;
            log_result("SingleSample", false, "Event mismatch");
            return;
        }
    }
    
    log_result("SingleSample", true);
}

void Test_StackTransition() {
    Samples input = {
        {1.0, {"main", "foo"}},
        {2.0, {"main", "bar"}}
    };
    
    Events expected = {
        {Kind::Begin, 1.0, "main"},
        {Kind::Begin, 1.0, "foo"},
        {Kind::End, 2.0, "foo"},
        {Kind::Begin, 2.0, "bar"},
        {Kind::End, 2.0, "bar"},
        {Kind::End, 2.0, "main"}
    };
    
    Events result = GenerateEvents(input);
    
    if (result.size() != expected.size()) {
        log_result("StackTransition", false, "Wrong event count");
        return;
    }
    
    for (size_t i = 0; i < result.size(); i++) {
        if (!compare_event(result[i], expected[i])) {
            log_result("StackTransition", false, "Event mismatch");
            return;
        }
    }
    
    log_result("StackTransition", true);
}

void Test_DeepNesting() {
    Samples input = {
        {1.0, {"main", "a", "b", "c"}},
        {2.0, {"main", "a", "b", "d"}},
        {3.0, {"main", "a", "e"}}
    };
    
    Events result = GenerateEvents(input);
    
    bool found_c_end = false;
    bool found_d_begin = false;
    bool correct_order = false;
    
    for (const auto& e : result) {
        if (e.Method == "c" && e.Type == Kind::End) found_c_end = true;
        if (found_c_end && e.Method == "d" && e.Type == Kind::Begin) {
            found_d_begin = true;
            correct_order = true;
        }
    }
    
    log_result("DeepNesting", found_c_end && found_d_begin && correct_order, 
               "Incorrect nesting transitions");
}

void Test_EmptyInput() {
    Samples input = {};
    Events result = GenerateEvents(input);
    log_result("EmptyInput", result.empty(), "Should return empty for empty input");
}

void Test_ComplexScenario() {
    Samples input = {
        {1.0, {"main"}},
        {2.0, {"main", "init"}},
        {3.0, {"main", "process", "parse"}},
        {4.0, {"main", "process", "validate"}},
        {5.0, {"main"}},
    };
    
    Events result = GenerateEvents(input);
    
    int begins = 0, ends = 0;
    for (const auto& e : result) {
        if (e.Type == Kind::Begin) begins++;
        else ends++;
    }
    
    log_result("ComplexScenario", begins == ends, "Unbalanced Begin/End events");
}

int main() {
    std::cout << "--- ⚡ PROFILER EVENT STREAM TESTS ⚡ ---" << std::endl;
    Test_SingleSample();
    Test_StackTransition();
    Test_DeepNesting();
    Test_EmptyInput();
    Test_ComplexScenario();
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    std::cout << "Passed: " << g_passed << " | Failed: " << g_failed << std::endl;
    return 0;
}
