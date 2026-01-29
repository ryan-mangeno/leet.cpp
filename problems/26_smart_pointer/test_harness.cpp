#include <test_framework.h>
#include "submission.h"

using namespace leet_cpp;
using namespace test_framework;

struct TestObject {
    static int count;
    int value;
    
    TestObject(int v = 0) : value(v) { count++; }
    ~TestObject() { count--; }
};
int TestObject::count = 0;

// Part 1: UniquePtr Tests
TEST(UniquePtr_BasicOwnership) {
    TestObject::count = 0;
    {
        UniquePtr<TestObject> p(new TestObject(42));
        ASSERT_EQ(p->value, 42);
        ASSERT_EQ(TestObject::count, 1);
    }
    ASSERT_EQ(TestObject::count, 0);  // Deleted!
}

TEST(UniquePtr_Move) {
    UniquePtr<int> p1(new int(42));
    UniquePtr<int> p2(std::move(p1));
    
    ASSERT_TRUE(!p1);  // Null after move
    ASSERT_TRUE(p2);
    ASSERT_EQ(*p2, 42);
}

TEST(UniquePtr_Release) {
    UniquePtr<int> p(new int(42));
    int* raw = p.release();
    
    ASSERT_TRUE(!p);  // No longer owns
    ASSERT_EQ(*raw, 42);
    delete raw;  // Manual cleanup
}

TEST(UniquePtr_Reset) {
    TestObject::count = 0;
    UniquePtr<TestObject> p(new TestObject(1));
    p.reset(new TestObject(2));
    
    ASSERT_EQ(TestObject::count, 1);  // Old one deleted
    ASSERT_EQ(p->value, 2);
}

// Part 2: SharedPtr Tests
TEST(SharedPtr_BasicRefCount) {
    TestObject::count = 0;
    {
        SharedPtr<TestObject> p1(new TestObject(42));
        ASSERT_EQ(p1.use_count(), 1);
        {
            SharedPtr<TestObject> p2 = p1;
            ASSERT_EQ(p1.use_count(), 2);
            ASSERT_EQ(p2.use_count(), 2);
        }
        ASSERT_EQ(p1.use_count(), 1);  // p2 destroyed
    }
    ASSERT_EQ(TestObject::count, 0);  // Object deleted
}

TEST(SharedPtr_CopySemantics) {
    SharedPtr<int> p1(new int(42));
    SharedPtr<int> p2 = p1;
    
    ASSERT_EQ(*p1, 42);
    ASSERT_EQ(*p2, 42);
    ASSERT_EQ(p1.use_count(), 2);
    
    *p1 = 100;
    ASSERT_EQ(*p2, 100);  // Shared ownership!
}

TEST(SharedPtr_MoveSemantics) {
    SharedPtr<int> p1(new int(42));
    ASSERT_EQ(p1.use_count(), 1);
    
    SharedPtr<int> p2(std::move(p1));
    ASSERT_TRUE(!p1);
    ASSERT_EQ(p2.use_count(), 1);  // Count unchanged
}

TEST(SharedPtr_SelfAssignment) {
    SharedPtr<int> p(new int(42));
    p = p;  // Should not crash
    ASSERT_EQ(p.use_count(), 1);
}

TEST(SharedPtr_Reset) {
    TestObject::count = 0;
    SharedPtr<TestObject> p(new TestObject(1));
    ASSERT_EQ(TestObject::count, 1);
    
    p.reset(new TestObject(2));
    ASSERT_EQ(TestObject::count, 1);  // Old deleted, new created
    ASSERT_EQ(p->value, 2);
}

// Part 3: WeakPtr Tests
TEST(WeakPtr_BasicUsage) {
    SharedPtr<int> sp(new int(42));
    WeakPtr<int> wp(sp);
    
    ASSERT_TRUE(!wp.expired());
    ASSERT_EQ(sp.use_count(), 1);  // WeakPtr doesn't increment!
}

TEST(WeakPtr_Lock) {
    SharedPtr<int> sp(new int(42));
    WeakPtr<int> wp(sp);
    
    SharedPtr<int> sp2 = wp.lock();
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 42);
    ASSERT_EQ(sp.use_count(), 2);
}

TEST(WeakPtr_Expired) {
    WeakPtr<int> wp;
    {
        SharedPtr<int> sp(new int(42));
        wp = WeakPtr<int>(sp);
        ASSERT_TRUE(!wp.expired());
    }
    ASSERT_TRUE(wp.expired());  // SharedPtr destroyed
    
    SharedPtr<int> sp2 = wp.lock();
    ASSERT_TRUE(!sp2);  // Returns null
}

TEST(WeakPtr_BreaksCycle) {
    struct Node {
        SharedPtr<Node> next;
        WeakPtr<Node> prev;
        int value;
        Node(int v) : value(v) {}
    };
    
    SharedPtr<Node> n1(new Node(1));
    SharedPtr<Node> n2(new Node(2));
    
    n1->next = n2;
    n2->prev = WeakPtr<Node>(n1);  // Weak link back
    
    ASSERT_EQ(n1.use_count(), 1);  // No cycle!
}

// Part 4: make_shared Tests
TEST(MakeShared_Basic) {
    auto p = make_shared<int>(42);
    ASSERT_EQ(*p, 42);
    ASSERT_EQ(p.use_count(), 1);
}

TEST(MakeShared_Object) {
    TestObject::count = 0;
    {
        auto p = make_shared<TestObject>(42);
        ASSERT_EQ(p->value, 42);
        ASSERT_EQ(TestObject::count, 1);
    }
    ASSERT_EQ(TestObject::count, 0);
}

// Memory Tests
TEST(Memory_NoLeaks) {
    TestObject::count = 0;
    
    // Create and destroy many pointers
    for (int i = 0; i < 100; i++) {
        SharedPtr<TestObject> p1(new TestObject(i));
        SharedPtr<TestObject> p2 = p1;
        SharedPtr<TestObject> p3 = p2;
    }
    
    ASSERT_EQ(TestObject::count, 0);
}

TEST(Memory_ComplexScenario) {
    TestObject::count = 0;
    
    SharedPtr<TestObject> p1(new TestObject(1));
    SharedPtr<TestObject> p2(new TestObject(2));
    SharedPtr<TestObject> p3 = p1;
    
    WeakPtr<TestObject> w1(p1);
    WeakPtr<TestObject> w2(p2);
    
    p1.reset();
    ASSERT_TRUE(!w1.expired());  // p3 still holds it
    
    p3.reset();
    ASSERT_TRUE(w1.expired());  // Now expired
    
    p2.reset();
    ASSERT_TRUE(w2.expired());
    
    ASSERT_EQ(TestObject::count, 0);
}

int main() {
    std::cout << "--- 🧠 SMART POINTER TESTS ---\n" << std::endl;
    
    SECTION("Part 1: UniquePtr (Exclusive Ownership)");
    SECTION("Part 2: SharedPtr (Reference Counting)");
    SECTION("Part 3: WeakPtr (Non-Owning Observer)");
    SECTION("Part 4: make_shared");
    SECTION("Memory Management");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Learnings:" << std::endl;
    std::cout << "   1. UniquePtr: Single owner, move-only" << std::endl;
    std::cout << "   2. SharedPtr: Multiple owners, ref counting" << std::endl;
    std::cout << "   3. WeakPtr: Breaks cycles, doesn't own" << std::endl;
    std::cout << "   4. Reference count must be atomic for thread safety" << std::endl;
    std::cout << "\n🔍 When to use which?" << std::endl;
    std::cout << "   - Default: UniquePtr (cheapest)" << std::endl;
    std::cout << "   - Need sharing: SharedPtr" << std::endl;
    std::cout << "   - Observing without owning: WeakPtr" << std::endl;
    
    return 0;
}
