#include "MainWind.h"



// Создаем верхнее меню
void MainMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU FileSubMenu = CreateMenu();

	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileOpen, L"Открыть файл");  // Создаем раздел Файл
	AppendMenu(FileSubMenu, MF_STRING, ON_MenuFileClose, L"Выход"); // Создаем раздел Файл

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileSubMenu, L"Файл");

	SetMenu(hWnd, RootMenu);
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
	TEXTMETRIC tm;
	static int cxChar, cyChar, cxCaps, cyClient, iVscrollMax, iVscrollPos;
	RECT rcClient;
	static int wheelDelta = 0;
	int iVscrollInc;
	switch (uMsg)
	{
	case WM_DESTROY:
		if (lpcBuffer != NULL)
		{
			UnmapViewOfFile(lpcBuffer);
			lpcBuffer = NULL;
		}
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		HDC hdc;
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 + 3;

		ReleaseDC(hwnd, hdc);
		MainMenu(hwnd);

	case WM_MOUSEWHEEL:
		wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		for (; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA)
		{
			iVscrollInc = -1;
		}
		for (; wheelDelta < 0; wheelDelta += WHEEL_DELTA)
		{
			iVscrollInc = 1;
		}
		iVscrollInc = max(-iVscrollPos, min(iVscrollInc, iVscrollMax - iVscrollPos));
		if (iVscrollInc != 0)
		{
			iVscrollPos += iVscrollInc;
			ScrollWindow(hwnd, 0, -cyChar * iVscrollInc, NULL, NULL);
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			UpdateWindow(hwnd);
		}
		// break;
		return 0;

	case WM_COMMAND: // Обработка комманд
		switch (wParam)
		{
		case ON_MenuFileOpen:
			if (!OpenFile(hwnd, &ullNumLines))
			{
				break;
			}
			SetScrollBySize(hwnd, cyClient, cyChar, &iVscrollPos, &iVscrollMax, &ullNumLines);
			break;

		case ON_MenuFileClose:
			if (lpcBuffer != NULL)
			{
				UnmapViewOfFile(lpcBuffer);
				lpcBuffer = NULL;
			}
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_SIZE:   // Изменение размера основного окна
		cyClient = HIWORD(lParam);

		SetScrollBySize(hwnd, cyClient, cyChar, &iVscrollPos, &iVscrollMax, &ullNumLines);
		GetClientRect(hwnd, &rcClient);
		return 0;



	case WM_PAINT:
	{
		if (lpcBuffer != NULL)
		{
			PaintText(hwnd, cyClient, cxChar, cyChar, cxCaps, iVscrollPos, ullNumLines);
		}
		else
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hwnd, &ps);
		}
	}

	return 0;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

void SetScrollBySize(HWND hWnd, int cyClient, int cyChar, int* iVscrollPos, int* iVscrollMax, UINT64* ullNumLines)
{
	*iVscrollMax = max(0, int(*ullNumLines - cyClient / cyChar));
	*iVscrollPos = min(*iVscrollPos, *iVscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, *iVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, *iVscrollPos, TRUE);
}

void PaintText(HWND hWnd, int cyClient, int cxChar, int cyChar, int cxCaps, int iVscrollPos, UINT64 ullNumLines)
{
	LPCSTR TextBuffer = lpcBuffer + ((UINT64)iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	HDC hdc;
	PAINTSTRUCT ps;
	int y;
	UINT64 ullCount = -1 + ((UINT64)iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	char cHexBuf[SIZE_HEX_BUF] = { 0 };
	char cOffsetBuf[SIZE_OFFSET_BUF] = { 0 };
	hdc = BeginPaint(hWnd, &ps);
	RECT rect;
	GetClientRect(hWnd, &rect);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	int iPaintBeg = max(-1, iVscrollPos + rect.top / cyChar - 1);
	int iPaintEnd = min(int(ullNumLines), iVscrollPos + rect.bottom / cyChar);

	for (int iteration = iPaintBeg; iteration < iPaintEnd; iteration++)
	{
		y = cyChar * (iteration - iVscrollPos + 1);
		int iLenght = sprintf(cOffsetBuf, "%08X: \t", (iteration + 1) * NUMBER_OF_SYMBOLS_PER_LINE);

		TextOutA(hdc, 0, y, cOffsetBuf, iLenght);
		for (int i = 0; i < NUMBER_OF_SYMBOLS_PER_LINE; i++)
		{
			ullCount++;
			if (ullCount == u_sizeFile)
			{
				break;
			}
			int iSymbol = (unsigned char)*TextBuffer;
			int iLenght = sprintf(cHexBuf, "%02X", iSymbol);
			if (i == 8)
			{
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * i + 1, y, (LPCSTR)"|", 1);
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * i + 8, y, cHexBuf, iLenght);
			}
			else if (i > 7)
			{
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * i + 8, y, cHexBuf, iLenght);
			}
			else {
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * i, y, cHexBuf, iLenght);
			}


			if (iswprint(iSymbol) == 0)
			{
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 2) + cxCaps * i - cxCaps, y, (LPCSTR)".", 1);
			}
			else
			{
				TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 2) + cxCaps * i - cxCaps, y, TextBuffer, 1);
			}
			TextBuffer++;
		}
		TextOutA(hdc, cxChar * SIZE_OFFSET_BUF + cxChar * SIZE_HEX_BUF * NUMBER_OF_SYMBOLS_PER_LINE + 10, y, (LPCSTR)"|", 1);

		if (ullCount == u_sizeFile)
		{
			break;
		}
	}
	EndPaint(hWnd, &ps);
}
