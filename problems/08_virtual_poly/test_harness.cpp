#include <iostream>
#include <memory>
#include "submission.h"

using namespace leet_cpp;

int main() {
    std::cout << "--- POLYMORPHISM TEST ---" << std::endl;

    // Test 1: Dynamic Dispatch
    Base* poly = new Derived();
    std::string name = poly->getName();
    
    if (name == "Derived") {
        std::cout << "✅ Dynamic Dispatch Working (Virtual Function correct)" << std::endl;
    } else {
        std::cout << "❌ FAILED: getName() returned '" << name << "'. Expected 'Derived'. Did you forget 'virtual'?" << std::endl;
        delete poly; // Cleanup manually to be safe
        exit(1);
    }

    // Test 2: Virtual Destructor
    // We redirect cout to check if "Derived Destructed" is printed
    std::streambuf* oldCout = std::cout.rdbuf();
    std::stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());

    delete poly; // Should trigger Derived destructor first, then Base

    std::cout.rdbuf(oldCout); // Restore cout
    
    std::string output = buffer.str();
    if (output.find("Derived Destructed") != std::string::npos) {
        std::cout << "✅ Virtual Destructor Working (Memory Leak prevented)" << std::endl;
    } else {
        std::cout << "❌ FAILED: Derived destructor NOT called. Potential Memory Leak." << std::endl;
        exit(1);
    }

    return 0;
}
