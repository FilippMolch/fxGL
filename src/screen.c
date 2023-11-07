//
// Created by Filipp on 07.11.2023.
//
#include <screen.h>

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
