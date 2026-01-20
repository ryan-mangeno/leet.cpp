### Implement a Lock-Free SPSC Queue

**Your Task**
Implement a lock-free single producer single consumer queue (LFSPSCQ). You are allowed to use `std::atomic`.

**Requirements:**
* **Power of 2:** Capacity will always be at least 2 (and always a power of 2). Throw `std::invalid_argument` otherwise.
* **RAII:** You must abide by RAII principles. All items remaining in the queue at the end of its lifetime must be destroyed.
* **No Copy/Move:** Your queue should not be movable or copyable.
* **O(1):** All operations must be O(1).
* **Constraints:** You cannot use `std::memory_order_seq_cst`.

**Functions to Implement:**
* `push(item)`: Returns true if successful, false if full.
* `pop(item)`: Returns true if successful, false if empty.
* `size()`, `empty()`, `full()`
