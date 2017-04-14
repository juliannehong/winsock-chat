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
	assert(hwnd == GetWindowHandle());
	switch(msg)
	{
	case WM_SETCURSOR:
		{
			if(!IsTrackingEnabled() && LOWORD(lparam) == HTCLIENT)
			{
				//Tracking isn't enabled and we hit the client area. 
				// We will set the cursor to the correct value.
				return TRUE; 
			}
			break; //Let the parent deal with it.
		}
	case WM_PAINT:
		{
			//do we need to repaint anything?
			if(GetUpdateRect(hwnd, nullptr, FALSE))
			{
				//yup. Do so.
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(hwnd, &ps);
				DrawClientArea(dc);
				EndPaint(hwnd, &ps);
			}
			return 0;
		}
	case WM_GETMINMAXINFO:
		//NOTE - We should handle WM_GETMINMAXINFO to prevent our window from becoming too small 
		// to display our panels adequately.
		// For now, do nothing.
		break;
	case WM_SIZE:
		{
			if(wparam != SIZE_MINIMIZED && HIWORD(lparam) > 0 && LOWORD(lparam) > 0)
			{
				//we changed the size of the form. recompute everything.
				RecomputeLayout();
			}
			break; //pass it to the parent.
		}
	case WM_CANCELMODE:
		//discard the changes we have queued.
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
		//paint the client window onto a predefined DC (print screen/print preview/etc.)
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
				// pt is relative coordinates - transform them into real ones we can use.
				pt.x += TrackingOffset.x;
				pt.y += TrackingOffset.y;
				// Enforce the tracking limits for the coordinates.
				if(pt.y < TrackingLimit.top)
					pt.y = TrackingLimit.top;
				else if(pt.y > TrackingLimit.bottom)
					pt.y = TrackingLimit.bottom;
				if(pt.x < TrackingLimit.left)
					pt.x = TrackingLimit.left;
				else if(pt.x > TrackingLimit.right)
					pt.x = TrackingLimit.right;
				//Now update the tracking bars.
				switch(TrackedObject.GetObjectType())
				{
				case Object_HorizontalSplit:
					{
						if(TrackingRect.top != pt.y)
						{
							InvertTracker(TrackingRect);
							OffsetRect(&TrackingRect, 0, pt.y - TrackingRect.top);
							InvertTracker(TrackingRect);
						}
					}
					break;
				case Object_VerticalSplit:
					{
						if(TrackingRect.left != pt.x)
						{
							InvertTracker(TrackingRect);
							OffsetRect(&TrackingRect, pt.x - TrackingRect.left, 0);
							InvertTracker(TrackingRect);
						}
					}
				}
			}
			else
			{
				// no tracking - just fixup the cursor.
				SetCursorFromTrackedObject(GetObjectAtPoint(pt));
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(!IsTrackingEnabled())
			{
				POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
				//Start tracking the object we clicked on, if it's of any value to us.
				StartTracking(GetObjectAtPoint(pt));
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
			//find out the sub window the mouse is over.
			POINT pt = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			CObjectPtr<CWindow> panel = GetPanelAtPoint(pt);
			if(panel)
			{
				// and forward the message to it.
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

void CPanelContainer::StartTracking(ObjectID obj)
{
	if(obj.GetObjectType() == Object_Panel || obj.GetObjectType() == Object_None)
	{
		//do nothing here.
		return;
	}
	// Get the tracking limit, compute the tracking offset, and create the tracker bar rectangle.
	TrackingLimit = GetLimitRect(obj.GetObjectIndex());
	Node n = Layout.at(obj.GetObjectIndex());
	if(n.Flags & NodeFlag_HorizontalSeparator)
	{
		//horizontal bar.
		U32 cy = (drawparams.SplitterY - ((drawparams.BorderY << 1) - 1));
		TrackingOffset.y = (cy >> 1); //Middle of the bar.
		TrackingRect.left = TrackingLimit.left;
		TrackingRect.right = TrackingLimit.right;
		TrackingRect.top = TrackingLimit.top + n.RelativePosition;
		TrackingRect.bottom = TrackingRect.top + cy;
	}
	else
	{
		//vertical bar.
		U32 cx = (drawparams.SplitterX - ((drawparams.BorderX << 1) - 1));
		TrackingOffset.x = (cx >> 1); //Middle of the bar.
		TrackingRect.left = TrackingLimit.left + n.RelativePosition;
		TrackingRect.right = TrackingRect.left + cx;
		TrackingRect.top = TrackingLimit.top;
		TrackingRect.bottom = TrackingLimit.bottom;
	}
	//Now grab the mouse and the current window focus
	SetCapture(GetWindowHandle());
	SetFocus(GetWindowHandle());
	//Clear all pending updates
	RedrawWindow(GetWindowHandle(), nullptr, nullptr, RDW_ALLCHILDREN | RDW_UPDATENOW);
	//and start tracking.
	istracking = true;
	InvertTracker(TrackingRect);
	TrackedObject = obj;
	SetCursorFromTrackedObject(TrackedObject);
}

void CPanelContainer::StopTracking(bool DiscardChanges)
{
	if(!IsTrackingEnabled())
	{
		return;
	}
	ReleaseCapture();
	InvertTracker(TrackingRect);
	istracking = false;
	if(!DiscardChanges)
	{
		//check if the tracking rectangle even moved in the first place.
		//No check for now - difficult to do.
		OffsetRect(&TrackingRect, -1, -1);
		switch(TrackedObject.GetObjectType())
		{
		case Object_HorizontalSplit:
			//TODO: Horizontal bar movement.
			
			break;
		case Object_VerticalSplit:
			//TODO: Vertical bar movement.
			
			break;
		default:
			//this should never happen - we've bugged it if it does.
			__debugbreak();
			return;
		}
		RecomputeLayout();
	}
}

RECT CPanelContainer::GetChildRect(U32 index, bool IsLeft) const
{
	RECT ret;
	if(index == InvalidNodeIndex)
	{
		GetClientRect(GetWindowHandle(), &ret);
		return ret;
	}
	else
	{
		Node n = Layout.at(index);
		ret = GetChildRect(n.Parent, (n.Flags & NodeFlag_IsLeftChild));
		if(IsLeft)
		{
			//Cut away the left hand rectangle.
			if(n.Flags & NodeFlag_HorizontalSeparator)
			{
				ret.bottom += n.RelativePosition;
			}
			else
			{
				ret.right += n.RelativePosition;
			}
		}
		else
		{
			//Cut away the right hand rectangle.
			if(n.Flags & NodeFlag_HorizontalSeparator)
			{
				ret.top += n.RelativePosition + drawparams.SplitterY;
			}
			else
			{
				ret.left += n.RelativePosition + drawparams.SplitterX;
			}
		}
		return ret;
	}
}

RECT CPanelContainer::GetLimitRect(U32 index) const
{
	Node n = Layout.at(index);
	return GetChildRect(n.Parent, (n.Flags & NodeFlag_IsLeftChild));
}

void CPanelContainer::ComputeBounds(const RECT & rBounds, PanelContainer::Node n, RECT & rSeparator, RECT & rLeft, RECT & rRight) const
{
	rSeparator = rLeft = rRight = rBounds;
	if(n.Flags & NodeFlag_HorizontalSeparator)
	{
		//l, t, r, b
		rSeparator.top += n.RelativePosition;
		rSeparator.bottom = rSeparator.top + drawparams.SplitterY;
		rLeft.bottom = rSeparator.top;
		rRight.top = rSeparator.bottom;
	}
	else
	{
		rSeparator.left += n.RelativePosition;
		rSeparator.right = rSeparator.left + drawparams.SplitterX;
		rLeft.right = rSeparator.left;
		rRight.left = rSeparator.right;
	}
}

ObjectID CPanelContainer::GetObjectAtPoint_Rec(POINT pt, U32 index, const RECT& currentbounds) const
{
	Node n = Layout.at(index);
	if(n.Left == InvalidNodeIndex || n.Right == InvalidNodeIndex)
	{
		//should never happen - all nodes should point to subnodes or to panes.
		__debugbreak();
		return ObjectID(Object_None);
	}
	RECT splitbounds, rLeft, rRight;
	ComputeBounds(currentbounds, n, splitbounds, rLeft, rRight);
	ObjectType type = (n.Flags & NodeFlag_HorizontalSeparator) ? Object_HorizontalSplit : Object_VerticalSplit;
	if(PtInRect(&splitbounds, pt))
	{
		return ObjectID(type, index);
	}
	if(PtInRect(&rLeft, pt))
	{
		if(n.Flags & NodeFlag_LeftIsNode)
		{
			return GetObjectAtPoint_Rec(pt, n.Left, rLeft);
		}
		else
		{
			return ObjectID(Object_Panel, n.Left);
		}
	}
	if(PtInRect(&rRight, pt))
	{
		if(n.Flags & NodeFlag_RightIsNode)
		{
			return GetObjectAtPoint_Rec(pt, n.Right, rRight);
		}
		else
		{
			return ObjectID(Object_Panel, n.Right);
		}
	}
	return ObjectID(Object_None);
}

ObjectID CPanelContainer::GetObjectAtPoint(POINT pt) const
{
	//Traverse the BSP tree and find the splitter it hits.
	RECT r;
	GetClientRect(GetWindowHandle(), &r);
	return GetObjectAtPoint_Rec(pt, 0, r);
}

void CPanelContainer::SetCursorFromTrackedObject(ObjectID o) const
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
	ObjectID id = GetObjectAtPoint(pt);
	if(id.GetObjectType() != Object_Panel)
	{
		return -1;
	}
	return id.GetObjectIndex();
}

void CPanelContainer::RecomputeLayout()
{
	//TODO: Recompute the layout based on changes in size or tracking.
}

void CPanelContainer::DrawClientArea(HDC hdc) const
{
	RECT r = { 0 };
	GetClientRect(GetWindowHandle(), &r);
	if(!hdc)
	{
		//force an update. This shouldn't happen, but we'll handle it gracefully.
		RedrawWindow(GetWindowHandle(), &r, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN);
		return;
	}
	//now recursively draw the BSP tree.
	DrawClientAreaRec(hdc, 0, r);
}

//Traverse the BSP tree and draw each split. 
// While traversing, when we've resolved a rect, draw the border of that rect.
void CPanelContainer::DrawClientAreaRec(HDC hdc, U32 currentindex, const RECT& currentbounds) const
{
	//get the root element.
	Node n = Layout.at(currentindex);
	if(n.Left == InvalidNodeIndex || n.Right == InvalidNodeIndex)
	{
		//should never happen - all nodes should point to subnodes or to panes.
		__debugbreak();
		return;
	}
	RECT splitbounds, rLeft, rRight;
	ComputeBounds(currentbounds, n, splitbounds, rLeft, rRight);
	//draw the separator.
	FillRect(hdc, &splitbounds, cglobals.ButtonFace);
	//Now subdivide the bounding rectangle by the splitter.
	if(!IsRectEmpty(&rLeft))
	{
		//rect should have something done.
		if(n.Flags & NodeFlag_LeftIsNode)
		{
			//recursively do things.
			DrawClientAreaRec(hdc, n.Left, rLeft);
		}
		else
		{
			//draw the border around the current panel.
			DrawBorder(hdc, rLeft, cglobals.ButtonShadow, cglobals.ButtonHilight);
			InflateRect(&rLeft, -1, -1);
			DrawBorder(hdc, rLeft, cglobals.WindowFrame, cglobals.ButtonFace);
		}
	}
	if(!IsRectEmpty(&rRight))
	{
		if(n.Flags & NodeFlag_RightIsNode)
		{
			//Recursively do things.
			DrawClientAreaRec(hdc, n.Right, rRight);
		}
		else
		{
			//draw the border around the current panel.
			DrawBorder(hdc, rRight, cglobals.ButtonShadow, cglobals.ButtonHilight);
			InflateRect(&rLeft, -1, -1);
			DrawBorder(hdc, rLeft, cglobals.WindowFrame, cglobals.ButtonFace);
		}
	}
}

void CPanelContainer::DrawBorder(HDC hdc, const RECT& BorderRect, HBRUSH TopLeft, HBRUSH BottomRight) const
{
	LONG x = BorderRect.left,
		y = BorderRect.top,
		cx = BorderRect.right - BorderRect.left,
		cy = BorderRect.bottom - BorderRect.top;
	RECT r = { x, y, cx - 1, 1 };
	FillRect(hdc, &r, TopLeft);
	r = { x, y, 1, cy - 1 };
	FillRect(hdc, &r, TopLeft);
	r = { x + cx, y, -1, cy };
	FillRect(hdc, &r, BottomRight);
	r = { x, y + cy, cx, -1 };
	FillRect(hdc, &r, BottomRight);
}

void CPanelContainer::InvertTracker(RECT trackRect) const
{
	if(IsRectEmpty(&trackRect))
	{
		__debugbreak();
	}
	HDC dc = GetDC(GetWindowHandle());
	HBRUSH old = (HBRUSH)SelectObject(dc, cglobals.HalftoneBrush);
	PatBlt(dc, trackRect.left, trackRect.top, trackRect.right - trackRect.left, trackRect.bottom - trackRect.top, PATINVERT);
	if(old)
	{
		SelectObject(dc, old);
	}
	ReleaseDC(GetWindowHandle(), dc);
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

