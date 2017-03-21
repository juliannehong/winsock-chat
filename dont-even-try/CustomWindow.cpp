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
			ccw->SavePointerToHandle(hwnd);
			return TRUE;
		}
	case WM_NCDESTROY:
		{
			//remove the pointer.
			ccw->ClearPointerFromHandle(hwnd);
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
	if(!RegisterClassEx(&wcx))
	{
		return false;
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
	cs.lpszClass = wcx.lpszClassName;
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
		UnregisterClass(wcx.lpszClassName, wcx.hInstance);
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
			else
			{
				OnMenuItem(LOWORD(wparam), (bool)HIWORD(wparam));
			}
			return 0;
		}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void CCustomWindow::OnChildNotify(CObjectPtr<CWindow> child, U32 NotificationCode)
{
	//default does nothing with any child windows we have.
}

void CCustomWindow::OnMenuItem(U32 MenuItemID, bool IsAccelerator)
{
	//default has no menu items.
}

bool CCustomWindow::CreateChildWindows(HWND parent)
{
	//go over all child windows and create them.
}

void CCustomWindow::ResizeChildWindows(RECT NewClientRect)
{
	//tell the layout engine to recompute the child window extents.

	//Now go over each child window and update the size.

}

CCustomWindow::CCustomWindow()
{
}


CCustomWindow::~CCustomWindow()
{
}
