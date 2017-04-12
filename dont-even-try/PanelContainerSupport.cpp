#include "PanelContainerSupport.h"
namespace PanelContainer
{

	CPanelTree::CPanelTree()
	{
	}


	CPanelTree::~CPanelTree()
	{
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
};
