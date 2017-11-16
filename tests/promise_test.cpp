#include "flatten.h"

#include <thread>
#include <gtest/gtest.h>

TEST(promise, IsReady) {
    Promise<int> promise;
    Future<int> f = promise.GetFuture();


    ASSERT_FALSE(f.IsReady());

    int const x = 10;
    promise.Set(x);
    ASSERT_TRUE(f.IsReady());
}

TEST(promise, Set_int) {
    Promise<int> promise;
    Future<int> f = promise.GetFuture();

    int const x = 10;
    promise.Set(x);
    ASSERT_EQ(f.Get(), x);
}

TEST(promise, Set_int_ampersand) {
    Promise<int &> promise;
    Future<int &> f = promise.GetFuture();

    int const y = 100;
    int x = 10;

    promise.Set(x);

    f.Get() = y;

    ASSERT_EQ(x, y);
}

TEST(promise, Set_void) {
    Promise<void> promise;
    Future<void> f = promise.GetFuture();

    ASSERT_FALSE(f.IsReady());

    promise.Set();

    f.Get();

    ASSERT_TRUE(f.IsReady());
}

TEST(promise, two_threads_get_set) {
    Promise<int> promise;
    Future<int> future = promise.GetFuture();

    int x = 777;

    std::thread t([x](Promise<int> promise) {
                      std::this_thread::sleep_for(std::chrono::seconds(1));
                      promise.Set(x);
                  }, std::move(promise)
    );

    for (int i = 0; !future.IsReady(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_EQ(future.Get(), x);

    t.join();
}

TEST(promise, two_threads_get_throw_exception) {
    Promise<int> promise;
    Future<int> future = promise.GetFuture();

    int x = 777;

    std::thread t(
            [x](Promise<int> promise) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }, std::move(promise)
    );

    ASSERT_ANY_THROW(future.Get());

    t.join();
}

TEST(flatten, flatten_future) {
    Promise<int> promise;
    Future<int> future = promise.GetFuture();
    Promise<int> promise1;
    Future<int> future1 = promise1.GetFuture();

    promise.Set(10);
    promise1.Set(10);
    Future<int> future3 = Flatten(future);
    ASSERT_EQ(future1.Get(), future3.Get());
}

TEST(flatten, flatten_a_lot_of_futures) {
    Promise<int> promise;
    Future<int> future = promise.GetFuture();
    promise.Set(10);

    Promise<int> promise1;
    Future<int> future1 = promise1.GetFuture();
    promise1.Set(10);

    Promise<Future<int>> promise2;
    Future<Future<int>> future2 = promise2.GetFuture();
    promise2.Set(future1);

    Promise<Future<Future<int>>> promise3;
    Future<Future<Future<int>>> future3 = promise3.GetFuture();
    promise3.Set(future2);

    Future<int> future4 = Flatten(future3);
    ASSERT_EQ(future.Get(), future4.Get());
}

TEST(flatten, flatten_vector) {
    std::vector<int> vv = {1, 2};
    std::vector<Future<int>> v;
    Promise<int> one;
    Promise<int> two;
    Future<int> onef = one.GetFuture();
    Future<int> twof = two.GetFuture();
    one.Set(1);
    two.Set(2);
    v.emplace_back(std::move(onef));
    v.emplace_back(std::move(twof));
    Future<std::vector<int>> out = Flatte(v);
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(vv[i],v[i].Get());
    }
}

