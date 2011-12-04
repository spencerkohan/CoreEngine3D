//
//  Random.m
//  CoreEngine3D
//
//  Created by sam on 2/27/11.
//  Copyright 2011 GameJelly Online. All rights reserved.
//

#import "Random.h"

u32 randCounter;
@implementation Random

-(s32) rand_IntRange:(s32) a max:(s32) b
{
	s32 num = a+[self rand32]%((b-a)+1);
//	NSLog(@"random system int: %d",num);

    return num;
}

-(f32) rand_FloatRange:(f32) a max:(f32) b
{
	f32 num = ((b-a)*((f32) [self rand32]/RAND_MAX))+a;
//	NSLog(@"random system float: %f",num);
	return num;
}

-(s32) rand32
{
    tmpseed = (unsigned long long)33614U * (unsigned long long)newseed;
    q = (u32)tmpseed; /* low */
    q = q >> 1;
    p = tmpseed >> 32 ; /* hi */
    mlcg = p + q;
    if (mlcg & 0x80000000) {
        mlcg = mlcg & 0x7FFFFFFF;
        mlcg++;
    }
    newseed = mlcg;
	randCounter ++;
//	NSLog(@"random system: returning a random number of %d, this is the %d th rand generated since seed ",mlcg,randCounter);
    return mlcg;
}
-(void) srand32:(unsigned)sd
{
	randCounter = 0;
    newseed = sd;   
//	NSLog(@"random system: seed set to %d");
}

@end
