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

    float points[18] =    {-1.9f,0.9f, 0.0f,
                            0.0f, -1.9f, 0.0f,
                            1.9f, 0.9f, 0.0f,

                           -0.9f,0.6f, 0.0f,
                           0.0f, -0.9f, 0.0f,
                           0.9f, 0.6f, 0.0f,
    };

    do {
        fill_color_buf(&scr, colors[1]);
        draw_primitive_arr(&scr, TRIANGLES, &points, 2, colors[4]);
        print_buf(&scr);
    } while (1);


    system("pause");
    return 0;
}
