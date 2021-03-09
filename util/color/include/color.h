#ifndef COLOR_H
#define COLOR_H

#include <ctype.h>

typedef enum { // 8-bit colors
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
} color_t;

typedef struct { // color attribs
	color_t foreground;
	color_t background;
} color_char_t;

/* return a color_char_t with the foreground and the background correspondient of a char */
color_char_t colorof(unsigned char ch);

#endif // COLOR_H
