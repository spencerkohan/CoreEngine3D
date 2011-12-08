//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_texcoord;

uniform mat4 worldViewProjMat;
uniform vec4 wiggleAmount;
uniform vec4 inputColor;

varying vec2 texcoord;
varying vec4 color;

void main()
{			
	gl_Position = worldViewProjMat * in_position;
    
	float lerpT = 1.0-in_texcoord.y;
	float wiggle = lerpT*sin(wiggleAmount.x+lerpT*wiggleAmount.y)*wiggleAmount.z;
	
	texcoord = in_texcoord + vec2(wiggle,0);
	
	color = inputColor;
}
