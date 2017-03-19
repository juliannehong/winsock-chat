#pragma once
#include "CustomWindow.h"
#include "ObjectPtr.h"
#include "ChatPanel.h"

class CMainWindow :
	public CCustomWindow
{
	CObjectPtr<CChatPanel> cp;
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool InitializeWindowClass(LPWNDCLASSEX wcx);
	bool InitializeWindowCreateStruct(LPCREATESTRUCT cs);
	bool CreateChildWindows(HWND parent);
	bool ResizeChildWindows(RECT NewSize);

public:
	CMainWindow();
	~CMainWindow();
};

