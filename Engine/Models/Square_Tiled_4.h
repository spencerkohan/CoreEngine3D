#include "GraphicsTypes.h"

//Model: "Square_Tiled_4"

static const float g_Square_Tiled_4_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.25f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.25f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.25f,0.25f,
};

static const u32 g_Square_Tiled_4_numberOfVertices = 4;
static const u32 g_Square_Tiled_4_numberOfPrimitives = 1;
static const u32 g_Square_Tiled_4_numberOfAttributes = 2;

static PrimitiveData g_Square_Tiled_4_PrimitiveArray[g_Square_Tiled_4_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Square_Tiled_4_VertexData0,NULL,g_Square_Tiled_4_numberOfVertices,sizeof(g_Square_Tiled_4_VertexData0),0,0,0},
};

static AttributeData g_Square_Tiled_4_AttributesArray[g_Square_Tiled_4_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
	{ATTRIB_TEXCOORD,GL_FLOAT,2,12},
};

ModelData g_Square_Tiled_4_modelData = {0,g_Square_Tiled_4_AttributesArray,g_Square_Tiled_4_numberOfAttributes,20,"Square_Tiled_4",g_Square_Tiled_4_PrimitiveArray,g_Square_Tiled_4_numberOfPrimitives,-1};
