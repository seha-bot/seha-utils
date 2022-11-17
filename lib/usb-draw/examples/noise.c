#include<draw.h>
#include<main.h>

void setup() {}
void loop()
{
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < W; j++)
        {
            pixel(i, j, rand()%MAXCOL);
        }
    }
    //Send data over USB
    sendBuffer();
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
