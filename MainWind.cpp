#include "MainWind.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		MainMenu(hwnd);

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

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
}

// Создаем верхнее меню
void MainMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU FileSubMenu = CreateMenu();

	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileOpen, L"Открыть файл"); // Создаем раздел Файл
	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileClose, L"Закрыть файл"); // Создаем раздел Файл

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileSubMenu, L"Файл");

	SetMenu(hWnd, RootMenu);
}
