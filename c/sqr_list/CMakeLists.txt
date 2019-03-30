cmake_minimum_required(VERSION 3.8)
project(sqr_list)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c sqr_list.h sqr_list.c)
add_executable(sqr_list ${SOURCE_FILES})