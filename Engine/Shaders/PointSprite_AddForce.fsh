//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D texture0; //backbuffer
uniform sampler2D texture1; //blurball

uniform vec2 touchVelocity;
varying vec2 texcoord;

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
	return vec4(Pack16Float(clamp(myVec.x,-16.0,16.0)),Pack16Float(clamp(myVec.y,-16.0,16.0)));
}

vec2 Unpack32Vec2(in vec4 myVec)
{
	return vec2(Unpack16Float(myVec.rg),Unpack16Float(myVec.ba));
}

void main(void)
{
	vec2 velocity = Unpack32Vec2(texture2D(texture0,texcoord));
	vec2 force = 2.0*touchVelocity*texture2D(texture1,gl_PointCoord).x;
		
	gl_FragColor = Pack32Vec2(velocity+force);
}