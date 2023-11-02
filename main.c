#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define W 120
#define H 40

uint8_t colors[4] = {176, 177, 178, 219};

uint8_t color_buffer[W*H];
uint8_t z_buffer[W*H];

int x = 0;
char str[121];

void fill_color_buf(uint8_t color) {
    uint64_t _fill = 0;

    int he[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    int yy = 0;

    for (unsigned int bit = 0; bit < 64; bit += 8){
        _fill |= (he[yy] << bit);
        yy++;
        printf("val = %#018"PRIx64"\n", _fill);
        printf("\n");
    }

    //printf("%x", _fill);

    for (int buf = 0; buf < (W*H*8); buf += 8) {
        //color_buffer[buf] = _fill;
    }

}
void print_buf(){
    str[120] = '\n';

    fill_color_buf(colors[3]);

    for (int buf = 0; buf < W*H; buf++) {
        if (x >= W-1){
            //fwrite(str, 121, 1, stdout);
            x = 0;
        }

        str[x] = color_buffer[buf];
        x++;
    }

}

int main() {
    system("mode con cols=120 lines=40");



    print_buf();

    system("pause");
    return 0;
}
