#pragma once

#include "Object.h"
#include "ObjectPtr.h"
#include "Win32.h"

class CWindow : public CObject
{
	HWND hwnd;
	bool mustrelease;
protected:
	HWND GetWindowHandle();
	CWindow* GetClassPointer();
	bool SaveClassPointerToWindow(HWND hwnd = nullptr);
	virtual bool CreateWindowHandle(HWND &hWindow, HWND parent) = 0;
public:
	CWindow();
	virtual ~CWindow();

	bool Create(HWND parent = nullptr);

	bool Resize(SIZE NewSize);

	bool Move(POINT NewPosition);

	U32 GetWidth() const;
	U32 GetHeight() const;

	U32 GetXPosition() const;
	U32 GetYPosition() const;
};

