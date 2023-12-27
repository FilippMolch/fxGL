//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_FXMATH_H
#define SOFT_3D_FXMATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>

#define VEC_INIT 223
#define MAT_INIT 223

#define ANGLE_TO_RADIANS(angle) angle * 0.01745329251994329576923690768489

#define FABS(a) fabsf(a)
#define COSF(x) cosf(x)
#define SINF(x) sinf(x)
#define SQRT(x) sqrt(x)
#define POW(x, y) pow(x, y)
#define TAN(x) tanf(x)

#define VEC4_SCALAR(vec3) SQRT(POW(vec3.vec[0], 2) + POW(vec3.vec[1], 2) + POW(vec3.vec[2], 2))
//#define VEC4_SCALAR(vec3) SQRT(vec3.vec[0] + vec3.vec[1] + vec3.vec[2])

#define VEC4_NORMALIZE(vec3, SCALAR)\
                                     vec3.vec[0] = vec3.vec[0] / SCALAR; \
                                     vec3.vec[1] = vec3.vec[1] / SCALAR; \
                                     vec3.vec[2] = vec3.vec[2] / SCALAR;

#define VEC3_MINUS(vec1, vec2, pos) vec1.vec[pos] - vec2.vec[pos]
#define VEC3_PLUS(vec1, vec2, pos) vec1.vec[pos] + vec2.vec[pos]

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

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
mat4 mat4_mult_some(int count, ...);

mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane);
mat4 look_at(vec3 camera_pos, vec3 camera_target, vec3 camera_up);

vec4 mat4_vec4_mult(mat4 mat, vec4 vec);
vec3 vec3_cross(vec3 vec_1, vec3 vec_2);

vec3 inline vec3_init(void);
vec4 inline vec4_init(void);
vec3 inline vec3_get(float x, float y, float z);
vec4 inline vec4_get(float x, float y, float z, float w);

vec3 baryCoord(float spc[3][2], float P[2]);

#endif //SOFT_3D_FXMATH_H
