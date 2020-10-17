#include <stdio.h>
#include <math.h>
#define PRECISION 100
#define SCREENW 100
#define SCREENH 100

float x(float u, float v);
float y(float u, float v);
float z(float u, float v);

const float K1 = 5.0f;

int main() {
    
    char screen[SCREENH][SCREENW] = {};

    float x0 = -1, x1 = 1, y0 = -1, y1 = 1;
    for (int i = 0; i < PRECISION; i++) {
        for (int j = 0; j < PRECISION; j++) {
            float u = x0 + (x1-x0)*((float)i/PRECISION);
            float v = y0 + (y1-y0)*((float)j/PRECISION);
            // map u v to x y z then to screen coordinates
            float coords[] = {x(u, v), y(u, v), z(u, v)};
            int screen_x = K1*coords[0]/coords[2];
            int screen_y = K1*coords[1]/coords[2];
            if (screen_x < 0 || screen_y < 0 || screen_x >= SCREENW || screen_y >= SCREENH) continue;
            screen[screen_y][screen_x] = '.'; 
        }
    }
    for (int i = 0 ; i < SCREENH; i++) {
        for (int j = 0; j < SCREENW; j++) {
            putchar(screen[i][j] ? '.' : ' ');
        }
        putchar('\n');
    }

}

float x(float u, float v) {

    return u;

}

float y(float u, float v) {
    
    return v;

}

float z(float u, float v) {

    return 2+0.005*u;

}
