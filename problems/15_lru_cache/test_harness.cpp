#include <iostream>
#include <cassert>
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

void Test_BasicOps() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    
    log_result("BasicGet", cache.get(1) == 1, "Should return 1");
    
    cache.put(3, 3);
    log_result("EvictLRU", cache.get(2) == -1, "Key 2 should be evicted");
    log_result("NewKey", cache.get(3) == 3, "Key 3 should exist");
}

void Test_UpdateExisting() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(1, 10);
    
    log_result("UpdateValue", cache.get(1) == 10, "Should return updated value");
}

void Test_AccessOrder() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.get(1);
    cache.put(3, 3);
    
    bool correct = (cache.get(2) == -1 && cache.get(1) == 1);
    log_result("LRUOrder", correct, "Wrong eviction order");
}

void Test_SingleCapacity() {
    LRUCache cache(1);
    cache.put(1, 1);
    cache.put(2, 2);
    
    log_result("Capacity1", cache.get(1) == -1 && cache.get(2) == 2,
               "Failed with capacity 1");
}

void Test_MultipleUpdates() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(1, 2);
    cache.put(1, 3);
    cache.put(2, 2);
    cache.put(3, 3);
    
    log_result("MultiUpdate", cache.get(1) == -1, "Should evict after updates");
}

int main() {
    std::cout << "--- ⚡ LRU CACHE TESTS ⚡ ---" << std::endl;
    Test_BasicOps();
    Test_UpdateExisting();
    Test_AccessOrder();
    Test_SingleCapacity();
    Test_MultipleUpdates();
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    return 0;
}
