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
		L"",						// Заголовок окна
		WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | WS_VSCROLL,   // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		hWnd,						// Parent window    
		NULL,						// Menu
		NULL,						// Instance handle
		NULL);						// Additional application data
}

// Открываем файл
bool OpenFile(HWND hWnd, UINT64* ullNumLines)
{
	if (lpcBuffer != NULL)
	{
		UnmapViewOfFile(lpcBuffer);
		lpcBuffer = NULL;
	}

	RECT rect;
	OPENFILENAMEW OpFileName;
	WCHAR FileName[MAX_PATH] = { 0 };

	ZeroMemory(&OpFileName, sizeof(OpFileName));
	OpFileName.lStructSize = sizeof(OpFileName);
	OpFileName.hwndOwner = hWnd;
	OpFileName.lpstrFile = FileName;
	OpFileName.nMaxFile = sizeof(FileName);
	// OpFileName.lpstrFilter = L"*.*";
	OpFileName.lpstrFileTitle = NULL;
	OpFileName.nMaxFileTitle = 0;
	OpFileName.lpstrInitialDir = NULL;
	OpFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (GetOpenFileNameW(&OpFileName))
	{
		if (!ReadFromFiles(FileName))
		{
			MessageBoxW(hWnd, L"File could not open!", L"ERROR", MB_OK);
			return false;
		}
	}
	long double ldNumLines = ceill(long double(u_sizeFile) / NUMBER_OF_SYMBOLS_PER_LINE);
	*ullNumLines = (UINT64)ldNumLines;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, TRUE);
	return true;
}


// Чтение данных с файла
bool ReadFromFiles(LPWSTR path)
{
	HANDLE FileToRead = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (FileToRead == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	HANDLE hFileMap = CreateFileMapping(FileToRead, NULL, PAGE_READONLY, 0, 0, NULL);

	if (hFileMap == NULL)
	{
		CloseHandle(FileToRead);
		return false;
	}

	g_BottomOffset = g_uiGranularity;

	DWORD dwFileSizeHigh = 0;

	u_sizeFile = GetFileSize(FileToRead, &dwFileSizeHigh);
	u_sizeFile += (((ULONGLONG)dwFileSizeHigh) << 32);

	CloseHandle(FileToRead);

	ULONGLONG ullFileOffset = 0;
	DWORD dwBytesInBlock = g_uiGranularity;

	if (u_sizeFile < g_uiGranularity)
	{
		dwBytesInBlock = (DWORD)u_sizeFile;
	}

	PVOID MappedMemory = MapViewOfFile(hFileMap,
		FILE_MAP_READ,
		(DWORD)(ullFileOffset >> 32),
		(DWORD)(ullFileOffset & 0xFFFFFFFF),
		dwBytesInBlock
	);



	lpcBuffer = (LPCSTR)MappedMemory;

	CloseHandle(hFileMap);

	return true;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static UINT64 ullNumLines;
	wchar_t* wc;
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
			SetWindowTextW(tStFil, L"");
			if (!OpenFile(hwnd, &ullNumLines))
			{
				break;	
			}
			// wchar_t m_reportFileName[256];
			//swprintf_s(m_reportFileName, L"%d", u_sizeFile);
			wc = new wchar_t[u_sizeFile];
			mbstowcs(wc, lpcBuffer, u_sizeFile);
			SetWindowTextW(tStFil, wc);
			// MessageBox(hwnd, m_reportFileName, L"New", MB_OK);
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

