#import "GraphicsTypes.h"

//Model: "Square_Anim_10x10"

//Note: Use for animated characters that have a texture with width = 1024, height = 1024

static const float g_Square_Anim_10x10_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.1f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.1f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.1f,0.1f,
};

#define g_Square_Anim_10x10_numberOfVertices	4
#define g_Square_Anim_10x10_numberOfPrimitives 1

static PrimitiveData g_Square_Anim_10x10_PrimitiveArray[g_Square_Anim_10x10_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square_Anim_10x10_VertexData0,NULL,g_Square_Anim_10x10_numberOfVertices,sizeof(g_Square_Anim_10x10_VertexData0),0,0,0},
};

ModelData g_Square_Anim_10x10_modelData = {VertexFormat_VT,"Square_Anim_10x10",g_Square_Anim_10x10_PrimitiveArray,g_Square_Anim_10x10_numberOfPrimitives,-1};
