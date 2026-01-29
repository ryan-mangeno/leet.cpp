#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
#include <list>

namespace leet_cpp {

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity) {
        // TODO
    }

    int get(int key) {
        // TODO: O(1) lookup and move to front
        return -1;
    }

    void put(int key, int value) {
        // TODO: O(1) insert/update and eviction
    }

private:
    int capacity_;
    // TODO: Add data structures
    // Hint: std::list<pair<int,int>> for order
    // Hint: std::unordered_map<int, list::iterator> for O(1) access
};

} // namespace leet_cpp

#endif
