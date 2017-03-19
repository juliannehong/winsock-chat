#pragma once
#include "StandardWindow.h"
class CStatusBar :
	public CStandardWindow
{

	bool InitializeCreateStruct(LPCREATESTRUCT cs);

public:
	CStatusBar();
	~CStatusBar();
};

