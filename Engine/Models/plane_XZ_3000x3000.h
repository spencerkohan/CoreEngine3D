#import "GraphicsTypes.h"

//Model: "plane_XZ_3000x3000"

static const float g_plane_XZ_3000x3000_VertexData0[] = {
	/*v:*/-3000.0f,  0.0f, -3000.0f, /*n:*/0.0f, 1.0f, 0.0f, /*t:*/0.0f,0.0f, 
	/*v:*/-3000.0f,  0.0f, 3000.0f, /*n:*/0.0f, 1.0f, 0.0f, /*t:*/0.0f,512.0f,
	/*v:*/ 3000.0f,  0.0f, -3000.0f, /*n:*/0.0f, 1.0f, 0.0f, /*t:*/512.0f,0.0f,  
	/*v:*/ 3000.0f,  0.0f, 3000.0f, /*n:*/0.0f, 1.0f, 0.0f, /*t:*/512.0f,512.0f,
};

#define g_plane_XZ_3000x3000_numberOfVertices	4
#define  g_plane_XZ_3000x3000_numberOfPrimitives 1

static PrimitiveData  g_plane_XZ_3000x3000_PrimitiveArray[g_plane_XZ_3000x3000_numberOfPrimitives]={
	{GL_TRIANGLE_STRIP,(f32*)g_plane_XZ_3000x3000_VertexData0,NULL,g_plane_XZ_3000x3000_numberOfVertices,sizeof(g_plane_XZ_3000x3000_VertexData0),0,0,0},
};

ModelData g_plane_XZ_3000x3000_modelData = {VertexFormat_VNT,"plane_XZ_3000x3000",g_plane_XZ_3000x3000_PrimitiveArray,g_plane_XZ_3000x3000_numberOfPrimitives,-1};
