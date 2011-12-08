//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

varying	vec4 color;

uniform mat4 worldViewProjMat;

void main()
{			
	vec4 expandedPosition = in_position;
	expandedPosition.xyz += in_normal*0.13;
	gl_Position = worldViewProjMat * expandedPosition;
	color = vec4(0.2,0.2,0.2,1.0);//3214
}
