//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_LIN_MATH_H
#define SOFT_3D_LIN_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


struct mat4 {
    float mat[4][4];
};

struct vec3 {
    float vec[3];
    float *x, *y, *z;
};


struct vec4 {
    float vec[4];
    float *x, *y, *z, *w;
};

typedef struct mat4 mat4;
typedef struct vec3 vec3;
typedef struct vec4 vec4;

mat4 mat4_init(void);

vec3 vec3_init(void);
vec4 vec4_init(void);

#endif //SOFT_3D_LIN_MATH_H
