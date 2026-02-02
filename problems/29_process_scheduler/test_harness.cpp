#include "submission.h"
#include <test_framework.h>
#include <cmath> // for floating point comparison

using namespace leet_cpp;
using namespace test_framework;

// Utility to compare doubles
bool approx_equal(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

// Part 2: FCFS Tests
TEST(FCFS_SimpleCase) {
    FCFSScheduler sched;
    sched.add_process(Process(1, 10, 0));
    sched.add_process(Process(2, 5, 0));
    sched.add_process(Process(3, 2, 0));

    sched.run();
    sched.print_stats();

    // Verify wait times
    ASSERT_EQ(sched.get_process(0).wait_time, 0);
    ASSERT_EQ(sched.get_process(1).wait_time, 10);
    ASSERT_EQ(sched.get_process(2).wait_time, 15);

    // Verify turnaround times
    ASSERT_EQ(sched.get_process(0).turnaround_time, 10);
    ASSERT_EQ(sched.get_process(1).turnaround_time, 15);
    ASSERT_EQ(sched.get_process(2).turnaround_time, 17);
}

TEST(FCFS_DifferentArrivals) {
    FCFSScheduler sched;
    sched.add_process(Process(1, 10, 0));
    sched.add_process(Process(2, 5, 5));
    sched.add_process(Process(3, 2, 10));

    sched.run();

    // Expected wait times
    ASSERT_EQ(sched.get_process(0).wait_time, 0);   // arrives at 0
    ASSERT_EQ(sched.get_process(1).wait_time, 5);   // arrives at 5, starts at 10
    ASSERT_EQ(sched.get_process(2).wait_time, 5);   // arrives at 10, starts at 15

    // Turnaround times
    ASSERT_EQ(sched.get_process(0).turnaround_time, 10);
    ASSERT_EQ(sched.get_process(1).turnaround_time, 10);
    ASSERT_EQ(sched.get_process(2).turnaround_time, 7);
}

// Part 3: Round Robin Tests
TEST(RoundRobin_BasicScheduling) {
    RoundRobinScheduler sched(4);
    sched.add_process(Process(1, 10, 0));
    sched.add_process(Process(2, 5, 0));
    sched.add_process(Process(3, 2, 0));

    sched.run();
    sched.print_stats();

    // Check that all processes terminated
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(static_cast<int>(sched.get_process(i).state), static_cast<int>(ProcessState::TERMINATED));
    }

    // Verify turnaround times roughly
    ASSERT_EQ(sched.get_process(0).turnaround_time, 17);
    ASSERT_EQ(sched.get_process(1).turnaround_time, 15);
    ASSERT_EQ(sched.get_process(2).turnaround_time, 10);
}

TEST(RoundRobin_QuantumEffect) {
    RoundRobinScheduler small_q(2);
    small_q.add_process(Process(1, 10, 0));
    small_q.add_process(Process(2, 10, 0));
    small_q.run();

    RoundRobinScheduler large_q(20);
    large_q.add_process(Process(1, 10, 0));
    large_q.add_process(Process(2, 10, 0));
    large_q.run();

    // Small quantum increases wait time for first process
    ASSERT_TRUE(small_q.get_process(0).wait_time > large_q.get_process(0).wait_time);
}

// Part 4: Priority Tests
TEST(Priority_HighPriorityFirst) {
    PriorityScheduler sched;
    sched.add_process(Process(1, 5, 0, 3));
    sched.add_process(Process(2, 5, 0, 8));
    sched.add_process(Process(3, 5, 0, 5));

    sched.run();
    sched.print_stats();

    // Highest priority process should finish first
    ASSERT_EQ(sched.get_process(0).pid, 2);
    ASSERT_EQ(static_cast<int>(sched.get_process(0).state), static_cast<int>(ProcessState::TERMINATED));
}

// Part 5: MLFQ Tests
TEST(MLFQ_MultiLevel) {
    MLFQScheduler sched({4, 8, 16});
    sched.add_process(Process(1, 20, 0));
    sched.add_process(Process(2, 3, 0));
    sched.add_process(Process(3, 5, 5));

    sched.run();
    sched.print_stats();

    // Interactive process should finish first
    ASSERT_EQ(sched.get_process(1).turnaround_time, 3);
}

// Comparison Test
TEST(CompareAlgorithms) {
    std::vector<Process> workload = {
        Process(1, 10, 0),
        Process(2, 5, 2),
        Process(3, 8, 4),
        Process(4, 2, 6)
    };

    std::cout << "\n=== Comparing Scheduling Algorithms ===" << std::endl;

    FCFSScheduler fcfs;
    for (auto& p : workload) fcfs.add_process(p);
    fcfs.run();
    fcfs.print_stats();

    RoundRobinScheduler rr(4);
    for (auto& p : workload) rr.add_process(p);
    rr.run();
    rr.print_stats();
}

int main() {
    std::cout << "--- ⏱️  PROCESS SCHEDULER TESTS ---\n" << std::endl;

    SECTION("Part 2: FCFS Scheduling");
    SECTION("Part 3: Round Robin");
    SECTION("Part 4: Priority Scheduling");
    SECTION("Part 5: Multilevel Feedback Queue");
    SECTION("Algorithm Comparison");

    RUN_ALL_TESTS();

    std::cout << "\n💡 Key OS Scheduling Concepts:" << std::endl;
    std::cout << "   1. FCFS: Simple but convoy effect" << std::endl;
    std::cout << "   2. Round Robin: Fair, prevents starvation" << std::endl;
    std::cout << "   3. Priority: Important tasks first, but can starve" << std::endl;
    std::cout << "   4. MLFQ: Adapts to workload automatically" << std::endl;

    std::cout << "\n🎯 Real-World OS Schedulers:" << std::endl;
    std::cout << "   - Linux: CFS (Completely Fair Scheduler)" << std::endl;
    std::cout << "   - macOS/iOS: XNU Mach scheduler" << std::endl;
    std::cout << "   - Windows: Multilevel feedback queue" << std::endl;
    std::cout << "   - RTOS: Rate Monotonic / EDF" << std::endl;

    return 0;
}
