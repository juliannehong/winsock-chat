#include "StatusBar.h"



bool CStatusBar::CreateWindowHandle(HWND & hwnd, HWND parent)
{
	hwnd = CreateWindowEx(0,
						  STATUSCLASSNAME,
						  L"",
						  WS_CHILD,
						  0, 0,
						  0, 0,
						  parent,
						  nullptr,
						  (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
						  nullptr);
	if(hwnd == nullptr)
	{
		return false;
	}
	SetWindowLongPtr(hwnd, )
}

CStatusBar::CStatusBar()
{
}


CStatusBar::~CStatusBar()
{
}
