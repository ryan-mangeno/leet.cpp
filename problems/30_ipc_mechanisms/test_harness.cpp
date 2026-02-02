#include <test_framework.h>
#include "submission.h"
#include <thread>
#include <vector>
#include <chrono>

using namespace leet_cpp;
using namespace test_framework;

// Part 1: Message Queue Tests
TEST(MessageQueue_SendReceive) {
    MessageQueue<int> mq(10);
    
    mq.send(42);
    
    int value;
    mq.receive(value);
    
    ASSERT_EQ(value, 42);
}

TEST(MessageQueue_Blocking) {
    MessageQueue<int> mq(2);
    
    mq.send(1);
    mq.send(2);
    // Queue now full
    
    bool sent = false;
    std::thread producer([&]() {
        mq.send(3);  // Blocks until space
        sent = true;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_TRUE(!sent);  // Still blocked
    
    int val;
    mq.receive(val);  // Make space
    
    producer.join();
    ASSERT_TRUE(sent);  // Now completed
}

TEST(MessageQueue_NonBlocking) {
    MessageQueue<int> mq(2);
    
    ASSERT_TRUE(mq.try_send(1));
    ASSERT_TRUE(mq.try_send(2));
    ASSERT_TRUE(!mq.try_send(3));  // Full
    
    int val;
    ASSERT_TRUE(mq.try_receive(val));
    ASSERT_EQ(val, 1);
}

TEST(MessageQueue_MultiProducerConsumer) {
    MessageQueue<int> mq(100);
    const int MESSAGES_PER_THREAD = 1000;
    const int NUM_PRODUCERS = 3;
    const int NUM_CONSUMERS = 3;
    
    std::atomic<int> received_count{0};
    std::vector<std::thread> threads;
    
    // Producers
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        threads.emplace_back([&, i]() {
            for (int j = 0; j < MESSAGES_PER_THREAD; j++) {
                mq.send(i * 1000 + j);
            }
        });
    }
    
    // Consumers
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        threads.emplace_back([&]() {
            int val;
            for (int j = 0; j < (NUM_PRODUCERS * MESSAGES_PER_THREAD) / NUM_CONSUMERS; j++) {
                mq.receive(val);
                received_count++;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    ASSERT_EQ(received_count, NUM_PRODUCERS * MESSAGES_PER_THREAD);
}

// Part 2: Shared Buffer Tests
TEST(SharedBuffer_Basic) {
    SharedBuffer<int> buf(5);
    
    ASSERT_TRUE(buf.empty());
    
    buf.write(10);
    buf.write(20);
    
    ASSERT_EQ(buf.size(), 2);
    
    int val;
    buf.read(val);
    ASSERT_EQ(val, 10);
}

TEST(SharedBuffer_Wraparound) {
    SharedBuffer<int> buf(3);
    
    buf.write(1);
    buf.write(2);
    buf.write(3);
    ASSERT_TRUE(buf.full());
    
    int val;
    buf.read(val);
    buf.read(val);
    
    buf.write(4);
    buf.write(5);
    
    buf.read(val);
    ASSERT_EQ(val, 3);
    buf.read(val);
    ASSERT_EQ(val, 4);
}

TEST(SharedBuffer_ThreadSafety) {
    SharedBuffer<int> buf(100);
    
    std::thread writer([&]() {
        for (int i = 0; i < 1000; i++) {
            while (!buf.write(i)) {
                std::this_thread::yield();
            }
        }
    });
    
    std::thread reader([&]() {
        int val;
        for (int i = 0; i < 1000; i++) {
            while (!buf.read(val)) {
                std::this_thread::yield();
            }
            ASSERT_EQ(val, i);
        }
    });
    
    writer.join();
    reader.join();
}

// Part 3: Signal Handler Tests
TEST(SignalHandler_Basic) {
    bool handler_called = false;
    
    SignalHandler::register_handler(SIGUSR1, [&](int sig) {
        handler_called = true;
    });
    
    SignalHandler::send_signal(SIGUSR1);
    
    ASSERT_TRUE(handler_called);
}

// Part 4: Pipe Tests
TEST(Pipe_WriteRead) {
    Pipe pipe;
    
    const char* msg = "Hello, IPC!";
    pipe.write(msg, 11);
    
    char buffer[100];
    size_t bytes = pipe.read(buffer, 100);
    
    ASSERT_EQ(bytes, 11);
    ASSERT_TRUE(std::string(buffer, 11) == msg);
}

TEST(Pipe_CloseWrite) {
    Pipe pipe;
    
    pipe.write("data", 4);
    pipe.close_write();
    
    char buffer[10];
    size_t bytes1 = pipe.read(buffer, 10);
    ASSERT_EQ(bytes1, 4);
    
    size_t bytes2 = pipe.read(buffer, 10);
    ASSERT_EQ(bytes2, 0);  // EOF
}

TEST(Pipe_ProducerConsumer) {
    Pipe pipe;
    
    std::thread writer([&]() {
        for (int i = 0; i < 100; i++) {
            std::string msg = "msg" + std::to_string(i);
            pipe.write(msg.c_str(), msg.length());
        }
        pipe.close_write();
    });
    
    std::thread reader([&]() {
        char buffer[100];
        int count = 0;
        while (true) {
            size_t bytes = pipe.read(buffer, 100);
            if (bytes == 0) break;  // EOF
            count++;
        }
        ASSERT_TRUE(count > 0);
    });
    
    writer.join();
    reader.join();
}

// Performance Comparison
TEST(Performance_Comparison) {
    const int ITERATIONS = 10000;
    
    // Message Queue
    auto start = std::chrono::high_resolution_clock::now();
    {
        MessageQueue<int> mq(100);
        std::thread t([&]() {
            int val;
            for (int i = 0; i < ITERATIONS; i++) {
                mq.receive(val);
            }
        });
        
        for (int i = 0; i < ITERATIONS; i++) {
            mq.send(i);
        }
        t.join();
    }
    auto mq_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    // Shared Buffer
    start = std::chrono::high_resolution_clock::now();
    {
        SharedBuffer<int> buf(100);
        std::thread t([&]() {
            int val;
            for (int i = 0; i < ITERATIONS; i++) {
                while (!buf.read(val)) std::this_thread::yield();
            }
        });
        
        for (int i = 0; i < ITERATIONS; i++) {
            while (!buf.write(i)) std::this_thread::yield();
        }
        t.join();
    }
    auto buf_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "  MessageQueue: " << mq_time << "μs" << std::endl;
    std::cout << "  SharedBuffer: " << buf_time << "μs" << std::endl;
    
    ASSERT_TRUE(true);
}

int main() {
    std::cout << "--- 🔌 IPC MECHANISMS TESTS ---\n" << std::endl;
    
    SECTION("Part 1: Message Queue");
    SECTION("Part 2: Shared Buffer");
    SECTION("Part 3: Signal Handler");
    SECTION("Part 4: Pipe");
    SECTION("Performance");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 IPC Comparison:" << std::endl;
    std::cout << "   - Message Queue: Easy, sync built-in" << std::endl;
    std::cout << "   - Shared Memory: Fastest, manual sync" << std::endl;
    std::cout << "   - Pipe: Simple, one-way" << std::endl;
    std::cout << "   - Signal: Async notifications only" << std::endl;
    
    std::cout << "\n🎯 Real-World Usage:" << std::endl;
    std::cout << "   - ROS: Shared memory for images, MQ for commands" << std::endl;
    std::cout << "   - Apple XNU: Mach ports (message-based)" << std::endl;
    std::cout << "   - Linux: Pipes, sockets, shared memory" << std::endl;
    
    return 0;
}
