#include "StatusBar.h"



bool CStatusBar::InitializeCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszClass = STATUSCLASSNAME;
	return true;
}

CStatusBar::CStatusBar()
{
}


CStatusBar::~CStatusBar()
{
}
