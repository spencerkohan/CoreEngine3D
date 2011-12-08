//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;
attribute vec4 in_color;

varying vec2 texcoord;
varying vec4 color;

uniform mat4 worldViewProjMat;

uniform float accumulatedTime;
uniform float scrollAmountU;
uniform float scrollAmountV;

void main()
{	
	gl_Position = worldViewProjMat * in_position;
	texcoord = in_texcoord;
    texcoord.x += scrollAmountU*accumulatedTime;
    texcoord.y += scrollAmountV*accumulatedTime;
    color = in_color;
}
