//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;
varying vec2 texcoord2;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{	
	gl_FragColor = texture2D(texture0,texcoord)*texture2D(texture1,texcoord2);
}
