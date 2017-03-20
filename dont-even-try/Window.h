#pragma once

#include "Object.h"
#include "ObjectPtr.h"
#include "Win32.h"

class CWindow : public CObject
{
	HWND hwnd;
protected:
	void SavePointerToHandle(HWND hwnd);
	void ClearPointerFromHandle(HWND hwnd);

	virtual bool CreateWindowHandle(HWND &hWindow, HWND parent) = 0;
public:
	CWindow();
	virtual ~CWindow();

	static CObjectPtr<CWindow> GetClassPointer(HWND hwnd);
	
	HWND GetWindowHandle();
	
	bool Create(HWND parent = nullptr);

	void Resize(SIZE NewSize);

	void Move(POINT NewPosition);

	void Close();

	virtual U32 GetMaxWidth() const;
	virtual U32 GetMaxHeight() const;
	virtual U32 GetMinWidth() const;
	virtual U32 GetMinHeight() const;

	U32 GetWidth() const;
	U32 GetHeight() const;

	U32 GetXPosition() const;
	U32 GetYPosition() const;
};

