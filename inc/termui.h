#ifndef SEHA_TERMUI
#define SEHA_TERMUI

// COOL COLORS
#define TERMUI_RESET "\033[0m"
#define TERMUI_OK "\033[30m"
#define TERMUI_BAD "\033[31m"

// CONTROL
#define TERMUI_NOBLINK "\033[?25l"
#define TERM_MC "\033[%d;%dH"

// ALIGNMENT
#define TERMUI_START  0x01
#define TERMUI_CENTER 0x02
#define TERMUI_END    0x04

// FLAGS
#define TERMUI_BORDER 0x01
#define TERMUI_ROW    0x02

void termui_terminal_size(int*, int*);
void termui_raw_mode(int);
char termui_read(char*);

struct termui_object
{
    struct termui_object **children;
    int width, height, x, y;
    const char* text;
    char flags;
};
typedef struct termui_object termui_object;

termui_object* termui_object_init(char, int, int, ...);
void termui_object_plot(termui_object*);

#endif /* SEHA_TERMUI */

