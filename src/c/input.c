#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#endif

#include <stdlib.h>

#include "../h/input.h"
#include "../h/util.h"

const s32 LEFT_MOUSE=SDL_BUTTON_LEFT;
const s32 RIGHT_MOUSE=SDL_BUTTON_RIGHT;
const s32 MIDDLE_MOUSE=SDL_BUTTON_MIDDLE;
const s32 MOUSE_MOTION=1;/*@bug i dont know what the range is on the SDL constants, this might also be another key*/
const s32 MOUSE_WHEEL=2;

const s32 KEY_LEFT=SDLK_LEFT;
const s32 KEY_RIGHT=SDLK_RIGHT;
const s32 KEY_UP=SDLK_UP;
const s32 KEY_DOWN=SDLK_DOWN;
const s32 KEY_INSERT=SDLK_INSERT;
const s32 KEY_HOME=SDLK_HOME;
const s32 KEY_PAGE_UP=SDLK_PAGEUP;
const s32 KEY_PAGE_DOWN=SDLK_PAGEDOWN;
const s32 KEY_PRINT_SCREEN=SDLK_PRINTSCREEN;
const s32 KEY_SCROLL_LOCK=SDLK_SCROLLLOCK;
const s32 KEY_PAUSE_BREAK=SDLK_PAUSE;
const s32 KEY_KP_0=SDLK_KP_0;
const s32 KEY_KP_1=SDLK_KP_1;
const s32 KEY_KP_2=SDLK_KP_2;
const s32 KEY_KP_3=SDLK_KP_3;
const s32 KEY_KP_4=SDLK_KP_4;
const s32 KEY_KP_5=SDLK_KP_5;
const s32 KEY_KP_6=SDLK_KP_6;
const s32 KEY_KP_7=SDLK_KP_7;
const s32 KEY_KP_8=SDLK_KP_8;
const s32 KEY_KP_9=SDLK_KP_9;
const s32 KEY_KP_NUM_LOCK=SDLK_NUMLOCKCLEAR;
const s32 KEY_KP_DIVIDE=SDLK_KP_DIVIDE;
const s32 KEY_KP_MULTIPLY=SDLK_KP_MULTIPLY;
const s32 KEY_KP_ADD=SDLK_PLUS;
const s32 KEY_KP_SUBTRACT=SDLK_MINUS;
const s32 KEY_KP_ENTER=SDLK_KP_ENTER;
const s32 KEY_KP_PERIOD=SDLK_KP_ENTER;
const s32 KEY_ESCAPE=SDLK_ESCAPE;
const s32 KEY_GRAVE=SDLK_BACKQUOTE;
const s32 KEY_0=SDLK_0;
const s32 KEY_1=SDLK_1;
const s32 KEY_2=SDLK_2;
const s32 KEY_3=SDLK_3;
const s32 KEY_4=SDLK_4;
const s32 KEY_5=SDLK_5;
const s32 KEY_6=SDLK_6;
const s32 KEY_7=SDLK_7;
const s32 KEY_8=SDLK_8;
const s32 KEY_9=SDLK_9;
const s32 KEY_SUBTRACT=SDLK_MINUS;
const s32 KEY_EQUALS=SDLK_EQUALS;
const s32 KEY_F1=SDLK_F1;
const s32 KEY_F2=SDLK_F2;
const s32 KEY_F3=SDLK_F3;
const s32 KEY_F4=SDLK_F4;
const s32 KEY_F5=SDLK_F5;
const s32 KEY_F6=SDLK_F6;
const s32 KEY_F7=SDLK_F7;
const s32 KEY_F8=SDLK_F8;
const s32 KEY_F9=SDLK_F9;
const s32 KEY_F10=SDLK_F10;
const s32 KEY_F11=SDLK_F11;
const s32 KEY_F12=SDLK_F12;
const s32 KEY_TAB=SDLK_TAB;
const s32 KEY_CAPS_LOCK=SDLK_CAPSLOCK;
const s32 KEY_ENTER=SDLK_RETURN;
const s32 KEY_BACKSPACE=SDLK_BACKSPACE;
const s32 KEY_LEFT_BRACKET=SDLK_LEFTBRACKET;
const s32 KEY_RIGHT_BRACKET=SDLK_RIGHTBRACKET;
const s32 KEY_SEMI_COLON=SDLK_SEMICOLON;
const s32 KEY_QUOTE=SDLK_QUOTE;
const s32 KEY_PERIOD=SDLK_PERIOD;
const s32 KEY_COMMA=SDLK_COMMA;
const s32 KEY_DIVIDE=SDLK_SLASH;
const s32 KEY_BACK_SLASH=SDLK_BACKSLASH;
const s32 KEY_A=SDLK_a;
const s32 KEY_B=SDLK_b;
const s32 KEY_C=SDLK_c;
const s32 KEY_D=SDLK_d;
const s32 KEY_E=SDLK_e;
const s32 KEY_F=SDLK_f;
const s32 KEY_G=SDLK_g;
const s32 KEY_H=SDLK_h;
const s32 KEY_I=SDLK_i;
const s32 KEY_J=SDLK_j;
const s32 KEY_K=SDLK_k;
const s32 KEY_L=SDLK_l;
const s32 KEY_M=SDLK_m;
const s32 KEY_N=SDLK_n;
const s32 KEY_O=SDLK_o;
const s32 KEY_P=SDLK_p;
const s32 KEY_Q=SDLK_q;
const s32 KEY_R=SDLK_r;
const s32 KEY_S=SDLK_s;
const s32 KEY_T=SDLK_t;
const s32 KEY_U=SDLK_u;
const s32 KEY_V=SDLK_v;
const s32 KEY_W=SDLK_w;
const s32 KEY_X=SDLK_x;
const s32 KEY_Y=SDLK_y;
const s32 KEY_Z=SDLK_z;
const s32 KEY_LEFT_SHIFT=SDLK_LSHIFT;
const s32 KEY_LEFT_CONTROL=SDLK_LCTRL;
const s32 KEY_LEFT_ALT=SDLK_LALT;
const s32 KEY_RIGHT_SHIFT=SDLK_RSHIFT;
const s32 KEY_RIGHT_CONTROL=SDLK_RCTRL;
const s32 KEY_RIGHT_ALT=SDLK_RALT;

event *poll(event *e)
{
	SDL_Event sdl_event;
    
	if(SDL_PollEvent( &sdl_event ))
	{
		if( sdl_event.type == SDL_QUIT ) exit( 0 );/*if they try to close the window we just quit immediately*/

		if( sdl_event.key.type == SDL_KEYDOWN )
		{
			e->type=sdl_event.key.keysym.sym;
			e->pressed=true;
		}
		if( sdl_event.key.type == SDL_KEYUP )
		{
			e->type=sdl_event.key.keysym.sym;
			e->pressed=false;
		}

		if( sdl_event.button.type == SDL_MOUSEBUTTONDOWN )
		{
			e->type=sdl_event.button.button;
			e->pressed=true;
		}
		if( sdl_event.button.type == SDL_MOUSEBUTTONUP )
		{
			e->type=sdl_event.button.button;
			e->pressed=false;
		}

		if( sdl_event.button.type == SDL_MOUSEMOTION )
		{
			e->type=MOUSE_MOTION;
			e->pressed=false;
			e->mouse_info.x = sdl_event.motion.x;
			e->mouse_info.y = sdl_event.motion.y;
		}
		if( sdl_event.wheel.type == SDL_MOUSEWHEEL )
		{
			e->type=MOUSE_WHEEL;
			e->pressed=false;
			e->mouse_info.x = -sdl_event.wheel.x;
			e->mouse_info.y = -sdl_event.wheel.y;
		}
		return e;
	}
	return NULL;
}
