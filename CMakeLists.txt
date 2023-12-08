cmake_minimum_required(VERSION 3.26)
project(soft_3d C)

set(CMAKE_C_STANDARD 11)

add_executable(soft_3d
        src/main.c
        src/lin_math.c
        src/engine.c)

target_include_directories(soft_3d PUBLIC "include/")
