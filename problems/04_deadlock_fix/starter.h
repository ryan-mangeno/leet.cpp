#pragma once
#include <mutex>
#include <thread>
#include <chrono>

namespace leet_cpp {

struct Account {
    std::mutex m;
    int balance{1000};
};

class Bank {
public:
    // BROKEN FUNCTION: Causes Deadlock
    void transfer(Account& from, Account& to, int amount) {
        // TODO: Fix this using RAII and correct locking strategy
        
        from.m.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Force context switch to trigger deadlock
        to.m.lock();

        from.balance -= amount;
        to.balance += amount;

        from.m.unlock();
        to.m.unlock();
    }
};

}
