#pragma once
#include "CustomWindow.h"
class CChatPanel :
	public CCustomWindow
{
	bool InitializeWindowClass(LPWNDCLASSEX wcx);
	bool InitializeWindowCreateStruct(LPCREATESTRUCT cs);
	void ResizeChildWindows(RECT NewSize);
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	CChatPanel();
	~CChatPanel();
};

