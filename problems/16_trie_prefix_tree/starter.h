#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <memory>
#include <array>

namespace leet_cpp {

class Trie {
public:
    Trie() {
        // TODO: Initialize root
    }

    void insert(const std::string& word) {
        // TODO: Insert character by character
    }

    bool search(const std::string& word) const {
        // TODO: Search and check if word ends here
        return false;
    }

    bool startsWith(const std::string& prefix) const {
        // TODO: Search prefix only
        return false;
    }

private:
    struct TrieNode {
        // TODO: Array of 26 children (a-z)
        // TODO: bool isEndOfWord
    };
    
    // TODO: Add root node
};

} // namespace leet_cpp

#endif
