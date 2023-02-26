#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define ON_MenuFileOpen		1
#define ON_MenuFileClose	2

#define NUMBER_OF_SYMBOLS_PER_LINE 16

static HWND tStFil;
static LPCSTR	lpcBuffer;
static ULONGLONG u_sizeFile;
static UINT	g_uiGranularity;
static UINT	g_BottomOffset;

static OPENFILENAMEW OpFileName;
static WCHAR FileName[MAX_PATH];


void MainMenu(HWND hWnd);
void AddTextWidgets(HWND hWnd);

bool OpenFile(HWND hWnd, UINT64* ullNumLines); // Открываем файл
bool ReadFromFiles(LPWSTR path); // Чтение данных с файла




LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
