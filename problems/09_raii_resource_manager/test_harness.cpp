#include "submission.h"
#include <iostream>
#include <cassert>
#include <fstream>

void test_basic_usage() {
    std::cout << "Test 1: Basic RAII usage..." << std::flush;
    {
        ResourceHandle handle("test1.txt");
        assert(handle.isValid());
        handle.write("Test data");
    }
    // Verify file was closed and data written
    std::ifstream check("test1.txt");
    std::string content;
    std::getline(check, content);
    assert(content == "Test data");
    std::remove("test1.txt");
    std::cout << " PASSED\n";
}

void test_move_semantics() {
    std::cout << "Test 2: Move semantics..." << std::flush;
    ResourceHandle h1("test2.txt");
    h1.write("Original");
    
    ResourceHandle h2(std::move(h1));
    assert(!h1.isValid());  // h1 should be empty after move
    assert(h2.isValid());   // h2 should own the resource
    h2.write(" Moved");
    
    ResourceHandle h3("test3.txt");
    h3 = std::move(h2);
    assert(!h2.isValid());
    assert(h3.isValid());
    
    std::remove("test2.txt");
    std::remove("test3.txt");
    std::cout << " PASSED\n";
}

void test_no_leak() {
    std::cout << "Test 3: No resource leaks..." << std::flush;
    for (int i = 0; i < 100; ++i) {
        ResourceHandle h("test_leak.txt");
        h.write("data");
    }
    std::remove("test_leak.txt");
    std::cout << " PASSED\n";
}

int main() {
    std::cout << "=== RAII Resource Manager Tests ===\n";
    try {
        test_basic_usage();
        test_move_semantics();
        test_no_leak();
        std::cout << "\n✓ All tests passed!\n";
    } catch (const std::exception& e) {
        std::cout << "\n✗ Test failed: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
