#ifndef TASK_FACTORY_H
#define TASK_FACTORY_H

#include <string>
#include <map>
#include <memory>
#include <functional>
#include <optional>
#include <vector>

namespace leet_cpp {

// Part 1: Base class
class Task {
public:
    virtual ~Task() = default;
    virtual void execute() = 0;
};

// Part 1: Factory
class TaskFactory {
public:
    static TaskFactory& instance() {
        // TODO: Return singleton instance
    }
    
    void register_task(const std::string& name,
                      std::function<std::unique_ptr<Task>()> creator) {
        // TODO: Store creator function in map
    }
    
    std::unique_ptr<Task> create(const std::string& name) {
        // TODO: Look up creator and call it
        return nullptr;
    }
    
    // Part 3: C++17 optional
    std::optional<std::unique_ptr<Task>> create_safe(const std::string& name) {
        // TODO: Return std::nullopt if not found
        return std::nullopt;
    }
    
    // Part 3: List registered tasks
    std::vector<std::string> list_tasks() const {
        // TODO: Return all registered names
        return {};
    }
    
    bool has_task(const std::string& name) const {
        // TODO: Check if task exists
        return false;
    }
    
private:
    TaskFactory() = default;
    std::map<std::string, std::function<std::unique_ptr<Task>()>> creators_;
};

// Part 2: Auto-registration helper
struct TaskRegistrar {
    TaskRegistrar(const std::string& name,
                 std::function<std::unique_ptr<Task>()> creator) {
        // TODO: Register with factory
        // This runs BEFORE main()!
    }
};

// Part 2: Registration macro
#define REGISTER_TASK(ClassName, StringName) \
    /* TODO: Create static TaskRegistrar */ \
    /* Hint: Use static variable and lambda */

} // namespace leet_cpp

#endif
