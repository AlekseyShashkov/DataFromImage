#include "Image.h"

Image::Image(const HWND &_hWnd)
{
    RECT lv_rect{};
    ::GetClientRect(_hWnd, &lv_rect);

    // Panel position.
    m_positionPanel = { static_cast<long>(lv_rect.right * 0.1) + 15, 15 };

    // Panel dimensions.
    const long lv_width = static_cast<long>(lv_rect.right * 0.5);
    const long lv_height = static_cast<long>(lv_rect.bottom * 0.7);

    // Rectangle for displaying image.
    ::SetRect(&m_imageRect, 15, 75, lv_width - 15, lv_height - 45);

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
    ::DeleteObject(lv_hBrush);  

    // Draw title.
    HFONT lv_hFont = ::CreateFont(18, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Arial"));
    ::SetBkMode(m_handlePanel, TRANSPARENT);
    ::SetTextColor(m_handlePanel, RGB(255, 255, 255));
    ::SelectObject(m_handlePanel, lv_hFont);
    const wchar_t lv_title[] = TEXT("Basic Image");
    ::TextOut(m_handlePanel, 15, 15, lv_title, wcslen(lv_title));

    // Draw subtitle.
    lv_hFont = ::CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Arial"));
    ::SetTextColor(m_handlePanel, RGB(85, 90, 110));
    ::SelectObject(m_handlePanel, lv_hFont);
    const wchar_t lv_subtitle[] = TEXT("Drawing location of the loaded image");
    ::TextOut(m_handlePanel, 15, 45, lv_subtitle, wcslen(lv_subtitle));
    ::DeleteObject(lv_hFont);
}

Image::~Image() noexcept
{

}

void Image::ProcessingData(BitmapPtr _image)
{
    // The ratio of the sides of the image to the sides of the 
    // target rectangle.
    float lv_ratioWidth = (m_imageRect.right - m_imageRect.left)
        / (float)_image.get()->GetWidth();
    float lv_ratioHeight = (m_imageRect.bottom - m_imageRect.top)
        / (float)_image.get()->GetHeight();

    // If the sides of the image are less than the sides of the
    // target rectangle.
    if (lv_ratioWidth >= 1.0 && lv_ratioHeight >= 1.0) {
        DrawImage(_image, 1.0);
    } 
    // Depending on which sides ratio is smaller, we scale the image and 
    // draw it in the middle of the target rectangle.
    else {
        if (lv_ratioWidth <= lv_ratioHeight) { 
            DrawImage(_image, lv_ratioWidth);
        } 
        else {
            DrawImage(_image, lv_ratioHeight);
        }
    }
}

void Image::DrawImage(BitmapPtr _image, const float _ratio)
{
    // Scale the image and center it relative to where it will be drawn. 
    int lv_x = static_cast<int>((m_imageRect.right + m_imageRect.left
        - _image.get()->GetWidth() * _ratio) / 2);
    int lv_y = static_cast<int>((m_imageRect.bottom + m_imageRect.top
        - _image.get()->GetHeight() * _ratio) / 2);
    int lv_width = static_cast<int>(_image.get()->GetWidth() * _ratio);
    int lv_height = static_cast<int>(_image.get()->GetHeight() * _ratio);

    // Draw an image and a frame around it.
    ::Gdiplus::Graphics lv_graphics(m_handlePanel);
    ::Gdiplus::Pen lv_pen((::Gdiplus::Color(255, 85, 90, 110)));
    ::Gdiplus::Rect lv_rectangle(lv_x, lv_y,
        lv_width, lv_height);
    lv_graphics.DrawImage(_image.get(), lv_rectangle);
    lv_graphics.DrawRectangle(&lv_pen, lv_x - 1, lv_y - 1,
        lv_width + 1, lv_height + 1);
    ::DeleteObject(&lv_pen);

    // Adding a postscript about resizing the image. 
    HFONT lv_hFont = ::CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, TEXT("Arial"));
    ::SetTextColor(m_handlePanel, RGB(85, 90, 110));
    ::SelectObject(m_handlePanel, lv_hFont);
    wchar_t lv_postscript[MAX_PATH];
    ::wsprintf(lv_postscript,
        TEXT("Displayed image at %d%% of the original size."),
        static_cast<int>(_ratio * 100));
    ::TextOut(m_handlePanel, 15, m_imageRect.bottom + 15, lv_postscript,
        wcslen(lv_postscript));
    ::DeleteObject(lv_hFont);    
}

POINT Image::GetPositionPanel() const noexcept
{
    return m_positionPanel;
}