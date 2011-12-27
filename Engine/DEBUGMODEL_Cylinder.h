#import "GraphicsTypes.h"

//Model: "DEBUGMODEL_Cylinder"

//Circle Bottom
static const float g_DEBUGMODEL_Cylinder_VertexData0[] = {
	0.500000f,-0.5f,-0.000000f,
    0.311745f,-0.5f,-0.390916f,
    -0.111261f,-0.5f,-0.487464f,
    -0.450484f,-0.5f,-0.216942f,
    -0.450484f,-0.5f,0.216942f,
    -0.111260f,-0.5f,0.487464f,
    0.311745f,-0.5f,0.390916f,
};

//Circle Top
static const float g_DEBUGMODEL_Cylinder_VertexData1[] = {
	0.500000f,0.5f,-0.000000f,
    0.311745f,0.5f,-0.390916f,
    -0.111261f,0.5f,-0.487464f,
    -0.450484f,0.5f,-0.216942f,
    -0.450484f,0.5f,0.216942f,
    -0.111260f,0.5f,0.487464f,
    0.311745f,0.5f,0.390916f,
};


//Sides
static const float g_DEBUGMODEL_Cylinder_VertexData2[] = {
	0.500000f,-0.5f,-0.000000f,
    0.500000f,0.5f,-0.000000f,
    0.311745f,-0.5f,-0.390916f,
    0.311745f,0.5f,-0.390916f,
    -0.111261f,-0.5f,-0.487464f,
    -0.111261f,0.5f,-0.487464f,
    -0.450484f,-0.5f,-0.216942f,
    -0.450484f,0.5f,-0.216942f,
    -0.450484f,-0.5f,0.216942f,
    -0.450484f,0.5f,0.216942f,
    -0.111260f,-0.5f,0.487464f,
    -0.111260f,0.5f,0.487464f,
    0.311745f,-0.5f,0.390916f,
    0.311745f,0.5f,0.390916f,
};

static const u32 g_DEBUGMODEL_Cylinder_numberOfVertices_Circle = 7;
static const u32 g_DEBUGMODEL_Cylinder_numberOfVertices_Sides =	14;
static const u32 g_DEBUGMODEL_Cylinder_numberOfPrimitives = 3;
static const u32 g_DEBUGMODEL_Cylinder_numberOfAttributes = 1;

static PrimitiveData  g_DEBUGMODEL_Cylinder_PrimitiveArray[g_DEBUGMODEL_Cylinder_numberOfPrimitives]={
	{GL_LINE_LOOP,(u8*)g_DEBUGMODEL_Cylinder_VertexData0,NULL,g_DEBUGMODEL_Cylinder_numberOfVertices_Circle,sizeof(g_DEBUGMODEL_Cylinder_VertexData0),0,0,0},
    {GL_LINE_LOOP,(u8*)g_DEBUGMODEL_Cylinder_VertexData1,NULL,g_DEBUGMODEL_Cylinder_numberOfVertices_Circle,sizeof(g_DEBUGMODEL_Cylinder_VertexData1),0,0,0},
    {GL_LINES,(u8*)g_DEBUGMODEL_Cylinder_VertexData2,NULL,g_DEBUGMODEL_Cylinder_numberOfVertices_Sides,sizeof(g_DEBUGMODEL_Cylinder_VertexData2),0,0,0},
};

static AttributeData g_DEBUGMODEL_Cylinder_AttributesArray[g_DEBUGMODEL_Cylinder_numberOfAttributes]=
{
	{ATTRIB_VERTEX,GL_FLOAT,3,0},
};


static ModelData g_DEBUGMODEL_Cylinder_modelData = {0,g_DEBUGMODEL_Cylinder_AttributesArray,g_DEBUGMODEL_Cylinder_numberOfAttributes,12,"DEBUGMODEL_Cylinder",g_DEBUGMODEL_Cylinder_PrimitiveArray,g_DEBUGMODEL_Cylinder_numberOfPrimitives,-1};
