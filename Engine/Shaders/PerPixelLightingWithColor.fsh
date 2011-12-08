//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec3 normal;

uniform vec3 lightDir;
uniform vec3 color;

void main()
{		 
	float lighting = 0.3+dot(normal,lightDir);
	gl_FragColor = vec4(lighting*color,1.0);
}
