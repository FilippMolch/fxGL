//
// Created by Filipp on 07.11.2023.
//
#include <screen.h>

void screen_init(screen *scr){
    for (int buf = 0; buf < (W*H)/8; ++buf)
        ((uint64_t*)scr->color_buffer)[buf] = 0;

    for (int buf = 0; buf < (W*H); ++buf)
        scr->z_buffer[buf] = Z_BUFFER_CLEAR_VALUE;

    char init_console[30];

#ifdef WIN32
    sprintf(init_console, "mode con cols=%d lines=%d", W, H);

    system(init_console);

    scr->console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(scr->console,&structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo( scr->console, &structCursorInfo );
    scr->init = SCREEN_INIT;
#endif

}

void fill_color_buf(screen *scr, uint8_t color) {

    if(scr->init == SCREEN_INIT) {
        uint64_t color64 = 0;
        uint8_t color64_arr[8];

        for (int byte = 0; byte < 8; ++byte)
            color64_arr[byte] = color;

        color64 = *(uint64_t*)color64_arr;

        for (int buf = 0; buf < (W*H)/8; buf++)
            ((uint64_t*)scr->color_buffer)[buf] = color64;
    }
    else{
        printf("SCREEN NOT INIT \n");
    }
}

void print_buf(screen *scr){

    if(scr->init == SCREEN_INIT) {
#ifdef WIN32
        COORD crd = {0, 0};
        SetConsoleCursorPosition(scr->console, crd);

        WriteConsoleA(scr->console, scr->color_buffer, W*H, 0, 0);
#endif
    }
    else{
        printf("SCREEN NOT INIT \n");
    }
}
