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

bool CMainWindow::CreateChildWindows()
{
	bool ret = cp->Create(this);
	ret &= stat->Create(this);
	ret &= edit1->Create(cp);
	ret &= edit2->Create(cp);
	ret &= edit3->Create(cp);
	edit1->SetEditText(L"Hello World 1");
	edit2->SetEditText(L"Hello World 2");
	edit3->SetEditText(L"Hello World 3");
	U32 main = cp->AddSeparator(0, Separator_Vertical, 0.25f, false);
	cp->AddPanel(main, edit1, true);
	U32 sub = cp->AddSeparator(main, Separator_Horizontal, 0.75f, false);
	cp->AddPanel(sub, edit2, true);
	cp->AddPanel(sub, edit3, false);
	return ret;
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
	stat->ResizeAndMove(nsz, np);
}

void CMainWindow::OnMenuItem(U32 MenuItemID, bool IsAccelerator)
{
	switch(MenuItemID)
	{
	case ID_FILE_EXIT:
		Close();
		break;
	case ID_EDIT_DEBUGON:
		cp->EnableDebugMode(true);
		EnableMenuItem(GetMenu(GetWindowHandle()), ID_EDIT_DEBUGOFF, MF_ENABLED);
		EnableMenuItem(GetMenu(GetWindowHandle()), ID_EDIT_DEBUGON, MF_GRAYED);
		break;
	case ID_EDIT_DEBUGOFF:
		cp->EnableDebugMode(false);
		EnableMenuItem(GetMenu(GetWindowHandle()), ID_EDIT_DEBUGON, MF_ENABLED);
		EnableMenuItem(GetMenu(GetWindowHandle()), ID_EDIT_DEBUGOFF, MF_GRAYED);
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
	stat(new CStatusBar(), true),
	edit1(new CEditControl(true), true),
	edit2(new CEditControl(true), true),
	edit3(new CEditControl(true), true)
{
}


CMainWindow::~CMainWindow()
{
}
