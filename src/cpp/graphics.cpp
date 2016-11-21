#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "../hpp/graphics.hpp"
#include "../hpp/util.hpp"
#include "../hpp/system.hpp"

namespace
{
	SDL_Renderer *renderer = 0;
	SDL_Window *window = 0;
	SDL_DisplayMode mode;
	color clear_color={0,0,0,255};
}

char const *get_error()
{
	return SDL_GetError();
}

color::color():r(0),g(0),b(0),a(255){}
color::color(u32 hex):r(0),g(0),b(0),a(255)
{
	u8 bpp=8;//@todo actually handle variable size pixel
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		r=(hex >> bpp*3);
		g=(hex >> bpp*2);
		b=(hex >> bpp*1);
		a=(hex >> bpp*0);
	}
	else
	{
		a=(hex >> bpp*3);
		b=(hex >> bpp*2);
		g=(hex >> bpp*1);
		r=(hex >> bpp*0);
	}
}
color::color(u8 r_p,u8 g_p,u8 b_p,u8 a_p):r(r_p),g(g_p),b(b_p),a(a_p){}
u8 &color::operator[](u32 index)
{
	switch(index){
		case 0:
		return r;
		break;
		case 1:
		return g;
		break;
		case 2:
		return b;
		break;
		case 3:
		return a;
		break;
		default:
		return a;
		break;
	}
}
u32 color::to_hex()//@bug assumes 32bit pixel
{
	u8 bpc=8;//@todo actually handle variable size pixel
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return ((r & 0xff) << bpc*3) + ((g & 0xff) << bpc*2) + ((b & 0xff) << bpc*1) + (a & 0xff << bpc*0);
	else
		return ((a & 0xff) << bpc*3) + ((b & 0xff) << bpc*2) + ((g & 0xff) << bpc*1) + (r & 0xff << bpc*0);
}

/*
If width or height is 0 sets width and height from screen size (displaymode).
Returns the screen width and height.
*/
vec2 init_graphics()
{
	char const *title=""; 
	int width=0;
	int height=0;
	int flags=SDL_WINDOW_FULLSCREEN;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		print( "Critical Error\n SDL_Init failed. Application will now exit.\n" );
		exit(-1);
	}
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		print( "SDL_GetCurrentDisplayMode failed." );
	}
	//Linear texture filtering
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		print( "SDL_SetHint failed." );
	}

	if( width==0 || height==0 )
	{
		width=mode.w;
		height=mode.h;
	}
	if( !( window = SDL_CreateWindow( title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN | flags ) ) )//show the window by default
	{
		print( "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	if( !( renderer = SDL_CreateRenderer( window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ) )//can or with SDL_RENDERER_PRESENTVSYNC here
	{
		print( "SDL_CreateRenderer failed." );
	}
	//White renderer
	if( SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 ) < 0 )
	{
		print( "SDL_SetRenderDrawColor failed. Trying software fallback." );
		renderer=SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE );
	}
	if( !IMG_Init( IMG_INIT_PNG ) )//returns bitmask on success, 0 is failure
	{
		print( "IMG_Init failed." );
	}

	if( TTF_Init() ) {//returns 0 on success, nonzero on error
		print( "TTF_Init failed." );
	}

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); 
	SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE); 
	if(SDL_GL_SetSwapInterval(-1)==-1)
	{
		SDL_GL_SetSwapInterval(1);
	}

	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

	//no longer spawning a polling thread here - must be polled on the main thread on all platforms
	return vec2(width, height);
}

void clear()
{
	SDL_SetRenderDrawColor(renderer, clear_color[0],clear_color[1],clear_color[2],clear_color[3]);
	SDL_RenderClear( renderer );
}
void flip()
{
	SDL_RenderPresent( renderer );
}

namespace
{
	void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
	{
		Uint8 *pixel_to_write=(Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32*)pixel_to_write=pixel;
	}
	Uint32 get_pixel(SDL_Surface *surface, int x, int y)
	{
		return *(Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * 4);
	}
	SDL_Surface *make_rgb_surface(int size_x,int size_y)
	{
		u32 rmask, gmask, bmask, amask;
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		}
		else
		{
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		}

		SDL_Surface *surf = SDL_CreateRGBSurface(0, size_x,size_y, 32, rmask, gmask, bmask, amask);

		if(surf == NULL)
		{
			print(SDL_GetError());
		}
		return surf;
	}
}
struct texture
{
	SDL_Texture *sdl_texture;
};
texture *ctor_texture(char const *image_file_path)
{
	SDL_Surface* surf = IMG_Load( image_file_path );
	texture *t=new texture();
	t->sdl_texture = SDL_CreateTextureFromSurface( renderer, surf );
    if(!t->sdl_texture)
    {
        print(SDL_GetError());
    }
	SDL_FreeSurface( surf );
    SDL_SetTextureBlendMode(t->sdl_texture, SDL_BLENDMODE_BLEND);
	return t;
}
texture *ctor_texture(color *pixels, u32 size_x, u32 size_y)
{
    SDL_Surface *surf=make_rgb_surface(size_x,size_y);

	if(surf==NULL) 
	{
        print(SDL_GetError());
	}

	SDL_LockSurface(surf);
	for(u32 a=0; a<size_x; a++)
	{
		for(u32 b=0; b<size_y; b++)
		{
			set_pixel(surf,a,b,pixels[a+b*size_x].to_hex());
		}
	}
	SDL_UnlockSurface(surf);

	SDL_Texture *sdl_texture=SDL_CreateTextureFromSurface(renderer,surf);
    if(!sdl_texture)print(SDL_GetError());
	SDL_FreeSurface(surf);
	texture *t=new texture();
	t->sdl_texture=sdl_texture;
    SDL_SetTextureBlendMode(t->sdl_texture, SDL_BLENDMODE_BLEND);
	return t;
}
texture *ctor_texture(std::vector<std::vector<color>> const &pixels);//impl
texture *ctor_texture(ttf_font *f,char const *text, color text_color);//impl
std::shared_ptr<std::vector<std::vector<color>>> query_texture(texture *);//impl
void dtor_texture(texture *t)
{
	SDL_DestroyTexture(t->sdl_texture);
	delete (t);
}
void draw_texture(texture *texture, rect *dest, f32 angle, vec2 *center, rect *src)
{
	SDL_Rect sdl_src=SDL_Rect();
	if(src)
	{
		sdl_src.x=src->x;
		sdl_src.y=src->y;
		sdl_src.w=src->w;
		sdl_src.h=src->h;
	}
	SDL_Rect sdl_dest=SDL_Rect();
	if(dest)
	{
		sdl_dest.x=dest->x;
		sdl_dest.y=dest->y;
		sdl_dest.w=dest->w;
		sdl_dest.h=dest->h;
	}
	SDL_Point sdl_center=SDL_Point();
	if(center)
	{
		sdl_center.x=center->x;
		sdl_center.y=center->y;
	}
	SDL_RenderCopyEx( renderer, texture->sdl_texture, src?&sdl_src:NULL, dest?&sdl_dest:NULL, angle, center?&sdl_center:NULL, SDL_FLIP_NONE );
}
void set_texture_alpha(texture *t,u8 alpha);//impl

struct ttf_font
{
	TTF_Font *font;
};
ttf_font *ctor_ttf_font(char const *font_file_path,int fontsize);//impl
void dtor_ttf_font(ttf_font *font);//impl
void size_ttf_font(ttf_font *font, const char *text, int *w, int *h);//impl
