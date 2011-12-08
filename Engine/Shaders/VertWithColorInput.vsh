//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;

varying vec4 color;

uniform mat4 worldViewProjMat;
uniform vec4 inputColor;

void main()
{			
	gl_Position = worldViewProjMat * in_position;
    
    color = inputColor;
}
