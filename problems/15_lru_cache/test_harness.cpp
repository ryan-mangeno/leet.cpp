#include <test_framework.h>
#include "submission.h"

using namespace leet_cpp;
using namespace test_framework;

// Part 1: Basic Put and Get Operations
TEST(LRUCache_BasicOps) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    
    ASSERT_EQ(cache.get(1), 1);
    
    // Adding 3 should evict the Least Recently Used (2)
    cache.put(3, 3);
    ASSERT_EQ(cache.get(2), -1); // Evicted
    ASSERT_EQ(cache.get(3), 3);  // Exists
}



// Part 2: Updating Values of Existing Keys
TEST(LRUCache_UpdateExisting) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    
    // Update key 1
    cache.put(1, 10);
    ASSERT_EQ(cache.get(1), 10);
    
    // Ensure key 1 is now "Recently Used" and won't be evicted next
    cache.put(3, 3);
    ASSERT_EQ(cache.get(2), -1); // 2 was LRU
    ASSERT_EQ(cache.get(1), 10); // 1 stays
}

// Part 3: Promotion on Get (The "Access" rule)
TEST(LRUCache_AccessPromotion) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    
    // Accessing 1 makes it MRU (Most Recently Used)
    ASSERT_EQ(cache.get(1), 1);
    
    // Adding 3 should now evict 2 instead of 1
    cache.put(3, 3);
    ASSERT_EQ(cache.get(2), -1);
    ASSERT_EQ(cache.get(1), 1);
}



// Part 4: Edge Case - Capacity of 1
TEST(LRUCache_CapacityOne) {
    LRUCache cache(1);
    cache.put(1, 1);
    cache.put(2, 2); // Immediately evicts 1
    
    ASSERT_EQ(cache.get(1), -1);
    ASSERT_EQ(cache.get(2), 2);
}

// Part 5: Comprehensive Stress / Logical Churn
TEST(LRUCache_Stress) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.put(4, 4); // Evicts 1
    
    ASSERT_EQ(cache.get(4), 4);
    ASSERT_EQ(cache.get(3), 3);
    ASSERT_EQ(cache.get(2), 2);
    ASSERT_EQ(cache.get(1), -1);
    
    cache.put(5, 5); // Evicts 4 (the LRU among 4, 3, 2 after those gets)
    ASSERT_EQ(cache.get(4), -1);
}

int main() {
    std::cout << "--- ⚡ LRU CACHE (LEETCODE 146) VALIDATION ⚡ ---" << std::endl;
    
    SECTION("Basic Eviction Policy");
    SECTION("Value Updates");
    SECTION("Access-Based Promotion");
    SECTION("Minimum Capacity Boundaries");
    SECTION("Sequential Churn Stress");
    
    RUN_ALL_TESTS();
    
    return 0;
}