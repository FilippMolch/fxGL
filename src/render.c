//
// Created by Filipp on 07.11.2023.
//
#include <render.h>

void draw_line(screen *scr, int x1, int y1, int x2, int y2, uint8_t color){
    const int delta_x = FABS(x2 - x1);
    const int delta_y = FABS(y2 - y1);
    const int sign_x = x1 < x2? 1 : -1;
    const int sign_y = y1 < y2? 1 : -1;

    int error = delta_x - delta_y;

    SET_PIXEL(scr, x2, y2, color);

    while (x1 != x2 || y1 != y2){
        SET_PIXEL(scr, x1, y1, color);
        int error2 = error * 2;

        if (error2 > -delta_y){
            error -= delta_y;
            x1 += sign_x;
        }

        if (error2 < delta_x){
            error += delta_x;
            y1 += sign_y;
        }
    }

}

screen_coord translate_coord(float x, float y){
    screen_coord scr_crd;

    scr_crd.x = (int)((x + 1.0f) * 0.5f * (W - 1));
    scr_crd.y = (int)((y + 1.0f) * 0.5f * (H - 1));

    return scr_crd;
}

void* apply_matrix_to_arr(mat4 matrix, void* arr, int count){
    void *memory = calloc(count, sizeof(float));
    float *arr_float = ((float*)arr);
    float *new_arr = ((float*)memory);

    vec4 mult = vec4_init();

    for (int i = 0; i < count; i += 4) {

        mult.vec[0] = arr_float[i];
        mult.vec[1] = arr_float[i + 1];
        mult.vec[2] = arr_float[i + 2];
        mult.vec[3] = arr_float[i + 3];

        vec4 new_coord = mat4_vec4_mult(matrix, mult);

        new_arr[i    ] = new_coord.vec[0];
        new_arr[i + 1] = new_coord.vec[1];
        new_arr[i + 2] = new_coord.vec[2];
        new_arr[i + 3] = new_coord.vec[2];

    }

    return new_arr;
}

void perspective_divide_arr(float* arr_float, int count){

    for (int buf = 0; buf < count; buf += 4) {
        arr_float[buf    ] /= arr_float[buf + 3];
        arr_float[buf + 1] /= arr_float[buf + 3];
        arr_float[buf + 2] /= arr_float[buf + 3];
    }

}

void draw_primitive_arr(screen *scr, int primitive_type, void* array, int primitive_count, uint8_t color){
    float *arr_float = ((float*)array);

    int16_f *arr = (int16_f*)malloc(PRIMITIVE_ARR_SIZE * sizeof(int16_f ));

    memset(arr, 0, PRIMITIVE_ARR_SIZE * sizeof(int16_f));

    for (int buf = 0; buf < PRIMITIVE_ARR_SIZE; buf += 4) {

        screen_coord coord = translate_coord(arr_float[buf], arr_float[buf + 1]);
        arr[buf] = coord.x;
        arr[buf + 1] = coord.y;

    }

    int _offset = 0;

    switch (primitive_type) {

        case POINT: {
            for (int buf = 0; buf < primitive_count * 4; buf += 4)
                SET_PIXEL(scr, arr[buf], arr[buf+1], color);

            break;
        }
        case LINE: {
            for (int buf = 0; buf < primitive_count * 4; buf += 4){

                draw_line(scr,  arr[buf + _offset],
                                arr[buf + 1 + _offset],
                                arr[buf + 4 + _offset],
                                arr[buf + 5 + _offset], color);

                _offset += 4;
            }

            break;
        }
        case TRIANGLES: {

            for (int buf = 0; buf < primitive_count * 4; buf += 4) {
                draw_line(scr, arr[buf + _offset], arr[buf + 1 + _offset], arr[buf + 4 + _offset], arr[buf + 5 + _offset], color);
                draw_line(scr, arr[buf + 4 + _offset], arr[buf + 5 + _offset], arr[buf + 8 + _offset], arr[buf + 9 + _offset], color);
                draw_line(scr, arr[buf + 8 + _offset], arr[buf + 9 + _offset], arr[buf + _offset], arr[buf + 1 + _offset], color);

                _offset += 8;
            }

            break;
        }

        default:{
            break;
        }
    }

    free(arr);
}
