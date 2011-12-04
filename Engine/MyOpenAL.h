//  MyOpenAL.h
//

#if DEBUG==1
#define CMLog(format, ...) NSLog(@"%s:%@", __PRETTY_FUNCTION__,[NSString stringWithFormat:format, ## __VA_ARGS__]);
#define MARK	CMLog(@"%s", __PRETTY_FUNCTION__);
#define START_TIMER NSTimeInterval start = [NSDate timeIntervalSinceReferenceDate];
#define END_TIMER(msg) 	NSTimeInterval stop = [NSDate timeIntervalSinceReferenceDate]; CMLog([NSString stringWithFormat:@"%@ Time = %f", msg, stop-start]);
//#define ALog(fmt, ...) NSLog((@"%s [DebugLine %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#define ALog(fmt, ...) NSLog((fmt), ##__VA_ARGS__);

#else
#define CMLog(format, ...)
#define MARK
#define START_TIMER
#define END_TIMER(msg)
//#define ALog(fmt, ...) NSLog((@"%s [ReleaseLine %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#define ALog(fmt, ...)
#endif



#import <Foundation/Foundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import "MathTypes.h"

#define MaxSourcesPerBuffer 8

struct soundData
{
	float volume;
	float posx;
	float posy;
	float posz;	
	float pitch;
	float shift;
	unsigned int sbufferid;
	NSUInteger thesources[MaxSourcesPerBuffer];
	s32 nextSourceToUse;
};

//extern struct soundData * lsound;

extern NSMutableArray * sourceArray1; // array of sources

@interface MyOpenAL : NSObject
{	
	ALCcontext* mContext; // stores the context (the 'air')
	ALCdevice* mDevice; // stores the device
	NSMutableArray * bufferStorageArray; // stores the buffer ids from openAL
	NSMutableDictionary * soundDictionary; // stores our soundkeys
	NSMutableDictionary * bufferDictionary; // stores our soundkeys	
}

// if you want to access directly the buffers or our sound dictionary
@property (nonatomic, retain) NSMutableArray * bufferStorageArray;
@property (nonatomic, retain) NSMutableDictionary * soundDictionary;
@property (nonatomic, retain) NSMutableDictionary * bufferDictionary;

- (id) init; // init once
- (bool) initOpenAL; // no need to make it public, but I post it here to show you which methods we need. initOpenAL will be called within init process once.
- (void) playSoundWithKey:(NSString*)soundKey; // play a sound by name
- (void) stopSoundWithKey:(NSString*)soundKey; // stop a sound by name
- (bool) isPlayingSoundWithKey:(NSString *) soundKey; // check if sound is playing by name
- (void) rewindSoundWithKey:(NSString *) soundKey; // rewind a sound by name so its playing again
- (bool) loadSoundWithKey:(NSString *)_soundKey File:(NSString *)_file Ext:(NSString *) _ext Loop:(bool)loops; // load a sound and give it a name
- (bool) loadSoundBufferWithKey:(NSString *)_soundKey volume:(float) _volume pitch:(float)_pitch shift:(float)_shift File:(NSString *)_file Ext:(NSString *) _ext ;
- (bool) PlaySoundBufferAtLocation:(NSString *)_buffer Location:(const vec3*)pSoundPos PitchAdd:(f32)pitchAdd Looping:(BOOL)loops;
- (void) setListenerVolume :(float) _volume;
- (void) StopAllSourcesForBuffer:(NSString *)_buffer;

+ (MyOpenAL*)sharedMyOpenAL; // access to our shared instance

@end
