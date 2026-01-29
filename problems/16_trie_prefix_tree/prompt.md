### Implement a Trie (Prefix Tree)

**Your Task**
Build a trie data structure for efficient string operations.

**Requirements:**
- `insert(word)`: Add word to trie
- `search(word)`: Return true if exact word exists
- `startsWith(prefix)`: Return true if any word has this prefix
- Time complexity: O(L) where L = word/prefix length

**Example:**
```cpp
Trie trie;
trie.insert("apple");
trie.search("apple");   // true
trie.search("app");     // false (not a complete word)
trie.startsWith("app"); // true (prefix exists)
trie.insert("app");
trie.search("app");     // true (now complete)
```

**Use Cases:** Autocomplete, spell check, IP routing, dictionary search
