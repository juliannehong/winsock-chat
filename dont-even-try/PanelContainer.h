#pragma once
#include "CustomWindow.h"
#include <vector>

class CPanelContainer :
	public CCustomWindow
{
	std::vector<CObjectPtr<CWindow>> panels;

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool InitializeWindowClass(LPWNDCLASSEX wcx);
	bool InitializeWindowCreateStruct(LPCREATESTRUCT cs);

public:
	CPanelContainer();
	virtual ~CPanelContainer();

	U32 GetNumPanels() const;
	bool IsIndexValid(U32 Index) const;
	CObjectPtr<CWindow> GetPanel(U32 Index) const;
};

