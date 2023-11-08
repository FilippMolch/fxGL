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
    vec3 scale = vec3_init();

    scale.vec[0] = 0.4;
    scale.vec[1] = 0.4;
    scale.vec[2] = 1.0;

    mat4_scale(&mat, scale);

    mat4 rotate2 = mat4_init(1);
    vec3 rotate_axis2 = vec3_init();

    rotate_axis2.vec[0] = 0.0f;
    rotate_axis2.vec[1] = 1.0f;
    rotate_axis2.vec[2] = 1.0f;

    mat4_rotate(&rotate2, 1.0f, rotate_axis2);

    apply_matrix_to_arr(mat, &poly, 18, 0);

    while (1){

        apply_matrix_to_arr(rotate2, &poly, 18, 0);
        fill_color_buf(&scr, colors[0]);
        draw_primitive_arr(&scr, TRIANGLES, &poly, 2, colors[4]);
        print_buf(&scr);

        Sleep(0);

    }

    system("pause");
    return 0;
}
