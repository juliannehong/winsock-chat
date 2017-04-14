#pragma once
#include "Window.h"
class CStandardWindow :
	public CWindow
{
	static LRESULT SubclassProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR SubclassID, DWORD_PTR refdata);

	bool CreateWindowHandle(HWND parent);

protected:

	virtual void InitializeCreateStruct(LPCREATESTRUCT cs) = 0;

public:
	CStandardWindow();
	virtual ~CStandardWindow();
};

