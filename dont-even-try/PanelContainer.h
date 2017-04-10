#pragma once
#include "CustomWindow.h"
#include <vector>

namespace PanelContainer
{
	struct CPanelContainerGlobals
	{
		HCURSOR arrow;
		HCURSOR hsize;
		HCURSOR vsize;

		CPanelContainerGlobals();
		~CPanelContainerGlobals();
	};

	enum ObjectType : U8
	{
		Object_None,
		Object_VerticalSplit,
		Object_HorizontalSplit,
	};

	struct TrackedObject
	{
	private:
		U32 Id;
	public:
		FORCEINLINE U8 GetObjectType(TrackedObject to)
		{
			return (U8)(((Id) >> 24) & 0xFF);
		}
		FORCEINLINE U32 GetObjectIndex(TrackedObject to)
		{
			return (U32)(((Id) & 0xFF'FFFF));
		}
		FORCEINLINE void SetObjectType(ObjectType t)
		{
			Id &= 0x00FF'FFFF;
			Id |= ((U8)t) << 24;

		}
		FORCEINLINE void SetObjectIndex(U32 Index)
		{
			Id &= 0xFF00'0000;
			Id |= (Index & 0x00FF'FFFF);
		}
		operator U32()
		{
			return Id;
		}
	};
};

class CPanelContainer :
	public CCustomWindow
{
	typedef CCustomWindow ParentClass;

	static PanelContainer::CPanelContainerGlobals cglobals;
	std::vector<CObjectPtr<CWindow>> panels;
	U32* IndexGrid;
	U8 RowCount;
	U8 ColumnCount;
	U8 UsedRows;
	U8 UsedColumns;
	U8 BorderX;
	U8 BorderY;
	bool istracking;

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	LPTSTR GetWindowClassName() const;

	void InitializeWindowClass(LPWNDCLASS wc);
	void InitializeWindowCreateStruct(LPCREATESTRUCT cs);

	bool IsTrackingEnabled();
	void StartTracking(PanelContainer::TrackedObject obj);
	void StopTracking(bool DiscardChanges = false);

	PanelContainer::TrackedObject GetObjectToTrack(POINT pt);
	void SetCursorFromTrackedObject(PanelContainer::TrackedObject o);

	POINT ConvertPointToCell(POINT pt);
	U32 ConvertPointToIndex(POINT pt);

	void RecomputeLayout();

	void DrawClientArea(HDC hdc);

public:
	CPanelContainer(U8 MaxRowCount, U8 MaxColumnCount);
	virtual ~CPanelContainer();

	U32 GetNumPanels() const;
	bool IsIndexValid(U32 Index) const;
	bool IsCellValid(U8 Row, U8 Column) const;
	bool IsCellSet(U8 Row, U8 Column) const;
	CObjectPtr<CWindow> GetPanel(U32 Index) const;
	CObjectPtr<CWindow> GetPanelAtCell(U8 Row, U8 Column) const;
};

