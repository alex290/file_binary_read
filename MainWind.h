#pragma once
#include <Windows.h>

#define ON_MenuFileOpen		1
#define ON_MenuFileClose	2
#define ID_TEXT_WIDG  100 


void MainMenu(HWND hWnd);
void AddTextWidgets(HWND hWnd);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumChildProc(HWND, LPARAM);

class MainWind
{
public:
	MainWind(HWND hWnd);
	~MainWind();

private:
	HWND hWnd_;
};

