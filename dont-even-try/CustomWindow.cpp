#include "CustomWindow.h"
#include "ObjectPtr.h"


LRESULT CCustomWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	CObjectPtr<CCustomWindow> ccw = CWindow::GetClassPointer(hwnd);
	switch(msg)
	{
	case WM_NCCREATE:
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lparam;
			ccw = (CCustomWindow*)cs->lpCreateParams;
			ccw->SavePointerToHandle(hwnd);
			//now forward it to the class.
			break;
		}
	case WM_NCDESTROY:
		{
			//remove the pointer.
			if(!ccw)
			{
				//the pointer has been nulled before destroying the window. 
				//This should never happen, and indicates a bug in our design.
				__debugbreak();
			}
			ccw->ClearPointerFromHandle(hwnd);
			//This is not forwarded - the class is no longer linked to a window.
			return 0;
		}
	}
	if(ccw)
	{
		return ccw->HandleMessage(hwnd, msg, wparam, lparam);
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool CCustomWindow::CreateWindowHandle(HWND & hwnd, HWND parent)
{
	hwnd = nullptr;
	//Is the window class already registered?
	WNDCLASS wc = { 0 };
	if(!GetClassInfo(GetModuleHandle(nullptr), GetWindowClassName(), &wc))
	{
		//Register the window class associated with this custom window.
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = GetWindowClassName();
		InitializeWindowClass(&wc);
		wc.lpfnWndProc = CCustomWindow::WndProc;
		if(RegisterClass(&wc) == 0)
		{
			return false;
		}
	}
	//we have a window class - now create the main window.
	CREATESTRUCT cs = { 0 };
	cs.hwndParent = parent;
	InitializeWindowCreateStruct(&cs);
	cs.hInstance = wc.hInstance;
	cs.lpCreateParams = this;
	if(parent != nullptr)
	{
		cs.style |= WS_CHILD;
	}
	cs.hwndParent = parent;
	cs.lpszClass = wc.lpszClassName;
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
		//UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}
	//SetWindowText(hwnd, cs.lpszName);
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
				CObjectPtr<CWindow> w = CWindow::GetClassPointer(hwnd);
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
	//no child windows by default.
	return true;
}

void CCustomWindow::ResizeChildWindows(RECT NewClientRect)
{
	//No resize logic is performed.
}

CCustomWindow::CCustomWindow()
{
}


CCustomWindow::~CCustomWindow()
{
}
