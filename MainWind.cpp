#include "MainWind.h"

MainWind::MainWind(HWND hWnd) : hWnd_(hWnd)
{

}

MainWind::~MainWind()
{
	// MessageBox(hwnd, L"Меню", L"Сообщение", MB_OK);
}




// Создаем верхнее меню
void MainMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU FileSubMenu = CreateMenu();

	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileOpen, L"Открыть файл");  // Создаем раздел Файл
	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileClose, L"Закрыть файл"); // Создаем раздел Файл

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileSubMenu, L"Файл");

	SetMenu(hWnd, RootMenu);
}


void AddTextWidgets(HWND hWnd)
{
	RECT rect = { 0 };

	GetWindowRect(hWnd, &rect);

	const wchar_t CLASS_NAME[] = L"static";

	CreateWindowEx(
		0,									// Optional window styles.
		L"static",							// Window class
		L"Текстовое поле",						// Заголовок окна
		WS_VISIBLE | WS_CHILD,   // Window style

		// Size and position
		5, 5, (rect.right - rect.left) - 25, (rect.bottom - rect.top) - 80,

		hWnd,						// Parent window    
		//NULL,						// Menu
		(HMENU)(int)ID_TEXT_WIDG,	// ID Виджета
		NULL,						// Instance handle
		NULL);						// Additional application data
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient;
	// static 
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		MainMenu(hwnd);
		AddTextWidgets(hwnd);

	case WM_COMMAND: // Обработка комманд
		switch (wParam)
		{
		case ON_MenuFileOpen:
			MessageBox(hwnd, L"Меню", L"Сообщение", MB_OK);
			break;
		default:
			break;
		}
		break;
	case WM_SIZE:   // Изменение размера основного окна
		GetClientRect(hwnd, &rcClient);
		EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&rcClient);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// MoveWindow()

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	LPRECT rcParent;
	int  idChild;

	// Retrieve the child-window identifier. Use it to set the 
	// position of the child window. 

	idChild = GetWindowLong(hwndChild, GWL_ID);

	if (idChild == ID_TEXT_WIDG)
	{
		// Size and position the child window.  

		rcParent = (LPRECT)lParam;
		MoveWindow(hwndChild,5, 5, (rcParent->right - rcParent->left) - 10, (rcParent->bottom - rcParent->top) - 10, TRUE);

		// Make sure the child window is visible. 

		ShowWindow(hwndChild, SW_SHOW);
	}

	return TRUE;
}