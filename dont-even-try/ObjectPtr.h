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

	CObjectPtr(T *const  pNewObjectPtr = nullptr, bool IsSoleOwner = false) : pObject(pNewObjectPtr)
	{
		if(!IsSoleOwner)
		{
			AddRef();
		}
	}

	CObjectPtr(const CObjectPtr<T> &other) :pObject(other.pObject)
	{
		AddRef();
	}

	CObjectPtr(CObjectPtr<T> &&other) : pObject(other.pObject)
	{
		other.pObject = nullptr;
	}

	template<typename Q>
	CObjectPtr(const CObjectPtr<Q>& other)
	{
		static_assert(std::is_base_of<T, Q>::value || std::is_base_of<Q, T>::value, "T is not convertible to Q!");
		pObject = dynamic_cast<T*>(other.GetInterface());
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

	T* operator->() const
	{
		return pObject;
	}

	T* GetInterface() const
	{
		return pObject;
	}

	CObjectPtr<T>& operator=(T* const pOther)
	{
		Release();
		pObject = pOther;
		AddRef();
		return *this;
	}

	CObjectPtr<T>& operator=(const CObjectPtr<T> &pOther)
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

	bool operator==(const T* const pOther) const
	{
		return pObject == pOther;
	}

	bool operator==(const CObjectPtr<T>& pOther) const
	{
		return pObject == pOther.pObject;
	}

	bool operator!=(const T* const pOther) const
	{
		return pObject != pOther;
	}

	bool operator!=(const CObjectPtr<T>& pOther) const
	{
		return pObject != pOther.pObject;
	}

	operator bool() const
	{
		return pObject != nullptr;
	}
};

