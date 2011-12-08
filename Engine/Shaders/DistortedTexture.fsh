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
uniform sampler2D texture1;

void main()
{
	vec4 distortion = texture2D(texture1, texcoord2);
   
    vec2 texCoordOffset = (distortion.xy*2.0-1.0)*0.004;
    float lighting = distortion.x*0.23;
    
    
	gl_FragColor = texture2D(texture0, texcoord1+texCoordOffset);
    gl_FragColor.rgb/=gl_FragColor.a;
    gl_FragColor.rgb += lighting;

}
