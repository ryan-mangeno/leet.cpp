#include "submission.h"
#include <iostream>
#include <cassert>

struct TestObject {
    static int count;
    int value;
    
    TestObject(int v) : value(v) { ++count; }
    ~TestObject() { --count; }
};

int TestObject::count = 0;

void test_basic() {
    std::cout << "Test 1: Basic construction and destruction..." << std::flush;
    {
        UniquePtr<TestObject> ptr(new TestObject(42));
        assert(TestObject::count == 1);
        assert(ptr->value == 42);
        assert((*ptr).value == 42);
    }
    assert(TestObject::count == 0);
    std::cout << " PASSED\n";
}

void test_move() {
    std::cout << "Test 2: Move semantics..." << std::flush;
    UniquePtr<TestObject> p1(new TestObject(10));
    UniquePtr<TestObject> p2(std::move(p1));
    
    assert(!p1.get());
    assert(p2.get());
    assert(p2->value == 10);
    assert(TestObject::count == 1);
    
    UniquePtr<TestObject> p3(new TestObject(20));
    p3 = std::move(p2);
    assert(TestObject::count == 1);
    assert(p3->value == 10);
    std::cout << " PASSED\n";
}

void test_reset_release() {
    std::cout << "Test 3: Reset and release..." << std::flush;
    UniquePtr<TestObject> ptr(new TestObject(5));
    ptr.reset(new TestObject(15));
    assert(TestObject::count == 1);
    assert(ptr->value == 15);
    
    TestObject* raw = ptr.release();
    assert(!ptr.get());
    assert(raw->value == 15);
    delete raw;
    assert(TestObject::count == 0);
    std::cout << " PASSED\n";
}

int main() {
    std::cout << "=== UniquePtr Implementation Tests ===\n";
    test_basic();
    test_move();
    test_reset_release();
    std::cout << "\n✓ All tests passed!\n";
    return 0;
}
