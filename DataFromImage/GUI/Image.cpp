#include "Image.h"

Image::Image(const HWND &_Window)
{
    RECT lv_Rect;
    ::GetClientRect(_Window, &lv_Rect);

    // Panel position.
    m_PanelPosition = { static_cast<int>(lv_Rect.right * 0.08f) +
                        BORDER_INDENT, BORDER_INDENT };

    // Panel dimensions.
    const int lv_Width  = static_cast<int>(lv_Rect.right * 0.5f);
    const int lv_Height = static_cast<int>(lv_Rect.bottom * 0.7f);

    // Rectangle for displaying image.
    ::SetRect(&m_ImageRect, BORDER_INDENT, BORDER_INDENT * 5,
              lv_Width - BORDER_INDENT, lv_Height - BORDER_INDENT * 3);

    // Create panel.
    HDC lv_DC = ::GetDC(_Window);
    m_PanelDC          = ::CreateCompatibleDC(lv_DC);
    m_PanelBasicDC     = ::CreateCompatibleDC(lv_DC);
    m_PanelBitmap      = ::CreateCompatibleBitmap(lv_DC, lv_Width, lv_Height);
    m_PanelBasicBitmap = ::CreateCompatibleBitmap(lv_DC, lv_Width, lv_Height);
    m_PanelBitmapDefault      = reinterpret_cast<HBITMAP>(
                                ::SelectObject(m_PanelDC, m_PanelBitmap));
    m_PanelBasicBitmapDefault = reinterpret_cast<HBITMAP>(
                                ::SelectObject(m_PanelBasicDC, 
                                               m_PanelBasicBitmap));
    ::ReleaseDC(_Window, lv_DC);

    // Paint panel background in color of the parent window.
    HBRUSH lv_Brush        = reinterpret_cast<HBRUSH>(
                             ::GetClassLongPtr(_Window, GCL_HBRBACKGROUND));
    HBRUSH lv_BrushDefault = reinterpret_cast<HBRUSH>(
                             ::SelectObject(m_PanelDC, lv_Brush));
    ::PatBlt(m_PanelDC, 0, 0, lv_Width, lv_Height, PATCOPY);

    // Draw panel.  
    HRGN lv_Region = ::CreateRoundRectRgn(1, 1, lv_Width, lv_Height, 15, 15);
    lv_Brush = reinterpret_cast<HBRUSH>(::GetStockObject(DC_BRUSH));
    ::SelectObject(m_PanelDC, lv_Brush);
    ::SetDCBrushColor(m_PanelDC, RGB(34, 36, 55));
    ::FillRgn(m_PanelDC, lv_Region, lv_Brush);
    ::SelectObject(m_PanelDC, lv_BrushDefault);
    ::DeleteObject(lv_Brush);
    ::DeleteObject(lv_Region);

    // Draw title.
    auto lv_Title = TEXT("Basic Image");
    DrawTextOut(18, FW_EXTRABOLD, RGB(255, 255, 255), lv_Title,
                BORDER_INDENT, BORDER_INDENT);

    // Draw subtitle.
    auto lv_Subtitle = TEXT("Drawing location of the loaded image");
    DrawTextOut(16, FW_BOLD, RGB(85, 90, 110), lv_Subtitle,
                BORDER_INDENT, BORDER_INDENT * 3);

    // Copy the panel image to add a cleanup function.
    ::BitBlt(m_PanelBasicDC, 0, 0, lv_Width, lv_Height,
             m_PanelDC, 0, 0, SRCCOPY);
}

Image::~Image()
{

}

void Image::ProcessingData(const BitmapPtr &_Image)
{
    // The ratio of the sides of the image to the sides of the 
    // target rectangle.
    float lv_WidthRatio  = (m_ImageRect.right - m_ImageRect.left) /
                           static_cast<float>(_Image.get()->GetWidth());
    float lv_HeightRatio = (m_ImageRect.bottom - m_ImageRect.top) /
                           static_cast<float>(_Image.get()->GetHeight());

    // If the sides of the image are less than the sides of the
    // target rectangle.
    if (lv_WidthRatio >= 1.0f && lv_HeightRatio >= 1.0f) {
        DrawImage(_Image, 1.0f);
    } else {
        // Depending on which sides ratio is smaller, we scale the image and
        // draw it in the middle of the target rectangle.
        lv_WidthRatio <= lv_HeightRatio ? DrawImage(_Image, lv_WidthRatio) :
                                          DrawImage(_Image, lv_HeightRatio);
    }
}

void Image::DrawImage(const BitmapPtr &_Image, const float _Ratio)
{
    Gdiplus::Graphics lv_Graphics(m_PanelDC);
    Gdiplus::Pen lv_Pen(Gdiplus::Color(255, 85, 90, 110));

    // Scale the image and center it relative to where it will be drawn. 
    int lv_X = static_cast<int>((m_ImageRect.right + m_ImageRect.left -
               _Image.get()->GetWidth() * _Ratio) / 2);
    int lv_Y = static_cast<int>((m_ImageRect.bottom + m_ImageRect.top -
               _Image.get()->GetHeight() * _Ratio) / 2);
    int lv_Width  = static_cast<int>(_Image.get()->GetWidth() * _Ratio);
    int lv_Height = static_cast<int>(_Image.get()->GetHeight() * _Ratio);

    Gdiplus::Rect lv_ImageRect(lv_X, lv_Y, lv_Width, lv_Height);
    Gdiplus::Rect lv_ImageFrameRect(lv_X - 1, lv_Y - 1,
                                    lv_Width + 1, lv_Height + 1);

    // Draw an image and a frame around it.
    lv_Graphics.DrawImage(_Image.get(), lv_ImageRect);
    lv_Graphics.DrawRectangle(&lv_Pen, lv_ImageFrameRect);

    // Adding a postscript about resizing the image.
    auto lv_RatioPercent = std::to_wstring(static_cast<int>(_Ratio * 100));
    auto lv_Postscript   = TEXT("Displayed image at ") + lv_RatioPercent +
                           TEXT("% of the original size.");
    DrawTextOut(14, FW_BOLD, RGB(85, 90, 110), lv_Postscript.c_str(),
                BORDER_INDENT, m_ImageRect.bottom + BORDER_INDENT);
}

void Image::DrawTextOut(const int _FontHeight, const int _FontWeight,
                        const COLORREF _TextColor, const wchar_t *_Text,
                        const int _TextX, const int _TextY)
{
    auto lv_Font = ::CreateFont(_FontHeight, 0, 0, 0,
                                _FontWeight,
                                false,
                                false,
                                false,
                                DEFAULT_CHARSET,
                                OUT_DEFAULT_PRECIS,
                                CLIP_DEFAULT_PRECIS,
                                DEFAULT_QUALITY,
                                DEFAULT_PITCH | FF_DONTCARE,
                                TEXT("Arial"));
    auto lv_FontDefault = reinterpret_cast<HFONT>(::SelectObject(m_PanelDC,
                                                                 lv_Font));
    auto lv_BackModeDefault  = ::SetBkMode(m_PanelDC, TRANSPARENT);
    auto lv_TextColorDefault = ::SetTextColor(m_PanelDC, _TextColor);

    ::TextOut(m_PanelDC, _TextX, _TextY, _Text, ::lstrlen(_Text));

    ::SetBkMode(m_PanelDC, lv_BackModeDefault);
    ::SetTextColor(m_PanelDC, lv_TextColorDefault);

    ::SelectObject(m_PanelDC, lv_FontDefault);
    ::DeleteObject(lv_Font);
}

void Image::CleanPanel()
{
    BITMAP lv_Bitmap;
    ::GetObject(m_PanelBasicBitmap, sizeof(BITMAP), &lv_Bitmap);
    ::BitBlt(m_PanelDC, 0, 0, lv_Bitmap.bmWidth, lv_Bitmap.bmHeight,
             m_PanelBasicDC, 0, 0, SRCCOPY);
}

POINT Image::GetPositionPanel() const
{
    return m_PanelPosition;
}