#pragma once
#include <vector>
#include "CustomWindow.h"
#include "PanelContainerSupport.h"

class CPanelContainer :
	public CCustomWindow
{
	typedef CCustomWindow ParentClass;

	//Globals for the entire class
	static PanelContainer::CPanelContainerGlobals cglobals;
	//Panels added to the container
	std::vector<CObjectPtr<CWindow>> panels;
	//the layout of the panels 
	// (BSP, technically a k-d tree as the planes are axis-aligned).
	PanelContainer::CPanelTree Layout;
	//Drawing parameters
	PanelContainer::DrawingParams drawparams;
	//Tracking parameters
	bool istracking;

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	LPTSTR GetWindowClassName() const;

	void InitializeWindowClass(LPWNDCLASS wc);
	void InitializeWindowCreateStruct(LPCREATESTRUCT cs);

	bool IsTrackingEnabled() const;
	void StartTracking(PanelContainer::TrackedObject obj);
	void StopTracking(bool DiscardChanges = false);

	PanelContainer::TrackedObject GetObjectToTrack(POINT pt) const;
	void SetCursorFromTrackedObject(PanelContainer::TrackedObject o);

	U32 ConvertPointToIndex(POINT pt) const;

	void RecomputeLayout();

	void DrawClientArea(HDC hdc);

public:
	CPanelContainer();
	virtual ~CPanelContainer();

	U32 GetNumPanels() const;
	bool IsIndexValid(U32 Index) const;
	bool IsPointValid(POINT pt) const;
	CObjectPtr<CWindow> GetPanel(U32 Index) const;
	CObjectPtr<CWindow> GetPanelAtPoint(POINT pt) const;
};

