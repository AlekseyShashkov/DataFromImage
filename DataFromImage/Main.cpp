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
        gui->AddElement(std::make_unique<Toolbar>(hWnd));
        gui->AddElement(std::make_unique<Image>(hWnd));
        gui->AddElement(std::make_unique<ColorData>(hWnd));

        return 0;

    case WM_PAINT: 
        hDC = ::BeginPaint(hWnd, &ps);

        gui->DrawElements(hDC, hWnd);

        ::EndPaint(hWnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (static_cast<Toolbar::Controller>(LOWORD(wParam))) {
            case Toolbar::Controller::LOAD_FILE_BUTTON:
                OPENFILENAME ofn;
                wchar_t szDirect[MAX_PATH];

                ::ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize     = sizeof(ofn);
                ofn.hwndOwner       = hWnd;
                ofn.lpstrFile       = szDirect;
                ofn.lpstrFile[0]    = '\0';
                ofn.nMaxFile        = sizeof(szDirect);
                ofn.lpstrFilter     = 0;
                ofn.nFilterIndex    = 1;
                ofn.lpstrFileTitle  = 0;
                ofn.lpstrFileTitle  = 0;
                ofn.lpstrInitialDir = 0;
                ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |
                                      OFN_EXPLORER;

                if (::GetOpenFileName(&ofn)) {
                    gui->CleanPanels();
                    gui->TakeImage(
                        std::make_unique<Gdiplus::Bitmap>(ofn.lpstrFile));

                    ::InvalidateRect(hWnd, 0, false);
                    ::UpdateWindow(hWnd);
                }
                break;

            case Toolbar::Controller::CLEAN_PANELS_BUTTON:
                gui->CleanPanels();

                ::InvalidateRect(hWnd, 0, false);
                ::UpdateWindow(hWnd);
                break;
        }
        return 0;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}