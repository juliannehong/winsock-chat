#pragma once
#include <vector>
#include "CustomWindow.h"
#include "PanelContainerSupport.h"

enum SeparatorType
{
	Separator_Vertical,
	Separator_Horizontal,
};


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
	vector<PanelContainer::Node> Layout;
	//Drawing parameters
	PanelContainer::DrawingParams drawparams;
	//Tracking parameters
	PanelContainer::ObjectID TrackedObject;
	RECT TrackingRect;
	RECT OriginalRect;
	RECT TrackingLimit;
	POINT TrackingOffset;
	bool istracking;

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	LPTSTR GetWindowClassName() const;

	void InitializeWindowClass(LPWNDCLASS wc);
	void InitializeWindowCreateStruct(LPCREATESTRUCT cs);

	bool IsTrackingEnabled() const;
	void StartTracking(PanelContainer::ObjectID obj);
	void StopTracking(bool DiscardChanges = false);

	RECT GetChildRect(U32 index, bool IsLeft) const;
	RECT GetLimitRect(U32 index) const;


	void RecomputeBounds_Rec(const RECT & rBounds, U32 Index);
	void RecomputeBounds(const RECT & rBounds, PanelContainer::Node &n);

	PanelContainer::ObjectID GetObjectAtPoint(POINT pt) const;
	PanelContainer::ObjectID GetObjectAtPoint_Rec(POINT pt, U32 index) const;

	void SetCursorFromTrackedObject(PanelContainer::ObjectID o) const;

	U32 ConvertPointToIndex(POINT pt) const;

	void RecomputeLayout(U32 Index = 0);
	void MovePanels_Rec(HDWP dwp, U32 index);

	void DrawClientArea(HDC hdc) const;
	void DrawClientAreaRec(HDC hdc, U32 currentindex) const;
	void DrawBorder(HDC hdc, const RECT& BorderRect, HBRUSH TopLeft, HBRUSH BottomRight) const;

	void InvertTracker(RECT trackRect) const;
public:
	CPanelContainer();
	virtual ~CPanelContainer();

	U32 GetNumPanels() const;
	bool IsLayoutIndexValid(U32 Index) const;
	bool IsPanelIndexValid(U32 Index) const;
	bool IsPointValid(POINT pt) const;
	CObjectPtr<CWindow> GetPanel(U32 Index) const;
	CObjectPtr<CWindow> GetPanelAtPoint(POINT pt) const;

	U32 AddSeparator(U32 Parent, SeparatorType Type, float StartingDivision, bool LeftHandChild);
	U32 AddPanel(U32 Parent, CObjectPtr<CWindow> NewPanel, bool LeftHandChild);

	void EnableDebugMode(bool Enable);
};

