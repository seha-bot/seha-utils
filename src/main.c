#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "termui.h"

volatile char c = 0;

void* always()
{
    while(1) termui_read((char*)&c);
    return 0;
}

int main()
{
    termui_raw_mode(1);
    printf(TERMUI_NOBLINK);

    termui_object* root = termui_object_init(TERMUI_BORDER | TERMUI_ROW, 0, 0,
        termui_object_init(TERMUI_BORDER, 5, 5, 0),
        termui_object_init(TERMUI_BORDER, 6, 0, 0),
        termui_object_init(TERMUI_BORDER, 7, 7, 0), 0
    );

    pthread_t tid = 0;
    pthread_create(&tid, 0, always, 0);

    while(c != 'q')
    {
        termui_terminal_size(&root->width, &root->height);
        termui_object_plot(root);
        usleep(10000);
    }
    return 0;
}

