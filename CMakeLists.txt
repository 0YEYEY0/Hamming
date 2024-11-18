cmake_minimum_required(VERSION 3.29)
project(Hamming)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hamming main.cpp
        hamming.cpp
        hammingx.h)
