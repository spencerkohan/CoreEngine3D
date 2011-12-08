//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

//NOTE: for now this is my fade to black shader
//TODO: rename this shader or make a new one

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform vec4 color;

void main()
{
	gl_FragColor = color;
}
