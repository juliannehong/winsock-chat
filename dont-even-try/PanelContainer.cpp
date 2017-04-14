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

CPanelContainerGlobals CPanelContainer::cglobals;

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
	cs->style = WS_CHILD;
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
		TrackingRect.top = (U32)(TrackingLimit.bottom * n.RelativePosition) + drawparams.BorderY;
		TrackingRect.bottom = TrackingRect.top + cy;
	}
	else
	{
		//vertical bar.
		U32 cx = (drawparams.SplitterX - ((drawparams.BorderX << 1) - 1));
		TrackingOffset.x = (cx >> 1); //Middle of the bar.
		TrackingRect.left = (U32)(TrackingLimit.right * n.RelativePosition) + drawparams.BorderX;
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
	OriginalRect = TrackingRect;
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
		//apply the movement of the tracking rect, if the rect changed.
		if(EqualRect(&TrackingRect, &OriginalRect))
		{
			//No change - do nothing.
			return;
		}
		RECT r;
		GetClientRect(GetWindowHandle(), &r);
		//There was a change - figure out what it was.
		Node n = Layout.at(TrackedObject.GetObjectIndex());
		switch(TrackedObject.GetObjectType())
		{
		case Object_HorizontalSplit:
			n.RelativePosition += (TrackingRect.top - OriginalRect.top) / (float)(r.bottom - r.top);
			break;
		case Object_VerticalSplit:
			n.RelativePosition += (TrackingRect.left - OriginalRect.left) / (float)(r.right - r.left);
			break;
		default:
			//this should never happen - we've bugged it if it does.
			__debugbreak();
			return;
		}
		Layout[TrackedObject.GetObjectIndex()] = n;
		//now recursively apply sizing limits and move the windows.
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
				ret.bottom = (U32)(ret.bottom * n.RelativePosition);
			}
			else
			{
				ret.right = (U32)(ret.right * n.RelativePosition);
			}
		}
		else
		{
			//Cut away the right hand rectangle.
			if(n.Flags & NodeFlag_HorizontalSeparator)
			{
				ret.top = (U32)(ret.bottom * n.RelativePosition) + drawparams.SplitterY;
			}
			else
			{
				ret.left += (U32)(ret.right * n.RelativePosition) + drawparams.SplitterX;
			}
		}
		return ret;
	}
}

RECT CPanelContainer::GetLimitRect(U32 index) const
{
	if(index >= Layout.size())
	{
		return RECT();
	}
	Node n = Layout.at(index);
	return GetChildRect(n.Parent, (n.Flags & NodeFlag_IsLeftChild));
}

void CPanelContainer::ComputeBounds(const RECT & rBounds, PanelContainer::Node n, RECT & rSeparator, RECT & rLeft, RECT & rRight) const
{
	rSeparator = rLeft = rRight = rBounds;
	if(n.Flags & NodeFlag_HorizontalSeparator)
	{
		//l, t, r, b
		rSeparator.top = (U32)(rBounds.bottom * n.RelativePosition);
		rSeparator.bottom = rSeparator.top + drawparams.SplitterY;
		rLeft.bottom = rSeparator.top;
		rRight.top = rSeparator.bottom;
	}
	else
	{
		rSeparator.left = (U32)(rBounds.right * n.RelativePosition);
		rSeparator.right = rSeparator.left + drawparams.SplitterX;
		rLeft.right = rSeparator.left;
		rRight.left = rSeparator.right;
	}
}

ObjectID CPanelContainer::GetObjectAtPoint_Rec(POINT pt, U32 index, const RECT& currentbounds) const
{
	if(index >= Layout.size())
	{
		return ObjectID(Object_None);
	}
	Node n = Layout.at(index);
	if(n.Left == InvalidNodeIndex || n.Right == InvalidNodeIndex)
	{
		//if both are null, we hit nothing. if one is null, we hit a panel.
		if(n.Left == n.Right)
		{
			return ObjectID(Object_None);
		}
		else
		{
			return ObjectID(Object_Panel, n.Left == InvalidNodeIndex ? n.Right : n.Left);
		}
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
		SetCursor(cglobals.vsize);
		break;
	case Object_VerticalSplit:
		SetCursor(cglobals.hsize);
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
	if(Layout.size() == 0)
	{
		//nothing to recompute.
		return;
	}
	//TODO: Recompute the layout based on changes in size or tracking.
	//due to FPU use, recomputation is unnecessary. Just reposition the windows.
	//Start the repositioning
	//This should be no larger than 2^24.
	HDWP dwp = BeginDeferWindowPos((int)(Layout.size() + 1));
	//Move the windows
	RECT cr;
	GetClientRect(GetWindowHandle(), &cr);
	MovePanels_Rec(dwp, 0, cr);
	//End the repositioning - this will move everything at once.
	if(dwp == nullptr || !EndDeferWindowPos(dwp))
	{
		if(IsDebuggerPresent())
		{
			OutputDebugString(L"Low on resources - fell back on SetWindowPos instead of DeferWindowPos.\n");
		}
	}
	DrawClientArea(nullptr);
}

void CPanelContainer::MovePanels_Rec(HDWP dwp, U32 index, const RECT & bounds)
{
	//get the node at this position.
	if(index >= Layout.size())
	{
		return;
	}
	//get the root element.
	Node n = Layout.at(index);
	if(n.Left == InvalidNodeIndex || n.Right == InvalidNodeIndex)
	{
		if(n.Left == n.Right)
		{
			//Both null - do nothing.
			return;
		}
		else
		{
			//one is null. move the other to cover everything.
			RECT r = bounds;
			CObjectPtr<CWindow> panel = GetPanel((n.Left == InvalidNodeIndex) ? n.Right : n.Left);
			if(!panel) return;
			InflateRect(&r, -1, -1);
			DeferWindowPos(dwp, panel->GetWindowHandle(), nullptr, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER);
		}
		return;
	}
	RECT splitbounds, rLeft, rRight;
	ComputeBounds(bounds, n, splitbounds, rLeft, rRight);
	//Now subdivide the bounding rectangle by the splitter.
	if(!IsRectEmpty(&rLeft))
	{
		//rect should have something done.
		if(n.Flags & NodeFlag_LeftIsNode)
		{
			//recursively do things.
			MovePanels_Rec(dwp, n.Left, rLeft);
		}
		else
		{
			CObjectPtr<CWindow> panel = GetPanel(n.Left);
			if(!panel) return;
			InflateRect(&rLeft, -1, -1);
			DeferWindowPos(dwp, panel->GetWindowHandle(), nullptr, rLeft.left, rLeft.top, rLeft.right - rLeft.left, rLeft.bottom - rLeft.top, SWP_NOZORDER);
		}
	}
	if(!IsRectEmpty(&rRight))
	{
		if(n.Flags & NodeFlag_RightIsNode)
		{
			//Recursively do things.
			MovePanels_Rec(dwp, n.Right, rRight);
		}
		else
		{
			CObjectPtr<CWindow> panel = GetPanel(n.Right);
			if(!panel) return;
			InflateRect(&rRight, -1, -1);
			DeferWindowPos(dwp, panel->GetWindowHandle(), nullptr, rRight.left, rRight.top, rRight.right - rRight.left, rRight.bottom - rRight.top, SWP_NOZORDER);
		}
	}
}

void CPanelContainer::DrawClientArea(HDC hdc) const
{
	RECT r = { 0 };
	GetClientRect(GetWindowHandle(), &r);
	//now recursively draw the BSP tree.
	DrawClientAreaRec(hdc, 0, r);
}

//Traverse the BSP tree and draw each split. 
// While traversing, when we've resolved a rect, draw the border of that rect.
void CPanelContainer::DrawClientAreaRec(HDC hdc, U32 currentindex, const RECT& currentbounds) const
{
	//if we're off the end, drop out.
	if(currentindex >= Layout.size())
	{
		return;
	}
	//get the root element.
	Node n = Layout.at(currentindex);
	if(n.Left == InvalidNodeIndex || n.Right == InvalidNodeIndex)
	{
		//we may be in the process of building the tree or modifying it. 
		//Here's what we define in degenerate cases:
		// Left or Right child is null - Other child takes up the whole area.
		//Both children are null - Region is erased with the window frame color.
		RECT r = currentbounds;
		DrawBorder(hdc, r, cglobals.ButtonShadow, cglobals.ButtonHilight);
		InflateRect(&r, -1, -1);
		if(n.Left == n.Right)
		{
			//Both null - fill the rect with the background color.
			FillRect(hdc, &r, cglobals.WindowFrame);
		}
		else
		{
			DrawBorder(hdc, r, cglobals.WindowFrame, cglobals.ButtonFace);
		}
		return;
	}
	RECT splitbounds, rLeft, rRight;
	ComputeBounds(currentbounds, n, splitbounds, rLeft, rRight);
	//draw the separator.
	if(hdc)
	{
		FillRect(hdc, &splitbounds, cglobals.ButtonFace);
	}
	else
	{
		//force an update. 
		RedrawWindow(GetWindowHandle(), &splitbounds, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN);
		return;
	}
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
			if(hdc)
			{
				//draw the border around the current panel.
				DrawBorder(hdc, rLeft, cglobals.ButtonShadow, cglobals.ButtonHilight);
				InflateRect(&rLeft, -1, -1);
				DrawBorder(hdc, rLeft, cglobals.WindowFrame, cglobals.ButtonFace);
			}
			else
			{
				//force an update.
				RedrawWindow(GetWindowHandle(), &rLeft, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN);
				return;
			}	
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
			if(hdc)
			{
				//draw the border around the current panel.
				DrawBorder(hdc, rRight, cglobals.ButtonShadow, cglobals.ButtonHilight);
				InflateRect(&rRight, -1, -1);
				DrawBorder(hdc, rRight, cglobals.WindowFrame, cglobals.ButtonFace);
			}
			else
			{
				//force an update.
				RedrawWindow(GetWindowHandle(), &rRight, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN);
				return;
			}
		}
	}
}

void CPanelContainer::DrawBorder(HDC hdc, const RECT& BorderRect, HBRUSH TopLeft, HBRUSH BottomRight) const
{
	LONG x = BorderRect.left,
		y = BorderRect.top,
		cx = BorderRect.right - BorderRect.left,
		cy = BorderRect.bottom - BorderRect.top;
	RECT r = { x, y, x + (cx - 1), y + 1 };
	FillRect(hdc, &r, TopLeft);
	r = { x, y, x + 1,  y + (cy - 1) };
	FillRect(hdc, &r, TopLeft);
	r = { x + cx, y, x + cx - 1, y + cy };
	FillRect(hdc, &r, BottomRight);
	r = { x, y + cy, x + cx, y + cy - 1 };
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

CPanelContainer::CPanelContainer():
	TrackedObject(Object_None),
	TrackingRect{ 0,0,0,0 },
	OriginalRect{ 0,0,0,0 },
	TrackingLimit{ 0,0,0,0 },
	TrackingOffset{ 0,0 },
	istracking(false)
{
}

CPanelContainer::~CPanelContainer()
{
}

U32 CPanelContainer::GetNumPanels() const
{
	return (U32)panels.size();
}

bool CPanelContainer::IsLayoutIndexValid(U32 Index) const
{
	return (Index < Layout.size() && Index >= 0);
}

bool CPanelContainer::IsPanelIndexValid(U32 Index) const
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
	if(IsPanelIndexValid(Index))
	{
		return panels[Index];
	}
	return nullptr;
}

CObjectPtr<CWindow> CPanelContainer::GetPanelAtPoint(POINT pt) const
{
	return GetPanel(ConvertPointToIndex(pt));
}

U32 CPanelContainer::AddSeparator(U32 Parent, SeparatorType Type, float StartingDivision, bool LeftHandChild)
{
	if(Parent == InvalidNodeIndex)
	{
		if(Layout.size() == 0)
		{
			//create a root element and return 0.
			Node n;
			n.Left = InvalidNodeIndex;
			n.Right = InvalidNodeIndex;
			n.RelativePosition = StartingDivision;
			n.Parent = InvalidNodeIndex;
			n.Flags = (Type == Separator_Horizontal) ? NodeFlag_HorizontalSeparator : NodeFlag_VerticalSeparator;
			Layout.insert(Layout.begin(), n);
			return 0;
		}
		return InvalidNodeIndex;
	}
	if(IsLayoutIndexValid(Parent))
	{
		//set up a node structure.
		//is the chosen child valid for the parent?
		Node p = Layout[Parent];
		if((LeftHandChild && p.Left != InvalidNodeIndex) ||
			(!LeftHandChild && p.Right != InvalidNodeIndex))
		{
			return InvalidNodeIndex;
		}
		Node n;
		n.Left = InvalidNodeIndex;
		n.Right = InvalidNodeIndex;
		n.Parent = Parent;
		n.RelativePosition = StartingDivision;
		n.Flags = (Type == Separator_Horizontal) ? NodeFlag_HorizontalSeparator : NodeFlag_VerticalSeparator;
		n.Flags |= (LeftHandChild ? NodeFlag_IsLeftChild : NodeFlag_IsRightChild);
		U32 ret = (U32)Layout.size();
		Layout.push_back(n);
		if(LeftHandChild)
		{
			p.Left = ret;
			p.Flags |= NodeFlag_LeftIsNode;
		}
		else
		{
			p.Right = ret;
			p.Flags |= NodeFlag_RightIsNode;
		}
		Layout[Parent] = p;
		return ret;
	}
	return InvalidNodeIndex;
}

U32 CPanelContainer::AddPanel(U32 Parent, CObjectPtr<CWindow> NewPanel, bool LeftHandChild)
{
	if(!IsLayoutIndexValid(Parent))
	{
		return InvalidNodeIndex;
	}
	Node n = Layout.at(Parent);
	if((LeftHandChild && n.Left != InvalidNodeIndex) ||
		(!LeftHandChild && n.Right != InvalidNodeIndex))
	{
		return InvalidNodeIndex;
	}
	//we have a slot. push the panel into the panel list.
	U32 index = (U32)panels.size();
	panels.push_back(NewPanel);
	if(LeftHandChild)
	{
		n.Left = index;
		n.Flags &= ~NodeFlag_LeftIsNode;
	}
	else
	{
		n.Right = index;
		n.Flags &= ~NodeFlag_RightIsNode;
	}
	Layout[Parent] = n;
	return index;
}

