#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <unordered_set>
#include <algorithm>
#include "submission.h"
#include "test_framework.h"

using namespace leet_cpp;
using namespace test_framework;

// --- Helper Structs ---

// Track allocations to detect memory leaks
struct LifetimeTracker {
    static std::atomic<int> active_count;
    int id;

    LifetimeTracker(int v = 0) : id(v) { active_count++; }
    ~LifetimeTracker() { active_count--; }
    
    LifetimeTracker(const LifetimeTracker& o) : id(o.id) { active_count++; }
    LifetimeTracker(LifetimeTracker&& o) noexcept : id(o.id) { active_count++; }
    LifetimeTracker& operator=(const LifetimeTracker& o) { id = o.id; return *this; }
    LifetimeTracker& operator=(LifetimeTracker&& o) noexcept { id = o.id; return *this; }
};
std::atomic<int> LifetimeTracker::active_count{0};

// --- Test Definitions ---

TEST(BasicPushPop) {
    LockFreeStack<int> stack;
    
    stack.push(1);
    stack.push(2);
    stack.push(3);
    
    int val;
    bool success = stack.pop(val);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(val, 3);
}

TEST(EmptyStack) {
    LockFreeStack<int> stack;
    
    ASSERT_TRUE(stack.empty());
    
    int val;
    bool success = stack.pop(val);
    
    // Pop on empty should return false
    ASSERT_TRUE(!success); 
}

TEST(LIFOOrder) {
    LockFreeStack<int> stack;
    
    for (int i = 1; i <= 5; i++) {
        stack.push(i);
    }
    
    int val;
    for (int expected = 5; expected >= 1; expected--) {
        bool success = stack.pop(val);
        ASSERT_TRUE(success);
        ASSERT_EQ(val, expected);
    }
}

TEST(RAII_Cleanup) {
    // Reset count before test just in case
    LifetimeTracker::active_count = 0;
    
    {
        LockFreeStack<LifetimeTracker> stack;
        stack.push(LifetimeTracker(1));
        stack.push(LifetimeTracker(2));
        stack.push(LifetimeTracker(3));
        
        LifetimeTracker val;
        stack.pop(val);
        // Stack destructor should clean up remaining 2 items when scope ends
    }
    
    // Check for leaks
    ASSERT_EQ(LifetimeTracker::active_count.load(), 0);
}

TEST(ConcurrentPush) {
    LockFreeStack<int> stack;
    const int THREADS = 4;
    const int ITEMS_PER_THREAD = 1000;
    
    std::vector<std::thread> threads;
    
    for (int t = 0; t < THREADS; t++) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < ITEMS_PER_THREAD; i++) {
                stack.push(t * ITEMS_PER_THREAD + i);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Count items
    int count = 0;
    int val;
    while (stack.pop(val)) {
        count++;
    }
    
    ASSERT_EQ(count, THREADS * ITEMS_PER_THREAD);
}

TEST(ConcurrentPushPop) {
    LockFreeStack<int> stack;
    const int THREADS = 4;
    const int OPERATIONS = 10000;
    
    std::atomic<int> push_count{0};
    std::atomic<int> pop_count{0};
    
    std::vector<std::thread> threads;
    
    // Mix of pushers and poppers
    for (int t = 0; t < THREADS; t++) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < OPERATIONS; i++) {
                if (i % 2 == 0) {
                    stack.push(i);
                    push_count++;
                } else {
                    int val;
                    if (stack.pop(val)) {
                        pop_count++;
                    }
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Pop remaining items
    int val;
    while (stack.pop(val)) {
        pop_count++;
    }
    
    // Total pushes must equal total pops to ensure no data was lost/dup
    ASSERT_EQ(push_count.load(), pop_count.load());
}

TEST(StressTest) {
    LockFreeStack<int> stack;
    const int THREADS = 8;
    const int ITERATIONS = 50000;
    
    std::atomic<bool> start_flag{false};
    std::vector<std::thread> threads;
    
    for (int t = 0; t < THREADS; t++) {
        threads.emplace_back([&, t]() {
            // Wait for all threads to be ready
            while (!start_flag.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            
            for (int i = 0; i < ITERATIONS; i++) {
                stack.push(t * ITERATIONS + i);
                
                int val;
                stack.pop(val);  // May or may not succeed
            }
        });
    }
    
    start_flag.store(true, std::memory_order_release);
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // If we reached here without crashing, the stress test passed
    ASSERT_TRUE(true);
}

TEST(NoDataRaces) {
    // This test acts as a harness for ThreadSanitizer
    LockFreeStack<int> stack;
    
    std::thread t1([&]() {
        for (int i = 0; i < 1000; i++) stack.push(i);
    });
    
    std::thread t2([&]() {
        int val;
        for (int i = 0; i < 1000; i++) stack.pop(val);
    });
    
    t1.join();
    t2.join();
    
    ASSERT_TRUE(true);
}

int main() {
    // RUN_ALL_TESTS() is defined in test_framework.h
    // It runs all functions registered via the TEST() macro
    RUN_ALL_TESTS();
    return 0;
}