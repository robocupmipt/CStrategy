cmake_minimum_required(VERSION 3.13)
project(CStrategy)

set(CMAKE_CXX_STANDARD 14)

add_executable(CStrategy main.cpp KernelStrategy.cpp KernelStrategy.hpp)