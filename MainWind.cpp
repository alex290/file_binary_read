#include "MainWind.h"

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

	// CreateWindowA(L"static", L"Текстовое поле", WS_VISIBLE | WS_CHILD, 10, 10, 400, 50, hWnd, NULL, NULL, NULL, NULL);

	tStFil = CreateWindowExW(
		0,										// Optional window styles.
		L"edit",								// Window class
		L"Текстовое поле",						// Заголовок окна
		WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | WS_VSCROLL,   // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		hWnd,						// Parent window    
		NULL,						// Menu
		NULL,						// Instance handle
		NULL);						// Additional application data
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient;
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

		case ON_MenuFileClose:
			SetWindowTextW(tStFil, L"");
			break;
		default:
			break;
		}
		break;
	case WM_SIZE:   // Изменение размера основного окна
		GetClientRect(hwnd, &rcClient);
		// EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&rcClient);
		MoveWindow(tStFil, 5, 5, (rcClient.right - rcClient.left) - 10, (rcClient.bottom - rcClient.top) - 25, TRUE);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

