#pragma once
#include <Windows.h>

#define ON_MenuFileOpen		1
#define ON_MenuFileClose	2

static HWND tStFil;

void MainMenu(HWND hWnd);
void AddTextWidgets(HWND hWnd);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
