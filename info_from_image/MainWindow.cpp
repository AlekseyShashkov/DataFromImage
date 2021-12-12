#include "MainWindow.h"

///////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION: int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//
// PARAMETERS: [in]  hInst           - handle of current window instance;
//             [in]  hPrevInstance   - handle of previous window instance;
//             [in]  lpCmdLine       - command line pointer;
//             [in]  nCmdShow        - shows state of the window.
//
// RETURN VALUE: exit value    - when receiving a message WM_QUIT;
//               0             - if function completes before 
//                               entering message loop. 
//
// PURPOSE: control function.
//
// COMMENTS: no.
//
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const WCHAR *szWindowClass = TEXT("MainWindow");
    const WCHAR *szWindowTitle = TEXT("");            

    HINSTANCE hInstance;
    HWND hWnd;                

    if (!::RegClassEx(hInst, szWindowClass))
    {
        ::MessageBox(NULL,
            TEXT("Failed to register class!"),
            TEXT("Error"),
            MB_OK);

        return NULL;
    }

    hInstance = hInst;

    hWnd = CreateWnd(hInst, szWindowClass, szWindowTitle);

    if (!hWnd)
    {
        ::MessageBox(NULL,
            TEXT("Failed to create window!"),
            TEXT("Error"),
            MB_OK);

        return NULL;
    }

    ::ShowWindow(hWnd, SW_SHOWNORMAL);
    ::UpdateWindow(hWnd);

    MSG msg;
    
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

///////////////////////////////////////////////////////////////////////////////
// 
//  FUNCTION: ATOM RegClass(HINSTANCE, const WCHAR *)
//
//  PURPOSE: register window class.
//
//  COMMENTS: no.
//
ATOM RegClassEx(HINSTANCE hInstance, const WCHAR *szWindowClass)
{
    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(wcex); 
    wcex.style         = CS_HREDRAW | CS_VREDRAW; 
    wcex.lpfnWndProc   = WndProc; 
    wcex.cbClsExtra    = 0; 
    wcex.cbWndExtra    = 0; 
    wcex.hInstance     = hInstance;
    wcex.hIcon         = 0; 
    wcex.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = ::CreateSolidBrush(RGB(24, 29, 48)); // 142, 180, 189
    wcex.lpszMenuName  = NULL; 
    wcex.lpszClassName = szWindowClass; 
    wcex.hIconSm       = 0; 

    return (::RegisterClassEx(&wcex) != 0);
}

///////////////////////////////////////////////////////////////////////////////
// 
//  FUNCTION: HWND CreateWnd(HINSTANCE, const WCHAR *, const WCHAR *)
//
//  PURPOSE: creating window.
//
//  COMMENTS: no.
//
HWND CreateWnd(HINSTANCE hInstance, const WCHAR *szWindowClass,
               const WCHAR *szWindowTitle)
{
    int x = static_cast<int>((::GetSystemMetrics(SM_CXSCREEN) 
        - WINDOW_MAINWINDOW_WIDTH) / 2);
    int y = static_cast<int>((::GetSystemMetrics(SM_CYSCREEN) 
        - WINDOW_MAINWINDOW_HEIGHT) / 2);

    return ::CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        szWindowClass,
        szWindowTitle,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        x,
        y,
        WINDOW_MAINWINDOW_WIDTH,
        WINDOW_MAINWINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
}
