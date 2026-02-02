### Polymorphic Container with Type Safety

**Context:**
Build a container that stores different types polymorphically but provides type-safe access. Combines inheritance, templates, and C++17 features.

**Real-World Use:** Game entity systems, heterogeneous data storage, event systems.

---

## Part 1: Basic Polymorphic Storage (10 min)

Store different derived types in one container:

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void draw() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    double area() const override { return 3.14 * radius_ * radius_; }
    void draw() const override { /* ... */ }
};

class Rectangle : public Shape {
    double width_, height_;
    // ...
};

// Store different shapes
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(5.0));
shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
```

**Challenge:** Iterate and call polymorphic methods.

---

## Part 2: Type-Safe Retrieval (15 min)

Add ability to get specific types back:

```cpp
class ShapeContainer {
public:
    void add(std::unique_ptr<Shape> shape);
    
    // Get all shapes
    const std::vector<std::unique_ptr<Shape>>& all() const;
    
    // Get only circles (type-safe!)
    template<typename T>
    std::vector<T*> get_all_of_type() {
        // TODO: Filter by dynamic_cast
    }
    
    // Get first of type
    template<typename T>
    T* get_first_of_type() {
        // TODO: Return first matching type or nullptr
    }
};
```

**Usage:**
```cpp
ShapeContainer container;
container.add(std::make_unique<Circle>(5));
container.add(std::make_unique<Rectangle>(3, 4));
container.add(std::make_unique<Circle>(10));

auto circles = container.get_all_of_type<Circle>();
// Returns 2 circles
```

---

## Part 3: Visitor Pattern (15 min)

Process shapes without knowing their type:

```cpp
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(const Circle& c) = 0;
    virtual void visit(const Rectangle& r) = 0;
};

class Shape {
public:
    virtual void accept(ShapeVisitor& visitor) const = 0;
};

class Circle : public Shape {
    void accept(ShapeVisitor& visitor) const override {
        visitor.visit(*this);
    }
};
```

**Usage:**
```cpp
class AreaCalculator : public ShapeVisitor {
    double total_area = 0;
public:
    void visit(const Circle& c) override {
        total_area += c.area();
    }
    void visit(const Rectangle& r) override {
        total_area += r.area();
    }
    double get_total() const { return total_area; }
};

AreaCalculator calc;
for (auto& shape : shapes) {
    shape->accept(calc);
}
```

---

## Part 4: C++17 std::variant Alternative (15 min)

Compare polymorphism vs std::variant:

```cpp
// Option 1: Polymorphism (what we've been doing)
std::vector<std::unique_ptr<Shape>> shapes;

// Option 2: std::variant (C++17)
using ShapeVariant = std::variant<Circle, Rectangle, Triangle>;
std::vector<ShapeVariant> shapes_v;

shapes_v.push_back(Circle{5.0});
shapes_v.push_back(Rectangle{3.0, 4.0});

// Visit with lambda
for (auto& shape : shapes_v) {
    std::visit([](auto& s) { s.draw(); }, shape);
}
```

**Comparison:**

| Feature | Polymorphism | std::variant |
|---------|-------------|--------------|
| Extensibility | Easy (add new class) | Hard (must modify variant) |
| Performance | Virtual call overhead | No vtable, faster |
| Memory | Pointer indirection | Inline storage |
| Type safety | Runtime | Compile-time |

---

## Part 5: Structured Bindings & Optional (10 min)

Use C++17 features for cleaner code:

```cpp
// Structured bindings
std::map<std::string, std::unique_ptr<Shape>> named_shapes;

for (const auto& [name, shape] : named_shapes) {
    std::cout << name << ": " << shape->area() << '\n';
}

// Optional for safe retrieval
std::optional<Circle*> get_circle(const std::string& name) {
    auto it = named_shapes.find(name);
    if (it != named_shapes.end()) {
        if (auto* circle = dynamic_cast<Circle*>(it->second.get())) {
            return circle;
        }
    }
    return std::nullopt;
}

// Usage
if (auto circle = get_circle("my_circle")) {
    (*circle)->draw();
}
```

---

## Your Task:

1. Implement Shape hierarchy
2. Create ShapeContainer with type-safe retrieval
3. Implement Visitor pattern
4. Compare with std::variant approach
5. Use C++17 structured bindings and optional

**Interview Focus:**
- Virtual functions and vtables
- dynamic_cast for runtime type checking
- Template functions
- Visitor pattern
- std::variant (C++17)
- Structured bindings (C++17)
- std::optional (C++17)
