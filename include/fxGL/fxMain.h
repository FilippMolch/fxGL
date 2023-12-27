//
// Created by Filipp on 24.12.2023.
//
#ifndef SOFT_3D_FXMAIN_H
#define SOFT_3D_FXMAIN_H

#include <stdbool.h>

#define TRANSLATE_COORD_X(x, W) (int)((x + 1.0f) * 0.5f * (W - 1))
#define TRANSLATE_COORD_Y(y, H) (int)((y + 1.0f) * 0.5f * (H - 1))

#define PTR_CHECK(d) if (d == NULL) return 0;

struct screen_struct {
    int W, H;

    char* color_buffer;
    float* z_buffer;

    void (*screen_output_func)();

    bool init;
};

struct renderer_struct {
    float* buffer;

    int data_count;
    int data_length;

    bool init;
};

struct fragment_struct {
    float z_coord;

    bool draw_fragment;
};

struct shader_program_struct {
    void (*vertex)();
    void (*fragment)();

    vec4 position;

    float attrs[3];
};

typedef struct screen_struct screen;
typedef struct renderer_struct renderer;
typedef struct fragment_struct fragment;
typedef struct shader_program_struct shader;

extern int screens_count;
extern int screen_bind;
extern screen* screens;

extern int renderers_count;
extern int renderer_bind;
extern renderer* renderers;

extern int shaders_count;
extern int shader_use;
extern shader* shaders;

void* fxDynamicAllocator(int* bufCount, void* bufArray, size_t byteSize);

#endif //SOFT_3D_FXMAIN_H
