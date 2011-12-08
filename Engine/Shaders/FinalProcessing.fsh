//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform vec3 lightDir;

void main()
{	
	vec4 normalDepth = texture2D(texture1,texcoord);
	vec3 normal = normalize(normalDepth.xyz*2.0-vec3(1.0));
	float lighting = 0.3+dot(normal,lightDir);
	
	//gl_FragColor = mix(lighting*texture2D(texture0,texcoord),vec4(0.8,0.6,0.2,1.0),normalDepth.w);
	gl_FragColor = lighting*texture2D(texture0,texcoord);
}
