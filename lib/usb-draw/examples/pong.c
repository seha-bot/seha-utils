#include<draw.h>
#include<main.h>

const float ballSpeed = 1.0f;
const float p1Speed = 0.5f;
const float p2Speed = 0.8f;
const int ph = 6;

float bx = 25.0f;
float by = 25.0f;
int bdx = 1;
int bdy = 1;

float p1 = 25.0f;
int p1d = 0;

float p2 = 0.0f;
int p2d = 0;

char buffer[20];
int s = 1;
int s1 = 0;
int s2 = 0;

int isIn(int x, int y, int px, int py, int ph)
{
    return py-ph < y && y < py+ph && px == x;
}

void setup() {}
void loop()
{
    //Line draw
    for(int i = 0; i < 50; i++) if((i & 4) < 4) pixel(24, i, 40);

    drawString("sehaOS", 1, 1, 15);

    //Players and ball drawing
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < W; j++)
        {
            if(isIn(i, j, 3, p1, ph) || isIn(i, j, 50-1-3, p2, ph) || (i==(int)bx && j==(int)by))
            {
                pixel(i, j, MAXCOL);
            }
            else if(get_pixel(i, j) != 15 && get_pixel(i, j) != 40) pixel(i, j, 0);
        }
    }

    //Score draw
    sprintf(buffer, "%d", s1);
    drawString(buffer, 12, 40, 70);
    sprintf(buffer, "%d", s2);
    drawString(buffer, 30, 40, 70);

    //Ball and player controls
    bx += bdx * ballSpeed;
    by += bdy * ballSpeed;
    p1 = clamp(p1 + p1d*p1Speed, ph-1, W-ph);
    p2 = clamp(p2 + p2d*p2Speed, ph-1, W-ph);

    p1d = by > p1 ? 1 : -1;
    p2d = by > p2 ? 1 : -1;
    
    //Player bounce
    if(isIn(bx, by, 3, p1, ph) || isIn(bx, by, 50-1-3, p2, ph))
    {
        bdx = -bdx;
        bdy = rand()&1 ? 1 : -1;
    }

    //Edge bounce
    if(bx < 24 && s != 2) s = -1;
    else if(bx > 24 && s != -2) s = 1;
    if(bx >= 50 || bx <= 0)
    {
        if(bdx == 1 && s == 1)
        {
            s1++;
            s = -2;
        }
        if(bdx == -1 && s == -1)
        {
            s2++;
            s = 2;
        }
        bdx = -bdx;
    }
    if(by >= 50 || by <= 0) bdy = -bdy;

    //Send data over USB
    sendBuffer();
    HAL_Delay(1); //TODO: make fps limit
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
