#include<draw.h>
#include<main.h>

float getUV(float v, float max)
{
    return v/max;
}

void setup() {}
void loop()
{
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < W; j++)
        {
            pixel(i, j, getUV(i, W) * MAXCOL);
        }
    }

    //Send data over USB
    sendBuffer();
    HAL_Delay(10); //TODO: make fps limit
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
