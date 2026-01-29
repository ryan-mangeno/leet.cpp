#include <iostream>
#include <vector>
#include "submission.h"

using namespace leet_cpp;

void log_result(const char* name, bool passed, const char* msg = "") {
    if (passed) {
        std::cout << "✅ [PASS] " << name << std::endl;
    } else {
        std::cout << "❌ [FAIL] " << name << ": " << msg << std::endl;
        exit(1);
    }
}

struct TestObj {
    int value;
    TestObj(int v = 0) : value(v) {}
};

void Test_BasicAllocDealloc() {
    MemoryPool<TestObj> pool(10);
    
    TestObj* obj = pool.allocate();
    log_result("AllocateNotNull", obj != nullptr, "Should return valid pointer");
    
    new (obj) TestObj(42);
    log_result("PlacementNew", obj->value == 42, "Object construction failed");
    
    obj->~TestObj();
    pool.deallocate(obj);
    
    log_result("BasicAllocDealloc", true);
}

void Test_MultipleAllocs() {
    MemoryPool<TestObj> pool(5);
    std::vector<TestObj*> objs;
    
    for (int i = 0; i < 5; i++) {
        TestObj* obj = pool.allocate();
        if (!obj) {
            log_result("MultiAlloc", false, "Failed to allocate");
            return;
        }
        new (obj) TestObj(i);
        objs.push_back(obj);
    }
    
    log_result("FullPool", pool.available() == 0, "Pool should be full");
    
    for (auto obj : objs) {
        obj->~TestObj();
        pool.deallocate(obj);
    }
    
    log_result("AfterDealloc", pool.available() == 5, "Should have 5 free");
}

void Test_Reuse() {
    MemoryPool<TestObj> pool(3);
    
    TestObj* obj1 = pool.allocate();
    new (obj1) TestObj(1);
    
    obj1->~TestObj();
    pool.deallocate(obj1);
    
    TestObj* obj2 = pool.allocate();
    log_result("ReuseMemory", obj2 == obj1, "Should reuse memory");
    
    obj2->~TestObj();
    pool.deallocate(obj2);
}

int main() {
    std::cout << "--- ⚡ MEMORY POOL TESTS ⚡ ---" << std::endl;
    Test_BasicAllocDealloc();
    Test_MultipleAllocs();
    Test_Reuse();
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    return 0;
}
