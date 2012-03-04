//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;

varying vec2 texcoord;
varying vec2 texcoord2;

uniform mat4 worldViewProjMat;

uniform vec2 texCoordOffset;
uniform vec2 texCoordOffset2;

void main()
{			
	gl_Position = worldViewProjMat * in_position;
	texcoord = in_texcoord+texCoordOffset;
	texcoord2 = in_texcoord+texCoordOffset2;
}
