#pragma once
#include "Window.h"
class CCustomWindow :
	public CWindow
{
	ATOM wndclass;
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool CreateWindowHandle(HWND& hwnd, HWND parent);
	bool DestroyWindowHandle(HWND hwnd);

	bool HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	virtual bool InitializeWindowClass(LPWNDCLASSEX wcx) = 0;
	virtual DWORD GetExtendedWindowStyle() = 0;
	virtual LPTSTR GetInitialWindowTitle() = 0;
	virtual DWORD GetWindowStyle() = 0;

public:
	CCustomWindow();
	virtual ~CCustomWindow();
};

