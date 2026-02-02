### Build a Process Scheduler Simulator

**Context:**
Implement different CPU scheduling algorithms. Understand how operating systems decide which process runs next.


---

## Part 1: Process Control Block (PCB) (10 min)

Define the process structure:

```cpp
enum class ProcessState {
    READY,      // Ready to run
    RUNNING,    // Currently executing
    BLOCKED,    // Waiting for I/O
    TERMINATED
};

struct Process {
    int pid;
    ProcessState state;
    int priority;           // 1-10 (10 = highest)
    int burst_time;         // CPU time needed
    int arrival_time;       // When process arrives
    int remaining_time;     // Time left to complete
    int wait_time;          // Time spent waiting
    int turnaround_time;    // Total time in system
};
```

**Questions:**
- What's the difference between wait time and turnaround time?
- Why separate READY and RUNNING states?
- How does priority inversion happen?

---

## Part 2: First-Come First-Served (FCFS) (15 min)

Implement FCFS scheduler:

```cpp
class Scheduler {
public:
    void add_process(const Process& p);
    void run();  // Simulate scheduling
    void print_stats();
    
private:
    std::queue<Process> ready_queue_;
    int current_time_;
};
```

**Algorithm:**
```
1. Processes run in order of arrival
2. No preemption - runs until completion
3. Simple but can cause "convoy effect"
```

**Convoy Effect:**
```
P1: burst=100ms (arrives t=0)
P2: burst=1ms   (arrives t=1)
P3: burst=1ms   (arrives t=2)

P2 and P3 wait 100ms for P1!
Average wait time: (0 + 100 + 101) / 3 = 67ms
```

---

## Part 3: Round Robin (RR) with Time Quantum (20 min)

Add preemptive scheduling:

```cpp
class RoundRobinScheduler {
public:
    RoundRobinScheduler(int time_quantum);
    void add_process(const Process& p);
    void run();
    
private:
    std::deque<Process> ready_queue_;
    int time_quantum_;  // How long each process runs
    int current_time_;
};
```

**Algorithm:**
```
1. Each process gets `time_quantum` ms
2. If not finished, move to back of queue
3. Preemptive - prevents starvation
```

**Time Quantum Trade-offs:**
```
Too small (1ms):  High context switch overhead
Too large (1s):   Becomes like FCFS
Typical: 10-100ms
```

**Example:**
```
time_quantum = 4ms
P1: burst=10ms
P2: burst=5ms
P3: burst=2ms

Timeline:
0-4:   P1 runs (6ms remaining)
4-8:   P2 runs (1ms remaining)
8-10:  P3 runs (done)
10-14: P1 runs (2ms remaining)
14-15: P2 runs (done)
15-17: P1 runs (done)
```

---

## Part 4: Priority Scheduling (20 min)

Implement priority-based scheduling:

```cpp
class PriorityScheduler {
public:
    void add_process(const Process& p);
    void run();
    
private:
    // Use priority queue: highest priority first
    std::priority_queue<Process, 
                       std::vector<Process>,
                       ProcessComparator> ready_queue_;
};
```

**Priority Inversion Problem:**
```
Low priority task L holds lock
High priority task H needs lock → BLOCKED
Medium priority task M runs (doesn't need lock)

H waits for L, but M prevents L from running!
Solution: Priority inheritance
```

**Aging:**
```cpp
// Prevent starvation: increase priority over time
void age_processes() {
    for (auto& p : ready_queue_) {
        if (p.wait_time > 100) {
            p.priority = std::min(10, p.priority + 1);
        }
    }
}
```

---

## Part 5: Multilevel Feedback Queue (25 min)

Most complex - used by Linux, macOS:

```cpp
class MLFQScheduler {
public:
    MLFQScheduler(std::vector<int> time_quanta);
    void add_process(const Process& p);
    void run();
    
private:
    std::vector<std::deque<Process>> queues_;  // Multiple priority levels
    std::vector<int> time_quanta_;             // Quantum per level
};
```

**Algorithm:**
```
Level 0: quantum=8ms  (highest priority, interactive)
Level 1: quantum=16ms
Level 2: quantum=32ms (lowest priority, CPU-bound)

New process → Level 0
Uses full quantum → demoted to Level 1
Uses full quantum → demoted to Level 2
I/O operation → promoted to Level 0 (reward interactive)
```

**Why This Works:**
- Interactive processes (user input): Short bursts → stay in high priority
- CPU-bound processes (number crunching): Long bursts → sink to low priority
- Automatically distinguishes I/O-bound from CPU-bound!

---

## Part 6: Real-World Considerations (Discussion)

**Context Switch Overhead:**
```cpp
void context_switch(Process& from, Process& to) {
    // Save state of 'from'
    // Load state of 'to'
    // Cost: ~1-10μs on modern CPUs
    // But: Cache misses, TLB flush = 100x worse!
}
```

**Metrics to Optimize:**
1. **Throughput**: Processes completed per second
2. **Turnaround time**: Submission to completion
3. **Response time**: First response to user
4. **Fairness**: All processes get fair share
5. **CPU utilization**: Keep CPU busy

**Real OS Schedulers:**

**Linux CFS (Completely Fair Scheduler):**
- Red-black tree sorted by virtual runtime
- O(log n) scheduling decision
- Targets fairness, not priority

**macOS/iOS:**
- Uses Mach kernel
- Multiple queues (real-time, timeshare, background)
- Quality of Service (QoS) levels

**Real-Time OS (RTOS):**
- Hard real-time: Must meet deadlines
- Rate Monotonic Scheduling (RMS)
- Earliest Deadline First (EDF)

**Interview Questions:**

**Q1:** Why can't FCFS be used for interactive systems?
**A:** No preemption → poor response time for short tasks behind long tasks

**Q2:** How does the OS prevent priority inversion?
**A:** Priority inheritance - temporarily boost low-priority task's priority

**Q3:** What's the difference between preemptive and cooperative multitasking?
**A:** Preemptive: OS forcibly switches. Cooperative: Process yields voluntarily.

**Q4:** Why do modern OSes use multilevel queues?
**A:** Automatically adapts to workload - interactive vs CPU-bound

**Q5:** What happens during a context switch?
**A:** Save registers, PC, stack pointer. Load new process state. Flush TLB/cache.

---

## Your Task:

Implement Parts 1-5. Understand trade-offs of each algorithm.

**Testing Focus:**
- Calculate average wait time, turnaround time
- Handle process arrival at different times
- Context switch counting
- Starvation prevention
- Priority inversion scenarios
