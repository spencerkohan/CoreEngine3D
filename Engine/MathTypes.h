//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifndef __MATH_TYPES
#define __MATH_TYPES

#define PI 3.141592653589f
#define PI_2 (2.0f*PI)

#define PI_DIV_2 (PI*0.5f)
#define PI_DIV_4 (PI*0.25f)

typedef float f32;
typedef double f64;
typedef signed int	s32;
typedef signed long int sl32;
typedef unsigned int u32;
typedef unsigned long ul32;
typedef char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef float mat4f[16];
typedef float mat3f[9];

#if DEBUG
	#define tconst 
#else
	#define tconst const
#endif

typedef struct
{
	f32 x;
	f32 y;
}  vec2;

typedef struct
{
	f32 x;
	f32 y;
	f32 z;
}  vec3;

typedef struct
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
}  vec4;

vec4 MakeVec4(float singleValue);

struct PlaneVec2 {
	vec2 point;
	vec2 normal;
};

struct PlaneVec3 {
	vec3 point;
	vec3 normal;
};

extern const vec3 vec3_up;
extern const vec3 vec3_down;
extern const vec3 vec3_at;
extern const vec3 vec3_away;
extern const vec3 vec3_left;
extern const vec3 vec3_right;
extern const vec3 vec3_zero;
extern const vec2 vec2_zero;
extern const vec2 vec2_up;
extern const vec2 vec2_left;
extern const vec2 vec2_right;
extern const vec4 vec4_one;

#endif
