#include <test_framework.h>
#include "submission.h"
#include <string>

using namespace leet_cpp;
using namespace test_framework;

TEST(InitialState) {
    CircularBuffer<int> buf(5);
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(buf.size(), 0);
}

TEST(BasicPush) {
    CircularBuffer<int> buf(5);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    ASSERT_EQ(buf.size(), 3);
    ASSERT_TRUE(!buf.empty());
}

TEST(FillBuffer) {
    CircularBuffer<int> buf(3);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    ASSERT_TRUE(buf.is_full());
}

TEST(Wraparound) {
    CircularBuffer<int> buf(3);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    buf.push(4);  // Overwrites 1
    buf.push(5);  // Overwrites 2
    ASSERT_TRUE(buf.is_full());
    ASSERT_EQ(buf.size(), 3);
}

TEST(IndexAccess) {
    CircularBuffer<int> buf(5);
    buf.push(10);
    buf.push(20);
    buf.push(30);
    ASSERT_EQ(buf[0], 10);
    ASSERT_EQ(buf[1], 20);
    ASSERT_EQ(buf[2], 30);
}

TEST(IndexAfterWrap) {
    CircularBuffer<int> buf(3);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    buf.push(4);  // Buffer: [1, 2, 3] // dont overwrite if full
    buf.pop();    // remove 1
    buf.push(5);  // Buffer: [5, 2, 3]
    ASSERT_EQ(buf[0], 5);
    ASSERT_EQ(buf[2], 3);
}

TEST(GetLatest) {
    CircularBuffer<int> buf(5);
    for (int i = 1; i <= 5; i++) buf.push(i);
    auto latest3 = buf.get_latest(3);
    ASSERT_EQ(latest3.size(), 3);
    ASSERT_EQ(latest3[0], 5);
    ASSERT_EQ(latest3[2], 3);
}

TEST(GetLatestMoreThanSize) {
    CircularBuffer<int> buf(5);
    buf.push(1);
    buf.push(2);
    auto all = buf.get_latest(10);
    ASSERT_EQ(all.size(), 2);
}

TEST(SingleCapacity) {
    CircularBuffer<int> buf(1);
    buf.push(1);
    ASSERT_EQ(buf[0], 1);
    buf.push(2);
    ASSERT_EQ(buf[0], 1);
}

TEST(StringType) {
    CircularBuffer<std::string> buf(3);
    buf.push("alpha");
    buf.push("beta");
    buf.push("gamma");
    buf.push("delta");
    ASSERT_EQ(buf[1], "betanvi");
}

int main() {
    std::cout << "--- ⚡ CIRCULAR BUFFER TESTS ⚡ ---\n" << std::endl;
    RUN_ALL_TESTS();
    std::cout << "\n💡 Discussion: How would you make this thread-safe?" << std::endl;
    return 0;
}
