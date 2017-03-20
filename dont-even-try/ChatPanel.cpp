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

void CChatPanel::ResizeChildWindows(RECT NewSize)
{
	
}

LRESULT CChatPanel::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	}
	return CCustomWindow::HandleMessage(hwnd, msg, wparam, lparam);
}

CChatPanel::CChatPanel()
{
}


CChatPanel::~CChatPanel()
{
}
