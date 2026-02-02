### Build an Object Pool & Factory System

**Scenario:**
You're building a system that frequently creates/destroys objects (network connections, render objects, AI agents). Allocation is expensive. Design an object pool that reuses objects instead of allocating new ones.

Think: Database connection pools, game object pools, thread pools.

---

## Part 1: Basic Object Pool (15 min)

Implement `ObjectPool<T>`:

```cpp
template<typename T>
class ObjectPool {
public:
    ObjectPool(size_t initial_size);
    
    T* acquire();           // Get object from pool
    void release(T* obj);   // Return to pool
    
    size_t available() const;
    size_t in_use() const;
};
```

**Requirements:**
- Pre-allocate objects on construction
- `acquire()` returns ready-to-use object
- `release()` returns object to pool
- Thread-safe

**Key Design Question:**
```cpp
ObjectPool<ExpensiveObject> pool(100);

auto* obj = pool.acquire();
// Use object...
pool.release(obj);  // Manual release - error prone!

// What if programmer forgets to release?
```

---

## Part 2: RAII Handle (20 min)

Add automatic release with RAII:

```cpp
template<typename T>
class PoolHandle {
    // RAII wrapper - auto-releases on destruction
};

PoolHandle<T> pool.acquire_handle();

// Usage:
{
    auto obj = pool.acquire_handle();
    obj->do_work();
} // Automatically released!
```

**Implementation Challenges:**
1. Handle knows pool - how to call `pool.release()`?
2. Prevent copying - move-only
3. What if handle outlives pool?

**Design Options:**

**Option A: Store pool pointer**
```cpp
class PoolHandle {
    T* obj_;
    ObjectPool<T>* pool_;  // Danger: Pool must outlive handle!
};
```

**Option B: Callback deleter**
```cpp
class PoolHandle {
    T* obj_;
    std::function<void(T*)> deleter_;
};
```

Which is better? Trade-offs?

---

## Part 3: Object Reset & State Management (20 min)

Objects must be cleaned before reuse:

```cpp
class Connection {
    void reset() {
        disconnect();
        clear_buffers();
    }
};

// Pool calls reset before acquire
T* ObjectPool::acquire() {
    T* obj = get_from_available();
    obj->reset();  // But what if T has no reset()?
    return obj;
}
```

**Problem: Not all types have reset()**

Solutions:
1. Require reset() - compile error if missing
2. Optional reset() - SFINAE/concepts
3. Custom reset callback

```cpp
// C++17 SFINAE
template<typename U = T>
auto call_reset(U* obj) -> decltype(obj->reset(), void()) {
    obj->reset();
}

void call_reset(...) { /* no-op */ }

// C++20 Concepts
template<typename T>
concept Resetable = requires(T obj) { obj.reset(); };
```

**Custom initialization:**
```cpp
auto obj = pool.acquire([](Connection* c) {
    c->set_timeout(5000);
    c->enable_ssl();
});
```

---

## Part 4: Dynamic Growth & Shrinking (25 min)

Pool should adapt to demand:

```cpp
class ObjectPool {
    void set_min_size(size_t min);
    void set_max_size(size_t max);
    void set_growth_factor(float factor);  // 1.5x default
    
    void shrink_to_fit();  // Remove excess
};
```

**Growth Strategy:**
```
Start: 10 objects
All acquired → grow to 15
All acquired → grow to 22 (floor(15 * 1.5))
Hit max_size → block or throw?
```

**Questions:**
- Lock during growth?
- What data structure? vector (realloc)? deque? list?
- Multiple threads hit empty pool simultaneously?

**Shrinking:**
```
Pool has 100 objects, 20 in use
Idle 60 seconds → shrink to 30
Background thread? Timer?
```

---

## Part 5: Polymorphic Pool with Factory (25 min)

Support different types from same pool:

```cpp
class Enemy {
public:
    virtual ~Enemy() = default;
    virtual void update() = 0;
};

class Zombie : public Enemy { };
class Skeleton : public Enemy { };

// Need polymorphic pool!
template<typename Base>
class PolymorphicPool {
public:
    template<typename Derived>
    void register_type(const std::string& name);
    
    PoolHandle<Base> acquire(const std::string& type);
};

// Usage:
PolymorphicPool<Enemy> pool;
pool.register_type<Zombie>("zombie");
pool.register_type<Skeleton>("skeleton");

auto z = pool.acquire("zombie");  // Returns Zombie as Enemy*
auto s = pool.acquire("skeleton");
```

**Implementation:**
```cpp
std::map<std::string, std::function<Base*()>> factories_;
std::map<std::string, std::deque<Base*>> type_pools_;
```

**Challenge: Type Safety**
```cpp
auto enemy = pool.acquire("zombie");
// Type is PoolHandle<Enemy>
// How to safely cast to Zombie*?

Zombie* z = dynamic_cast<Zombie*>(enemy.get());
if (!z) { /* error */ }

// Better: Type-safe acquisition?
auto z = pool.acquire_as<Zombie>();
```

---

## Part 6: Performance & Design Discussion

**Benchmarking:**
```cpp
// Without pool (1000 creates):
for (int i = 0; i < 1000; i++) {
    auto* obj = new Heavy();
    delete obj;
}
// Time: ~50ms (allocation overhead)

// With pool:
ObjectPool<Heavy> pool(100);
for (int i = 0; i < 1000; i++) {
    auto obj = pool.acquire_handle();
    // use it
}
// Time: ~0.5ms (100x faster!)
```

**Memory vs Performance:**
```
Pool of 1000 objects at 1KB each = 1MB always allocated
vs
Dynamic allocation: Only what you need, but slower
```

**Questions:**

**Q1: Thread-Local Pools**
```cpp
thread_local ObjectPool<Connection> pool(10);
// Pros: No locking!
// Cons: Can't share between threads
```
When appropriate?

**Q2: Smart Pool Sizing**
Track usage patterns:
```cpp
pool.set_adaptive_sizing(true);
// Learns: Peak usage = 73 objects
// Maintains pool size around 80
```

**Q3: Leak Detection**
```cpp
ObjectPool::~ObjectPool() {
    if (in_use_count_ > 0) {
        log_error("Pool destroyed with " + 
                  std::to_string(in_use_count_) + 
                  " objects still in use!");
    }
}
```

**Q4: Object Validation**
```cpp
void release(T* obj) {
    assert(obj_belongs_to_pool(obj));  // Detect double-release
    assert(!already_in_available(obj)); // Detect corruption
}
```

---

## Your Task:

Implement Parts 1-5 progressively.

**Testing Focus:**
- Concurrent acquire/release from 10 threads
- RAII handles prevent leaks
- Growth when pool exhausted
- Polymorphic object support
- Performance vs raw new/delete
