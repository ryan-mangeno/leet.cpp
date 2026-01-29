### Implement a Lock-Free Stack

**Your Task**
Implement a lock-free concurrent stack using atomic operations and the ABA problem solution.

**Requirements:**
* **Lock-Free:** Must use `std::atomic` operations only - no mutexes allowed.
* **Thread-Safe:** Multiple threads can push/pop concurrently without data races.
* **ABA Problem:** You must handle the ABA problem using one of:
  - Tagged pointers (store version counter with pointer)
  - Hazard pointers
  - Reference counting
* **RAII:** Properly manage memory - no leaks.
* **Memory Ordering:** Cannot use `std::memory_order_seq_cst` everywhere (too slow).

**Functions to Implement:**
* `push(item)`: Push item onto stack (always succeeds)
* `pop(item&)`: Pop item from stack, return true if successful, false if empty
* `empty()`: Check if stack is empty

**The ABA Problem:**
```
Thread 1: Reads head = A
Thread 2: Pops A, pops B, pushes A back
Thread 1: CAS succeeds (head is A again!) but B was lost
```

**Hints:**
- Use `compare_exchange_weak` for CAS operations
- Node structure needs version counter to prevent ABA
- Use `memory_order_acquire` for loads, `memory_order_release` for stores
- Consider using `std::atomic<uint64_t>` to pack pointer + version

**Example:**
```cpp
LockFreeStack<int> stack;

// Thread 1
stack.push(42);

// Thread 2
int value;
if (stack.pop(value)) {
    // Got value = 42
}
```
