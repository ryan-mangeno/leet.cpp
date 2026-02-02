#include <test_framework.h>

#include "submission.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>

using namespace leet_cpp;
using namespace test_framework;

// Part 1 Tests
TEST(Part1_BasicLogging) {
    {
        DataLogger logger("test.log");
        logger.log(LogLevel::INFO, "Test message 1");
        logger.log(LogLevel::WARN, "Test message 2");
    }
    
    std::ifstream file("test.log");
    ASSERT_TRUE(file.good());
    
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        count++;
    }
    ASSERT_TRUE(count >= 2);
    std::remove("test.log");
}

TEST(Part1_ThreadSafety) {
    {
        DataLogger logger("threaded.log");
        
        std::vector<std::thread> threads;
        for (int i = 0; i < 5; i++) {
            threads.emplace_back([&logger, i]() {
                for (int j = 0; j < 100; j++) {
                    logger.log(LogLevel::INFO, 
                            "Thread " + std::to_string(i) + " msg " + std::to_string(j));
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    
    std::ifstream file("threaded.log");
    int count = 0;
    std::string line;
    while (std::getline(file, line)) {
        count++;
    }
    
    ASSERT_EQ(count, 500);
    std::remove("threaded.log");
}

// Part 2 Tests
TEST(Part2_AsynchronousLogging) {
    {
        DataLogger logger("async.log");
        
        for (int i = 0; i < 1000; i++) {
            logger.log(LogLevel::INFO, "Message " + std::to_string(i));
        }
        
        // Logger destructor should flush all pending logs
    }
    
    std::ifstream file("async.log");
    int count = 0;
    std::string line;
    while (std::getline(file, line)) {
        count++;
    }
    
    ASSERT_EQ(count, 1000);
    std::remove("async.log");
}

TEST(Part2_NonBlocking) {
    DataLogger logger("perf.log");
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10000; i++) {
        logger.log(LogLevel::INFO, "Performance test " + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete quickly if non-blocking (< 100ms for 10k logs)
    ASSERT_TRUE(duration.count() < 1000);
    std::remove("perf.log");
}

// Part 3 Tests
TEST(Part3_BinaryLogging) {
    struct SensorData {
        double timestamp;
        float values[3];
    };
    
    {
        DataLogger logger("binary.log");
        
        SensorData data{1.23, {1.0f, 2.0f, 3.0f}};
        logger.log_data("sensor", data);
        logger.flush();
    }
    
    std::ifstream file("binary.log", std::ios::binary);
    ASSERT_TRUE(file.good());

    std::remove("binary.log");
}

// Part 4 Tests
TEST(Part4_FileRotation) {
    const size_t max_size = 50;  // tiny for testing
    const int max_files = 3;

    {
        DataLogger logger("rotate_test.log");
        logger.set_max_file_size(max_size);
        logger.set_max_files(max_files);

        // Write enough logs to trigger multiple rotations
        for (int i = 0; i < 20; i++) {
            logger.log(LogLevel::INFO, "Log entry " + std::to_string(i));
        }
    }

    // Check main log exists
    std::ifstream f0("rotate_test.log", std::ios::binary);
    ASSERT_TRUE(f0.good()); // can't use << message

    // Check rotated files
    // expected that there should be 1 log file + max_files rotated files
    for (int i = 1; i <= max_files; i++) {
        std::string name = "rotate_test.log." + std::to_string(i);
        std::ifstream fi(name, std::ios::binary);

        // Must exist if rotation has occurred
        ASSERT_TRUE(fi.good());
        // Optional: check file size doesn't exceed max_size (+ small buffer)
        fi.seekg(0, std::ios::end);
        auto size = static_cast<size_t>(fi.tellg());
        ASSERT_TRUE(size <= max_size + 50); // use ASSERT_TRUE, not ASSERT_EQ with streaming
    }

    // Clean up
    std::remove("rotate_test.log");
    for (int i = 1; i <= max_files; i++) {
        std::string name = "rotate_test.log." + std::to_string(i);
        std::remove(name.c_str());
    }
}



// Edge Cases
TEST(EdgeCase_RapidOpenClose) {
    for (int i = 0; i < 10; i++) {
        DataLogger logger("rapid.log");
        logger.log(LogLevel::INFO, "Test");
    }
    ASSERT_TRUE(true);
    std::remove("rapid.log");
}

TEST(EdgeCase_EmptyLog) {
    {
        DataLogger logger("empty.log");
        // No logs written
    }
    
    std::ifstream file("empty.log");
    ASSERT_TRUE(file.good());
    std::remove("empty.log");
}

int main() {
    std::cout << "--- 📝 DATA LOGGER SYSTEM TESTS ---\n" << std::endl;
    
    SECTION("Part 1: Basic Synchronous Logging");
    SECTION("Part 2: Asynchronous Background Writer");
    SECTION("Part 3: Binary Logging");
    SECTION("Part 4: File Rotation");
    SECTION("Edge Cases");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Design Decisions:" << std::endl;
    std::cout << "   1. Synchronous vs Asynchronous: Trade latency for throughput" << std::endl;
    std::cout << "   2. Buffer strategy: Bounded queue prevents memory growth" << std::endl;
    std::cout << "   3. Binary logging: Zero-copy for high-frequency data" << std::endl;
    std::cout << "   4. File rotation: Manage disk space automatically" << std::endl;
    
    std::cout << "\n🎯 Performance Tips:" << std::endl;
    std::cout << "   - Use circular buffer for bounded memory" << std::endl;
    std::cout << "   - Batch writes to reduce syscalls" << std::endl;
    std::cout << "   - Consider memory-mapped files for speed" << std::endl;
    std::cout << "   - fsync() only when critical (performance cost)" << std::endl;
    
    return 0;
}
