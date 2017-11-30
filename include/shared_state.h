#pragma once
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <exception>
#include <memory>
#include <atomic>

template<typename _T>
struct SharedState {
    SharedState() : ready(false), still_promise(false) {};

    std::atomic_bool ready, still_promise;
    std::exception_ptr e_ptr;
    _T data;
    std::mutex mut;
    std::condition_variable cond;
};

//template<typename _T>
//struct SharedState<_T &> {
//    SharedState() : ready(false), still_promise(false) {};
//
//    std::atomic_bool ready, still_promise;
//    std::exception_ptr e_ptr;
//    _T *data;
//    std::mutex mut;
//    std::condition_variable cond;
//};
//
//template<>
//struct SharedState<void> {
//    SharedState() : ready(false), still_promise(false) {};
//
//    std::atomic_bool ready, still_promise;
//    std::exception_ptr e_ptr;
//    std::mutex mut;
//    std::condition_variable cond;
//};
