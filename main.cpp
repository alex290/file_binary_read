#ifndef UNICODE
#define UNICODE
#endif 

#include "MainWind.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"MainWindowClass";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// �������� ����.

	HWND hwnd = CreateWindowEx(
		0,									// Optional window styles.
		CLASS_NAME,							// Window class
		L"Read Binary",						// ��������� ����
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,   // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,				// Parent window    
		NULL,				// Menu
		hInstance,			// Instance handle
		NULL);				// Additional application data


	if (hwnd == NULL)
	{
		return 0;
	}


	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	
	return 0;
}

