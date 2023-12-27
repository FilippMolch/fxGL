#include <stdlib.h>
#include "fxGL/fxMath.h"
#include "fxGL/fxGL.h"
#include <Windows.h>
#include <time.h>
#include <obj_loader.h>

void _out(char* color_buffer, int w, int h){
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),
                  color_buffer,
                  w*h,
                  0,
                  0);
}

#include <fxGL/fxMain.h>
#include <fxGL/fxShader.h>

float angle = 0.0f;

void vert(void){
    float* loc = fx_AttrLocations(0);

    vec4 pos = vec4_get(loc[0], loc[1], loc[2], 1.0f);

    mat4 rotate = mat4_init(1);
    mat4_rotate(&rotate, angle, vec3_get(1.0f, 0.5f, 0.0f));
    pos = mat4_vec4_mult(rotate, pos);

    mat4 scl = mat4_init(1);
    mat4_scale(&scl, vec3_get(0.8f, 0.8f, 0.8f));
    pos = mat4_vec4_mult(scl, pos);

    fx_Position(pos);
}

void frag(void){
    printf("frag! \n");
}

void test_func(void){

}

int main(){
    HANDLE console;
    console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(console,&structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo( console, &structCursorInfo );

    COORD crd = {0, 0};
    SetConsoleCursorPosition(console, crd);

    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;

    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib,
                                &shapes,
                                &num_shapes,
                                &materials,
                                &num_materials,
                                "../spyro.obj",
                                get_file_data,
                                NULL,
                                flags);

    if (ret != TINYOBJ_SUCCESS) {
        printf("error load\n");
    }

    float* trng = (float*)malloc(attrib.num_face_num_verts * 9 * sizeof(float));
    int trng_count = attrib.num_face_num_verts;

    for (int i = 0; i < attrib.num_faces; ++i){
        trng[(i * 3)    ] = attrib.vertices[(3) * attrib.faces[i].v_idx];
        trng[(i * 3) + 1] = attrib.vertices[(3) * attrib.faces[i].v_idx + 1];
        trng[(i * 3) + 2] = attrib.vertices[(3) * attrib.faces[i].v_idx + 2];
    }

    int scr_1 = fxGenScreen(190, 60);
    int ren_1 = fxGenRenBuffer();
    int prog = fxGenShaderProgram();
    fxAttachShaders(vert, frag, prog);

    fxBindScreen(scr_1);
    fxSetDrawOutput(_out);

    fxBindRenBuffer(ren_1);
    fxRenBufferData(trng_count * 3, 3, trng);

    time_t t0 = clock();
    while (true){
        angle += 0.1f;
        if (angle >= 360.0f) angle = 0.0f;

        fxUseProgram(prog);
        fxDrawArray(FX_TRIANGLES, trng_count);
    }
    time_t t1 = clock();
    double time_in_seconds = t1 - t0;

    fxBindRenBuffer(0);
    fxBindScreen(0);

    printf("triangles count: %d\n", trng_count);
    printf("draw time (in seconds): %f\n", time_in_seconds/1000);
    test_func();

    system("pause");
    fxFreeScreen(scr_1);
    fxFreeRenderer(ren_1);
    free(trng);
    return 0;
}