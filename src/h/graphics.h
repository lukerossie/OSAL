#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../h/util.h"

/*Returns the screen width and height.*/
vec2 init_graphics();


void clear();
void flip();
char const *get_error();

typedef struct color
{
	u8 r,g,b,a;
} color;
u32 color_to_hex(color c);

#endif