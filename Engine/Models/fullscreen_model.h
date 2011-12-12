#import "GraphicsTypes.h"

//Model: "Fullscreen_Model"

static const float g_Fullscreen_Model_VertexData0[] = {
	/*v:*/-1.0f, -1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.0f,0.0f, 
	/*v:*/-1.0f,  1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/0.0f,1.0f,
	/*v:*/ 1.0f, -1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/1.0f,0.0f,  
	/*v:*/ 1.0f,  1.0f, 0.0f, /*n:*/0.574267f, 0.583422f, -0.574267f, /*t:*/1.0f,1.0f,
};

static const u32 g_Fullscreen_Model_numberOfVertices = 4;
static const u32 g_Fullscreen_Model_numberOfPrimitives = 1;
static const u32 g_Fullscreen_Model_numberOfAttributes = 3;

static PrimitiveData g_Fullscreen_Model_PrimitiveArray[g_Fullscreen_Model_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(u8*)g_Fullscreen_Model_VertexData0,NULL,g_Fullscreen_Model_numberOfVertices,sizeof(g_Fullscreen_Model_VertexData0),0,0,0},
};

static AttributeData g_Fullscreen_Model_AttributesArray[g_Fullscreen_Model_numberOfVertices]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
	{ATTRIB_NORMAL,GL_FLOAT,3,12},
	{ATTRIB_TEXCOORD,GL_FLOAT,2,24},
};

ModelData g_Fullscreen_Model_modelData = {g_Fullscreen_Model_AttributesArray,g_Fullscreen_Model_numberOfVertices,32,"Square_TopHalf",g_Fullscreen_Model_PrimitiveArray,g_Fullscreen_Model_numberOfPrimitives,-1};
