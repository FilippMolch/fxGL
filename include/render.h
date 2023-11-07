//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_RENDER_H
#define SOFT_3D_RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <screen.h>

#include <lin_math.h>

#define POINT 1
#define LINE 2
#define TRIANGLES 3

#define PRIMITIVE_ARR_SIZE primitive_count * primitive_type * 3

#define SET_PIXEL(scr, x, y, color) if( (x < W && y < H) && (x >= 0 && y >= 0)) scr->color_buffer[W * y + x] = color;

#define F_ABS(a) fabs(a);

typedef short int int16_f;

static uint8_t render_mat_trig;

screen_coord translate_coord(float x, float y);

void draw_line(screen *scr, int x1, int y1, int x2, int y2, uint8_t color);
void draw_primitive_arr(screen *scr, int primitive_type, void* array, int primitive_count, uint8_t color, mat4 matrix);
void set_render_mat_trig(uint8_t trig);

#endif //SOFT_3D_RENDER_H
