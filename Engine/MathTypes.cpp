//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#include "MathTypes.h"

const vec3 vec3_up = {0.0f,1.0f,0.0f};
const vec3 vec3_down = {0.0f,-1.0f,0.0f};
const vec3 vec3_at = {0.0f,0.0f,1.0f};
const vec3 vec3_away = {0.0f,0.0f,-1.0f};
const vec3 vec3_left = {1.0f,0.0f,0.0f};
const vec3 vec3_right = {-1.0f,0.0f,0.0f};
const vec3 vec3_zero = {0.0f,0.0f,0.0f};
const vec2 vec2_zero = {0.0f,0.0f};
const vec2 vec2_up = {0.0f,1.0f};
const vec2 vec2_left = {-1.0f,0.0f};
const vec2 vec2_right = {1.0f,0.0f};
const vec4 vec4_one = {1.0f,1.0f,1.0f,1.0f};

vec4 MakeVec4(float singleValue)
{
	vec4 newVec4;
	newVec4.x = singleValue;
	newVec4.y = singleValue;
	newVec4.z = singleValue;
	newVec4.w = singleValue;
	return newVec4;
}
