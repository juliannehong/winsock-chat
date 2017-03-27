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
	void ResizeAndMove(SIZE NewSize, POINT NewPosition);

	void Close();

	virtual I32 GetMaxWidth() const;
	virtual I32 GetMaxHeight() const;
	virtual I32 GetMinWidth() const;
	virtual I32 GetMinHeight() const;

	I32 GetWidth() const;
	I32 GetHeight() const;

	I32 GetXPosition() const;
	I32 GetYPosition() const;
};

