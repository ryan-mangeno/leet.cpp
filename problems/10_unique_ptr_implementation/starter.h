#pragma once
#include <utility>

template<typename T>
class UniquePtr {
public:
    // Constructor
    explicit UniquePtr(T* ptr = nullptr) {
        // TODO: implement
    }
    
    // Destructor
    ~UniquePtr() {
        // TODO: implement
    }
    
    // Delete copy operations
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept {
        // TODO: implement
    }
    
    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        // TODO: implement
        return *this;
    }
    
    // Dereference
    T& operator*() const {
        // TODO: implement
    }
    
    T* operator->() const {
        // TODO: implement
    }
    
    // Get raw pointer
    T* get() const {
        // TODO: implement
    }
    
    // Release ownership
    T* release() {
        // TODO: implement
    }
    
    // Reset to new pointer
    void reset(T* ptr = nullptr) {
        // TODO: implement
    }
    
    // Check if owns an object
    explicit operator bool() const {
        // TODO: implement
    }

private:
    T* ptr_;
};
