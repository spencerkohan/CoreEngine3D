//
//  Game.h
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OSX__Game_h
#define InfiniSurv_OSX__Game_h

#include "MathTypes.h"
#include "OpenGLRenderer.h"

class Game
{
public:
	virtual void Init(OpenGLRenderer* pRenderer) = 0;
	virtual void Update(f32 timeElapsed) = 0;
	virtual void CleanUp() = 0;
protected:
	OpenGLRenderer*  m_pRenderer;
};

#endif
