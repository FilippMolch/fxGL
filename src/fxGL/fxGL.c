//
// Created by Filipp on 07.11.2023.
//
#include <fxGL/fxGL.h>
#include <fxGL/fxMain.h>
#include <fxGL/fxShader.h>

int screens_count = 0;
int screen_bind = 0;
screen* screens = NULL;

int renderers_count = 0;
int renderer_bind = 0;
renderer* renderers = NULL;

int shaders_count = 0;
int shader_use = 0;
shader* shaders = NULL;

int fxGenShaderProgram(){
    shaders = (shader*)fxDynamicAllocator(&shaders_count, shaders, sizeof(shader));
    if(!shaders){
        printf("fxGL GenShaderProgram ERROR: Dynamic buffer allocation error \n");
        return FX_ERROR_GEN_SHADER_PROGRAM;
    }

    shader* shd = &shaders[shaders_count - 1];

    shd->vertex = NULL;
    shd->fragment = NULL;

    shd->position = vec4_get(0.0f, 0.0f, 0.0f, 1.0f);

    return shaders_count;
}

void fxUseProgram(int program){
    shader_use = program;
}

void fxAttachShaders(void (*vertex)(), void (*fragment)(), int program){
    shader* shd = &shaders[program - 1];

    shd->vertex = vertex;
    shd->fragment = fragment;
}

void fx_Position(vec4 pos){
    shader* shd = &shaders[shader_use - 1];

    shd->position = pos;
}

float* fx_AttrLocations(int loc){
    if (shader_use){
        shader* shd = &shaders[shader_use - 1];
        return shd->attrs;
    }
}

void* fxDynamicAllocator(int* bufCount, void* bufArray, size_t byteSize){
    void* new = NULL;

    if (!*bufCount){
        *bufCount += 1;
        new = malloc(*bufCount * byteSize);
        PTR_CHECK(new)
        return new;
    }
    if (*bufCount > 0){
        *bufCount += 1;
        new = realloc(bufArray, *bufCount * byteSize);
        PTR_CHECK(new)
        return new;
    }

    return NULL;
}

int fxGenScreen(int width, int height){
    screens = (screen*)fxDynamicAllocator(&screens_count, screens, sizeof(screen));
    if(!screens){
        printf("fxGL genScreen ERROR: Dynamic buffer allocation error \n");
        return FX_ERROR_GEN_SCREEN_BUFFER;
    }

    screen* scr = &screens[screens_count - 1];

    scr->W = width;
    scr->H = height;

    scr->color_buffer = (char*)malloc((size_t)width * height * sizeof(char));
    scr->z_buffer = (float*)malloc((size_t)width * height * sizeof(float));
    PTR_CHECK(scr->color_buffer)
    PTR_CHECK(scr->z_buffer)

    memset(scr->color_buffer, ' ', (size_t)width * height * sizeof(char));

    for (int i = 0; i < width * height; ++i) {
        scr->z_buffer[i] = 1.0f;
    }

    scr->screen_output_func = NULL;

    scr->init = 1;

    return screens_count;
}

int fxGenRenBuffer(){
    renderers = (renderer*)fxDynamicAllocator(&renderers_count, renderers, sizeof(renderer));
    if(!renderers){
        printf("fxGL genRenBuf ERROR: Dynamic buffer allocation error \n");
        return FX_ERROR_GEN_REN_BUFFER;
    }

    renderer* ren = &renderers[renderers_count - 1];

    ren->buffer = NULL;
    ren->data_count = 0;
    ren->data_length = 0;
    ren->init = 1;

    return renderers_count;
}

int fxSetDrawOutput(void (*func)()){
    if (screens == NULL)
        return 0;

    screen* scr = &screens[screen_bind - 1];

    if (func != NULL && scr->init && screen_bind > 0){
        scr->screen_output_func = func;
        return 1;
    }

    return 0;
}

void fxBindScreen(int desc){

    if(desc){
        screen* scr = &screens[desc-1];

        char init_console[50];
        sprintf(init_console, "mode con cols=%d lines=%d", scr->W, scr->H);
        system(init_console);
    }

    screen_bind = desc;
}

void fxBindRenBuffer(int desc){
    renderer_bind = desc;
}

int fxFreeRenderer(int desc){
    if (renderers == NULL)
        return 0;

    renderer* ren = &renderers[desc - 1];
    free(ren->buffer);

    return 1;
}

int fxFreeScreen(int desc){
    if (screens == NULL)
        return 0;

    screen* scr = &screens[desc - 1];

    PTR_CHECK(scr)

    free(scr->color_buffer);
    free(scr->z_buffer);

    return 1;
}

int fxRenBufferData(int data_count, int data_length, float* data_ptr){
    if (renderers == NULL && renderer_bind == 0)
        return 0;

    size_t size = (data_count * data_length) * sizeof(float);

    renderer* ren = &renderers[renderer_bind - 1];

    ren->buffer = (float*)malloc(size);
    PTR_CHECK(ren->buffer)
    ren->data_count = data_count;
    ren->data_length = data_length;

    memcpy(ren->buffer, data_ptr, size);

    return 1;
}
