#include "ChatPanel.h"

LRESULT CChatPanel::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return CCustomWindow::HandleMessage(hwnd, msg, wparam, lparam);
}

CChatPanel::CChatPanel()
{
}


CChatPanel::~CChatPanel()
{
}

