#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <deque>
#include <mutex>
#include <memory>
#include <functional>
#include <map>
#include <string>
#include <stdexcept>

namespace leet_cpp {

template<typename T> class ObjectPool;

// Part 2: RAII Handle
template<typename T>
class PoolHandle {
public:
    PoolHandle() : obj_(nullptr), pool_(nullptr) {}
    
    PoolHandle(T* obj, ObjectPool<T>* pool) : obj_(obj), pool_(pool) {}
    
    ~PoolHandle() {
        if (obj_ && pool_) {
            pool_->release(obj_);
        }
    }
    
    PoolHandle(const PoolHandle&) = delete;
    PoolHandle& operator=(const PoolHandle&) = delete;
    
    PoolHandle(PoolHandle&& other) noexcept 
        : obj_(other.obj_), pool_(other.pool_) {
        other.obj_ = nullptr;
        other.pool_ = nullptr;
    }
    
    PoolHandle& operator=(PoolHandle&& other) noexcept {
        if (this != &other) {
            if (obj_ && pool_) pool_->release(obj_);
            obj_ = other.obj_;
            pool_ = other.pool_;
            other.obj_ = nullptr;
            other.pool_ = nullptr;
        }
        return *this;
    }
    
    T* get() { return obj_; }
    T* operator->() { return obj_; }
    T& operator*() { return *obj_; }
    explicit operator bool() const { return obj_ != nullptr; }
    
private:
    T* obj_;
    ObjectPool<T>* pool_;
};

// Part 1: Basic Object Pool
template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t initial_size) 
        : min_size_(initial_size), max_size_(initial_size * 10), 
          growth_factor_(1.5f), in_use_count_(0) {
        // TODO Part 1: Allocate initial_size objects
        for (size_t i = 0; i < initial_size; i++) {
            available_.push_back(new T());
        }
    }
    
    ~ObjectPool() {
        // TODO: Delete all objects
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto* obj : available_) {
            delete obj;
        }
        // Note: Objects still "in use" leak! (Should log warning)
    }
    
    // Part 1: Raw pointer acquire
    T* acquire() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // TODO Part 4: Grow if empty and can grow
        if (available_.empty()) {
            if (total_size() < max_size_) {
                grow();
            } else {
                throw std::runtime_error("Pool exhausted");
            }
        }
        
        if (available_.empty()) {
            throw std::runtime_error("Pool exhausted");
        }
        
        T* obj = available_.back();
        available_.pop_back();
        in_use_count_++;
        
        // TODO Part 3: Call reset if exists
        try_reset(obj);
        
        return obj;
    }
    
    void release(T* obj) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // TODO: Validate obj belongs to pool
        available_.push_back(obj);
        in_use_count_--;
    }
    
    // Part 2: RAII acquire
    PoolHandle<T> acquire_handle() {
        return PoolHandle<T>(acquire(), this);
    }
    
    // Part 4: Dynamic sizing
    void set_min_size(size_t min) { min_size_ = min; }
    void set_max_size(size_t max) { max_size_ = max; }
    void set_growth_factor(float factor) { growth_factor_ = factor; }
    
    void shrink_to_fit() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (available_.size() > min_size_) {
            T* obj = available_.back();
            available_.pop_back();
            delete obj;
        }
    }
    
    size_t available() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return available_.size();
    }
    
    size_t in_use() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return in_use_count_;
    }
    
    size_t total_size() const {
        return available_.size() + in_use_count_;
    }
    
private:
    // Part 3: Optional reset support (SFINAE)
    template<typename U = T>
    auto try_reset(U* obj) -> decltype(obj->reset(), void()) {
        obj->reset();
    }
    
    template<typename... Args>
    void try_reset(Args...) {
        // No reset method
    }
    
    // Part 4: Grow pool
    void grow() {
        size_t current = total_size();
        size_t new_size = static_cast<size_t>(current * growth_factor_);
        new_size = std::min(new_size, max_size_);
        
        size_t to_add = new_size - current;
        for (size_t i = 0; i < to_add; i++) {
            available_.push_back(new T());
        }
    }
    
    mutable std::mutex mutex_;
    std::deque<T*> available_;
    size_t in_use_count_;
    size_t min_size_;
    size_t max_size_;
    float growth_factor_;
};

// Part 5: Polymorphic Pool with Factory
template<typename Base>
class PolymorphicPool {
public:
    PolymorphicPool(size_t initial_per_type = 10) 
        : initial_size_(initial_per_type) {}
    
    ~PolymorphicPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, pool] : type_pools_) {
            for (auto* obj : pool) {
                delete obj;
            }
        }
    }
    
    template<typename Derived>
    void register_type(const std::string& type_name) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Store factory function
        factories_[type_name] = []() -> Base* {
            return new Derived();
        };
        
        // Pre-allocate pool
        auto& pool = type_pools_[type_name];
        for (size_t i = 0; i < initial_size_; i++) {
            pool.push_back(factories_[type_name]());
        }
    }
    
    PoolHandle<Base> acquire(const std::string& type_name) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = type_pools_.find(type_name);
        if (it == type_pools_.end()) {
            throw std::runtime_error("Unknown type: " + type_name);
        }
        
        auto& pool = it->second;
        
        // Grow if needed
        if (pool.empty()) {
            auto factory_it = factories_.find(type_name);
            if (factory_it != factories_.end()) {
                pool.push_back(factory_it->second());
            } else {
                throw std::runtime_error("No factory for type: " + type_name);
            }
        }
        
        Base* obj = pool.back();
        pool.pop_back();
        
        // Use callback deleter to return to correct pool
        return PoolHandle<Base>(obj, 
            [this, type_name](Base* o) {
                this->release_polymorphic(type_name, o);
            });
    }
    
    template<typename Derived>
    PoolHandle<Derived> acquire_as() {
        // TODO: Type-safe acquisition
        // Challenge: How to return PoolHandle<Derived> from PoolHandle<Base>?
        throw std::runtime_error("Not implemented");
    }
    
private:
    void release_polymorphic(const std::string& type_name, Base* obj) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = type_pools_.find(type_name);
        if (it != type_pools_.end()) {
            it->second.push_back(obj);
        }
    }
    
    std::mutex mutex_;
    std::map<std::string, std::function<Base*()>> factories_;
    std::map<std::string, std::deque<Base*>> type_pools_;
    size_t initial_size_;
};

// Note: PolymorphicPool's PoolHandle uses std::function deleter
// This requires a custom PoolHandle variant:
template<typename T>
class PolymorphicHandle {
public:
    PolymorphicHandle() = default;
    
    PolymorphicHandle(T* obj, std::function<void(T*)> deleter)
        : obj_(obj), deleter_(deleter) {}
    
    ~PolymorphicHandle() {
        if (obj_ && deleter_) {
            deleter_(obj_);
        }
    }
    
    PolymorphicHandle(const PolymorphicHandle&) = delete;
    PolymorphicHandle& operator=(const PolymorphicHandle&) = delete;
    
    PolymorphicHandle(PolymorphicHandle&& other) noexcept
        : obj_(other.obj_), deleter_(std::move(other.deleter_)) {
        other.obj_ = nullptr;
    }
    
    PolymorphicHandle& operator=(PolymorphicHandle&& other) noexcept {
        if (this != &other) {
            if (obj_ && deleter_) deleter_(obj_);
            obj_ = other.obj_;
            deleter_ = std::move(other.deleter_);
            other.obj_ = nullptr;
        }
        return *this;
    }
    
    T* get() { return obj_; }
    T* operator->() { return obj_; }
    T& operator*() { return *obj_; }
    explicit operator bool() const { return obj_ != nullptr; }
    
private:
    T* obj_ = nullptr;
    std::function<void(T*)> deleter_;
};

// Update PolymorphicPool to return PolymorphicHandle
template<typename Base>
class PolymorphicPool;  // Forward declare updated version

} // namespace leet_cpp

#endif
