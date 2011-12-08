//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_normal;

varying vec3 normal;

uniform mat4 worldViewProjMat;

uniform mat4 worldMat;
uniform float inverseModelScale;

void main()
{			
	gl_Position = worldViewProjMat * in_position;

	vec3 worldPos = (worldMat * in_position).xyz;

	//I happen to know the matrix is scaled by 50 so I multiply by 1/50 which is 0.02
	normal = inverseModelScale*((worldMat*vec4(in_normal,0.0)).xyz);
}
