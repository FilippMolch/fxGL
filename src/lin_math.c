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

        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                final.mat[x][y] =   mat_1.mat[x][0] * mat_2.mat[0][y] +
                                    mat_1.mat[x][1] * mat_2.mat[1][y] +
                                    mat_1.mat[x][2] * mat_2.mat[2][y] +
                                    mat_1.mat[x][3] * mat_2.mat[3][y];
            }
        }

    }
    else{
        printf("MAT NOT INIT \n");
    }

    return final;
}

vec4 mat4_vec4_mult(mat4 mat, vec4 vec){
    vec4 final = vec4_init();

    if(mat.init + vec.init == MAT_INIT + VEC_INIT) {
        float vec_element = 0.0f;

        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y)
                vec_element += mat.mat[x][y] * vec.vec[y];

            final.vec[x] = vec_element;
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

void mat4_rotate(mat4 *mat, float angle, vec3 vec){

    float radians = ANGLE_TO_RADIANS(angle);

    float sin_rad = SINF(radians);
    float cos_rad = COSF(radians);
    float cos_rad_min = 1.0f - COSF(radians);

    float scalar = VEC4_SCALAR(vec);
    VEC4_NORMALIZE(vec, scalar);

    float r_x = vec.vec[0], r_y = vec.vec[1], r_z = vec.vec[2];


    mat->mat[0][0] = cos_rad + POW(r_x, 2) * cos_rad_min;
    mat->mat[0][1] = r_x * r_y * cos_rad_min - r_z * sin_rad;
    mat->mat[0][2] = r_x * r_z * cos_rad_min + r_y * sin_rad;

    mat->mat[1][0] = r_y * r_x * cos_rad_min + r_z * sin_rad;
    mat->mat[1][1] = cos_rad + POW(r_y, 2) * cos_rad_min;
    mat->mat[1][2] = r_y * r_z * cos_rad_min - r_x * sin_rad;

    mat->mat[2][0] = r_z * r_x * cos_rad_min - r_y * sin_rad;
    mat->mat[2][1] = r_z * r_y * cos_rad_min + r_x * sin_rad;
    mat->mat[2][2] = cos_rad + POW(r_z, 2) * cos_rad_min;
}

mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane){

}

vec3 vec3_init(void){
    return (vec3){0.0f, 0.0f, 0.0f, VEC_INIT};
}

vec4 vec4_init(void){
    return (vec4){0.0f, 0.0f, 0.0f, 1.0f, VEC_INIT};
}