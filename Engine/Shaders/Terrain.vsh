//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

varying vec2 texcoord;

varying float lighting;

uniform mat4 worldViewProjMat;

void main()
{			
	gl_Position = worldViewProjMat * in_position;

	//lighting = mix(0.4,1.0,in_normal.x)+0.1;
	lighting = 1.0;
	
	texcoord = in_texcoord;
}
