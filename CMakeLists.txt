#set(CMAKE_CXX_STANDARD 11)


project(promise_future)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -pedantic -Wall -Wextra -Wno-char-subscripts -Wno-unused-result -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ggdb -g")

add_definitions(-D_GTEST)
#add_definitions(-DNDEBUG)
#add_definitions(-D_GLIBCXX_DEBUG)
add_subdirectory(tests/googletest-master)
include_directories(tests/googletest-master/googletest/include)
include_directories(tests/googletest-master/googlemock/include)
file(GLOB_RECURSE SOURCE_FILES source/*.cpp tests/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h tests/*.h)

include_directories(include)

if ("$ENV{GTEST}" STREQUAL "y")
    message("!Gtests're enabled!")

    find_package(GTest REQUIRED)

    include_directories(tests)
    add_definitions(-D_GTEST)
endif ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} include/flatten.h include/ThreadPool.h include/Map.h)

if ("$ENV{GTEST}" STREQUAL "y")
    target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} gmock)
endif ()

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} pthread)