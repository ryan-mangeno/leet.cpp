#include <test_framework.h>
#include <thread>
#include <vector>
#include <numeric>
#include "submission.h"

using namespace leet_cpp;
using namespace test_framework;

// Helper for the threads
void transfer_worker(Bank& bank, Account& a, Account& b, int count) {
    for (int i = 0; i < count; ++i) {
        bank.transfer(a, b);
    }
}

TEST(Bank_Deadlock_Safety) {
    Account alice(5000), bob(5000);
    Bank bank;
    int iterations = 1000;

    // We use a high number of threads and iterations to increase the probability 
    // of an interleaved execution that causes a deadlock.
    

    std::thread t1(transfer_worker, std::ref(bank), std::ref(alice), std::ref(bob), iterations);
    std::thread t2(transfer_worker, std::ref(bank), std::ref(bob), std::ref(alice), iterations);
    std::thread t3(transfer_worker, std::ref(bank), std::ref(alice), std::ref(bob), iterations);
    std::thread t4(transfer_worker, std::ref(bank), std::ref(bob), std::ref(alice), iterations);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // If we reach here, no deadlock occurred.
    // Now check for Race Conditions (Atomicity)
    ASSERT_EQ(alice.balance + bob.balance, 10000);
}

TEST(Bank_LargeScale_Consistency) {
    const int num_accounts = 10;
    std::vector<Account*> accounts;
    for(int i = 0; i < num_accounts; ++i) {
        accounts.push_back(new Account(1000));
    }
    
    Bank bank;
    std::vector<std::thread> workers;

    for (int i = 0; i < 8; ++i) {
        workers.emplace_back([&]() {
            for (int j = 0; j < 500; ++j) {
                int idx1 = rand() % num_accounts;
                int idx2 = rand() % num_accounts;
                if (idx1 != idx2) {
                    bank.transfer(*accounts[idx1], *accounts[idx2]);
                }
            }
        });
    }

    for (auto& t : workers) t.join();

    int total_balance = 0;
    for (auto* acc : accounts) {
        total_balance += acc->balance;
        delete acc;
    }

    // Verify total money is conserved
    ASSERT_EQ(total_balance, 10000);
}

int main() {
    std::cout << "--- 🏦 BANKING CONCURRENCY CHALLENGE ---" << std::endl;
    
    SECTION("Deadlock & Circular Wait Stress");
    SECTION("Multi-Account Atomicity");

    RUN_ALL_TESTS();
    
    return 0;
}