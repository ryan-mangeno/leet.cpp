### 3D Vector Math Library

**Context:**
You're building a component for a navigation system that needs efficient 3D vector operations.

**Part 1: Basic Implementation**
Implement a `Vector3D` class with:
- Constructor: `Vector3D(x, y, z)`
- Member access: `x()`, `y()`, `z()`
- Basic operations: `+`, `-`, `*` (scalar multiplication)

**Part 2: Advanced Operations**
Add these methods:
- `dot(other)`: Dot product
- `cross(other)`: Cross product  
- `magnitude()`: Length of vector
- `normalize()`: Return unit vector (magnitude = 1)

**Part 3: Edge Cases & Performance**
- Handle division by zero in normalize()
- Make operations `const` where appropriate
- What would you change if processing 1M vectors per second?

**Example:**
```cpp
Vector3D v1(1, 0, 0);
Vector3D v2(0, 1, 0);
Vector3D cross = v1.cross(v2);  // (0, 0, 1)
double dot = v1.dot(v2);         // 0
```
