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

bool CWindow::SaveClassPointerToWindow(HWND Window)
{
	if(Window == nullptr)
	{
		if(hwnd == nullptr)
		{
			return false;
		}
		Window = hwnd;
	}
	AddRef();
	SetWindowLongPtr(Window, GWLP_USERDATA, (LONG_PTR)(this));
	mustrelease = true;
	return true;
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
		if(mustrelease)
		{
			if(hwnd)
			{
				SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
				hwnd = nullptr;
			}
			Release();
		}
		return false;
	}
	ShowWindow(hwnd, SW_RESTORE);
	UpdateWindow(hwnd);
	return true;
}

bool CWindow::OnResize(RECT NewSize)
{
	//We've been resized. change the dimensions of the window.
	//SetWindowPos(hwnd, nullptr, 0, 0, NewSize.right - NewSize.left, NewSize.bottom - NewSize.top, SWP_NOZORDER | SWP_NOMOVE);
	//now resize any child windows contained by this window.
	return ResizeChildWindows(NewSize);
}

bool CWindow::OnMove(RECT NewPosition)
{
	//move the window to the new position.
	SetWindowPos(hwnd, nullptr, NewPosition.left, NewPosition.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	return true;
}
