### Implement a Memory Pool Allocator

**Your Task**
Create a fast, fixed-size memory pool for frequent allocations/deallocations.

**Why Memory Pools?**
- Reduce fragmentation
- Faster than malloc/free (O(1) vs O(log n))
- Better cache locality
- Common in games, embedded systems

**Requirements:**
- `allocate()`: Return pointer to fixed-size block
- `deallocate(ptr)`: Return block to pool
- Both must be O(1)
- Pre-allocate all memory in constructor
- Use free list to track available blocks

**Constraints:** Cannot use `new`, `delete`, `malloc`, `free` (use placement new on pre-allocated buffer)
