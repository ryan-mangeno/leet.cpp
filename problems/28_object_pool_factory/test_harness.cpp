#include <test_framework.h>

#include "submission.h"
#include <thread>
#include <vector>

using namespace leet_cpp;
using namespace test_framework;

// Test object with lifecycle tracking
struct TestObject {
    static int constructed;
    static int destructed;
    static int reset_called;
    
    int id;
    
    TestObject() : id(0) { constructed++; }
    ~TestObject() { destructed++; }
    
    void reset() {
        id = 0;
        reset_called++;
    }
    
    static void reset_counters() {
        constructed = destructed = reset_called = 0;
    }
};

int TestObject::constructed = 0;
int TestObject::destructed = 0;
int TestObject::reset_called = 0;

// Part 1 Tests
TEST(Part1_BasicPool) {
    TestObject::reset_counters();
    {
        ObjectPool<TestObject> pool(5);
        ASSERT_EQ(pool.available(), 5);
        ASSERT_EQ(pool.in_use(), 0);
    }
    ASSERT_EQ(TestObject::constructed, 5);
    ASSERT_EQ(TestObject::destructed, 5);
}

TEST(Part1_AcquireRelease) {
    ObjectPool<TestObject> pool(3);
    
    auto* obj1 = pool.acquire();
    ASSERT_EQ(pool.available(), 2);
    ASSERT_EQ(pool.in_use(), 1);
    
    auto* obj2 = pool.acquire();
    ASSERT_EQ(pool.in_use(), 2);
    
    pool.release(obj1);
    ASSERT_EQ(pool.available(), 1);
    ASSERT_EQ(pool.in_use(), 1);
}

TEST(Part1_ThreadSafety) {
    ObjectPool<TestObject> pool(100);
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&]() {
            for (int j = 0; j < 100; j++) {
                auto* obj = pool.acquire();
                pool.release(obj);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    ASSERT_EQ(pool.available(), 100);
}

// Part 2 Tests
TEST(Part2_RAIIHandle) {
    TestObject::reset_counters();
    ObjectPool<TestObject> pool(5);
    
    {
        auto handle = pool.acquire_handle();
        ASSERT_EQ(pool.in_use(), 1);
    }
    
    ASSERT_EQ(pool.available(), 5);  // Auto-released!
}

TEST(Part2_MoveSemantics) {
    ObjectPool<TestObject> pool(5);
    
    auto h1 = pool.acquire_handle();
    ASSERT_TRUE(h1);
    ASSERT_EQ(pool.in_use(), 1);
    
    auto h2 = std::move(h1);
    ASSERT_TRUE(!h1);
    ASSERT_TRUE(h2);
    ASSERT_EQ(pool.in_use(), 1);  // Still just 1
}

TEST(Part2_NoLeaks) {
    TestObject::reset_counters();
    {
        ObjectPool<TestObject> pool(10);
        for (int i = 0; i < 100; i++) {
            auto h = pool.acquire_handle();
            // Automatically released each iteration
        }
    }
    // All objects properly cleaned up
    ASSERT_EQ(TestObject::constructed, TestObject::destructed);
}

// Part 3 Tests
TEST(Part3_ResetCalled) {
    TestObject::reset_counters();
    ObjectPool<TestObject> pool(5);
    
    auto* obj = pool.acquire();
    obj->id = 42;
    pool.release(obj);
    
    auto* obj2 = pool.acquire();
    ASSERT_EQ(obj2->id, 0);  // Reset was called
    ASSERT_TRUE(TestObject::reset_called > 0);
}

struct NoReset {
    int value = 0;
};

TEST(Part3_OptionalReset) {
    ObjectPool<NoReset> pool(5);  // Should compile even without reset()
    auto* obj = pool.acquire();
    obj->value = 123;
    pool.release(obj);
    ASSERT_TRUE(true);  // Just checking it compiles
}

// Part 4 Tests
TEST(Part4_DynamicGrowth) {
    ObjectPool<TestObject> pool(5);
    pool.set_max_size(20);
    
    std::vector<TestObject*> objs;
    for (int i = 0; i < 15; i++) {
        objs.push_back(pool.acquire());
    }
    
    ASSERT_TRUE(pool.total_size() >= 15);  // Pool grew
}

TEST(Part4_MaxSizeEnforced) {
    ObjectPool<TestObject> pool(2);
    pool.set_max_size(2);
    
    auto* obj1 = pool.acquire();
    auto* obj2 = pool.acquire();
    
    ASSERT_THROWS([&]() {
        pool.acquire();  // Should throw - pool exhausted
    });
}

TEST(Part4_Shrinking) {
    TestObject::reset_counters();
    ObjectPool<TestObject> pool(2);
    pool.set_max_size(20);
    
    // Grow it
    std::vector<TestObject*> objs;
    for (int i = 0; i < 15; i++) {
        objs.push_back(pool.acquire());
    }
    
    // Release all
    for (auto* obj : objs) {
        pool.release(obj);
    }
    
    size_t before = pool.available();
    pool.shrink_to_fit();
    ASSERT_TRUE(pool.available() < before);
}

// Part 5 Tests
TEST(Part5_PolymorphicPool) {
    struct Animal {
        virtual ~Animal() = default;
        virtual std::string sound() = 0;
    };
    
    struct Dog : Animal {
        std::string sound() override { return "woof"; }
    };
    
    struct Cat : Animal {
        std::string sound() override { return "meow"; }
    };
    
    PolymorphicPool<Animal> pool(5);
    pool.register_type<Dog>("dog");
    pool.register_type<Cat>("cat");
    
    auto d = pool.acquire("dog");
    auto c = pool.acquire("cat");
    
    ASSERT_EQ(d->sound(), "woof");
    ASSERT_EQ(c->sound(), "meow");
}

// Performance Test
TEST(Performance_PoolVsNew) {
    const int ITERATIONS = 10000;
    
    // Pool approach
    auto start_pool = std::chrono::high_resolution_clock::now();
    {
        ObjectPool<TestObject> pool(100);
        for (int i = 0; i < ITERATIONS; i++) {
            auto h = pool.acquire_handle();
        }
    }
    auto end_pool = std::chrono::high_resolution_clock::now();
    auto pool_time = std::chrono::duration_cast<std::chrono::microseconds>(
        end_pool - start_pool).count();
    
    // Raw new/delete
    auto start_raw = std::chrono::high_resolution_clock::now();
    {
        for (int i = 0; i < ITERATIONS; i++) {
            auto* obj = new TestObject();
            delete obj;
        }
    }
    auto end_raw = std::chrono::high_resolution_clock::now();
    auto raw_time = std::chrono::duration_cast<std::chrono::microseconds>(
        end_raw - start_raw).count();
    
    std::cout << "  Pool time: " << pool_time << "μs" << std::endl;
    std::cout << "  Raw time:  " << raw_time << "μs" << std::endl;
    std::cout << "  Speedup:   " << (raw_time / (double)pool_time) << "x" << std::endl;
    
    ASSERT_TRUE(pool_time < raw_time);  // Pool should be faster
}

int main() {
    std::cout << "--- 🏊 OBJECT POOL TESTS ---\n" << std::endl;
    
    SECTION("Part 1: Basic Pool");
    SECTION("Part 2: RAII Handles");
    SECTION("Part 3: Reset & Initialization");
    SECTION("Part 4: Dynamic Sizing");
    SECTION("Part 5: Polymorphic Pool");
    SECTION("Performance");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Learnings:" << std::endl;
    std::cout << "   1. Object pools amortize allocation cost" << std::endl;
    std::cout << "   2. RAII handles prevent resource leaks" << std::endl;
    std::cout << "   3. SFINAE enables optional reset()" << std::endl;
    std::cout << "   4. Dynamic sizing adapts to demand" << std::endl;
    std::cout << "   5. Factory pattern enables polymorphism" << std::endl;
    
    std::cout << "\n🎯 When to use Object Pools:" << std::endl;
    std::cout << "   ✅ Frequent create/destroy cycles" << std::endl;
    std::cout << "   ✅ Expensive object construction" << std::endl;
    std::cout << "   ✅ Predictable object lifetimes" << std::endl;
    std::cout << "   ❌ Objects with widely varying sizes" << std::endl;
    std::cout << "   ❌ Unpredictable memory needs" << std::endl;
    
    return 0;
}
