#pragma once
#include "Window.h"

class CCustomWindow :
	public CWindow
{
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool CreateWindowHandle(HWND& hwnd, HWND parent);
protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	virtual bool CreateChildWindows(HWND parent);
	virtual LPTSTR GetWindowClassName() const = 0;
	virtual void InitializeWindowClass(LPWNDCLASS wc) = 0;
	virtual void InitializeWindowCreateStruct(LPCREATESTRUCT cs) = 0;
	virtual void OnChildNotify(CObjectPtr<CWindow> child, U32 NotificationCode);
	virtual void OnMenuItem(U32 MenuItemID, bool IsAccelerator);
	virtual void ResizeChildWindows(RECT NewClientRect);

public:
	CCustomWindow();
	virtual ~CCustomWindow();

};

