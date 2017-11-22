#include "promise.h"

template<typename T>
struct get_inner_type
{
    typedef T type;
};

template<template<typename> class E, typename T>
struct get_inner_type<E<T>>
{
    typedef typename get_inner_type<T>::type type;
};

template<typename T>
using get_type = typename get_inner_type<T>::type;

//template<typename _T>
//Future<_T> Flatten(Future<_T> const &fut) {
//    Promise<_T> p;
//    auto ret_fut = p.GetFuture();
//    p.Set(fut.Get());
//    return ret_fut;
//};
template<typename _T>
Future<_T> Flatten(Future<_T> &&fut) {
    return std::move(fut);
}

//template<typename _T>
//Future<get_type<_T>>  Flatten(Future<Future<_T>> const &fut) {
//    Promise<Future<_T>> p;
//    auto ret_fut = p.GetFuture();
//    p.Set(fut.Get());
//    return Flatten(ret_fut.Get());
//}

template<typename T>
Future<get_type<T>> Flatten(Future<Future<T>> future) {
    Promise<Future<T>> promise;
    Future<Future<T>> result = promise.GetFuture();

    std::thread t([] (Promise<Future<T>> promise, Future<Future<T>> future) {
        promise.Set(std::move(future.Get()));
    }, std::move(promise), std::move(future));
    t.detach();

    return Flatten(std::move(result.Get()));
}

template<template<typename, typename...> class _C, typename _T>
Future<_C<_T>> Flatte(_C<Future<_T>> c) {
    Promise<_C<_T>> p;
    auto ret_fut = p.GetFuture();
    std::thread t([](Promise<_C<_T>> p, _C<Future<_T>> c){
        _C<_T> result;
        for (auto &future : c) {
            result.emplace_back(std::move(Flatten(std::move(future)).Get()));
        }
        p.Set(std::move(result));
    }, std::move(p), std::move(c));
    t.detach();
    return ret_fut;
}