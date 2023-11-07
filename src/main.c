#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <lin_math.h>
#include <render.h>
#include <screen.h>

int main() {
    screen scr;
    screen_init(&scr);

    float poly[18] =    {-0.9f,-0.9f, 0.0f,
                            0.9f, -0.9f, 0.0f,
                            -0.9f, 0.9f, 0.0f,

                           0.9f,0.9f, 0.0f,
                           0.9f, -0.9f, 0.0f,
                           -0.9f, 0.9f, 0.0f,
    };

    mat4 mat = mat4_init(1);
    vec3 *scale = vec3_init();

    *scale->x = 0.1;
    *scale->y = 0.5;
    *scale->z = 0.1;

    mat4_scale(&mat, *scale);

    set_render_mat_trig(0);

    while (1){
        fill_color_buf(&scr, colors[1]);
        draw_primitive_arr(&scr, TRIANGLES, &poly, 2, colors[4], mat);
        print_buf(&scr);
    }


    system("pause");
    return 0;
}
