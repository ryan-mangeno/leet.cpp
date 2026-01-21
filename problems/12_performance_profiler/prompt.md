# Performance Profiling Utilities

**Difficulty**: Medium  
**Topics**: RAII, Timing, Diagnostics

## Description
Create utilities to measure and report function performance.
This is critical for systems like EdgeOS where performance matters.

## Requirements
1. `ScopedTimer` class that measures time in a scope
2. Automatic reporting on destruction
3. Nested timer support with indentation
4. Aggregate statistics (min/max/avg)

## Example Usage
```cpp
{
    ScopedTimer t("MyFunction");
    // ... work ...
}  // Prints: "MyFunction took 15ms"
```

## Advanced Feature
Support accumulating multiple measurements:
```cpp
for (int i = 0; i < 100; ++i) {
    ScopedTimer t("Loop iteration");
    // ...
}
// Prints stats: min, max, avg, total
```
