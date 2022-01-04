#include "Image.h"

Image::Image(const HWND &_hWnd)
{
    RECT lv_rect{};
    ::GetClientRect(_hWnd, &lv_rect);

    m_positionPanel = { static_cast<long>(lv_rect.right * 0.1) + 20, 10 };
    
    // Dimensions.
    const long lv_width = static_cast<long>(lv_rect.right * 0.5);
    const long lv_height = static_cast<long>(lv_rect.bottom * 0.7);

    // Create panel.
    HDC lv_hDC = ::GetDC(_hWnd);
    m_memoryPanel = ::CreateCompatibleBitmap(lv_hDC, lv_width, lv_height);
    m_handlePanel = ::CreateCompatibleDC(lv_hDC);
    ::SelectObject(m_handlePanel, m_memoryPanel);
    ::ReleaseDC(_hWnd, lv_hDC);

    // Paint panel background in color of the parent window.
    HBRUSH lv_hBrush = (HBRUSH)::GetClassLongPtr(_hWnd, GCL_HBRBACKGROUND);
    ::SelectObject(m_handlePanel, lv_hBrush);
    ::PatBlt(m_handlePanel, 0, 0, lv_width, lv_height, PATCOPY);

    // Draw panel.  
    HRGN lv_hRgn = ::CreateRoundRectRgn(1, 1, lv_width, lv_height, 15, 15);
    ::SetDCBrushColor(m_handlePanel, RGB(34, 36, 55));
    ::FillRgn(m_handlePanel, lv_hRgn, (HBRUSH)::GetStockObject(DC_BRUSH));
    ::DeleteObject(lv_hRgn);

    // Draw place for image.
    RECT lv_image = { 20, 60, lv_width - 20, lv_height - 20 };
    ::SetDCBrushColor(m_handlePanel, RGB(75, 75, 93));
    ::FillRect(m_handlePanel, &lv_image, (HBRUSH)::GetStockObject(DC_BRUSH));
    ::DeleteObject(lv_hBrush);

    // Draw title.
    HFONT lv_hFont = ::CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Arial"));
    ::SetBkMode(m_handlePanel, TRANSPARENT);
    ::SetTextColor(m_handlePanel, RGB(255, 255, 255));
    ::SelectObject(m_handlePanel, lv_hFont);
    ::TextOut(m_handlePanel, 20, 15, TEXT("Main Image"), 10);

    // Draw subtitle.
    lv_hFont = ::CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Arial"));
    ::SetBkMode(m_handlePanel, TRANSPARENT);
    ::SetTextColor(m_handlePanel, RGB(75, 75, 93));
    ::SelectObject(m_handlePanel, lv_hFont);
    ::TextOut(m_handlePanel, 20, 40, TEXT("Image displayed here at"), 23);
    ::DeleteObject(lv_hFont);
}

Image::~Image() noexcept
{

}

POINT Image::GetPositionPanel() const noexcept
{
    return m_positionPanel;
}

void Image::CreatePanel(const HWND &_hWnd)
{

}