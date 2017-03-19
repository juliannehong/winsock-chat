#include "MainWindow.h"



LRESULT CMainWindow::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return CCustomWindow::HandleMessage(hwnd, msg, wparam, lparam);
}

bool CMainWindow::InitializeWindowClass(LPWNDCLASSEX wcx)
{
	wcx->hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcx->hIcon = LoadIcon(0, IDI_APPLICATION);
	wcx->hIconSm = wcx->hIcon;
	wcx->hCursor = LoadCursor(0, IDC_ARROW);
	wcx->lpszClassName = L"MainWindow";
	return true;
}

bool CMainWindow::InitializeWindowCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszName = L"winsock-chat";
	cs->dwExStyle = WS_EX_OVERLAPPEDWINDOW;
	cs->style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX;
	cs->x = CW_USEDEFAULT;
	cs->y = 0;
	cs->cx = 800;
	cs->cy = 600;
	return true;
}

bool CMainWindow::CreateChildWindows(HWND hwnd)
{
	cp->Create(hwnd);
	stat->Create(hwnd);

	return true;
}

bool CMainWindow::ResizeChildWindows(RECT NewSize)
{
	return (cp->Resize(NewSize) && stat->Resize(NewSize));
}

CMainWindow::CMainWindow() : 
	cp(new CChatPanel(), true),
	stat(new CStatusBar(), true)
{
}


CMainWindow::~CMainWindow()
{
}
