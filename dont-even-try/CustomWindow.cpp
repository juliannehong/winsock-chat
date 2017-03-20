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
			ccw->AddRef();
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ccw.GetInterface());
			return TRUE;
		}
	case WM_NCDESTROY:
		{
			//remove the pointer.
			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
			ccw->Release();
			return 0;
		}
	default:
		if(ccw)
		{
			return ccw->HandleMessage(hwnd, msg, wparam, lparam);
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
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
	if(wcx.hInstance == nullptr)
	{
		wcx.hInstance = GetModuleHandle(nullptr);
	}
	wcx.lpfnWndProc = CCustomWindow::WndProc;
	wcx.style |= (CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
	if(wcx.lpszClassName == nullptr)
	{
		return false;
	}
	wndclass = RegisterClassEx(&wcx);
	if(wndclass == 0)
	{
		return 0;
	}
	//we have a window class - now create the main window.
	CREATESTRUCT cs = { 0 };
	cs.hwndParent = parent;
	InitializeWindowCreateStruct(&cs);
	cs.hInstance = wcx.hInstance;
	cs.lpCreateParams = this;
	if(parent != nullptr)
	{
		cs.style |= WS_CHILD;
	}
	cs.lpszClass = (LPCWSTR)((UINT_PTR)0 | wndclass);
	hwnd = CreateWindowEx(cs.dwExStyle,
						  cs.lpszClass,
						  cs.lpszName,
						  cs.style,
						  cs.x, cs.y,
						  cs.cx, cs.cy,
						  cs.hwndParent,
						  cs.hMenu,
						  cs.hInstance,
						  cs.lpCreateParams);
	if(hwnd == nullptr)
	{
		return false;
	}
	return true;
}

LRESULT CCustomWindow::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_CREATE:
		{
			if(!CreateChildWindows(hwnd))
			{
				return FALSE;
			}
			return TRUE;
		}
	case WM_SIZE:
		{
			RECT newsz;
			GetClientRect(hwnd, &newsz);
			ResizeChildWindows(newsz);
			return 0;
		}
	case WM_COMMAND:
		{
			if(lparam)
			{
				CObjectPtr<CWindow> w = (CWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				OnChildNotify(w, HIWORD(wparam));
			}
		}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

bool CCustomWindow::CreateChildWindows(HWND parent)
{
	return true;
}

bool CCustomWindow::ResizeChildWindows(RECT NewClientRect)
{
	//default has no child windows.
	return true;
}

CCustomWindow::CCustomWindow()
{
}


CCustomWindow::~CCustomWindow()
{
}
