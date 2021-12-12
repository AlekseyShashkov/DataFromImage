#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <CommCtrl.h>

#define WINDOW_MAINWINDOW_WIDTH  1024
#define WINDOW_MAINWINDOW_HEIGHT 600

ATOM RegClassEx(HINSTANCE hInstance, const WCHAR *szWindowClass);
HWND CreateWnd(HINSTANCE hInstance, const WCHAR *szWindowClass,
    const WCHAR *szWindowTitle);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // MAINWINDOW_H