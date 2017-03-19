#include "Win32.h"

#pragma comment(lib, "ComCtl32.lib")

bool InitializeWin32()
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_INTERNET_CLASSES | ICC_PROGRESS_CLASS;
	return InitCommonControlsEx(&iccx) == TRUE;
}
