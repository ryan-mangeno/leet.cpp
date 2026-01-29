#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <vector>
#include <random>
#include <limits>

namespace leet_cpp {

class SkipList {
public:
    SkipList() {
        // TODO: Initialize with head node
    }

    bool search(int target) {
        // TODO: Start from top level, go down and right
        return false;
    }

    void insert(int num) {
        // TODO: Find position, random level, insert
    }

    bool erase(int num) {
        // TODO: Find and remove from all levels
        return false;
    }

private:
    struct Node {
        int val;
        std::vector<Node*> forward;
        
        Node(int v, int level) : val(v), forward(level, nullptr) {}
    };

    static constexpr int MAX_LEVEL = 16;
    Node* head_;
    int current_level_;
    std::mt19937 gen_;
    
    int randomLevel() {
        // TODO: Random level generator (coin flip)
        return 1;
    }
};

} // namespace leet_cpp

#endif
