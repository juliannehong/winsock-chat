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

	LPTSTR GetWindowClassName() const;
	void InitializeWindowClass(LPWNDCLASS wc);
	void InitializeWindowCreateStruct(LPCREATESTRUCT cs);
	bool CreateChildWindows(HWND parent);
	void ResizeChildWindows(RECT NewSize);

	void OnMenuItem(U32 MenuItemID, bool IsAccelerator);
public:
	CMainWindow();
	~CMainWindow();
};

