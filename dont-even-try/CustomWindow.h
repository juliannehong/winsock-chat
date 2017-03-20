#pragma once
#include "Window.h"
class CCustomWindow :
	public CWindow
{
	ATOM wndclass;
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool CreateWindowHandle(HWND& hwnd, HWND parent);

protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	virtual bool InitializeWindowClass(LPWNDCLASSEX wcx) = 0;
	virtual bool InitializeWindowCreateStruct(LPCREATESTRUCT cs) = 0;
	virtual void OnChildNotify(CObjectPtr<CWindow> child, U32 NotificationCode);
	virtual void OnMenuItem(U32 MenuItemID, bool IsAccelerator);
	virtual bool CreateChildWindows(HWND parent);
	virtual void ResizeChildWindows(RECT NewClientRect);

public:
	CCustomWindow();
	virtual ~CCustomWindow();
};

