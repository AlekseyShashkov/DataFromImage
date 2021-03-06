#pragma once

#include <memory>

#include <windows.h>
#include <gdiplus.h>
#include <CommCtrl.h>

#include "GUI\GUI.h"
#include "GUI\Toolbar.h"
#include "GUI\Image.h"
#include "GUI\ColorData.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*'\
						publicKeyToken='6595b64144ccf1df' language='*'\"")

ATOM RegClassEx(HINSTANCE hInstance, const WCHAR *szWindowClass);
HWND CreateWnd(HINSTANCE hInstance, const WCHAR *szWindowClass,
               const WCHAR *szWindowTitle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);