#pragma once
#include "shared_state.h"

template<typename _T>
class Future {
    std::shared_ptr<SharedState<_T>> state;
public:

    Future() {
        state = std::shared_ptr<SharedState<_T>>(new SharedState<_T>());
    };

    Future(std::shared_ptr<SharedState<_T>> come_in_state) : state(come_in_state) {};

    ~Future() {};

    Future(const Future &) = delete;

    Future &operator=(const Future &) = delete;

    Future(Future &&other) noexcept : state(nullptr) {
        state = other.state;
        other.state = nullptr;
    }

    Future &operator=(Future &&other) noexcept  {
        if (this != &other) {
            state = other.state;
            other.state = nullptr;
        }
        return *this;
    }

    _T &  Get() const {

        if (!state->still_promise) {
            throw std::runtime_error("No promise or result");
        }
        Wait();
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise and result");
        }
        if (state->e_ptr != NULL) {
            throw state->e_ptr;
        }
        Wait();
        return state->data;
    }

    bool IsReady() {
        return state->ready.load();
    }

    void Wait() const {
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise");
        }
        if (state->ready) {
            return;
        }
        {
            std::unique_lock<std::mutex> lock(state->mut);
            state->cond.wait(lock, [this] { return state->ready.load(); });
        }
    }
};

template<typename _T>
class Future<_T &> {
    std::shared_ptr<SharedState<_T &>> state;
public:
    Future(std::shared_ptr<SharedState<_T &>> come_in_state) : state(come_in_state) {};

    ~Future() {};

    Future(const Future &) = delete;

    Future &operator=(const Future &) = delete;

    Future(Future &&other) : state(nullptr) {
        state = other.state;
        other.state = nullptr;
    }

    Future &operator=(Future &&other) {
        if (this != &other) {
            state = other.state;
            other.state = nullptr;
        }
        return *this;
    }

    _T &Get() {
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise and result");
        }
        if (state->e_ptr != NULL) {
            throw state->e_ptr;
        }
        _T *res;
        {
            std::lock_guard<std::mutex> lock(state->mut);
            if (!state->ready) {
                throw std::runtime_error("Result is not already ready");
            }
            res = state->data;
        }
        state->cond.notify_all();
        return *res;
    }

    bool IsReady() {
        bool ready;
        {
            std::lock_guard<std::mutex> lock(state->mut);
            ready = state->ready;
        }
        state->cond.notify_all();
        return ready;
    }

    void Wait() {
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise");
        }
        if (state->ready) {
            return;
        }
        {
            std::unique_lock<std::mutex> lock(state->mut);
            state->cond.wait(lock, [this] { return state->ready.load(); });
        }
    }
};

template<>
class Future<void> {
    std::shared_ptr<SharedState<void>> state;
public:
    Future(std::shared_ptr<SharedState<void>> come_in_state) : state(come_in_state) {}

    void Get() {
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise and result");
        }
        if (state->e_ptr != NULL) {
            throw state->e_ptr;
        }
        {
            std::lock_guard<std::mutex> lock(state->mut);
            if (!state->ready) {
                throw std::runtime_error("Result is not already ready");
            }
        }
    }

    bool IsReady() {
        bool ready;
        {
            std::lock_guard<std::mutex> lock(state->mut);
            ready = state->ready;
        }
        state->cond.notify_all();
        return ready;
    }

    void Wait() {
        if (!state->still_promise && !state->ready) {
            throw std::runtime_error("No promise");
        }
        if (state->ready) {
            return;
        }
        {
            std::unique_lock<std::mutex> lock(state->mut);
            state->cond.wait(lock, [this] { return state->ready.load(); });
        }
    }
};
