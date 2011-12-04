#ifndef __VIEWFLAGS_H
#define __VIEWFLAGS_H

/*
 *  ViewFlags.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

typedef enum {
	CameraView0,
	//CameraView1,
	//CameraView2,
	//CameraView3,
	NumCameraViews,
}CameraView;

typedef enum {
	View_0 = 1 << CameraView0,
	//View_1 = 1 << CameraView1,
	//View_2 = 1 << CameraView2,
	//View_3 = 1 << CameraView3,
} ViewFlags;

extern const char* g_ViewFlagNames[NumCameraViews];

#endif
