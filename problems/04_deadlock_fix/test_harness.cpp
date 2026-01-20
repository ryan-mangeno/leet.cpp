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
        for(int i=0; i<500; ++i) bank.transfer(alice, bob);
    });
    
    std::thread t2([&]() {
        for(int i=0; i<500; ++i) bank.transfer(bob, alice);
    });

    std::thread t3([&]() {
        for(int i=0; i<500; ++i) bank.transfer(alice, bob);
    });
    
    std::thread t4([&]() {
        for(int i=0; i<500; ++i) bank.transfer(bob, alice);
    });

    std::thread t5([&]() {
        for(int i=0; i<500; ++i) bank.transfer(alice, bob);
    });
    
    std::thread t6([&]() {
        for(int i=0; i<500; ++i) bank.transfer(bob, alice);
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    int expected = 10000;
    int observed = alice.balance + bob.balance;

    if (expected == observed) {
        std::cout << "✅ PASSED: Transfers completed without deadlock." << std::endl;
    } else {
        std::cout << "❌ FAILED: Balance corrupted (Race Condition)\n" << "Expected: " << expected << " Observed: " << observed << std::endl;
    }
    
    return 0;
}
