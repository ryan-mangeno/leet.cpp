# Stack vs Heap Memory Analysis

**Difficulty**: Easy-Medium  
**Topics**: Memory Management, Performance Trade-offs

## Description
Implement functions that demonstrate understanding of stack vs heap allocation.

## Tasks
1. Implement `createOnStack()` - returns a value created on stack
2. Implement `createOnHeap()` - returns a pointer to heap-allocated object
3. Implement `whenToUseHeap()` - explain when heap is necessary
4. Analyze performance differences

## Questions to Answer (in code comments)
- When MUST you use heap allocation?
- What are the performance implications?
- What are the lifetime differences?
- How does this relate to RAII?

## Example
```cpp
// Stack: automatic lifetime, fast, size known at compile-time
Data stackData = createOnStack();

// Heap: manual lifetime, slower, dynamic size
std::unique_ptr<Data> heapData = createOnHeap();
```
