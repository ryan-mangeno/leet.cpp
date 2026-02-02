#include <test_framework.h>
#include "submission.h"

using namespace leet_cpp;
using namespace test_framework;

// Test Tasks
class TestTask1 : public Task {
public:
    void execute() override {
        executed = true;
    }
    static bool executed;
};
bool TestTask1::executed = false;

class TestTask2 : public Task {
public:
    void execute() override {
        executed = true;
    }
    static bool executed;
};
bool TestTask2::executed = false;

// Auto-register test tasks
REGISTER_TASK(TestTask1, "task1");
REGISTER_TASK(TestTask2, "task2");

TEST(Factory_CreateByName) {
    auto task = TaskFactory::instance().create("task1");
    ASSERT_TRUE(task != nullptr);
    task->execute();
    ASSERT_TRUE(TestTask1::executed);
}

TEST(Factory_UnknownTask) {
    auto task = TaskFactory::instance().create("unknown");
    ASSERT_TRUE(task == nullptr);
}

TEST(Factory_OptionalCreate) {
    auto task = TaskFactory::instance().create_safe("task2");
    ASSERT_TRUE(task.has_value());
    
    auto missing = TaskFactory::instance().create_safe("missing");
    ASSERT_TRUE(!missing.has_value());
}

TEST(Factory_ListTasks) {
    auto tasks = TaskFactory::instance().list_tasks();
    ASSERT_TRUE(tasks.size() >= 2);
    
    bool has_task1 = false;
    for (const auto& name : tasks) {
        if (name == "task1") has_task1 = true;
    }
    ASSERT_TRUE(has_task1);
}

TEST(Factory_HasTask) {
    ASSERT_TRUE(TaskFactory::instance().has_task("task1"));
    ASSERT_TRUE(!TaskFactory::instance().has_task("nonexistent"));
}

TEST(StaticInit_BeforeMain) {
    // Tasks should already be registered
    // This verifies static initialization worked
    ASSERT_TRUE(TaskFactory::instance().has_task("task1"));
    ASSERT_TRUE(TaskFactory::instance().has_task("task2"));
}

TEST(MultipleCreates) {
    auto t1 = TaskFactory::instance().create("task1");
    auto t2 = TaskFactory::instance().create("task1");
    auto t3 = TaskFactory::instance().create("task2");
    
    ASSERT_TRUE(t1 != nullptr);
    ASSERT_TRUE(t2 != nullptr);
    ASSERT_TRUE(t3 != nullptr);
}

int main() {
    std::cout << "--- 🏭 AUTO-REGISTERING FACTORY TESTS ---\n" << std::endl;
    
    SECTION("Factory Creation");
    SECTION("C++17 Optional");
    SECTION("Static Initialization");
    
    RUN_ALL_TESTS();
    
    std::cout << "\n💡 Key Concepts:" << std::endl;
    std::cout << "   - Singleton pattern (static local)" << std::endl;
    std::cout << "   - Static initialization (before main)" << std::endl;
    std::cout << "   - std::function for type erasure" << std::endl;
    std::cout << "   - Macros with token pasting (##)" << std::endl;
    std::cout << "   - std::optional (C++17)" << std::endl;
    
    return 0;
}
