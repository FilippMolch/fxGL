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

    if(mat_1.init + mat_2.init == MAT_INIT*2) {

        vec4 *col_mult[4 * 4];

        for (int i = 0; i < 4 * 4; i++)
            col_mult[i] = vec4_init();

        int mat_2_row_offset = 0;

        float row_mat_element[4] = {0.0f, 0.0f, 0.0f, 0.0f};

        for (int i = 0; i < 4 * 4; i += 4) {
            for (int k = 0; k < 4; k++) {
                *col_mult[0 + i + k]->x = mat_1.mat[0][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
                *col_mult[0 + i + k]->y = mat_1.mat[1][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
                *col_mult[0 + i + k]->z = mat_1.mat[2][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];
                *col_mult[0 + i + k]->w = mat_1.mat[3][k] * mat_2.mat[k - mat_2_row_offset][i + mat_2_row_offset];

                for (int i = 0; i < 4; i++)
                    row_mat_element[i] += col_mult[0 + i + k]->vec[i];
            }

            for (int k = 0; k < 4; k++)
                final.mat[k][i / 4] = row_mat_element[k];

            for (int j = 0; j < 4; ++j)
                row_mat_element[j] = 0.0f;

            mat_2_row_offset++;
        }
    }
    else{
        printf("MAT NOT INIT \n");
    }

    return final;
}

vec4* mat4_vec4_mult(mat4 mat, vec4 vec){
    static vec4 *final;
    final = vec4_init();

    if(mat.init + vec.init == MAT_INIT + VEC_INIT) {
        float vec_element = 0.0f;

        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y)
                vec_element += mat.mat[x][y] * vec.vec[y];

            final->vec[x] = vec_element;
            vec_element = 0.0f;
        }
    }
    else{
        printf("VEC or MAT NOT INIT \n");
    }

    return final;
}

void mat4_scale(mat4 *mat, vec3 vec){

    if (mat->init + vec.init == MAT_INIT + VEC_INIT){

        mat->mat[0][0] = vec.vec[0];
        mat->mat[1][1] = vec.vec[1];
        mat->mat[2][2] = vec.vec[2];

    }
    else{
        printf("MAT OR VEC NOT INIT \n");
    }

}

void mat4_translate(mat4 *mat, vec3 vec){

    if (mat->init + vec.init == MAT_INIT + VEC_INIT) {

        mat->mat[0][3] = vec.vec[0];
        mat->mat[1][3] = vec.vec[1];
        mat->mat[2][3] = vec.vec[2];

    }
    else{
        printf("MAT OR VEC NOT INIT \n");
    }
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