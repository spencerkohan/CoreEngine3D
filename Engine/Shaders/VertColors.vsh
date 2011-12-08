//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec4 in_color;

varying vec4 color;

uniform mat4 worldViewProjMat;

void main()
{	
	gl_Position = worldViewProjMat * in_position;
	
	color = in_color;
}
