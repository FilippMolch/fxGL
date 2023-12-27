cmake_minimum_required(VERSION 3.26)
project(soft_3d C)

set(CMAKE_C_STANDARD 99)

add_executable(soft_3d
        src/main.c
        src/fxGL/fxMath.c
        src/fxGL/fxGL.c
        src/fxGL/fxRasterize.c
        include/obj_loader.h
        include/fxGL/fxShader.h)

target_include_directories(soft_3d PUBLIC "include/")
