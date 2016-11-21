#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#endif

#include "../hpp/input.hpp"
#include "../hpp/util.hpp"

#include <map>//placeholder - probably shouldnt use a map at all 
using namespace std;

namespace
{
	map<int, bool> keyState;
	SDL_mutex *mutex = SDL_CreateMutex();

	map<char, bool> mouseState;
	vec2 mousePos;//internal use
	/*
	mouse() will either return null or a pointer to this mouseCopy,
	copied respecting mutex.
	*/
	vec2 mouseCopy;
	vec2 *mousePointer;
	vec2 mouseScrollVal;
}

vec2 mouse_scroll;

void poll()
{
    SDL_Event event;
	mouse_scroll.x=0;
	mouse_scroll.y=0;
    while(SDL_PollEvent( &event ))
	{
		if( event.type == SDL_QUIT ) exit( 0 );//if they try to close the window we just quit immediately

		if( event.key.type == SDL_KEYDOWN )
		{
			keyState[event.key.keysym.sym] = true;
		}
		if( event.key.type == SDL_KEYUP )
		{
			keyState[event.key.keysym.sym] = false;
		}

		if( event.button.type == SDL_MOUSEBUTTONDOWN )
		{
			mouseState[event.button.button] = true;
			mousePos.x = event.button.x;
			mousePos.y = event.button.y;
		}
		if( event.button.type == SDL_MOUSEBUTTONUP )
		{
			mouseState[event.button.button] = false;
			mousePos.x = event.button.x;
			mousePos.y = event.button.y;
		}

		if( event.button.type == SDL_MOUSEMOTION )
		{
			mousePos.x = event.motion.x;
			mousePos.y = event.motion.y;
		}
		if( event.wheel.type == SDL_MOUSEWHEEL )
		{
			mouse_scroll.x = -event.wheel.x;
			mouse_scroll.y = -event.wheel.y;
			mouse_scroll=mouse_scroll;
		}
	}
}

bool key( SDL_Keycode whichKey )
{
	if( whichKey > 1073742106 || whichKey < 0 ) return false;
	bool value = keyState[whichKey];
	return value;
}
vec2 *mouse( MB flag )
{
	SDL_LockMutex( mutex );
	mouseCopy = mousePos;
	SDL_UnlockMutex( mutex );
	mousePointer = &mouseCopy;

	if(flag == MB::LEFT)
	{
		if(mouseState[SDL_BUTTON_LEFT])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::RIGHT)
	{
		if(mouseState[SDL_BUTTON_RIGHT])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::MIDDLE)
	{
		if(mouseState[SDL_BUTTON_MIDDLE])
		{
			return mousePointer;
		}
		else
		{
			return 0;
		}
	}
	else if(flag == MB::NONE)
	{
		return mousePointer;
	}
	return 0;
}

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