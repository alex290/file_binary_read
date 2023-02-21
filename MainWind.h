#pragma once
#include <Windows.h>

#define ON_MenuFileOpen		1
#define ON_MenuFileClose	2

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void MainMenu(HWND hWnd);

class MainWind
{
};

