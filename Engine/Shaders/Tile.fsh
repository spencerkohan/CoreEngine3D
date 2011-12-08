//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

varying vec2 texcoord;

uniform sampler2D texture0;

uniform float accumulatedTime;

void main()
{
	float NumTiles = 16.0;
	vec3 EdgeColor = vec3(0.8);
	vec3 invOff = vec3(1.0)-EdgeColor;
	float Threshhold = 0.1;
	float threshholdB = 1.0 - Threshhold;

    float size = 1.0/NumTiles;
    vec2 Pbase = texcoord - mod(texcoord,vec2(size));
    vec2 PCenter = Pbase + vec2(size/2.0);
    vec2 st = (texcoord - Pbase)/size;
    
	vec3 c1 = st.x > st.y?invOff:vec3(0.0);    
	vec3 cBottom = (st.x > threshholdB || st.y > threshholdB)?c1:vec3(0.0);
	if(st.y > threshholdB)
	{
		cBottom = invOff;
	}
	vec3 cTop = (st.x < Threshhold || st.y < Threshhold)?c1:vec3(0.0);
	
    vec4 tileColor = texture2D(texture0,PCenter);
    
	gl_FragColor = vec4(tileColor.rgb + cTop - cBottom,tileColor.a);
}
