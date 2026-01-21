#include "submission.h"
#include <iostream>
#include <chrono>
#include <cassert>

void test_basic_usage() {
    std::cout << "Test 1: Basic stack and heap creation..." << std::flush;
    
    Data stackData = createOnStack();
    assert(stackData.values[0] == 0);
    assert(stackData.values[999] == 999);
    
    auto heapData = createOnHeap();
    assert(heapData != nullptr);
    assert(heapData->values[0] == 0);
    assert(heapData->values[999] == 999);
    
    std::cout << " PASSED\n";
}

void test_performance() {
    std::cout << "Test 2: Performance comparison...\n";
    
    const int N = 100000;
    
    // Stack allocation performance
    auto start_stack = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        Data d = createOnStack();
        (void)d;  // Use the variable
    }
    auto end_stack = std::chrono::high_resolution_clock::now();
    auto stack_time = std::chrono::duration_cast<std::chrono::microseconds>(end_stack - start_stack).count();
    
    // Heap allocation performance
    auto start_heap = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto d = createOnHeap();
    }
    auto end_heap = std::chrono::high_resolution_clock::now();
    auto heap_time = std::chrono::duration_cast<std::chrono::microseconds>(end_heap - start_heap).count();
    
    std::cout << "  Stack: " << stack_time << " μs\n";
    std::cout << "  Heap:  " << heap_time << " μs\n";
    std::cout << "  Ratio: " << (double)heap_time / stack_time << "x\n";
    std::cout << " PASSED\n";
}

void test_dynamic_sizing() {
    std::cout << "Test 3: Dynamic sizing..." << std::flush;
    
    auto container = createDynamicallySizedContainer(1000);
    assert(container != nullptr);
    assert(container->size() == 1000);
    
    std::cout << " PASSED\n";
}

int main() {
    std::cout << "=== Stack vs Heap Analysis Tests ===\n";
    test_basic_usage();
    test_performance();
    test_dynamic_sizing();
    std::cout << "\n✓ All tests passed!\n";
    return 0;
}
