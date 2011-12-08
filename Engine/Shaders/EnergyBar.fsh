//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoordArt;
varying vec2 texcoordEnergy;
varying vec2 texcoordAnimated1;
varying vec2 texcoordAnimated2;
varying float lerpT;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 energyColor;
uniform vec3 bgColor;

void main()
{
	vec4 colorSample = texture2D(texture0, texcoordArt);
    vec4 energyPowerMult = texture2D(texture1, texcoordAnimated1);
    vec4 energyPowerAdd = texture2D(texture1, texcoordAnimated2);
    //gl_FragColor = vec4(energyPower.xyz,1.0);

	vec3 finalColor = mix(energyColor*energyPowerMult.x+vec3(energyPowerAdd.y*1.3*lerpT),bgColor,texture2D(texture0, texcoordEnergy).y);
	
	gl_FragColor = vec4(colorSample.x*finalColor,colorSample.z);
}
