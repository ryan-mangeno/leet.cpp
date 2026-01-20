### Virtual Functions & V-Tables

**Theory:**
Adding `virtual` to a method adds a hidden pointer (vptr) to every object instance, pointing to a v-table. This allows **Dynamic Dispatch** (Runtime Polymorphism).
Without `virtual`, C++ uses **Static Dispatch** (Compile-time), which is faster but ignores derived overrides when called via a base pointer.

**Your Task**
The code below fails because the `Base` destructor and the `print()` method are NOT virtual.
1. Fix the memory leak (Base destructor must be virtual).
2. Fix the output error (Make `print` virtual so the Derived version is called).

**Constraints:**
Do not change `main()` (in the test harness). You can only modify the classes.
