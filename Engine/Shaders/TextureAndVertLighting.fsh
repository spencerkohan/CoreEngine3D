//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;
varying float lighting;
varying vec3 specColor;

void main()
{
	gl_FragColor = vec4(specColor+(lighting*texture2D(texture0, texcoord).rgb),1.0);
}
