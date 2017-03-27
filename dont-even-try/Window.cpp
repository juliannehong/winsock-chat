#include "Window.h"


HWND CWindow::GetWindowHandle()
{
	return hwnd;
}

CObjectPtr<CWindow> CWindow::GetClassPointer(HWND hwnd)
{
	if(hwnd == nullptr)
	{
		return nullptr;
	}
	return CObjectPtr<CWindow>((CWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

void CWindow::SavePointerToHandle(HWND hwnd)
{
	AddRef();
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

void CWindow::ClearPointerFromHandle(HWND hwnd)
{
	SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
	Release();
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

void CWindow::Resize(SIZE NewSize)
{
	//Change the dimensions of the window.
	SetWindowPos(hwnd, nullptr, 0, 0, NewSize.cx, NewSize.cy, SWP_NOZORDER | SWP_NOMOVE);
}

void CWindow::Move(POINT NewPosition)
{
	//Change the coordinates of the window.
	SetWindowPos(hwnd, nullptr, NewPosition.x, NewPosition.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void CWindow::ResizeAndMove(SIZE NewSize, POINT NewPosition)
{
}

void CWindow::Close()
{
	SendMessage(hwnd, WM_CLOSE, 0, 0);
}

I32 CWindow::GetMaxWidth() const
{
	return 0;
}

I32 CWindow::GetMaxHeight() const
{
	return 0;
}

I32 CWindow::GetMinWidth() const
{
	return 0;
}

I32 CWindow::GetMinHeight() const
{
	return 0;
}

I32 CWindow::GetWidth() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.right - r.left;
}

I32 CWindow::GetHeight() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.bottom - r.top;
}

I32 CWindow::GetXPosition() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.left;
}

I32 CWindow::GetYPosition() const
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.top;
}

