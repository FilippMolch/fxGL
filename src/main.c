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

    mat4 mat_1 = {{
                    {2, 5, 32, 3},
                    {4, 5, 2, 9},
                    {4, 3, 0, 9},
                    {5, 6, 7, 8}
                    },
                    MAT_INIT
    };

    mat4 mat_2 = {
            {
                    {9, 8, 7, 85},
                    {9, 7, 7, 5},
                    {8, 5, 3, 4},
                    {7, 6, 2, 3}
                    },
                    MAT_INIT
    };

    mat4 final = mat4_mult(mat_1, mat_2);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%f ", final.mat[i][j]);
        }

        printf("\n");
    }

    system("pause");
    return 0;
}
