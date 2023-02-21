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
			MessageBox(hwnd, L"����", L"���������", MB_OK);
			break;
		default:
			break;
		}
		break;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
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
