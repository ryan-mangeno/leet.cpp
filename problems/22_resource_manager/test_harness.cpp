#include <test_framework.h>
#include "submission.h"

using namespace leet_cpp;
using namespace test_framework;

struct MockResource {
    static int count;
    int id;
    MockResource() : id(++count) {}
    void use() {}
};
int MockResource::count = 0;

TEST(RAIIAutoRelease) {
    int released = 0;
    {
        auto res = std::make_unique<MockResource>();
        ResourceHandle<MockResource> handle(
            std::move(res),
            [&](MockResource*) { released++; }
        );
    }
    ASSERT_EQ(released, 1);
}

TEST(PoolInitialization) {
    ResourcePool<MockResource> pool(3);
    ASSERT_EQ(pool.available(), 3);
    ASSERT_EQ(pool.total(), 3);
}

TEST(AcquireRelease) {
    ResourcePool<MockResource> pool(3);
    {
        auto res = pool.acquire();
        ASSERT_EQ(pool.available(), 2);
    }
    ASSERT_EQ(pool.available(), 3);
}

TEST(MultipleAcquire) {
    ResourcePool<MockResource> pool(3);
    auto r1 = pool.acquire();
    auto r2 = pool.acquire();
    ASSERT_EQ(pool.available(), 1);
}

TEST(PoolExhaustion) {
    ResourcePool<MockResource> pool(2);
    auto r1 = pool.acquire();
    auto r2 = pool.acquire();
    ASSERT_EQ(pool.available(), 0);
    ASSERT_THROWS([&](){ pool.acquire(); });
}

TEST(MoveSemantics) {
    ResourcePool<MockResource> pool(2);
    auto r1 = pool.acquire();
    auto r2 = std::move(r1);
    ASSERT_EQ(pool.available(), 1);
}

int main() {
    std::cout << "--- ⚡ RESOURCE MANAGER TESTS ⚡ ---\n" << std::endl;
    RUN_ALL_TESTS();
    return 0;
}
