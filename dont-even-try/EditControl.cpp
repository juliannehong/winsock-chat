#include "EditControl.h"



void CEditControl::InitializeCreateStruct(LPCREATESTRUCT cs)
{
	cs->lpszClass = WC_EDIT;
	cs->style = 0;
	cs->style |= HasVScroll ? WS_VSCROLL : 0;
	cs->style |= HasHScroll ? WS_HSCROLL : 0;
	cs->style |= IsMultiline ? ES_MULTILINE : 0;
}

CEditControl::CEditControl(bool Multiline) : 
	IsMultiline(Multiline),
	HasVScroll(Multiline),
	HasHScroll(false)
{
}


CEditControl::~CEditControl()
{
}

void CEditControl::SetEditText(LPTSTR Text)
{
	SetWindowText(GetWindowHandle(), Text);
}
