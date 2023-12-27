//
// Created by Filipp on 07.11.2023.
//
#include "fxGL/fxMath.h"

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

mat4 mat4_mult_some(int count, ...){
    mat4 final = mat4_init(1);

    va_list factor;
    va_start(factor, count);

    mat4 *mat = calloc(count, sizeof(mat4));

    for (int i = 0; (i < count); i++)
        mat[i] = va_arg(factor, mat4);

    for (int i = count - 1; i >= 0; i--)
        final = mat4_mult(mat[i], final);

    va_end(factor);
    free(mat);

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
    mat4 final = mat4_init(0);

    float tan_half_fov = TAN(ANGLE_TO_RADIANS(fov) / 2);

    final.mat[0][0] = 1 / (aspect * tan_half_fov);
    final.mat[1][1] = 1 / (tan_half_fov);
    final.mat[2][2] = far_plane / (far_plane - near_plane);
    final.mat[3][2] = 1;
    final.mat[2][3] = -(far_plane * near_plane) / (far_plane - near_plane);

    return final;
}

mat4 look_at(vec3 camera_pos, vec3 camera_target, vec3 camera_up){
    mat4 camera = mat4_init(1);
    mat4 pos = mat4_init(1);

    float scalar = 0.0f;

    vec3 z_axis = {VEC3_MINUS(camera_pos, camera_target, 0),
                   VEC3_MINUS(camera_pos, camera_target, 1),
                   VEC3_MINUS(camera_pos, camera_target, 2), VEC_INIT};
    scalar = VEC4_SCALAR(z_axis);
    VEC4_NORMALIZE(z_axis, scalar);

    vec3 normalize_camera_up = camera_up;
    scalar = VEC4_SCALAR(normalize_camera_up);
    VEC4_NORMALIZE(normalize_camera_up, scalar);

    vec3 x_axis = vec3_cross(normalize_camera_up, z_axis);
    scalar = VEC4_SCALAR(x_axis);
    VEC4_NORMALIZE(x_axis, scalar);

    vec3 y_axis = vec3_cross(z_axis, x_axis);

    camera.mat[0][0] = x_axis.vec[0];
    camera.mat[0][1] = x_axis.vec[1];
    camera.mat[0][2] = x_axis.vec[2];
    camera.mat[1][0] = y_axis.vec[0];
    camera.mat[1][1] = y_axis.vec[1];
    camera.mat[1][2] = y_axis.vec[2];
    camera.mat[2][0] = z_axis.vec[0];
    camera.mat[2][1] = z_axis.vec[1];
    camera.mat[2][2] = z_axis.vec[2];

    mat4_translate(&pos, (vec3){     -camera_pos.vec[0],
                                             -camera_pos.vec[1],
                                             -camera_pos.vec[2], VEC_INIT});
    mat4 final = mat4_mult(camera, pos);

    return final;
}

vec3 vec3_cross(vec3 vec_1, vec3 vec_2){
    vec3 final = vec3_init();

    final.vec[0] = (vec_1.vec[1] * vec_2.vec[2]) - (vec_1.vec[2] * vec_2.vec[1]);
    final.vec[1] = (vec_1.vec[2] * vec_2.vec[0]) - (vec_1.vec[0] * vec_2.vec[2]);
    final.vec[2] = (vec_1.vec[0] * vec_2.vec[1]) - (vec_1.vec[1] * vec_2.vec[0]);

    return final;
}

vec3 baryCoord(float spc[3][2], float P[2]){

    vec3 vec_1 = vec3_get(spc[2][0] - spc[0][0], spc[1][0] - spc[0][0], spc[0][0] - P[0]);
    vec3 vec_2 = vec3_get(spc[2][1] - spc[0][1], spc[1][1] - spc[0][1], spc[0][1] - P[1]);
    vec3 u = vec3_cross(vec_1, vec_2);

    if (FABS(u.vec[2]) < 1.0f){
        return vec3_get(-1.0f, 1.0f, 1.0f);
    }

    return vec3_get(1.0f - (u.vec[0] + u.vec[1]) / u.vec[2],
                    u.vec[1] / u.vec[2],
                    u.vec[0] / u.vec[2]);
}

vec3 vec3_init(void){
    return (vec3){0.0f, 0.0f, 0.0f, VEC_INIT};
}

vec4 vec4_init(void){
    return (vec4){0.0f, 0.0f, 0.0f, 1.0f, VEC_INIT};
}

vec3 vec3_get(float x, float y, float z){
    return (vec3){x, y, z, VEC_INIT};
}

vec4 vec4_get(float x, float y, float z, float w){
    return (vec4){x, y, z, w, VEC_INIT};
}