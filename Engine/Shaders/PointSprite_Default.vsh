//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

attribute vec4 in_position;
attribute vec2 in_texcoord;
attribute vec4 in_color;

varying vec2 texcoord;
varying vec4 color;

uniform mat4 worldViewProjMat;

void main()
{
	texcoord = in_texcoord;
	
	color = in_color;

	gl_Position = worldViewProjMat * in_position;
}
