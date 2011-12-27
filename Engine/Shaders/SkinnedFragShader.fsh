uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool bUseDot3;

varying mediump vec2 texcoord;
varying mediump vec3 lightDir;

void main()
{
	/*
	if(bUseDot3)
	{
		mediump vec3 fNormal = texture2D(texture1, texcoord).grb;
		mediump float fNDotL = dot((fNormal - 0.5) * 2.0, lightDir);
		
		gl_FragColor = texture2D(texture0, texcoord) * fNDotL;
    }
    else
	{
		gl_FragColor = texture2D(texture0, texcoord) * lightDir.x;
	}
	*/
	
	gl_FragColor = texture2D(texture0, texcoord);
}
