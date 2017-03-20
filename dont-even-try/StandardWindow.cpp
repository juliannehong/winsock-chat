#include "StandardWindow.h"

LRESULT CStandardWindow::SubclassProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR SubclassID, DWORD_PTR refdata)
{
	if(msg == WM_NCDESTROY)
	{
		RemoveWindowSubclass(hwnd, CStandardWindow::SubclassProc, SubclassID);
		CObjectPtr<CStandardWindow> csw = (CStandardWindow*)SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
		csw->Release();
	}
	return DefSubclassProc(hwnd, msg, wparam, lparam);
}

bool CStandardWindow::CreateWindowHandle(HWND & hWindow, HWND parent)
{
	CREATESTRUCT cs = { 0 };
	InitializeCreateStruct(&cs);
	if(parent)
	{
		cs.style |= WS_CHILD;
	}
	hWindow = CreateWindowEx(cs.dwExStyle,
							 cs.lpszClass,
							 cs.lpszName,
							 cs.style,
							 cs.x,
							 cs.y,
							 cs.cx,
							 cs.cy,
							 parent,
							 nullptr,
							 (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
							 nullptr);
	if(hWindow == nullptr)
	{
		return false;
	}
	//hook WM_NCDESTROY to remove our class pointer and drop the refcount.
	//this way we won't leak resources.
	SavePointerToHandle(hWindow);
	SetWindowSubclass(hWindow, CStandardWindow::SubclassProc, 0x10105050, 0);
	return true;
}

CStandardWindow::CStandardWindow()
{
}


CStandardWindow::~CStandardWindow()
{
}
