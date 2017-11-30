#pragma once

#include "future.h"

template<typename _T>
class Promise {
public:
    std::shared_ptr<SharedState<_T>> state;

    Promise() {
        state = std::shared_ptr<SharedState<_T>>(new SharedState<_T>());
        state->still_promise = std::move(true);
    };

    ~Promise() {
        if (state != NULL) {
            {
                std::lock_guard<std::mutex> lock(state->mut);
                state->still_promise = false;
            }
            state->cond.notify_all();
        }
    };

    Promise(const Promise &) = delete;

    Promise(Promise &&) = default;

    Promise &operator=(const Promise &) = delete;

    Promise &operator=(Promise &&) = default;

    void Set(_T const &value) {
        if (state->ready.load()) {
            throw std::runtime_error("Result is already ready");
        }

            std::lock_guard<std::mutex> lock(state->mut);
            state->data = value;
            state->ready = true;

        state->cond.notify_all();
    };

    void Set(_T &&value) {
        if (state->ready.load()) {
            throw std::runtime_error("Result is already set");
        }

            std::lock_guard<std::mutex> lock(state->mut);
            state->data = std::move(value);
            state->ready = true;

        state->cond.notify_all();
    };

    Future<_T> GetFuture() {
        return Future<_T>(state);
    };

    void SetException(const std::exception_ptr p) {
        if (state->ready.load()) {
            throw std::runtime_error("Result is already set");
        }

            std::lock_guard<std::mutex> lock(state->mut);
            state->e_ptr = p;
            state->ready = true;

        state->cond.notify_all();
    };

};

//template<typename _T>
//class Promise<_T &> {
//    std::shared_ptr<SharedState<_T &>> state;
//public:
//    Promise() {
//        state = std::shared_ptr<SharedState<_T &>>(new SharedState<_T &>());
//        state->still_promise = true;
//    };
//
//    ~Promise() {
//        {
//            std::lock_guard<std::mutex> lock(state->mut);
//            state->still_promise = false;
//        }
//        state->cond.notify_all();
//    };
//
//    Promise(const Promise &) = delete;
//
//    Promise &operator=(const Promise &) = delete;
//
//    Promise(Promise &&other) : state(nullptr) {
//        state = other.state;
//        other.state = nullptr;
//    }
//
//    Promise &operator=(Promise &&other) {
//        if (this != &other) {
//            state = other.state;
//            other.state = nullptr;
//        }
//        return *this;
//    }
//
//    void Set(_T &value) {
//        {
//            std::lock_guard<std::mutex> lock(state->mut);
//            if (state->ready) {
//                throw std::runtime_error("Result is already ready");
//            }
//            state->data = &value;
//            state->ready = true;
//        }
//        state->cond.notify_all();
//    }
//
//    Future<_T &> GetFuture() {
//        return Future<_T &>(state);
//    }
//
//    void SetException(std::exception_ptr p) {
//        {
//            std::lock_guard<std::mutex> lock(state->mut);
//            state->e_ptr = p;
//        }
//        state->cond.notify_all();
//    };
//};
//
//template<>
//class Promise<void> {
//    std::shared_ptr<SharedState<void>> state;
//public:
//    Promise() {
//        state = std::shared_ptr<SharedState<void>>(new SharedState<void>());
//        state->still_promise = true;
//    };
//
//    ~Promise() {
//        if (state != NULL) {
//            {
//                std::lock_guard<std::mutex> lock(state->mut);
//                state->still_promise = false;
//            }
//            state->cond.notify_all();
//        }
//    };
//
//    Promise(const Promise &) = delete;
//
//    Promise &operator=(const Promise &) = delete;
//
//    Promise(Promise &&other) : state(nullptr) {
//        state = other.state;
//        other.state = nullptr;
//    }
//
//    Promise &operator=(Promise &&other) {
//        if (this != &other) {
//            state = other.state;
//            other.state = nullptr;
//        }
//        return *this;
//    }
//
//    void Set() {
//        {
//            std::lock_guard<std::mutex> lock(state->mut);
//            if (state->ready) {
//                throw std::runtime_error("Result is already ready");
//            }
//            state->ready = true;
//        }
//        state->cond.notify_all();
//    };
//
//    Future<void> GetFuture() {
//        return Future<void>(state);
//    };
//
//    void SetException(std::exception_ptr p) {
//        {
//            std::lock_guard<std::mutex> lock(state->mut);
//            state->e_ptr = p;
//        }
//        state->cond.notify_all();
//    };
//};
