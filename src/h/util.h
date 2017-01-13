#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define DEBUG 1

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef double f64;
typedef float f32;

#ifndef __cplusplus
	#ifndef bool
	typedef u8 bool;
	#endif

#define true 1
#define false 0
#endif

extern const f64 PI;

typedef struct vec2
{
	f32 x;
	f32 y;
} vec2;
typedef struct vec3
{
	f32 x;
	f32 y;
	f32 z;
} vec3;
typedef struct vec4
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} vec4;

vec2 vec2_scale(vec2 v,f64 factor);
f64 vec2_to_angle();
f64 vec2_len();
vec2 vec2_abs();
vec2 vec2_unit();

#endif
