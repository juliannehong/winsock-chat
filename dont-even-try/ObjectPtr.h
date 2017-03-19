#pragma once
#include <type_traits>
#include <exception>
#include "Object.h"

template<typename T>
class CObjectPtr
{
	static_assert(std::is_base_of<CObject, T>::value, "T must derive from CObject!");

	T* pObject;
public:

	CObjectPtr(T* pNewObjectPtr = nullptr, bool IsSoleOwner = false) : pObject(pNewObjectPtr)
	{
		if(!IsSoleOwner)
		{
			AddRef();
		}
	}

	CObjectPtr(CObjectPtr<T> &other) :pObject(other.pObject)
	{
		AddRef();
	}

	CObjectPtr(CObjectPtr<T> &&other) : pObject(other.pObject)
	{
		other.pObject = nullptr;
	}

	template<typename Q>
	CObjectPtr(CObjectPtr<Q> const& other)
	{
		pObject = dynamic_cast<T*>(other.pObject);
		AddRef();
	}

	~CObjectPtr()
	{
		Release();
	}

	U32 AddRef()
	{
		if(pObject)
		{
			return pObject->AddRef();
		}
		return 0;
	}

	U32 Release()
	{
		U32 r = 0;
		if(pObject)
		{
			r = pObject->Release();
			pObject = nullptr;
		}
		return r;
	}

	T* operator->()
	{
		return pObject;
	}

	T* GetInterface()
	{
		return pObject;
	}

	CObjectPtr<T>& operator=(T* pOther)
	{
		Release();
		pObject = pOther;
		AddRef();
		return *this;
	}

	CObjectPtr<T>& operator=(CObjectPtr<T> &pOther)
	{
		Release();
		pObject = pOther.pObject;
		AddRef();
		return *this;
	}

	CObjectPtr<T>& operator=(CObjectPtr<T> &&pOther)
	{
		Release();
		std::swap(pObject, pOther.pObject);
		return *this;
	}

	bool operator==(T* pOther)
	{
		return pObject == pOther;
	}

	bool operator==(CObjectPtr<T>& pOther)
	{
		return pObject == pOther.pObject;
	}

	bool operator!=(T* pOther)
	{
		return pObject != pOther;
	}

	bool operator!=(CObjectPtr<T>& pOther)
	{
		return pObject != pOther.pObject;
	}

	operator bool()
	{
		return pObject != nullptr;
	}
};

