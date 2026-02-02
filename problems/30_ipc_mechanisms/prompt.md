### Inter-Process Communication (IPC)

**Context:**
Build IPC mechanisms to understand how processes communicate. Critical for systems programming, robotics (ROS), and distributed systems.

---

## Part 1: Thread-Safe Message Queue (15 min)

Simulate message passing between processes:

```cpp
template<typename T>
class MessageQueue {
public:
    MessageQueue(size_t capacity);
    
    bool send(const T& msg);              // Blocking
    bool try_send(const T& msg);          // Non-blocking
    bool receive(T& msg);                 // Blocking
    bool try_receive(T& msg);             // Non-blocking
};
```

**Key Concepts:**
- Blocking vs non-blocking I/O
- Producer-consumer pattern
- Condition variables for waiting

**Example:**
```cpp
MessageQueue<int> mq(10);

// Thread 1 (producer)
mq.send(42);

// Thread 2 (consumer)
int value;
mq.receive(value);  // Gets 42
```

---

## Part 2: Shared Memory Buffer (20 min)

Implement shared circular buffer:

```cpp
template<typename T>
class SharedBuffer {
public:
    SharedBuffer(size_t capacity);
    
    bool write(const T& item);
    bool read(T& item);
    
    size_t size() const;
    bool empty() const;
    bool full() const;
    
private:
    std::vector<T> buffer_;
    size_t head_, tail_, count_;
    mutable std::mutex mutex_;
};
```

**vs Message Queue:**
```
Shared Memory:
  ✅ Fastest - direct memory access
  ❌ Must handle synchronization yourself
  ❌ More complex

Message Queue:
  ✅ Easier to use
  ✅ Sync built-in
  ❌ Kernel overhead
```

---

## Part 3: Signal Handling (15 min)

Implement basic signal handling:

```cpp
class SignalHandler {
public:
    using Callback = std::function<void(int)>;
    
    static void register_handler(int signal, Callback cb);
    static void send_signal(int signal);
    
private:
    static std::map<int, Callback> handlers_;
};
```

**Important Signals:**
```cpp
SIGINT  - Ctrl+C (interrupt)
SIGTERM - Graceful shutdown
SIGUSR1 - User-defined
```

**Signal Safety:**
```cpp
// ❌ UNSAFE in signal handler
printf("Got signal\n");
malloc(100);

// ✅ SAFE in signal handler
write(STDOUT_FILENO, "Signal\n", 7);
```

---

## Part 4: Pipe Communication (15 min)

Simple pipe for one-way communication:

```cpp
class Pipe {
public:
    Pipe();
    ~Pipe();
    
    void write(const void* data, size_t size);
    size_t read(void* buffer, size_t size);
    
    void close_write();
    void close_read();
    
private:
    int fds_[2];  // [0] = read, [1] = write
};
```

**Use Case:**
```cpp
// Parent-child communication
Pipe pipe;

if (fork() == 0) {
    // Child: Read from pipe
    char buf[100];
    pipe.read(buf, 100);
} else {
    // Parent: Write to pipe
    pipe.write("Hello", 5);
}
```

---

## Part 5: Comparison & Real-World Usage

**Speed Comparison:**
```
Shared Memory:  ~0.1 μs  (fastest)
Unix Socket:    ~1-5 μs
Message Queue:  ~5-10 μs
TCP Socket:     ~50-100 μs
```

**When to Use:**

| Scenario | Best Choice | Why |
|----------|-------------|-----|
| High-frequency data (1000+ Hz) | Shared Memory | Lowest latency |
| Task queue | Message Queue | Easy, built-in sync |
| Parent-child IPC | Pipe | Simple, one-way |
| Client-server | Socket | Standard, flexible |
| Async notifications | Signal | Lightweight |

**Real-World Examples:**

**ROS (Robot Operating System):**
- Topics: Shared memory for large data
- Services: Message queue for commands
- Actions: Long-running tasks

**Apple XNU/Darwin:**
- Mach ports: Message-based IPC
- Used throughout macOS/iOS

**Linux:**
- D-Bus: Message bus for desktop
- Netlink: Kernel ↔ user space

---

## Discussion Questions:

**Q1:** Why is shared memory dangerous?
**A:** Race conditions if sync wrong, can corrupt data

**Q2:** What happens if message queue is full?
**A:** send() blocks or returns error (depends on flags)

**Q3:** Can signals carry data?
**A:** No! Just signal number. Use other IPC for data.

**Q4:** How to avoid deadlock with multiple locks?
**A:** Always acquire locks in same order, use try_lock

**Q5:** What's a semaphore vs mutex?
**A:** Semaphore: counting (N resources), Mutex: binary (1 resource)

---

## Your Task:

Implement Parts 1-4. Focus on synchronization correctness.

**Key Learning:**
- Blocking vs non-blocking operations
- Producer-consumer synchronization
- When to use which IPC mechanism
- Trade-offs: speed vs ease of use
