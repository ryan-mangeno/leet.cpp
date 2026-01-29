### Resource Manager with RAII

**Context:**
Systems must safely manage hardware resources. Implement a resource manager using modern C++ RAII.

**Part 1: Resource Handle (RAII wrapper)**
- Takes ownership via `unique_ptr`
- Ensures cleanup on destruction
- Move-only semantics

**Part 2: Resource Pool**
- Pre-allocate N resources
- `acquire()`: Get resource (returns handle)
- Auto-release when handle destroyed

**Part 3: Design Questions**
- What if someone forgets to release?
- When would you use `shared_ptr`?
- Exception safety guarantees?

**Constraints:**
- No raw `new`/`delete`
- Must follow RAII
- Exception-safe

**Example:**
```cpp
ResourcePool<Camera> pool(5);
{
    auto cam = pool.acquire();
    cam->capture();
}  // Auto-released
```
