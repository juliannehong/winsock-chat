#pragma once
#include "StandardWindow.h"
class CStatusBar :
	public CStandardWindow
{

	void InitializeCreateStruct(LPCREATESTRUCT cs);

public:
	CStatusBar();
	~CStatusBar();
};

