#import "GraphicsTypes.h"

//Model: "cubemodel"

static const float g_cubemodel_VertexData[] = {
-0.5,-0.5,0.5,
0.5,-0.5,0.5,
-0.5,0.5,0.5,
0.5,0.5,0.5,
-0.5,0.5,0.5,
0.5,0.5,0.5,
-0.5,0.5,-0.5,
0.5,0.5,-0.5,
-0.5,0.5,-0.5,
0.5,0.5,-0.5,
-0.5,-0.5,-0.5,
0.5,-0.5,-0.5,
-0.5,-0.5,-0.5,
0.5,-0.5,-0.5,
-0.5,-0.5,0.5,
0.5,-0.5,0.5,
0.5,-0.5,0.5,
0.5,0.5,0.5,
-0.5,-0.5,0.5,
-0.5,0.5,0.5,
};

static const unsigned short g_cubemodel_IndexData0[] = {
0,1,2,3,3,12,12,18,6,19,19,4,4,5,6,7,7,16,16,13,17,7,7,12,12,13,14,15,15,8,8,9,
10,11,
};


#define g_cubemodel_numberOfVertices 36
#define g_cubemodel_numberOfPrimitives 1

static PrimitiveData g_cubemodel_PrimitiveArray[g_cubemodel_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_cubemodel_VertexData,(GLushort*)g_cubemodel_IndexData0,34,sizeof(g_cubemodel_VertexData),68,0,0},
};

ModelData g_cubemodel_modelData = {VertexFormat_V,"cubemodel",g_cubemodel_PrimitiveArray,g_cubemodel_numberOfPrimitives};
