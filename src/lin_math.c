//
// Created by Filipp on 07.11.2023.
//
#include <lin_math.h>

mat4 mat4_init(void){
    mat4 mat;

    float fill_arr[] = {0.0f, 0.0f};

    for (int i = 0; i < 8; ++i) {
        ((uint64_t*)mat.mat)[i] = *(uint64_t*)fill_arr;
    }

    int k = 0;
    for (int i = 0; i < 4; ++i) {
        mat.mat[i][k] = 1.0f;
        k++;
    }

    return mat;
}

vec3 vec3_init(void){
    vec3 vec = {0.0f, 0.0f, 0.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];

    return vec;
}

vec4 vec4_init(void){
    vec4 vec = {0.0f, 0.0f, 0.0f, 1.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];
    vec.w = &vec.vec[3];

    return vec;
}