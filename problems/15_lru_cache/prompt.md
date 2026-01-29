### Implement an LRU Cache

**Your Task**
Design and implement a Least Recently Used (LRU) cache with O(1) operations.

**Requirements:**
- `get(key)`: Return value if exists, -1 otherwise. Mark as recently used.
- `put(key, value)`: Insert or update. Evict LRU item if capacity exceeded.
- Both operations must be O(1) time complexity
- Use a combination of hash map + doubly linked list

**Example:**
```cpp
LRUCache cache(2);  // capacity = 2
cache.put(1, 10);
cache.put(2, 20);
cache.get(1);       // returns 10, marks 1 as recent
cache.put(3, 30);   // evicts key 2 (LRU)
cache.get(2);       // returns -1 (evicted)
cache.get(3);       // returns 30
```

**Hint:** Most recently used should be at front, LRU at back.
