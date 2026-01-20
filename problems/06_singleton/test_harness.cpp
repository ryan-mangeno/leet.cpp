#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include "submission.h"

using namespace leet_cpp;

std::vector<Singleton*> instances;
std::mutex m;

void worker() {
    // This requires the user to have implemented 'static Singleton& get()'
    Singleton& s = Singleton::get();
    
    std::lock_guard<std::mutex> lock(m);
    instances.push_back(&s);
}

int main() {
    std::cout << "--- SINGLETON CONCURRENCY TEST ---" << std::endl;
    
    const int THREADS = 100;
    std::vector<std::thread> pool;
    
    for(int i=0; i<THREADS; ++i) pool.emplace_back(worker);
    for(auto& t : pool) t.join();
    
    // Check if all instances are the same address
    Singleton* master = instances[0];
    for(auto* ptr : instances) {
        if(ptr != master) {
            std::cout << "❌ FAILED: Multiple instances detected!" << std::endl;
            exit(1);
        }
    }
    
    std::cout << "✅ PASSED: All threads accessed the same unique instance." << std::endl;
    return 0;
}
