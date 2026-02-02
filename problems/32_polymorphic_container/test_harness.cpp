#include <test_framework.h>
#include "submission.h"
#include <iostream>
#include <variant>
#include <optional>
#include <map>
#include <vector>

using namespace leet_cpp;
using namespace test_framework;

// --- TESTS ---

TEST(Shape_PolymorphicCall) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    
    double total = 0;
    for (const auto& shape : shapes) {
        total += shape->area();
    }
    
    ASSERT_TRUE(total > 0);
}

TEST(Container_AddShapes) {
    ShapeContainer container;
    container.add(std::make_unique<Circle>(5.0));
    container.add(std::make_unique<Rectangle>(3.0, 4.0));
    
    ASSERT_EQ(container.size(), 2);
}

TEST(Container_TypeSafeRetrieval) {
    ShapeContainer container;
    container.add(std::make_unique<Circle>(5.0));
    container.add(std::make_unique<Rectangle>(3.0, 4.0));
    container.add(std::make_unique<Circle>(10.0));
    
    auto circles = container.get_all_of_type<Circle>();
    ASSERT_EQ(circles.size(), 2);
    
    auto rectangles = container.get_all_of_type<Rectangle>();
    ASSERT_EQ(rectangles.size(), 1);
}

TEST(Container_GetFirst) {
    ShapeContainer container;
    container.add(std::make_unique<Rectangle>(3.0, 4.0));
    container.add(std::make_unique<Circle>(5.0));
    
    auto* circle = container.get_first_of_type<Circle>();
    ASSERT_TRUE(circle != nullptr);
    ASSERT_EQ(circle->radius(), 5.0);
    
    container = ShapeContainer();
    auto* missing = container.get_first_of_type<Circle>();
    ASSERT_TRUE(missing == nullptr);
}

// --- Part 3: Visitor pattern tests ---
TEST(VisitorPattern_AcceptDispatch) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Circle>(2.0));

    AreaAccumulatingVisitor visitor;

    for (const auto& shape : shapes) {
        shape->accept(visitor);
    }

    ASSERT_TRUE(visitor.total > 0);
}

TEST(VisitorPattern_EqualsDynamicCast) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));

    double dynamic_total = 0;
    for (const auto& shape : shapes) {
        if (auto* c = dynamic_cast<Circle*>(shape.get())) {
            dynamic_total += c->area();
        } else if (auto* r = dynamic_cast<Rectangle*>(shape.get())) {
            dynamic_total += r->area();
        }
    }

    AreaAccumulatingVisitor visitor;
    for (const auto& shape : shapes) {
        shape->accept(visitor);
    }

    ASSERT_EQ(dynamic_total, visitor.total);
}

// --- Part 4: std::variant tests ---
TEST(Variant_BasicUsage) {
    std::vector<ShapeVariant> shapes;
    shapes.push_back(Circle{5.0});
    shapes.push_back(Rectangle{3.0, 4.0});
    
    double total = 0;
    for (const auto& shape : shapes) {
        AreaVisitor av;
        std::visit(av, shape);
        total += av.area;
    }
    
    ASSERT_TRUE(total > 0);
}

TEST(Variant_ExhaustiveDispatch) {
    std::vector<ShapeVariant> shapes;
    shapes.emplace_back(Circle{5.0});
    shapes.emplace_back(Rectangle{3.0, 4.0});
    shapes.emplace_back(Circle{2.0});

    double total = 0;
    for (const auto& shape : shapes) {
        AreaVisitor av;
        std::visit(av, shape);
        total += av.area;
    }

    ASSERT_TRUE(total > 0);
}

// --- C++17 feature tests ---
TEST(CPP17_StructuredBinding) {
    std::map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
    
    int sum = 0;
    for (const auto& [key, value] : data) {
        sum += value;
    }
    
    ASSERT_EQ(sum, 6);
}

TEST(CPP17_Optional) {
    std::optional<int> maybe_value = 42;
    ASSERT_TRUE(maybe_value.has_value());
    ASSERT_EQ(*maybe_value, 42);
    
    std::optional<int> empty;
    ASSERT_TRUE(!empty.has_value());
}

// --- Dynamic cast tests ---
TEST(DynamicCast_Basics) {
    std::unique_ptr<Shape> shape = std::make_unique<Circle>(5.0);
    
    Circle* circle = dynamic_cast<Circle*>(shape.get());
    ASSERT_TRUE(circle != nullptr);
    
    Rectangle* rect = dynamic_cast<Rectangle*>(shape.get());
    ASSERT_TRUE(rect == nullptr);
}

// --- Main ---
int main() {
    std::cout << "--- 📦 POLYMORPHIC CONTAINER TESTS ---\n" << std::endl;
    
    SECTION("Polymorphism Basics");
    SECTION("Type-Safe Retrieval");
    SECTION("Visitor Pattern");
    SECTION("std::variant Alternative");
    SECTION("C++17 Features");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Concepts:" << std::endl;
    std::cout << "   - Virtual functions & polymorphism" << std::endl;
    std::cout << "   - dynamic_cast for runtime type checking" << std::endl;
    std::cout << "   - Template member functions" << std::endl;
    std::cout << "   - std::variant (C++17)" << std::endl;
    std::cout << "   - Structured bindings (C++17)" << std::endl;
    std::cout << "   - std::optional (C++17)" << std::endl;
    
    std::cout << "\n🎯 Polymorphism vs std::variant:" << std::endl;
    std::cout << "   Polymorphism: Runtime, extensible, vtable overhead" << std::endl;
    std::cout << "   Variant: Compile-time, faster, closed set of types" << std::endl;
    
    return 0;
}
