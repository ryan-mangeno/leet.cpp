### Auto-Registering Factory System

**Context:**
Build a factory that automatically registers classes using static initialization. Classes register themselves before `main()` runs, allowing creation by string name at runtime.

**Real-World Use:** Plugin systems, command registries, test frameworks.

---

## Part 1: Manual Factory (10 min)

Create a basic factory:

```cpp
class Task {
public:
    virtual ~Task() = default;
    virtual void execute() = 0;
};

class TaskFactory {
public:
    static TaskFactory& instance();
    
    void register_task(const std::string& name, 
                      std::function<std::unique_ptr<Task>()> creator);
    
    std::unique_ptr<Task> create(const std::string& name);
};
```

**Usage:**
```cpp
class PrintTask : public Task {
    void execute() override { std::cout << "Printing\n"; }
};

// Manual registration
TaskFactory::instance().register_task("print", 
    []() { return std::make_unique<PrintTask>(); });

// Create by name
auto task = TaskFactory::instance().create("print");
task->execute();
```

---

## Part 2: Auto-Registration (15 min)

Make registration automatic using static initialization:

```cpp
struct TaskRegistrar {
    TaskRegistrar(const std::string& name, 
                 std::function<std::unique_ptr<Task>()> creator);
};

#define REGISTER_TASK(ClassName, StringName) \
    /* TODO: Create static registrar that runs before main() */
```

**Goal:**
```cpp
class SaveTask : public Task {
    void execute() override { std::cout << "Saving\n"; }
};

REGISTER_TASK(SaveTask, "save");
// Now registered automatically!

// In main():
auto task = TaskFactory::instance().create("save");
```

**Key Concept:** Static objects constructed before `main()` starts.

---

## Part 3: Error Handling & Listing (10 min)

Add safety features:

```cpp
// Return optional instead of nullptr
std::optional<std::unique_ptr<Task>> create_safe(const std::string& name);

// List all registered tasks
std::vector<std::string> list_tasks();

// Check if task exists
bool has_task(const std::string& name);
```

---

## Your Task:

1. Implement singleton TaskFactory
2. Create REGISTER_TASK macro with static initialization
3. Add C++17 std::optional for safe creation
4. Test with multiple task types
5. Verify tasks registered before main()

**Interview Focus:**
- Singleton pattern
- Static initialization order
- Function pointers vs std::function
- Macros and token pasting (##)
- C++17 std::optional
