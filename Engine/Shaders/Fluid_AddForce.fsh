//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;
varying vec2 rotatedTexCoord;

uniform sampler2D texture0; //velocity

uniform vec2 touchCoords;
uniform vec2 touchVelocity;
uniform float touchRadius;

vec2 Pack16Float(in float myFloat)
{
	float bsInput = myFloat*8.0 + 128.0;
	
	vec2 packedFloat;
	packedFloat.x = floor(bsInput);
	packedFloat.y = bsInput-packedFloat.x;
	
	packedFloat.x /= 255.0;
	
	return packedFloat;
}

float Unpack16Float(in vec2 myVec)
{
	return (myVec.x*255.0 + myVec.y - 128.0)/8.0;
}

vec4 Pack32Vec2(in vec2 myVec)
{
	return vec4(Pack16Float(myVec.x),Pack16Float(myVec.y));
}

vec2 Unpack32Vec2(in vec4 myVec)
{
	return vec2(Unpack16Float(myVec.rg),Unpack16Float(myVec.ba));
}

void main()
{	
	vec2 velocity = Unpack32Vec2(texture2D(texture0, texcoord));
								
	float dist = length(texcoord-touchCoords);
	dist = clamp(dist/touchRadius,0.0,1.0);
	dist = 1.0-dist;
	
	vec2 force = touchVelocity*dist;
		
	gl_FragColor = Pack32Vec2(velocity+force);
}
