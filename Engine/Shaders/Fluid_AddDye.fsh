//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0; //dye texture

uniform vec2 touchCoords;
uniform vec2 touchVelocity;
uniform float touchRadius;
uniform vec4 paintColor;

void main()
{	
	float dist = length(texcoord-touchCoords);
	dist = 1.0-clamp(dist/touchRadius,0.0,1.0);
		
	gl_FragColor = mix(texture2D(texture0,texcoord),paintColor,dist*paintColor.a);
}
