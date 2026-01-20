#pragma once
#include <mutex>
#include <thread>
#include <chrono>

namespace leet_cpp {

struct Account {
    std::mutex m;
    float balance{5000.0f};
};

class Bank {
public:
    // BROKEN FUNCTION: Causes Deadlock
    void transfer(Account& from, Account& to) {
        // TODO: Fix this using RAII and correct locking strategy
        std::scoped_lock s(from.m, to.m);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Force context switch to trigger deadlock
        
        float half = from.balance/2.0f;
        from.balance -= half;
        to.balance += half; 
    }
};

}
