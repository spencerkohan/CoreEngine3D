//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;

uniform mat4 worldViewProjMat;

void main()
{
	gl_Position = worldViewProjMat * in_position;
}
