#include <iostream>
#include <cstring>
#include <string>
#include "submission.h"

using namespace leet_cpp;

void check(const char* input, const char* expected) {
    char buffer[100];
    if (input) strcpy(buffer, input);
    else buffer[0] = '\0'; // Handle null case simulation
    
    // Run user function
    reverse(input ? buffer : nullptr);
    
    if (input == nullptr) {
        // If we didn't crash on nullptr, it's a pass for that case
        std::cout << "✅ Nullptr check: Safe" << std::endl;
        return;
    }

    if (strcmp(buffer, expected) == 0) {
        std::cout << "✅ '" << input << "' -> '" << buffer << "'" << std::endl;
    } else {
        std::cout << "❌ FAILED: Input '" << input << "' became '" << buffer << "', expected '" << expected << "'" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "--- REVERSE STRING TEST ---" << std::endl;
    check("hello", "olleh");
    check("racecar", "racecar");
    check("1234", "4321");
    check("", "");
    check(nullptr, ""); // Should simply return
    return 0;
}
