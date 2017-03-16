#include "Window.h"


HWND CWindow::GetWindowHandle()
{
	return hwnd;
}

CWindow * CWindow::GetClassPointer()
{
	if(hwnd == nullptr)
	{
		return nullptr;
	}
	return (CWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

CWindow::CWindow() : hwnd(nullptr)
{
}

CWindow::~CWindow()
{
	if(hwnd)
	{
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
}

bool CWindow::Create()
{
	if(!CreateWindowHandle(hwnd))
	{
		hwnd = nullptr;
		return false;
	}
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(this));
	AddRef();
	ShowWindow(hwnd, SW_RESTORE);
	UpdateWindow(hwnd);
	return true;
}

bool CWindow::Destroy()
{
	if(!DestroyWindow(hwnd))
	{
		return false;
	}
	hwnd = nullptr;
	//Release the reference held by the window handle.
	Release();
	return true;
}

bool CWindow::OnResize(RECT NewSize)
{
	//parent window has been resized - we need to resize our child controls.
	SetWindowPos(hwnd, nullptr, 0, 0, NewSize.right - NewSize.left, NewSize.bottom - NewSize.top, SWP_NOZORDER | SWP_NOMOVE);
	return true;
}

bool CWindow::OnMove(RECT NewPosition)
{
	SetWindowPos(hwnd, nullptr, NewPosition.left, NewPosition.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	return true;
}
