//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;

varying vec4 color;
varying vec2 texcoord;

uniform mat4 worldViewProjMat;
uniform vec4 inputColor;

void main()
{			
	gl_Position = worldViewProjMat * in_position;
    
	texcoord = in_texcoord;
    
    color = inputColor;
}
