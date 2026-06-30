cmake_minimum_required(VERSION 3.16)
project(TJC_Studio VERSION 1.7.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(TJC_Studio
    src/cli/main.cpp
)
