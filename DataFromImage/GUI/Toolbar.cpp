#include "Toolbar.h"

Toolbar::Toolbar(const HWND &_Window)
{
    RECT lv_Rect;
    ::GetClientRect(_Window, &lv_Rect);

    // Panel position.
    m_PanelPosition = { 0, 0 };

    // Panel dimensions.
    const int lv_Width  = static_cast<int>(lv_Rect.right * 0.08f);
    const int lv_Height = lv_Rect.bottom;

    // Create panel.
    HDC lv_DC = ::GetDC(_Window);
    m_PanelDC     = ::CreateCompatibleDC(lv_DC);
    m_PanelBitmap = ::CreateCompatibleBitmap(lv_DC, lv_Width, lv_Height);
    m_PanelBitmapDefault = reinterpret_cast<HBITMAP>(
                           ::SelectObject(m_PanelDC, m_PanelBitmap));
    ::ReleaseDC(_Window, lv_DC);

    // Draw panel.
    RECT lv_Panel;
    ::SetRect(&lv_Panel, 0, 0, lv_Width, lv_Height);
    HBRUSH lv_Brush        = reinterpret_cast<HBRUSH>(
                             ::GetClassLongPtr(_Window, GCL_HBRBACKGROUND));
    HBRUSH lv_BrushDefault = reinterpret_cast<HBRUSH>(
                             ::SelectObject(m_PanelDC, lv_Brush));
    ::SetDCBrushColor(m_PanelDC, RGB(34, 36, 55));
    ::FillRect(m_PanelDC, &lv_Panel,
               reinterpret_cast<HBRUSH>(::GetStockObject(DC_BRUSH)));
    ::SelectObject(m_PanelDC, lv_BrushDefault);
    ::DeleteObject(lv_Brush);

    HINSTANCE lv_Instance = reinterpret_cast<HINSTANCE>(
                            ::GetWindowLongPtr(_Window, GWL_HINSTANCE));

    // Create button download.
    auto  lv_DownloadPath = TEXT("download.bmp");
    HMENU lv_DownloadID   = reinterpret_cast<HMENU>(Controller::
                                                    LOAD_FILE_BUTTON);
    CreateButton(m_LoadFileButton, m_LogoLoadFile, lv_DownloadPath,
                 BORDER_INDENT * 2, BORDER_INDENT * 2, _Window, lv_Instance,
                 lv_DownloadID);

    // Create button clean.
    auto  lv_CleanPath = TEXT("clean.bmp");
    HMENU lv_CleanID   = reinterpret_cast<HMENU>(Controller::
                                                 CLEAN_PANELS_BUTTON);
    CreateButton(m_CleanPanelsButton, m_LogoCleanPanels, lv_CleanPath,
                 BORDER_INDENT * 2, BORDER_INDENT * 6, _Window, lv_Instance,
                 lv_CleanID);
}

Toolbar::~Toolbar()
{
    ::DestroyWindow(m_LoadFileButton);
    ::DestroyWindow(m_CleanPanelsButton);
    ::DeleteObject(m_LogoLoadFile);
    ::DeleteObject(m_LogoCleanPanels);
}

void Toolbar::ProcessingData(const BitmapPtr &_image)
{

}

void Toolbar::CleanPanel()
{

}

void Toolbar::CreateButton(HWND &_Button, HBITMAP &_Logo, 
                           const wchar_t *_LogoFilePath, const int _X,
                           const int _Y, const HWND _Window, 
                           const HINSTANCE _Instance,
                           const HMENU _ControllerID)
{
    const int lv_Width  = 40;
    const int lv_Height = 40;

    _Button = ::CreateWindowEx(0,
                               TEXT("Button"),
                               0,
                               WS_CHILD | WS_VISIBLE | BS_BITMAP,
                               _X, 
                               _Y,
                               lv_Width,
                               lv_Height,
                               _Window,
                               _ControllerID,
                               _Instance,
                               nullptr);
    _Logo = reinterpret_cast<HBITMAP>(::LoadImage(nullptr,
                                                  _LogoFilePath,
                                                  IMAGE_BITMAP,
                                                  lv_Width,
                                                  lv_Height,
                                                  LR_LOADFROMFILE));
    ::SendMessage(_Button, BM_SETIMAGE, IMAGE_BITMAP,
                  reinterpret_cast<LPARAM>(_Logo));
}

POINT Toolbar::GetPositionPanel() const
{
    return m_PanelPosition;
}