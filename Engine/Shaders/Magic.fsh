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

uniform float accumulatedTime;
uniform float desertCubeAmount;

void main()
{	
	vec2 finalTexCoord = texcoord;
	finalTexCoord.x += texture2D(texture0, texcoord).x+accumulatedTime*3.0;
	finalTexCoord.y = 0.25;
	
	vec4 textureColor0 = texture2D(texture1, finalTexCoord);
	finalTexCoord.y = 0.75;
	vec4 textureColor1 = texture2D(texture1, finalTexCoord);
	
	gl_FragColor = mix(textureColor0,textureColor1,desertCubeAmount);
}
