#pragma once
#include <atomic>
#include <cassert>
#include "Types.h"

class CObject
{
	std::atomic<U32> refcount;

public:
	CObject() : refcount(1)
	{
	}

	virtual ~CObject()
	{
		assert(refcount == 0);
	}

	U32 AddRef()
	{
		return ++refcount;
	}

	U32 Release()
	{
		U32 r = --refcount;
		if(r == 0)
		{
			delete this;
		}
		return r;
	}
};

