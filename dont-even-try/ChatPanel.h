#pragma once
#include "PanelContainer.h"

class CChatPanel :
	public CPanelContainer
{
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	CChatPanel();
	~CChatPanel();

};

