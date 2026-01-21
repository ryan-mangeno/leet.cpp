# Design a Simple Test Framework API

**Difficulty**: Medium  
**Topics**: API Design, Macros, Function Pointers

## Description
Design a minimal test framework API similar to Catch2/GTest.
This tests your ability to design clean, usable APIs.

## Requirements
1. `TEST_CASE(name)` macro to define tests
2. `REQUIRE(condition)` for assertions
3. Automatic test registration
4. Run all tests and report results
5. Gracefully handle test failures

## Design Goals
- Simple and intuitive API
- Minimal boilerplate
- Clear error messages
- Support for multiple test cases

## Example Usage
```cpp
TEST_CASE("Addition works") {
    REQUIRE(1 + 1 == 2);
    REQUIRE(5 + 3 == 8);
}

TEST_CASE("Subtraction works") {
    REQUIRE(5 - 3 == 2);
}
```
