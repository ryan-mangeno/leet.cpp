#pragma once
#include <string>
#include <cstdio>

class ResourceHandle {
public:
    // Constructor: opens the file
    ResourceHandle(const std::string& filename);
    
    // Destructor: closes the file
    ~ResourceHandle();
    
    // Delete copy operations
    ResourceHandle(const ResourceHandle&) = delete;
    ResourceHandle& operator=(const ResourceHandle&) = delete;
    
    // Implement move operations
    ResourceHandle(ResourceHandle&& other) noexcept;
    ResourceHandle& operator=(ResourceHandle&& other) noexcept;
    
    // Write data to the file
    void write(const std::string& data);
    
    // Check if handle is valid
    bool isValid() const;

private:
    FILE* file_;
};

// TODO: Implement all methods
