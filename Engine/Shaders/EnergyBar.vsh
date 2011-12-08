//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

attribute vec4 in_position;
attribute vec2 in_normal;
attribute vec2 in_texcoord;

uniform mat4 worldViewProjMat;

varying vec2 texcoordArt;
varying vec2 texcoordEnergy;
varying vec2 texcoordAnimated1;
varying vec2 texcoordAnimated2;
varying float lerpT;

uniform float EnergyT;
uniform float accumulatedTime;

void main()
{
	gl_Position = worldViewProjMat * in_position;
	texcoordArt = in_texcoord;
    
	texcoordEnergy = in_normal.xy+vec2(EnergyT,0.0);

    lerpT = EnergyT*2.0;

    texcoordAnimated1 = in_texcoord*0.75+vec2(0.015*accumulatedTime,0.0);
    texcoordAnimated2 = in_texcoord*0.75*0.5+vec2(0.03*accumulatedTime,0.0);
}
