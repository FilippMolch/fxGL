#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <lin_math.h>
#include <render.h>
#include <screen.h>

int main() {
    system("mode con cols=121 lines=42");

    screen scr;
    screen_init(&scr);

    float points[18] =    {-1.9f,0.9f, 0.0f,
                            0.0f, -1.9f, 0.0f,
                            1.9f, 0.9f, 0.0f,

                           -0.9f,0.6f, 0.0f,
                           0.0f, -0.9f, 0.0f,
                           0.9f, 0.6f, 0.0f,
    };

    fill_color_buf(&scr, colors[1]);
    draw_primitive_arr(&scr, TRIANGLES, &points, 2, colors[4]);
    print_buf(&scr);


    system("pause");
    return 0;
}
