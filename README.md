C++ realization of ThreadPool, standart promise and future classes and other 
---

```
class ThreadPool {
    ThreadPool(size_t num_threads)
    void execute(function<void()> const &foo)
}
```

```
template<typename _T>
class Promise {
    Promise() 

    void Set(_T const &value) 

    void Set(_T &&value) 

    Future<_T> GetFuture() 

    void SetException(const std::exception_ptr p)
}
```

```
template<typename _T>
class Future {
    Future() 

    Future(std::shared_ptr<SharedState<_T>> come_in_state)

    _T &  Get() 

    bool IsReady() 

    void Wait()
}
```

```
template<typename _T>
Future<_T> Flatten(Future<_T> &&fut)

template<typename T>
Future<get_type<T>> Flatten(Future<Future<T>> future)

template<template<typename, typename...> class _C, typename _T>
Future<_C<_T>> Flatte(_C<Future<_T>> c) 
```

```
template<typename T, typename F>
Future<std::result_of_t<F(T)>> Map(Future<T> const &future, const F &f)
```
