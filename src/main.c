#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <lin_math.h>
#include <render.h>
#include <screen.h>

int main() {
    screen scr;
    //screen_init(&scr);

    float poly[18] =    {-0.9f,-0.9f, 0.0f,
                            0.9f, -0.9f, 0.0f,
                            -0.9f, 0.9f, 0.0f,

                           0.9f,0.9f, 0.0f,
                           0.9f, -0.9f, 0.0f,
                           -0.9f, 0.9f, 0.0f,
    };

    while (0){
        fill_color_buf(&scr, colors[1]);
        draw_primitive_arr(&scr, TRIANGLES, &poly, 2, colors[4]);
        print_buf(&scr);
    }

    vec4 vec = {
                    {2, 4, 44, 5},
                    0,0,0,0,
                    VEC_INIT
    };

    mat4 mat = {
            {
                    {9, 8, 7, 85},
                    {9, 7, 7, 5},
                    {8, 5, 3, 4},
                    {7, 6, 2, 3}
                    },
                    MAT_INIT
    };

    vec4* final = mat4_vec4_mult(mat, vec);

    for (int j = 0; j < 4; ++j) {
            printf("%f \n", final->vec[j]);
    }

    system("pause");
    return 0;
}
