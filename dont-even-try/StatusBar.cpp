#include "StatusBar.h"



void CStatusBar::InitializeCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszClass = STATUSCLASSNAME;
	cs->style = WS_SIZEBOX;
}

CStatusBar::CStatusBar()
{
}


CStatusBar::~CStatusBar()
{
}
