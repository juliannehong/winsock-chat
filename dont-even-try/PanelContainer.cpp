#include "PanelContainer.h"
using namespace PanelContainer;
/*
ON_WM_SETCURSOR()
ON_WM_MOUSEMOVE()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONUP()
ON_WM_KEYDOWN()
ON_WM_SIZE()
ON_WM_HSCROLL()
ON_WM_VSCROLL()
ON_WM_NCCREATE()
ON_WM_SYSCOMMAND()
ON_WM_CANCELMODE()
ON_MESSAGE_VOID(WM_DISPLAYCHANGE,CSplitterWnd::OnDisplayChange)
ON_MESSAGE_VOID(WM_WININICHANGE, CSplitterWnd::OnDisplayChange)
ON_MESSAGE_VOID(WM_SETTINGCHANGE,CSplitterWnd::OnDisplayChange)
ON_WM_PRINTCLIENT()
ON_WM_MOUSEWHEEL()
*/

LRESULT CPanelContainer::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_SETCURSOR:
		assert(hwnd == GetWindowHandle());
		{
			if(!IsTrackingEnabled() && LOWORD(lparam) == HTCLIENT)
			{
				return TRUE; //we will do this ourselves.
			}
			break; //Let the parent handle it.
		}
	case WM_PAINT:
		{
			if(GetUpdateRect(hwnd, nullptr, FALSE))
			{
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(hwnd, &ps);
				DrawClientArea(dc);
				EndPaint(hwnd, &ps);
			}
			return 0;
		}
	case WM_SIZE:
		{
			if(wparam != SIZE_MINIMIZED && HIWORD(lparam) > 0 && LOWORD(lparam) > 0)
			{
				RecomputeLayout();
			}
			break; //pass it to the parent.
		}
	case WM_CANCELMODE:
		StopTracking(true);
		return 0;
	case WM_NCCREATE:
		{
			// Remove WS_EX_CLIENTEDGE style from the parent - we take care of it.
			HWND p = GetParent(hwnd);
			DWORD style = GetWindowLongPtr(hwnd, GWL_EXSTYLE) & ~(WS_EX_CLIENTEDGE);
			SetWindowLongPtr(hwnd, GWL_EXSTYLE, style);
			SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			return TRUE;
		}
	case WM_COMMAND:
	case WM_NOTIFY:
		//forward child panel notifications to the parent.
		return SendMessage(GetParent(hwnd), msg, wparam, lparam);
	case WM_DISPLAYCHANGE:
	case WM_WININICHANGE:
		//Display changed.
		if(!IsIconic(hwnd) && IsWindowVisible(hwnd))
		{
			RecomputeLayout();
		}
		return 0;
	case WM_PRINTCLIENT:
		//paint the client window onto a predefined DC.
		if(wparam == NULL)
		{
			return -1;
		}
		DrawClientArea((HDC)wparam);
		return 0;
	case WM_MOUSEMOVE:
		{
			if(GetCapture() != GetWindowHandle())
			{
				StopTracking();
			}
			//mouse movement
			POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			if(IsTrackingEnabled())
			{

			}
			else
			{
				SetCursorFromTrackedObject(GetObjectToTrack(pt));
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(!IsTrackingEnabled())
			{
				POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
				StartTracking(GetObjectToTrack(pt));
			}
			return 0;
		}
	case WM_LBUTTONUP:
		StopTracking();
		return 0;
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		{
			//subwindow scrolling.
			//find out the sub window the mouse is over, and forward the message to it.
			POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			U32 index = ConvertPointToIndex(pt);
			if(index != -1)
			{
				return SendMessage(panels.at(index)->GetWindowHandle(), msg, wparam, lparam);
			}
			break;
		}
	case WM_SYSCOMMAND:
		//Forward SC_SIZE to the parent.
		if((wparam & 0xFFF0) == SC_SIZE)
		{
			return SendMessage(GetParent(hwnd), msg, wparam, lparam);
		}
		break;
	}
	return ParentClass::HandleMessage(hwnd, msg, wparam, lparam);
}

LPTSTR CPanelContainer::GetWindowClassName() const
{
	return TEXT("CPanelContainer");
}

void CPanelContainer::InitializeWindowClass(LPWNDCLASS wc)
{
	wc->hCursor = LoadCursor(nullptr, IDC_ARROW);
}

void CPanelContainer::InitializeWindowCreateStruct(LPCREATESTRUCT cs)
{
	cs->style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL;
}

bool CPanelContainer::IsTrackingEnabled() const
{
	return istracking;
}

TrackedObject CPanelContainer::GetObjectToTrack(POINT pt) const
{

	return TrackedObject();
}

void CPanelContainer::SetCursorFromTrackedObject(TrackedObject o)
{
	switch(o.GetObjectType())
	{
	case Object_HorizontalSplit:
		SetCursor(cglobals.hsize);
		break;
	case Object_VerticalSplit:
		SetCursor(cglobals.vsize);
		break;
	default:
		SetCursor(cglobals.arrow);
		break;
	}
}

POINT CPanelContainer::ConvertPointToCell(POINT pt) const
{
	POINT ret;
	//LERP here. we need the result BETWEEN 0 and CCount/RCount, NOT just divided by them!
	//LERP from [r.left, r.right) -> [0, CCount) and [r.top, r.bottom) -> [0, RCount)
	//ret.x = pt.x / ColumnCount;
	//ret.y = pt.y / RowCount;
	return ret;
}

U32 CPanelContainer::ConvertPointToIndex(POINT pt) const
{
	return U32();
}

U32 CPanelContainer::ConvertCellToIndex(U8 Row, U8 Column) const
{
	return U32();
}

void CPanelContainer::RecomputeLayout()
{
}

void CPanelContainer::DrawClientArea(HDC hdc)
{
	RECT r = { 0 };
	GetClientRect(GetWindowHandle(), &r);
	InflateRect(&r, -BorderX, -BorderY);
	int cxInside = r.right;
	int cyInside = r.bottom;
	GetClientRect(GetWindowHandle(), &r);
	//go through each cell, and build the borders as needed.

}

CPanelContainer::CPanelContainer(U8 MaxRowCount, U8 MaxColumnCount)
{
}

CPanelContainer::~CPanelContainer()
{
}

U32 CPanelContainer::GetNumPanels() const
{
	return (U32)panels.size();
}

bool CPanelContainer::IsIndexValid(U32 Index) const
{
	return (Index < panels.size() && Index >= 0);
}

bool CPanelContainer::IsCellValid(U8 Row, U8 Column) const
{
	return ((Row < 0 || Row > UsedRows) &&
		(Column < 0 || Column > UsedColumns));
}

bool CPanelContainer::IsCellSet(U8 Row, U8 Column) const
{
	return (IsCellValid(Row, Column) && (ConvertCellToIndex(Row, Column) != -1));
}

CObjectPtr<CWindow> CPanelContainer::GetPanel(U32 Index) const
{
	if(IsIndexValid(Index))
	{
		return panels[Index];
	}
	return nullptr;
}

CObjectPtr<CWindow> CPanelContainer::GetPanelAtCell(U8 Row, U8 Column) const
{
	if(IsCellSet(Row, Column))
	{
		return GetPanel(ConvertCellToIndex(Row, Column));
	}
	return nullptr;
}

CPanelContainerGlobals::CPanelContainerGlobals() :
	arrow(LoadCursor(nullptr, IDC_ARROW)),
	hsize(LoadCursor(nullptr, IDC_SIZEWE)),
	vsize(LoadCursor(nullptr, IDC_SIZENS)),
	ButtonFace(GetSysColorBrush(COLOR_BTNFACE)),
	ButtonHilight(GetSysColorBrush(COLOR_BTNHIGHLIGHT)),
	ButtonShadow(GetSysColorBrush(COLOR_BTNSHADOW)),
	WindowFrame(GetSysColorBrush(COLOR_WINDOWFRAME)),
	cxVScroll(GetSystemMetrics(SM_CXVSCROLL)),
	cyHScroll(GetSystemMetrics(SM_CYHSCROLL))
{
}

CPanelContainerGlobals::~CPanelContainerGlobals()
{
	//don't destroy the cursors - Windows owns them.
	//see https://msdn.microsoft.com/en-us/library/windows/desktop/ms648386(v=vs.85).aspx, Remarks section.

	//we can delete the brushes. Since the brushes are not affected by deletion, this 
	// is safe to do, and will allow us to use custom brushes later if we wish.
	DeleteObject(ButtonFace);
	DeleteObject(ButtonHilight);
	DeleteObject(ButtonShadow);
	DeleteObject(WindowFrame);
}
