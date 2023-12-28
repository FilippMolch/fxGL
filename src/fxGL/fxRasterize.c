//
// Created by Filipp on 23.12.2023.
//
#include <fxGL/fxRasterize.h>
#include <fxGL/fxMain.h>

#include <stdlib.h>

void draw_line(vec3 src, vec3 dst){

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
        //SET_FRAG(x1, y1, 0.0f);
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

char colors[3] = {176, 177, 178};
int fxDrawArray(int prim_type, int prim_count){

    if (renderer_bind == 0)
        return 0;

    renderer* ren = &renderers[renderer_bind - 1];
    screen* scr = &screens[screen_bind - 1];
    shader* shd = NULL;

    if (shader_use)
        shd = &shaders[shader_use - 1];

    float* vert_buf = ren->vert_buf;
    int vertex = 0;

    for (size_t ver = 0; ver < ren->data_size; ver += ren->attribs[0].data_size) {

        for (int cur_attrib = 0; cur_attrib < ren->shader_attribs_count; ++cur_attrib) {
            attrib_pointer* c_a = &ren->attribs[cur_attrib];
            for (int attr_line = 0; attr_line < c_a->data_size; ++attr_line) {
                float* b = ren->buffer;
                ren->attrs[cur_attrib][attr_line] = b[ver + attr_line + c_a->begin];
            }
        }

        shd->vertex();

        vert_buf[vertex + 0] = shd->position.vec[0];
        vert_buf[vertex + 1] = shd->position.vec[1];
        vert_buf[vertex + 2] = shd->position.vec[2];
        vertex += 3;

    }

    switch (prim_type) {
        case FX_POINT: {
            int pnt[2] = {0};

            for (int i = 0; i < prim_count * 3 * 3; i += 3) {
                pnt[0] = TRANSLATE_COORD_X(ren->buffer[i+0], scr->W);
                pnt[1] = TRANSLATE_COORD_Y(ren->buffer[i+1], scr->H);

                scr->color_buffer[scr->W * pnt[1] + pnt[0]] = '#';
            }
            break;
        }

        case FX_LINE: {
            vec3 src = vec3_get(ren->buffer[0], ren->buffer[1], ren->buffer[2]);
            vec3 dst = vec3_get(ren->buffer[3], ren->buffer[4], ren->buffer[5]);

            //draw_line(rasterize_arr, src, dst);
            break;
        }

        case FX_TRIANGLES: {
            int left_p, right_p, top_p, bottom_p;
            float tri[3][2] = {0};
            float P[2] = {0};

            for (int i = 0; i < prim_count * 9; i += 9) {

                if (!shader_use){
                    vert_buf[i + 0] = ren->buffer[i + 0];
                    vert_buf[i + 1] = ren->buffer[i + 1];
                    vert_buf[i + 2] = ren->buffer[i + 2];

                    vert_buf[i + 3] = ren->buffer[i + 3];
                    vert_buf[i + 4] = ren->buffer[i + 4];
                    vert_buf[i + 5] = ren->buffer[i + 5];

                    vert_buf[i + 6] = ren->buffer[i + 6];
                    vert_buf[i + 7] = ren->buffer[i + 7];
                    vert_buf[i + 8] = ren->buffer[i + 8];
                }

                tri[0][0] = TRANSLATE_COORD_X(vert_buf[i+0], scr->W);
                tri[0][1] = TRANSLATE_COORD_Y(vert_buf[i+1], scr->H);

                tri[1][0] = TRANSLATE_COORD_X(vert_buf[i+3], scr->W);
                tri[1][1] = TRANSLATE_COORD_Y(vert_buf[i+4], scr->H);

                tri[2][0] = TRANSLATE_COORD_X(vert_buf[i+6], scr->W);
                tri[2][1] = TRANSLATE_COORD_Y(vert_buf[i+7], scr->H);

                left_p = min(min(tri[0][0], tri[1][0]), tri[2][0]);
                right_p = max(max(tri[0][0], tri[1][0]), tri[2][0]);

                top_p = min(min(tri[0][1], tri[1][1]), tri[2][1]);
                bottom_p = max(max(tri[0][1], tri[1][1]), tri[2][1]);

                for (int x = left_p; x < right_p; ++x) {
                    for (int y = top_p; y < bottom_p; ++y) {
                        P[0] = (float)x;
                        P[1] = (float)y;

                        vec3 final = baryCoord(tri, P);
                        if(final.vec[0] < 0 || final.vec[1] < 0 || final.vec[2] < 0) continue;

                        scr->color_buffer[scr->W * y + x] = colors[2];
                    }
                }
            }

            break;
        }

        default: {
            break;
        }
    }

    scr->screen_output_func(scr->color_buffer, scr->W, scr->H);
}
