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

	case WM_COMMAND: // ��������� �������
		switch (wParam)
		{
		case ON_MenuFileOpen:
			MessageBox(hwnd, L"����", L"gfdgfd", MB_OK);
			break;
		default:
			break;
		}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ������� ������� ����
void MainMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU FileSubMenu = CreateMenu();

	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileOpen, L"������� ����"); // ������� ������ ����
	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileClose, L"������� ����"); // ������� ������ ����

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileSubMenu, L"����");

	SetMenu(hWnd, RootMenu);
}
