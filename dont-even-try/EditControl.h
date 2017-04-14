#pragma once
#include "StandardWindow.h"

class CEditControl :
	public CStandardWindow
{
	bool IsMultiline;
	bool HasVScroll;
	bool HasHScroll;
	void InitializeCreateStruct(LPCREATESTRUCT cs);

public:
	CEditControl(bool Multiline);
	~CEditControl();

	void SetEditText(LPTSTR Text);
};

