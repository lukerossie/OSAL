#ifndef INPUT_HPP
#define INPUT_HPP

#include "../../IFUM/hpp/IFUM.hpp"

extern vec2 mouse_scroll;
bool key( int keycode );
//on windows you must call this every frame
void poll();

enum class MB
{
	LEFT,
	RIGHT,
	MIDDLE,
	NONE
};
/*
If you specify a button and it isn't pressed, returns null.
If it is pressed, returns mouse position.
If you specify NONE it will return *Point. (if you just want position)
*/
vec2 *mouse( MB flag=MB::NONE );

#endif