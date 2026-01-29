#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <vector>
#include <stdexcept>

namespace leet_cpp {

template<typename T>
class MemoryPool {
public:
    explicit MemoryPool(size_t pool_size) {
        // TODO: Allocate buffer, setup free list
    }

    ~MemoryPool() {
        // TODO: Clean up buffer
    }

    T* allocate() {
        // TODO: Pop from free list, return pointer
        return nullptr;
    }

    void deallocate(T* ptr) {
        // TODO: Push back to free list
    }

    size_t available() const {
        // TODO: Return free blocks count
        return 0;
    }

private:
    // TODO: char buffer, free list, etc.
};

} // namespace leet_cpp

#endif
