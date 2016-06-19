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
#include "../../IFUM/hpp/IFUM.hpp"

namespace
{
	map<SDL_Keycode, bool> keyState;
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
			if(keyDownEvent)keyDownEvent(event.key.keysym.sym);
		}
		if( event.key.type == SDL_KEYUP )
		{
			keyState[event.key.keysym.sym] = false;
			if(keyUpEvent)keyUpEvent(event.key.keysym.sym);
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
