//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;

uniform mat4 worldViewProjMat;
uniform float accumulatedTime;

varying vec2 texcoord1;
varying vec2 texcoord2;

void main()
{
	gl_Position = worldViewProjMat * in_position;
	
	float timeAdjusted = accumulatedTime*1.5;

	texcoord1 = in_texcoord + vec2(0.0,timeAdjusted);
	texcoord2 = in_texcoord + vec2(0.0,timeAdjusted*0.5);
}
