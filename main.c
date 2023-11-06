#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define W 120
#define H 40

#define POINT 1
#define LINE 2
#define TRIANGLES 3

#define PRIMITIVE_ARR_SIZE primitive_count * primitive_type * 3

#define Z_BUFFER_CLEAR_VALUE 1000.0f

#define SET_PIXEL(scr, x, y, color) if( (x < W && y < H) && (x >= 0 && y >= 0)) scr->color_buffer[W * y + x] = color;

static uint8_t colors[5] = {' ', 176, 177, 178, 219};

uint8_t color_buffer[W*H];

struct screen{
    uint8_t color_buffer[W*H];
    float z_buffer[W*H];
};

struct screen_coord {
    int x;
    int y;
};

typedef struct screen screen;
typedef struct screen_coord screen_coord;
typedef short int int16_f;

void screen_init(screen *scr){
    for (int buf = 0; buf < (W*H)/8; ++buf)
        ((uint64_t*)scr->color_buffer)[buf] = 0;

    for (int buf = 0; buf < (W*H); ++buf)
        scr->z_buffer[buf] = Z_BUFFER_CLEAR_VALUE;
}

void fill_color_buf(screen *scr, uint8_t color) {

    uint64_t color64 = 0;
    uint8_t color64_arr[8];

    for (int byte = 0; byte < 8; ++byte)
        color64_arr[byte] = color;

    color64 = *(uint64_t*)color64_arr;

    for (int buf = 0; buf < (W*H)/8; buf++)
        ((uint64_t*)scr->color_buffer)[buf] = color64;

}

void print_buf(screen *scr){
    static int x;
    static char str[122];
    str[121] = '\n';

    x = 0;
    for (int buf = 0; buf < W*H; buf++) {
        if (x >= W){
            fwrite(str, 122, 1, stdout);
            x = 0;
        }

        str[x] = scr->color_buffer[buf];
        x++;
    }

    printf("\n");
}

void draw_line(screen *scr, int x1, int y1, int x2, int y2, uint8_t color){
    const int delta_x = fabs(x2 - x1);
    const int delta_y = fabs(y2 - y1);
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

int main() {
    system("mode con cols=121 lines=42");

    screen scr;
    screen_init(&scr);

    float points[18] =    {-1.9f,0.9f, 0.0f,
                            0.0f, -1.9f, 0.0f,
                            1.9f, 0.9f, 0.0f,

                           -0.9f*0.8f,0.9f*0.8f, 0.0f,
                           0.0f*0.8f, -0.9f*0.8f, 0.0f,
                           0.9f*0.8f, 0.9f*0.8f, 0.0f,
    };

    fill_color_buf(&scr, colors[1]);
    draw_primitive_arr(&scr, TRIANGLES, &points, 2, colors[4]);
    print_buf(&scr);

    system("pause");
    return 0;
}
