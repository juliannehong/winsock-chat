#pragma once
#include "CustomWindow.h"
#include "ObjectPtr.h"
#include "ChatPanel.h"
#include "StatusBar.h"

class CMainWindow :
	public CCustomWindow
{
	CObjectPtr<CChatPanel> cp;
	CObjectPtr<CStatusBar> stat;
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool InitializeWindowClass(LPWNDCLASSEX wcx);
	bool InitializeWindowCreateStruct(LPCREATESTRUCT cs);
	bool CreateChildWindows(HWND parent);
	bool ResizeChildWindows(RECT NewSize);

public:
	CMainWindow();
	~CMainWindow();
};

