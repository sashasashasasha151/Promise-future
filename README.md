# Implementation of ThreadPool, Promise, Future and connected functions
# ThreadPool
Takes number of threads and execute your functions
### ThreadPool semantic
```c++
class ThreadPool {
public:
    static thread_local ThreadPool * this_tp;

    ThreadPool(size_t num_threads);

    void execute(function<void()> const &foo);
}
```
# Future and Promise
Future and Promise work the same way as std::future and std::promise
### Future and Promise semantic
```c++
template<typename _T>
class Future {
public:
    Future();

    Future(std::shared_ptr<SharedState<_T>> come_in_state) : state(come_in_state);

    Future(const Future &) noexcept;

    Future(Future &&) noexcept;

    Future &operator=(const Future &) noexcept;

    Future &operator=(Future &&) noexcept;

    _T &  Get() const;

    bool IsReady();

    void Wait() const;
}
```
```c++
template<typename _T>
class Promise {
public:
    Promise();

    Promise(const Promise &);

    Promise(Promise &&);

    Promise &operator=(const Promise &);

    Promise &operator=(Promise &&);

    void Set(_T const &value);

    void Set(_T &&value);

    Future<_T> GetFuture();

    void SetException(const std::exception_ptr p);
}
```
# Flatten
Reveals nested `Future` in one
### Flatten semantic
```c++
template<typename T>
Future<T> Flatten(Future<T> &&fut);

template<typename T>
Future<T> Flatten(Future<Future<T>> future);

template<template<typename, typename...> class C, typename T>
Future<C<T>> Flatte(C<Future<T>> c);
```
# Map
Returns `Future` from applying an argument contained in `future` to a function `F`
### Flatten semantic
```c++
template<typename T, typename F>
Future<F(T)> Map(Future<T> const &future, const F &f);
```
# Tests
You can run google tests using [___main.cpp___](https://gitlab.com/sashasashasasha151/future/blob/master/source/main.cpp) and add your own to [___either_tests.cpp___](https://gitlab.com/sashasashasasha151/future/blob/master/tests/promise_test.cpp)