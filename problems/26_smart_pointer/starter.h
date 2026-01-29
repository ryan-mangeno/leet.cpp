#ifndef SMART_POINTER_H
#define SMART_POINTER_H

#include <atomic>
#include <cstddef>
#include <utility>

namespace leet_cpp {

// Part 1: UniquePtr
template<typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}

    ~UniquePtr() {
        // TODO: Delete pointer
        delete ptr_;
    }

    // No copy!
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move only
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    T* release() {
        // TODO: Give up ownership, return pointer
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* ptr = nullptr) {
        // TODO: Delete current, take new
        delete ptr_;
        ptr_ = ptr;
    }

    explicit operator bool() const { return ptr_ != nullptr; }

private:
    T* ptr_;
};

// Part 2: SharedPtr Control Block
template<typename T>
struct ControlBlock {
    T* ptr;
    std::atomic<int> ref_count;
    
    ControlBlock(T* p) : ptr(p), ref_count(1) {}
    
    ~ControlBlock() {
        delete ptr;
    }
};

template<typename T>
class SharedPtr {
public:
    SharedPtr() : ptr_(nullptr), control_(nullptr) {}

    explicit SharedPtr(T* ptr) : ptr_(ptr) {
        if (ptr_) {
            // TODO: Allocate control block
            control_ = new ControlBlock<T>(ptr);
        } else {
            control_ = nullptr;
        }
    }

    ~SharedPtr() {
        // TODO: Decrement ref count, delete if 0
        release();
    }

    // Copy constructor
    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), control_(other.control_) {
        // TODO: Increment ref count
        if (control_) {
            control_->ref_count.fetch_add(1, std::memory_order_relaxed);
        }
    }

    // Copy assignment
    SharedPtr& operator=(const SharedPtr& other) {
        // TODO: Handle self-assignment
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            control_ = other.control_;
            if (control_) {
                control_->ref_count.fetch_add(1, std::memory_order_relaxed);
            }
        }
        return *this;
    }

    // Move constructor
    SharedPtr(SharedPtr&& other) noexcept 
        : ptr_(other.ptr_), control_(other.control_) {
        other.ptr_ = nullptr;
        other.control_ = nullptr;
    }

    // Move assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            control_ = other.control_;
            other.ptr_ = nullptr;
            other.control_ = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    int use_count() const {
        // TODO: Return reference count
        return control_ ? control_->ref_count.load(std::memory_order_relaxed) : 0;
    }

    void reset(T* ptr = nullptr) {
        // TODO: Release current, acquire new
        release();
        if (ptr) {
            ptr_ = ptr;
            control_ = new ControlBlock<T>(ptr);
        }
    }

    explicit operator bool() const { return ptr_ != nullptr; }

private:
    void release() {
        if (control_) {
            // TODO: Decrement, check if last reference
            if (control_->ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete control_;
            }
        }
        ptr_ = nullptr;
        control_ = nullptr;
    }

    T* ptr_;
    ControlBlock<T>* control_;
    
    template<typename U> friend class WeakPtr;
};

// Part 3: WeakPtr
template<typename T>
class WeakPtr {
public:
    WeakPtr() : ptr_(nullptr), control_(nullptr) {}

    WeakPtr(const SharedPtr<T>& shared) 
        : ptr_(shared.ptr_), control_(shared.control_) {
        // TODO: WeakPtr doesn't increment ref count!
    }

    SharedPtr<T> lock() const {
        // TODO: Try to create SharedPtr if object still alive
        // This is simplified - real implementation needs weak_count
        if (control_ && control_->ref_count.load(std::memory_order_relaxed) > 0) {
            SharedPtr<T> result;
            result.ptr_ = ptr_;
            result.control_ = control_;
            control_->ref_count.fetch_add(1, std::memory_order_relaxed);
            return result;
        }
        return SharedPtr<T>();
    }

    bool expired() const {
        // TODO: Check if object was deleted
        return !control_ || control_->ref_count.load(std::memory_order_relaxed) == 0;
    }

private:
    T* ptr_;
    ControlBlock<T>* control_;
};

// Part 4: make_shared
template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    // TODO: Allocate and construct object, wrap in SharedPtr
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

} // namespace leet_cpp

#endif
