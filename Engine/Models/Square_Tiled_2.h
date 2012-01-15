#include "GraphicsTypes.h"

//Model: "Square_Tiled_2"

static const float g_Square_Tiled_2_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.5f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.5f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.5f,0.5f,
};

static const u32 g_Square_Tiled_2_numberOfVertices = 4;
static const u32 g_Square_Tiled_2_numberOfPrimitives = 1;
static const u32 g_Square_Tiled_2_numberOfAttributes = 2;

static PrimitiveData g_Square_Tiled_2_PrimitiveArray[g_Square_Tiled_2_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Square_Tiled_2_VertexData0,NULL,g_Square_Tiled_2_numberOfVertices,sizeof(g_Square_Tiled_2_VertexData0),0,0,0},
};

static AttributeData g_Square_Tiled_2_AttributesArray[g_Square_Tiled_2_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
	{ATTRIB_TEXCOORD,GL_FLOAT,2,12},
};

ModelData g_Square_Tiled_2_modelData = {0,g_Square_Tiled_2_AttributesArray,g_Square_Tiled_2_numberOfAttributes,20,"Square_Tiled_2",g_Square_Tiled_2_PrimitiveArray,g_Square_Tiled_2_numberOfPrimitives,-1};
