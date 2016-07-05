#ifndef INPUT_HPP
#define INPUT_HPP

#include "../hpp/util.hpp"

bool key( s32 keycode );
//on windows you must call this every frame
void poll();

extern vec2 mouse_scroll;
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
If you specify NONE it will return vec2*. (if you just want position)
*/
vec2 *mouse( MB flag=MB::NONE );

extern const s32 KEY_LEFT;
extern const s32 KEY_RIGHT;
extern const s32 KEY_UP;
extern const s32 KEY_DOWN;
extern const s32 KEY_INSERT;
extern const s32 KEY_HOME;
extern const s32 KEY_PAGE_UP;
extern const s32 KEY_PAGE_DOWN;
extern const s32 KEY_PRINT_SCREEN;
extern const s32 KEY_SCROLL_LOCK;
extern const s32 KEY_PAUSE_BREAK;
extern const s32 KEY_KP_0;
extern const s32 KEY_KP_1;
extern const s32 KEY_KP_2;
extern const s32 KEY_KP_3;
extern const s32 KEY_KP_4;
extern const s32 KEY_KP_5;
extern const s32 KEY_KP_6;
extern const s32 KEY_KP_7;
extern const s32 KEY_KP_8;
extern const s32 KEY_KP_9;
extern const s32 KEY_KP_NUM_LOCK;
extern const s32 KEY_KP_DIVIDE;
extern const s32 KEY_KP_MULTIPLY;
extern const s32 KEY_KP_ADD;
extern const s32 KEY_KP_SUBTRACT;
extern const s32 KEY_KP_ENTER;
extern const s32 KEY_KP_PERIOD;
extern const s32 KEY_ESCAPE;
extern const s32 KEY_GRAVE;
extern const s32 KEY_0;
extern const s32 KEY_1;
extern const s32 KEY_2;
extern const s32 KEY_3;
extern const s32 KEY_4;
extern const s32 KEY_5;
extern const s32 KEY_6;
extern const s32 KEY_7;
extern const s32 KEY_8;
extern const s32 KEY_9;
extern const s32 KEY_SUBTRACT;
extern const s32 KEY_EQUALS;
extern const s32 KEY_F1;
extern const s32 KEY_F2;
extern const s32 KEY_F3;
extern const s32 KEY_F4;
extern const s32 KEY_F5;
extern const s32 KEY_F6;
extern const s32 KEY_F7;
extern const s32 KEY_F8;
extern const s32 KEY_F9;
extern const s32 KEY_F10;
extern const s32 KEY_F11;
extern const s32 KEY_F12;
extern const s32 KEY_TAB;
extern const s32 KEY_CAPS_LOCK;
extern const s32 KEY_ENTER;
extern const s32 KEY_BACKSPACE;
extern const s32 KEY_LEFT_BRACKET;
extern const s32 KEY_RIGHT_BRACKET;
extern const s32 KEY_SEMI_COLON;
extern const s32 KEY_QUOTE;
extern const s32 KEY_PERIOD;
extern const s32 KEY_COMMA;
extern const s32 KEY_DIVIDE;
extern const s32 KEY_BACK_SLASH;
extern const s32 KEY_A;
extern const s32 KEY_B;
extern const s32 KEY_C;
extern const s32 KEY_D;
extern const s32 KEY_E;
extern const s32 KEY_F;
extern const s32 KEY_G;
extern const s32 KEY_H;
extern const s32 KEY_I;
extern const s32 KEY_J;
extern const s32 KEY_K;
extern const s32 KEY_L;
extern const s32 KEY_M;
extern const s32 KEY_N;
extern const s32 KEY_O;
extern const s32 KEY_P;
extern const s32 KEY_Q;
extern const s32 KEY_R;
extern const s32 KEY_S;
extern const s32 KEY_T;
extern const s32 KEY_U;
extern const s32 KEY_V;
extern const s32 KEY_W;
extern const s32 KEY_X;
extern const s32 KEY_Y;
extern const s32 KEY_Z;
extern const s32 KEY_LEFT_SHIFT;
extern const s32 KEY_LEFT_CONTROL;
extern const s32 KEY_LEFT_ALT;
extern const s32 KEY_RIGHT_SHIFT;
extern const s32 KEY_RIGHT_CONTROL;
extern const s32 KEY_RIGHT_ALT;

#endif