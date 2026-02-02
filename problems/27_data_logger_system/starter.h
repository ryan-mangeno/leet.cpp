#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <cstring>

namespace leet_cpp {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

// Part 1: Basic log entry
struct LogEntry {
    uint64_t timestamp_ns;
    LogLevel level;
    std::string message;
};

// Part 1: Basic synchronous logger
class DataLogger {
public:
    explicit DataLogger(const std::string& filename) {
        // TODO: Open file for writing
    }

    ~DataLogger() {
        // TODO: Part 2 - shutdown background thread
        // TODO: Flush remaining logs
        // TODO: Close file
    }

    // Part 1: Basic synchronous logging
    void log(LogLevel level, const std::string& msg) {
        // TODO Part 1: Lock, format entry, write to file
        // TODO Part 2: Add to buffer, notify background thread
    }

    // Part 3: Binary logging
    template<typename T>
    void log_data(const std::string& channel, const T& data) {
        // TODO: Check T is POD (std::is_trivially_copyable)
        // TODO: Create binary entry with channel name
        // TODO: Add to buffer for background writer
    }

    // Part 4: File rotation settings
    void set_max_file_size(size_t bytes) {
        max_file_size_ = bytes;
    }

    void set_max_files(int count) {
        max_files_ = count;
    }

    void flush() {
        // TODO: Ensure all buffered logs are written
    }

private:
    // Part 2: Background writer thread
    void writer_thread() {
        // TODO: Loop until shutdown
        // TODO: Wait for entries in buffer (condition variable)
        // TODO: Write entries to file
        // TODO: Check file size for rotation
    }

    // Part 4: Rotate log files
    void rotate_files() {
        // TODO: Close current file
        // TODO: Rename existing files (.1 -> .2, .2 -> .3, etc.)
        // TODO: Delete oldest if exceeds max_files
        // TODO: Open new file
    }

    std::string filename_;
    std::ofstream file_;
    
    // Part 2: Background thread infrastructure
    std::queue<LogEntry> buffer_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread writer_;
    std::atomic<bool> shutdown_{false};
    
    // Part 4: Rotation settings
    size_t max_file_size_ = 100 * 1024 * 1024;  // 100MB default
    int max_files_ = 5;
    size_t current_file_size_ = 0;
};

} // namespace leet_cpp

#endif
