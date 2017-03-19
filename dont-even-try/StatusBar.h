#pragma once
#include "Window.h"

class CStatusBar :
	public CWindow
{

	bool CreateWindowHandle(HWND &hwnd, HWND parent);

public:
	CStatusBar();
	~CStatusBar();
};

