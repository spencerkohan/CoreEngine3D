#import "GraphicsTypes.h"

//Model: "DEBUGMODEL_Circle"

static const float g_DEBUGMODEL_Circle_VertexData0[] = {
	0.500000f,0.000000f,-0.000000f,0.311745f,0.000000f,-0.390916f,-0.111261f,0.000000f,-0.487464f,-0.450484f,0.000000f,-0.216942f,-0.450484f,0.000000f,0.216942f,-0.111260f,0.000000f,0.487464f,0.311745f,0.000000f,0.390916f,
};

#define g_DEBUGMODEL_Circle_numberOfVertices	7
#define g_DEBUGMODEL_Circle_numberOfPrimitives 1

static PrimitiveData  g_DEBUGMODEL_Circle_PrimitiveArray[g_DEBUGMODEL_Circle_numberOfPrimitives]={
	{GL_LINE_LOOP,(f32*)g_DEBUGMODEL_Circle_VertexData0,NULL,g_DEBUGMODEL_Circle_numberOfVertices,sizeof(g_DEBUGMODEL_Circle_VertexData0),0,0,0},
};

static ModelData g_DEBUGMODEL_Circle_modelData = {VertexFormat_V,"DEBUGMODEL_Circle",g_DEBUGMODEL_Circle_PrimitiveArray,g_DEBUGMODEL_Circle_numberOfPrimitives,-1};
