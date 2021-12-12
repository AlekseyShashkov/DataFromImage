#include "MainWindow.h"

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
//
// PARAMETERS: [in]  hWind     - window handle;
//             [in]  uMsg      - message for window;
//             [in]  wParam    - additional information for message;
//             [in]  lParam    - additional information for message.
//
// RETURN VALUE: 0  - in all cases.
//
// PURPOSE: message processing function.
//
// COMMENTS: no.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;

    switch (uMsg) {
    case WM_CREATE:
        return 0;

    case WM_PAINT: 
        hDC = ::BeginPaint(hWnd, &ps);

        ::EndPaint(hWnd, &ps);
        return 0;
    
    case WM_DESTROY:
        ::PostQuitMessage(NULL);
        return 0;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}