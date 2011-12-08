//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

uniform float accumulatedTime;

attribute vec4 in_position;
attribute vec2 in_texcoord;

uniform mat4 worldViewProjMat;

varying vec2 texcoord1;
varying vec2 texcoord2;

void main()
{
	gl_Position = worldViewProjMat * in_position;
	
    float time = accumulatedTime*0.003;
	//gl_Position.y += sin(accumulatedTime);
	texcoord1 = in_texcoord;
	texcoord2 = 2.2*vec2(in_texcoord.x,in_texcoord.y-time);
}
