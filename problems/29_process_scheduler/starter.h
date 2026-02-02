#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <deque>
#include <vector>
#include <iostream>
#include <algorithm>

namespace leet_cpp {

enum class ProcessState {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

struct Process {
    int pid;
    ProcessState state;
    int priority;           // 1-10 (10 highest)
    int burst_time;         // Total CPU time needed
    int arrival_time;       // When process arrives
    int remaining_time;     // Time left
    int wait_time;          // Time spent waiting
    int turnaround_time;    // Total time in system
    int start_time;         // When first executed
    
    Process(int id, int burst, int arrival, int prio = 5)
        : pid(id), state(ProcessState::READY), priority(prio),
          burst_time(burst), arrival_time(arrival),
          remaining_time(burst), wait_time(0), 
          turnaround_time(0), start_time(-1) {}
};

// Part 2: FCFS Scheduler
class FCFSScheduler {
public:
    void add_process(const Process& p) {
    }

    Process& get_process(int idx) { 
    }

    
    void run() {
        // TODO: Sort by arrival time
        // TODO: Run each process to completion
        // TODO: Calculate wait times and turnaround times
        
    }
    
    void print_stats() {
        // implement average wait time and turnaround time calculation

        std::cout << "FCFS Statistics:" << std::endl;
        std::cout << "  Average Wait Time: " << avg_wait << "ms" << std::endl;
        std::cout << "  Average Turnaround: " << avg_turnaround << "ms" << std::endl;
    }
    
private:
    std::vector<Process> processes_;
};

// Part 3: Round Robin Scheduler
class RoundRobinScheduler {
public:
    explicit RoundRobinScheduler(int quantum) : time_quantum_(quantum), current_time_(0) {}
    
    Process& get_process(int idx) { 
    }

    void add_process(const Process& p) {
    }
    
    void run() {
        // TODO: Implement round robin
        // Use deque for ready queue
        // Each process gets time_quantum_
        // If not done, back to queue
    }
    
    void print_stats() {
        // implement average wait time and turnaround time calculation
        
        std::cout << "Round Robin (quantum=" << time_quantum_ << ") Statistics:" << std::endl;
        std::cout << "  Average Wait Time: " << avg_wait << "ms" << std::endl;
        std::cout << "  Average Turnaround: " << avg_turnaround << "ms" << std::endl;
    }
    
private:
    std::vector<Process> processes_;
    int time_quantum_;
    int current_time_;
};

// Part 4: Priority Scheduler
struct PriorityComparator {
    bool operator()(const Process* a, const Process* b) const {
        // Higher priority value = higher priority
        // priority_queue is max-heap, so this is correct
    }
};

class PriorityScheduler {
public:

    Process& get_process(int idx) { 
    }


    void add_process(const Process& p) {
    }
    
    void run() {
        // TODO: Implement priority scheduling
        // Use priority queue
        // Handle priority inversion with aging
    }
    
    void print_stats() {
        // TODO: Similar to above
    }
    
private:
    std::vector<Process> processes_;
    int current_time_ = 0;
};

// Part 5: Multilevel Feedback Queue
class MLFQScheduler {
public:
    explicit MLFQScheduler(std::vector<int> quanta) 
        : time_quanta_(quanta), current_time_(0) {
    }

    Process& get_process(int idx) { 
    }

    
    void add_process(const Process& p) {
    }
    
    void run() {
        // TODO: Implement MLFQ
        // New processes start at queue 0 (highest priority)
        // If use full quantum, demote to next queue
        // Service queues in priority order
    }
    
    void print_stats() {
        // TODO
    }
    
private:
    std::vector<Process> processes_;
    std::vector<std::deque<Process*>> queues_;
    std::vector<int> time_quanta_;
    int current_time_;
};

} // namespace leet_cpp

#endif
