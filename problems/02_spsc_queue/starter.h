#ifndef SPSCQ_H
#define SPSCQ_H

#include <atomic>
#include <vector>
#include <stdexcept>
#include <cstddef>

namespace leet_cpp {

template <typename T>
class SPSCQ {
public:
    explicit SPSCQ(size_t capacity) {
        // TODO: Validate capacity is power of 2 and >= 2
        // TODO: Initialize your buffer and atomic indices
    }

    ~SPSCQ() {
        // TODO: Clean up remaining items (RAII)
    }

    SPSCQ(const SPSCQ&) = delete;
    SPSCQ& operator=(const SPSCQ&) = delete;
    SPSCQ(SPSCQ&&) = delete;
    SPSCQ& operator=(SPSCQ&&) = delete;

    bool push(const T& item) {
        return false; // TODO
    }

    bool pop(T& item) {
        return false; // TODO
    }

    [[nodiscard]] size_t size() const {
        return 0; // TODO
    }

    [[nodiscard]] bool empty() const {
        return true; // TODO
    }
    
    [[nodiscard]] bool full() const {
        return false; // TODO
    }

private:
    // TODO: Add private members. 
    // Hint: alignas(64) on atomic indices avoids false sharing.
};

} // namespace leet_cpp

#endif // SPSCQ_H
