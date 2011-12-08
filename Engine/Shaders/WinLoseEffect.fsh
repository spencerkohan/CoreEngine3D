//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord1;
varying vec2 texcoord2;

uniform sampler2D texture0;

void main()
{
	float t1 = texture2D(texture0, texcoord1).x;
	float t2 = texture2D(texture0, texcoord2).z*0.5;
	
	gl_FragColor = vec4(vec3(t1+t2,0.0,0.0),1.0);
}
