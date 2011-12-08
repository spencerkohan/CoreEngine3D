//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;

uniform float accumulatedTime;

void main()
{
	float distance = length(vec2(0.5,0.5)-texcoord);
	
	float cosT = cos((distance+accumulatedTime)*10.0)*0.025;
	
	vec4 color0 = texture2D(texture0,texcoord+cosT);
	
	gl_FragColor = color0;
}
