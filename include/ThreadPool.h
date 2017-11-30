#pragma once
#include <condition_variable>
#include <iostream>
#include <functional>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

using namespace std;

class ThreadPool {
public:

    static thread_local ThreadPool * this_tp;

    ThreadPool(size_t num_threads) : check_end(0), is_finished(false) {
        for (int i = 0; i < num_threads; ++i)
            threads.emplace_back(threadStarter, this);
    };

    void execute(function<void()> const &foo) {
        unique_lock<mutex> lock(mutex_queue);
        fn_queue.emplace(foo);
        condition_queue.notify_one();
    };

    void checkEnd() {
        unique_lock<mutex> lock(mutex_queue);
        condition_end.wait(lock, [this] { return fn_queue.empty() && check_end == 0; });
    };

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(mutex_queue);
            is_finished = true;
        }
        condition_queue.notify_all();
        for (thread &t: threads) {
            t.join();
        }
    };

private:
    void threadStarter() {
        this_tp = this;
        for (;;) {
            unique_lock<mutex> lock(mutex_queue);
            condition_queue.wait(lock, [this] { return is_finished || !fn_queue.empty(); });
            if (!fn_queue.empty()) {
                check_end++;

                function<void()> func;
                func = fn_queue.front();
                fn_queue.pop();

                lock.unlock();

                func();

                lock.lock();
                check_end--;
                condition_end.notify_one();
            }
            if (is_finished) {
                return;
            }
        }
    };

    vector<thread> threads;
    queue<function<void()>> fn_queue;
    mutex mutex_queue;
    condition_variable condition_queue, condition_end;
    int check_end;
    bool is_finished;
};

thread_local ThreadPool * ThreadPool::this_tp = nullptr;