#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../../IFUM/hpp/IFUM.hpp"

//Returns the screen width and height.
vec2 init_graphics( char const *title, int height, int width, int flags );

//Clear screen
void clear();
//Copy your renderer to the window
void flip();

struct color
{
	u8 r,g,b,a;
	color();
	color(u32 hex);
	color(u8 r_p, u8 g_p, u8 b_p, u8 a_p);
	u8 &operator[](u32 index);
	u32 to_hex();
};

struct texture;
texture *ctor_texture(char *image_file_path);
texture *ctor_texture(list<list<color>> *pixels);

#endif