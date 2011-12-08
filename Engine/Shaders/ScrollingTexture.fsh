//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;//input
uniform sampler2D texture1;//sandy texture

uniform float accumulatedTime;

void main()
{
	vec4 color0 = texture2D(texture0,texcoord);
	
	color0.rgb -= (texture2D(texture1,texcoord+vec2(accumulatedTime*5.0,0.0)).x)*0.2*(1.0-texcoord.y);
	
	gl_FragColor = color0;
}
