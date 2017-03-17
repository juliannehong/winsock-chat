#include "CustomWindow.h"
#include "ObjectPtr.h"


LRESULT CCustomWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CObjectPtr<CCustomWindow> ccw = (CCustomWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

}

bool CCustomWindow::CreateWindowHandle(HWND & hwnd, HWND parent)
{
	hwnd = nullptr;
	//Register the window class associated with this custom window.
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.cbWndExtra = sizeof(this);
	wcx.cbClsExtra = 0;
	wcx.hInstance = GetModuleHandle(nullptr); //This executable module.
	wcx.lpfnWndProc = CCustomWindow::WndProc;
	wcx.lpszMenuName = nullptr;
	wcx.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wcx.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcx.lpszClassName = GetClassNameString();
	wcx.hIcon = GetLargeIcon();
	wcx.hIconSm = GetSmallIcon();
	wcx.hCursor = GetCursorIcon();
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
						  GetMenuHandle(),
						  GetModuleHandle(nullptr),
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

CCustomWindow::CCustomWindow()
{
}


CCustomWindow::~CCustomWindow()
{
}
