### Implement a String Class (Memory Management Deep Dive)

**Learning Goals:**
Understand RAII, the Rule of Five, memory management, and why C++ strings work the way they do.

---

## Part 1: Basic String with Manual Memory
Implement `String` class with:
- Constructor from C-string: `String(const char* s)`
- Destructor (cleanup!)
- `c_str()`: Return underlying C-string
- `length()`: Return length

**Key Concepts:**
- RAII: Constructor acquires, destructor releases
- Manual memory: `new[]` and `delete[]`
- Null terminator handling

**Example:**
```cpp
String s("hello");
std::cout << s.c_str();  // "hello"
```

---

## Part 2: Copy Constructor & Assignment (Rule of Three)
Add:
- Copy constructor: `String(const String& other)`
- Copy assignment: `String& operator=(const String& other)`

**Why This Matters:**
```cpp
String s1("hello");
String s2 = s1;       // Copy constructor
String s3("world");
s3 = s1;              // Copy assignment
```
Without these, you get double-delete crashes!

**Key Concepts:**
- Deep copy vs shallow copy
- Self-assignment check (`if (this == &other)`)
- Copy-and-swap idiom

---

## Part 3: Move Semantics (Rule of Five)
Add:
- Move constructor: `String(String&& other) noexcept`
- Move assignment: `String& operator=(String&& other) noexcept`

**Why This Matters:**
```cpp
String create() { return String("temp"); }
String s = create();  // Should move, not copy!
```

**Key Concepts:**
- Rvalue references (`&&`)
- `std::move` transfers ownership
- `noexcept` enables optimizations
- Leave moved-from object in valid state

---

## Part 4: Operators & Edge Cases
Add:
- `operator+`: String concatenation
- `operator==`: Comparison
- `operator[]`: Character access ... throw an `ExpectedException(error_message)` if out of bounds
- Empty string handling
- Null pointer handling

**Think About:**
- What should `String(nullptr)` do?
- How to handle `s[100]` when length is 5?
- Should `operator+` return by value or reference?

---

## Part 5: Performance & Small String Optimization
**Discussion Questions:**
1. Every copy allocates. How expensive is this?
2. What is Small String Optimization (SSO)?
3. When would you use `std::string_view` instead?
4. Copy-on-write vs eager copy trade-offs?

**Challenge:** 
Measure your String vs std::string performance.
