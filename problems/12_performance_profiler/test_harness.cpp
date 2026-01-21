#include "submission.h"
#include <thread>
#include <cassert>

void simulate_work(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void test_basic_timing() {
    std::cout << "Test 1: Basic scoped timing\n";
    {
        ScopedTimer t("Fast operation");
        simulate_work(10);
    }
    {
        ScopedTimer t("Slow operation");
        simulate_work(50);
    }
    std::cout << "\n";
}

void test_nested_timers() {
    std::cout << "Test 2: Nested timers\n";
    {
        ScopedTimer outer("Outer scope");
        simulate_work(20);
        {
            ScopedTimer inner("Inner scope");
            simulate_work(30);
        }
        simulate_work(10);
    }
    std::cout << "\n";
}

void test_accumulated_stats() {
    std::cout << "Test 3: Accumulated statistics\n";
    for (int i = 0; i < 5; ++i) {
        ScopedTimer t("Repeated operation", true);
        simulate_work(10 + i * 5);
    }
    
    std::cout << "\nStatistics Report:\n";
    TimerRegistry::instance().printReport();
}

int main() {
    std::cout << "=== Performance Profiler Tests ===\n\n";
    test_basic_timing();
    test_nested_timers();
    test_accumulated_stats();
    return 0;
}
