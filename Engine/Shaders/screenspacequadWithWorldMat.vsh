//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;

uniform mat4 worldViewProjMat;
varying vec2 texcoord;

void main()
{
	gl_Position = worldViewProjMat * in_position;
	texcoord = in_texcoord;
}
