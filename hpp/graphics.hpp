#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../hpp/util.hpp"

//Returns the screen width and height.
vec2 init_graphics();

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
texture *ctor_texture(char const *image_file_path);
texture *ctor_texture(color *pixels, u32 size_x, u32 size_y);
void dtor_texture(texture *t);
void draw_texture(texture *texture, rect *dest=0, f32 angle=0, vec2 *center=0, rect *src=0);
#endif