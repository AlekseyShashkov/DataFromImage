#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GUI\GUI.h"
#include "GUI\Toolbar.h"
#include "GUI\Image.h"
#include <windows.h>
#include <CommCtrl.h>
#include <memory>

#pragma comment (lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*'\
						publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WINDOW_MAINWINDOW_WIDTH  1024
#define WINDOW_MAINWINDOW_HEIGHT 600

ATOM RegClassEx(HINSTANCE hInstance, const WCHAR *szWindowClass);
HWND CreateWnd(HINSTANCE hInstance, const WCHAR *szWindowClass,
    const WCHAR *szWindowTitle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // MAINWINDOW_H