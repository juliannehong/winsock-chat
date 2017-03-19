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

CWindow::CWindow() : hwnd(nullptr), mustrelease(false)
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

bool CWindow::Create(HWND parent)
{
	if(!CreateWindowHandle(hwnd, parent))
	{
		return false;
	}
	ShowWindow(hwnd, SW_RESTORE);
	UpdateWindow(hwnd);
	return true;
}

bool CWindow::Resize(SIZE NewSize)
{
	//Change the dimensions of the window.
	SetWindowPos(hwnd, nullptr, 0, 0, NewSize.cx, NewSize.cy, SWP_NOZORDER | SWP_NOMOVE);
	return true;
}

bool CWindow::Move(POINT NewPosition)
{
	//Change the coordinates of the window.
	SetWindowPos(hwnd, nullptr, NewPosition.x, NewPosition.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	return true;
}

U32 CWindow::GetWidth() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.right - r.left;
}

U32 CWindow::GetHeight() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.bottom - r.top;
}

U32 CWindow::GetXPosition() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.left;
}

U32 CWindow::GetYPosition() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.top;
}

