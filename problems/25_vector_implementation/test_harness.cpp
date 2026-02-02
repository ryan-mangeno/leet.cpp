#include <test_framework.h>
#include "submission.h"
#include <string>

using namespace leet_cpp;
using namespace test_framework;

// Track object lifecycle
struct LifecycleTracker {
    static int constructed;
    static int destructed;
    static int copied;
    static int moved;
    
    int value;
    
    LifecycleTracker(int v = 0) : value(v) { constructed++; }
    ~LifecycleTracker() { destructed++; }
    LifecycleTracker(const LifecycleTracker& o) : value(o.value) { copied++; }
    LifecycleTracker(LifecycleTracker&& o) noexcept : value(o.value) { moved++; }
    
    static void reset() { constructed = destructed = copied = moved = 0; }
};

int LifecycleTracker::constructed = 0;
int LifecycleTracker::destructed = 0;
int LifecycleTracker::copied = 0;
int LifecycleTracker::moved = 0;

// Part 1 Tests
TEST(Part1_EmptyVector) {
    Vector<int> v;
    ASSERT_EQ(v.size(), 0);
    ASSERT_TRUE(v.empty());
}

TEST(Part1_PushBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v[0], 1);
    ASSERT_EQ(v[2], 3);
}

TEST(Part1_CapacityGrowth) {
    Vector<int> v;
    size_t prev_cap = 0;
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
        if (v.capacity() > prev_cap) {
            prev_cap = v.capacity();
        }
    }
    ASSERT_TRUE(v.capacity() >= 100);  // Grew geometrically
}

TEST(Part1_Indexing) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v[0] = 15;
    ASSERT_EQ(v[0], 15);
}

// Part 2 Tests
TEST(Part2_Destructor) {
    LifecycleTracker::reset();
    {
        Vector<LifecycleTracker> v;
        v.reserve(3);
        v.push_back(LifecycleTracker(1));
        v.push_back(LifecycleTracker(2));
        v.push_back(LifecycleTracker(3));
    }
    // All objects should be destroyed
    // if we didnt reserve then we wouldve destructed 3 times since it starts at 0, 1 when we push first, then we destruct 1 to copy 1 over, then destruct 2 to copy 2 over, then destruct 3 to copy 3 over, then finally destruct the 3 at the end
    // making a total of 6 destructions during resizing, and 3 destructors since the 3 we push back are temporaries totaling 9 destructions
    // if we reserve then we only have 3 destructions at the end since no resizing happens + 3 destructions for the temporaries totaling 6 destructions
    ASSERT_TRUE(LifecycleTracker::constructed == 3 && LifecycleTracker::destructed == 6);

    LifecycleTracker::reset();
    {
        Vector<LifecycleTracker> v;
        // v.reserve(3); no reserve just to show the above explanation, I was trying to pass this test case and realize it was written wrong lol
        v.push_back(LifecycleTracker(1));
        v.push_back(LifecycleTracker(2));
        v.push_back(LifecycleTracker(3));
    }
    ASSERT_TRUE(LifecycleTracker::constructed == 3 && LifecycleTracker::destructed == 9);
}

TEST(Part2_CopyConstructor) {
    Vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    
    Vector<int> v2(v1);
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0], 1);
    
    v1[0] = 999;
    ASSERT_EQ(v2[0], 1);  // Deep copy!
}

TEST(Part2_CopyAssignment) {
    Vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    
    Vector<int> v2;
    v2 = v1;
    ASSERT_EQ(v2.size(), 2);
}

TEST(Part2_SelfAssignment) {
    Vector<int> v;
    v.push_back(1);
    v = v;  // Should not crash
    ASSERT_EQ(v[0], 1);
}

TEST(Part2_MoveConstructor) {
    Vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    
    Vector<int> v2(std::move(v1));
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v1.size(), 0);  // v1 is empty now
}

TEST(Part2_MoveAssignment) {
    Vector<int> v1;
    v1.push_back(1);
    
    Vector<int> v2;
    v2 = std::move(v1);
    ASSERT_EQ(v2.size(), 1);
}

// Part 3 Tests
TEST(Part3_RangeBasedFor) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    int sum = 0;
    for (auto x : v) {
        sum += x;
    }
    ASSERT_EQ(sum, 6);
}

TEST(Part3_Iterators) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    ASSERT_EQ(*v.begin(), 1);
    ASSERT_EQ(v.end() - v.begin(), 2);
}

// Part 4 Tests
TEST(Part4_Reserve) {
    Vector<int> v;
    v.reserve(100);
    ASSERT_TRUE(v.capacity() >= 100);
    ASSERT_EQ(v.size(), 0);
}

TEST(Part4_PopBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 1);
}

TEST(Part4_At_BoundsCheck) {
    Vector<int> v;
    v.push_back(1);
    ASSERT_THROWS([&](){ v.at(10); });
}

// Advanced Tests
TEST(Advanced_StringVector) {
    Vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    ASSERT_EQ(v[0], "hello");
}

TEST(Advanced_NestedVector) {
    Vector<Vector<int>> v;
    Vector<int> inner;
    inner.push_back(1);
    v.push_back(inner);
    ASSERT_EQ(v[0][0], 1);
}

int main() {
    std::cout << "--- 📚 VECTOR IMPLEMENTATION TESTS ---\n" << std::endl;
    
    SECTION("Part 1: Basic Dynamic Array");
    SECTION("Part 2: Rule of Five");
    SECTION("Part 3: Iterators");
    SECTION("Part 4: Exception Safety");
    SECTION("Advanced Usage");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Learnings:" << std::endl;
    std::cout << "   1. Capacity != Size: Pre-allocate to avoid reallocation" << std::endl;
    std::cout << "   2. Geometric growth (2x): Amortized O(1) push_back" << std::endl;
    std::cout << "   3. Must call destructors explicitly for templates" << std::endl;
    std::cout << "   4. Placement new: Construct in pre-allocated memory" << std::endl;
    std::cout << "\n🎯 Performance: Why 2x growth?" << std::endl;
    std::cout << "   Linear: 1+2+3+...+n = O(n²)" << std::endl;
    std::cout << "   Geometric: 1+2+4+8+16 = O(n) amortized" << std::endl;
    
    return 0;
}
