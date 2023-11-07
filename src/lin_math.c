//
// Created by Filipp on 07.11.2023.
//
#include <lin_math.h>

vec3 vec3_init(void){
    vec3 vec = {0.0f, 0.0f, 0.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];

    return vec;
}

vec4 vec4_init(void){
    vec4 vec = {0.0f, 0.0f, 0.0f, 0.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];
    vec.w = &vec.vec[3];

    return vec;
}