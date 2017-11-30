
#include <gtest/gtest.h>
#include <thread>
#include "future.h"
#include "promise.h"
#include "flatten.h"

using namespace std;

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}