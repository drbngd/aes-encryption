cmake_minimum_required(VERSION 3.10)
project(aes_m13272240)

# Locate GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_compile_options(-std=c++17)

#set(CMAKE_CXX_STANDARD 17)

add_executable(aes_m13272240 src/main.cpp
        src/aes.h
        src/aes.cpp)
