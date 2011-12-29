#import "GraphicsTypes.h"

//Model: "DEBUGMODEL_CircleXZ"

static const float g_DEBUGMODEL_CircleXZ_VertexData0[] = {
	0.500000f*2.0f,0.000000f,-0.000000f*2.0f,
	0.311745f*2.0f,0.000000f,-0.390916f*2.0f,
	-0.111261f*2.0f,0.000000f,-0.487464f*2.0f,
	-0.450484f*2.0f,0.000000f,-0.216942f*2.0f,
	-0.450484f*2.0f,0.000000f,0.216942f*2.0f,
	-0.111260f*2.0f,0.000000f,0.487464f*2.0f,
	0.311745f*2.0f,0.000000f,0.390916f*2.0f,
};

#define g_DEBUGMODEL_CircleXZ_numberOfVertices	7
#define g_DEBUGMODEL_CircleXZ_numberOfPrimitives 1
#define g_DEBUGMODEL_CircleXZ_numberOfAttributes 1

static PrimitiveData  g_DEBUGMODEL_CircleXZ_PrimitiveArray[g_DEBUGMODEL_CircleXZ_numberOfPrimitives]={
	{GL_LINE_LOOP,(u8*)g_DEBUGMODEL_CircleXZ_VertexData0,NULL,g_DEBUGMODEL_CircleXZ_numberOfVertices,sizeof(g_DEBUGMODEL_CircleXZ_VertexData0),0,0,0},
};

static AttributeData g_DEBUGMODEL_CircleXZ_AttributesArray[g_DEBUGMODEL_CircleXZ_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
};


static ModelData g_DEBUGMODEL_CircleXZ_modelData = {0,g_DEBUGMODEL_CircleXZ_AttributesArray,g_DEBUGMODEL_CircleXZ_numberOfAttributes,12,"DEBUGMODEL_CircleXZ",g_DEBUGMODEL_CircleXZ_PrimitiveArray,g_DEBUGMODEL_CircleXZ_numberOfPrimitives,-1};
