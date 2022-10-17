cmake_minimum_required(VERSION 3.23)
project(siaod2)

set(CMAKE_CXX_STANDARD 14)

add_executable(siaod2 main.cpp cmake-build-debug/hashClass.cpp cmake-build-debug/hashClass.h)
