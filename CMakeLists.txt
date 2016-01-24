cmake_minimum_required(VERSION 3.3)
project(jetbrains_emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_D daemon.cpp mongoose.h mongoose.c)
add_executable(jetservd ${SOURCE_FILES_D})
set(SOURCE_FILES_N nondaemon.cpp mongoose.h mongoose.c)
add_executable(jetserv ${SOURCE_FILES_N})