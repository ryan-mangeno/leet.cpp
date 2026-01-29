#ifndef LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_H

#include <atomic>
#include <memory>
#include <cstddef>
#include <cstdint>

namespace leet_cpp {

template <typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    // Tagged pointer to solve ABA problem
    struct TaggedPointer {
        Node* ptr;
        uintptr_t tag;  // Version counter
        
        TaggedPointer() : ptr(nullptr), tag(0) {}
        TaggedPointer(Node* p, uintptr_t t) : ptr(p), tag(t) {}
    };
    
    // Pack pointer and tag into single atomic uint64_t for lock-free CAS
    // On 64-bit: lower 48 bits = pointer, upper 16 bits = tag
    static constexpr uintptr_t PTR_MASK = 0x0000FFFFFFFFFFFF;
    static constexpr uintptr_t TAG_MASK = 0xFFFF000000000000;
    static constexpr int TAG_SHIFT = 48;
    
    static uint64_t pack(Node* ptr, uintptr_t tag) {
        return (reinterpret_cast<uintptr_t>(ptr) & PTR_MASK) | 
               ((tag << TAG_SHIFT) & TAG_MASK);
    }
    
    static TaggedPointer unpack(uint64_t packed) {
        return TaggedPointer(
            reinterpret_cast<Node*>(packed & PTR_MASK),
            (packed & TAG_MASK) >> TAG_SHIFT
        );
    }

public:
    LockFreeStack() {
        m_Head.store(0, std::memory_order_relaxed);
    }

    ~LockFreeStack() {
        // TODO: Clean up all remaining nodes (RAII)
        // Hint: Pop everything and delete nodes
    }

    // Delete copy/move
    LockFreeStack(const LockFreeStack&) = delete;
    LockFreeStack& operator=(const LockFreeStack&) = delete;
    LockFreeStack(LockFreeStack&&) = delete;
    LockFreeStack& operator=(LockFreeStack&&) = delete;

    void push(const T& item) {
        // TODO: Implement lock-free push
        // 1. Allocate new node
        // 2. Loop:
        //    a. Load current head (with acquire semantics)
        //    b. Set new node's next to current head
        //    c. Try CAS: if head hasn't changed, update to new node (increment tag!)
        //    d. Retry if CAS fails
        
        Node* new_node = new Node(item);
        
        while (true) {
            // TODO: Load head, unpack pointer and tag
            // TODO: Set new_node->next = old_head
            // TODO: CAS with incremented tag
            // TODO: Break on success, retry on failure
        }
    }

    bool pop(T& item) {
        // TODO: Implement lock-free pop
        // 1. Loop:
        //    a. Load current head
        //    b. If nullptr, return false (empty)
        //    c. Load next node
        //    d. Try CAS: if head unchanged, update to next (increment tag!)
        //    e. If CAS succeeds, copy data and delete old head
        //    f. Retry if CAS fails
        
        while (true) {
            // TODO: Load head, check if empty
            // TODO: Get next node
            // TODO: CAS to update head to next with incremented tag
            // TODO: If success, extract data, delete node, return true
            // TODO: Retry on failure
        }
        
        return false;
    }

    bool empty() const {
        // TODO: Load head and check if nullptr
        // Use memory_order_acquire for proper synchronization
        return true;
    }

private:
    alignas(64) std::atomic<uint64_t> m_Head;  // Packed pointer + tag
};

} // namespace leet_cpp

#endif // LOCK_FREE_STACK_H
