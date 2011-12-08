//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec3 in_tangent;
attribute vec3 in_normal;
attribute vec2 in_texcoord;

varying vec2 texcoord;
varying vec3 hVec;
varying float rimLighting;

uniform mat4 worldMat;
uniform mat4 worldViewProjMat;
uniform vec3 lightDir;
uniform vec3 camPos;
uniform float inverseModelScale;
uniform float texcoordMult;

void main()
{			
	gl_Position = worldViewProjMat * in_position;

	vec3 worldPos = (worldMat * in_position).xyz;
	
	vec3 eyeDistVec = camPos-worldPos;
	
	float distToEye = length(eyeDistVec);
	vec3 eyeVec = eyeDistVec/distToEye;
    
    vec3 normal = inverseModelScale*((worldMat*vec4(in_normal,0.0)).xyz);
    
    rimLighting = 1.0-clamp(dot(normal,eyeVec),0.0,1.0);
    
    vec3 tangent = inverseModelScale*((worldMat*vec4(in_tangent,0.0)).xyz);
    vec3 binormal = cross(normal,tangent);

    vec3 tempHVec = normalize(lightDir+eyeVec);
    hVec.x = dot(tempHVec, tangent);
	hVec.y = dot(tempHVec, binormal);
	hVec.z = dot(tempHVec, normal);
    
	texcoord = in_texcoord;
}
