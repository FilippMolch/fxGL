//
// Created by Filipp on 24.12.2023.
//
#ifndef SOFT_3D_FXMAIN_H
#define SOFT_3D_FXMAIN_H

#include <stdbool.h>

#define TRANSLATE_COORD_X(x, W) (int)((x + 1.0f) * 0.5f * (W - 1))
#define TRANSLATE_COORD_Y(y, H) (int)((y + 1.0f) * 0.5f * (H - 1))

#define PTR_CHECK(d) if (d == NULL) return 0;

typedef struct {
    int W, H;

    char* color_buffer;
    float* z_buffer;

    void (*screen_output_func)();

    bool init;
} screen;

typedef struct {
    uint32_t count;

    size_t data_size;
    uint16_t data_type;

    uint16_t begin;
}  attrib_pointer;

typedef struct {
    float* buffer;
    float* vert_buf;

    int data_size;

    attrib_pointer* attribs;
    float** attrs;
    int shader_attribs_count;

    bool init;
} renderer;

typedef struct {
    void (*vertex)();
    void (*fragment)();

    vec4 position;
} shader;

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
