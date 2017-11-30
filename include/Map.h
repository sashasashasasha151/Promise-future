#pragma once

#include <type_traits>

#include "flatten.h"
#include "ThreadPool.h"

template<typename T, typename F>
Future<std::result_of_t<F(T)>> Map(Future<T> const &future, const F &f) {
    ThreadPool *th = ThreadPool::this_tp;
    std::shared_ptr<Promise<std::result_of_t<F(T)>>> promise = std::shared_ptr<Promise<std::result_of_t<F(T)>>>(
            new Promise<std::result_of_t<F(T)>>());
    auto fut = promise->GetFuture();
    if (th != nullptr) {
        th->execute([promise, &future, &f]() {
            T data = future.Get();
            std::result_of_t<F(T)> result = f(data);
            promise->Set(std::move(result));
        });
    } else {
        std::thread t([promise, &future, &f]() {
            T data = future.Get();
            std::result_of_t<F(T)> result = f(data);
            promise->Set(std::move(result));
        });
        t.detach();
    }
    fut.Wait();
    return std::move(fut);
}
