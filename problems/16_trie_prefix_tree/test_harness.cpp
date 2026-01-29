#include <iostream>
#include "submission.h"

using namespace leet_cpp;

void log_result(const char* name, bool passed, const char* msg = "") {
    if (passed) {
        std::cout << "✅ [PASS] " << name << std::endl;
    } else {
        std::cout << "❌ [FAIL] " << name << ": " << msg << std::endl;
        exit(1);
    }
}

void Test_BasicInsertSearch() {
    Trie trie;
    trie.insert("apple");
    
    log_result("InsertSearch", trie.search("apple"), "Should find 'apple'");
    log_result("PartialWord", !trie.search("app"), "Should not find partial 'app'");
}

void Test_Prefix() {
    Trie trie;
    trie.insert("apple");
    
    log_result("HasPrefix", trie.startsWith("app"), "Should find prefix 'app'");
    log_result("NoPrefix", !trie.startsWith("b"), "Should not find prefix 'b'");
}

void Test_CompleteAfterPrefix() {
    Trie trie;
    trie.insert("apple");
    
    log_result("Before", !trie.search("app"), "Should not find 'app' initially");
    
    trie.insert("app");
    log_result("After", trie.search("app"), "Should find 'app' after insert");
    log_result("Original", trie.search("apple"), "Should still find 'apple'");
}

void Test_MultipleWords() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("apply");
    
    bool all = trie.search("apple") && trie.search("app") && 
               trie.search("application") && trie.search("apply");
    
    log_result("MultipleWords", all, "Should find all words");
}

void Test_EdgeCases() {
    Trie trie;
    trie.insert("");
    log_result("EmptyString", trie.search(""), "Should handle empty string");
    
    trie.insert("a");
    log_result("SingleChar", trie.search("a"), "Should handle single char");
    
    log_result("NotInserted", !trie.search("xyz"), "Should not find non-existent");
}

int main() {
    std::cout << "--- ⚡ TRIE TESTS ⚡ ---" << std::endl;
    Test_BasicInsertSearch();
    Test_Prefix();
    Test_CompleteAfterPrefix();
    Test_MultipleWords();
    Test_EdgeCases();
    std::cout << "--- 🏆 ALL TESTS PASSED 🏆 ---" << std::endl;
    return 0;
}
