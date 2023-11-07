//
// Created by Filipp on 07.11.2023.
//
#include <lin_math.h>

mat4 mat4_init(uint8_t mode){
    mat4 mat;

    float fill_arr[] = {0.0f, 0.0f};

    for (int i = 0; i < 8; ++i) {
        ((uint64_t*)mat.mat)[i] = *(uint64_t*)fill_arr;
    }

    if (mode){
        int k = 0;
        for (int i = 0; i < 4; ++i) {
            mat.mat[i][k] = 1.0f;
            k++;
        }
    }

    mat.init = MAT_INIT;
    return mat;
}

mat4 mat4_mult(mat4 mat_1, mat4 mat_2){
    mat4 final;
    final = mat4_init(0);

    vec4* col_mult[4*4];

    for (int i = 0; i < 4*4; i++)
        col_mult[i] = vec4_init();

    int mat_2_row_offset = 0;

    float row_mat_element[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 4*4; i += 4) {
        for (int k = 0; k < 4; k++) {
            *col_mult[0 + i + k]->x = mat_1.mat[0][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
            *col_mult[0 + i + k]->y = mat_1.mat[1][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
            *col_mult[0 + i + k]->z = mat_1.mat[2][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
            *col_mult[0 + i + k]->w = mat_1.mat[3][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];

            for (int i = 0; i < 4; i++)
                row_mat_element[i] += col_mult[0 + i + k]->vec[i];
        }

        for (int k = 0; k < 4; k++)
           final.mat[k][i/4] = row_mat_element[k];

        for (int j = 0; j < 4; ++j)
            row_mat_element[j] = 0.0f;

        mat_2_row_offset++;
    }

    return final;
}

vec3* vec3_init(void){
    static vec3 vec = {0.0f, 0.0f, 0.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];

    vec.init = VEC_INIT;
    return &vec;
}

vec4* vec4_init(void){
    static vec4 vec = {0.0f, 0.0f, 0.0f, 1.0f};

    vec.x = &vec.vec[0];
    vec.y = &vec.vec[1];
    vec.z = &vec.vec[2];
    vec.w = &vec.vec[3];

    vec.init = VEC_INIT;
    return &vec;
}

