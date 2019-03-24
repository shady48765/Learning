cmake_minimum_required(VERSION 3.10)
project(unidirection_link_list)

set(CMAKE_C_STANDARD 99)

aux_source_directory(. Dir_work_source)


add_executable(undir_list ${Dir_work_source})