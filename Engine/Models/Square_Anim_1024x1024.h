#include "GraphicsTypes.h"

//Model: "Square_Anim_1024x1024"

//Note: Use for animated characters that have a texture with width = 1024, height = 1024

static const float g_Square_Anim_1024x1024_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.125f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.125f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.125f,0.125f,
};

#define g_Square_Anim_1024x1024_numberOfVertices	4
#define g_Square_Anim_1024x1024_numberOfPrimitives 1

static PrimitiveData g_Square_Anim_1024x1024_PrimitiveArray[g_Square_Anim_1024x1024_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square_Anim_1024x1024_VertexData0,NULL,g_Square_Anim_1024x1024_numberOfVertices,sizeof(g_Square_Anim_1024x1024_VertexData0),0,0,0},
};

ModelData g_Square_Anim_1024x1024_modelData = {VertexFormat_VT,"Square_Anim_1024x1024",g_Square_Anim_1024x1024_PrimitiveArray,g_Square_Anim_1024x1024_numberOfPrimitives,-1};
