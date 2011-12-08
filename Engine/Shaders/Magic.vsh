//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;
attribute vec3 in_normal;

varying vec2 texcoord;

uniform mat4 worldViewProjMat;

uniform float accumulatedTime;
uniform float transformT;

void main()
{	
	vec4 inPosition = in_position;
	
	inPosition.xyz += in_normal*transformT*6.0;
	
	gl_Position = worldViewProjMat * inPosition;
	texcoord = in_texcoord;
}
