#import "GraphicsTypes.h"

//Model: "Square_Tiled_16"

static const float g_Square_Tiled_16_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.0625f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.0625f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.0625f,0.0625f,
};

#define g_Square_Tiled_16_numberOfVertices	4
#define g_Square_Tiled_16_numberOfPrimitives 1

static PrimitiveData g_Square_Tiled_16_PrimitiveArray[g_Square_Tiled_16_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square_Tiled_16_VertexData0,NULL,g_Square_Tiled_16_numberOfVertices,sizeof(g_Square_Tiled_16_VertexData0),0,0,0},
};

ModelData g_Square_Tiled_16_modelData = {VertexFormat_VT,"Square_Tiled_16",g_Square_Tiled_16_PrimitiveArray,g_Square_Tiled_16_numberOfPrimitives,-1};
