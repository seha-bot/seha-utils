#include<draw.h>
#include<main.h>

#define HW (W>>1)

float p[2];
int r = 90;
#define M_PI 3.14159265358979323846
#define SIN(a) sin(a*M_PI/180.0)
#define COS(a) cos(a*M_PI/180.0)

#define GS 10
uint8_t walls[GS][GS] = {
    {1,1,1,1,1,1,1,1,1,1,},
    {1,0,0,0,0,0,0,0,0,1,},
    {1,0,0,0,0,0,0,0,0,1,},
    {1,0,0,1,0,0,1,0,0,1,},
    {1,0,0,0,0,0,0,0,0,1,},
    {1,0,1,0,0,0,0,1,0,1,},
    {1,0,1,1,1,1,1,1,0,1,},
    {1,0,0,0,0,0,0,0,0,1,},
    {1,0,0,0,0,0,0,0,0,1,},
    {1,1,1,1,1,1,1,1,1,1,},
};
#define CW (W/GS)
#define CH (W/GS)

void box(int x, int y, int w, int h, u_int32_t c)
{
    for(int i = x; i < x+w; i++)
    {
        for(int j = y; j < y+h; j++)
        {
            pixel(i, j, c);
        }
    }
}

float t_ray[2] = {0,0};
float t_delta;
int t_side;
void ray(int a)
{
    if(a < 0) a += 360;
    if(a > 359) a -= 360;

    float rayDirX = COS(a);
    float rayDirY = SIN(a);

    float dX = sqrt( 1 + (rayDirY*rayDirY) / (rayDirX*rayDirX) );
    float dY = sqrt( 1 + (rayDirX*rayDirX) / (rayDirY*rayDirY) );

    int mapX = p[0];
    int mapY = p[1];
    int stepX, stepY;

    if(rayDirX < 0)
    {
        stepX = -1;
        t_ray[0] = (p[0] - mapX) * dX;
    }
    else
    {
        stepX = 1;
        t_ray[0] = (mapX + 1.0f - p[0]) * dX;
    }
    if(rayDirY < 0)
    {
        stepY = -1;
        t_ray[1] = (p[1] - mapY) * dY;
    }
    else
    {
        stepY = 1;
        t_ray[1] = (mapY + 1.0f - p[1]) * dY;
    }

    while(1)
    {
        if(t_ray[0] < t_ray[1])
        {
            mapX += stepX;
            t_side = 0;
            t_ray[0] += dX;
        }
        else
        {
            mapY += stepY;
            t_side = 1;
            t_ray[1] += dY;
        }
        if(mapX < 0 || mapY < 0 || mapX >= GS || mapY >= GS) break;
        if(walls[GS-1-mapY][mapX]) break;
    }

    t_delta = t_side ? (t_ray[1]-dY) : (t_ray[0]-dX);
    t_ray[0] = p[0] + rayDirX * t_delta;
    t_ray[1] = p[1] + rayDirY * t_delta;
}

void draw2D()
{
    for(int x = 0; x < GS; x++)
    {
        for(int y = 0; y < GS; y++)
        {
            box(x*CW, y*CH, CW, CH, walls[GS-1-y][x] ? MAXCOL : 0);
            box(x*CW-1, y*CH-1, 1, CH, MAXCOL>>2);
            box(x*CW-1, y*CH-1, CH, 1, MAXCOL>>2);
        }
    }

    int fov = 30;
    for(int i = 0; i < fov; i++)
    {
        ray(r+i-(fov>>1));
        line(p[0]*CW, p[1]*CH, t_ray[0]*CW, t_ray[1]*CH, MAXCOL>>1);
    }
}

void draw3D()
{
    int fov = 60;
    int c;
    for(int i = 0; i < fov; i++)
    {
        ray(r+i-(fov>>1));
        if(t_delta < 1) c = MAXCOL;
        else c = (int)(MAXCOL / t_delta);
        box(W-i*(W/(float)fov), HW-((1/t_delta)*HW), (W/(float)fov)+1, (1/t_delta*2)*HW, clamp(c + (40*t_side),0,MAXCOL));
    }
}

void setup()
{
    p[0] = 5;
    p[1] = 5;
}
void loop()
{
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < W; j++)
        {
            pixel(i, j, 0);
        }
    }
    draw2D();
    r += 3;
    if(r >= 360) r -= 360;

    //Send data over USB
    sendBuffer();
    HAL_Delay(5);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
