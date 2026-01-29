#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>


namespace leet_cpp {

template<typename T>
class ResourceHandle {
public:
    explicit ResourceHandle(std::unique_ptr<T> resource,
                           std::function<void(T*)> on_release)
        : resource_(std::move(resource)), on_release_(on_release) {}

    ~ResourceHandle() {
        if (resource_ && on_release_) {
            on_release_(resource_.get());
        }
    }

    ResourceHandle(const ResourceHandle&) = delete;
    ResourceHandle& operator=(const ResourceHandle&) = delete;

    ResourceHandle(ResourceHandle&& other) noexcept
        : resource_(std::move(other.resource_)),
          on_release_(std::move(other.on_release_)) {}

    ResourceHandle& operator=(ResourceHandle&& other) noexcept {
        if (this != &other) {
            resource_ = std::move(other.resource_);
            on_release_ = std::move(other.on_release_);
        }
        return *this;
    }

    T* get() { return resource_.get(); }
    T* operator->() { return resource_.get(); }

private:
    std::unique_ptr<T> resource_;
    std::function<void(T*)> on_release_;
};

template<typename T>
class ResourcePool {
public:
    explicit ResourcePool(size_t pool_size) : total_size_(pool_size) {
        // TODO: Pre-allocate resources
    }

    ResourceHandle<T> acquire() {
        // TODO: Get resource, wrap in handle with release callback
        throw std::runtime_error("No resources available");
    }

    size_t available() const { return available_.size(); }
    size_t total() const { return total_size_; }

private:
    void release(T* resource) {
        // TODO: Return to pool
    }

    std::vector<std::unique_ptr<T>> available_;
    size_t total_size_;
};

} // namespace leet_cpp

#endif
