#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE _, LPWSTR cmdlineW, int showcmd)
{
	InitializeWin32();
	CObjectPtr<CMainWindow> mw(new CMainWindow(), true);
	if(!mw->Create())
	{
		return 0;
	}
	HACCEL acc = LoadAccelerators(hinst, TEXT("MainAccel"));
	MSG msg = { 0 };
	while(GetMessage(&msg, nullptr, 0, 0))
	{
		if(!TranslateAccelerator(mw->GetWindowHandle(), acc, &msg))
		{
			TranslateMessage(&msg);
		}
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}