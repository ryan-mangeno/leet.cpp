#include <test_framework.h>
#include "submission.h"
#include <vector>

using namespace leet_cpp;
using namespace test_framework;

// Part 1: Basic Operations (Insert & Search)
TEST(SkipList_BasicOps) {
    SkipList list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    
    ASSERT_TRUE(list.search(1));
    ASSERT_TRUE(list.search(2));
    ASSERT_TRUE(list.search(3));
    ASSERT_TRUE(!list.search(4)); // Should not find non-existent
}

// Part 2: Deletion Logic
TEST(SkipList_Erase) {
    SkipList list;
    list.insert(10);
    list.insert(20);
    list.insert(30);
    
    // Test erasing existing
    ASSERT_TRUE(list.erase(20));
    ASSERT_TRUE(!list.search(20));
    
    // Test erasing non-existent
    ASSERT_TRUE(!list.erase(40));
    
    // Test erasing head/tail
    ASSERT_TRUE(list.erase(10));
    ASSERT_TRUE(list.erase(30));
    ASSERT_TRUE(!list.search(10));
}



// Part 3: Handling Duplicate Values
TEST(SkipList_Duplicates) {
    SkipList list;
    list.insert(5);
    list.insert(5);
    list.insert(5);
    
    ASSERT_TRUE(list.search(5));
    
    // Erase one '5'
    ASSERT_TRUE(list.erase(5));
    // It should still find '5' because two more remain
    ASSERT_TRUE(list.search(5));
}

// Part 4: Stress Test with Large Dataset
TEST(SkipList_LargeDataset) {
    SkipList list;
    const int count = 1000;
    
    // Insert even numbers
    for (int i = 0; i < count; i += 2) {
        list.insert(i);
    }
    
    // Verify all even numbers exist
    for (int i = 0; i < count; i += 2) {
        ASSERT_TRUE(list.search(i));
    }
    
    // Verify odd numbers do NOT exist
    for (int i = 1; i < count; i += 2) {
        ASSERT_TRUE(!list.search(i));
    }
}

// Part 5: Boundary Values
TEST(SkipList_Boundaries) {
    SkipList list;
    int min_val = std::numeric_limits<int>::min();
    int max_val = std::numeric_limits<int>::max();
    
    list.insert(min_val);
    list.insert(max_val);
    
    ASSERT_TRUE(list.search(min_val));
    ASSERT_TRUE(list.search(max_val));
    
    ASSERT_TRUE(list.erase(min_val));
    ASSERT_TRUE(!list.search(min_val));
}

int main() {
    std::cout << "--- ⚡ SKIP LIST VALIDATION ⚡ ---" << std::endl;
    
    SECTION("Standard Insert/Search");
    SECTION("Node Erasure Logic");
    SECTION("Duplicate Key Handling");
    SECTION("Large Scale Integration");
    SECTION("Integer Boundaries");
    
    RUN_ALL_TESTS();
    
    return 0;
}