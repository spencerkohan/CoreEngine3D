//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0; //u = input velocity
uniform sampler2D texture1; //x = qty to advect

uniform vec2 viewPortSize;
uniform float rdxDT;
uniform vec2 rViewPortSize;

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
	//u = texture0
	//x = texture1
	
	// follow the velocity field "back in time"
	vec2 offsetTex = texcoord - rdxDT * (Unpack32Vec2(texture2D(texture0, texcoord)));

	//No operations being done so just copy the pixels directly (MAYBE)
	//It DOES say interpolate

	// interpolate and write to the output fragment
	gl_FragColor = texture2D(texture1,offsetTex);
}
