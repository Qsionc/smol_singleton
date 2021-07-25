//
// Created by lord on 24/07/2021.
//

#ifndef SINGLETONWRAPPER_SINGLETONWRAPPER_H
#define SINGLETONWRAPPER_SINGLETONWRAPPER_H

#include <mutex>
#include <memory>
#include <atomic>

namespace singleton {
    template<class T, class Deleter = std::default_delete<T>>
    class SingletonWrapper {
    private:
        static std::atomic_bool initialized_;
        static std::mutex mutex_;
        static std::unique_ptr<T, Deleter> object_;
    public:
        SingletonWrapper() = default;

        SingletonWrapper(SingletonWrapper const &) = default;

        SingletonWrapper &operator=(SingletonWrapper const &) = default;

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

        ~SingletonWrapper() = default;
    };

    template<class T, class Deleter>
    std::atomic_bool SingletonWrapper<T, Deleter>::initialized_(false);

    template<class T, class Deleter>
    std::mutex SingletonWrapper<T, Deleter>::mutex_;

    template<class T, class Deleter>
    std::unique_ptr<T, Deleter> SingletonWrapper<T, Deleter>::object_(nullptr);

    template<class T, class Deleter = std::default_delete<T>>
    using wrapper = SingletonWrapper<T, Deleter>;
}

#endif //SINGLETONWRAPPER_SINGLETONWRAPPER_H
