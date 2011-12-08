//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;
varying vec4 color;

void main()
{
    vec4 textureColor = texture2D(texture0, texcoord);
    //Divide by textureColor.w to fix pre-multiplied alpha issue
    gl_FragColor = vec4(color.xyz*(textureColor.xyz/textureColor.w),color.w*textureColor.w);
}
