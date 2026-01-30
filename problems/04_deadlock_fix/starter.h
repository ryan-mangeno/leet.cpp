#pragma once
#include <mutex>
#include <thread>
#include <chrono>

namespace leet_cpp {

struct Account {
    std::mutex m;
    int32_t balance{5000};

    Account(int32_t initial_balance) : balance(initial_balance) {}
};

class Bank {
public:
    // BROKEN FUNCTION: Causes Deadlock
    void transfer(Account& from, Account& to) {
        // TODO: Fix this using RAII and correct locking strategy
        
        from.m.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Force context switch to trigger deadlock
        to.m.lock();

        from.balance -= 5;
        to.balance += 5;

        from.m.unlock();
        to.m.unlock();
    }
};

}
