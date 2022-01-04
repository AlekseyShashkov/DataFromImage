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

    static auto gui = std::make_unique<GUI>();

    switch (uMsg) {
    case WM_CREATE:
        gui->AddElement(hWnd, std::make_unique<Toolbar>(hWnd));
        gui->AddElement(hWnd, std::make_unique<Image>(hWnd));
        return 0;

    case WM_PAINT: 
        hDC = ::BeginPaint(hWnd, &ps);

        gui->DrawElements(hDC, hWnd);

        ::EndPaint(hWnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case (size_t)Controllers::ID_BUTTON_LOADFILE: 
                OPENFILENAME ofn;
                wchar_t szDirect[MAX_PATH] = {};
                wchar_t szFileName[MAX_PATH] = {};

                ::ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szDirect;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szDirect);
                ofn.lpstrFilter = NULL;
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = szFileName;
                ofn.lpstrFileTitle = 0;
                ofn.nMaxFileTitle = sizeof(szFileName);
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

                if (::GetOpenFileName(&ofn) == TRUE) {

                }
                break;
        }
        return 0;

    case WM_DESTROY:
        ::PostQuitMessage(NULL);
        return 0;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}