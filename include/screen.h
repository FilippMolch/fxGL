//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_SCREEN_H
#define SOFT_3D_SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#ifdef WIN32
#include <Windows.h>
#endif

#define W 180
#define H 52

#define Z_BUFFER_CLEAR_VALUE 1000.0f

#define SCREEN_INIT 224

static uint8_t colors[5] = {' ', 176, 177, 178, 219};

struct screen{
    uint8_t color_buffer[W*H];
    float z_buffer[W*H];
    HANDLE console;

    uint8_t init;
};

struct screen_coord {
    int x;
    int y;
};

typedef struct screen screen;
typedef struct screen_coord screen_coord;

void screen_init(screen *scr);
void fill_color_buf(screen *scr, uint8_t color);
void print_buf(screen *scr);

#endif //SOFT_3D_SCREEN_H
