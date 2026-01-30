#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include <test_framework.h>
using namespace test_framework; // to throw ExpectedException

namespace leet_cpp {

template<typename T>
class Vector {
public:
    // Part 1: Basic constructor
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    // Part 1: Constructor with size
    explicit Vector(size_t n) {
        // TODO: Allocate for n elements, default construct them
    }

    // Part 2: Destructor
    ~Vector() {
        // TODO: CRITICAL - Must call destructor for each element!
        // Then free memory
    }

    // Part 2: Copy constructor
    Vector(const Vector& other) {
        // TODO: Allocate new memory, copy construct each element
    }

    // Part 2: Copy assignment
    Vector& operator=(const Vector& other) {
        // TODO: Self-assignment check!
        // Destroy old elements, copy new ones
        return *this;
    }

    // Part 2: Move constructor
    Vector(Vector&& other) noexcept {
        // TODO: Steal other's data
    }

    // Part 2: Move assignment  
    Vector& operator=(Vector&& other) noexcept {
        // TODO: Swap or steal
        return *this;
    }

    // Part 1: Add element
    void push_back(const T& value) {
        // TODO: Check capacity, grow if needed, construct element
        // Hint: if (size_ == capacity_) grow();
    }

    // Part 4: Remove last element
    void pop_back() {
        // TODO: Check not empty, destroy last element, decrement size
    }

    // Part 4: Reserve capacity
    void reserve(size_t n) {
        // TODO: If n > capacity_, allocate new memory and move elements
    }

    // Part 1: Access
    T& operator[](size_t index) {
        // TODO: No bounds checking (like std::vector)
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& at(size_t index) {
        // TODO: WITH bounds checking
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    // Part 1: Queries
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    // Part 3: Iterators
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

private:
    void grow() {
        // TODO: Double capacity (or start with 1)
        // Allocate new memory, move/copy elements, free old
        size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reserve(new_capacity);
    }

    T* data_;
    size_t size_;
    size_t capacity_;
};

} // namespace leet_cpp

#endif
