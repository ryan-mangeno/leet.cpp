#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <vector>

// Statistics for a named timer
struct TimerStats {
    size_t count = 0;
    double total_ms = 0.0;
    double min_ms = 1e9;
    double max_ms = 0.0;
    
    void update(double ms) {
        // TODO: implement
    }
    
    double average() const {
        // TODO: implement
    }
};

// Global registry for timer statistics
class TimerRegistry {
public:
    static TimerRegistry& instance() {
        static TimerRegistry reg;
        return reg;
    }
    
    void record(const std::string& name, double ms) {
        // TODO: implement - update stats for this timer name
    }
    
    void printReport() const {
        // TODO: implement - print all accumulated stats
    }
    
private:
    // TODO: add member to store stats by name
};

// RAII timer class
class ScopedTimer {
public:
    ScopedTimer(const std::string& name, bool accumulate = false) 
        : name_(name), accumulate_(accumulate) {
        // TODO: start timing
    }
    
    ~ScopedTimer() {
        // TODO: stop timing and report/record
    }
    
    // Prevent copying
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

private:
    std::string name_;
    bool accumulate_;
    std::chrono::high_resolution_clock::time_point start_;
    
    static int depth_;  // For nested indentation
};
