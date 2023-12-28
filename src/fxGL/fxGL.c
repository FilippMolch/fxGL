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

void fxClearColor(){
    if (screen_bind){
        screen* scr = &screens[screen_bind - 1];
        memset(scr->color_buffer, ' ', (size_t)scr->W * scr->H * sizeof(char));
    }
}

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

int fxVertexAttribPointer(uint32_t count, size_t data_size, uint16_t data_type, uint16_t begin){
    renderer* ren = &renderers[renderer_bind - 1];
    ren->attribs = fxDynamicAllocator(&ren->shader_attribs_count, ren->attribs, sizeof(attrib_pointer));
    if(!ren->attribs){
        printf("fxGL VertexAttribPointer ERROR: Dynamic buffer allocation error \n");
        return FX_ERROR_GEN_ATTRIB_POINTER;
    }

    ren->attribs[ren->shader_attribs_count - 1].count = count;
    ren->attribs[ren->shader_attribs_count - 1].data_size = data_size;
    ren->attribs[ren->shader_attribs_count - 1].data_type = data_type;
    ren->attribs[ren->shader_attribs_count - 1].begin = begin;

    free(ren->attrs);
    ren->attrs = (float**)malloc(ren->shader_attribs_count * sizeof(float*));

    for (int i = 0; i < ren->shader_attribs_count; ++i) {
        attrib_pointer* current_attrib = &ren->attribs[i];

        size_t data_t = 0;

        switch (current_attrib->data_type) {
            case FX_FLOAT: {
                data_t = sizeof(float);
                break;
            }
            default: {
                printf("fxGL VertexAttribPointer ERROR: DATA_TYPE");
                return 0;
            }
        }

        ren->attrs[i] = (float*)malloc(data_t * current_attrib->count);
    }

    return 1;
}

void fx_Position(vec4 pos){
    shader* shd = &shaders[shader_use - 1];

    shd->position = pos;
}

float* fx_AttrLocations(int loc){
    if (shader_use){
        renderer* ren = &renderers[renderer_bind - 1];
        return ren->attrs[loc];
    }

    return NULL;
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
        scr->z_buffer[i] = -1.0f;
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
    ren->data_size = 0;

    ren->attrs = NULL;
    ren->attribs = NULL;
    ren->shader_attribs_count = 0;
    ren->vert_buf = NULL;

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

    for (int i = 0; i < ren->shader_attribs_count; ++i) {
        free(ren->attrs[i]);
    }

    free(ren->attrs);

    free(ren->buffer);
    free(ren->vert_buf);
    free(ren->attribs);

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

int fxRenBufferData(int data_size, float* data_ptr){
    if (renderers == NULL && renderer_bind == 0)
        return 0;

    size_t size = (data_size) * sizeof(float);

    renderer* ren = &renderers[renderer_bind - 1];

    ren->buffer = (float*)malloc(size);
    ren->vert_buf = (float*)calloc(data_size, sizeof(float));

    PTR_CHECK(ren->buffer)
    ren->data_size = data_size;

    memcpy(ren->buffer, data_ptr, size);

    return 1;
}
