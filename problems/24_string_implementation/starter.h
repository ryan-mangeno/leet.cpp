#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <test_framework.h>

using namespace test_framework;

namespace leet_cpp {

class String {
public:
    // Part 1: Basic constructor
    String(const char* s = "") {
        // TODO: Allocate memory, copy string
        // Handle nullptr case!
    }

    // Part 1: Destructor
    ~String() {
        // TODO: Clean up memory (RAII!)
    }

    // Part 2: Copy constructor
    String(const String& other) {
        // TODO: Deep copy
    }

    // Part 2: Copy assignment
    String& operator=(const String& other) {
        // TODO: Handle self-assignment!
        // TODO: Clean up old data, copy new data
        return *this;
    }

    // Part 3: Move constructor
    String(String&& other) noexcept {
        // TODO: Steal other's data, leave other in valid state
    }

    // Part 3: Move assignment
    String& operator=(String&& other) noexcept {
        // TODO: Swap or steal
        return *this;
    }

    // Part 1: Accessors
    const char* c_str() const {
        // TODO
        return nullptr;
    }

    size_t length() const {
        // TODO
        return 0;
    }

    // Part 4: Concatenation
    String operator+(const String& other) const {
        // TODO: Create new String with concatenated content
        return String("");
    }

    // Part 4: Comparison
    bool operator==(const String& other) const {
        // TODO
        return false;
    }

    // Part 4: Character access
    char& operator[](size_t index) {
        // TODO: Bounds checking?
        throw std::out_of_range("Index out of bounds");
    }

    const char& operator[](size_t index) const {
        // TODO
        throw std::out_of_range("Index out of bounds");
    }

    // Helper for debugging
    void print() const {
        std::cout << (data_ ? data_ : "null") << std::endl;
    }

private:
    char* data_;
    size_t length_;
};

} // namespace leet_cpp

#endif
