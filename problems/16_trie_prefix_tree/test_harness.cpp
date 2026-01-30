#include <test_framework.h>
#include "submission.h"
#include <string>

using namespace leet_cpp;
using namespace test_framework;

// Part 1: Standard Insertion and Exact Matching
TEST(Trie_BasicSearch) {
    Trie trie;
    trie.insert("apple");
    
    ASSERT_TRUE(trie.search("apple"));
    ASSERT_TRUE(!trie.search("app"));  // Prefix exists, but word doesn't
    ASSERT_TRUE(!trie.search("apples")); // Super-word doesn't exist
}

// Part 2: Prefix Matching (startsWith)
TEST(Trie_PrefixMatching) {
    Trie trie;
    trie.insert("apple");
    
    ASSERT_TRUE(trie.startsWith("a"));
    ASSERT_TRUE(trie.startsWith("app"));
    ASSERT_TRUE(trie.startsWith("apple"));
    ASSERT_TRUE(!trie.startsWith("b"));
}



// Part 3: Word Overlapping Logic
TEST(Trie_OverlappingWords) {
    Trie trie;
    trie.insert("apple");
    
    // Test promoting a prefix to a full word
    ASSERT_TRUE(!trie.search("app")); 
    trie.insert("app");
    ASSERT_TRUE(trie.search("app"));
    
    // Ensure the original long word is still intact
    ASSERT_TRUE(trie.search("apple"));
}

// Part 4: Multiple Word Branching
TEST(Trie_Branching) {
    Trie trie;
    trie.insert("apple");
    trie.insert("apply");
    trie.insert("application");
    
    ASSERT_TRUE(trie.startsWith("appl"));
    ASSERT_TRUE(trie.search("apply"));
    ASSERT_TRUE(trie.search("application"));
    ASSERT_TRUE(!trie.search("appli")); // Intermediate node not marked as end
}



// Part 5: Edge Cases
TEST(Trie_EdgeCases) {
    Trie trie;
    
    // Empty String (if supported by your implementation)
    trie.insert("");
    ASSERT_TRUE(trie.search(""));
    
    // Single Character
    trie.insert("z");
    ASSERT_TRUE(trie.search("z"));
    ASSERT_TRUE(trie.startsWith("z"));
    
    // Non-existent Search
    ASSERT_TRUE(!trie.search("unknown"));
    ASSERT_TRUE(!trie.startsWith("unk"));
}

int main() {
    std::cout << "--- ⚡ TRIE (PREFIX TREE) VALIDATION ⚡ ---" << std::endl;
    
    SECTION("Exact Word Search");
    SECTION("Prefix Logic (startsWith)");
    SECTION("Word Overlap & Promotion");
    SECTION("Branching Integrity");
    SECTION("Boundary Conditions");
    
    RUN_ALL_TESTS();
    
    return 0;
}