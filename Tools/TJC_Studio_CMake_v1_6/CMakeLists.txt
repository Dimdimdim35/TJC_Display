cmake_minimum_required(VERSION 3.16)
project(TJC_Studio VERSION 1.6.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(TJC_Studio
    src/cli/main.cpp
    src/parser/HMIParser.cpp
    src/generator/CppGenerator.cpp
    src/generator/JsonGenerator.cpp
)
target_include_directories(TJC_Studio PRIVATE include)
