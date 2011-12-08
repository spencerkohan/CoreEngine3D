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

void main()
{
	vec4 colorSample = texture2D(texture0, texcoord);
	
	gl_FragColor = vec4(mix(colorSample.rgb,color.rgb*colorSample.a,color.a),colorSample.a);
    
    if(gl_FragColor.a == 0.0)
    {
        discard;
    }
}
