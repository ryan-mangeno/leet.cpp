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
        if ( capacity < 2 || (capacity & (capacity-1)) != 0) {
            throw std::invalid_argument("Capacity < 2 or not a power of 2");
        }
        m_Capacity = capacity;
        m_Mask = m_Capacity - 1;
        m_Buffer.reserve(m_Capacity);
        m_Head.store(0, std::memory_order_relaxed);
        m_Tail.store(0, std::memory_order_relaxed);
    }

    ~SPSCQ() {
        // TODO: Clean up remaining items (RAII)
    }

    SPSCQ(const SPSCQ& other) = delete;
    SPSCQ(SPSCQ&& other) = delete;
    SPSCQ& operator=(const SPSCQ&) = delete;
    SPSCQ& operator=(SPSCQ&&) = delete;

    bool push(const T& item) {
        size_t current_head = m_Head.load(std::memory_order_relaxed);
        size_t current_tail = m_Tail.load(std::memory_order_acquire);
    
        if (current_head - current_tail >= m_Capacity) {
            return false;
        }
        
        m_Buffer[current_head & m_Mask] = item;
        m_Head.store(current_head + 1, std::memory_order_release);
        
        return true;
    }

    bool pop(T& item) {
        size_t current_head = m_Head.load(std::memory_order_acquire);
        size_t current_tail = m_Tail.load(std::memory_order_relaxed);
        
        if (current_head == current_tail) {
            return false;
        }
        
        item = std::move(m_Buffer[current_tail & m_Mask]);
        
        m_Tail.store(current_tail + 1, std::memory_order_release);
        
        return true;
    }

    [[nodiscard]] size_t size() const {
        size_t current_head = m_Head.load(std::memory_order_acquire);
        size_t current_tail = m_Tail.load(std::memory_order_acquire);
        return current_head - current_tail;
    }

    [[nodiscard]] bool empty() const {
        size_t current_head = m_Head.load(std::memory_order_acquire);
        size_t current_tail = m_Tail.load(std::memory_order_acquire);
        return current_head == current_tail; // TODO
    }
    
    [[nodiscard]] bool full() const {
        size_t current_head = m_Head.load(std::memory_order_acquire);
        size_t current_tail = m_Tail.load(std::memory_order_acquire);
        return current_head - current_tail == m_Capacity; // TODO
    }

private:
    std::vector<T> m_Buffer;

    size_t m_Capacity;
    size_t m_Mask;
    
    alignas(64) std::atomic<size_t> m_Head;
    alignas(64) std::atomic<size_t> m_Tail;
};

} // namespace leet_cpp

#endif // SPSCQ_H
