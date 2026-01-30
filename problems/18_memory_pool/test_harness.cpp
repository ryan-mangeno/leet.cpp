#include <test_framework.h>
#include "submission.h"
#include <vector>

using namespace leet_cpp;
using namespace test_framework;

struct TestObj {
    int value;
    TestObj(int v = 0) : value(v) {}
};

// Part 1: Basic Allocation Logic
TEST(MemoryPool_Basic) {
    MemoryPool<TestObj> pool(10);
    
    TestObj* obj = pool.allocate();
    ASSERT_TRUE(obj != nullptr);
    
    // Test placement new construction
    new (obj) TestObj(42);
    ASSERT_EQ(obj->value, 42);
    
    obj->~TestObj();
    pool.deallocate(obj);
}

// Part 2: Capacity and Boundary Testing
TEST(MemoryPool_Exhaustion) {
    MemoryPool<TestObj> pool(5);
    std::vector<TestObj*> objs;
    
    for (int i = 0; i < 5; i++) {
        TestObj* obj = pool.allocate();
        ASSERT_TRUE(obj != nullptr);
        new (obj) TestObj(i);
        objs.push_back(obj);
    }
    
    // Check if the pool is correctly reporting it's full
    ASSERT_EQ(pool.available(), 0);
    
    // Next allocation should return nullptr
    ASSERT_TRUE(pool.allocate() == nullptr);
    
    for (auto obj : objs) {
        obj->~TestObj();
        pool.deallocate(obj);
    }
    
    ASSERT_EQ(pool.available(), 5);
}

// Part 3: Memory Reuse (The Freelink/LIFO check)
TEST(MemoryPool_Reuse) {
    MemoryPool<TestObj> pool(3);
    
    TestObj* obj1 = pool.allocate();
    pool.deallocate(obj1);
    
    // A standard pool should return the same address for the next allocation
    TestObj* obj2 = pool.allocate();
    ASSERT_TRUE(obj1 == obj2);
    
    pool.deallocate(obj2);
}



// Part 4: Stress and Fragmentation
TEST(MemoryPool_Stress) {
    MemoryPool<int> pool(100);
    std::vector<int*> ptrs;

    // Rapid churn of allocations and deallocations
    for(int i = 0; i < 500; ++i) {
        int* p = pool.allocate();
        if(p) ptrs.push_back(p);
        
        if(ptrs.size() > 20) {
            pool.deallocate(ptrs.front());
            ptrs.erase(ptrs.begin());
        }
    }
    
    // Clean up remaining
    for(int* p : ptrs) pool.deallocate(p);
    ASSERT_EQ(pool.available(), 100);
}

int main() {
    std::cout << "--- ⚡ MEMORY POOL VALIDATION ⚡ ---" << std::endl;
    
    SECTION("Allocation & Construction");
    SECTION("Capacity Management");
    SECTION("Block Reuse (Freelist)");
    SECTION("Stress Performance");
    
    RUN_ALL_TESTS();
    
    return 0;
}