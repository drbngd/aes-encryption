cmake_minimum_required(VERSION 3.27)
project(aes_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(aes_project src/aes.cpp
        src/main.cpp
        src/aes.h)
