#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include <chrono>
#include <cassert>
#include "submission.h"

using namespace leet_cpp;

// ==========================================
// MINI TESTING FRAMEWORK
// ==========================================
#define ANSI_GREEN "\033[32m"
#define ANSI_RED   "\033[31m"
#define ANSI_RESET "\033[0m"

static int g_TestsPassed = 0;
static int g_TestsFailed = 0;

#define TEST_CASE(name) \
    void name(); \
    int main_##name = []{ name(); return 0; }(); \
    void name()

void log_result(const char* name, bool passed, const char* msg = "") {
    if (passed) {
        std::cout << ANSI_GREEN << "✅ [PASS] " << name << ANSI_RESET << std::endl;
        g_TestsPassed++;
    } else {
        std::cout << ANSI_RED << "❌ [FAIL] " << name << ": " << msg << ANSI_RESET << std::endl;
        g_TestsFailed++;
        exit(1); // Fail fast
    }
}

// ==========================================
// 🔍 HELPER CLASSES
// ==========================================

// Tracks how many instances exist. Used to verify RAII cleanup.
struct LifetimeTracker {
    static std::atomic<int> active_count;
    int id;

    LifetimeTracker(int v = 0) : id(v) { active_count++; }
    ~LifetimeTracker() { active_count--; }
    
    // Copy/Move noise to ensure queue handles them
    LifetimeTracker(const LifetimeTracker& o) : id(o.id) { active_count++; }
    LifetimeTracker(LifetimeTracker&& o) noexcept : id(o.id) { active_count++; }
    LifetimeTracker& operator=(const LifetimeTracker& o) { id = o.id; return *this; }
    LifetimeTracker& operator=(LifetimeTracker&& o) noexcept { id = o.id; return *this; }
};
std::atomic<int> LifetimeTracker::active_count{0};

// ==========================================
// 🚀 TEST CASES
// ==========================================

void Test_PowerOfTwo() {
    bool caught = false;
    try {
        SPSCQ<int> q(10); // 10 is not power of 2
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    log_result("Power of 2 Constraint", caught, "Constructor accepted non-power-of-2");
}

void Test_RingBufferLogic() {
    // Capacity 4 means Mask is 3 (0011)
    SPSCQ<int> q(4); 
    
    // Fill it
    if (!q.push(1)) log_result("RingBuffer", false, "Failed push 1");
    if (!q.push(2)) log_result("RingBuffer", false, "Failed push 2");
    if (!q.push(3)) log_result("RingBuffer", false, "Failed push 3");
    if (!q.push(4)) log_result("RingBuffer", false, "Failed push 4");
    
    if (q.push(5)) log_result("RingBuffer", false, "Pushed to full queue");
    if (!q.full()) log_result("RingBuffer", false, "Queue should be full");

    // Pop 2 items to make space at the "front" (indices 0 and 1)
    int val;
    q.pop(val); // 1
    q.pop(val); // 2

    // Indices should now be wrapping soon. 
    // Head is 4, Tail is 2. Size is 2. Capacity 4.
    // Push 2 more. These should wrap around to index 0 and 1 via the mask.
    if (!q.push(5)) log_result("RingBuffer", false, "Failed wrap-around push 5");
    if (!q.push(6)) log_result("RingBuffer", false, "Failed wrap-around push 6");

    // Verify order is 3, 4, 5, 6
    int out;
    q.pop(out); if(out != 3) log_result("RingBuffer", false, "Order corrupted (3)");
    q.pop(out); if(out != 4) log_result("RingBuffer", false, "Order corrupted (4)");
    q.pop(out); if(out != 5) log_result("RingBuffer", false, "Order corrupted (5)");
    q.pop(out); if(out != 6) log_result("RingBuffer", false, "Order corrupted (6)");

    log_result("RingBuffer Wrap-Around", true);
}

void Test_RAII_Cleanup() {
    {
        SPSCQ<LifetimeTracker> q(8);
        q.push(LifetimeTracker(1));
        q.push(LifetimeTracker(2));
        q.push(LifetimeTracker(3));
        // Queue dies here. Destructor should fire for 3 items
    }
    
    // If implementation relied on std::vector's destructor, this should be 0.
    // If they used raw pointers and forgot delete, this will be > 0.
    bool leaks = (LifetimeTracker::active_count != 0);
    log_result("RAII Memory Cleanup", !leaks, "Memory leak detected! Objects not destroyed.");
}

void Test_HeavyConcurrency() {
    SPSCQ<std::string> q(256); // String forces memory allocation checks
    const int ITERATIONS = 200000;
    
    std::thread producer([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            std::string s = "item_" + std::to_string(i);
            while (!q.push(s)) { 
                std::this_thread::yield(); // Spin
            }
        }
    });

    std::thread consumer([&]() {
        std::string item;
        for (int i = 0; i < ITERATIONS; ++i) {
            while (!q.pop(item)) { 
                std::this_thread::yield(); // Spin
            }
            
            std::string expected = "item_" + std::to_string(i);
            if (item != expected) {
                std::cout << "Data Corruption! Expected " << expected << " got " << item << std::endl;
                exit(1);
            }
        }
    });

    producer.join();
    consumer.join();
    log_result("Concurrency Stress Test (Strings)", true);
}

int main() {
    std::cout << "--- ⚡ RUNNING DOJO TEST SUITE ⚡ ---" << std::endl;
    Test_PowerOfTwo();
    Test_RingBufferLogic();
    Test_RAII_Cleanup();
    Test_HeavyConcurrency();
    
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    return 0;
}