#include "PanelContainerSupport.h"
namespace PanelContainer
{

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
		WORD bits[8];
		for(int i = 0; i < 8; i++)
			bits[i] = (WORD)(0x5555 << (i & 1));
		HBITMAP patt = CreateBitmap(8, 8, 1, 1, bits);
		if(patt != NULL)
		{
			HalftoneBrush = ::CreatePatternBrush(patt);
			DeleteObject(patt);
		}
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
		DeleteObject(HalftoneBrush);
	}
};
