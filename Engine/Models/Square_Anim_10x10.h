#import "GraphicsTypes.h"

//Model: "Square_Anim_10x10"

//Note: Use for animated characters that have a texture with width = 1024, height = 1024

static const float g_Square_Anim_10x10_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.1f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.1f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.1f,0.1f,
};

static const u32 g_Square_Anim_10x10_numberOfVertices =	4;
static const u32 g_Square_Anim_10x10_numberOfPrimitives = 1;
static const u32 g_Square_Anim_10x10_numberOfAttributes = 2;

static PrimitiveData g_Square_Anim_10x10_PrimitiveArray[g_Square_Anim_10x10_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Square_Anim_10x10_VertexData0,NULL,g_Square_Anim_10x10_numberOfVertices,sizeof(g_Square_Anim_10x10_VertexData0),0,0,0},
};

static AttributeData g_Square_Anim_10x10_AttributesArray[g_Square1x1_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
	{ATTRIB_TEXCOORD,GL_FLOAT,2,12},
};


ModelData g_Square_Anim_10x10_modelData = {0,g_Square_Anim_10x10_AttributesArray,20,g_Square_Anim_10x10_numberOfAttributes,"Square_Anim_10x10",g_Square_Anim_10x10_PrimitiveArray,g_Square_Anim_10x10_numberOfPrimitives,-1};
