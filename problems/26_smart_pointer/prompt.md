### Implement Smart Pointers (Ownership & Lifetime)

**Learning Goals:**
Understand ownership semantics, reference counting, and why raw pointers are dangerous.

---

## Part 1: UniquePtr (Exclusive Ownership)
Implement `UniquePtr<T>` with:
- Constructor takes raw pointer
- Destructor deletes pointer
- Move semantics (no copy!)
- `get()`, `operator*`, `operator->`
- `release()`: Give up ownership
- `reset()`: Delete current, take new

**Key Concept: Single Owner**
```cpp
UniquePtr<int> p1(new int(42));
UniquePtr<int> p2 = p1;        // ERROR: No copy!
UniquePtr<int> p2 = std::move(p1);  // OK: Transfer ownership
// p1 is now null
```

---

## Part 2: SharedPtr (Shared Ownership)
Implement `SharedPtr<T>` with:
- Constructor takes raw pointer, creates control block
- Copy constructor: Increment ref count
- Destructor: Decrement ref count, delete if 0
- `use_count()`: Return ref count
- Thread-safe reference counting (atomic)

**Key Concept: Reference Counting**
```cpp
SharedPtr<int> p1(new int(42));  // ref_count = 1
{
    SharedPtr<int> p2 = p1;      // ref_count = 2
}                                // ref_count = 1
// p1 still valid, ref_count = 1
```

---

## Part 3: WeakPtr (Non-Owning Observer)
Implement `WeakPtr<T>` with:
- Constructor from `SharedPtr`
- `lock()`: Try to get `SharedPtr` (may fail if expired)
- `expired()`: Check if object was deleted
- Doesn't affect reference count!

**Breaking Circular References:**
```cpp
struct Node {
    SharedPtr<Node> next;  // Strong
    WeakPtr<Node> prev;    // Weak - breaks cycle!
};
```

---

## Part 4: make_shared
Implement `make_shared<T>(args...)`:
- Single allocation for object + control block
- Better cache locality
- Exception safe

---

## Part 5: Discussion
- Why are circular references bad?
- When to use UniquePtr vs SharedPtr?
- Thread safety of reference counting
- Custom deleters use cases
