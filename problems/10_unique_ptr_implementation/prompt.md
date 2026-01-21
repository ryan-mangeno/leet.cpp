# Implement a Simplified unique_ptr

**Difficulty**: Medium  
**Topics**: Smart Pointers, RAII, Template Programming

## Description
Implement a simplified version of `std::unique_ptr` called `UniquePtr<T>`.

## Requirements
1. Single ownership semantics
2. Automatic deletion on destruction
3. Move-only (no copying)
4. Dereference operators (`*` and `->`)
5. `reset()` and `release()` methods
6. `get()` to access raw pointer

## Example Usage
```cpp
UniquePtr<int> ptr(new int(42));
std::cout << *ptr << std::endl;  // 42
ptr.reset(new int(100));
int* raw = ptr.release();  // ptr is now null
delete raw;
```
