#include "StatusBar.h"



bool CStatusBar::InitializeCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszClass = STATUSCLASSNAME;
	cs->style = WS_SIZEBOX;
	return true;
}

CStatusBar::CStatusBar()
{
}


CStatusBar::~CStatusBar()
{
}
