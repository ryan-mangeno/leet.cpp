#pragma once
#include <memory>
#include <vector>
#include <string>

struct Data {
    int values[1000];  // 4KB of data
    
    Data() {
        for (int i = 0; i < 1000; ++i) {
            values[i] = i;
        }
    }
};

// TODO: Implement this to return a stack-allocated Data object
Data createOnStack() {
    // TODO
}

// TODO: Implement this to return a heap-allocated Data object
std::unique_ptr<Data> createOnHeap() {
    // TODO
}

// TODO: Explain scenarios where heap allocation is necessary
// Write your explanation as code comments here:
/*
 * When to use HEAP allocation:
 * 1. 
 * 2. 
 * 3. 
 * 
 * Performance differences:
 * - Stack: 
 * - Heap: 
 * 
 * Lifetime differences:
 * - Stack: 
 * - Heap: 
 */

// TODO: Implement a function that demonstrates when stack won't work
// For example: returning a local array, polymorphism, dynamic sizing
std::unique_ptr<std::vector<int>> createDynamicallySizedContainer(size_t size) {
    // TODO: Why can't we use stack here?
}

// TODO: Implement a function that shows stack overflow danger
void demonstrateStackLimits() {
    // TODO: Create an array that's too big for stack
    // Comment out if it causes crashes, but explain why
}
