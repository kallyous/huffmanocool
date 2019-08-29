cmake_minimum_required(VERSION 3.10)
project(Huffmanocool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES main.c assist.c sort.c argdefs.c fileloader.c stdoutdebug.c huffmanstructs.c bytetable.c packing.c)
add_executable(huffmanocool ${SOURCE_FILES})
