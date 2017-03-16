#pragma once

#include "Object.h"

class CWindow : public CObject
{
	HWND hwnd;
protected:
	HWND GetWindowHandle();
	CWindow* GetClassPointer();
	virtual bool CreateWindowHandle(HWND &hWindow) = 0;
	virtual bool DestroyWindowHandle(HWND hWindow) = 0;
	virtual bool ResizeChildWindows(RECT NewSize) = 0;
public:
	CWindow();
	virtual ~CWindow();

	bool Create();
	bool Destroy();

	bool OnMove(RECT NewSize);
	bool OnResize(RECT NewSize);
};

