#include "Toolbar.h"

Toolbar::Toolbar(const HWND &_hWnd)
{
    RECT lv_rect{};
    ::GetClientRect(_hWnd, &lv_rect);

    m_positionPanel = { 0, 0 };

    // Dimensions.
    const long lv_width = static_cast<long>(lv_rect.right * 0.1);
    const long lv_height = static_cast<long>(lv_rect.bottom);

    // Create panel.
    HDC lv_hDC = ::GetDC(_hWnd);
    m_memoryPanel = ::CreateCompatibleBitmap(lv_hDC, lv_width, lv_height);
    m_handlePanel = ::CreateCompatibleDC(lv_hDC);
    ::SelectObject(m_handlePanel, m_memoryPanel);
    ::ReleaseDC(_hWnd, lv_hDC);

    // Draw panel.
    RECT lv_panel = { 0, 0, lv_width, lv_height };
    ::SetDCBrushColor(m_handlePanel, RGB(34, 36, 55));
    ::FillRect(m_handlePanel, &lv_panel, (HBRUSH)::GetStockObject(DC_BRUSH));

    // Create controllers.
    HINSTANCE lv_hInstance = (HINSTANCE)::GetWindowLongPtr(_hWnd,
        GWL_HINSTANCE);
    m_loadFileButton = ::CreateWindowW(L"Button",
        NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP ,
        30, 30, 40, 40, _hWnd, (HMENU)Controllers::ID_BUTTON_LOADFILE,
        lv_hInstance, NULL);
    m_logoLoadFileButton = (HBITMAP)::LoadImage(nullptr, L"download.bmp", IMAGE_BITMAP, 40, 40, LR_LOADFROMFILE);
    ::SendMessage(m_loadFileButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)m_logoLoadFileButton);
}

Toolbar::~Toolbar() noexcept
{
    ::DestroyWindow(m_loadFileButton);
    ::DeleteObject(m_logoLoadFileButton);
}

POINT Toolbar::GetPositionPanel() const noexcept
{
    return m_positionPanel;
}

void Toolbar::CreatePanel(const HWND &_hWnd)
{

}