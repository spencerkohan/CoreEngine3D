//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;
varying vec3 hVec;
varying float rimLighting;

uniform vec3 blockColor;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{	
    //Normal
    vec4 colorSample = texture2D(texture1, texcoord);
    
    //X = rimLight amount, Y = Darkness, Z = Alpha
    vec4 colorSampleEdge = texture2D(texture0, texcoord);
    
    vec3 sampledNormal = (colorSample.xyz*2.0-1.0);

    float nDotH = dot(sampledNormal,hVec);
    float specular = pow(nDotH,9.0);

    vec3 color = blockColor*(colorSampleEdge.z + colorSampleEdge.x*rimLighting)+specular;
    gl_FragColor = vec4(color,colorSampleEdge.y+rimLighting+specular);
}