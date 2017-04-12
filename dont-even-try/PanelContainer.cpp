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
				pt.Offset(m_ptTrackOffset); // pt is the upper right of hit detect
											// limit the point to the valid split range
				if(pt.y < m_rectLimit.top)
					pt.y = m_rectLimit.top;
				else if(pt.y > m_rectLimit.bottom)
					pt.y = m_rectLimit.bottom;
				if(pt.x < m_rectLimit.left)
					pt.x = m_rectLimit.left;
				else if(pt.x > m_rectLimit.right)
					pt.x = m_rectLimit.right;

				switch(trackedobject.GetObjectType())
				{
				case Object_HorizontalSplit:
					{
						if(m_rectTracker.top != pt.y)
						{
							OnInvertTracker(m_rectTracker);
							m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
							OnInvertTracker(m_rectTracker);
						}
					}
					break;
				case Object_VerticalSplit:
					{
						if(m_rectTracker.left != pt.x)
						{
							OnInvertTracker(m_rectTracker);
							m_rectTracker.OffsetRect(pt.x - m_rectTracker.left, 0);
							OnInvertTracker(m_rectTracker);
						}
					}
				}
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
			CObjectPtr<CWindow> panel = GetPanelAtPoint(pt);
			if(panel)
			{
				return SendMessage(panel->GetWindowHandle(), msg, wparam, lparam);
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

void CPanelContainer::StartTracking(PanelContainer::TrackedObject obj)
{
}

void CPanelContainer::StopTracking(bool DiscardChanges)
{
}

TrackedObject CPanelContainer::GetObjectToTrack(POINT pt) const
{
	//Traverse the BSP tree and find the splitter it hits.
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

U32 CPanelContainer::ConvertPointToIndex(POINT pt) const
{
	//Traverse the BSP tree and see which cell the point falls into.
}

void CPanelContainer::RecomputeLayout()
{

}

void CPanelContainer::DrawClientArea(HDC hdc)
{
	RECT r = { 0 };
	GetClientRect(GetWindowHandle(), &r);
	InflateRect(&r, -drawparams.BorderX, -drawparams.BorderY);
	int cxInside = r.right;
	int cyInside = r.bottom;
	GetClientRect(GetWindowHandle(), &r);
	//go through each cell, and build the borders as needed.

}

CPanelContainer::CPanelContainer()
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

bool CPanelContainer::IsPointValid(POINT pt) const
{
	RECT r;
	GetClientRect(GetWindowHandle(), &r);
	return PtInRect(&r, pt) == TRUE;
}

CObjectPtr<CWindow> CPanelContainer::GetPanel(U32 Index) const
{
	if(IsIndexValid(Index))
	{
		return panels[Index];
	}
	return nullptr;
}

CObjectPtr<CWindow> CPanelContainer::GetPanelAtPoint(POINT pt) const
{
	return GetPanel(ConvertPointToIndex(pt));
}

