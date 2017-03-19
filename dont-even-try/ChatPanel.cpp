#include "ChatPanel.h"



bool CChatPanel::InitializeWindowClass(LPWNDCLASSEX wcx)
{
	wcx->hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wcx->hCursor = LoadCursor(0, IDC_ARROW);
	wcx->lpszClassName = L"ChatPanel";
	return true;
}

bool CChatPanel::InitializeWindowCreateStruct(LPCREATESTRUCT cs)
{
	RECT r = { 0 };
	GetClientRect(cs->hwndParent, &r);
	cs->lpszName = L"";
	cs->style = WS_VSCROLL | WS_VISIBLE;
	cs->x = r.left;
	cs->y = r.top;
	cs->cx = r.right;
	cs->cy = r.bottom;
	return true;
}

bool CChatPanel::ResizeChildWindows(RECT NewSize)
{
	return true;
}

LRESULT CChatPanel::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_COMMAND:
		{
			if(lparam)
			{
				//get the associated sub item.
				
				return 0;
			}
			else
			{
				//OnMenuItem(HIWORD(wparam), LOWORD(wparam));
				return 0;
			}
		}
	}
	return CCustomWindow::HandleMessage(hwnd, msg, wparam, lparam);
}

CChatPanel::CChatPanel()
{
}


CChatPanel::~CChatPanel()
{
}
