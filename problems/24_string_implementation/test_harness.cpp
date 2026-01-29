#include <test_framework.h>
#include "submission.h"
#include <cstring>

using namespace leet_cpp;
using namespace test_framework;

// Part 1 Tests
TEST(Part1_BasicConstruction) {
    String s("hello");
    ASSERT_EQ(s.length(), 5);
    ASSERT_TRUE(strcmp(s.c_str(), "hello") == 0);
}

TEST(Part1_EmptyString) {
    String s("");
    ASSERT_EQ(s.length(), 0);
}

TEST(Part1_NullptrHandling) {
    String s(nullptr);
    ASSERT_EQ(s.length(), 0);
}

// Part 2 Tests
TEST(Part2_CopyConstructor) {
    String s1("hello");
    String s2(s1);
    ASSERT_TRUE(strcmp(s1.c_str(), s2.c_str()) == 0);
    ASSERT_TRUE(s1.c_str() != s2.c_str());  // Different memory!
}

TEST(Part2_CopyAssignment) {
    String s1("hello");
    String s2("world");
    s2 = s1;
    ASSERT_TRUE(strcmp(s2.c_str(), "hello") == 0);
}

TEST(Part2_SelfAssignment) {
    String s1("hello");
    s1 = s1;  // Should not crash!
    ASSERT_TRUE(strcmp(s1.c_str(), "hello") == 0);
}

TEST(Part2_DeepCopy) {
    String s1("original");
    String s2(s1);
    String s3("modified");
    s1 = s3;
    ASSERT_TRUE(strcmp(s2.c_str(), "original") == 0);  // s2 unchanged!
}

// Part 3 Tests
TEST(Part3_MoveConstructor) {
    String s1("hello");
    const char* original_ptr = s1.c_str();
    String s2(std::move(s1));
    ASSERT_EQ(s2.c_str(), original_ptr);  // Stole the pointer!
    ASSERT_EQ(s1.length(), 0);  // s1 is now empty (valid state)
}

TEST(Part3_MoveAssignment) {
    String s1("hello");
    String s2("world");
    const char* s1_ptr = s1.c_str();
    s2 = std::move(s1);
    ASSERT_EQ(s2.c_str(), s1_ptr);
}

TEST(Part3_MoveEfficiency) {
    // Move should be O(1), not O(n) like copy
    String large(nullptr);
    // In real impl, this would be huge
    String moved(std::move(large));
    ASSERT_TRUE(true);  // Just checking it compiles
}

// Part 4 Tests
TEST(Part4_Concatenation) {
    String s1("hello");
    String s2(" world");
    String s3 = s1 + s2;
    ASSERT_TRUE(strcmp(s3.c_str(), "hello world") == 0);
}

TEST(Part4_Equality) {
    String s1("hello");
    String s2("hello");
    String s3("world");
    ASSERT_TRUE(s1 == s2);
    ASSERT_TRUE(!(s1 == s3));
}

TEST(Part4_IndexAccess) {
    String s("hello");
    ASSERT_EQ(s[0], 'h');
    ASSERT_EQ(s[4], 'o');
}

TEST(Part4_IndexModify) {
    String s("hello");
    s[0] = 'H';
    ASSERT_EQ(s[0], 'H');
}

TEST(Part4_IndexOutOfBounds) {
    String s("hi");
    ASSERT_THROWS([&](){ s[10]; });
}

// Edge Cases
TEST(EdgeCase_ChainedAssignment) {
    String s1("a");
    String s2("b");
    String s3("c");
    s3 = s2 = s1;  // Should work
    ASSERT_TRUE(s3 == s1);
}

TEST(EdgeCase_EmptyConcatenation) {
    String s1("");
    String s2("hello");
    String s3 = s1 + s2;
    ASSERT_TRUE(strcmp(s3.c_str(), "hello") == 0);
}

// Memory Test
TEST(Memory_NoLeaks) {
    // Create and destroy many strings
    for (int i = 0; i < 1000; i++) {
        String s("test");
        String s2(s);
        String s3 = s + s2;
    }
    ASSERT_TRUE(true);  // Run with valgrind to verify
}

int main() {
    std::cout << "--- 📚 STRING IMPLEMENTATION TESTS ---\n" << std::endl;
    
    SECTION("Part 1: Basic Construction & RAII");
    SECTION("Part 2: Copy Semantics (Rule of Three)");
    SECTION("Part 3: Move Semantics (Rule of Five)");
    SECTION("Part 4: Operators & Functionality");
    SECTION("Edge Cases & Memory");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Learnings:" << std::endl;
    std::cout << "   1. RAII: Constructor allocates, destructor cleans up" << std::endl;
    std::cout << "   2. Rule of Five: destructor, copy ctor, copy assign, move ctor, move assign" << std::endl;
    std::cout << "   3. Move semantics: Transfer ownership, don't copy" << std::endl;
    std::cout << "   4. Self-assignment check prevents crashes" << std::endl;
    std::cout << "\n🔍 Run with: valgrind ./runner (to check for leaks)" << std::endl;
    
    return 0;
}
