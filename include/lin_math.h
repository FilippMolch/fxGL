//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_LIN_MATH_H
#define SOFT_3D_LIN_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define VEC_INIT 223
#define MAT_INIT 223

#define ROTATE_AXIS_X 0
#define ROTATE_AXIS_Y 1
#define ROTATE_AXIS_Z 2
#define NOT_ROTATE 3

#define ANGLE_TO_RADIANS(angle) angle * (M_PI / 180.0f)

#define FABS(a) fabs(a)
#define COSF(x) cosf(x)
#define SINF(x) sinf(x)
#define SQRT(x) sqrt(x)
#define POW(x, y) pow(x, y)

#define VEC4_SCALAR(vec3) SQRT(POW(vec3.vec[0], 2) + POW(vec3.vec[1], 2) + POW(vec3.vec[2], 2))

#define VEC4_NORMALIZE(vec3, SCALAR) vec3.vec[0] = vec3.vec[0] / SCALAR; \
                                     vec3.vec[1] = vec3.vec[1] / SCALAR; \
                                     vec3.vec[2] = vec3.vec[2] / SCALAR;


struct mat4 {
    float mat[4][4];
    uint8_t init;
};

struct vec3 {
    float vec[3];
    uint8_t init;
};

struct vec4 {
    float vec[4];
    uint8_t init;
};

typedef struct mat4 mat4;
typedef struct vec3 vec3;
typedef struct vec4 vec4;

mat4 mat4_init(uint8_t mode);
mat4 mat4_mult(mat4 mat_1, mat4 mat_2);
void mat4_scale(mat4 *mat, vec3 vec);
void mat4_translate(mat4 *mat, vec3 vec);
void mat4_rotate(mat4 *mat, float angle, vec3 vec);
mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane);

vec4 mat4_vec4_mult(mat4 mat, vec4 vec);
vec3 vec3_init(void);
vec4 vec4_init(void);

#endif //SOFT_3D_LIN_MATH_H
