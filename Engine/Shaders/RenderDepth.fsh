//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec4 position;
varying vec3 normal;

const vec4 packFactors = vec4( 256.0 * 256.0 * 256.0,256.0 * 256.0,256.0,1.0);
const vec4 bitMask     = vec4(0.0,1.0/256.0,1.0/256.0,1.0/256.0);

void main(void)
{
	//float normalizedDistance  = position.z / position.w;
	//normalizedDistance = (normalizedDistance + 1.0) / 2.0;

	//vec4 packedValue = vec4(fract(packFactors*normalizedDistance));
	//packedValue -= packedValue.xxyz * bitMask;
	
	//gl_FragColor  = packedValue;

	gl_FragColor  = vec4(normal,position.z/800.0);
}
