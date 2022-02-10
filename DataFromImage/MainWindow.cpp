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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    
    Gdiplus::Status st = Gdiplus::GdiplusStartup(&gdiplusToken, 
                                                 &gdiplusStartupInput, 0);

    if (st != Gdiplus::Ok) {
        ::MessageBox(nullptr,
                     TEXT("Failed to initialize GDI+!"),
                     TEXT("Error"),
                     MB_OK);

        return 0;
    }

    const wchar_t *szWindowClass = TEXT("MainWindow");
    const wchar_t *szWindowTitle = TEXT("");

    if (!::RegClassEx(hInstance, szWindowClass)) {
        ::MessageBox(nullptr,
                     TEXT("Failed to register class!"),
                     TEXT("Error"),
                     MB_OK);

        return 0;
    }

    HWND hWnd = CreateWnd(hInstance, szWindowClass, szWindowTitle);

    if (!hWnd) {
        ::MessageBox(nullptr,
                     TEXT("Failed to create window!"),
                     TEXT("Error"),
                     MB_OK);

        return 0;
    }

    ::ShowWindow(hWnd, SW_SHOWNORMAL);
    ::UpdateWindow(hWnd);

    MSG msg;
    
    while (::GetMessage(&msg, nullptr, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return static_cast<int>(msg.wParam);
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
    wcex.hCursor       = ::LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = ::CreateSolidBrush(RGB(27, 29, 46));
    wcex.lpszMenuName  = 0; 
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
    int width  = 1280;
    int height = 720;
    int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;     
    int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;     

    return ::CreateWindowEx(WS_EX_DLGMODALFRAME,
                            szWindowClass,
                            szWindowTitle,
                            WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                            x,
                            y,
                            width,
                            height,
                            nullptr,
                            nullptr,
                            hInstance,
                            nullptr);
}
