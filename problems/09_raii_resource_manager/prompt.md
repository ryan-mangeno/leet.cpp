# RAII Resource Manager

**Difficulty**: Medium  
**Topics**: RAII, Rule of Five, Move Semantics

## Description
Implement a `ResourceHandle` class that manages a file descriptor using RAII principles.
The class should:
- Open a file in the constructor
- Close it in the destructor
- Support move semantics (but not copying)
- Provide a method to write data to the file

## Requirements
1. Implement the Rule of Five correctly
2. Ensure no resource leaks
3. Handle move operations efficiently
4. Prevent copying (delete copy constructor/assignment)

## Example Usage
```cpp
ResourceHandle handle("output.txt");
handle.write("Hello, Shield AI!");
// File automatically closed when handle goes out of scope
```
