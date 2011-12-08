//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

//varying vec3 normal;
varying vec2 texcoord;
//varying lowp vec3 specColor;
//varying lowp float lighting;

uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;
//uniform vec3 camPos;
//uniform lowp vec3 lightDir;
uniform vec3 playerPos;
uniform float accumulatedTime;

uniform float transformT;

void main()
{			
	gl_Position = projMat*viewMat*worldMat * in_position;

	vec3 worldPos = (worldMat * in_position).xyz;
	
	vec3 distVec = worldPos-playerPos;
	distVec.y = 0.0;
	float lengthOfVec = length(distVec);
	float sinEffect = clamp((lengthOfVec/100.0)*transformT,0.0,1.0)*25.0*transformT;
	
	gl_Position.y -= (sin(accumulatedTime*20.0+lengthOfVec)*0.5+0.5)*sinEffect;
	
	//normal = (worldMat*vec4(in_normal,0.0)).xyz;
	//TODO: normalize?
	
	//vec3 eyeDistVec = camPos-worldPos;
	
	//float distToEye = length(eyeDistVec);
	//vec3 eyeVec = eyeDistVec/distToEye;
	
	//vec3 h = normalize(lightDir+eyeVec);
	//float nDotH = dot(normal,h);
	//specColor = vec3(pow(nDotH,3.0)*0.15);
	
	//lighting = mix(0.5,1.0,dot(normal,lightDir));
	
	texcoord = in_texcoord;
}
