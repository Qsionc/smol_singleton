/**
 * This software is free to use.
 * Consider leaving a comment with some insight.
 */

#ifndef SINGLETONWRAPPER_SINGLETON_H
#define SINGLETONWRAPPER_SINGLETON_H

#include <mutex>
#include <memory>
#include <atomic>

namespace smol {
    template<class T, class Deleter = std::default_delete<T>>
    class singleton {
    private:
        static std::atomic_bool initialized_;
        static std::mutex mutex_;
        static std::unique_ptr<T, Deleter> object_;
    public:
        singleton() = default;

        singleton(singleton const &) = default;

        singleton &operator=(singleton const &) = default;

        [[nodiscard]] bool initialized() const {
            return initialized_;
        }

        operator bool() const {
            return initialized();
        }

        template<class... Args>
        void init(Args &&... _args) try {
            if(!initialized()) {
                std::lock_guard lockGuard(mutex_);
                initialized_ = true;
                object_ = std::unique_ptr<T, Deleter>(new T(std::forward<Args>(_args)...));
            }
        }
        catch (...) {
            initialized_ = false;
            throw;
        }

        void destroy() {
            std::lock_guard lockGuard(mutex_);
            object_.reset(nullptr);
            initialized_ = false;
        }

        T *operator->() const {
            std::lock_guard lockGuard(mutex_);
            return object_.get();
        }

        T& operator*() const {
            std::lock_guard lockGuard(mutex_);
            return *object_;
        }

        ~singleton() = default;
    };

    template<class T, class Deleter>
    std::atomic_bool singleton<T, Deleter>::initialized_(false);

    template<class T, class Deleter>
    std::mutex singleton<T, Deleter>::mutex_;

    template<class T, class Deleter>
    std::unique_ptr<T, Deleter> singleton<T, Deleter>::object_(nullptr);
}

#endif //SINGLETONWRAPPER_SINGLETON_H
