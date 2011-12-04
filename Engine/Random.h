//
//  Random.h
//  CoreEngine3D
//
//  Created by sam on 2/27/11.
//  Copyright 2011 GameJelly Online. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MathTypes.h"

@interface Random : NSObject 
{
	unsigned long long tmpseed;
	u32 newseed;
	u32 mlcg,p,q;	
}



-(s32) rand32;
-(s32) rand_IntRange:(s32) a max:(s32) b;
-(void) srand32:(unsigned)sd;
-(f32) rand_FloatRange:(f32) a max:(f32) b;

@end
