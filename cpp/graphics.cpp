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
#include "../../IFUM/hpp/IFUM.hpp"

namespace
{
	SDL_Renderer *renderer = 0;
	SDL_Window *window = 0;
	SDL_DisplayMode mode;
	color clear_color={0,0,0,255};
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
u8 &color::operator[](int index)
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
vec2 init_graphics( char const *title, int width, int height, int flags )
{
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