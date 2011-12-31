#import "GraphicsTypes.h"

//Model: "cubemodel"

static const float g_Cube_VertexData[] = {
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

static const u32 g_Cube_numberOfVertices = 36;
static const u32 g_Cube_numberOfPrimitives = 1;
static const u32 g_Cube_numberOfAttributes = 1;

static PrimitiveData g_Cube_PrimitiveArray[g_Cube_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Cube_VertexData,NULL,g_Cube_numberOfVertices,sizeof(g_Cube_VertexData),0,0,0},
};

static AttributeData g_Cube_AttributesArray[g_Cube_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
};


ModelData g_Cube_modelData = {0,g_Cube_AttributesArray,g_Cube_numberOfAttributes,34,"Square1x1",g_Cube_PrimitiveArray,g_Cube_numberOfPrimitives,-1};

