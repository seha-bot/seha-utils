#include "termui.h"
#include "nec.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

void termui_terminal_size(int* width, int* height)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    *width = w.ws_col;
    *height = w.ws_row;
}

struct termios orig_termios;

void termui_raw_mode(int enable)
{
    if(enable)
    {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG);
        raw.c_iflag &= ~(IXON | ICRNL);
        raw.c_oflag &= ~(OPOST);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }
    else tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf(TERMUI_RESET);
}

char termui_read(char* out)
{
    char c;
    read(STDIN_FILENO, &c, 1);
    if(out) *out = c;
    return c;
}

termui_object* termui_object_init(char flags, int width, int height, ...)
{
    va_list args;
    va_start(args, height);

    termui_object** children = 0;
    
    while(1)
    {
        termui_object* child = va_arg(args, termui_object*);
        if(!child) break;
        nec_push(children, child);
    }

    termui_object object = { children, width, height, 0, 0, 0, flags };
    termui_object* object_pointer = 0;
    nec_push(object_pointer, object); // TODO: Store this someplace else
    va_end(args);
    return object_pointer;
}

void draw_border(termui_object* obj)
{
    for(int y = 0; y < obj->height; y++)
    {
        printf(TERM_MC, obj->y + 1 + y, obj->x + 1);
        for(int x = 0; x < obj->width; x++)
        {
            if(!y && !x || y == obj->height - 1 && x == obj->width - 1) printf("/");
            else if(!y && x == obj->width - 1 || !x && y == obj->height - 1) printf("\\");
            else if(!y || y == obj->height - 1) printf("-");
            else if(!x || x == obj->width - 1) printf("|");
            else printf(" ");
        }
    }
}

int constrain_to_parent(int size, int parent)
{
    if(size) return min(size, parent);
    return parent;
}

void get_child_area(termui_object* obj, int* width, int* height)
{
    if(!obj->children) return;
    const int hasBorder = obj->flags & TERMUI_BORDER;
    const int isRow = (obj->flags & TERMUI_ROW) >> 1;
    if(hasBorder) draw_border(obj);

    int offset = 0;
    int maxSize[2] = { 0, 0 };

    int expandedChildCount = 0;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        if(isRow && obj->children[i]->width == 0 || !isRow && obj->children[i]->height == 0) expandedChildCount++;
    }

    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui_object* child = obj->children[i];
        int oldSize[2] = { child->width, child->height };
        child->width = constrain_to_parent(child->width, obj->width - 1 - hasBorder);
        child->height = constrain_to_parent(child->height, obj->height - 1 - hasBorder);
        child->x = obj->x + hasBorder + offset * isRow;
        child->y = obj->y + hasBorder + offset * !isRow;
        termui_object_plot(child);
        offset += isRow ? child->width : child->height;
        maxSize[0] = max(maxSize[0], child->width);
        maxSize[1] = max(maxSize[1], child->height);
        child->width = oldSize[0];
        child->height = oldSize[1];
    }

    if(nec_size(obj->children) > 1)
    {
        obj->width = isRow ? offset : maxSize[0];
        obj->height = isRow ? maxSize[1] : offset;
    }
}

void termui_object_plot(termui_object* obj)
{
    const int hasBorder = obj->flags & TERMUI_BORDER;
    const int isRow = (obj->flags & TERMUI_ROW) >> 1;
    if(hasBorder) draw_border(obj);

    int offset = 0;
    int maxSize[2] = { 0, 0 };

    int expandedChildCount = 0;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        if(isRow && obj->children[i]->width == 0) expandedChildCount++;
        else if(!isRow && obj->children[i]->height == 0) expandedChildCount++;
    }

    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui_object* child = obj->children[i];
        int oldSize[2] = { child->width, child->height };
        child->width = constrain_to_parent(child->width, obj->width - 1 - hasBorder);
        child->height = constrain_to_parent(child->height, obj->height - 1 - hasBorder);
        child->x = obj->x + hasBorder + offset * isRow;
        child->y = obj->y + hasBorder + offset * !isRow;
        termui_object_plot(child);
        offset += isRow ? child->width : child->height;
        maxSize[0] = max(maxSize[0], child->width);
        maxSize[1] = max(maxSize[1], child->height);
        child->width = oldSize[0];
        child->height = oldSize[1];
    }

    if(nec_size(obj->children) > 1)
    {
        obj->width = isRow ? offset : maxSize[0];
        obj->height = isRow ? maxSize[1] : offset;
    }

    fflush(stdout);
}

