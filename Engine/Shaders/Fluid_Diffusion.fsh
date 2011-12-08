//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0; // x vector (Ax = b)
uniform sampler2D texture1; // b vector (Ax = b) CONSTANT

uniform vec2 rViewPortSize;

uniform float rBeta; // reciprocal beta
uniform float alpha; // alpha

/*
Notice that the rBeta parameter is the reciprocal of b from Equation 16.
To solve the diffusion equation, we set alpha to (dx)2/ndt, rBeta to 1/(4 + (dx)2/ndt),
and the x and b parameters to the velocity texture. We then run a number of
iterations (usually 20 to 50, but more can be used to reduce the error).
*/

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
	//b = texture0
	//x = texture1
	
	// left, right, bottom, and top x samples
	vec2 xL = Unpack32Vec2(texture2D(texture0, texcoord - vec2(rViewPortSize.x, 0.0)));
	vec2 xR = Unpack32Vec2(texture2D(texture0, texcoord + vec2(rViewPortSize.x, 0.0)));
	vec2 xB = Unpack32Vec2(texture2D(texture0, texcoord - vec2(0.0, rViewPortSize.y)));
	vec2 xT = Unpack32Vec2(texture2D(texture0, texcoord + vec2(0.0, rViewPortSize.y)));

	// b sample, from center
	vec2 bC = Unpack32Vec2(texture2D(texture1, texcoord));

	// evaluate Jacobi iteration
	gl_FragColor = Pack32Vec2((xL + xR + xB + xT + alpha * bC) * rBeta);
}
