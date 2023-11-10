
#include <stdlib.h>
#include <lin_math.h>
#include <render.h>
#include <screen.h>

void get_keys(screen *scr){
    for (int i = 0; i < 304; ++i)
        scr->keys[i] = GetAsyncKeyState(i)? i : 0;
}

#define KEY_PRESS 1
#define KEY_RELEASE 2

uint8_t key_pressed(screen *scr, uint8_t key){
    return scr->keys[key]? KEY_PRESS : KEY_RELEASE;
}

#define KEY_LEFT 293u
#define KEY_UP 294u
#define KEY_DOWN 296u
#define KEY_RIGHT 295u

vec3 camera_pos = {0.0f, 0.0f, -3.0f, VEC_INIT};
vec3 camera_target = {0.0f, 0.0f, -1.0f, VEC_INIT};
vec3 camera_up = {0.0f, 1.0f, 0.0f, VEC_INIT};

float x_pos = 0.0f, y_pos = 0.0f;
float last_x_pos = 0.0f, last_y_pos = 0.0f;

float yaw = 56.0f, pitch = 0.0f;

void euler_camera(){
    float x_offset = x_pos - last_x_pos;
    float y_offset = last_y_pos - y_pos;
    last_x_pos = x_pos;
    last_y_pos = y_pos;

    float  sensitivity = 3;	// Change this value to your liking
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    camera_target.vec[0] = COSF(ANGLE_TO_RADIANS(yaw)) * COSF(ANGLE_TO_RADIANS(pitch));
    camera_target.vec[1] = SINF(ANGLE_TO_RADIANS(pitch));
    camera_target.vec[2] = SINF(ANGLE_TO_RADIANS(yaw)) * COSF(ANGLE_TO_RADIANS(pitch));

    float scl = VEC4_SCALAR(camera_target);
    VEC4_NORMALIZE(camera_target, scl);
    //printf("X: %f, Y: %f, Z: %f \n", camera_target.vec[0], camera_target.vec[1], camera_target.vec[2]);
}

int main(void) {
    screen scr;
    screen_init(&scr);

    float poly[] =    {
            -0.5f, -0.5f, -0.5f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f,

            -0.5f, -0.5f,  0.5f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f,

            0.5f,  0.5f,  0.5f,  1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,

            -0.5f, -0.5f, -0.5f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f,

            -0.5f,  0.5f, -0.5f,  1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f,
    };

    mat4 mat = mat4_init(1);
    vec3 scale = vec3_init();

    scale.vec[0] = 0.7f;
    scale.vec[1] = 0.7f;
    scale.vec[2] = 1.0f;

    mat4_scale(&mat, scale);

    /*
    for (int i = 0; i < 24; i += 4) {
        printf("%f %f %f %f \n", arr_float[i],
               arr_float[i + 1],
               arr_float[i + 2],
               arr_float[i + 3]);
    }*/

    float angle = 0;

    mat4 model = mat4_init(1);
    vec3 rotate_axis2 = vec3_init();

    rotate_axis2.vec[0] = 0.4f;
    rotate_axis2.vec[1] = 1.0f;
    rotate_axis2.vec[2] = 0.0f;

    //mat4_translate(&model, (vec3){0.0f, 0.0f, 1.0f, VEC_INIT});


    mat4 test = mat4_init(1);
    mat4_translate(&test, (vec3){0.0f, 0.0f, 1.9f, VEC_INIT});

    vec3 move = vec3_init();
    move.vec[2] = 1.0f;

    mat4 project;
    project = mat4_perspective(45.0f, ((float)W/(float)H) / 2.1f, 0.1f, 100.0f);

    mat4 final_mat;

    while (1){

        mat4 camera = look_at(camera_pos, camera_target, camera_up);

        get_keys(&scr);

        if (key_pressed(&scr, KEY_UP) == KEY_PRESS){
            y_pos -= 1;
            euler_camera();
        }

        if (key_pressed(&scr, KEY_DOWN) == KEY_PRESS){
            y_pos += 1;
            euler_camera();
        }

        if (key_pressed(&scr, KEY_RIGHT) == KEY_PRESS){
            x_pos += 1;
            euler_camera();
        }

        if (key_pressed(&scr, KEY_LEFT) == KEY_PRESS){
            x_pos -= 1;
            euler_camera();
        }

        final_mat = mat4_mult_some(3, project, camera, model);

        void* new_arr = apply_matrix_to_arr(final_mat, &poly, 144);
        perspective_divide_arr(new_arr, 144);

        fill_color_buf(&scr, colors[1]);
        draw_primitive_arr(&scr, TRIANGLES, new_arr, 12, colors[4]);
        print_buf(&scr);

        mat4_rotate(&model, angle, rotate_axis2);

        angle += 1.0f;
        if (angle >= 360) angle = 0;

        //printf("X: %f Y: %f Z: %f \n", camera_target.vec[0], camera_target.vec[1], camera_target.vec[2]);

        Sleep(10);
        free(new_arr);

    }

    system("pause");
    return 0;
}
