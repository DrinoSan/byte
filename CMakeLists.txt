cmake_minimum_required(VERSION 3.12)

project(ArpChat)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_COMPILER clang++)


# Create the executable target
add_executable(main main.cpp chunk.cpp memory.cpp debug.cpp value.cpp)
