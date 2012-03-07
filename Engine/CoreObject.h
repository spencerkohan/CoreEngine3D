//
//  CoreObject.h
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_iOS__CoreObject_h
#define CoreEngine3D_iOS__CoreObject_h

#include "MathTypes.h"
#include "stddef.h" //for NULL -_-

class CoreObject;
class CoreObjectManager;

enum CoreObjectType
{
	CoreObjectType_CoreObject,
	CoreObjectType_CoreGameObject,
};

struct CoreObjectHandle
{
    CoreObjectHandle() : m_index(0), m_counter(0), m_type(0)
    {}
	
    CoreObjectHandle(u32 index, u32 counter, u32 type)
	: m_index(index), m_counter(counter), m_type(type)
    {}
	
    inline operator u32() const
	{
		return m_type << 27 | m_counter << 12 | m_index;
	}
	
    u32 m_index : 12;
    u32 m_counter : 15;
    u32 m_type : 5;
	
	bool IsValid() const {return (*this) != 0;}
};

class CoreObject
{
	friend class CoreObjectManager;
	
public:
	virtual CoreObjectType GetType(){return CoreObjectType_CoreObject;}
	static void InitClass(){};
	u32 GetEntityType();
	void SetEntityType(u32 type){m_entityTypeHash = type;}	//WARNING: never call this EVER... unless you really know why
	CoreObjectHandle GetHandle() const;
	
	virtual void UpdateHandle();	//Call when the memory location changes
	void DeleteObject();			//Use with new system

	virtual bool Init(u32 type);
	virtual bool SpawnInit(void* pSpawnStruct){return true;}
	virtual bool PostSpawnInit(void* pSpawnStruct){return true;}
	virtual void Uninit();
	virtual void Update(f32 timeElapsed){};
	
	virtual void ProcessMessage(u32 message){};	//Pass in a hash value
	
	void InvalidateHandle();
	bool m_markedForDeletion;
	
protected:

private:
	
	u32 m_entityTypeHash;
	CoreObjectHandle handle;
	
};

#endif
