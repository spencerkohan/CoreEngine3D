//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D texture0; //backbuffer

uniform vec4 paintColor;

void main(void)
{
    gl_FragColor = vec4(paintColor.rgb,texture2D(texture0,gl_PointCoord).x*paintColor.a);
}