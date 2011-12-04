#import "GraphicsTypes.h"

//Model: "Square_TopHalf"

static const float g_Square_TopHalf_VertexData0[] = {
	/*v:*/-1.0f, -1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.25f,0.0f, 
	/*v:*/-1.0f,  0.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.25f,1.0f,
	/*v:*/ 1.0f, -1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.75f,0.0f,  
	/*v:*/ 1.0f,  0.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.75f,1.0f,
};

#define g_Square_TopHalf_numberOfVertices	4
#define g_Square_TopHalf_numberOfPrimitives 1

static PrimitiveData g_Square_TopHalf_PrimitiveArray[g_Square_TopHalf_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square_TopHalf_VertexData0,NULL,g_Square_TopHalf_numberOfVertices,sizeof(g_Square_TopHalf_VertexData0),0,0,0},
};

ModelData g_Square_TopHalf_modelData = {VertexFormat_VNT,"Square_TopHalf",g_Square_TopHalf_PrimitiveArray,g_Square_TopHalf_numberOfPrimitives,-1};
