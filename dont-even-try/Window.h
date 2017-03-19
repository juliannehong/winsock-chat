#pragma once

#include "Object.h"
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

	bool Resize(RECT NewSize);

};

