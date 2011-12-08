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
	const vec2 constantList = vec2(1.0, 0.0);

	gl_FragColor = constantList.xxxy * vec4(0.3)
				 + constantList.xxxy * texture2D(texture0, texcoord1) * texture2D(texture0, texcoord2)
				 + vec4(0.0,0.0,0.0,0.8);
}
