#pragma once
#include "CustomWindow.h"
#include <vector>

class CPanelContainer :
	public CCustomWindow
{
	typedef CCustomWindow ParentClass;
	std::vector<CObjectPtr<CWindow>> panels;

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	LPTSTR GetWindowClassName() const;

	void InitializeWindowClass(LPWNDCLASS wc);
	void InitializeWindowCreateStruct(LPCREATESTRUCT cs);

	bool IsTrackingEnabled();
	void StartTracking();
	void StopTracking(bool DiscardChanges = false);

	void RecomputeLayout();

public:
	CPanelContainer();
	virtual ~CPanelContainer();

	U32 GetNumPanels() const;
	bool IsIndexValid(U32 Index) const;
	CObjectPtr<CWindow> GetPanel(U32 Index) const;
};

