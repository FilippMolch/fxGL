//
// Created by Filipp on 07.11.2023.
//
#include <render.h>

void draw_line(screen *scr, int x1, int y1, int x2, int y2, uint8_t color){
    const int delta_x = F_ABS(x2 - x1);
    const int delta_y = F_ABS(y2 - y1);
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

void draw_primitive_arr(screen *scr, int primitive_type, void* array, int primitive_count, uint8_t color){
    float *arr_float = ((float*)array);
    int16_f *arr = (int16_f*)malloc(PRIMITIVE_ARR_SIZE * 2);

    for (int buf = 0; buf < PRIMITIVE_ARR_SIZE; buf += 3) {
        screen_coord coord = translate_coord(arr_float[buf], arr_float[buf + 1]);
        arr[buf] = coord.x;
        arr[buf + 1] = coord.y;
    }

    int _offset = 0;

    switch (primitive_type) {

        case POINT: {
            for (int buf = 0; buf < primitive_count * 3; buf += 3)
                SET_PIXEL(scr, arr[buf], arr[buf+1], color);

            break;
        }
        case LINE: {
            for (int buf = 0; buf < primitive_count * 3; buf += 3){
                draw_line(scr,  arr[buf + _offset],
                          arr[buf + 1 + _offset],
                          arr[buf + 3 + _offset],
                          arr[buf + 4 + _offset], color);

                _offset += 3;
            }

            break;
        }
        case TRIANGLES: {
            for (int buf = 0; buf < primitive_count * 3; buf += 3) {
                draw_line(scr, arr[buf + _offset], arr[buf + 1 + _offset], arr[buf + 3 + _offset], arr[buf + 4 + _offset], color);
                draw_line(scr, arr[buf + 3 + _offset], arr[buf + 4 + _offset], arr[buf + 6 + _offset], arr[buf + 7 + _offset], color);
                draw_line(scr, arr[buf + 6 + _offset], arr[buf + 7 + _offset], arr[buf + _offset], arr[buf + 1 + _offset], color);

                _offset += 6;
            }

            break;
        }
    }

    free(arr);
}
