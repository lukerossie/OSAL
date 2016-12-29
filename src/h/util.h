#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

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

typedef u8 bool;
#define true 1
#define false 0

extern const f64 PI;

u32 cstr_len(char const *cstr);

typedef struct vec2
{
	f64 x, y;
} vec2;

vec2 vec2_scale(vec2 v,f64 factor);
f64 vec2_to_angle();
f64 vec2_len();
vec2 vec2_abs();
vec2 vec2_unit();

#endif
