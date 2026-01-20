### Fix the Deadlock (Multithreading)

**The Situation**
The code below simulates a bank transfer between two accounts. However, it uses manual `lock()` and `unlock()` in a way that causes a **Deadlock** when two threads try to transfer money to each other simultaneously.

**Your Task**
1. Analyze why the deadlock happens (Lock Ordering Inversion).
2. Rewrite the `transfer` function to be **Deadlock-Free** and **Exception-Safe**.
3. **Requirement:** Use `std::scoped_lock` (C++17) or `std::lock_guard` with `std::lock` to solve this cleanly.

**Constraint:**
You must NOT use manual `unlock()`.
