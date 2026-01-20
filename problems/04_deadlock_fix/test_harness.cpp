#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "submission.h"

using namespace leet_cpp;

int main() {
    std::cout << "--- DEADLOCK STRESS TEST ---" << std::endl;
    
    Account alice, bob;
    Bank bank;
    
    // We run two threads attempting conflicting transfers
    // Thread 1: Alice -> Bob
    // Thread 2: Bob -> Alice
    // If deadlock exists, this program will HANG indefinitely (Timeout).
    
    std::thread t1([&]() {
        for(int i=0; i<100; ++i) bank.transfer(alice, bob, 1);
    });
    
    std::thread t2([&]() {
        for(int i=0; i<100; ++i) bank.transfer(bob, alice, 1);
    });

    t1.join();
    t2.join();
    
    if (alice.balance + bob.balance == 2000) {
        std::cout << "✅ PASSED: Transfers completed without deadlock." << std::endl;
    } else {
        std::cout << "❌ FAILED: Balance corrupted (Race Condition)." << std::endl;
    }
    
    return 0;
}
