#pragma once
#include <condition_variable>
#include <iostream>
#include <functional>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <atomic>

using namespace std;

class ThreadPool {
public:

    static thread_local ThreadPool * this_tp;

    ThreadPool(size_t num_threads) : is_finished(false) {
        for (int i = 0; i < num_threads; ++i)
            threads.emplace_back(&threadStarter, this);
    };

    void execute(function<void()> const &foo) {
        mutex_queue.lock();
        fn_queue.push(foo);
        mutex_queue.unlock();
        condition_queue.notify_one();
    };

    ~ThreadPool() {
        is_finished = true;
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
            while(!is_finished.load() && fn_queue.empty()) {
                condition_queue.wait(lock);
            }
            if (!fn_queue.empty() && !is_finished.load()) {

                function<void()> func = fn_queue.front();
                fn_queue.pop();

                lock.unlock();

                func();

                lock.lock();
            }
            if (is_finished.load()) {
                return;
            }
        }
    };

    vector<thread> threads;
    queue<function<void()>> fn_queue;
    mutex mutex_queue;
    condition_variable condition_queue, condition_end;
    atomic_bool is_finished;
};

thread_local ThreadPool * ThreadPool::this_tp = nullptr;