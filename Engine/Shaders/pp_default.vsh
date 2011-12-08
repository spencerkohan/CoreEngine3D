//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;

uniform mat4 worldViewProjMat;
uniform float yFlip;

varying vec2 texcoord;

void main()
{
	gl_Position = worldViewProjMat * in_position;
	
	texcoord.x = in_position.x*0.5+0.5;
	texcoord.y = in_position.y*0.5+0.5;
}
