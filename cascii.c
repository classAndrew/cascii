#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define SCREENH 30
#define SCREENW 60
#define PRECISION 100

#define cls() printf("\x1b[H")
// all of this should get moved to a header

typedef struct
{
    double x, y, z;
} vec3;

double x(double u, double v);
double y(double u, double v);
double z(double u, double v);
void render(char screen[SCREENH][SCREENW], double zbuff[SCREENH][SCREENW]);
void r_u(double u, double v, vec3 *vec);
void r_v(double u, double v, vec3 *vec);
double dot(vec3 *a, vec3 *b);
void cross(vec3 *a, vec3 *b);
void normalize(vec3 *a);

const double x0 = 0, x1 = 3.14159265, yp0 = 0, yp1 = 2 * 3.14159265;
const int PROJ_CENTER_X = SCREENW / 2;
const int PROJ_CENTER_Y = SCREENH;
const int PERSPECTIVE = SCREENW * 0.75;
const double scale = 10;
const double y_scale = 0.5;
const double dx = 0.00001;
vec3 light = {1, 0, 0};

double ang = 0;

int main()
{

    char screen[SCREENH][SCREENW] = {};
    double zbuff[SCREENH][SCREENW] = {};
    while (1)
    {
        cls();
        render(screen, zbuff);

        ang += 0.01;
        light.x = cos(ang);
        light.z = sin(ang);
        memset(screen, 0, SCREENH * SCREENW);
        memset(zbuff, 0, SCREENH * SCREENW * sizeof(double));
        usleep(10000);
    }
}

// Render function- maps i, j -> u, v -> x(u, v), y(u, v), z(u, v) then projects and does lighting calculation
void render(char screen[SCREENH][SCREENW], double zbuff[SCREENH][SCREENW])
{
    for (int i = 0; i < PRECISION; i++)
    {
        for (int j = 0; j < PRECISION; j++)
        {

            double u = x0 + (x1 - x0) * ((double)i / PRECISION);
            double v = yp0 + (yp1 - yp0) * ((double)j / PRECISION);
            double coord[3] = {x(u, v), y(u, v), z(u, v)};
            double scale_proj = PERSPECTIVE / (PERSPECTIVE + coord[2]);
            int scr_x = (coord[0] * scale_proj) + PROJ_CENTER_X;
            int scr_y = y_scale * ((coord[1] * scale_proj) + PROJ_CENTER_Y);

            vec3 a, b;
            r_u(u, v, &a);
            r_v(u, v, &b);
            cross(&a, &b);
            normalize(&a);

            double zrec = 1 / coord[2];
            if (zrec > zbuff[scr_y][scr_x])
            {

                zbuff[scr_y][scr_x] = zrec;
                double dotp = dot(&a, &light);
                int lighting = (dotp > 0 ? dotp : 0) * 6;
                screen[scr_y][scr_x] = "-.+*#@"[lighting];
            }
        }
    }
    for (int i = 0; i < SCREENH; i++)
    {
        for (int j = 0; j < SCREENW; j++)
        {
            putchar(screen[i][j] ? screen[i][j] : ' ');
        }
        putchar('\n');
    }
}

// Parameterizations
double x(double u, double v)
{
    return 27 * sin(u) * cos(v);
}

double y(double u, double v)
{
    return 27 * cos(u);
}

double z(double u, double v)
{
    return 27 * sin(u) * sin(v) + 7;
}

// Partial derivative of the vector function at u and v
void r_u(double u, double v, vec3 *vec)
{
    vec->x = (x(u + dx, v) - x(u, v)) / dx;
    vec->y = (y(u + dx, v) - y(u, v)) / dx;
    vec->z = (z(u + dx, v) - z(u, v)) / dx;
}

void r_v(double u, double v, vec3 *vec)
{
    vec->x = (x(u, v) - x(u, v + dx)) / dx;
    vec->y = (y(u, v) - y(u, v + dx)) / dx;
    vec->z = (z(u, v) - z(u, v + dx)) / dx;
}

// Vector operations- Resultant vectors are stored in vector a
void cross(vec3 *a, vec3 *b)
{
    double x = a->y * b->z - a->z * b->y;
    double y = -(a->x * b->z - a->z * b->x);
    double z = a->x * b->y - a->y * b->x;
    a->x = x, a->y = y, a->z = z;
}

double dot(vec3 *a, vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void normalize(vec3 *a)
{

    double magnitude = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
    a->x /= magnitude;
    a->y /= magnitude;
    a->z /= magnitude;
}
