#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../h/graphics.h"
#include "../h/util.h"
#include "../h/system.h"

static SDL_Renderer *renderer = 0;
static SDL_Window *window = 0;
static SDL_DisplayMode mode;
static color clear_color={0,0,0,255};

char const *get_error()
{
	return SDL_GetError();
}

u32 color_to_hex(color c)/*@bug assumes 32bit pixel*/
{
	u8 bpc=8;/*@todo actually handle variable size pixel*/
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		return ((c.r & 0xff) << bpc*3) + ((c.g & 0xff) << bpc*2) + ((c.b & 0xff) << bpc*1) + (c.a & 0xff << bpc*0);
	}
	else
	{
		return ((c.a & 0xff) << bpc*3) + ((c.b & 0xff) << bpc*2) + ((c.g & 0xff) << bpc*1) + (c.r & 0xff << bpc*0);
	}
}

/*
If width or height is 0 sets width and height from screen size (displaymode).
Returns the screen width and height.
*/
vec2 init_graphics()
{
	vec2 retval;
	char const *title=""; 
	int width=0;
	int height=0;
	int flags=SDL_WINDOW_FULLSCREEN;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "Critical Error\n SDL_Init failed. Application will now exit.\n" );
		exit(-1);
	}
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		printf( "SDL_GetCurrentDisplayMode failed." );
	}
	/*linear texture filtering*/
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "SDL_SetHint failed." );
	}

	if( width==0 || height==0 )
	{
		width=mode.w;
		height=mode.h;
	}
	if( !( window = SDL_CreateWindow( title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN | flags ) ) )/*show the window by default*/
	{
		printf( "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	if( !( renderer = SDL_CreateRenderer( window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ) )/*can | with SDL_RENDERER_PRESENTVSYNC here*/
	{
		printf( "SDL_CreateRenderer failed." );
	}
	/*white renderer*/
	if( SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 ) < 0 )
	{
		printf( "SDL_SetRenderDrawColor failed. Trying software fallback." );
		renderer=SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE );
	}
	if( !IMG_Init( IMG_INIT_PNG ) )/*returns bitmask on success, 0 is failure*/
	{
		printf( "IMG_Init failed." );
	}

	if( TTF_Init() ) {/*returns 0 on success, nonzero on error*/
		printf( "TTF_Init failed." );
	}

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); 
	SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE); 
	if(SDL_GL_SetSwapInterval(-1)==-1)
	{
		SDL_GL_SetSwapInterval(1);
	}

	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	retval.x=width;
	retval.y=height;
	/*no longer spawning a polling thread here - must be polled on the main thread on all platforms*/
	return retval;
}

void clear()
{
	SDL_SetRenderDrawColor(renderer, clear_color.r,clear_color.g,clear_color.b,clear_color.a);
	SDL_RenderClear( renderer );
}
void flip()
{
	SDL_RenderPresent( renderer );
}
