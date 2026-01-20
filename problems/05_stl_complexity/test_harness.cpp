#include <iostream>
#include <string>
#include "submission.h"

using namespace leet_cpp;

void check(std::string actual, std::string expected, const char* name) {
    if (actual == expected) {
        std::cout << "✅ " << name << ": Correct" << std::endl;
    } else {
        std::cout << "❌ " << name << ": Expected '" << expected << "', got '" << actual << "'" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "--- STL KNOWLEDGE CHECK ---" << std::endl;
    check(STLQuiz::vector_push_back_complexity(), "O(1)", "Vector Push Back");
    check(STLQuiz::map_search_complexity(), "O(log n)", "Map Search");
    check(STLQuiz::unordered_map_search_complexity(), "O(1)", "Unordered Map Search");
    check(STLQuiz::vector_growth_behavior(), "reallocation", "Vector Growth");
    return 0;
}
