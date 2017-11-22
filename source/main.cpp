
#include <gtest/gtest.h>
#include <thread>
#include "future.h"
#include "promise.h"
#include "flatten.h"

using namespace std;

int main(int argc, char *argv[])
{
//    Promise<int> p;
//    Future<int> f = p.GetFuture();
//
//    std::cerr << "before thread" << std::endl;
//    std::thread t(
//        [] (Future<int> f) {
//            std::cerr << f.Get() << std::endl;
//        },
//        std::move(f)
//    );
//
//    std::cerr << "before sleep" << std::endl;
//    std::this_thread::sleep_for(2s);
//    p.Set(10);
//
//    std::cerr << "before join" << std::endl;
//
//    t.join();
//
//    std::cerr << "end" << std::endl;
//
//
//    return 0;
//    Promise<Future<int>> pf;
//    Future<Future<int>> ff = pf.GetFuture();
//
//    Promise<int> p;
//    Future<int> f = p.GetFuture();
//
//    Future<int> f_result = Flatten(std::move(ff));
//
//    p.Set(10);
//    pf.Set(std::move(f));
//
//    std::cerr << f_result.Get() << std::endl;
//    return 0;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}