//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

#define NUMSAMPLES 6

varying vec2 texcoord;

uniform sampler2D texture0;
uniform vec2 viewPortSize;
uniform float gaussianWeights[NUMSAMPLES];


void main()
{
	vec4 sum = vec4(0.0,0.0,0.0,0.0);
	
	for (int i=0; i<NUMSAMPLES; i++)
	{
		float offset = float(i)-float(NUMSAMPLES)*0.5;
		
		vec2 currTexCoord = texcoord + offset*vec2(0.0,1.0/viewPortSize.y);
		sum += gaussianWeights[i] * texture2D(texture0,currTexCoord);
	}
	
	gl_FragColor = sum;
}
