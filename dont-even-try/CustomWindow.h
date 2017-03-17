#pragma once
#include "Window.h"
class CCustomWindow :
	public CWindow
{
	ATOM wndclass;
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool CreateWindowHandle(HWND& hwnd, HWND parent);
	bool DestroyWindowHandle(HWND hwnd);

	virtual bool CreateChildWindows() = 0;
	virtual bool DestroyChildWindows() = 0;
	virtual LPTSTR GetClassNameString() = 0;
	virtual HICON GetLargeIcon() = 0;
	virtual HICON GetSmallIcon() = 0;
	virtual HCURSOR GetCursorIcon() = 0;
	virtual DWORD GetExtendedWindowStyle() = 0;
	virtual LPTSTR GetInitialWindowTitle() = 0;
	virtual DWORD GetWindowStyle() = 0;
	virtual HMENU GetMenuHandle() = 0;

public:
	CCustomWindow();
	virtual ~CCustomWindow();
};

