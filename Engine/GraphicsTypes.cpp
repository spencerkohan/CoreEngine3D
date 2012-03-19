#include "GraphicsTypes.h"

const color3f color3f_black = {0.0f,0.0f,0.0f};
const color4f color4f_yellow = {1.0f,1.0f,0.0f,1.0f};
const color4f color4f_blue = {0.0f,0.0f,1.0f,1.0f};
const color4f color4f_green = {0.0f,1.0f,0.0f,1.0f};
const color4f color4f_red = {1.0f,0.0f,0.0f,1.0f};
const color4f color4f_white = {1.0f,1.0f,1.0f,1.0f};
const color4f color4f_pink = {1.0f,0.0f,1.0f,1.0f};

const u32 PVRTEX_PIXELTYPE = 0xff;			// pixel type is always in the last 16bits of the flags


const u32 g_UniformSizes[Uniform_NumTypes] =
{
	sizeof(s32),//Uniform_Int,
	sizeof(f32),//Uniform_Float,
	sizeof(vec2),//Uniform_Vec2,
	sizeof(vec3),//Uniform_Vec3,
	sizeof(vec4),//Uniform_Vec4,
	sizeof(mat3f),//Uniform_Mat3x3,
    sizeof(mat4f),//Uniform_Mat4x4,
};
