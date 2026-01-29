#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include <stdexcept>

#include <test_framework.h>

using namespace test_framework;

namespace leet_cpp {

template<typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity) : buffer_(capacity), capacity_(capacity) {}

    void push(const T& item) {
        // TODO
    }

    size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }
    bool is_full() const { return count_ == capacity_; }

    T get_front() const {
        // TODO
    }

    T get_back() const {
        // TODO
    }

    void pop() {
        // TODO: 
    }

    const T& operator[](size_t index) const {
        // TODO: Handle circular indexing
        throw std::out_of_range("Index out of range");
    }

    std::vector<T> get_latest(size_t n) const {
        // TODO
        return {};
    }

private:
    std::vector<T> buffer_;
    int head_ = 0;
    size_t count_ = 0;
    size_t capacity_;
};

} // namespace leet_cpp

#endif
