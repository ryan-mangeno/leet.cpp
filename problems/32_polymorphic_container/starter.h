#ifndef SHAPE_CONTAINER_H
#define SHAPE_CONTAINER_H

#include <vector>
#include <memory>
#include <optional>
#include <variant>
#include <map>
#include <string>

namespace leet_cpp {

// Part 1: Base Shape
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void draw() const = 0;
    
    // Part 3: Visitor pattern
    // virtual void accept(ShapeVisitor& visitor) const = 0;
};

// Part 1: Concrete Shapes
class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    
    double area() const override {
        // TODO: Calculate circle area
        return 0.0;
    }
    
    void draw() const override {
        // TODO: Print or draw circle
    }
    
    double radius() const { return radius_; }
    
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) 
        : width_(width), height_(height) {}
    
    double area() const override {
        // TODO: Calculate rectangle area
        return 0.0;
    }
    
    void draw() const override {
        // TODO: Print or draw rectangle
    }
    
    double width() const { return width_; }
    double height() const { return height_; }
    
private:
    double width_;
    double height_;
};

// Part 2: Container with type-safe retrieval
class ShapeContainer {
public:
    void add(std::unique_ptr<Shape> shape) {
        // TODO: Add to shapes_
    }
    
    const std::vector<std::unique_ptr<Shape>>& all() const {
        return shapes_;
    }
    
    // Get all of specific type
    template<typename T>
    std::vector<T*> get_all_of_type() {
        std::vector<T*> result;
        // TODO: Use dynamic_cast to filter
        // for (auto& shape : shapes_) {
        //     if (T* ptr = dynamic_cast<T*>(shape.get())) {
        //         result.push_back(ptr);
        //     }
        // }
        return result;
    }
    
    // Get first of specific type
    template<typename T>
    T* get_first_of_type() {
        // TODO: Return first match or nullptr
        return nullptr;
    }
    
    size_t size() const { return shapes_.size(); }
    
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

// Part 3: Visitor Pattern
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(const Circle& c) = 0;
    virtual void visit(const Rectangle& r) = 0;
};

// TODO: Update Shape classes to support accept()

// Part 4: std::variant alternative
using ShapeVariant = std::variant<Circle, Rectangle>;

// Helper to calculate area with variant
struct AreaVisitor {
    // implement callable methods for circle and rectangle for std::visit to work
};

struct AreaAccumulatingVisitor {
    // implement callable methods for circle and rectangle for std::visit to work

    double total = 0.0;
};

} // namespace leet_cpp

#endif
