//
//  OpenGLRenderer.mm (must be .mm because it can call NS functions)
//  InfiniSurv
//
//  Created by Jody McAdams on 11/21/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#if defined PLATFORM_OSX || defined PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

#include "OpenGLRenderer.h"
#include "matrix.h"
#include <iostream>
#include <cmath>
#include "GameUtil.h"
#include "ArrayUtil.h"
#include "GameDefines.h"
#include "zlib.h"
#include "png.h"
#include "SOIL.h"

#if defined PLATFORM_OSX 
#include <OpenGL/glu.h>
#endif

#include "MaterialDeclarations.h"

//DEBUG MODELS
#include "DEBUGMODEL_Circle.h"
#include "DEBUGMODEL_Cylinder.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//Circle verts: 2D
static const GLfloat g_circleVAR[] = {
	0.0000f,  1.0000f,
	0.2588f,  0.9659f,
	0.5000f,  0.8660f,
	0.7071f,  0.7071f,
	0.8660f,  0.5000f,
	0.9659f,  0.2588f,
	1.0000f,  0.0000f,
	0.9659f, -0.2588f,
	0.8660f, -0.5000f,
	0.7071f, -0.7071f,
	0.5000f, -0.8660f,
	0.2588f, -0.9659f,
	0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	0.0000f,  1.0000f,
	0.0f, 0.0f, // For an extra line to see the rotation.
};
static const int g_circleVAR_count = sizeof(g_circleVAR)/sizeof(GLfloat)/2;

//Debug verts
int g_numDebugVertices = 0;
GLfloat g_DebugVertices[DEBUG_LINES_MAXVERTICES*3];

//Verts to draw a 3D matrix
const GLfloat matrixVertices[] = {
	0.0f, 0.0f,0.0f,
	10.0f, 0.0f,0.0f,
	0.0f, 0.0f,0.0f,
	0.0f, 10.0f,0.0f,
	0.0f, 0.0f,0.0f,
	0.0f, 0.0f,10.0f,
};

//Colors for drawing the 3D matrix
const GLfloat matrixColors[] = {
	1.0f, 0.0f,0.0f,
	1.0f, 0.0f,0.0f,
	0.0f, 1.0f,0.0f,
	0.0f, 1.0f,0.0f,
	0.0f, 0.0f,1.0f,
	0.0f, 0.0f,1.0f,
};

//Verts for drawing a 2D square
const GLfloat squareVertices[] = {
	-1.0f, -1.0f,
	1.0f,  -1.0f,
	-1.0f,  1.0f,
	1.0f,   1.0f,
};

//Verts for drawing a 2D square
const GLfloat squareVerticesTR[] = {
	0.0f, 0.0f,
	1.0f,  0.0f,
	0.0f,  1.0f,
	1.0f,   1.0f,
};

//Verts for drawing a 2D square
const GLfloat squareVerticesTL[] = {
	-1.0f, 1.0f,
	0.0f,  1.0f,
	-1.0f,  0.0f,
	0.0f,   0.0f,
};

//Verts for drawing a 2D square
const GLfloat squareVerticesBL[] = {
	-1.0f, -1.0f,
	0.0f,  -1.0f,
	-1.0f,  0.0f,
	0.0f,   0.0f,
};

//Verts for drawing a 2D square
const GLfloat squareVerticesBR[] = {
	1.0f, -1.0f,
	0.0f,  -1.0f,
	1.0f,  0.0f,
	0.0f,   0.0f,
};

//Texcoords for the 2D square
const GLfloat squareTexCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};

static Material g_Materials[NumRenderMaterials];
static GLuint texture_pointSpriteAtlas = 0;
static GLuint texture_default = 0;

void OpenGLRenderer::Init(f32 screenWidth, f32 screenHeight)
{

#ifdef PLATFORM_IOS
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	GLuint colorRenderbuffer;
	glGenRenderbuffers(1, &colorRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, screenWidth, screenHeight);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	
	GLuint depthRenderbuffer;
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
	if(status != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"failed to make complete framebuffer object %x", status);
	}
#else
	
#endif
	
	
	
	PrintOpenGLError("At init!");
	
	mat4f_LoadIdentity(m_identityMat);
	
	screenWidth_points = screenWidth;
	screenHeight_points = screenHeight;
	
	aspectRatio = screenWidth/screenHeight;
	
	mat4f_LoadPerspective(m_projMats[ProjMatType_Perspective], 3.14f*0.25f,aspectRatio,1.0f,3000.0f);

	for(int i=0; i<NumCameraViews; ++i)
	{
		m_view[i] = m_identityMat;
	}
	
	//Set up projection matrices
	mat4f_LoadOrtho(m_projMats[ProjMatType_Orthographic_NDC], -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f);
	mat4f_LoadOrtho(m_projMats[ProjMatType_Orthographic_Points], 0.0f, screenWidth_points, screenHeight_points, 0.0f, 1.0f, -1.0f);
	
	printf("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

	/*** INITIALIZE INITIALIZE INITIALIZE ***/
    
	m_numVertexShaders = 0;
	m_numPixelShaders = 0;
	
	SetVec3(&m_fadeToScreenColor,1.0f,1.0f,1.0f);
	
    m_sortRenderablesByZ = false;
	
	m_numTexturedLines = 0;

#ifdef PLATFORM_OSX
	m_supportsFeaturesFromiOS4 = true;
#endif
	
#ifdef PLATFORM_IOS
	m_supportsFeaturesFromiOS4 = [[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f;
#endif
	
#if TARGET_IPHONE_SIMULATOR
    m_supportsMultisampling = NO;
#else
    //const s32 processorCount = (s32)[[NSProcessInfo processInfo] processorCount];
    //printf("This device has %d processors.\n",processorCount);
    
	const s32 processorCount = 1;
    m_supportsMultisampling = m_supportsFeaturesFromiOS4 && processorCount > 1; //Hack to enable multi-sampling only on iPad 2
#endif
    
    CopyVec3(&m_clearColor, &vec3_zero);
    
    SetVec3(&m_gravityDir,0.0f,-1.0f,0.0f);
    
    for (int i=0; i<5; ++i)
    {
        m_currTextureInTextureUnit[i] = 0;
    }
    
    for(int i=0; i<MAX_TRAILS; ++i)
    {
        m_trails[i].m_numTrailParticles = 0;
        m_trails[i].inUse = 0;
        m_trails[i].pCallbackTrailPointer = 0;
    }
    
    m_screenShakeTimer = -1.0f;
    m_currScreenShakeAmount = 10.0f;
    
    m_maxNumRenderables = 0;
    m_maxNumTrails = 0;
	
    m_numDebugLinePoints2D = 0;
    m_numDebugLinePoints3D = 0;
    
    m_renderStateFlags = 0;
    
    m_currTextureUnit = 999;
    
    m_requestedFadeIn = false;
	m_glBufferCount = 0;
	m_glTotalBufferSize = 0;
	m_glTotalTextureSize = 0;
    
	m_numModels = 0;
	
    m_screenShakerT_X = 0.0f;
    m_screenShakerT_Y = 0.0f;
    m_screenShakerSpeed_X = 50.0f;
    m_screenShakerSpeed_Y = 100.0f;
	
    paused = false;
	
	m_renderableObject3DsNeedSorting = false;
	
	ClearParticles();
	
	for (s32 i=0; i<NUM_SINCOS_BUCKETS; ++i)
	{
		m_sinCosBuckets[i].multiplier = ((f32)(i+1)/(f32)NUM_SINCOS_BUCKETS)*NUM_SINCOS_BUCKET_MAX_MULT;
	}
	
	m_fadeState = FadeState_Idle;
    m_flashState = FadeState_Idle;
    m_pauseFadeState = FadeState_Idle;
    
	m_totalFadeTime = 0.0f;
	m_currFadeTime = 0.0f;
	m_currFadeValue = 1.0f;
	
	m_numRenderableGeom3Ds = 0;
	m_numRenderableUIObjects = 0;
	
	m_readyToGo = false;
	
	m_lastUsedMaterial = (RenderMaterial) RENDERMATERIAL_INVALID;
    
	PrintOpenGLError("Before loading textures");
	
	// Load all the textures
	LoadTexture("TempTexture.tga", ImageType_TGA, &texture_default, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	LoadTexture("ParticleAtlas_01.png", ImageType_PNG, &texture_pointSpriteAtlas, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	
	//Create VBO buffers for terrain
	//CreateBuffers();
	
	PrintOpenGLError("Before creating materials");
	
	CreateMaterials();
	
	firstRun = true;
	
	m_accumulatedTime = 0.0f;
	m_accumulatedFrames = 0;
	ComputeGaussianWeights(m_gaussianWeights,GAUSSIAN_NUMSAMPLES,GAUSSIAN_STANDARDDEVIATION);
	
	m_currViewIndex = CameraView0;
	
	PrintOpenGLError("Before registering models");
	
    RegisterModel(&g_DEBUGMODEL_Circle_modelData);
    RegisterModel(&g_DEBUGMODEL_Cylinder_modelData);
    
	PrintOpenGLError("Initialized Renderer");
	
	//Reset debug lines
	m_numDebugLinePoints2D_saved = 0;
	m_numDebugLinePoints3D_saved = 0;
	m_numDebugDrawObjects_saved = 0;
	m_numTexturedLines_saved = 0;
}


bool OpenGLRenderer::GetFadeFinished()
{
	return m_fadeState == FadeState_WaitingForFadeIn;
}


void OpenGLRenderer::ClearRenderables()
{
	for(u32 i=0; i<m_numRenderableGeom3Ds; ++i)
    {
        RenderableGeometry3D* pCurrObject = m_renderableGeometry3DList[i];
        if(pCurrObject == NULL)
        {
            continue;
        }
        
        pCurrObject->flags = 0;
    }
	
	for(u32 i=0; i<m_numRenderableUIObjects; ++i)
    {
        RenderableGeometry3D* pCurrObject = m_renderableUIList[i];
        if(pCurrObject == NULL)
        {
            continue;
        }
        
        pCurrObject->flags = 0;
    }
    
    m_numRenderableGeom3Ds = 0;
	m_numRenderableUIObjects = 0;
}


void OpenGLRenderer::ClearOneFrameGeometry()
{
	m_numDebugLinePoints2D_saved = 0;
	m_numDebugLinePoints3D_saved = 0;
	m_numDebugDrawObjects_saved = 0;
	m_numTexturedLines_saved = 0;
	
	m_numDebugLinePoints2D = 0;
	m_numDebugLinePoints3D = 0;
	m_numDebugDrawObjects = 0;
	m_numTexturedLines = 0;
}


void OpenGLRenderer::RenderLoop(u32 camViewIDX,RenderableGeometry3D** renderableObjectArray, u32 numRenderableObjects)
{
	u8* vertexData = NULL;
	
#if RENDERLOOP_MODE
	for (u32 renderIDX = 0; renderIDX<numRenderableObjects; ++renderIDX)
	{
		const RenderableObject3D* pCurrRenderableObject3D = renderableObjectArray[renderIDX];
		
		//Don't mess with or draw stuff not in this view
		if (!(pCurrRenderableObject3D->viewFlags & (1<<camViewIDX)))
		{
			continue;
		}
		
		const u32 renderFlags = pCurrRenderableObject3D->flags;
		
		//Don't mess with or draw invisible stuff
		if (!(renderFlags & RenderFlag_Visible))
		{
			continue;
		}
		
		if(renderFlags & RenderFlag_UseOrthographicProjection_NDC)
		{
			m_currProjMatType = ProjMatType_Orthographic_NDC;
		}
		else if(renderFlags & RenderFlag_UseOrthographicProjection_Points)
		{
			m_currProjMatType = ProjMatType_Orthographic_Points;
		}
		else
		{
			m_currProjMatType = ProjMatType_Perspective;
		}
		
		//Set material whenever it has changed
		
		RenderMaterial nextMaterial = pCurrRenderableObject3D->materialID;
		if(nextMaterial != m_lastUsedMaterial)
		{
			SetMaterial(nextMaterial);
		}
		
		//At this point, either the texture was set by a material or its still set to
		//the last override texture
		
		//The material groups are sub-sorted by override texture, so all
		//the non-overridden things draw first.  Therefore, there is
		//never any need to reset back to the original texture from the
		//material.
		
		//Override material texture0 if needed
		//Will do nothing if the texture is set to 0 or the texture is already set
		SetTexture(pCurrRenderableObject3D->customTexture0, 0);
		SetTexture(pCurrRenderableObject3D->customTexture1, 1);
		
		//Upload uniforms that have unique values per object
		UploadUniqueUniforms(pCurrRenderableObject3D->uniqueUniformValues);
		
		//Draw the current object
		
		if (renderFlags & RenderFlag_IgnoreViewMatrix)
		{
			UploadWorldProjMatrix(pCurrRenderableObject3D->worldMat);
		}
		else
		{
			UploadWorldViewProjMatrix(pCurrRenderableObject3D->worldMat);
		}
		
		//Enable or disable depth reading based on the RenderFlag
		
		//Enable or disable depth writing based on the RenderFlag
		const u32 currDepthTestValue = renderFlags & RenderFlag_DisableDepthTest;
		const u32 prevDepthTestValue = m_renderStateFlags & RenderFlag_DisableDepthTest;
		
		if(currDepthTestValue != prevDepthTestValue)
		{
			if (renderFlags & RenderFlag_DisableDepthTest)
			{
				glDisable( GL_DEPTH_TEST );
			}
			else
			{
				glEnable( GL_DEPTH_TEST );
			}
		}
		
		//These other values rely on the depth test being on so if it's not on,
		//don't do any of this
		if(!(renderFlags & RenderFlag_DisableDepthTest))
		{
			const u32 currDepthReadValue = renderFlags & RenderFlag_DisableDepthRead;
			const u32 prevDepthReadValue = m_renderStateFlags & RenderFlag_DisableDepthRead;
			
			if(currDepthReadValue != prevDepthReadValue)
			{
				if (renderFlags & RenderFlag_DisableDepthRead)
				{
					glDepthFunc(GL_ALWAYS);
				}
				else
				{
					glDepthFunc(GL_LEQUAL) ;
				}
			}
			
			
			
			const u32 currDepthWriteValue = renderFlags & RenderFlag_DisableDepthWrite;
			const u32 prevDepthWriteValue = m_renderStateFlags & RenderFlag_DisableDepthWrite;
			
			if(currDepthWriteValue != prevDepthWriteValue)
			{
				if (renderFlags & RenderFlag_DisableDepthWrite)
				{
					glDepthMask( GL_FALSE );
				}
				else
				{
					glDepthMask( GL_TRUE );
				}
			}
		}
		
		
		const u32 currAlphaBlendValue = (renderFlags & RenderFlag_AlphaBlended) | (renderFlags & RenderFlag_AdditiveBlending) | (renderFlags & RenderFlag_NonPremultipliedAlpha) | (renderFlags & RenderFlag_DisableBlending);
		const u32 prevAlphaBlendValue = (m_renderStateFlags & RenderFlag_AlphaBlended) | (m_renderStateFlags & RenderFlag_AdditiveBlending) | (m_renderStateFlags & RenderFlag_NonPremultipliedAlpha) | (m_renderStateFlags & RenderFlag_DisableBlending);
		
		if(currAlphaBlendValue != prevAlphaBlendValue)
		{
			//Enable or disable alpha blended based on the RenderFlag
			if (renderFlags & RenderFlag_AlphaBlended)
			{
				glEnable(GL_BLEND);
				if(renderFlags & RenderFlag_AdditiveBlending)
				{
					//Additive blending
					glBlendFunc (GL_SRC_ALPHA, GL_ONE);
				}
				else
				{
					if(renderFlags & RenderFlag_NonPremultipliedAlpha)
					{
						//Normal blending
						glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}
					else
					{
						//Normal blending
						glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
					}
				}
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}
		
		
		const u32 currCullValue = (renderFlags & RenderFlag_DisableCulling) | (renderFlags & RenderFlag_InverseCulling);
		const u32 prevCullValue = (m_renderStateFlags & RenderFlag_DisableCulling) | (m_renderStateFlags & RenderFlag_InverseCulling);
		
		if(currCullValue != prevCullValue)
		{
			//Turn on or off culling based on the RenderFlags
			if(renderFlags & RenderFlag_DisableCulling)
			{
				glDisable(GL_CULL_FACE);
			}
			else
			{
				glEnable(GL_CULL_FACE);
				
				if (renderFlags & RenderFlag_InverseCulling)
				{
					//Inverse culling
					glFrontFace(GL_CW);
				}
				else
				{
					//Normal culling
					glFrontFace(GL_CCW);
				}
				
			}
		}
		
		//Save the current flags so we don't repeat renderstate calls
		m_renderStateFlags = renderFlags;
		
		const ModelData* pModelData = renderableObjectArray[renderIDX]->pModel;
		
		for(unsigned int primIDX=0; primIDX<pModelData->numPrimitives; ++primIDX)
		{
			
			const PrimitiveData* currPrim = &pModelData->primitiveArray[primIDX];
			if (currPrim->vertexData != vertexData)
			{
				vertexData = currPrim->vertexData;
				if (m_supportsFeaturesFromiOS4)
				{
					BindVertexArrayObject(currPrim);
				}
				else
				{
					glBindBuffer(GL_ARRAY_BUFFER, currPrim->vertexArrayObjectID);
					EnableAttributes(pModelData->format);
				}
			}
			
			//VAOs don't save this I guess?
			BindIndexData(currPrim);
			
			//Draw the primitive!
			if(currPrim->vertexArrayObjectID)
			{
				//NULL means non-indexed triangles
				if (currPrim->indexData == NULL)
				{
					glDrawArrays(currPrim->drawMethod, 0, currPrim->numVerts);
				}
				//Render using indices
				else
				{
					glDrawElements(currPrim->drawMethod, currPrim->numVerts, GL_UNSIGNED_SHORT, 0);
				}
			}
			/*else
			 {
			 printf("INSANE ERROR: You're trying to draw model '%s' but it hasn't been registered! WTF!?\n",pModelData->modelName);
			 }*/
		}
	}
#else
	u32 renderIndex = 0;
	
	while(renderIndex < numRenderableObjects)
	{				
		const ModelData* pModelData = renderableObjectArray[renderIndex]->pModel;
		
		u32 renderIndexStart=renderIndex;
		//printf("\n *** STARTING MODEL RUN ***\n");
		//printf("Start Index %d Model Name: %s\n",renderIndexStart,pModelData->modelName);
		
		for (; renderIndex < numRenderableObjects && renderableObjectArray[renderIndex]->pModel == pModelData; ++renderIndex);
		
		//unsigned int renderIndexEnd = renderIndex-1;
		
		//printf("End Index %d Model Name: %s\n",renderIndex,m_renderableObject3DList[renderIndexEnd]->pModel->modelName);
		//printf("\n *** ENDING MODEL RUN ***\n\n");
		
		//Now we can loop through the things that were using the same model
		//FIRST: Loop through all primitives of the model, binding them as we go
		for(unsigned int primIDX=0; primIDX<pModelData->numPrimitives; ++primIDX)
		{
			const PrimitiveData* currPrim = &pModelData->primitiveArray[primIDX];
			if (currPrim->vertexData != vertexData)
			{
				vertexData = currPrim->vertexData;
				
				if (m_supportsFeaturesFromiOS4)
				{
					BindVertexArrayObject(currPrim);
				}
				else
				{
					glBindBuffer(GL_ARRAY_BUFFER, currPrim->vertexArrayObjectID);
					EnableAttributes(pModelData);
				}
			}
			
			//VAOs don't save this I guess?
			BindIndexData(currPrim);
			
			//SECOND: for each bound, render all the stuff
			for (unsigned int jRenderIDX=renderIndexStart; jRenderIDX<renderIndex; ++jRenderIDX)
			{
				const RenderableGeometry3D* pCurrRenderable = renderableObjectArray[jRenderIDX];
				
				const u32 renderFlags = pCurrRenderable->flags;
				
				//Don't mess with or draw stuff not in this view
				if (!(pCurrRenderable->viewFlags & (1<<camViewIDX)))
				{
					continue;
				}
				
				//Don't mess with or draw invisible stuff
				if (!(renderFlags & RenderFlag_Visible))
				{
					continue;
				}
				
				if(renderFlags & RenderFlag_UseOrthographicProjection_NDC)
				{
					m_currProjMatType = ProjMatType_Orthographic_NDC;
				}
				else if(renderFlags & RenderFlag_UseOrthographicProjection_Points)
				{
					m_currProjMatType = ProjMatType_Orthographic_Points;
				}
				else
				{
					m_currProjMatType = ProjMatType_Perspective;
				}
				
				//Enable or disable depth reading based on the RenderFlag
				
				//Enable or disable depth writing based on the RenderFlag
				const u32 currDepthTestValue = renderFlags & RenderFlag_DisableDepthTest;
				const u32 prevDepthTestValue = m_renderStateFlags & RenderFlag_DisableDepthTest;
				
				if(currDepthTestValue != prevDepthTestValue)
				{
					if (renderFlags & RenderFlag_DisableDepthTest)
					{
						glDisable( GL_DEPTH_TEST );
					}
					else
					{
						glEnable( GL_DEPTH_TEST );
					}
				}
				
				//These other values rely on the depth test being on so if it's not on,
				//don't do any of this
				if(!(renderFlags & RenderFlag_DisableDepthTest))
				{
					const u32 currDepthReadValue = renderFlags & RenderFlag_DisableDepthRead;
					const u32 prevDepthReadValue = m_renderStateFlags & RenderFlag_DisableDepthRead;
					
					if(currDepthReadValue != prevDepthReadValue)
					{
						if (renderFlags & RenderFlag_DisableDepthRead)
						{
							glDepthFunc(GL_ALWAYS);
						}
						else
						{
							glDepthFunc(GL_LEQUAL) ;
						}
					}
					
					
					
					const u32 currDepthWriteValue = renderFlags & RenderFlag_DisableDepthWrite;
					const u32 prevDepthWriteValue = m_renderStateFlags & RenderFlag_DisableDepthWrite;
					
					if(currDepthWriteValue != prevDepthWriteValue)
					{
						if (renderFlags & RenderFlag_DisableDepthWrite)
						{
							glDepthMask( GL_FALSE );
						}
						else
						{
							glDepthMask( GL_TRUE );
						}
					}
				}
				
				
				const u32 currAlphaBlendValue = (renderFlags & RenderFlag_AlphaBlended) | (renderFlags & RenderFlag_AdditiveBlending) | (renderFlags & RenderFlag_NonPremultipliedAlpha) | (renderFlags & RenderFlag_DisableBlending);
				const u32 prevAlphaBlendValue = (m_renderStateFlags & RenderFlag_AlphaBlended) | (m_renderStateFlags & RenderFlag_AdditiveBlending) | (m_renderStateFlags & RenderFlag_NonPremultipliedAlpha) | (m_renderStateFlags & RenderFlag_DisableBlending);
				
				if(currAlphaBlendValue != prevAlphaBlendValue)
				{
					//Enable or disable alpha blended based on the RenderFlag
					if (renderFlags & RenderFlag_AlphaBlended)
					{
						glEnable(GL_BLEND);
						if(renderFlags & RenderFlag_AdditiveBlending)
						{
							//Additive blending
							glBlendFunc (GL_SRC_ALPHA, GL_ONE);
						}
						else
						{
							if(renderFlags & RenderFlag_NonPremultipliedAlpha)
							{
								//Normal blending
								glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							}
							else
							{
								//Normal blending
								glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
							}
						}
					}
					else
					{
						glDisable(GL_BLEND);
					}
				}
				
				
				const u32 currCullValue = (renderFlags & RenderFlag_DisableCulling) | (renderFlags & RenderFlag_InverseCulling);
				const u32 prevCullValue = (m_renderStateFlags & RenderFlag_DisableCulling) | (m_renderStateFlags & RenderFlag_InverseCulling);
				
				if(currCullValue != prevCullValue)
				{
					//Turn on or off culling based on the RenderFlags
					if(renderFlags & RenderFlag_DisableCulling)
					{
						glDisable(GL_CULL_FACE);
					}
					else
					{
						glEnable(GL_CULL_FACE);
						
						if (renderFlags & RenderFlag_InverseCulling)
						{
							//Inverse culling
							glFrontFace(GL_CW);
						}
						else
						{
							//Normal culling
							glFrontFace(GL_CCW);
						}
						
					}
				}
				
				//Set material whenever it has changed
				
				RenderMaterial nextMaterial = pCurrRenderable->materialID;
				if(nextMaterial != m_lastUsedMaterial)
				{
					SetMaterial(nextMaterial);
				}
				
				//At this point, either the texture was set by a material or its still set to
				//the last override texture
				
				//The material groups are sub-sorted by override texture, so all
				//the non-overridden things draw first.  Therefore, there is
				//never any need to reset back to the original texture from the
				//material.
				
				//Override material texture0 if needed
				//Will do nothing if the texture is set to 0 or the texture is already set
				SetTexture(pCurrRenderable->customTexture0, 0);
				//SetTexture(pCurrRenderableObject3D->customTexture1, 1);

				//Upload uniforms that have unique values per object
				UploadUniqueUniforms(pCurrRenderable->uniqueUniformValues);
				
				//Draw the current object
				
				if (renderFlags & RenderFlag_IgnoreViewMatrix)
				{
					UploadWorldProjMatrix(pCurrRenderable->pWorldMat);
				}
				else
				{
					UploadWorldViewProjMatrix(pCurrRenderable->pWorldMat);
				}
				
				//Draw the primitive!
				if(currPrim->vertexArrayObjectID)
				{
					//NULL means non-indexed triangles
					if (currPrim->indexData == NULL)
					{
						glDrawArrays(currPrim->drawMethod, 0, currPrim->numVerts);
					}
					//Render using indices
					else
					{
						//PrintOpenGLError("/*** Tried to render something ***/");
						glDrawElements(currPrim->drawMethod, currPrim->numVerts, GL_UNSIGNED_SHORT, 0);
					}
				}
				/*else
				 {
				 printf("INSANE ERROR: You're trying to draw model '%s' but it hasn't been registered! WTF!?\n",pModelData->modelName);
				 }*/
				
				//Save the current flags so we don't repeat renderstate calls
				m_renderStateFlags = renderFlags;
			}
		}	
	}
#endif
}


void OpenGLRenderer::RenderEffects()
{
#ifdef DEBUG_RENDERLOOP
	//printf("Skipped swaps: %d\n",skippedTextureSwaps);
	
	//printf("**Render End.  Rendered %d objects.\n",m_numRenderableObject3Ds);
#endif
	
    //Do this to disable vertex array objects
    if(m_supportsFeaturesFromiOS4)
    {
#ifdef PLATFORM_IOS
		glBindVertexArrayOES(0);
#else
		glBindVertexArrayAPPLE(0);	
#endif
    }
	
    
	//Do this to use normal non-VBO memory before starting post processing
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);
	
    glDepthFunc(GL_LEQUAL) ;
	
	//Disable depth write
	glDepthMask( GL_FALSE );
    glDepthFunc(GL_LEQUAL) ;
	
	//Disable culling
	glDisable(GL_CULL_FACE);
	
	//Enable alpha blend
	glEnable(GL_BLEND);
	
	/*** RENDER TEXTURED LINES ***/
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Set line material
    SetMaterial(MT_TextureAndDiffuseColor);
    m_currProjMatType = ProjMatType_Perspective;
    UploadWorldViewProjMatrix(m_identityMat);
	
	GLuint currLineTexture = NULL;
	
	for(s32 renderLineIDX=0; renderLineIDX<m_numTexturedLines_saved; ++renderLineIDX)
	{
		TexturedLineObject* pCurrLine = &m_texturedLineObjects[renderLineIDX];
		GLuint nextLineTexture = pCurrLine->texturedID;
		
		if(nextLineTexture != currLineTexture)
		{
			SetTexture(&nextLineTexture,0);
			
			currLineTexture = nextLineTexture;
		}
		
		vec3 lineVec;
		SubVec3(&lineVec, &pCurrLine->line.p0, &pCurrLine->line.p1);
		
		vec3 camAt;
		mat4f_GetViewAt(&camAt, m_view[0]);
		
		vec3 sideVec;
		CrossVec3(&sideVec, &lineVec, &camAt);
		
		TryNormalizeVec3_Self(&sideVec);
		
		vec3 sideVec0;
		vec3 sideVec1;
		
		ScaleVec3(&sideVec0, &sideVec, pCurrLine->lineWidth0*0.5f);
		ScaleVec3(&sideVec1, &sideVec, pCurrLine->lineWidth1*0.5f);
		
		const f32 numRepeats = pCurrLine->numTextureRepeats;
		
		m_texturedLineVerts[0].texcoord.x = 0.0f;
		m_texturedLineVerts[0].texcoord.y = 0.0f;
		CopyVec3(&m_texturedLineVerts[0].position,&pCurrLine->line.p0);
		AddVec3_Self(&m_texturedLineVerts[0].position, &sideVec0);
		
		m_texturedLineVerts[1].texcoord.x = 1.0f;
		m_texturedLineVerts[1].texcoord.y = 0.0f;
		CopyVec3(&m_texturedLineVerts[1].position,&pCurrLine->line.p0);
		AddScaledVec3_Self(&m_texturedLineVerts[1].position, &sideVec0,-1.0f);
		
		m_texturedLineVerts[2].texcoord.x = 0.0f;
		m_texturedLineVerts[2].texcoord.y = numRepeats;
		CopyVec3(&m_texturedLineVerts[2].position,&pCurrLine->line.p1);
		AddVec3_Self(&m_texturedLineVerts[2].position, &sideVec1);
		
		m_texturedLineVerts[3].texcoord.x = 1.0f;
		m_texturedLineVerts[3].texcoord.y = numRepeats;
		CopyVec3(&m_texturedLineVerts[3].position,&pCurrLine->line.p1);
		AddScaledVec3_Self(&m_texturedLineVerts[3].position, &sideVec1,-1.0f);
		
		//Set diffuse color
		vec4 color = { pCurrLine->diffuseColor.x, pCurrLine->diffuseColor.y, pCurrLine->diffuseColor.z, 1.0f };
		glUniform4fv(g_Materials[MT_TextureAndDiffuseColor].uniforms_unique[0],1,(f32*)&color);
		
		//Draw verts
		//Draw particles
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(TexturedLineVert), &m_texturedLineVerts[0].position);
		
		glEnableVertexAttribArray(ATTRIB_TEXCOORD);
		glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(TexturedLineVert), &m_texturedLineVerts[0].texcoord);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	
    /*** RENDER TRAILS ***/
    
    //Set trail material
    SetMaterial(MT_TextureWithColor);
    m_currProjMatType = ProjMatType_Perspective;
    UploadWorldViewProjMatrix(m_identityMat);
    
    glUniform1fv(trailShaderUniform_accumulatedTime,1,&m_accumulatedTime);
    
    
	GLuint currTrailTexture = 0;
	
    for(u32 renderTrailIDX=0; renderTrailIDX<m_numRenderableTrails; ++renderTrailIDX)
    {
        GFX_Trail* currTrail = m_renderableTrails[renderTrailIDX];
        const u32 renderFlags = currTrail->renderFlags;
        
        //Don't render trails that can't be drawn
        if(currTrail->m_numTrailParticles < 1)
        {
            continue;
        }
        
        const GFX_TrailSettings* pTrailSettings = &currTrail->trailSettings;
        
        glUniform1fv(trailShaderUniform_scrollAmountU,1,&pTrailSettings->scrollAmountU);
        glUniform1fv(trailShaderUniform_scrollAmountV,1,&pTrailSettings->scrollAmountV);
		
        //Enable or disable alpha blended based on the RenderFlag
        if (renderFlags & RenderFlag_AlphaBlended)
        {
            glEnable(GL_BLEND);
            if(renderFlags & RenderFlag_AdditiveBlending)
            {
                //Additive blending
                glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            }
            else
            {
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//For premultiplied alpha
            }
        }
        else
        {
            glDisable(GL_BLEND);
        }
		
        //Set current trail texture
        //JAMTODO: sort list by texture
		
		GLuint nextTrailTexture = m_renderableTrails[renderTrailIDX]->texture;
		
		if(nextTrailTexture != currTrailTexture)
		{
			SetTexture(&nextTrailTexture,0);
			
			currTrailTexture = nextTrailTexture;
		}
        
        s32 numToRender = 0;
        s32 bufferOffset = 0;
        
        s32 numDrawableTrailParticles = currTrail->m_numTrailParticles;
        f32 maxParticlesForUV = (f32)(numDrawableTrailParticles-1);
        
        //Make a dupe of the last particle
        currTrail->trailParticles[currTrail->m_numTrailParticles] = currTrail->trailParticles[currTrail->m_numTrailParticles-1];
        currTrail->trailParticles[currTrail->m_numTrailParticles].position = currTrail->currPos;
        
        
        //Fill in vert data
        for(s32 trailParticleIDX=0; trailParticleIDX<numDrawableTrailParticles; ++trailParticleIDX)
        {
            f32 texCoordV = 1.0f-((f32)trailParticleIDX)/MaxF(1.0f,maxParticlesForUV);
            const f32 topLerpT = 1.0f-texCoordV;
            
            texCoordV *= pTrailSettings->texCoordMultV;
			
            const vec2 texCoordTopLeft = {0.0f,texCoordV};
            const vec2 texCoordTopRight = {1.0f*pTrailSettings->texCoordMultU,texCoordV};
            
            const GFX_TrailParticle* pParticleHead = &currTrail->trailParticles[trailParticleIDX+1];
            const GFX_TrailParticle* pParticleTail = &currTrail->trailParticles[trailParticleIDX];
            
            const vec3* posHead = &pParticleHead->position;
            const vec3* posTail = &pParticleTail->position;
            
            vec3 distVec;
            SubVec3(&distVec, posHead, posTail);
            
            const f32 magnitude = MagnitudeVec3(&distVec);
            
            const f32 epsilon = 0.02f;
            if (magnitude < epsilon)
            {
                //Skip this particle if it is too close to the previous one
                continue;
            }
            
            vec3 normalizedDistVec;
            ScaleVec3(&normalizedDistVec,&distVec,1.0f/magnitude);
            
            const vec3 leftVec = {normalizedDistVec.y,-normalizedDistVec.x,0.0f};
            
            GFX_TrailParticleData* pTopLeft = &m_trailParticleRenderablePoints[bufferOffset];
            GFX_TrailParticleData* pTopRight = &m_trailParticleRenderablePoints[1+bufferOffset];
            
            //const f32 topLerpT = pParticleHead->timeToLiveCurr/pParticleHead->timeToLiveStart;
			
            
            
            f32 topScale = Lerp(pTrailSettings->fatnessEnd,pTrailSettings->fatnessBegin,topLerpT);
            
            f32 sinCosOffset = 0.0f;
            if(pTrailSettings->sinCosBucket > -1)
            {
                //TODO: if there's a way to bucketize this, do it "currTrail->sinCosBucket"
                //TODO: verify the use of texCoordV here
                sinCosOffset = sinf(0.1f*(f32)m_accumulatedFrames*(f32)pTrailSettings->sinCosBucket
                                   +pTrailSettings->sinCosFreq*texCoordV)*Lerp(pTrailSettings->sinCosEffectScaleBegin,pTrailSettings->sinCosEffectScaleEnd,1.0f-topLerpT);
            }
            
            vec4 topColor;
            LerpVec4(&topColor,&pTrailSettings->colorEnd,&pTrailSettings->colorBegin,topLerpT);
            
            topColor.x *= currTrail->diffuseColor.x;
            topColor.y *= currTrail->diffuseColor.y;
            topColor.z *= currTrail->diffuseColor.z;
            topColor.w *= currTrail->diffuseColor.w;
			
            if(topLerpT > pTrailSettings->alphaFadeInTime)
            {
                topColor.w *= 1.0f-((topLerpT-pTrailSettings->alphaFadeInTime)/pTrailSettings->alphaFadeInTime);
            }
            else if(topLerpT < pTrailSettings->alphaFadeOutTime)
            {
                topColor.w *= topLerpT/pTrailSettings->alphaFadeOutTime;
            }
			
            pTopLeft->texcoord = texCoordTopLeft;
            pTopLeft->color = topColor;
            AddScaledVec3(&pTopLeft->position, posHead, &leftVec, -topScale-sinCosOffset);
            
            pTopRight->texcoord = texCoordTopRight;
            pTopRight->color = topColor;
            AddScaledVec3(&pTopRight->position, posHead, &leftVec, topScale-sinCosOffset);
            
            ++numToRender;
            bufferOffset += VERTS_PER_TRAILPARTICLE;
        }
        
        if(numToRender > 0)
        {
            //Draw verts
            //Draw particles
            glEnableVertexAttribArray(ATTRIB_VERTEX);
            glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(GFX_TrailParticleData), &m_trailParticleRenderablePoints[0].position);
            
            glEnableVertexAttribArray(ATTRIB_TEXCOORD);
            glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(GFX_TrailParticleData), &m_trailParticleRenderablePoints[0].texcoord);
            
            glEnableVertexAttribArray(ATTRIB_COLOR);
            glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(GFX_TrailParticleData), &m_trailParticleRenderablePoints[0].color);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, numToRender*VERTS_PER_TRAILPARTICLE);
        }
    }
    
	/*** PARTICLES ***/
	
	//Enable blending
	glEnable(GL_BLEND);
    
#if DRAWPARTICLES
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//For non-premultiplied alpha
	
	
	//HACK: Draw particles in CameraView0
	if (m_currViewIndex == CameraView0)
	{
		m_currProjMatType = ProjMatType_Perspective;
		
		//Set particle material
		
		for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
		{
			RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
			if(pBucket->m_numParticlesToDraw == 0)
			{
				continue;
			}
			
			switch(bucketIDX)
			{
				case ParticleBucket_Normal:
				{
					SetMaterial(MT_BasicPointSprite);
					UploadWorldViewProjMatrix(m_identityMat);
                    
					//Additive Blending
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					
					break;
				}
				case ParticleBucket_ColorShine:
				{
					//NOTE: NO here assumes additive happens first and sets the texture
					SetMaterial(MT_PointSpriteColorShine);
					UploadWorldViewProjMatrix(m_identityMat);
					
					//Normal blending
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					
					break;
				}
                case ParticleBucket_Additive:
				{
					SetMaterial(MT_BasicPointSprite);
					UploadWorldViewProjMatrix(m_identityMat);
                    
					//Additive Blending
					glBlendFunc (GL_SRC_ALPHA, GL_ONE);
					
					break;
				}
				default:
				{
					break;
				}
			}
			
			
			
			//Draw particles
			glEnableVertexAttribArray(ATTRIB_VERTEX);
			glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(ParticleData), pBucket->m_spriteData);
			
			glEnableVertexAttribArray(ATTRIB_TEXCOORD);
			glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, 0, sizeof(ParticleData), &pBucket->m_spriteData[0].texcoord);
			
			glEnableVertexAttribArray(ATTRIB_COLOR);
			glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(ParticleData), &pBucket->m_spriteData[0].color);
			
			glDrawArrays(GL_TRIANGLE_STRIP, 0, pBucket->m_numParticlesToDraw*VERTS_PER_PARTICLE);
		}
	}	
#endif
}


void OpenGLRenderer::SetClearColor(f32 r, f32 g, f32 b)
{
	m_clearColor.x = r;
    m_clearColor.y = g;
    m_clearColor.z = b;
}


void OpenGLRenderer::SetGravityDir(const vec3* pNewGravityDir)
{
	CopyVec3(&m_gravityDir,pNewGravityDir);
}


void OpenGLRenderer::SetSortRenderablesByZ(bool sortRenderablesByZ)
{
	m_sortRenderablesByZ = sortRenderablesByZ;
}


void OpenGLRenderer::Render(f32 timeElapsed)
{
	if(!paused)
	{
		m_numDebugLinePoints2D_saved = m_numDebugLinePoints2D;
		m_numDebugLinePoints3D_saved = m_numDebugLinePoints3D;
		m_numDebugDrawObjects_saved = m_numDebugDrawObjects;
		m_numTexturedLines_saved = m_numTexturedLines;
		
		m_numDebugLinePoints2D = 0;
		m_numDebugLinePoints3D = 0;
		m_numDebugDrawObjects = 0;
		m_numTexturedLines = 0;
	}
	
    if ENABLE_MULTISAMPLING
    {
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer); 
    }
    
    if(m_numRenderableGeom3Ds > m_maxNumRenderables)
    {
        m_maxNumRenderables = m_numRenderableGeom3Ds;
        //printf("*** Max number of models has increased to: %d\n",m_maxNumRenderables);
    }
    
    if(m_numRenderableTrails > m_maxNumTrails)
    {
        m_maxNumTrails = m_numRenderableTrails;
        printf("*** Max number of trails has increased to: %d\n",m_maxNumTrails);
    }
    
    
    
    //Update screen shake
    m_screenShakerT_X += m_screenShakerSpeed_X*timeElapsed;
    if(m_screenShakerT_X > 1.0f)
    {
        m_screenShakerT_X = 1.0f;
        m_screenShakerSpeed_X *= -1.0f;
    }
    else if(m_screenShakerT_X < -1.0f)
    {
        m_screenShakerT_X = -1.0f;
        m_screenShakerSpeed_X *= -1.0f;
    }
    
    m_screenShakerT_Y += m_screenShakerSpeed_Y*timeElapsed;
    if(m_screenShakerT_Y > 1.0f)
    {
        m_screenShakerT_Y = 1.0f;
        m_screenShakerSpeed_Y *= -1.0f;
    }
    else if(m_screenShakerT_Y < -1.0f)
    {
        m_screenShakerT_Y = -1.0f;
        m_screenShakerSpeed_Y *= -1.0f;
    }
    
    //TODO: comment this back in later when we want to use post processing
    //[self setRenderTarget: &m_screenTarget];
	
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL) ;
    glDepthMask( GL_TRUE );
    glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z,1.0);
#ifdef PLATFORM_IOS
	glClearDepthf(1.0f);
#else
	glClearDepth(1.0f);
#endif
    
	
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    //glClear(GL_DEPTH_BUFFER_BIT);
    
    
    /*if(m_readyToGo == NO)
	 {
	 return;
	 }*/	
    
	if (!paused)
	{
		++m_accumulatedFrames;
        
        if (m_accumulatedFrames == 10000)
        {
            m_accumulatedFrames = 0;
        }
		
		m_accumulatedTime = (f32)m_accumulatedFrames;
	}
	
	
	for (s32 i=0; i<NUM_SINCOS_BUCKETS; ++i)
	{
		const f32 sinCosT = (f32)m_accumulatedFrames*m_sinCosBuckets[i].multiplier;
		m_sinCosBuckets[i].sinTheta = sinf(sinCosT);
		m_sinCosBuckets[i].cosTheta = cosf(sinCosT);
	}
	
	//Sort particles if needed
	SortParticleQueues();
	
	//Sort renderables if needed
	if(m_renderableObject3DsNeedSorting == true)
	{
		SortRenderableGeometry3DList();
		
		m_renderableObject3DsNeedSorting = false;
	}
    
    if(m_sortRenderablesByZ)
    {
        SortRenderablesInLayerRangeByZ(RenderLayer_Normal,RenderLayer_AlphaBlended1);
    }
	
	if (!paused)
	{
		//Update particles (some particles might die here)
		UpdateParticleQueues(timeElapsed);
        
        
        //update trails
        m_numRenderableTrails = 0;
        UpdateTrails(timeElapsed);
		
		//Copy positions and other data into array LOL!
		for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
		{
			RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
            
            //I keep a separate count for particles to draw for the case of someone adding particles
            //when the graphics are paused.  It would not update the vert buffer but it would tell it
            //to draw all these new particles.  NOT GOOD.
			pBucket->m_numParticlesToDraw = 0;
            
			s32 particleVertIDX = 0;
			for (s32 i=0; i<pBucket->m_numParticles; ++i,particleVertIDX+=VERTS_PER_PARTICLE,++pBucket->m_numParticlesToDraw)
			{
				Particle3D* pCurrParticle = &pBucket->m_particleQueue[i];
				vec3 particleCenterPos;
				CopyVec3(&particleCenterPos, pCurrParticle->pPositionOverride?pCurrParticle->pPositionOverride:&pCurrParticle->position);
				
				const f32 ttl = pCurrParticle->totalTTL-pCurrParticle->timeToLive;
				
				f32 particleAlpha;
				
				if (ttl < pCurrParticle->alphaInTime)
				{
					particleAlpha = ttl/pCurrParticle->alphaInTime;
				}
				else if(ttl > pCurrParticle->alphaOutTime)
				{
					const f32 alphaOutTime = pCurrParticle->alphaOutTime;
					particleAlpha = 1.0f-(ttl-alphaOutTime)/(pCurrParticle->totalTTL-alphaOutTime);
				}
				else
				{
					particleAlpha = 1.0f;
				}
				
				f32 particleSize;
				
				particleSize = Lerp(pCurrParticle->size_max, pCurrParticle->size_min, ttl/pCurrParticle->totalTTL);
				
				vec3 cameraLeft,cameraUp;
				mat4f_GetViewUp(&cameraUp,m_view[CameraView0]);
				mat4f_GetViewLeft(&cameraLeft,m_view[CameraView0]);
				
				vec3* topLeft = &pBucket->m_spriteData[particleVertIDX+1].position;
				vec3* topRight = &pBucket->m_spriteData[particleVertIDX+2].position;
				vec3* bottomLeft = &pBucket->m_spriteData[particleVertIDX+3].position;
				vec3* bottomRight = &pBucket->m_spriteData[particleVertIDX+4].position;
				
				const f32 fatMult = (pCurrParticle->particleFlags & ParticleFlag_MultWidthByAlpha?particleAlpha:(pCurrParticle->particleFlags & ParticleFlag_DivWidthByAlpha?1.0f/particleAlpha:1.0f));
				const f32 flipUMult = (((pCurrParticle->particleFlags & ParticleFlag_FlipU) && pCurrParticle->sinCosBucket<0)?-pCurrParticle->flipRotMult:pCurrParticle->flipRotMult)*fatMult;
				const f32 sizePositiveX = particleSize*flipUMult;
				const f32 sizeNegativeX = -particleSize*flipUMult;
				
				const f32 flipVMult = ((pCurrParticle->particleFlags & ParticleFlag_FlipV) && pCurrParticle->sinCosBucket<0)?-1.0f:1.0f;
				const f32 sizePositiveY = particleSize*flipVMult;
				const f32 sizeNegativeY = -particleSize*flipVMult;
				
				
				AddScaledVec3(topLeft,&particleCenterPos,&cameraLeft,sizeNegativeX);
				AddScaledVec3_Self(topLeft,&cameraUp,sizePositiveY);
				
				AddScaledVec3(topRight,&particleCenterPos,&cameraLeft,sizePositiveX);
				AddScaledVec3_Self(topRight,&cameraUp,sizePositiveY);
				
				AddScaledVec3(bottomLeft,&particleCenterPos,&cameraLeft,sizeNegativeX);
				AddScaledVec3_Self(bottomLeft,&cameraUp,sizeNegativeY);
				
				AddScaledVec3(bottomRight,&particleCenterPos,&cameraLeft,sizePositiveX);
				AddScaledVec3_Self(bottomRight,&cameraUp,sizeNegativeY);
				
				AddVec3_Self(topLeft,&pCurrParticle->positionOffset);
				AddVec3_Self(topRight,&pCurrParticle->positionOffset);
				AddVec3_Self(bottomLeft,&pCurrParticle->positionOffset);
				AddVec3_Self(bottomRight,&pCurrParticle->positionOffset);
				
				CopyVec3(&pBucket->m_spriteData[particleVertIDX].position,topLeft);
				CopyVec3(&pBucket->m_spriteData[particleVertIDX+5].position,bottomRight);
				
				
				//Make rotated base coordinates
				vec2 texcoord_TopLeft;
				
				texcoord_TopLeft.x = -HALF_PARTICLE_TEXCOORD_OFFSET;
				texcoord_TopLeft.y = 0;
                
				if (pCurrParticle->sinCosBucket > -1)
				{
					const f32 sinTheta = m_sinCosBuckets[pCurrParticle->sinCosBucket].sinTheta;
					const f32 cosTheta = m_sinCosBuckets[pCurrParticle->sinCosBucket].cosTheta;
					
                    MakeRotatedVec2_SinCos(&texcoord_TopLeft, sinTheta, cosTheta);
                    ScaleVec2_Self(&texcoord_TopLeft, -HALF_PARTICLE_TEXCOORD_OFFSET);
				}
				else if(pCurrParticle->particleFlags & ParticleFlag_AlignToVel_All)
				{
					vec3 dir = vec3_left;
					TryNormalizeVec3( &dir, &pCurrParticle->velocity );
					f32 angle = atan2( dir.x, dir.y );
					
					pCurrParticle->alignmentSin = sinf(angle);
					pCurrParticle->alignmentCos = -cosf(angle);
					
                    MakeRotatedVec2_SinCos(&texcoord_TopLeft, pCurrParticle->alignmentSin, pCurrParticle->alignmentCos);
                    ScaleVec2_Self(&texcoord_TopLeft, -HALF_PARTICLE_TEXCOORD_OFFSET);
				}
				else if(pCurrParticle->particleFlags & ParticleFlag_AlignToVel_Start)
				{
                    MakeRotatedVec2_SinCos(&texcoord_TopLeft, pCurrParticle->alignmentSin, pCurrParticle->alignmentCos);
                    ScaleVec2_Self(&texcoord_TopLeft, -HALF_PARTICLE_TEXCOORD_OFFSET);
				}
                
                //texcoord_TopLeft is now simply the left direction scaled by -HALF_PARTICLE_TEXCOORD_OFFSET
				
                //Save left direction before you shift texcoord_TopLeft
                vec2 texCoordDirLeft = texcoord_TopLeft;
                
                //Get texCoordUp from texCoordDirLeft
                vec2 texCoordDirUp;
                SetVec2(&texCoordDirUp,-texCoordDirLeft.y,texCoordDirLeft.x);
                
                //Move top left coord into position
                
                //Now it's the top left
                AddVec2_Self(&texcoord_TopLeft, &texCoordDirUp);
				
                //Now it's adjusted by the offset
                AddVec2_Self(&texcoord_TopLeft, &pCurrParticle->texcoordOffset);
				
                texcoord_TopLeft.x += HALF_PARTICLE_TEXCOORD_OFFSET;
                texcoord_TopLeft.y += HALF_PARTICLE_TEXCOORD_OFFSET;
				
                //Initially set all tex coords to be top left, then we'll add offsets
                vec2 texcoord_TopRight = texcoord_TopLeft;
                vec2 texcoord_BottomRight = texcoord_TopLeft;
                vec2 texcoord_BottomLeft = texcoord_TopLeft;
                
                AddScaledVec2_Self(&texcoord_TopRight,&texCoordDirLeft, -2.0f);
                AddScaledVec2(&texcoord_BottomRight,&texcoord_TopRight,&texCoordDirUp, -2.0f);
                AddScaledVec2(&texcoord_BottomLeft,&texcoord_TopLeft,&texCoordDirUp, -2.0f);
                
				//0,0
				CopyVec2(&pBucket->m_spriteData[particleVertIDX+1].texcoord,&texcoord_TopLeft);
				
				//1,0
				CopyVec2(&pBucket->m_spriteData[particleVertIDX+2].texcoord,&texcoord_TopRight);
				
				//0,1
				CopyVec2(&pBucket->m_spriteData[particleVertIDX+3].texcoord,&texcoord_BottomLeft);
				
				//1,1
				CopyVec2(&pBucket->m_spriteData[particleVertIDX+4].texcoord,&texcoord_BottomRight);
				
				//Create a vec4 of color!
				vec4 finalColor;
				CopyVec3((vec3*)&finalColor,&pCurrParticle->color);
				finalColor.w = particleAlpha*pCurrParticle->overallAlpha;
				
				//Copy all other data over
				for (s32 pDataIDX=particleVertIDX+1; pDataIDX<particleVertIDX+VERTS_PER_PARTICLE-1; ++pDataIDX)
				{
					CopyVec4(&pBucket->m_spriteData[pDataIDX].color,&finalColor);
				}	
			}
		}
	}
	
#if DEBUG_DRAW && PARTICLE_TEST_PRINT
	printf("Num particles: %d\n",m_numParticles);
#endif
	
	glEnable(GL_CULL_FACE);
	
	//Render renderables
	
#ifdef DEBUG_RENDERLOOP
	printf("**Render start:\n\n");
	int skippedTextureSwaps = 0;
#endif
	
	//Render models in each camera view
	m_lastUsedMaterial = (RenderMaterial)RENDERMATERIAL_INVALID;
	
	
    //Reset renderstate flags
    m_renderStateFlags = 0;
	
    
	for (u32 camViewIDX=0; camViewIDX<NumCameraViews; ++camViewIDX)
	{	
		//Set what camera we're currently rendering with
		m_currViewIndex = camViewIDX;
		
		//Create view projection matrices once per view
		for(u32 i=0; i<NumProjMatTypes; ++i)
		{
            
            if (m_screenShakeTimer > 0.0f)
            {
                m_screenShakeTimer -= timeElapsed;
                
                const f32 shakeAmount = m_currScreenShakeAmount*MaxF(m_screenShakeTimer/m_screenShakeStartTime,0.0f);
                
                f32 viewMatCopy[16];
                memcpy(viewMatCopy,m_view[m_currViewIndex],sizeof(mat4f));
                
                AddScaledVec3_Self(mat4f_GetPos(viewMatCopy),mat4f_GetLeft(viewMatCopy),(m_screenShakerT_X+m_screenShakerT_Y)*shakeAmount);
                AddScaledVec3_Self(mat4f_GetPos(viewMatCopy),mat4f_GetUp(viewMatCopy),(m_screenShakerT_X-m_screenShakerT_Y)*shakeAmount);
                
                mat4f_Multiply(m_viewProjMats[m_currViewIndex][i],m_projMats[i], viewMatCopy);
                
            }
            else
            {
                mat4f_Multiply(m_viewProjMats[m_currViewIndex][i],m_projMats[i], m_view[m_currViewIndex]);
            }
		}
		
		glDepthMask( GL_TRUE );
        if(camViewIDX > 0)
        {
            glClear(GL_DEPTH_BUFFER_BIT);
        }
		//TODO: one day if we have multiple viewports, we'll have to link cameras to viewports
#if RENDERLOOP_ENABLED
		RenderLoop(camViewIDX,m_renderableGeometry3DList,m_numRenderableGeom3Ds);
		RenderLoop(0,m_renderableUIList,m_numRenderableUIObjects);
#endif        
	}
	
	RenderEffects();
    
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    
    //Normal blending
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	//Handle fading
	switch (m_pauseFadeState)
	{
        case FadeState_FadeOut:
		{
			m_pauseCurrFadeTime += timeElapsed;
			if (m_pauseCurrFadeTime > m_pauseTotalFadeTime)
			{
                m_pauseCurrFadeTime = m_pauseTotalFadeTime;
				m_pauseFadeState = FadeState_WaitingForFadeIn;
			}
			const f32 fadeT = m_pauseCurrFadeTime/m_pauseTotalFadeTime;
            
            const f32 finalFade = Lerp(0.0f,m_pauseFinalFade,fadeT);
			SetVec4(&m_fadeColor,0.0f,0.0f,0.0f,finalFade);
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
            
			break;
		}
        case FadeState_WaitingForFadeIn:
        {
            SetVec4(&m_fadeColor,0.0f,0.0f,0.0f,m_pauseFinalFade);
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
            
            if (m_pauseRequestedFadeIn == true)
            {
                m_pauseRequestedFadeIn = false;
                m_pauseCurrFadeTime = m_pauseTotalFadeTime;
                
                m_pauseFadeState = FadeState_FadeIn;
            }
            break;
        }
		case FadeState_FadeIn:
		{
			m_pauseCurrFadeTime -= timeElapsed;
			if (m_pauseCurrFadeTime < 0.0f)
			{
				m_pauseFadeState = FadeState_Idle;
				m_pauseCurrFadeTime = 0.0f;
			}
			const f32 fadeT = m_pauseCurrFadeTime/m_pauseTotalFadeTime;
            
            const f32 finalFade = Lerp(0.0f,m_pauseFinalFade,fadeT);
			SetVec4(&m_fadeColor,0.0f,0.0f,0.0f,finalFade);
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
			
			break;
		}
		case FadeState_Idle:
		{
			break;
		}
		default:
			break;
	}
    
    
    //Additive blending
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    
    //Handle flashing
	switch (m_flashState)
	{
		case FadeState_FadeOut:
		{
			m_currFlashTime += timeElapsed;
			if (m_currFlashTime > m_totalFlashTime)
			{
                m_currFlashTime = m_totalFlashTime;
				m_flashState = FadeState_FadeIn;
			}
			const f32 fadeT = m_currFlashTime/m_totalFlashTime;
            
			SetVec4(&m_fadeColor,m_flashColor.x,m_flashColor.y,m_flashColor.z,fadeT);
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
            
			break;
		}
		case FadeState_FadeIn:
		{
			m_currFlashTime -= timeElapsed;
			if (m_currFlashTime < 0.0f)
			{
				m_flashState = FadeState_Idle;
				m_currFlashTime = 0.0f;
			}
			const f32 fadeT = m_currFlashTime/m_totalFlashTime;
            
			SetVec4(&m_fadeColor,m_flashColor.x,m_flashColor.y,m_flashColor.z,fadeT);
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
			
			break;
		}
		default:
			break;
	}
    
    //Normal blending
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	//Handle fading
	switch (m_fadeState)
	{
		case FadeState_FadeOut:
		{
			m_currFadeTime += timeElapsed;
			if (m_currFadeTime > m_totalFadeTime)
			{
                m_currFadeTime = m_totalFadeTime;
				m_fadeState = FadeState_WaitingForFadeIn;
			}
			const f32 fadeT = m_currFadeTime/m_totalFadeTime;
			
			m_currFadeValue = 1.0f-fadeT;
			
			CopyVec3((vec3*)&m_fadeColor,&m_fadeToScreenColor);
			m_fadeColor.w = fadeT;
			
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
			
			break;
		}
        case FadeState_WaitingForFadeIn:
        {
            CopyVec3((vec3*)&m_fadeColor,&m_fadeToScreenColor);
			m_fadeColor.w = 1.0f;
			
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
            
            if (m_requestedFadeIn == true)
            {
                m_requestedFadeIn = false;
                m_currFadeTime = m_totalFadeTime;
				
                m_fadeState = FadeState_FadeIn;
            }
            break;
        }
		case FadeState_FadeIn:
		{
			m_currFadeTime -= timeElapsed;
			if (m_currFadeTime < 0.0f)
			{
				m_fadeState = FadeState_Idle;
				m_currFadeTime = 0.0f;
			}
			const f32 fadeT = m_currFadeTime/m_totalFadeTime;
			
			m_currFadeValue = 1.0f-fadeT;
			
			CopyVec3((vec3*)&m_fadeColor,&m_fadeToScreenColor);
			m_fadeColor.w = fadeT;
			
			PostProcess(PPMT_PureColor,&m_screenTarget,PPDrawArea_FullScreen,NULL,NULL,NULL);
			
			break;
		}
		case FadeState_Idle:
		{
			m_currFadeValue = 1.0f; 
			
			break;
		}
		default:
			break;
	}
	
#if DEBUG_DRAW
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    //Render debug stuff
    SetMaterial(MT_VertColors);
    
    m_currProjMatType = ProjMatType_Orthographic_Points;
    UploadWorldProjMatrix(m_identityMat);
    
    //Disable depth test
	glEnable ( GL_DEPTH_TEST );
	
	//Disable depth write
	glDepthMask( GL_FALSE );
	
	//Disable culling
	glDisable(GL_CULL_FACE);
    
    if(m_numDebugLinePoints2D_saved > 0)
    {
        //Draw 2D lines
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(PointData), &m_debugLinePoints2D[0].position);
        
        glEnableVertexAttribArray(ATTRIB_COLOR);
        glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(PointData), &m_debugLinePoints2D[0].color);
        
        glDrawArrays(GL_LINES, 0, m_numDebugLinePoints2D_saved);
    }
    
    if(m_numDebugLinePoints3D_saved > 0)
    {
        //Draw 3D lines
        m_currProjMatType = ProjMatType_Perspective;
        UploadWorldViewProjMatrix(m_identityMat);
        
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(PointData), &m_debugLinePoints3D[0].position);
        
        glEnableVertexAttribArray(ATTRIB_COLOR);
        glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(PointData), &m_debugLinePoints3D[0].color);
        
        glDrawArrays(GL_LINES, 0, m_numDebugLinePoints3D_saved);
    }
    
    //DEBUG DRAW OBJECTS
    
    m_currProjMatType = ProjMatType_Perspective;
    SetMaterial(MT_VertWithColorInput);
    
    for(u32 i=0; i<m_numDebugDrawObjects_saved; ++i)
    {
        DebugDrawObject* pCurrObject = &m_debugDrawObjects[i];
        
        UploadWorldViewProjMatrix(pCurrObject->matrix);
        
        glUniform4fv(g_Materials[MT_VertWithColorInput].uniforms_unique[0],1,(f32*)&pCurrObject->color);
		
        ModelData* pModelData = NULL;
        
        switch(pCurrObject->objectType)
        {
            case DebugDrawObjectType_Circle:
            {
                pModelData = &g_DEBUGMODEL_Circle_modelData;
                break;
            }
            case DebugDrawObjectType_Cylinder:
            {
                pModelData = &g_DEBUGMODEL_Cylinder_modelData;
                break;
            }
            default:
            {
                break;
            }
        }
        
        for(u32 i=0; i<pModelData->numPrimitives; ++i)
        {
            PrimitiveData* pCurrPrim = &pModelData->primitiveArray[i];
            if (m_supportsFeaturesFromiOS4)
            {
                BindVertexArrayObject(pCurrPrim);
            }
            else
            {
                glBindBuffer(GL_ARRAY_BUFFER, pCurrPrim->vertexArrayObjectID);
                EnableAttributes(pModelData);
            }
            
            glDrawArrays(pCurrPrim->drawMethod, 0, pCurrPrim->numVerts);
        }
    }
#endif
    
	/*if (parentView.drawMatrices)
	 {	
	 m_currProjMatType = ProjMatType_Perspective;
	 
	 [self setMaterial: MT_VertColors];
	 
	 for (u32 renderIndex=0; renderIndex<m_numRenderableObject3Ds; ++renderIndex)
	 {
	 const RenderableObject3D* pCurrRenderableObject3D = m_renderableObject3DList[renderIndex];
	 if(pCurrRenderableObject3D->flags & RenderFlag_Visible)
	 {
	 [self uploadWorldViewProjMatrix:pCurrRenderableObject3D->worldMat];		
	 Draw_matrix();
	 }
	 }
	 }*/
    
#if DEBUG_DRAWPARTICLESASLINESTRIP
    //HACK: Draw particles in CameraView0
	if (m_currViewIndex == CameraView0)
	{
		//Set particle material
        m_currProjMatType = ProjMatType_Perspective;
        [self setMaterial: MT_VertColors];
        [self uploadWorldViewProjMatrix:m_identityMat];
        glDisable(GL_BLEND);
        
		for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
		{
			RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
			
			if(pBucket->m_numParticlesToDraw == 0)
			{
				continue;
			}
			
			//Draw particles
			glEnableVertexAttribArray(ATTRIB_VERTEX);
			glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(ParticleData), pBucket->m_spriteData);
			
			glEnableVertexAttribArray(ATTRIB_COLOR);
			glVertexAttribPointer(ATTRIB_COLOR, 4, GL_FLOAT, 0, sizeof(ParticleData), &pBucket->m_spriteData[0].color);
			
			glDrawArrays(GL_LINE_STRIP, 0, pBucket->m_numParticlesToDraw*VERTS_PER_PARTICLE);
		}
	}
#endif
    
	
	/* if (ENABLE_MULTISAMPLING)
	 {
	 glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, viewFrameBuffer);
	 glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE,  msaaFramebuffer);
	 glResolveMultisampleFramebufferAPPLE();
	 
	 const GLenum discards[]  = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
	 glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE,2,discards);
	 
	 glBindRenderbuffer(GL_RENDERBUFFER, viewFrameBuffer);
	 }
	 else
	 {
	 const GLuint discards[]  = {GL_DEPTH_ATTACHMENT};
	 glDiscardFramebufferEXT(GL_FRAMEBUFFER,1,discards);
	 }*/
	
	
	//dump contents of buffer into the presentRenderBuffer and show on the screen
	//[context presentRenderbuffer:GL_RENDERBUFFER];
}


bool OpenGLRenderer::LoadTextureFromData(GLuint* out_textureName,const void* data,u32 texWidth, u32 texHeight, u32 format, u32 type, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV)
{
	if (filterMode == GL_LINEAR_MIPMAP_LINEAR || filterMode == GL_LINEAR_MIPMAP_NEAREST)
	{
		//NSLog(@"ERROR: loadTextureFromUIImage-> MIPMAP_LINEAR && MIPMAP_NEAREST unsupported!");
		return false;
	}
	
	glGenTextures(1, out_textureName);
	glBindTexture(GL_TEXTURE_2D, *out_textureName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filterMode); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filterMode);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeV);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth,texHeight, 0, format, type, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}


bool OpenGLRenderer::UpdateTextureFromData(GLuint* out_textureName, const void* data, u32 texWidth, u32 texHeight, u32 format, u32 type)
{
	glBindTexture(GL_TEXTURE_2D, *out_textureName);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth,texHeight, 0, format, type, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}


void OpenGLRenderer::RegisterModel(ModelData* pModelData)
{
	PrintOpenGLError("/*** Entered registerModel ***/");
	
	for(u32 i=0; i<pModelData->numPrimitives; ++i)
	{
		PrimitiveData* currPrim = &pModelData->primitiveArray[i];
		
        PrintOpenGLError("Before binding VertexArrayObject");
		
        //Use vertex array object
        if (m_supportsFeaturesFromiOS4)
        {
            GLuint vertexBufferID;
            glGenBuffers(1,&vertexBufferID);

#ifdef PLATFORM_IOS
			glGenVertexArraysOES(1, &currPrim->vertexArrayObjectID);
            glBindVertexArrayOES(currPrim->vertexArrayObjectID);
#else
			glGenVertexArraysAPPLE(1, &currPrim->vertexArrayObjectID);
            glBindVertexArrayAPPLE(currPrim->vertexArrayObjectID);
#endif
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
            PrintOpenGLError("Binding vertex buffer");
        }
        //Use plain old VBO
        else
        {
            glGenBuffers(1,&currPrim->vertexArrayObjectID);
            glBindBuffer(GL_ARRAY_BUFFER, currPrim->vertexArrayObjectID);
        }
		
		glBufferData(GL_ARRAY_BUFFER,currPrim->sizeOfVertexData,currPrim->vertexData,GL_STATIC_DRAW);
		PrintOpenGLError("Uploading vertex buffer");
		
        PrintOpenGLError("After binding VertexArrayObject");
        
        //If we're using VAO, enable the attributes here once and never again
        if (m_supportsFeaturesFromiOS4)
        {
            EnableAttributes(pModelData);
			
            //Make sure nothing randomly writes to our new VAO
#ifdef PLATFORM_IOS
			glBindVertexArrayOES(0);
#else
			glBindVertexArrayAPPLE(0);
#endif
            
            PrintOpenGLError("After resetting VAO");
        }  
        
        //Use index data if it exists
		if (currPrim->indexData != NULL)
		{
			glGenBuffers(1,&currPrim->indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currPrim->indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currPrim->sizeOfIndexData, currPrim->indexData, GL_STATIC_DRAW);
		}
	}
	
	pModelData->modelID = m_numModels++;
	
	PrintOpenGLError("End of Register Model");
}


void OpenGLRenderer::AddRenderableObject3DToList(RenderableObject3D* pRenderable)
{
	if (!(pRenderable->geom.flags & RenderFlag_Initialized))
	{
		//assert(0);
		//NSLog(@"Insane Error: addRenderableObject3D -> You can't add an uninitialized RenderableObject3D to the renderer!");
		
		return;
	}
	
	if(pRenderable->geom.renderLayer == RenderLayer_UI)
	{
		if(m_numRenderableUIObjects < MAX_RENDERABLE_UI_OBJECTS)
		{
			m_renderableUIList[m_numRenderableUIObjects++] = &pRenderable->geom;
			
			m_renderableUINeedSorting = true;
		}
		else
		{
			NSLog(@"Insane Error: addRenderableObject3D -> You tried to add more than MAX_RENDERABLE_UI_OBJECTS!  Added nothing.");
		}
	}
	else
	{
		if(m_numRenderableGeom3Ds < MAX_RENDERABLE_3D_OBJECTS)
		{
			m_renderableGeometry3DList[m_numRenderableGeom3Ds++] = &pRenderable->geom;
			
			m_renderableObject3DsNeedSorting = true;
		}
		else
		{
			NSLog(@"Insane Error: addRenderableObject3D -> You tried to add more than MAX_RENDERABLE_3D_OBJECTS!  Added nothing.");
		}
	}
}


void OpenGLRenderer::AddParticleToQueue(Particle3D* pParticle, vec3* pPosition, vec3* pCallbackPos, ParticleBuckets particleBucket)
{
	RendererParticleBucket* pBucket = &m_particleBuckets[particleBucket];
	
	if (pBucket->m_numParticles < MAX_PARTICLES)
	{
		//Copy particle contents
		Particle3D* pCurrParticle = &pBucket->m_particleQueue[pBucket->m_numParticles];
		*pCurrParticle = *pParticle;
        pCurrParticle->pCallbackPos = pCallbackPos;
		
		//TODO: remove
		pCurrParticle->totalTTL = pCurrParticle->timeToLive;
		
		if (pCurrParticle->particleFlags & ParticleFlag_OverridePosition)
		{
			pCurrParticle->pPositionOverride = pPosition;
		}
		else
		{
			pCurrParticle->pPositionOverride = NULL;
			
			//Set the sprite position
			CopyVec3(&pCurrParticle->position,pPosition);
		}
		
		
#if DEBUG_DRAW && PARTICLE_TEST_PRINT
		//printf("-- Added particle(%d) with pos:<%f,%f,%f>, TTL: %f\n",m_numParticles,pCurrParticle->position.x, pCurrParticle->position.y, pCurrParticle->position.z, m_particleQueue[m_numParticles].timeToLive);
#endif
		
		++pBucket->m_numParticles;
		
		pBucket->m_particlesNeedSorting = true;
	}
#if PRINT_ERRORS
	/*else 
	 {
	 NSLog(@"INSANE ERROR: You've reached the particle limit!");
	 }*/
#endif
}


void OpenGLRenderer::SpawnParticles(vec3* pPosition, const vec3* pColor, const ParticleSettings* particleSettings, s32 numParticles)
{
	RendererParticleBucket* pBucket = &m_particleBuckets[particleSettings->particleBucket];
	
	const s32 particlesLeft = MAX_PARTICLES-pBucket->m_numParticles;
	const s32 numToSpawn = MaxS32(0,MinS32(particlesLeft, numParticles));
	
	for (s32 i=0; i<numToSpawn; ++i)
	{
		Particle3D newParticle;
		
		newParticle.particleFlags = particleSettings->particleFlags;
		
		newParticle.timeToLive = rand_FloatRange(particleSettings->timeToLive_Min, particleSettings->timeToLive_Max);
		newParticle.alphaInTime = newParticle.timeToLive*rand_FloatRange(particleSettings->alphaInTime_Min, particleSettings->alphaInTime_Max);
		newParticle.alphaOutTime = newParticle.timeToLive-newParticle.timeToLive*rand_FloatRange(particleSettings->alphaOutTime_Min, particleSettings->alphaOutTime_Max);
		
		vec2 rotatedVec2;
		const f32 velocityAngle = rand_FloatRange(particleSettings->velocityAngle_Min,particleSettings->velocityAngle_Max);
		
		MakeRotatedVec2(&rotatedVec2,velocityAngle);
		const u8 flipVel = rand_IntRange(0, 1);
		if (newParticle.particleFlags & ParticleFlag_FlipVelOnY && flipVel)
		{
			rotatedVec2.x *= -1.0f;
		}
		
		newParticle.positionOffset.x = rotatedVec2.x*particleSettings->positionOffsetAmount;
		newParticle.positionOffset.y = rotatedVec2.y*particleSettings->positionOffsetAmount;
		newParticle.positionOffset.z = 0.0f;
		
		if (newParticle.particleFlags & ParticleFlag_AlignToVel_Start ||
			newParticle.particleFlags & ParticleFlag_AlignToVel_All )
		{
			f32 finalAngle = velocityAngle-PI_DIV_2;
			if (newParticle.particleFlags & ParticleFlag_FlipVelOnY && flipVel)
			{
				finalAngle *= -1.0f;
			}
			newParticle.alignmentSin = sinf(finalAngle);
			newParticle.alignmentCos = -cosf(finalAngle);
		}
		
		newParticle.overallAlpha = particleSettings->overallAlpha;
		
		if ((newParticle.particleFlags & ParticleFlag_AlignToVel_Start ||
			 newParticle.particleFlags & ParticleFlag_AlignToVel_All )
			&& newParticle.particleFlags & ParticleFlag_CancelVelocity)
		{
			SetVec3(&newParticle.velocity, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			vec3 rotatedVec3;
			rotatedVec3.x = rotatedVec2.x;
			rotatedVec3.y = rotatedVec2.y;
			rotatedVec3.z = 0.0f;
			ScaleVec3(&newParticle.velocity, &rotatedVec3, rand_FloatRange(particleSettings->speed_Min, particleSettings->speed_Max));
		}
		
		newParticle.gravity = rand_FloatRange(particleSettings->gravity_Min,particleSettings->gravity_Max);
		
		const s32 numCells = rand_IntRange(0,particleSettings->atlas_numCells-1);
		
		s32 cellX = particleSettings->atlasCell_X+numCells;
		s32 cellY = particleSettings->atlasCell_Y;
		
		//TODO: see if I can directly compute this but too tired (should be easy)
		const s32 max_X = 15;
		while (cellX > max_X)
		{
			cellX -= max_X;
			++cellY;
		}
		
		newParticle.texcoordOffset.x = (f32)cellX*PARTICLE_TEXCOORD_OFFSET;
		newParticle.texcoordOffset.y = (f32)cellY*PARTICLE_TEXCOORD_OFFSET;
		
		newParticle.size_max = rand_FloatRange(particleSettings->size_Min, particleSettings->size_Max);
		
		if (newParticle.particleFlags & ParticleFlag_FlipScalePercent && rand_IntRange(0, 1))
		{
			newParticle.size_min = newParticle.size_max/particleSettings->scalePercent;
		}
		else
		{
			newParticle.size_min = newParticle.size_max*particleSettings->scalePercent;
		}
		
		
		CopyVec3(&newParticle.color, pColor);
		
		newParticle.sinCosBucket = rand_IntRange(particleSettings->sinCosBucket_Min, particleSettings->sinCosBucket_Max);
		
		newParticle.flipRotMult = (newParticle.particleFlags & ParticleFlag_FlipRotation)?(rand_IntRange(0, 1)?-1.0f:1.0f):1.0f;
		
		newParticle.particleFlags &= ~ParticleFlag_FlipU;
		newParticle.particleFlags &= ~ParticleFlag_FlipV;
		
		if ((particleSettings->particleFlags & ParticleFlag_FlipU) && rand_IntRange(0, 1))
		{
			newParticle.particleFlags |= ParticleFlag_FlipU;
		}
		
		if ((particleSettings->particleFlags & ParticleFlag_FlipV) && rand_IntRange(0, 1))
		{
			newParticle.particleFlags |= ParticleFlag_FlipV;
		}
		
		newParticle.pCallbackCounter = NULL;
		newParticle.timeForCallbackCounter = 0.0f;
		
		AddParticleToQueue(&newParticle,pPosition,NULL,particleSettings->particleBucket);
	}
}


void OpenGLRenderer::UpdateParticleQueues(f32 timeElapsed)
{
	for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
	{
		RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
		
		const s32 currNumParticles = pBucket->m_numParticles;
		for (s32 i=0; i < currNumParticles; ++i)
		{
			Particle3D* pCurrParticle = &pBucket->m_particleQueue[i];
			
			const f32 currTTL = pCurrParticle->timeToLive;
			
			pCurrParticle->timeToLive -= timeElapsed;
			
			AddScaledVec3_Self(&pCurrParticle->position, &pCurrParticle->velocity, timeElapsed);
            AddScaledVec3_Self(&pCurrParticle->velocity,&m_gravityDir,pCurrParticle->gravity*timeElapsed);
			
            if (pCurrParticle->pCallbackPos)
            {
                CopyVec3(pCurrParticle->pCallbackPos,&pCurrParticle->position);
            }
            
            //Increment the callback counter once when the timeForCallbackCounter is reached
			if (pCurrParticle->pCallbackCounter && currTTL>pCurrParticle->timeForCallbackCounter
				&& pCurrParticle->timeToLive <= pCurrParticle->timeForCallbackCounter)
			{
				++(*pCurrParticle->pCallbackCounter);
			}
			
			if (pCurrParticle->timeToLive < 0.0f)
			{
				pCurrParticle->timeToLive = 0.0f;
				
				--pBucket->m_numParticles;
				
#if DEBUG_DRAW && PARTICLE_TEST_PRINT
				//printf("- Particle died...\n");
#endif	
			}
		}
	}
}


void OpenGLRenderer::UpdateTrails(f32 timeElapsed)
{
	m_numRenderableTrails = 0;
    
    for (s32 trailIDX=0; trailIDX<MAX_TRAILS; ++trailIDX)
    {
        GFX_Trail* pCurrTrail = &m_trails[trailIDX];
        const GFX_TrailSettings* pTrailSettings = &pCurrTrail->trailSettings;
        
        if (!pCurrTrail->inUse)
        {
            continue;
        }
        
        vec3 distVec;
        SubVec3(&distVec,&pCurrTrail->currPos, &pCurrTrail->trailParticles[pCurrTrail->m_numTrailParticles-1].position);
        const f32 distSqFromLastParticle = MagnitudeSqVec3(&distVec);
        
        
        const bool createNewParticle = distSqFromLastParticle > pCurrTrail->segmentLengthSq;
        
        if(createNewParticle && pCurrTrail->m_numTrailParticles == MAX_TRAIL_PARTICLES_PER_TRAIL)
        {   
            //If we're out of trail particles, just kill the first one to make room
            pCurrTrail->trailParticles[0].timeToLiveCurr = -1.0f;
        }
        
        //Count up the dead pieces
        s32 numDeadParticles = 0;
        for (s32 i=0; i<pCurrTrail->m_numTrailParticles; ++i)
        {
            pCurrTrail->trailParticles[i].timeToLiveCurr -= timeElapsed;
            if (pCurrTrail->trailParticles[i].timeToLiveCurr < 0.0f)
            {
                ++numDeadParticles;
            }
        }
        
        //Shift out the dead ones
        for (s32 i=numDeadParticles; i<pCurrTrail->m_numTrailParticles; ++i)
        {
            pCurrTrail->trailParticles[i-numDeadParticles] = pCurrTrail->trailParticles[i];
        }
        
        //Update the number of particles
        pCurrTrail->m_numTrailParticles -= numDeadParticles;       
        
        //Check if we should add a new particle
        if(createNewParticle)
        {
            //printf("DistSq: %f\n",distSqFromLastParticle);
            
            GFX_TrailParticle* pCurrParticle = &pCurrTrail->trailParticles[pCurrTrail->m_numTrailParticles];
            CopyVec3(&pCurrParticle->position,&pCurrTrail->currPos);
            pCurrParticle->timeToLiveStart = pTrailSettings->timeToLivePerParticle;
            pCurrParticle->timeToLiveCurr = pTrailSettings->timeToLivePerParticle;
            
            ++pCurrTrail->m_numTrailParticles;
        }
        
        //Trails with negative lifetimes live forever so only decrement if
        //the time to live is more than 1
        if(pCurrTrail->timeToLive >= 0.0f)
        {
            pCurrTrail->timeToLive -= timeElapsed;
            if (/*pCurrTrail->m_numTrailParticles == 0 && */pCurrTrail->timeToLive < 0.0f)
            {
                pCurrTrail->inUse = 0;
                if(pCurrTrail->pCallbackTrailPointer != NULL)
                {
                    *pCurrTrail->pCallbackTrailPointer = NULL;   //TODO: if this ever gets threaded WATCH OUT
                }
            }
        }
        
        //If the trail is still good
        if(pCurrTrail->inUse)
        {
            m_renderableTrails[m_numRenderableTrails] = pCurrTrail;
            ++m_numRenderableTrails;
#if DEBUG_DRAW && DEBUG_DRAW_TRAIL_LINES
            
            for (s32 i=1; i<pCurrTrail->m_numTrailParticles; ++i)
            {
                vec4 color;
                color.w = 1.0f;
                const vec3 color0 = {1.0f,0.0f,0.0f};
                const vec3 color1 = {1.0f,1.0f,0.0f};
                LerpVec3((vec3*)&color,&color1,&color0,pCurrTrail->trailParticles[i].timeToLiveCurr/pCurrTrail->trailParticles[i].timeToLiveStart);
                
                [self DEBUGDRAW3D_DrawLineSegment:&pCurrTrail->trailParticles[i].position:&pCurrTrail->trailParticles[i-1].position:&color];
            }
            
            vec4 color;
            color.w = 1.0f;
            const vec3 color0 = {1.0f,0.0f,0.0f};
            const vec3 color1 = {1.0f,1.0f,0.0f};
            LerpVec3((vec3*)&color,&color1,&color0,pCurrTrail->trailParticles[pCurrTrail->m_numTrailParticles-1].timeToLiveCurr/pCurrTrail->trailParticles[pCurrTrail->m_numTrailParticles-1].timeToLiveStart);
            
            [self DEBUGDRAW3D_DrawLineSegment:&pCurrTrail->trailParticles[pCurrTrail->m_numTrailParticles-1].position:&pCurrTrail->currPos:&color];
#endif
        }
    }
}


void OpenGLRenderer::InitRenderableObject3D(RenderableObject3D* renderableObject, ModelData* pModel, RenderMaterial materialID, GLuint* customTexture, mat4f matrix4x4, RenderLayer renderLayer, u32 viewFlags, u32 renderFlags)
{
	if(pModel->modelID == -1)
	{
		printf("INSANE ERROR: You are tried to initialize a renderable with an unregistered model '%s'!  Skipping...\n",pModel->modelName);
		return;
	}

	renderableObject->geom.pModel = pModel;
	renderableObject->geom.materialID = materialID;
	renderableObject->geom.flags = renderFlags|RenderFlag_Initialized;
	renderableObject->geom.renderLayer = renderLayer;
	renderableObject->geom.customTexture0 = customTexture;
	renderableObject->geom.customTexture1 = NULL;
	renderableObject->geom.viewFlags = viewFlags;
	renderableObject->geom.debugName = NULL;
	renderableObject->geom.postRenderLayerSortValue = 0;

	for (s32 i=0; i<MAX_UNIQUE_UNIFORM_VALUES; ++i)
	{
		renderableObject->geom.uniqueUniformValues[i] = NULL;
	}
	
	if(matrix4x4 == NULL)
	{
		mat4f_LoadIdentity(renderableObject->worldMat);
	}
	else
	{
		mat4f_Copy(renderableObject->worldMat,matrix4x4);
	}
	
	renderableObject->geom.pWorldMat = renderableObject->worldMat;
	
	//You just changed this thing's material so resort
	m_renderableObject3DsNeedSorting = true;
}


void OpenGLRenderer::SortRenderableGeometry3DList()
{
	m_numCellshadedRenderables = 0;
    
	//Create sort values
	for(u32 i=0; i<m_numRenderableGeom3Ds; ++i)
	{
		//I got 32 bits to use up
		m_renderableGeometry3DList[i]->sortValue = 0;
		
		//Going to consider 0 to be the most significant bit for ease of reading
		GU_InsertPositiveValueAsBits(&m_renderableGeometry3DList[i]->sortValue,m_renderableGeometry3DList[i]->renderLayer,0,4);
        
        GU_InsertPositiveValueAsBits(&m_renderableGeometry3DList[i]->sortValue,m_renderableGeometry3DList[i]->postRenderLayerSortValue,4,5);
		
		//printf("SortValue: %d\n",m_renderableObject3DList[i]->postRenderLayerSortValue);
        
		GU_InsertPositiveValueAsBits(&m_renderableGeometry3DList[i]->sortValue,m_renderableGeometry3DList[i]->pModel->modelID,9,5);
        
		GU_InsertPositiveValueAsBits(&m_renderableGeometry3DList[i]->sortValue,m_renderableGeometry3DList[i]->materialID,14,5);
        
        const u32 texture = m_renderableGeometry3DList[i]->customTexture0?*m_renderableGeometry3DList[i]->customTexture0:0;
		
		GU_InsertPositiveValueAsBits(&m_renderableGeometry3DList[i]->sortValue,texture,19,5);
        
#if ENABLE_OUTLINE_RENDERING
        if(m_renderableObject3DList[i]->flags & RenderFlag_HasOutline)
        {
            m_cellshadedRenderableObject3DList[m_numCellshadedRenderables++] = m_renderableObject3DList[i];
        }
#endif
	}
	
	//Sort by RenderLayer
	Array_InsertionSort((void**)m_renderableGeometry3DList, m_numRenderableGeom3Ds, RenderableGeometry3DCompare_SortValue);
	
	//SORT UI
	
	//Create sort values
	for(u32 i=0; i<m_numRenderableUIObjects; ++i)
	{
		//I got 32 bits to use up
		m_renderableUIList[i]->sortValue = 0;
		
		//Going to consider 0 to be the most significant bit for ease of reading
		GU_InsertPositiveValueAsBits(&m_renderableUIList[i]->sortValue,m_renderableUIList[i]->renderLayer,0,4);
        
        GU_InsertPositiveValueAsBits(&m_renderableUIList[i]->sortValue,m_renderableUIList[i]->postRenderLayerSortValue,4,5);
		
		//printf("SortValue: %d\n",m_renderableUIList[i]->postRenderLayerSortValue);
        
		GU_InsertPositiveValueAsBits(&m_renderableUIList[i]->sortValue,m_renderableUIList[i]->pModel->modelID,9,5);
        
		GU_InsertPositiveValueAsBits(&m_renderableUIList[i]->sortValue,m_renderableUIList[i]->materialID,14,5);
        
        const u32 texture = m_renderableUIList[i]->customTexture0?*m_renderableUIList[i]->customTexture0:0;
		
		GU_InsertPositiveValueAsBits(&m_renderableUIList[i]->sortValue,texture,19,5);
	}
	
	//Sort by RenderLayer
	Array_InsertionSort((void**)m_renderableUIList, m_numRenderableUIObjects, RenderableGeometry3DCompare_SortValue);
}


const char* OpenGLRenderer::GetPathToFile(const char* filename)
{
#if defined PLATFORM_OSX || defined PLATFORM_IOS
	NSString* fileString = [NSString stringWithCString:filename encoding:NSUTF8StringEncoding];
	NSString* path = [[NSBundle mainBundle] pathForResource:fileString ofType:@""];
	
	return [path UTF8String];
#else
	return filename;
#endif
}


s32 OpenGLRenderer::AddVertexShaderToList(const char* filename)
{
	if(m_numVertexShaders < MAX_COMPILED_VERTEX_SHADERS)
	{
		const s32 newIndex = ++m_numVertexShaders;
		m_vertexShaders[newIndex].filename = filename;
		m_vertexShaders[newIndex].openGLID = 0;

		return newIndex;
	}
	else
	{
		return -1;
	}
}

s32 OpenGLRenderer::AddPixelShaderToList(const char* filename)
{
	if(m_numPixelShaders < MAX_COMPILED_PIXEL_SHADERS)
	{
		const s32 newIndex = ++m_numPixelShaders;
		m_pixelShaders[newIndex].filename = filename;
		m_pixelShaders[newIndex].openGLID = 0;
		
		return newIndex;
	}
	else
	{
		return -1;
	}
}


void OpenGLRenderer::CreateMaterials()
{
	//Vertex Shaders
	const s32 VSH_WiggleUsingTexcoordV = AddVertexShaderToList("WiggleUsingTexcoordV.vsh");
	const s32 VSH_FullScreenQuad = AddVertexShaderToList("pp_default.vsh");
    const s32 VSH_FullScreenQuadNoTexcoord = AddVertexShaderToList("pp_noTexcoord.vsh");
	const s32 VSH_VertColors = AddVertexShaderToList("VertColors.vsh");
    const s32 VSH_VertWithColorInput = AddVertexShaderToList("VertWithColorInput.vsh");
    const s32 VSH_VertWithColorInputAndTexture = AddVertexShaderToList("VertWithColorInputAndTexture.vsh");
    const s32 VSH_VertWithColorInputWithTexcoordOffset = AddVertexShaderToList("VertWithColorInputWithTexcoordOffset.vsh");
	const s32 VSH_TextureOnly = AddVertexShaderToList("TextureOnly.vsh");
    const s32 VSH_TextureWithColor = AddVertexShaderToList("TextureWithColor.vsh");
	const s32 VSH_ScreenspaceWithTexcoordOffset = AddVertexShaderToList("ScreenspaceWithTexcoordOffset.vsh");
	const s32 VSH_TextureOnlyWithTexcoordAndWorldOffset = AddVertexShaderToList("TextureOnlyWithTexcoordAndWorldOffset.vsh");
	const s32 VSH_PointSprite_Basic = AddVertexShaderToList("PointSprite_Default.vsh");
	//const s32 VSH_SkinnedVertShader = AddVertexShaderToList("SkinnedVertShader.vsh");
	
	//Pixel Shaders
	const s32 PP_BlendUsingTexture = AddPixelShaderToList("BlendUsingTexture.fsh");
	const s32 PP_PureColor = AddPixelShaderToList("PureColor.fsh");
    const s32 PS_TextureOnlySimple = AddPixelShaderToList("TextureOnlySimple.fsh");
    const s32 PS_TextureOnlyDiscard = AddPixelShaderToList("TextureOnlyDiscard.fsh");
    const s32 PS_TextureAndDiffuseColor = AddPixelShaderToList("TextureAndDiffuseColor.fsh");
    const s32 PS_TextureAndDiffuseColorDiscard = AddPixelShaderToList("TextureAndDiffuseColorDiscard.fsh");
	const s32 PS_TextureAndFogColorDiscard = AddPixelShaderToList("TextureAndFogColorDiscard.fsh");
	const s32 PS_Colors = AddPixelShaderToList("Colors.fsh");
	const s32 PS_PointSprite_ColorShine = AddPixelShaderToList("PointSprite_ColorShine.fsh");
    const s32 PS_TextureWithColor = AddPixelShaderToList("TextureWithColor.fsh");
	//const s32 PS_SkinnedFragShader = AddPixelShaderToList("SkinnedFragShader.fsh");
	
	
	//Vertex Shaders
	
	//Setup common to all materials
	for(int i=0; i<NumRenderMaterials; ++i)
	{
		//nil the textures
		g_Materials[i].texture0 = NULL;
		g_Materials[i].texture1 = NULL;
		g_Materials[i].texture2 = NULL;
		
		//Zero out uniforms
		g_Materials[i].numUniforms_shared = 0;
        g_Materials[i].numUniforms_shared_const = 0;
		g_Materials[i].numUniforms_unique = 0;
		
		g_Materials[i].materialName = g_MaterialNames[i];
	}
	
	//-- Create post process materials --
	
	const AttributeFlags ppAttribs = (AttributeFlags) (ATTRIBFLAG_VERTEX);
    
	//PPMT_BlendUsingTexture
	if(CreateShaderProgram(VSH_FullScreenQuad,PP_BlendUsingTexture,ppAttribs,&g_Materials[PPMT_BlendUsingTexture].shaderProgram))
	{
		
	}
	
	//PPMT_PureColor
	if(!CreateShaderProgram(VSH_FullScreenQuadNoTexcoord,PP_PureColor,ppAttribs,&g_Materials[PPMT_PureColor].shaderProgram))
	{
		AddUniform_Shared(PPMT_PureColor,"color",Uniform_Vec4,&m_fadeColor.x,1);
	}
	
	/*attribute highp   vec3 inVertex;
	 attribute mediump vec3 inNormal;
	 attribute mediump vec3 inTangent;
	 attribute mediump vec3 inBiNormal;
	 attribute mediump vec2 inTexCoord;
	 attribute mediump vec4 inBoneIndex;
	 attribute mediump vec4 inBoneWeights;
	 */
	
    const AttributeFlags attribs_V = (AttributeFlags) (ATTRIBFLAG_VERTEX);
	const AttributeFlags attribs_VT = (AttributeFlags) (ATTRIBFLAG_VERTEX | ATTRIBFLAG_TEXCOORD);
	const AttributeFlags attribs_VTC = AttributeFlags (ATTRIBFLAG_VERTEX | ATTRIBFLAG_TEXCOORD | ATTRIBFLAG_COLOR);
    const AttributeFlags attribs_VC = (AttributeFlags) (ATTRIBFLAG_VERTEX | ATTRIBFLAG_COLOR);
	//const AttributeFlags attribs_skinned = (AttributeFlags) (ATTRIBFLAG_VERTEX | ATTRIBFLAG_NORMAL | ATTRIBFLAG_TANGENT | ATTRIBFLAG_BINORMAL | ATTRIBFLAG_TEXCOORD | ATTRIBFLAG_BONEINDEX | ATTRIBFLAG_BONEWEIGHT);
	
    //MT_VertColors
	//TODO: ppAttribs... what?

	if(CreateShaderProgram(VSH_VertColors,PS_Colors,attribs_VC,&g_Materials[MT_VertColors].shaderProgram))
	{
		
	}
	
	//MT_SkinnedWithNormalMapping
    /*if(CreateShaderProgram(VSH_SkinnedVertShader,PS_SkinnedFragShader,attribs_skinned,&g_Materials[MT_SkinnedWithNormalMapping].shaderProgram))
    {
        //AddUniform_Unique(MT_SkinnedWithNormalMapping,"inputColor",Uniform_Vec4,1);
    }*/
    
    //MT_VertWithColorInput
    if(CreateShaderProgram(VSH_VertWithColorInput,PS_Colors,attribs_V,&g_Materials[MT_VertWithColorInput].shaderProgram))
    {
        AddUniform_Unique(MT_VertWithColorInput,"inputColor",Uniform_Vec4,1);
    }
	
	//MT_BasicPointSprite
	if(CreateShaderProgram(VSH_PointSprite_Basic,PS_TextureWithColor,attribs_VTC,&g_Materials[MT_BasicPointSprite].shaderProgram))
	{
		g_Materials[MT_BasicPointSprite].texture0 = &texture_pointSpriteAtlas;
	}
	
	//MT_PointSpriteColorShine
	if(CreateShaderProgram(VSH_PointSprite_Basic,PS_PointSprite_ColorShine,attribs_VTC,&g_Materials[MT_PointSpriteColorShine].shaderProgram))
	{
		g_Materials[MT_PointSpriteColorShine].texture0 = &texture_pointSpriteAtlas;
	}
	
	//MT_WiggleUsingTexcoordV
	if(CreateShaderProgram(VSH_WiggleUsingTexcoordV,PS_TextureAndFogColorDiscard,attribs_VT,&g_Materials[MT_WiggleUsingTexcoordV].shaderProgram))
	{
		AddUniform_Unique(MT_WiggleUsingTexcoordV,"inputColor",Uniform_Vec4,1);
		AddUniform_Unique(MT_WiggleUsingTexcoordV,"wiggleAmount",Uniform_Vec4,1);
	}

	//MT_TextureOnlySimple
	if(CreateShaderProgram(VSH_TextureOnly,PS_TextureOnlySimple,attribs_VT,&g_Materials[MT_TextureOnlySimple].shaderProgram))
	{
		
	}
    
    //MT_TextureOnlyDiscard
	if(CreateShaderProgram(VSH_TextureOnly,PS_TextureOnlyDiscard,attribs_VT,&g_Materials[MT_TextureOnlyDiscard].shaderProgram))
	{
		
	}

    //MT_TextureAndDiffuseColorDiscard
	if(CreateShaderProgram(VSH_VertWithColorInputAndTexture,PS_TextureAndDiffuseColorDiscard,attribs_VT,&g_Materials[MT_TextureAndDiffuseColorDiscard].shaderProgram))
	{
		AddUniform_Unique(MT_TextureAndDiffuseColorDiscard,"inputColor",Uniform_Vec4,1);
	}

	
	//MT_TextureAndFogColorDiscard
	if(CreateShaderProgram(VSH_VertWithColorInputAndTexture,PS_TextureAndFogColorDiscard,attribs_VT,&g_Materials[MT_TextureAndFogColorDiscard].shaderProgram))
	{
		AddUniform_Unique(MT_TextureAndFogColorDiscard,"inputColor",Uniform_Vec4,1);
	}

    //MT_TextureAndDiffuseColor
	if(CreateShaderProgram(VSH_VertWithColorInputAndTexture,PS_TextureAndDiffuseColor,attribs_VT,&g_Materials[MT_TextureAndDiffuseColor].shaderProgram))
	{
		AddUniform_Unique(MT_TextureAndDiffuseColor,"inputColor",Uniform_Vec4,1);
	}
    
    //MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard
	if(CreateShaderProgram(VSH_VertWithColorInputWithTexcoordOffset,PS_TextureAndDiffuseColorDiscard,attribs_VT,&g_Materials[MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard].shaderProgram))
	{
		AddUniform_Unique(MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard,"texCoordOffset",Uniform_Vec2,1);
		AddUniform_Unique(MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard,"inputColor",Uniform_Vec4,1);
	}
    
    //MT_TextureWithColor
	if(CreateShaderProgram(VSH_TextureWithColor,PS_TextureWithColor,attribs_VTC,&g_Materials[MT_TextureWithColor].shaderProgram))
	{
		
	}
	
	//MT_Animation_NewMethod3D
	if(CreateShaderProgram(VSH_TextureOnlyWithTexcoordAndWorldOffset,PS_TextureOnlySimple,attribs_VT,&g_Materials[MT_Animation_NewMethod3D].shaderProgram))
	{
		AddUniform_Unique(MT_Animation_NewMethod3D,"texCoordOffset",Uniform_Vec2,1);
	}

	//MT_TextureOnlyWithTexcoordOffset
	if(CreateShaderProgram(VSH_ScreenspaceWithTexcoordOffset,PS_TextureOnlySimple,attribs_VT,&g_Materials[MT_TextureOnlyWithTexcoordOffset].shaderProgram))
	{
		AddUniform_Unique(MT_TextureOnlyWithTexcoordOffset,"texCoordOffset",Uniform_Vec2,1);
	}

    
    //MT_TextureOnlyWithTexcoordOffsetDiscard
	if(CreateShaderProgram(VSH_ScreenspaceWithTexcoordOffset,PS_TextureOnlyDiscard,attribs_VT,&g_Materials[MT_TextureOnlyWithTexcoordOffsetDiscard].shaderProgram))
	{
		AddUniform_Unique(MT_TextureOnlyWithTexcoordOffsetDiscard,"texCoordOffset",Uniform_Vec2,1);
	}

	//Setup common to all materials
	//TODO: Don't do this
	for(int i=0; i<NumRenderMaterials; ++i)
	{
		
		g_Materials[i].uniform_worldViewProjMat = glGetUniformLocation(g_Materials[i].shaderProgram, "worldViewProjMat");
		g_Materials[i].uniform_camPos = glGetUniformLocation(g_Materials[i].shaderProgram, "camPos");
	}

}


void OpenGLRenderer::SetViewMatrixForIndex(f32* pCameraMatrix, u32 viewIndex)
{
	m_view[viewIndex] = pCameraMatrix;
}


void OpenGLRenderer::SetViewPos(vec3* pCamPos, u32 viewIndex)
{
	CopyVec3(&m_camPos[viewIndex], pCamPos);
}


f32* OpenGLRenderer::GetCameraMatrix(u32 viewIndex)
{
	return m_view[viewIndex];
}


void OpenGLRenderer::StartFadeOut(f32 timeInSeconds)
{
	m_fadeState = FadeState_FadeOut;
	m_totalFadeTime = timeInSeconds*0.5f;
	m_currFadeTime = 0.0f;
}


void OpenGLRenderer::ForceFadeIn(f32 timeInSeconds)
{
	m_fadeState = FadeState_WaitingForFadeIn;
    m_requestedFadeIn = true;
    
    m_totalFadeTime = timeInSeconds;
	m_currFadeTime = timeInSeconds;
}


void OpenGLRenderer::StartFadeIn()
{
	m_requestedFadeIn = true;
}


void OpenGLRenderer::StartPauseFadeIn()
{
	 m_pauseRequestedFadeIn = true;
}


void OpenGLRenderer::StartPauseFadeOut(f32 finalFade, f32 timeInSeconds)
{
	m_pauseFadeState = FadeState_FadeOut;
	m_pauseTotalFadeTime = timeInSeconds*0.5f;
	m_pauseCurrFadeTime = 0.0f;
    m_pauseFinalFade = 1.0f-finalFade;
}


void OpenGLRenderer::ResetPauseFade()
{
	m_pauseFadeState = FadeState_Idle;
}


void OpenGLRenderer::ClearParticles()
{
	//Clear normal particles
	for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
	{
		RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
		
		pBucket->m_particlesNeedSorting = false;
		pBucket->m_numParticles = 0;
        pBucket->m_numParticlesToDraw = 0;
		
		for (s32 i=0; i<MAX_PARTICLES; ++i)
		{
			pBucket->m_particleQueue[i].timeToLive = -1.0f;
            pBucket->m_particleQueue[i].pCallbackCounter = NULL;
            pBucket->m_particleQueue[i].pCallbackPos = NULL;
		}
	}
    
    m_numRenderableTrails = 0;
    
    //Clear trails
    for(int i=0; i<MAX_TRAILS; ++i)
    {
        m_trails[i].m_numTrailParticles = 0;
        m_trails[i].inUse = 0;
        m_trails[i].pCallbackTrailPointer = 0;
    }
}


void OpenGLRenderer::SetScreenFadeColor(vec3* screenFadeColor)
{
	CopyVec3(&m_fadeToScreenColor,screenFadeColor);
}


void OpenGLRenderer::ForceRenderablesNeedSorting()
{
	m_renderableObject3DsNeedSorting = true;
}


void OpenGLRenderer::SortRenderablesWithMaterialByZ(RenderMaterial materialID)
{
	for (u32 i=0; i<m_numRenderableGeom3Ds; ++i)
	{
		const RenderableGeometry3D* pCurrRenderable = m_renderableGeometry3DList[i];
		if (pCurrRenderable->materialID == materialID)
		{
			u32 startIDX = i;
			while (i<m_numRenderableGeom3Ds && m_renderableGeometry3DList[i]->materialID == materialID)
			{
				++i;
			}
			
			Array_InsertionSort((void**)&m_renderableGeometry3DList[startIDX], (i-startIDX), RenderableGeometry3DCompare_SortByZ);
		}
	}
}


void OpenGLRenderer::SortRenderablesInLayerRangeByZ(RenderLayer layerBegin, RenderLayer layerEnd)
{
	//TODO: make this a little better.  It's a rushed version. :)
	for (u32 i=0; i<m_numRenderableGeom3Ds; ++i)
	{
		const RenderableGeometry3D* pCurrRenderable = m_renderableGeometry3DList[i];
        const bool isInLayer = pCurrRenderable->renderLayer >= layerBegin && pCurrRenderable->renderLayer <= layerEnd;
		if (isInLayer)
		{
			u32 startIDX = i;
            
			while (i<m_numRenderableGeom3Ds && m_renderableGeometry3DList[i]->renderLayer >= layerBegin && m_renderableGeometry3DList[i]->renderLayer <= layerEnd)
			{
				++i;
			}
			
			Array_InsertionSort((void**)&m_renderableGeometry3DList[startIDX], (i-startIDX), RenderableGeometry3DCompare_SortByNegativeZ);
		}
	}
}


void OpenGLRenderer::FlashScreen(const vec3* pColor, f32 timeInSeconds)
{
	CopyVec3(&m_flashColor, pColor);
    m_flashState = FadeState_FadeOut;
	m_totalFlashTime = timeInSeconds*0.5f;
	m_currFlashTime = 0.0f;
}


void OpenGLRenderer::ShakeScreen(f32 shakeAmount,f32 shakeSpeed, f32 shakeTime)
{
	//Shake how we told it to shake
    if (m_screenShakeTimer <= 0.0f)
    {
        m_screenShakeSpeed = shakeSpeed;
		
        m_currScreenShakeAmount = shakeAmount;
        
        m_screenShakeStartTime = shakeTime;
        m_screenShakeTimer = m_screenShakeStartTime;
    }
    //Shake as much as possible
    else
    {
        //If the screen wants to shake harder, use the highest possible shake speed
        if(shakeAmount > m_currScreenShakeAmount)
        {
            m_screenShakeSpeed = MaxF(shakeSpeed,m_screenShakeSpeed);
            
            //Shake is more crazy so use the passed in time
            m_screenShakeStartTime = shakeTime;
        }
        else
        {
            m_screenShakeSpeed = shakeSpeed;
            
            //Shake is less so use the higher of the two times
            m_screenShakeStartTime = MaxF(m_screenShakeStartTime,shakeTime);
        }
        
        m_currScreenShakeAmount = MaxF(m_currScreenShakeAmount,shakeAmount);
        
		
        m_screenShakeTimer = m_screenShakeStartTime;
    }
    
    //TODO: this is a little hardcoded
    const f32 shakeSpeedTFinal = 0.5f+0.5f*m_screenShakeSpeed;
    
    m_screenShakerSpeed_X = shakeSpeedTFinal*50.0f;
    m_screenShakerSpeed_Y = shakeSpeedTFinal*100.0f;
}

void OpenGLRenderer::CleanUp()
{
	glDeleteTextures(1,&texture_pointSpriteAtlas);
	glDeleteTextures(1,&texture_default);
}


void OpenGLRenderer::LoadTexture(const char* fileName,ImageType imageType, GLuint* pGLTexture, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV, bool flipY)
{
	//You're not allowed to load different textures into one location
    //You're not allowed to load into NULL
    if (pGLTexture == NULL || *pGLTexture != 0)
    {       
        return;
    }
	
    switch (imageType)
    {
		case ImageType_TGA:
		{
			*pGLTexture = SOIL_load_OGL_texture
			(
			 GetPathToFile(fileName),
			 SOIL_LOAD_AUTO,
			 SOIL_CREATE_NEW_ID,
			 SOIL_FLAG_INVERT_Y
			 );
			
			break;
		}
        case ImageType_PNG:
        {
			int width,height;
			bool hasAlpha;
			GLubyte *textureImage;
			
			const bool success = LoadPngImage(GetPathToFile(fileName), width, height, hasAlpha, &textureImage, flipY);
			
			if(success)
			{
				//NEW CODE
				glGenTextures(1, pGLTexture);
				glBindTexture(GL_TEXTURE_2D, *pGLTexture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filterMode); 
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filterMode);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeV);
				
				GLint type = hasAlpha?GL_RGBA:GL_RGB;
				
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, type, width,height, 0, type, GL_UNSIGNED_BYTE, textureImage);
				
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				printf("INSANE ERROR: LoadTexture failed to load the file: %s\n",fileName);
			}
            
            break;
        }   
        case ImageType_PVR:
        {
            //[self loadTextureFromFile:pGLTexture :fileName :filterMode: wrapModeU: wrapModeV];
            
            break;
        }
    }
}


GFX_Trail* OpenGLRenderer::CreateTrail(GFX_Trail** pCallbackTrail, vec3* pInitialPos, f32 timeToLive, const GFX_TrailSettings* pTrailSettings, u32 renderFlags)
{
	for(int i=0; i<MAX_TRAILS; ++i)
    {
        if (m_trails[i].inUse == 0)
        {
            m_trails[i].inUse = 1;
            m_trails[i].trailSettings = *pTrailSettings;
            m_trails[i].segmentLengthSq = pTrailSettings->segmentLength*pTrailSettings->segmentLength;
            CopyVec3(&m_trails[i].currPos,pInitialPos);
            m_trails[i].timeToLive = timeToLive;
            m_trails[i].texture = *pTrailSettings->texture;
            m_trails[i].pCallbackTrailPointer = (void**)pCallbackTrail;
            SetVec4(&m_trails[i].diffuseColor,1.0f,1.0f,1.0f,1.0f);
			
            for (s32 j=1; j<MAX_TRAIL_PARTICLES_PER_TRAIL; ++j)
            {
                m_trails[i].trailParticles[j].timeToLiveCurr = -1.0f;
            }
            
            //set up first particle
            CopyVec3(&m_trails[i].trailParticles[0].position,pInitialPos);
            m_trails[i].trailParticles[0].timeToLiveStart = pTrailSettings->timeToLivePerParticle;
            m_trails[i].trailParticles[0].timeToLiveCurr = pTrailSettings->timeToLivePerParticle;
            m_trails[i].m_numTrailParticles = 1;
            m_trails[i].renderFlags = renderFlags;
			
			
            return &m_trails[i];
        }
    }
    
    return NULL;
}


void OpenGLRenderer::DRAW_DrawTexturedLine(const vec3* p0, const vec3* p1, const vec4* pDiffuseColor, GLuint texturedID, f32 lineWidth0, f32 lineWidth1, f32 numTextureRepeats)
{
	if(m_numTexturedLines == MAX_TEXTURED_LINES)
	{
		return;
	}
	
	TexturedLineObject* pNextLine = &m_texturedLineObjects[m_numTexturedLines];
	CopyVec3(&pNextLine->line.p0,p0);
	CopyVec3(&pNextLine->line.p1,p1);
	pNextLine->texturedID = texturedID;
	pNextLine->lineWidth0 = lineWidth0;
	pNextLine->lineWidth1 = lineWidth1;
	pNextLine->numTextureRepeats = numTextureRepeats;
	CopyVec4(&pNextLine->diffuseColor,pDiffuseColor);
	
	++m_numTexturedLines;
}


void OpenGLRenderer::DEBUGDRAW2D_DrawLineSegment(const vec3* p0, const vec3* p1, const vec4* color)
{
	//TODO: put safety check in here
    CopyVec3(&m_debugLinePoints2D[m_numDebugLinePoints2D].position,p0);
    CopyVec4(&m_debugLinePoints2D[m_numDebugLinePoints2D].color,color);
    
    CopyVec3(&m_debugLinePoints2D[m_numDebugLinePoints2D+1].position,p1);
    CopyVec4(&m_debugLinePoints2D[m_numDebugLinePoints2D+1].color,color);
    
    m_numDebugLinePoints2D += 2;
}


void OpenGLRenderer::DEBUGDRAW2D_DrawCircle(const vec3* p0, f32 radius, const vec4* color)
{
#if DEBUG_DRAW
    const f32 angleInc = PI_2/16.0f;
    f32 currAngle = angleInc;
    
    vec3 lastPos;
    CopyVec3(&lastPos, p0);
    lastPos.x += radius;
    
	
    for (u32 i=0; i<32; ++i)
    {
        vec2 rotatedVec2;
        MakeRotatedVec2(&rotatedVec2,currAngle);
		
        vec3 finalPos;
        AddScaledVec2((vec2*)&finalPos, (const vec2*)p0, (const vec2*)&rotatedVec2, radius);
        finalPos.z = 0.0f;
        
        CopyVec3(&m_debugLinePoints2D[m_numDebugLinePoints2D].position,&lastPos);
        CopyVec4(&m_debugLinePoints2D[m_numDebugLinePoints2D].color,color);
        ++m_numDebugLinePoints2D;
        
        CopyVec3(&m_debugLinePoints2D[m_numDebugLinePoints2D].position,&finalPos);
        CopyVec4(&m_debugLinePoints2D[m_numDebugLinePoints2D].color,color);
        ++m_numDebugLinePoints2D;
        
        CopyVec3(&lastPos, &finalPos);
        
        currAngle += angleInc;
    }
#endif

}


void OpenGLRenderer::DEBUGDRAW3D_DrawLineSegment(const vec3* p0, const vec3* p1, const vec4* color)
{
	//TODO: put safety check in here
    CopyVec3(&m_debugLinePoints3D[m_numDebugLinePoints3D].position,p0);
    CopyVec4(&m_debugLinePoints3D[m_numDebugLinePoints3D].color,color);
    
    CopyVec3(&m_debugLinePoints3D[m_numDebugLinePoints3D+1].position,p1);
    CopyVec4(&m_debugLinePoints3D[m_numDebugLinePoints3D+1].color,color);
    
    m_numDebugLinePoints3D += 2;
}


void OpenGLRenderer::DEBUGDRAW3D_DrawLineSegment_2Color(const vec3* p0, const vec3* p1, const vec4* color1, const vec4* color2)
{
	//TODO: put safety check in here
    CopyVec3(&m_debugLinePoints3D[m_numDebugLinePoints3D].position,p0);
    CopyVec4(&m_debugLinePoints3D[m_numDebugLinePoints3D].color,color1);
    
    CopyVec3(&m_debugLinePoints3D[m_numDebugLinePoints3D+1].position,p1);
    CopyVec4(&m_debugLinePoints3D[m_numDebugLinePoints3D+1].color,color2);
    
    m_numDebugLinePoints3D += 2;
}


void OpenGLRenderer::DEBUGDRAW3D_DrawCircle(mat4f matrix4x4, const vec4* color)
{
	if(m_numDebugDrawObjects == DEBUGDRAW_MAXDEBUGOBJECTS)
	{
		return;
	}
	
    DebugDrawObject* pObject = &m_debugDrawObjects[m_numDebugDrawObjects];
    
    mat4f_Copy(pObject->matrix, matrix4x4);
    CopyVec4(&pObject->color,color);
    pObject->objectType = DebugDrawObjectType_Circle;
    
    ++m_numDebugDrawObjects;
}


void OpenGLRenderer::DEBUGDRAW3D_DrawCylinder(mat4f matrix4x4, const vec4* color)
{
	if(m_numDebugDrawObjects == DEBUGDRAW_MAXDEBUGOBJECTS)
	{
		return;
	}
	
    DebugDrawObject* pObject = &m_debugDrawObjects[m_numDebugDrawObjects];
    
    mat4f_Copy(pObject->matrix, matrix4x4);
    CopyVec4(&pObject->color,color);
    pObject->objectType = DebugDrawObjectType_Cylinder;
    
    ++m_numDebugDrawObjects;
}


void OpenGLRenderer::SetMaterial(RenderMaterial material)
{
	//Enable the shader
	const Material* currMaterial = &g_Materials[material];
	const GLuint program = currMaterial->shaderProgram;
	glUseProgram(program);
	
	//Upload camPos (TODO: don't put this here)
	glUniform3f(currMaterial->uniform_camPos, m_camPos[m_currViewIndex].x,m_camPos[m_currViewIndex].y,m_camPos[m_currViewIndex].z);
	
	//Set the textures
	SetTexture(currMaterial->texture0,0);
    SetTexture(currMaterial->texture1,1);
    SetTexture(currMaterial->texture2,2);
	
	m_lastUsedMaterial = material;
	
	UploadSharedUniforms();
	
#ifdef DEBUG_RENDERLOOP
	//printf("Changing material: %s\n",g_MaterialNames[material]);
#endif
}


void Draw_FSQuad()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
    
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);
	
    //Perform post processing by rendering a full screen quad with the given input texture
	//es2time1=CACurrentMediaTime();
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//es2time2=CACurrentMediaTime();
}


void Draw_TLQuad()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVerticesTL);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	
    //Perform post processing by rendering a full screen quad with the given input texture
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Draw_TRQuad()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVerticesTR);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	
    //Perform post processing by rendering a full screen quad with the given input texture
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Draw_BLQuad()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVerticesBL);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	
    //Perform post processing by rendering a full screen quad with the given input texture
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Draw_BRQuad()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVerticesBR);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	
    //Perform post processing by rendering a full screen quad with the given input texture
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Draw_Matrix()
{
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, matrixVertices);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, 0, 0, matrixColors);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	
    glDrawArrays(GL_LINES, 0, 6);
}

void OpenGLRenderer::PostProcess(RenderMaterial ppMaterial, RenderTarget* renderTarget, PostProcessDrawArea drawArea, GLuint* texture0, GLuint* texture1, GLuint* texture2)
{
	//TODO: comment this back in later when we want to use post processing
	//[self setRenderTarget:renderTarget];
	
	//Set up textures to be used
	g_Materials[ppMaterial].texture0 = texture0;
	g_Materials[ppMaterial].texture1 = texture1;
	g_Materials[ppMaterial].texture2 = texture2;
	
	//Set material like normal
	SetMaterial(ppMaterial);
	UploadProjMatrix(m_projMats[ProjMatType_Orthographic_NDC]);
	
	//Upload uniforms for this post process
	//TODO: remove this later if there are some that share the same uniforms
	UploadSharedUniforms();
	
	//Draw a full screen quad
	switch (drawArea) {
		case PPDrawArea_FullScreen:
		{
			Draw_FSQuad();
			break;
		}
		case PPDrawArea_TopLeft:
		{
			Draw_TLQuad();
			break;
		}
		case PPDrawArea_TopRight:
		{
			Draw_TRQuad();
			break;
		}
		case PPDrawArea_BottomLeft:
		{
			Draw_BLQuad();
			break;
		}
		case PPDrawArea_BottomRight:
		{
			Draw_BRQuad();
			break;
		}
		default:
		{
			//NSLog(@"Error: Unsupported PostProcessDrawArea selected.\n");
		}
			
	}
}


void OpenGLRenderer::PrintOpenGLError(const char* callerName)
{
	/*int iErr = glGetError();
	if (iErr != GL_NO_ERROR)
	{
		printf("GL error: %d (0x%x)\n\n", iErr, iErr);
	}*/
	
#if defined PLATFORM_OSX 
	GLenum errCode;
	const GLubyte *errString;
	
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf (stderr, "OpenGL Error: %s\n", errString);
	}
#endif
}


bool OpenGLRenderer::CreateRenderTarget(RenderTarget* renderTarget, GLuint FBO, GLuint width, GLuint height)
{
	return false;
}


//Just an OpenGL example from Apple I might care about later
/*- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(_program, ATTRIB_NORMAL, "normal");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    
    return YES;
}*/


//NOTE: You are responsible for deleting this memory
char* FileToCharArray(const char* filename)
{
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        return NULL;
    }
	
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);
	
    char* content = (char*)calloc(size + 1, 1);
	
    fread(content,1,size,file);
	
	fclose(file);
	
    return content;
}


/* Create and compile a shader from the provided source(s) */
bool OpenGLRenderer::CompileShader(GLuint *shader, GLenum type, GLsizei count, const char* filename)
{
	GLint status;
   
    const GLchar* source = FileToCharArray(filename);
	
    if (!source)
	{
		printf("ERROR: CompileShader-> Failed to load shader file: %s\n",filename);
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
	
	//Release the source code!
	free((void*)source);

    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
	{
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        printf("Compile log for %s: %s\n",filename,log);
        free(log);
    }

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}


/* Link a program with all currently attached shaders */
bool OpenGLRenderer::LinkProgram(GLuint prog)
{
	GLint status;
    glLinkProgram(prog);
    
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        printf("Program link log: %s\n",log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}


/* Validate a program (for i.e. inconsistent samplers) */
GLint OpenGLRenderer::ValidateProgram(GLuint prog)
{
	GLint logLength, status;
	
	glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength+1);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
		NSLog(@"Failed to validate program %d", prog);
	
	return status;
}


bool OpenGLRenderer::CreateShaderProgram(s32 vertexShaderIndex, s32 pixelShaderIndex,  AttributeFlags attribs, GLuint* out_resultProgram)
{
	PrintOpenGLError("Before creating shader program.");
	
	//ALog(@"Creating Shader: VSH:'%@' + FSH:'%@'\n",g_VertexShader_Filenames[vertexShader],g_PixelShader_Filenames[pixelShader]);

	// create shader program
	const GLuint shaderProgram = glCreateProgram();

	if (!shaderProgram)
	{
		return false;
	}

	// create and compile vertex shader
	if (!CreateVertexShader(vertexShaderIndex))
	{
		//NSLog(@"ERROR: createShaderProgram -> failed to create vertex shader '%@'.\n",g_VertexShader_Filenames[vertexShader]);
		glDeleteProgram(shaderProgram);
		
		return false;
	}


	// create and compile pixel shader
	if (!CreatePixelShader(pixelShaderIndex))
	{
		//NSLog(@"ERROR: createShaderProgram -> failed to create pixel shader '%@'.\n",g_PixelShader_Filenames[pixelShader]);
		glDeleteProgram(shaderProgram);
		
		return false;
	}

	// attach vertex shader to program
	glAttachShader(shaderProgram, m_vertexShaders[vertexShaderIndex].openGLID);

	// attach pixel shader to program
	glAttachShader(shaderProgram, m_pixelShaders[pixelShaderIndex].openGLID);

	// bind attribute locations
	// this needs to be done prior to linking

	PrintOpenGLError("Before binding attributes.");
	
	if (attribs & ATTRIBFLAG_VERTEX)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_VERTEX, "in_position");
	}
	if (attribs & ATTRIBFLAG_VERTEX2)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_VERTEX2, "in_position2");
	}
	if (attribs & ATTRIBFLAG_NORMAL)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_NORMAL, "in_normal");
	}
	if (attribs & ATTRIBFLAG_NORMAL2)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_NORMAL2, "in_normal2");
	}
	if (attribs & ATTRIBFLAG_COLOR)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_COLOR, "in_color");
	}
	if (attribs & ATTRIBFLAG_TEXCOORD)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_TEXCOORD, "in_texcoord");
	}
	if (attribs & ATTRIBFLAG_TEXCOORD2)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_TEXCOORD2, "in_texcoord2");
	}
	if (attribs & ATTRIBFLAG_TANGENT)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_TANGENT, "in_tangent");
	}
	if (attribs & ATTRIBFLAG_BINORMAL)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_BINORMAL, "in_binormal");
	}
	if (attribs & ATTRIBFLAG_BONEINDEX)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_BINORMAL, "in_boneindex");
	}
	if (attribs & ATTRIBFLAG_BONEWEIGHT)
	{
		glBindAttribLocation(shaderProgram, ATTRIB_BINORMAL, "in_boneweight");
	}
	

	PrintOpenGLError("After binding attributes.");

	// link program
	if (!LinkProgram(shaderProgram))
	{	
		//NSLog(@"ERROR: createShaderProgram -> failed to link program ('%@.vsh' + '%@.fsh')\n",g_VertexShader_Filenames[vertexShader],g_PixelShader_Filenames[pixelShader]);
		
		GLint logLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char* log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
			printf("Reason: %s\n\n", log);
			free(log);
		}
		
		return false;
	}

	if (out_resultProgram != NULL)
	{
		*out_resultProgram = shaderProgram;
	}

	PrintOpenGLError("Before texture uniforms.");
	
	glUseProgram(shaderProgram);
	
	//TODO: make this only set up as many as the material uses (if it even matters)
	GLuint uniform_texture0 = glGetUniformLocation(shaderProgram,"texture0");
	GLuint uniform_texture1 = glGetUniformLocation(shaderProgram,"texture1");
	GLuint uniform_texture2 = glGetUniformLocation(shaderProgram,"texture2");

	glUniform1i(uniform_texture0,0);
	glUniform1i(uniform_texture1,1);
	glUniform1i(uniform_texture2,2);

	PrintOpenGLError("After creating shader program.");
	
	return true;
}


void OpenGLRenderer::SetRenderTarget(RenderTarget* renderTarget)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);
	
	if(renderTarget == &m_screenTarget)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, renderTarget->texture);
		
		//Attach the render buffer to the frame buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderTarget->texture);		
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, renderTarget->texture);
		
		//Attach the texture to the frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget->texture, 0);
	}
	
	glViewport(0, 0, renderTarget->width, renderTarget->height);
}


void OpenGLRenderer::UploadWorldViewProjMatrix(const f32* pWorldMat)
{
	f32 worldViewProj[16];
	mat4f_Multiply(worldViewProj, m_viewProjMats[m_currViewIndex][m_currProjMatType], pWorldMat);
	
	const Material* currMaterial = &g_Materials[m_lastUsedMaterial];
	glUniformMatrix4fv(currMaterial->uniform_worldViewProjMat, 1, GL_FALSE, worldViewProj);
} //only call after setMaterial


void OpenGLRenderer::UploadProjMatrix(const f32* pProjMat)
{
	const Material* currMaterial = &g_Materials[m_lastUsedMaterial];
	glUniformMatrix4fv(currMaterial->uniform_worldViewProjMat, 1, GL_FALSE, pProjMat);
} //only call after setMaterial


void OpenGLRenderer::UploadWorldProjMatrix(const f32* pWorldMat)
{
	f32 viewProj[16];
	mat4f_Multiply(viewProj, m_projMats[m_currProjMatType], pWorldMat);
	
	const Material* currMaterial = &g_Materials[m_lastUsedMaterial];
	glUniformMatrix4fv(currMaterial->uniform_worldViewProjMat, 1, GL_FALSE, viewProj);
} //only call after setMaterial


void OpenGLRenderer::UploadSharedUniforms()
{
	const Material* currMaterial = &g_Materials[m_lastUsedMaterial];
	
	const int numUniforms = currMaterial->numUniforms_shared;
	
	for(int i=0; i<numUniforms; ++i)
	{
		const UniformBlob* currUniform = &currMaterial->uniforms_shared[i];
		
		switch(currUniform->value.type)
		{
			case Uniform_Float:
			{
				glUniform1fv(currMaterial->uniforms_shared[i].uniform,currUniform->size,currUniform->value.data);
				break;
			}
			case Uniform_Vec2:
			{
				glUniform2fv(currMaterial->uniforms_shared[i].uniform,currUniform->size,currUniform->value.data);
				break;
			}
			case Uniform_Vec3:
			{
				glUniform3fv(currMaterial->uniforms_shared[i].uniform,currUniform->size,currUniform->value.data);
				break;
			}
			case Uniform_Vec4:
			{
				glUniform4fv(currMaterial->uniforms_shared[i].uniform,currUniform->size,currUniform->value.data);
				break;
			}
            case Uniform_Mat3x3:
			{
				glUniformMatrix3fv(currMaterial->uniforms_shared[i].uniform, currUniform->size, GL_FALSE, currUniform->value.data);
				break;
			}
			case Uniform_Mat4x4:
			{
				glUniformMatrix4fv(currMaterial->uniforms_shared[i].uniform, currUniform->size, GL_FALSE, currUniform->value.data);
				break;
			}
			default:
			{
				//NSLog(@"ERROR: uploadSharedUniforms-> You are uploading a weirdly typed uniform.\n");
				break;
			}
		}
	}
}


void OpenGLRenderer::UploadUniqueUniforms(f32* const * pFloatArray)
{
	const Material* currMaterial = &g_Materials[m_lastUsedMaterial];
	
	const int numUniforms = currMaterial->numUniforms_unique;
	
	for(int i=0; i<numUniforms; ++i)
	{
		const float* pCurrValue = pFloatArray[i];
		const GLint uniformID = currMaterial->uniforms_unique[i];
		
		if (uniformID == -1 || pCurrValue == NULL)
		{
			continue;
		}
		
		const GLsizei uniformSize = currMaterial->uniforms_unique_sizes[i];
		
		switch(currMaterial->uniforms_unique_types[i])
		{
			case Uniform_Float:
			{
				glUniform1fv(uniformID,uniformSize,pCurrValue);
				break;
			}
			case Uniform_Vec2:
			{
				glUniform2fv(uniformID,uniformSize,pCurrValue);
				break;
			}
			case Uniform_Vec3:
			{
				glUniform3fv(uniformID,uniformSize,pCurrValue);
				break;
			}
			case Uniform_Vec4:
			{
				glUniform4fv(uniformID,uniformSize,pCurrValue);
				break;
			}
            case Uniform_Mat3x3:
			{
				glUniformMatrix3fv(uniformID, uniformSize, GL_FALSE, pCurrValue);
				break;
			}
			case Uniform_Mat4x4:
			{
				glUniformMatrix4fv(uniformID, uniformSize, GL_FALSE, pCurrValue);
				break;
			}
			default:
			{
				//NSLog(@"ERROR: uploadSharedUniforms-> You are uploading a weirdly typed uniform.\n");
				break;
			}
		}
	}
}


void OpenGLRenderer::SetTexture(const GLuint* pTexture,GLuint textureUnit)
{
	//If the texture is NULL or the deferenced texture is 0, do nothing
    //else use the deferenced texture
    GLuint textureToUse = (pTexture && *pTexture) ? *pTexture : 0;

	//assert(textureToUse);
	
    if (textureToUse && m_currTextureInTextureUnit[textureUnit] != textureToUse)
    {
        m_currTextureInTextureUnit[textureUnit] = textureToUse;
        
        if(m_currTextureUnit != textureUnit)
        {
            m_currTextureUnit = textureUnit;
            glActiveTexture(GL_TEXTURE0+textureUnit);
        }
        
        glBindTexture(GL_TEXTURE_2D, textureToUse);
    }
}


bool OpenGLRenderer::CreatePixelShader(s32 pixelShaderIndex)
{
	ShaderCreationStatus* pStatus = &m_pixelShaders[pixelShaderIndex];
	
	if(pStatus->openGLID)
	{
		return true;
	}
	
	const char* filePath = GetPathToFile(pStatus->filename);
	
	if (!CompileShader(&pStatus->openGLID, GL_FRAGMENT_SHADER, 1, filePath))
	{
		glDeleteShader(pStatus->openGLID);
		pStatus->openGLID = 0;
		
		return false;
	}

	return true;
}


bool OpenGLRenderer::CreateVertexShader(s32 vertexShaderIndex)
{
	ShaderCreationStatus* pStatus = &m_vertexShaders[vertexShaderIndex];
	
	if(pStatus->openGLID)
	{
		return true;
	}
	
	const char* filePath = GetPathToFile(pStatus->filename);
	
	if (!CompileShader(&pStatus->openGLID, GL_VERTEX_SHADER, 1, filePath))
	{
		glDeleteShader(pStatus->openGLID);
		pStatus->openGLID = 0;
		
		return false;
	}
	
	return true;
}

f32 OpenGLRenderer::ComputeGaussianValue(f32 x, f32 stdDevSq)
{
	// The gaussian equation is defined as such:
	/*    
	 |-(x - mean)^2|
	 |-------------|
	 |		  1.0		  |		| 2*std_dev^2 |
	 f(x,mean,std_dev) = |--------------------| * e^
	 |sqrt(2*pi*std_dev^2)|
	 
	 */
	
	return exp(-(x*x)/(2.0f*stdDevSq))/sqrtf(2.0f*PI*stdDevSq);
}


void OpenGLRenderer::ComputeGaussianWeights(f32* out_pWeights, s32 numWeights, f32 standardDeviationSquared)
{
	const float numWeightsMin1 = (float)(numWeights-1);
	
	float sum = 0.0f;
	for (int i=0;i<numWeights; i++)
	{
		float x = (2.0f * (float)i - numWeightsMin1) / numWeightsMin1;
		out_pWeights[i] = ComputeGaussianValue(x,standardDeviationSquared);
		sum += out_pWeights[i];
	}
	
	for(int i=0; i<numWeights; i++)
	{
		out_pWeights[i] /= sum;
	}
}


void OpenGLRenderer::EnableAttributes(const ModelData* pModelData)
{
	const u32 stride = pModelData->stride;
	
	for(u32 i=0; i<pModelData->numAttributes; ++i)
	{
		const AttributeData* pAttrib = &pModelData->attributeArray[i];
		glEnableVertexAttribArray(pAttrib->attribute);
		glVertexAttribPointer(pAttrib->attribute, pAttrib->size, pAttrib->type, 0, stride, BUFFER_OFFSET(pAttrib->offset));
	}
		
    PrintOpenGLError("After enabling attributes");
}


void OpenGLRenderer::BindVertexArrayObject(const PrimitiveData* pPrimitive)
{
#ifdef PLATFORM_IOS
	glBindVertexArrayOES(pPrimitive->vertexArrayObjectID);
#else
	glBindVertexArrayAPPLE(pPrimitive->vertexArrayObjectID);
#endif
	
}


void OpenGLRenderer::BindIndexData(const PrimitiveData* pPrimitive)
{
	//If it's NULL, it won't use indices
	if (pPrimitive->indexData != NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pPrimitive->indexBufferID);
	}
}


void OpenGLRenderer::AddUniform_Unique(RenderMaterial renderMaterial, const char* nameOfUniformInShader,UniformType uniformType, u32 amount)
{
	const int index = g_Materials[renderMaterial].numUniforms_unique;
	const GLint uniformID = glGetUniformLocation(g_Materials[renderMaterial].shaderProgram,nameOfUniformInShader);
	
	if (uniformID == -1)
	{
		NSLog(@"ERROR: addUniform_Unique -> Unable to add uniform: %s for material %s!\n", nameOfUniformInShader,g_MaterialNames[renderMaterial]);
	}
	
	g_Materials[renderMaterial].uniforms_unique[index] = uniformID;
	g_Materials[renderMaterial].uniforms_unique_sizes[index] = amount;
	g_Materials[renderMaterial].uniforms_unique_types[index] = uniformType;
	++g_Materials[renderMaterial].numUniforms_unique;
}


void OpenGLRenderer::AddUniform_Shared(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, f32* pData, u32 amount)
{
	const int index = g_Materials[renderMaterial].numUniforms_shared;
	g_Materials[renderMaterial].uniforms_shared[index].uniform = glGetUniformLocation(g_Materials[renderMaterial].shaderProgram,nameOfUniformInShader);
	
	if (g_Materials[renderMaterial].uniforms_shared[index].uniform == -1)
	{
		NSLog(@"ERROR: addUniform_Shared -> Unable to add uniform: %s for material %s!\n", nameOfUniformInShader,g_MaterialNames[renderMaterial]);
		
		//Return early and don't add this uniform
		return;
	}
	
	g_Materials[renderMaterial].uniforms_shared[index].value.type = uniformType;
	g_Materials[renderMaterial].uniforms_shared[index].value.data = pData;
	g_Materials[renderMaterial].uniforms_shared[index].size = amount;
	++g_Materials[renderMaterial].numUniforms_shared;
}


void OpenGLRenderer::AddUniform_Shared_Const(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, f32* pData, u32 amount)
{
	const int index = g_Materials[renderMaterial].numUniforms_shared_const;
	g_Materials[renderMaterial].uniforms_shared_const[index].uniform = glGetUniformLocation(g_Materials[renderMaterial].shaderProgram,nameOfUniformInShader);
	
	if (g_Materials[renderMaterial].uniforms_shared_const[index].uniform == -1)
	{
		NSLog(@"ERROR: addUniform_Shared_Const -> Unable to add uniform: %s for material %s!\n", nameOfUniformInShader,g_MaterialNames[renderMaterial]);
		
		//Return early and don't add this uniform
		return;
	}
	
	g_Materials[renderMaterial].uniforms_shared_const[index].value.type = uniformType;
	g_Materials[renderMaterial].uniforms_shared_const[index].value.data = pData;
	g_Materials[renderMaterial].uniforms_shared_const[index].size = amount;
	++g_Materials[renderMaterial].numUniforms_shared_const;
}


void OpenGLRenderer::SortParticleQueues()
{
	//Sort every particle bucket
	for (s32 bucketIDX=0; bucketIDX<NumParticleBuckets; ++bucketIDX)
	{
		RendererParticleBucket* pBucket = &m_particleBuckets[bucketIDX];
		
		if (pBucket->m_particlesNeedSorting == NO)
		{
			continue;
		}
		
		//Else it needs sorting
		pBucket->m_particlesNeedSorting = NO;
		
		//INSERTION SORT
		for (s32 i=0; i < pBucket->m_numParticles; ++i)
		{
			Particle3D particleV = pBucket->m_particleQueue[i];
			
			s32 j;
			for (j = i - 1; j > -1; --j)
			{
				if (pBucket->m_particleQueue[j].timeToLive >= particleV.timeToLive)
				{
					break;
				}
				
				pBucket->m_particleQueue[j + 1] = pBucket->m_particleQueue[j];
			}
			
			pBucket->m_particleQueue[j + 1] = particleV;	
		}
	}
}

CPVRTModelPOD* OpenGLRenderer::LoadPOD(const char* fileName)
{
	CPVRTModelPOD* newPod = new CPVRTModelPOD;
	
	EPVRTError readResult = newPod->ReadFromFile(GetPathToFile(fileName));
	
	if(readResult == PVR_SUCCESS)
	{
		return newPod;
	}
	
	delete newPod;
	
	return NULL;
}


void OpenGLRenderer::AddRenderableScene3DToList(RenderableScene3D* pRenderableScene)
{
	for(u32 i=0; i<pRenderableScene->numGeom; ++i)
	{
		RenderableGeometry3D* pGeom = &pRenderableScene->pGeom[i];
		
		if (!(pGeom->flags & RenderFlag_Initialized))
		{
			//assert(0);
			//NSLog(@"Insane Error: addRenderableObject3D -> You can't add an uninitialized RenderableObject3D to the renderer!");
			
			return;
		}
		
		if(pGeom->renderLayer == RenderLayer_UI)
		{
			if(m_numRenderableUIObjects < MAX_RENDERABLE_UI_OBJECTS)
			{
				m_renderableUIList[m_numRenderableUIObjects++] = pGeom;
				
				m_renderableUINeedSorting = true;
			}
			else
			{
				NSLog(@"Insane Error: AddRenderableScene3DToList -> You tried to add more than MAX_RENDERABLE_UI_OBJECTS!  Added nothing.");
			}
		}
		else
		{
			if(m_numRenderableGeom3Ds < MAX_RENDERABLE_3D_OBJECTS)
			{
				m_renderableGeometry3DList[m_numRenderableGeom3Ds++] = pGeom;
				
				m_renderableObject3DsNeedSorting = true;
			}
			else
			{
				NSLog(@"Insane Error: AddRenderableScene3DToList -> You tried to add more than MAX_RENDERABLE_3D_OBJECTS!  Added nothing.");
			}
		}
	}
}


bool OpenGLRenderer::InitSceneFromPOD(RenderableScene3D* pScene, const char* fileName,  mat4f matrix4x4, u32 viewFlags)
{
	CPVRTModelPOD* newPod = LoadPOD(fileName);

	if(newPod)
	{
		if(matrix4x4 == NULL)
		{
			mat4f_LoadIdentity(pScene->worldMat);
		}
		else
		{
			mat4f_Copy(pScene->worldMat,matrix4x4);
		}
		
		pScene->pGeom = new RenderableGeometry3D[newPod->nNumMesh];
		pScene->numGeom = newPod->nNumMesh;

		// To draw a scene, you must go through all the MeshNodes and draw the referenced meshes.
		for (unsigned int i = 0; i < newPod->nNumMeshNode; ++i)
		{
			// Get the mesh data from the POD file
			int i32MeshIndex = newPod->pNode[i].nIdx;

			SPODMesh& Mesh = newPod->pMesh[i32MeshIndex];
			
			//Create model data
			ModelData* pModelData = new ModelData;
			pModelData->modelName = NULL;
			pModelData->primitiveArray = new PrimitiveData[1];	//TODO: I think POD just has one primitive unless it's a triangle strip
			pModelData->numPrimitives = 1;	//TODO: this will be wrong if a triangle strip comes in
			pModelData->numAttributes = 0;
			pModelData->stride = Mesh.sVertex.nStride;
			
			//Fill out the geom
			RenderableGeometry3D* pCurrGeom = &pScene->pGeom[i];
			pCurrGeom->pModel = pModelData;
			pCurrGeom->pWorldMat = pScene->worldMat;
			pCurrGeom->customTexture0 = &texture_default;
			pCurrGeom->customTexture1 = 0;
			pCurrGeom->flags = RenderFlag_Visible|RenderFlag_Initialized|RenderFlag_DisableBlending;
			pCurrGeom->materialID = MT_TextureOnlySimple;
			pCurrGeom->postRenderLayerSortValue = 0;
			pCurrGeom->renderLayer = RenderLayer_Normal;
			pCurrGeom->viewFlags = viewFlags;
			
			pModelData->attributeArray = new AttributeData[8];	//TODO: if this goes beyond 8 we're all gunna die
			pModelData->modelID = 0;
			
			
			AttributeData* pAttribs = &pModelData->attributeArray[0];

			u32 attribOffset = 0;
			
			if(Mesh.sVertex.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_VERTEX;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sVertex.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sNormals.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_NORMAL;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sNormals.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sTangents.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_TANGENT;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sTangents.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sBinormals.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_BINORMAL;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sBinormals.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.psUVW->n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_TEXCOORD;
				pCurrAttrib->type = GL_FLOAT;
				
				pCurrAttrib->size = Mesh.psUVW->n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats or nNumUVW is more than 1
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sVtxColours.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_COLOR;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sVtxColours.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: no idea if this is 16 bytes!
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sBoneWeight.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_BONEWEIGHT;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sBoneWeight.n;
				pCurrAttrib->offset = attribOffset;
				attribOffset += pCurrAttrib->size*sizeof(GL_FLOAT); //TODO: won't hold up if you change to non-floats
				
				++pModelData->numAttributes;
			}
			
			if(Mesh.sBoneIdx.n > 0)
			{
				AttributeData* pCurrAttrib = &pAttribs[pModelData->numAttributes];
				
				pCurrAttrib->attribute = ATTRIB_BONEINDEX;
				pCurrAttrib->type = GL_FLOAT;
				pCurrAttrib->size = Mesh.sBoneIdx.n;
				pCurrAttrib->offset = attribOffset;
				
				++pModelData->numAttributes;
			}
			
			PrimitiveData* pCurrData = &pModelData->primitiveArray[0];
			
			pCurrData->vertexArrayObjectID = 0;
			pCurrData->indexBufferID = 0;
			
			pCurrData->numVerts = Mesh.nNumVertex;

			unsigned int vertexBufferSize = Mesh.nNumVertex * Mesh.sVertex.nStride;
			
			pCurrData->drawMethod = GL_TRIANGLES;

			pCurrData->vertexData = Mesh.pInterleaved;
			pCurrData->sizeOfVertexData = vertexBufferSize;			
			
			// Load index data into buffer object if available
			pCurrData->indexBufferID = 0;
			
			if (Mesh.sFaces.pData)
			{
				const u32 numIndices = PVRTModelPODCountIndices(Mesh);
				pCurrData->numVerts = numIndices;
				const u32 sizeOfGLShort = sizeof(GLshort);
				const u32 indexDataSize = numIndices * sizeOfGLShort;
				
				pCurrData->indexData = (GLushort*)Mesh.sFaces.pData;
				pCurrData->sizeOfIndexData = indexDataSize;
				/*
				//DEBUG!
				printf("\n****************\nNum indices: %d, Triangles: %d\n****************\n",pModelData->primitiveArray[0].numVerts,pModelData->primitiveArray[0].numVerts/3);
				for(int indexIDX=0; indexIDX<pModelData->primitiveArray[0].numVerts; ++indexIDX)
				{
					const u32 index = pModelData->primitiveArray[0].indexData[indexIDX];
					printf("Index %d: %d\n",indexIDX,index);
					
					u8* pData = &pModelData->primitiveArray[0].vertexData[index*pModelData->stride];
					vec3* pPos = (vec3*)&pData[0];
					vec2* pUV = (vec2*)&pData[12];
					printf("Vert: V:<%f,%f,%f>, T:<%f,%f>\n",pPos->x,pPos->y,pPos->z,pUV->x,pUV->y);
				}*/
			}
			
			RegisterModel(pModelData);
		}
		
		return true;
	}
	
	return false;
}


//helpers

bool LoadPngImage(const char* fileName, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData, bool flipY)
{
	FILE         *infile;         /* PNG file pointer */
	png_structp   png_ptr;        /* internally used by libpng */
	png_infop     info_ptr;       /* user requested transforms */
	
	GLubyte         *image_data;      /* raw png image data */
	char         sig[8];           /* PNG signature array */
	/*char         **row_pointers;   */
	
	int           bit_depth;
	int           color_type;
	int			  interlace_type;
	
	png_uint_32 width;            /* PNG image width in pixels */
	png_uint_32 height;           /* PNG image height in pixels */
	png_size_t rowbytes;         /* raw bytes at row n in image */
	
	image_data = NULL;
	
	png_bytepp row_pointers = NULL;
	
	/* Open the file. */
	infile = fopen(fileName, "rb");
	if (!infile) {
		return 0;
	}
	
	
	/*
	 * 		13.3 readpng_init()
	 */
	
	/* Check for the 8-byte signature */
	fread(sig, 1, 8, infile);
	
	if (!png_check_sig((unsigned char *) sig, 8)) {
		fclose(infile);
		return 0;
	}
	
	/* 
	 * Set up the PNG structs 
	 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(infile);
		return 4;    /* out of memory */
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(infile);
		return 4;    /* out of memory */
	}
	
	
	/*
	 * block to handle libpng errors, 
	 * then check whether the PNG file had a bKGD chunk
	 */
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(infile);
		return 0;
	}
	
	/* 
	 * takes our file stream pointer (infile) and 
	 * stores it in the png_ptr struct for later use.
	 */
	/* png_ptr->io_ptr = (png_voidp)infile;*/
	png_init_io(png_ptr, infile);
	
	/*
	 * lets libpng know that we already checked the 8 
	 * signature bytes, so it should not expect to find 
	 * them at the current file pointer location
	 */
	png_set_sig_bytes(png_ptr, 8);
	
	/* Read the image info.*/
	
	/*
	 * reads and processes not only the PNG file's IHDR chunk 
	 * but also any other chunks up to the first IDAT 
	 * (i.e., everything before the image data).
	 */
	
	/* read all the info up to the image data  */
	png_read_info(png_ptr, info_ptr);
	
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
				 &interlace_type, NULL, NULL);
	
	outWidth = width;
	outHeight = height;
	
	/* Set up some transforms. */
	if (color_type & PNG_COLOR_MASK_ALPHA)
	{
		outHasAlpha = true;
		//png_set_strip_alpha(png_ptr);
	}
	else
	{
		outHasAlpha = false;
	}
	
	if (bit_depth > 8) {
		png_set_strip_16(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}
	
	/* Update the png info struct.*/
	png_read_update_info(png_ptr, info_ptr);
	
	/* Rowsize in bytes. */
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	
	
	/* Allocate the image_data buffer. */
	if ((image_data = (GLubyte *) malloc(rowbytes * height))==NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 4;
    }
	
	if ((row_pointers = (png_bytepp)malloc(height*sizeof(png_bytep))) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        image_data = NULL;
        return 4;
    }
	
	
    /* set the individual row_pointers to point at the correct offsets */
	
	if(flipY)
	{
		for (unsigned int i = 0;  i < height;  ++i)
		{
			row_pointers[i] = image_data + i*rowbytes;
		}
	}
	else
	{
		for (unsigned int i = 0;  i < height;  ++i)
		{
			row_pointers[height - 1 - i] = image_data + i*rowbytes;
		}
	}
	
    
	
    /* now we can go ahead and just read the whole image */
    png_read_image(png_ptr, row_pointers);
	
    /* and we're done!  (png_read_end() can be omitted if no processing of
     * post-IDAT text/time/etc. is desired) */
	
	/* Clean up. */
	free(row_pointers);
	
	/* Clean up. */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(infile);
	
	*outData = (GLubyte*)image_data;
	
	return 1;
}


s32 RenderableGeometry3DCompare_SortValue(const void* lhs, const void* rhs)
{
	return ((RenderableGeometry3D*)lhs)->sortValue <= ((RenderableGeometry3D*)rhs)->sortValue;
}


s32 RenderableGeometry3DCompare_SortByZ(const void* lhs, const void* rhs)
{
	const vec3* pPosLHS = mat4f_GetPos(((RenderableObject3D*)lhs)->worldMat);
	const vec3* pPosRHS = mat4f_GetPos(((RenderableObject3D*)rhs)->worldMat);
	
	return pPosLHS->z <= pPosRHS->z;
}


s32 RenderableGeometry3DCompare_SortByNegativeZ(const void* lhs, const void* rhs)
{
	const vec3* pPosLHS = mat4f_GetPos(((RenderableObject3D*)lhs)->worldMat);
	const vec3* pPosRHS = mat4f_GetPos(((RenderableObject3D*)rhs)->worldMat);
	
	return pPosLHS->z > pPosRHS->z;
}
