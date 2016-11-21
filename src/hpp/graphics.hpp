#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../hpp/util.hpp"
#include <vector>
#include <memory>

//Returns the screen width and height.
vec2 init_graphics();

//Clear screen
void clear();
//Copy your renderer to the window
void flip();
//pop implementation defined error message
char const *get_error();

struct color
{
	u8 r,g,b,a;
	color();
	color(u32 hex);
	color(u8 r_p, u8 g_p, u8 b_p, u8 a_p);
	u8 &operator[](u32 index);
	u32 to_hex();
};

//if all ctors return 0 on error, dont have to move error handling to inside

struct ttf_font;
ttf_font *ctor_ttf_font(char const *font_file_path,int fontsize);
void dtor_ttf_font(ttf_font *font);
void size_ttf_font(ttf_font *font, const char *text, int *w, int *h);

struct texture;
texture *ctor_texture(char const *image_file_path);
texture *ctor_texture(std::vector<std::vector<color>> const &pixels);
texture *ctor_texture(ttf_font *f,char const *text, color text_color);
std::shared_ptr<std::vector<std::vector<color>>> query_texture(texture *);
void dtor_texture(texture *t);
void draw_texture(texture *texture, rect *dest=0, f32 angle=0, vec2 *center=0, rect *src=0);
void set_texture_alpha(texture *t,u8 alpha);

#endif