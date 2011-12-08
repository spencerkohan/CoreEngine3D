//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

varying vec2 texcoord;
varying vec3 specColor;

uniform mat4 worldMat;
uniform mat4 worldViewProjMat;
uniform vec3 lightDir;
uniform vec3 camPos;
uniform float inverseModelScale;
uniform float meltT;

void main()
{			
	vec4 inPosition = in_position;
	inPosition.xyz -= in_normal*meltT;
	
	gl_Position = worldViewProjMat * inPosition;

	vec3 worldPos = (worldMat * inPosition).xyz;
	
	//I happen to know the matrix is scaled by 50 so I multiply by 1/50 which is 0.02
	vec3 normal = inverseModelScale*((worldMat*vec4(in_normal,0.0)).xyz);

	vec3 eyeDistVec = camPos-worldPos;
	
	float distToEye = length(eyeDistVec);
	vec3 eyeVec = eyeDistVec/distToEye;
	
	vec3 h = normalize(lightDir+eyeVec);
	float nDotH = dot(normal,h);
	
	specColor.x = pow(nDotH,20.0);
	
	specColor.y = 1.4-dot(eyeVec,normal);

	texcoord = in_texcoord;
}
