/*
 *  EngineModels.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#if defined (PLATFORM_OSX)
#include <OpenGL/gl.h>
#endif

#if defined (PLATFORM_WIN)
#include "glew/glew.h"
#endif

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#include "stddef.h" //for NULL -_-

#include "Models/square1x1.h"
#include "Models/fullscreen_model.h"
#include "Models/Square_Tiled_32.h"
#include "Models/Square_Tiled_16.h"
#include "Models/Square_Tiled_8.h"
#include "Models/Square_Tiled_4.h"
#include "Models/Square_Tiled_2.h"
