cmake_minimum_required(VERSION 3.10)
project(Huffmanocool C)

set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(SOURCE_FILES main.c src/assist.c src/sort.c src/argdefs.c src/fileloader.c src/debug.c src/huffmanstructs.c src/bytetable.c src/packing.c)
add_executable(huffmanocool ${SOURCE_FILES})
