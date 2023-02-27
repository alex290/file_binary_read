#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define ON_MenuFileOpen		1
#define ON_MenuFileClose	2

#define NUMBER_OF_SYMBOLS_PER_LINE 16

#define SIZE_HEX_BUF 3
#define SIZE_OFFSET_BUF 11

// static HWND tStFil;
static LPCSTR	lpcBuffer;
static ULONGLONG u_sizeFile;
static UINT	g_uiGranularity;
static UINT	g_BottomOffset;

static OPENFILENAMEW OpFileName;
static WCHAR FileName[MAX_PATH];


void MainMenu(HWND hWnd);

bool OpenFile(HWND hWnd, UINT64* ullNumLines); // Открываем файл
bool ReadFromFiles(LPWSTR path); // Чтение данных с файла


void SetScrollBySize(HWND hWnd, int cyClient, int cyChar, int* iVscrollPos, int* iVscrollMax, UINT64* ullNumLines);
void PaintText(HWND hWnd, int cyClient, int cxChar, int cyChar, int cxCaps, int iVscrollPos, UINT64 ullNumLines);


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
