//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;

void main()
{
	vec4 color0 = texture2D(texture0,texcoord);
	float brightness = (color0.r+color0.g+color0.b)/3.0;
	if (brightness > 0.35)
	{
		gl_FragColor = color0;
	}
	else
	{
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	}
}
