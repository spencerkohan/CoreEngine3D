#import "GraphicsTypes.h"

//Model: "Square1x1"

static const float g_Square1x1_VertexData0[] = {
	/*v:*/-0.5f, -0.5f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.0f,0.0f, 
	/*v:*/-0.5f,  0.5f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.0f,1.0f,
	/*v:*/ 0.5f, -0.5f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/1.0f,0.0f,  
	/*v:*/ 0.5f,  0.5f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/1.0f,1.0f,
};

#define g_Square1x1_numberOfVertices	4
#define g_Square1x1_numberOfPrimitives 1

static PrimitiveData  g_Square1x1_PrimitiveArray[g_Square1x1_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_Square1x1_VertexData0,NULL,g_Square1x1_numberOfVertices,sizeof(g_Square1x1_VertexData0),0,0,0},
};

ModelData g_Square1x1_modelData = {VertexFormat_VNT,"Square1x1",g_Square1x1_PrimitiveArray,g_Square1x1_numberOfPrimitives,-1};
