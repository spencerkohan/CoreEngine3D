#import "GraphicsTypes.h"

//Model: "Square_Tiled_8"

static const float g_Square_Tiled_8_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*t:*/0.0f,0.125f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*t:*/0.125f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*t:*/0.125f,0.125f,
};

#define g_Square_Tiled_8_numberOfVertices	4
#define g_Square_Tiled_8_numberOfPrimitives 1

static PrimitiveData g_Square_Tiled_8_PrimitiveArray[g_Square_Tiled_8_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square_Tiled_8_VertexData0,NULL,g_Square_Tiled_8_numberOfVertices,sizeof(g_Square_Tiled_8_VertexData0),0,0,0},
};

ModelData g_Square_Tiled_8_modelData = {VertexFormat_VT,"Square_Tiled_8",g_Square_Tiled_8_PrimitiveArray,g_Square_Tiled_8_numberOfPrimitives,-1};
