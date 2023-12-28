//
// Created by Filipp on 07.11.2023.
//

#ifndef SOFT_3D_FXGL_H
#define SOFT_3D_FXGL_H

#include <fxGL/fxRasterize.h>
#include "fxMath.h"

#define FX_ERROR_GEN_REN_BUFFER 328
#define FX_ERROR_GEN_SCREEN_BUFFER 329
#define FX_ERROR_GEN_SHADER_PROGRAM 330
#define FX_ERROR_GEN_ATTRIB_POINTER 331

#define FX_FLOAT 1

void fxClearColor();

int fxGenScreen(int width, int height);
void inline fxBindScreen(int desc);
int fxSetDrawOutput(void (*func)());
int fxFreeScreen(int desc);

int fxGenRenBuffer();
void inline fxBindRenBuffer(int desc);
int fxFreeRenderer(int desc);
int fxRenBufferData(int data_size, float* data_ptr);

int fxGenShaderProgram();
void inline fxUseProgram(int program);
void fxAttachShaders(void (*vertex)(), void (*fragment)(), int program);
int fxVertexAttribPointer(uint32_t count, size_t data_size, uint16_t data_type, uint16_t begin);

#endif //SOFT_3D_FXGL_H
