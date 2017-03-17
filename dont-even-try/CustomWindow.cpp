#include "CustomWindow.h"
#include "ObjectPtr.h"


LRESULT CCustomWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CObjectPtr<CCustomWindow> ccw = (CCustomWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch(msg)
	{
	case WM_NCCREATE:
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lparam;
			ccw = (CCustomWindow*)cs->lpCreateParams;
			if(!ccw->SaveClassPointerToWindow(hwnd))
			{
				return FALSE;
			}
			return FALSE;
		}

	}
}

bool CCustomWindow::CreateWindowHandle(HWND & hwnd, HWND parent)
{
	TCHAR defaultname[32] = { 0 };
	hwnd = nullptr;
	//Register the window class associated with this custom window.
	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(wcx);
	InitializeWindowClass(&wcx);
	wcx.cbWndExtra = sizeof(this);
	wcx.lpfnWndProc = CCustomWindow::WndProc;
	wcx.style |= (CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
	if(wcx.lpszClassName == nullptr)
	{
		wcx.lpszClassName = defaultname;
	}
	wndclass = RegisterClassEx(&wcx);
	if(wndclass == 0)
	{
		return 0;
	}
	//we have a window class - now create the main window.
	hwnd = CreateWindowEx(GetExtendedWindowStyle(), 
						  (LPCWSTR)(0l | wndclass),
						  GetInitialWindowTitle(),
						  GetWindowStyle() | (parent != nullptr) ? WS_CHILD : 0,
						  0, 0,
						  0, 0,
						  parent,
						  nullptr,
						  wcx.hInstance,
						  (LPVOID)this);
	if(hwnd == nullptr)
	{
		return false;
	}
	return true;
}

bool CCustomWindow::DestroyWindowHandle(HWND hwnd)
{
	if(hwnd == nullptr)
	{
		return false;
	}
	if(!DestroyWindow(hwnd))
	{
		return false;
	}
	return true;
}

bool CCustomWindow::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return false;
}

CCustomWindow::CCustomWindow()
{
}


CCustomWindow::~CCustomWindow()
{
}
