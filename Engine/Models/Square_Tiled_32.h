#include "GraphicsTypes.h"

//Model: "Square_Tiled_32"

static const float g_Square_Tiled_32_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.03125f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.03125f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.03125f,0.03125f,
};

static const u32 g_Square_Tiled_32_numberOfVertices	= 4;
static const u32 g_Square_Tiled_32_numberOfPrimitives = 1;
static const u32 g_Square_Tiled_32_numberOfAttributes = 2;

static PrimitiveData g_Square_Tiled_32_PrimitiveArray[g_Square_Tiled_32_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Square_Tiled_32_VertexData0,NULL,g_Square_Tiled_32_numberOfVertices,sizeof(g_Square_Tiled_32_VertexData0),0,0,0},
};

static AttributeData g_Square_Tiled_32_AttributesArray[g_Square1x1_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
	{ATTRIB_TEXCOORD,GL_FLOAT,2,12},
};

ModelData g_Square_Tiled_32_modelData = {0,g_Square_Tiled_32_AttributesArray,g_Square_Tiled_32_numberOfAttributes,20,"Square_Tiled_32",g_Square_Tiled_32_PrimitiveArray,g_Square_Tiled_32_numberOfPrimitives,-1};
