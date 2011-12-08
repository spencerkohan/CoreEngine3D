//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

//varying vec3 worldPos;
//varying vec3 normal;
varying vec2 texcoord;

uniform sampler2D texture0;
varying float lighting;

//varying lowp vec3 specColor;

void main()
{
	const vec2 constantList = vec2(1.0, 0.0);
	gl_FragColor = (lighting*texture2D(texture0, texcoord)) * constantList.xxxy + constantList.yyyx;
}
