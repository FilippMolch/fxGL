#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define W 120
#define H 41

#define POINT 0
#define LINE 1
#define TRIANGLES 2

#define SET_PIXEL(scr, x, y, color) scr->color_buffer[W * y + x] = color;

static uint8_t colors[5] = {' ', 176, 177, 178, 219};

uint8_t color_buffer[W*H];

struct screen{
    uint8_t color_buffer[W*H];
    uint64_t z_buffer[W*H];
};

typedef struct screen screen;

void screen_init(screen *scr){
    for (int buf = 0; buf < (W*H)/8; ++buf)
        ((uint64_t*)scr->color_buffer)[buf] = 0;

    for (int buf = 0; buf < (W*H); ++buf)
        scr->z_buffer[buf] = 0;
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

void draw_primitive_arr(screen *scr, int primitive_type, void* array, int primitive_count, uint8_t color){
    uint8_t *arr = ((uint8_t*)array);
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
}

int main() {
    system("mode con cols=121 lines=42");

    screen scr;
    screen_init(&scr);

    uint8_t points[18] =    {10,2, 0,
                            109, 2, 0,
                            109, 37, 0,

                             10,2, 0,
                             10, 37, 0,
                             109, 37, 0,

    };

    fill_color_buf(&scr, colors[1]);

    draw_primitive_arr(&scr, TRIANGLES, &points, 2, colors[4]);

    print_buf(&scr);

    system("pause");
    return 0;
}
