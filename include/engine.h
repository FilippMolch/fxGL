//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_ENGINE_H
#define SOFT_3D_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <lin_math.h>

#ifdef _WIN64
#include <Windows.h>
#endif

#define FX_POINT 1
#define FX_LINE 2
#define FX_TRIANGLES 3

#define TRANSLATE_COORD_X(x, W) (int)((x + 1.0f) * 0.5f * (W - 1))
#define TRANSLATE_COORD_Y(y, H) (int)((y + 1.0f) * 0.5f * (H - 1))

#define SET_FRAG(x, y, z) rast_arr[screens[screen_bind - 1].W * y + x].draw_fragment = 1;


int create_screen(int width, int height);
int bind_screen(int desc);
int set_output(void (*func)());
int delete_screen(int desc);

int create_renderer();
int bind_renderer(int desc);
int delete_renderer(int desc);
int renderer_buffer_data(int data_count, int data_length, float* data_ptr);
int draw_primitives(int prim_type, int vertex_count);

#ifdef DEBUG
void DEBUG_FUNC();
#endif

#endif //SOFT_3D_ENGINE_H
