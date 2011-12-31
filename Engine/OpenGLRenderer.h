//
//  OpenGLRenderer.h
//  InfiniSurv
//
//  Created by Jody McAdams on 11/21/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OpenGLRenderer_h
#define InfiniSurv_OpenGLRenderer_h

class OpenGLRenderer;

extern OpenGLRenderer* GLRENDERER;

//** INCLUDES **/
#ifdef PLATFORM_OSX
#include <OpenGL/gl.h>
#endif

#ifdef PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "MathUtil.h"
#include "GraphicsTypes.h"
#include "PhysicsTypes.h"
#include "MaterialDeclarations.h"
#include "ArrayUtil.h"

#include "PVRTModelPOD.h"

#include "EngineModels.h"

#define RENDERLOOP_ENABLED 1

#define DEBUG_DRAW 1



#define GAUSSIAN_NUMSAMPLES 6
#define MAX_TRAILS 64
#define MAX_PARTICLES 512
#define VERTS_PER_PARTICLE 6
#define VERTS_PER_TRAILPARTICLE 2
#define PARTICLE_TEXTURE_SIZE 256.0f
#define PARTICLES_PER_ROW 8.0f
#define PARTICLE_CELL_SIZE (PARTICLE_TEXTURE_SIZE/PARTICLES_PER_ROW)
#define PARTICLE_TEXCOORD_OFFSET (PARTICLE_CELL_SIZE/PARTICLE_TEXTURE_SIZE)
#define HALF_PARTICLE_TEXCOORD_OFFSET (PARTICLE_TEXCOORD_OFFSET*0.5f)
#define DEBUG_LINES_MAXVERTICES 30
#define NUM_SINCOS_BUCKETS 10
#define NUM_SINCOS_BUCKET_MAX_MULT 0.3f

#define DEBUGDRAW_MAXLINESEGMENTS 512
#define DEBUGDRAW_MAXDEBUGOBJECTS 32

#define MAX_RENDERABLE_3D_OBJECTS 2500
#define MAX_RENDERABLE_UI_OBJECTS 64

#define MAX_ANIMATED_PODS 16
#define MAX_SCENES 64

#define GAUSSIAN_STANDARDDEVIATION 0.9f

#define MAX_COMPILED_VERTEX_SHADERS 32
#define MAX_COMPILED_PIXEL_SHADERS 32

#define MAX_TEXTURED_LINES 16

#define MAX_NUM_TEXTURE_UNITS 5

/**ENUMS**/
enum DebugDrawMode
{
	DebugDrawMode_2D,
	DebugDrawMode_3D,
	DebugDrawMode_Screen2D,
	DebugDrawMode_Num,
};

/**STRUCTS**/
struct SinCosBucket
{
	f32 sinTheta;
	f32 cosTheta;
	f32 multiplier;
};


struct RendererParticleBucket
{
	Particle3D m_particleQueue[MAX_PARTICLES];
	ParticleData m_spriteData[MAX_PARTICLES*VERTS_PER_PARTICLE];
	s32 m_numParticles;
    s32 m_numParticlesToDraw;
	bool m_particlesNeedSorting;
};


struct TexturedLineObject
{
	GLuint texturedID;
	LineSegment line;
	vec4 diffuseColor;
	f32 lineWidth0;
	f32 lineWidth1;
	f32 numTextureRepeats;
};


struct TexturedLineVert
{
	vec3 position;
	vec2 texcoord;
};

struct ShaderCreationStatus
{
	const char* filename;
	GLuint openGLID;
};


enum DebugDrawObjectType
{
    DebugDrawObjectType_CircleXY,
    DebugDrawObjectType_CircleXZ,
	DebugDrawObjectType_Cylinder,
    DebugDrawObjectType_Cube, //unsupported
};


struct DebugDrawObject
{
    DebugDrawObjectType objectType;
    mat4f matrix;
    vec4 color;
};

class OpenGLRenderer
{
public:
	
	//public functions
	
	void Init(f32 screenWidthPixels, f32 screenHeightPixels,f32 screenWidthPoints, f32 screenHeightPoints);
	bool InitSceneFromPOD(RenderableScene3D* pScene, CPVRTModelPOD* pPod, u32 viewFlags, const char* relativePath);
	void CleanUp();
	void LoadParticleAtlas(const char* filename);
	bool GetFadeFinished();
	void ClearRenderables();
	void ClearOneFrameGeometry();
	void RenderLoop(u32 camViewIDX,RenderableGeometry3D** renderableObjectArray, u32 numRenderableObjects);
	void RenderEffects();
	void SetClearColor(f32 r, f32 g, f32 b);
	void SetGravityDir(const vec3* pNewGravityDir);
	void SetSortRenderablesByZ(bool sortRenderablesByZ);
	void Render(f32 timeElapsed);
	void DrawAnimatedPOD(AnimatedPOD* pAnimatedPod);
	void DrawSceneObject(RenderableSceneObject3D* pScene);
	CPVRTModelPOD* LoadPOD(const char* fileName);
	AnimatedPOD* AddAnimatedPOD(CPVRTModelPOD* pPod, RenderableScene3D* pScene, mat4f matrix4x4);
	void RemoveRenderableSceneObject3DFromList(RenderableSceneObject3D* pSceneObject);
	void AddRenderableSceneObject3DToList(RenderableSceneObject3D* pSceneObject);
	bool LoadTextureFromData(GLuint* out_textureName,const void* data,u32 texWidth, u32 texHeight, u32 format, u32 type, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV);
	bool UpdateTextureFromData(GLuint* out_textureName, const void* data, u32 texWidth, u32 texHeight, u32 format, u32 type);
	void RegisterModel(ModelData* pModelData);
	void AddRenderableObject3DToList(RenderableObject3D* pRenderable);
	void RemoveRenderableGeometry3DFromList(RenderableGeometry3D* pGeom);
	void AddParticleToQueue(Particle3D* pParticle, vec3* pPosition, vec3* pCallbackPos, ParticleBuckets particleBucket);
	void SpawnParticles(vec3* pPosition, const vec3* pColor, const ParticleSettings* particleSettings, s32 numParticles);
	void UpdateParticleQueues(f32 timeElapsed);
	void UpdateTrails(f32 timeElapsed);
	void InitRenderableObject3D(RenderableObject3D* renderableObject, ModelData* pModel, RenderMaterial materialID, GLuint* customTexture, mat4f matrix4x4, RenderLayer renderLayer, u32 viewFlags, u32 renderFlags);
	void InitRenderableSceneObject3D(RenderableSceneObject3D* renderableObject, RenderableScene3D* pScene, RenderMaterial materialID, GLuint* customTexture, mat4f matrix4x4, RenderLayer renderLayer, u32 viewFlags, u32 renderFlags);
	void InitRenderableSceneObject3D_Simple(RenderableSceneObject3D* renderableObject, RenderableScene3D* pScene, mat4f matrix4x4, u32 viewFlags);
	void SortRenderableGeometry3DList();
	void CreateMaterials();
	void SetViewMatrixForIndex(f32* pCameraMatrix, u32 viewIndex);
	void SetViewPos(vec3* pCamPos, u32 viewIndex);
	f32* GetCameraMatrix(u32 viewIndex);
	void StartFadeOut(f32 timeInSeconds);
	void ForceFadeIn(f32 timeInSeconds);
	void StartFadeIn();
	void StartPauseFadeIn();
	void StartPauseFadeOut(f32 finalFade, f32 timeInSeconds);
	void ResetPauseFade();
	void DeleteTexture(GLuint* pTextureID);
	void ClearParticles();
	void SetScreenFadeColor(vec3* screenFadeColor);
	void ForceRenderablesNeedSorting();
	void SortRenderablesWithMaterialByZ(RenderMaterial materialID);
	void SortRenderablesInLayerRangeByZ(RenderLayer layerBegin, RenderLayer layerEnd);
	void FlashScreen(const vec3* pColor, f32 timeInSeconds);
	void ShakeScreen(f32 shakeAmount,f32 shakeSpeed, f32 shakeTime);
	void LoadTexture(const char* fileName,ImageType imageType, GLuint* pGLTexture, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV, bool flipY = false);
	GFX_Trail* CreateTrail(GFX_Trail** pCallbackTrail, vec3* pInitialPos, f32 timeToLive, const GFX_TrailSettings* pTrailSettings, u32 renderFlags);
	
	void DRAW_DrawTexturedLine(const vec3* p0, const vec3* p1, const vec4* pDiffuseColor, GLuint texturedID, f32 lineWidth0, f32 lineWidth1, f32 numTextureRepeats);
	void DEBUGDRAW_DrawLineSegment(DebugDrawMode drawMode, const vec3* p0, const vec3* p1, const vec4* color);
	void DEBUGDRAW_DrawLineSegment(DebugDrawMode drawMode, const vec3* p0, const vec3* p1, const vec4* color1, const vec4* color2);
	void DEBUGDRAW_DrawCircleXY(DebugDrawMode drawMode, mat4f matrix4x4, const vec4* color);
	void DEBUGDRAW_DrawCircleXZ(DebugDrawMode drawMode, mat4f matrix4x4, const vec4* color);
	void DEBUGDRAW_DrawCircleXY(DebugDrawMode drawMode, const vec3* pCenter, f32 radius, const vec4* color);
	void DEBUGDRAW_DrawCircleXZ(DebugDrawMode drawMode, const vec3* pCenter, f32 radius, const vec4* color);
	void DEBUGDRAW_DrawCylinder(DebugDrawMode drawMode, mat4f matrix4x4, const vec4* color);
	
	//public member variables
	f32 screenWidth_points;
	f32 screenHeight_points;
	f32 screenWidth_pixels;
	f32 screenHeight_pixels;
	f32 aspectRatio;
	bool paused;
	
private:
	
	//private functions
	void SetMaterial(RenderMaterial material);
	void DeleteScene(RenderableScene3D* pScene);
	void PostProcess(RenderMaterial ppMaterial, RenderTarget* renderTarget, PostProcessDrawArea drawArea, GLuint* texture0, GLuint* texture1, GLuint* texture2);
	void PrintOpenGLError(const char* callerName);
	bool CreateRenderTarget(RenderTarget* renderTarget, GLuint FBO, GLuint width, GLuint height);
	bool CreateShaderProgram(s32 vertexShaderIndex, s32 pixelShaderIndex, AttributeFlags attribs, GLuint* out_resultProgram);
	void SetRenderTarget(RenderTarget* renderTarget);
	void UploadWorldViewProjMatrix(const f32* pWorldMat); //only call after setMaterial
	void UploadProjMatrix(const f32* pProjMat); //only call after setMaterial
	void UploadWorldProjMatrix(const f32* pWorldMat); //only call after setMaterial
	void UploadSharedUniforms();
	void UploadUniqueUniforms(u8* const * pValuePointerArray);
	void SetTexture(const GLuint* pTexture,GLuint textureUnit);
	void SetRenderState(u32 renderFlags);
	f32 ComputeGaussianValue(f32 x, f32 stdDevSq);
	void ComputeGaussianWeights(f32* out_pWeights, s32 numWeights, f32 standardDeviationSquared);
	void EnableAttributes(const ModelData* pModelData);
	void BindVertexArrayObject(const PrimitiveData* pPrimitive);
	void BindIndexData(const PrimitiveData* pPrimitive);
	void AddUniform_Unique(RenderMaterial renderMaterial, const char* nameOfUniformInShader,UniformType uniformType, u32 amount);
	void AddUniform_Shared(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, u8* pData, u32 amount);
	void AddUniform_Shared_Const(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, u8* pData, u32 amount);
	void SortParticleQueues();
	bool CompileShader(GLuint *shader, GLenum type, GLsizei count, const char* filename);
	bool LinkProgram(GLuint prog);
	GLint ValidateProgram(GLuint prog);
	const char* GetPathToFile(const char* filename);
	
	bool CreatePixelShader(s32 pixelShaderIndex);
	bool CreateVertexShader(s32 vertexShaderIndex);
	s32 AddVertexShaderToList(const char* filename);
	s32 AddPixelShaderToList(const char* filename);
	
	//private member variables
	ShaderCreationStatus m_vertexShaders[MAX_COMPILED_VERTEX_SHADERS];
	ShaderCreationStatus m_pixelShaders[MAX_COMPILED_PIXEL_SHADERS];
	s32 m_numVertexShaders;
	s32 m_numPixelShaders;
	GLint backingWidth;
	GLint backingHeight;
	RenderTarget m_screenTarget;
	vec3 m_camPos[NumCameraViews];
	// The OpenGL names for the framebuffer and renderbuffer used to render to this view
	GLuint viewFrameBuffer, colorRenderbuffer, depthRenderbuffer, msaaFramebuffer,msaaRenderBuffer,msaaDepthBuffer;
	GLuint m_postProcessDefaultVertShader;
	f32 m_gaussianWeights[GAUSSIAN_NUMSAMPLES];
	f32 m_accumulatedTime;
	f32* m_view[NumCameraViews];
	mat4f m_projMats[NumProjMatTypes];
	mat4f m_viewProjMats[NumCameraViews][NumProjMatTypes];
	ProjMatType m_currProjMatType;
	vec2 m_gaussianViewportSize;
	bool firstRun;
	bool m_readyToGo;
	f32 m_totalFadeTime;
    f32 m_totalFlashTime;
    f32 m_currFlashTime;
	f32 m_currFadeTime;
	f32 m_currFadeValue;
    f32 m_finalFade;
	FadeState m_fadeState;
    FadeState m_flashState;
    bool m_requestedFadeIn;
	RenderMaterial m_lastUsedMaterial;
	vec4 m_fadeColor;
	vec3 m_fadeToScreenColor;
	u32 m_currViewIndex;
	u32 m_numRenderableGeom3Ds;
	u32 m_numRenderableUIObjects;
	bool m_renderableObject3DsNeedSorting;
	bool m_renderableUINeedSorting;
	u32 m_glBufferCount;
	u32 m_glTotalBufferSize;
	u32 m_glTotalTextureSize;
	u32 m_accumulatedFrames;
	s32 m_numModels;
    u32 m_maxNumRenderables;
    u32 m_maxNumTrails;
    u32 m_maxNumParticles;
    u32 m_numCellshadedRenderables;
	GLuint m_currTexture;
    vec3 m_flashColor;
    GLuint m_currTextureInTextureUnit[MAX_NUM_TEXTURE_UNITS];
    GLuint m_currTextureUnit;
    SinCosBucket m_sinCosBuckets[NUM_SINCOS_BUCKETS];
    RenderableGeometry3D* m_renderableGeometry3DList[MAX_RENDERABLE_3D_OBJECTS];
	RenderableGeometry3D* m_renderableUIList[MAX_RENDERABLE_UI_OBJECTS];
	AnimatedPOD m_animatedPODs[MAX_ANIMATED_PODS];
	u32 m_numAnimatedPods;
	RenderableSceneObject3D* m_scenes[MAX_SCENES];
	u32 m_numScenes;
    RendererParticleBucket m_particleBuckets[NumParticleBuckets];
    GLuint trailShaderUniform_accumulatedTime;
    GLuint trailShaderUniform_scrollAmountU;
    GLuint trailShaderUniform_scrollAmountV;
    GFX_Trail m_trails[MAX_TRAILS];
    PointData m_debugLinePoints[DebugDrawMode_Num][DEBUGDRAW_MAXLINESEGMENTS*2];
    s32 m_numDebugLinePoints[DebugDrawMode_Num];
	s32 m_numDebugLinePoints_saved[DebugDrawMode_Num];
    TexturedLineObject m_texturedLineObjects[MAX_TEXTURED_LINES];
	s32 m_numTexturedLines;
	s32 m_numTexturedLines_saved;
	TexturedLineVert m_texturedLineVerts[4];
    GFX_Trail* m_renderableTrails[MAX_TRAILS];
    u32 m_numRenderableTrails;
    GFX_TrailParticleData m_trailParticleRenderablePoints[MAX_TRAIL_PARTICLES_PER_TRAIL*VERTS_PER_TRAILPARTICLE];
    u32 m_renderStateFlags;
    f32 m_screenShakerT_X;
    f32 m_screenShakerT_Y;
    f32 m_screenShakerSpeed_X;
    f32 m_screenShakerSpeed_Y;
    f32 m_currScreenShakeAmount;
    f32 m_screenShakeTimer;
    f32 m_screenShakeStartTime;
    f32 m_screenShakeSpeed;
    vec3 m_gravityDir;
    bool m_supportsFeaturesFromiOS4;
    bool m_supportsMultisampling;
    FadeState m_pauseFadeState;
	f32 m_pauseTotalFadeTime;
    f32 m_pauseFadeTimeAtStart;
	f32 m_pauseCurrFadeTime;
    f32 m_pauseFinalFade;
    bool m_pauseRequestedFadeIn;
    bool m_sortRenderablesByZ;
    vec3 m_clearColor;
    DebugDrawObject m_debugDrawObjects[DebugDrawMode_Num][DEBUGDRAW_MAXDEBUGOBJECTS];
    u32 m_numDebugDrawObjects[DebugDrawMode_Num];
	u32 m_numDebugDrawObjects_saved[DebugDrawMode_Num];
	char* m_pArtPath;
	f32 m_identityMat[16];
	vec3 m_lightPos;
};


//Helpers
char* FileToCharArray(const char* filename);
bool LoadPNGImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData, bool flipY);
s32 RenderableGeometry3DCompare_SortByZ(const void* lhs, const void* rhs);
s32 RenderableGeometry3DCompare_SortByNegativeZ(const void* lhs, const void* rhs);
s32 RenderableGeometry3DCompare_SortValue(const void* lhs, const void* rhs);
void PackFloat16(float myFloat, float* out_x, float* out_y);
float UnpackFloat16(float x, float y);
void DoubleRenderTarget_Flip(DoubleRenderTarget* doubleRenderTarget);
void Draw_FSQuad();
void Draw_TLQuad();
void Draw_TRQuad();
void Draw_BLQuad();
void Draw_BRQuad();
void Draw_Matrix();

#endif
