cmake_minimum_required(VERSION 3.8)
project(seq_que)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c seq_que.c seq_que.h)
add_executable(seq_que ${SOURCE_FILES} seq_que.c seq_que.h)