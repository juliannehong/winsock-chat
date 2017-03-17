#pragma once

#include "Object.h"

class CWindow : public CObject
{
	HWND hwnd;
	bool mustrelease;
protected:
	HWND GetWindowHandle();
	CWindow* GetClassPointer();
	bool SaveClassPointerToWindow(HWND hwnd = nullptr);
	virtual bool CreateWindowHandle(HWND &hWindow, HWND parent) = 0;
	virtual bool DestroyWindowHandle(HWND hWindow) = 0;
	virtual bool ResizeChildWindows(RECT NewSize) = 0;
public:
	CWindow();
	virtual ~CWindow();

	bool Create(CWindow* parent);
	bool Destroy();

	bool OnMove(RECT NewSize);
	bool OnResize(RECT NewSize);
};

