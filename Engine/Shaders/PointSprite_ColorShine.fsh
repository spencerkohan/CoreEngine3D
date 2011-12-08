//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;
varying vec4 color;

uniform sampler2D texture0;

void main(void)
{
	vec4 sampleColor = texture2D(texture0,texcoord);
    gl_FragColor = vec4(mix(color.xyz,vec3(1.0),sampleColor.x),sampleColor.y*color.w+sampleColor.x);
}
