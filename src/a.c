#include<gl_utils.h>
#include<pthread.h>

#define W 50
#define MS (W*W)
uint8_t map[MS];

#define MAXCOL ('~'-' ')

int loop()
{
    for(int i = 0; i < W; i++)
    {
        for(int j = 0; j < W; j++)
        {
            int val = ((double)(map[i + j * W] - ' ') / MAXCOL) * 255;
            pixel(i, j, (val/255.0) * 0xFFFFFF);
        }
    }
    return 0;
}

char buff[MS];
void* threadDraw()
{
    while(1)
    {
        FILE * fp = fopen("/dev/ttyACM0", "r");
        fread(buff, MS, 1, fp);
        int i = 0;
        while(i < MS)
        {
            char c = buff[i];
            map[i++] = c;
        }
        fclose(fp);
    }
}

int main(int argc, char** argv)
{
    pthread_t thread_id;
    pthread_create(&thread_id, 0, threadDraw, 0);

    createWindow("Window", W<<1, W<<1);
    if(!window) return 1;
    glPointSize(2.0);
    start(loop);

    return 0;
}
