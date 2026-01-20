#pragma once
#include <iostream>
#include <string>

namespace leet_cpp {

class Base {
public:
    // TODO: Fix destructor
    ~Base() { std::cout << "Base Destructed\n"; }
    
    // TODO: Fix polymorphism
    std::string getName() { return "Base"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived Destructed\n"; }
    
    std::string getName() { return "Derived"; }
};

}
