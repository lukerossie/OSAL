#include <math.h>
#include "../h/util.h"
#include "../h/system.h"
#include <stdio.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

const f64 PI=3.1415926535897;

vec2 vec2_scale(vec2 v,f64 factor)
{
	v.x*=factor;
	v.y*=factor;
	return v;
}
f64 vec2_to_angle(vec2 v)
{
	return atan2(v.y, v.x)*180/PI;
}
f64 vec2_len(vec2 v)
{
	return sqrt(pow(v.x,2)+pow(v.y,2));
}
vec2 vec2_unit(vec2 v)
{
	f64 lenvalue=vec2_len(v);
	v.x=v.x/lenvalue;
	v.y=v.y/lenvalue;
	return v;
}
vec2 vec2_abs(vec2 v)
{
	v.x=(f64)fabs(v.x);
	v.y=(f64)fabs(v.y);
	return v;
}