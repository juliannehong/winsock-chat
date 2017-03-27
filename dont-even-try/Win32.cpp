#include "Win32.h"

//Enable visual styles in Windows 7
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//Auto link to the common control library.
#pragma comment(lib, "ComCtl32.lib")

bool InitializeWin32()
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_PROGRESS_CLASS;
	return InitCommonControlsEx(&iccx) == TRUE;
}
