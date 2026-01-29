### Circular Buffer for Data Recording

**Context:**
Systems continuously record sensor data. You need a fixed-size circular buffer that overwrites old data when full.

**Part 1: Basic Implementation**
- Constructor takes capacity
- `push(item)`: Add item, overwrite oldest if full
- `pop()`: Pop from queue
- `get_front()`: Get element at head
- `get_back()`: Get element at tail
- `size()`, `empty()`, `is_full()`
- Note: return an ExpectedException if you are trying to return an element from an empty queue

**Part 2: Data Access**
- `operator[]`: Access by index (0 = oldest)
- `get_latest(n)`: Return last n items as vector

**Part 3: Thread Safety (Discussion)**
- What if one thread pushes while another reads?
- How would you make this thread-safe?
- Mutex vs lock-free trade-offs?

**Example:**
```cpp
CircularBuffer<int> buf(3);
buf.push(1); buf.push(2); buf.push(3);
buf.push(4);  // dont overrwrite
// Buffer: [1, 2, 3]
buf.pop();
buf.push(5);
// Buffer: [5, 2, 3]
```
