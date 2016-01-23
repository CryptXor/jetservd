cmake_minimum_required(VERSION 3.3)
project(jetbrains_emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp mongoose.h mongoose.c)
add_executable(jetbrains_emulator ${SOURCE_FILES})