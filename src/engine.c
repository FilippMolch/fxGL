//
// Created by Filipp on 07.11.2023.
//
#include <engine.h>

#define PTR_CHECK(d) if (d == NULL) return 0;

#define SCREEN_BYTE_SIZE sizeof(screen) * (size_t)screens_count
#define RENDERER_BYTE_SIZE sizeof(screen) * (size_t)screens_count

struct screen_struct {
    int W, H;

    char* color_duffer;
    float* z_buffer;

    void (*screen_output_func)();

    bool init;
};

struct renderer_struct {
    float* buffer;

    int data_count;
    int data_length;

    void (*vert_shader)();
    void (*frag_shader)();

    bool init;
};

struct fragment_struct {
    float z_coord;

    bool draw_fragment;
};

typedef struct screen_struct screen;
typedef struct renderer_struct renderer;
typedef struct fragment_struct fragment;

static int screens_count = 0;
static int screen_bind = 0;
static screen* screens = NULL;

static int renderers_count = 0;
static int renderer_bind = 0;
static renderer* renderers = NULL;

static void* vert_shader_data = NULL;
static void* frag_shader_data = NULL;

int create_screen(int width, int height){
    bool init = false;

    if (screens == NULL && !init){
        screens_count++;
        screens = (screen*)malloc(SCREEN_BYTE_SIZE);
        PTR_CHECK(screens)
        init = true;
    }

    if (screens_count > 0 && !init){
        screen* screens_old = (screen*)malloc(SCREEN_BYTE_SIZE);
        PTR_CHECK(screens_old)

        memcpy(screens_old, screens, SCREEN_BYTE_SIZE);
        screens_count++;

        free(screens);

        screens = (screen*)malloc(SCREEN_BYTE_SIZE);
        PTR_CHECK(screens)

        memcpy(screens, screens_old, SCREEN_BYTE_SIZE);
        free(screens_old);
    }

    screen* scr = &screens[screens_count - 1];

    scr->W = width;
    scr->H = height;

    scr->color_duffer = (char*)malloc((size_t)width * height * sizeof(char));
    scr->z_buffer = (float*)malloc((size_t)width * height * sizeof(float));
    PTR_CHECK(scr->color_duffer)
    PTR_CHECK(scr->z_buffer)

    memset(scr->color_duffer, ' ', (size_t)width * height * sizeof(char));
    memset(scr->z_buffer, 1.0f, (size_t)width * height * sizeof(float));

    scr->screen_output_func = NULL;

    scr->init = 1;

    return screens_count;
}

int bind_screen(int desc){
    screen_bind = desc;
}

int set_output(void (*func)()){
    if (screens == NULL)
        return 0;

    if (func != NULL && screens[screen_bind - 1].init && screen_bind > 0){
        screens[screen_bind - 1].screen_output_func = func;
        return 1;
    }

    return 0;
}

void draw_buf(){
    screen* bnd = &screens[screen_bind - 1];
    bnd->screen_output_func(bnd->color_duffer, bnd->W, bnd->H);
}

int delete_screen(int desc){
    if (screens == NULL)
        return 0;

    free(screens[desc - 1].color_duffer);
    free(screens[desc - 1].z_buffer);

    return 1;
}

int create_renderer(){
    bool init = false;

    if (renderers == NULL && !init){
        renderers_count++;
        renderers = (renderer*)malloc(RENDERER_BYTE_SIZE);
        PTR_CHECK(renderers)
        init = true;
    }

    if (renderers_count > 0 && !init){
        renderer* renderers_old = (renderer*)malloc(RENDERER_BYTE_SIZE);
        PTR_CHECK(renderers_old)

        memcpy(renderers_old, renderers, RENDERER_BYTE_SIZE);
        renderers_count++;

        free(renderers);

        renderers = (renderer*)malloc(RENDERER_BYTE_SIZE);
        PTR_CHECK(renderers)

        memcpy(renderers, renderers_old, RENDERER_BYTE_SIZE);
        free(renderers_old);
    }

    renderer* ren = &renderers[renderers_count - 1];

    ren->buffer = NULL;

    ren->data_count = 0;
    ren->data_length = 0;

    ren->vert_shader = NULL;
    ren->frag_shader = NULL;

    ren->init = 1;

    return renderers_count;
}

int bind_renderer(int desc){
     renderer_bind = desc;
}

int delete_renderer(int desc){
    if (renderers == NULL)
        return 0;

    free(renderers[desc - 1].buffer);

    return 1;
}

int renderer_buffer_data(int data_count, int data_length, float* data_ptr){
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

void draw_line(fragment* rast_arr, vec3 src, vec3 dst){

    screen* scree = &screens[screen_bind - 1];

    int x1 = TRANSLATE_COORD_X(src.vec[0], scree->W);
    int y1 = TRANSLATE_COORD_Y(src.vec[1], scree->H);

    int x2 = TRANSLATE_COORD_X(dst.vec[0], scree->W);
    int y2 = TRANSLATE_COORD_Y(dst.vec[1], scree->H);

    const int delta_x = FABS(x2 - x1);
    const int delta_y = FABS(y2 - y1);
    const int sign_x = x1 < x2? 1 : -1;
    const int sign_y = y1 < y2? 1 : -1;

    int error = delta_x - delta_y;

    while (x1 != x2 || y1 != y2){
        //TODO: Z inter
        SET_FRAG(x1, y1, 0.0f);
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

int frag_sh_handler(fragment* rast_arr){
    if (renderers[renderer_bind - 1].frag_shader == NULL)
        return 0;

    size_t frag_count = screens[screen_bind - 1].W * screens[screen_bind - 1].H;

    for (size_t i = 0; i < frag_count; ++i) {

    }
}

int draw_primitives(int prim_type, int vertex_count){
    /*
     ***RENDERER STAGES***
     * Создание буфера для вывода шейдерных данных (размер буфера как у color_buffer).
     * Вызов вертексного шейдера к каждой координате примитива
       (если в шейдере передаются данные, то они записываются в буфер шейдерных данных, на определённые координаты).
     * Растеризация линий по точкам и инициализация шейдерных структур.
     * Растеризация внутренности примитивов.
       (пока что только треугольника, остальные примитивы этого не требуют, достаточно предыдущего пункта)
     * Интерполяция данных полученных с вертексного шейдера на все фрагменты созданные растеризатором
       и запись их в буфер для фрагментного шейдера.
     * Тест глубины и выключение фрагментов не прошедших тест (эту стадию можно отключить)
     * Вызов фрагментного шейдера к каждому элементу буфера и запись в color_bufer.
    */

    if (renderer_bind == 0)
        return 0;

    renderer* ren = &renderers[renderer_bind - 1];

    size_t frag_count = screens[screen_bind - 1].W * screens[screen_bind - 1].H;

    fragment* rasterize_arr = (fragment*)malloc(frag_count * sizeof(fragment));
    PTR_CHECK(rasterize_arr)

    for (int i = 0; i < frag_count; ++i)
        rasterize_arr[i] = (fragment){0.0f, 0};

    switch (prim_type) {
        case FX_POINT: {

            break;
        }

        case FX_LINE: {
            vec3 src = vec3_get(ren->buffer[0], ren->buffer[1], ren->buffer[2]);
            vec3 dst = vec3_get(ren->buffer[3], ren->buffer[4], ren->buffer[5]);

            draw_line(&rasterize_arr, src, dst);
            break;
        }

        case FX_TRIANGLES: {

            break;
        }

        default: {
            break;
        }
    }

    free(rasterize_arr);

    screen* scree = &screens[screen_bind - 1];
    scree->screen_output_func(scree->color_duffer,
                              scree->W,
                              scree->H);

}

void _out(char* color_buffer, int w, int h){
    char init_console[50];

    sprintf(init_console, "mode con cols=%d lines=%d", w, h);
    system(init_console);

    HANDLE console;

    console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(console,&structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo( console, &structCursorInfo );

    COORD crd = {0, 0};
    SetConsoleCursorPosition(console, crd);
    WriteConsoleA(console, color_buffer, w*h, 0, 0);
}

void DEBUG_FUNC(){
    int scr_1 = create_screen(100, 40);

    int ren_1 = create_renderer();

    float trng[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    bind_screen(scr_1);
    set_output(_out);
    bind_renderer(ren_1);
    renderer_buffer_data(3, 3, &trng);
    draw_primitives(FX_TRIANGLES, 1);

    bind_screen(0);
    bind_renderer(0);

    delete_screen(scr_1);
    delete_renderer(ren_1);

}
