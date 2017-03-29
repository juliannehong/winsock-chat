#include "MainWindow.h"
#include "resource.h"


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

LPTSTR CMainWindow::GetWindowClassName() const
{
	return TEXT("CMainWindow");
}

void CMainWindow::InitializeWindowClass(LPWNDCLASS wc)
{
	wc->hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wc->hIcon = LoadIcon(0, IDI_APPLICATION);
	wc->hCursor = LoadCursor(0, IDC_ARROW);
	wc->lpszMenuName = TEXT("MainMenu");
	wc->style = CS_HREDRAW | CS_VREDRAW;
}

void CMainWindow::InitializeWindowCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszName = TEXT("winsock-chat");
	cs->dwExStyle = 0;
	cs->style = WS_OVERLAPPEDWINDOW;
	cs->x = CW_USEDEFAULT;
	cs->y = 0;
	cs->cx = 800;
	cs->cy = 600;
}

bool CMainWindow::CreateChildWindows(HWND hwnd)
{
	return cp->Create(hwnd) && stat->Create(hwnd);
}

void CMainWindow::ResizeChildWindows(RECT NewSize)
{
	SIZE nsz;
	nsz.cx = NewSize.right - NewSize.left;
	nsz.cy = NewSize.bottom - NewSize.top - (stat->GetHeight());
	cp->Resize(nsz);
	POINT np;
	np.x = NewSize.left;
	np.y = nsz.cy;
	nsz.cy = stat->GetHeight();
	stat->Move(np);
	stat->Resize(nsz);
}

void CMainWindow::OnMenuItem(U32 MenuItemID, bool IsAccelerator)
{
	switch(MenuItemID)
	{
	case ID_FILE_EXIT:
		Close();
		break;
	case ID_EDIT_CUT:
	case ID_EDIT_COPY:
	case ID_EDIT_PASTE:
	case ID_EDIT_SETTINGS:
	case ID_CONNECT_SERVER:
	case ID_CONNECT_CLIENT:
	case ID_CONNECT_CHANGEALIAS:
	case ID_HELP_ABOUT:
	default:
		MessageBeep(0);
		break;
	}
}

CMainWindow::CMainWindow() : 
	cp(new CChatPanel(), true),
	stat(new CStatusBar(), true)
{
	//AddChildWindow(new CChatPanel(), XMFLOAT2(0,0), XMFLOAT2(0,0), LATCH_PARENT_TOP | LATCH_PARENT_LEFT);
	//AddChildWindow(new CStatusBar(), XMFLOAT2(0,0), XMFLOAT2(0,0), LATCH_PARENT_LEFT | LATCH_PARENT_BOTTOM | FIXED_HEIGHT);
}


CMainWindow::~CMainWindow()
{
}
