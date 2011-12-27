/*
 *  EngineModels.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#ifdef PLATFORM_OSX
#include <OpenGL/gl.h>
#endif

#ifdef PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "stddef.h" //for NULL -_-

#include "square1x1.h"
#include "Square_Anim_10x10.h"
#include "fullscreen_model.h"
#include "Square_Tiled_16.h"
#include "Square_Tiled_8.h"
