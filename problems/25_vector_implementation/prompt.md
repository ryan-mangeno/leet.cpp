### Implement a Vector Class (Templates & Dynamic Arrays)

**Learning Goals:**
Understand templates, dynamic memory management, capacity vs size, and exception safety.

---

## Part 1: Basic Dynamic Array
Implement `Vector<T>` with:
- Default constructor (empty vector)
- `push_back(value)`: Add element
- `size()`: Number of elements
- `capacity()`: Allocated space
- `operator[]`: Element access

**Key Concepts:**
- Template class: Works with any type T
- Capacity vs Size: `[x][x][x][_][_]` (size=3, capacity=5)
- Geometric growth: Double capacity when full
- Placement new: Construct objects in pre-allocated memory

---

## Part 2: Rule of Five for Templates
Add:
- Destructor: Call destructor for each element, free memory
- Copy constructor: Deep copy all elements
- Copy assignment: Handle self-assignment
- Move constructor: Steal pointer, leave other empty
- Move assignment: Swap or steal

**Why Templates Are Harder:**
```cpp
Vector<std::string> v;
v.push_back("hello");  // Must properly construct string
// Destructor must call ~string() for each element!
```

**Key Concepts:**
- Must explicitly call destructors: `data_[i].~T()`
- Use placement new: `new (&data_[i]) T(value)`
- Can't just `delete[]` - must destroy objects first!

---

## Part 3: Iterators & Range-Based For
Add:
- `begin()` and `end()` returning pointers
- Enable: `for (auto& x : vec) { ... }`

**Key Concepts:**
- Iterators are just pointers to elements
- `begin()` returns `&data_[0]`
- `end()` returns `&data_[size_]` (one past last)

---

## Part 4: Exception Safety
Add:
- `reserve(n)`: Pre-allocate capacity
- `pop_back()`: Remove last element
- Strong exception guarantee for `push_back`

**What If Constructors Throw?**
```cpp
Vector<Thing> v;
v.push_back(Thing());  // Constructor throws!
// Memory must not leak!
```

**Key Concepts:**
- RAII + scope guards for exception safety
- Copy-and-swap idiom
- Never leave object in invalid state

---

## Part 5: Advanced Topics
**Discussion:**
1. Why geometric growth (2x) instead of linear (+N)?
2. What is `emplace_back` and why is it faster?
3. How does `std::vector<bool>` cheat?
4. What's the performance of `insert(begin(), x)` and why?

**Memory Layout:**
```
Vector with capacity=8, size=5:
[obj][obj][obj][obj][obj][uninitialized][uninitialized][uninitialized]
 ↑                       ↑               ↑
data_               data_+size_    data_+capacity_
```
