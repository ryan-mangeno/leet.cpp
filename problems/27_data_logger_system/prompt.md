### Build a Data Logging System

**Scenario:**
You're building a data logging system for a robot/vehicle that records sensor data, events, and diagnostics. It must handle high-frequency data (1000+ Hz) without blocking the main application.

Think: Flight recorder, ROS bag files, telemetry systems.

---

## Part 1: Basic Logger (15 min)

Implement `DataLogger` that writes log entries to a file:

```cpp
struct LogEntry {
    uint64_t timestamp_ns;
    LogLevel level;      // DEBUG, INFO, WARN, ERROR
    std::string message;
};

class DataLogger {
public:
    DataLogger(const std::string& filename);
    void log(LogLevel level, const std::string& msg);
    void flush();
    ~DataLogger();
};
```

**Requirements:**
- Thread-safe: Multiple threads can log concurrently
- Non-blocking: `log()` should return immediately
- RAII: Flush and close file in destructor

**Implementation Choices:**
1. **Buffering Strategy:** Write immediately or buffer?
2. **Locking:** Mutex per log call? What's the cost?
3. **File I/O:** `std::ofstream`, `fwrite()`, or `write()`?

**Think About:**
```cpp
// What happens here?
for (int i = 0; i < 1000000; i++) {
    logger.log(INFO, "Sensor reading: " + std::to_string(i));
}
// Is this blocking the caller? How long does it take?
```

---

## Part 2: Asynchronous Background Writer (20 min)

Make logging truly non-blocking with a background thread:

```cpp
class DataLogger {
public:
    DataLogger(const std::string& filename);
    void log(LogLevel level, const std::string& msg);
    // ... rest
    
private:
    void writer_thread();  // Background thread
    
    std::queue<LogEntry> buffer_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread writer_;
    std::atomic<bool> shutdown_;
};
```

**Architecture:**
```
Main Thread(s)          Background Thread
    |                         |
log() → buffer_ (fast) → writer_thread()
    |                         |
    ↓                         ↓
  return                  write to disk
```

**Key Challenges:**
1. **Producer-Consumer:** How to signal the writer thread?
2. **Shutdown:** How to ensure all logs written before exit?
3. **Full Buffer:** What if buffer fills up? Drop or block?

**Questions:**
- Use `std::queue` or `std::deque`? Why?
- Condition variable vs busy-wait?
- How to handle destructor with active thread?

---

## Part 3: Zero-Copy Binary Logging (25 min)

Add binary logging for high-frequency numeric data:

```cpp
template<typename T>
void log_data(const std::string& channel, const T& data);

// Example usage:
struct SensorData {
    double timestamp;
    float position[3];
    float velocity[3];
};

logger.log_data("imu", sensor_data);  // No string formatting!
```

**Requirements:**
- No `std::to_string()` conversions
- Write raw bytes directly
- Support custom structs (POD types)

**Binary Format Design:**
```
[Header: 8 bytes]
  - timestamp: 8 bytes
  - channel_id: 2 bytes
  - data_size: 2 bytes
  - checksum: 4 bytes (optional)
[Data: variable bytes]
  - raw struct bytes
```

**Memory Considerations:**
```cpp
// String logging: Allocates!
log(INFO, "Position: " + std::to_string(x) + "," + std::to_string(y));

// Binary logging: No allocation!
log_data("position", Vec2{x, y});
```

**Think About:**
- Endianness for cross-platform logs?
- Alignment requirements for structs?
- How to prevent logging non-POD types?

---

## Part 4: Log Rotation & Resource Management (20 min)

Add automatic file rotation:

```cpp
class DataLogger {
public:
    void set_max_file_size(size_t bytes);
    void set_max_files(int count);
    
    // When file reaches max size:
    // data.log → data.log.1
    // data.log.1 → data.log.2
    // Start new data.log
};
```

**Requirements:**
- Rotate when file size limit reached
- Keep only N most recent files
- Thread-safe rotation (no lost logs!)

**Implementation Challenges:**
```cpp
// Thread 1 logging:
logger.log(INFO, "message 1");
logger.log(INFO, "message 2");  // File rotates here!
logger.log(INFO, "message 3");  // New file

// Thread 2 logging concurrently:
logger.log(INFO, "concurrent");  // Which file?
```

**Rotation Algorithm:**
1. Check file size threshold
2. Flush current buffer
3. Close current file
4. Rename files (data.log.2 → data.log.3, etc.)
5. Open new file
6. Resume logging

**Questions:**
- Use `std::filesystem::rename()`?
- What if disk is full during rotation?
- Atomic rotation vs lock-free?

---

## Part 5: Performance & Real-World Design (Discussion)

**Benchmarking:**
Your logger must handle:
- 1000 log calls/second from main thread
- 10,000 binary data logs/second from sensor threads
- Sustained for hours without memory growth

**Memory Analysis:**
```cpp
// Bad: Unbounded growth
std::queue<LogEntry> buffer_;  // What if writer can't keep up?

// Better: Bounded queue
CircularBuffer<LogEntry> buffer_{10000};  // Drop oldest when full

// Best: Lock-free SPSC queue (like problem #2!)
SPSCQueue<LogEntry> buffer_{4096};
```

**Design Questions:**

**Q1: String Storage**
```cpp
// Option A: Store by value
struct LogEntry {
    std::string message;  // Allocates per message
};

// Option B: String pool
struct LogEntry {
    const char* message;  // Points into pool
};

// Option C: Fixed-size buffer
struct LogEntry {
    char message[256];  // No allocation, but wastes space
};
```
Which would you choose? Trade-offs?

**Q2: Multiple Log Files**
How would you support multiple concurrent log files?
```cpp
auto sensor_log = logger.create_channel("sensors");
auto event_log = logger.create_channel("events");

sensor_log->log_data(imu_data);
event_log->log(INFO, "Started");
```

**Q3: Crash Recovery**
If application crashes, how to ensure logs are preserved?
- `fsync()` after each write? (slow!)
- Periodic flush? (may lose recent logs)
- Memory-mapped files?

**Q4: Compression**
Add log compression:
```cpp
logger.set_compression(CompressionLevel::FAST);
```
When to compress? Background thread? On rotation?

---

## Your Implementation:

Implement Parts 1-4 progressively. Each part builds on the previous.

**Testing Focus:**
- Concurrent logging from 10 threads
- High-frequency binary logging (10k/sec)
- Graceful shutdown with pending logs
- File rotation during active logging
- Memory stability over time
