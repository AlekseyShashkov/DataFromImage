#include "ColorData.h"

ColorData::ColorData(const HWND &_Window)
{
    RECT lv_Rect;
    ::GetClientRect(_Window, &lv_Rect);

    // Panel position.
    m_PanelPosition = { static_cast<int>(lv_Rect.right * 0.58f) +
                        BORDER_INDENT * 2, BORDER_INDENT };

    // Panel dimensions.
    const int lv_Width  = static_cast<int>(lv_Rect.right * 0.28f);
    const int lv_Height = static_cast<int>(lv_Rect.bottom * 0.95f);

    // Rectangle for displaying charts.
    ::SetRect(&m_ChartRect, BORDER_INDENT, BORDER_INDENT * 5,
              lv_Width - BORDER_INDENT, lv_Height - BORDER_INDENT);

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
    ::DeleteObject(::SelectObject(m_PanelDC, lv_BrushDefault));
    ::DeleteObject(lv_Brush);
    ::DeleteObject(lv_Region);

    // Draw title.   
    auto lv_Title = TEXT("Color Data");
    DrawTextOut(18, FW_EXTRABOLD, RGB(255, 255, 255), lv_Title,
                BORDER_INDENT, BORDER_INDENT);

    // Draw subtitle. 
    auto lv_Subtitle = TEXT("Retrieving information about image channels");
    DrawTextOut(16, FW_BOLD, RGB(85, 90, 110), lv_Subtitle,
                BORDER_INDENT, BORDER_INDENT * 3);
    
    // Copy the panel image to add a cleanup function.
    ::BitBlt(m_PanelBasicDC, 0, 0, lv_Width, lv_Height,
             m_PanelDC, 0, 0, SRCCOPY);
}

ColorData::~ColorData()
{

}

void ColorData::ProcessingData(const BitmapPtr &_Image)
{
    Gdiplus::BitmapData lv_BitmapData;
    Gdiplus::Rect lv_Rect(0, 0, _Image->GetWidth(), _Image->GetHeight());
    _Image->LockBits(&lv_Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppRGB,
                     &lv_BitmapData);

    const int lv_IntensityRGB = 256;
    const int *lv_PixelData = static_cast<int *>(lv_BitmapData.Scan0);
    int lv_ChannelR[lv_IntensityRGB] {};
    int lv_ChannelG[lv_IntensityRGB] {};
    int lv_ChannelB[lv_IntensityRGB] {};
    int lv_ChannelRAverageValue = 0;
    int lv_ChannelGAverageValue = 0;
    int lv_ChannelBAverageValue = 0;

    // Get information about RGB color channels.
    int lv_ImageWidth  = static_cast<int>(_Image->GetWidth());
    int lv_ImageHeight = static_cast<int>(_Image->GetHeight());
    for (int x = 0; x < lv_ImageWidth; ++x) {
        int lv_ChannelRSum = 0;
        int lv_ChannelGSum = 0;
        int lv_ChannelBSum = 0;

        for (int y = 0; y < lv_ImageHeight; ++y) {
            int lv_Color = lv_PixelData[y * lv_BitmapData.Stride / 4 + x];
            int lv_Red   = (lv_Color & 0xff0000) >> 16;
            int lv_Green = (lv_Color & 0xff00) >> 8;
            int lv_Blue  = lv_Color & 0xff;

            ++lv_ChannelR[lv_Red];
            ++lv_ChannelG[lv_Green];
            ++lv_ChannelB[lv_Blue];

            lv_ChannelRSum += lv_Red;
            lv_ChannelGSum += lv_Green;
            lv_ChannelBSum += lv_Blue;
        }

        lv_ChannelRAverageValue += lv_ChannelRSum / lv_ImageHeight;
        lv_ChannelGAverageValue += lv_ChannelGSum / lv_ImageHeight;
        lv_ChannelBAverageValue += lv_ChannelBSum / lv_ImageHeight;
    }

    _Image->UnlockBits(&lv_BitmapData);

    lv_ChannelRAverageValue /= lv_ImageWidth;
    lv_ChannelGAverageValue /= lv_ImageWidth;
    lv_ChannelBAverageValue /= lv_ImageWidth;

    // Draw the histogram of the R channel.
    int lv_Position = 0;
    DrawHistogram(lv_ChannelR, lv_ChannelRAverageValue, RGB(200, 30, 45),
                  lv_IntensityRGB, lv_Position);

    // Draw the histogram of the G channel.
    lv_Position = static_cast<int>((m_ChartRect.bottom - m_ChartRect.top) *
                                   0.25f);
    DrawHistogram(lv_ChannelG, lv_ChannelGAverageValue, RGB(20, 145, 45),
                  lv_IntensityRGB, lv_Position);

    // Draw the histogram of the B channel.
    lv_Position = static_cast<int>((m_ChartRect.bottom - m_ChartRect.top) *
                                   0.5f);
    DrawHistogram(lv_ChannelB, lv_ChannelBAverageValue, RGB(10, 105, 200),
                  lv_IntensityRGB, lv_Position);

    // Draw an RGB pie chart.
    lv_Position = static_cast<int>((m_ChartRect.bottom - m_ChartRect.top) *
                                   0.75f);
    DrawPieChart(lv_ChannelRAverageValue, lv_ChannelGAverageValue,
                 lv_ChannelBAverageValue, lv_Position);
}

void ColorData::DrawHistogram(const int *_Channel, const int _AverageValue,
                              const COLORREF _Color, const int _Intensity,
                              const int _Position)
{
    int  lv_X;
    int  lv_Y;
    SIZE lv_Size; // Dimensions of inscriptions 
    
    HFONT lv_Font = reinterpret_cast<HFONT>(::GetStockObject(SYSTEM_FONT));
    HFONT lv_FontDefault = reinterpret_cast<HFONT>(::SelectObject(m_PanelDC,
                                                                  lv_Font));
    ::SetBkMode(m_PanelDC, TRANSPARENT);

    // Draw average value title.
    lv_Font = ::CreateFont(20, 0, 0, 0,
                           FW_BOLD,
                           false,
                           false,
                           false,
                           DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE,
                           TEXT("Arial"));
    ::SelectObject(m_PanelDC, lv_Font);
    ::SetTextColor(m_PanelDC, RGB(255, 255, 255));
    auto lv_Title = std::to_wstring(_AverageValue);
    ::GetTextExtentPoint32(m_PanelDC, lv_Title.c_str(), lv_Title.length(),
                           &lv_Size);
    lv_X = (m_ChartRect.left + m_ChartRect.right - lv_Size.cx) / 2;
    lv_Y = m_ChartRect.top + _Position;
    ::TextOut(m_PanelDC, lv_X, lv_Y, lv_Title.c_str(), lv_Title.length());

    // Draw average value subtitle.
    lv_Font = ::CreateFont(14, 0, 0, 0, 
                           FW_BOLD, 
                           false, 
                           false, 
                           false,
                           DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE,
                           TEXT("Arial"));
    ::SelectObject(m_PanelDC, lv_Font);
    ::SetTextColor(m_PanelDC, RGB(85, 90, 110));
    auto lv_Subtitle = TEXT("Average value");
    lv_Y += lv_Size.cy;
    ::GetTextExtentPoint32(m_PanelDC, lv_Subtitle, ::lstrlen(lv_Subtitle),
                           &lv_Size);
    lv_X = (m_ChartRect.left + m_ChartRect.right - lv_Size.cx) / 2;
    ::TextOut(m_PanelDC, lv_X, lv_Y, lv_Subtitle, ::lstrlen(lv_Subtitle));

    // Find the maximum amount of color of the channel.
    int lv_MaxAmount = 0;
    for (int i = 0; i < _Intensity; ++i) {
        if (lv_MaxAmount < _Channel[i]) {
            lv_MaxAmount = _Channel[i];
        }
    }

    // Get the maximum value for the scale intervals.
    int lv_Degree = static_cast<int>(floor(log10(abs(lv_MaxAmount))));
    int lv_Scaled = static_cast<int>(lv_MaxAmount * pow(10, -lv_Degree));
    int lv_Base = ((lv_Scaled + 5) / 5) * 5;
    int lv_MaxValue = static_cast<int>(lv_Base * pow(10, lv_Degree));

    // Find the indent depending on the length of the string of the
    // maximum value.
    auto lv_ScaleMaxValue = std::to_wstring(lv_MaxValue);
    ::GetTextExtentPoint32(m_PanelDC, lv_ScaleMaxValue.c_str(),
                           lv_ScaleMaxValue.length(), &lv_Size);
    const int lv_Indent = lv_Size.cx;

    HPEN lv_Pen        = reinterpret_cast<HPEN>(::GetStockObject(DC_PEN));
    HPEN lv_PenDefault = reinterpret_cast<HPEN>(::SelectObject(m_PanelDC,
                                                               lv_Pen));
    ::SetDCPenColor(m_PanelDC, RGB(39, 42, 63));

    // Scale interval options.
    const int   lv_ScaleIntervalCount = 4;
    const float lv_ScaleIntervalStart = 0.06f;
    const float lv_ScaleIntervalStep  = 0.04f;

    // Draw a vertical scale intervals.
    for (int i = 0; i < lv_ScaleIntervalCount + 1; ++i) {
        float lv_ScaleInterval = lv_ScaleIntervalStart +
                                 lv_ScaleIntervalStep * i;
        int   lv_ScaleValue    = lv_MaxValue - (lv_MaxValue /
                                 lv_ScaleIntervalCount * i);
        
        lv_X = m_ChartRect.left + lv_Indent;
        lv_Y = _Position + m_ChartRect.top + static_cast<int>(
               (m_ChartRect.bottom - m_ChartRect.top) * lv_ScaleInterval);

        // Draw a scale value inscription.
        auto lv_ScaleValueText = std::to_wstring(lv_ScaleValue);
        ::GetTextExtentPoint32(m_PanelDC, lv_ScaleValueText.c_str(),
                               lv_ScaleValueText.length(), &lv_Size);
        ::TextOut(m_PanelDC, lv_X - lv_Size.cx, lv_Y - lv_Size.cy / 2,
                  lv_ScaleValueText.c_str(), lv_ScaleValueText.length());

        // Draw a horizontal scale line.
        int lv_LineWidth = _Intensity + ELEMENT_INDENT * 2;
        ::MoveToEx(m_PanelDC, lv_X, lv_Y, NULL);
        ::LineTo(m_PanelDC, lv_X + lv_LineWidth, lv_Y);
    }

    int lv_HistogramTop    = m_ChartRect.top + static_cast<int>(
                             (m_ChartRect.bottom - m_ChartRect.top) *
                             lv_ScaleIntervalStart);
    int lv_HistogramBottom = m_ChartRect.top + static_cast<int>(
                             (m_ChartRect.bottom - m_ChartRect.top) * 
                             (lv_ScaleIntervalStart + lv_ScaleIntervalStep *
                             lv_ScaleIntervalCount));      
    // The ratio of the maximum channel color to the chart size.
    float lv_HeightRatio = static_cast<float>(lv_HistogramBottom -
                           lv_HistogramTop) / lv_MaxValue;

    // Draw a histogram and horizontal scale intervals.
    for (int i = 0; i < _Intensity; ++i) {
        lv_X = m_ChartRect.left + lv_Indent + ELEMENT_INDENT + i;
        lv_Y = _Position + lv_HistogramBottom;
        
        if (i % 50 == 0) {
            // Draw a scale value inscription.
            auto lv_ScaleValueText = std::to_wstring(i);
            ::GetTextExtentPoint32(m_PanelDC, lv_ScaleValueText.c_str(),
                                   lv_ScaleValueText.length(), &lv_Size);
            ::TextOut(m_PanelDC, lv_X - lv_Size.cx / 2, lv_Y + ELEMENT_INDENT,
                      lv_ScaleValueText.c_str(), lv_ScaleValueText.length());

            // Draw a vertical scale line.
            ::SetDCPenColor(m_PanelDC, RGB(39, 42, 63));
            ::MoveToEx(m_PanelDC, lv_X, lv_Y + ELEMENT_INDENT, NULL);
            ::LineTo(m_PanelDC, lv_X, _Position + lv_HistogramTop);
            ::SetDCPenColor(m_PanelDC, _Color);
        }

        if (_Channel[i] > 0) {
            // Draw histogram line.
            int lv_LineHeight = static_cast<int>(_Channel[i] * lv_HeightRatio);
            ::MoveToEx(m_PanelDC, lv_X, lv_Y, NULL);
            ::LineTo(m_PanelDC, lv_X, lv_Y - lv_LineHeight);
        }
    }

    ::SetBkMode(m_PanelDC, OPAQUE);
    ::SetTextColor(m_PanelDC, RGB(0, 0, 0));

    ::SelectObject(m_PanelDC, lv_PenDefault);
    ::SelectObject(m_PanelDC, lv_FontDefault);
    ::DeleteObject(lv_Pen);
    ::DeleteObject(lv_Font);
}

void ColorData::DrawPieChart(const int _AverageValueR, 
                             const int _AverageValueG,
                             const int _AverageValueB, const int _Position)
{
    // Retrieve information about the ratio of color channels.
    float lv_AveragesSum = static_cast<float>(_AverageValueR + _AverageValueG +
                                              _AverageValueB);
    float lv_ValueRRatio = round(_AverageValueR / lv_AveragesSum * 10000) /
                           10000;
    float lv_ValueGRatio = round(_AverageValueG / lv_AveragesSum * 10000) /
                           10000;
    float lv_ValueBRatio = round(_AverageValueB / lv_AveragesSum * 10000) /
                           10000;
    
    // Save information about the ratio of color channels and 
    // sort in descending order.
    std::multimap<float, std::pair<Gdiplus::Color, std::wstring>,
                  std::greater<float>> lv_Channels;
    lv_Channels.emplace(lv_ValueRRatio, std::make_pair(
        Gdiplus::Color(255, 200, 30, 45), TEXT("R")));
    lv_Channels.emplace(lv_ValueGRatio, std::make_pair(
        Gdiplus::Color(255, 20, 145, 45), TEXT("G")));
    lv_Channels.emplace(lv_ValueBRatio, std::make_pair(
        Gdiplus::Color(255, 10, 105, 200), TEXT("B")));

    // Calculate dimensions of pie chart elements.
    float lv_Size = static_cast<float>(m_ChartRect.bottom) - (_Position +
                    m_ChartRect.top) - ELEMENT_INDENT * 4;
    float lv_InscriptionCircleSize = lv_Size / 6;
    float lv_PieChartSize = lv_Size - lv_InscriptionCircleSize;
   
    // Calculate the bounds of the inscription circle.
    float lv_InscriptionCircleY = _Position + m_ChartRect.top +
                                  lv_PieChartSize + ELEMENT_INDENT * 4;
    Gdiplus::RectF lv_InscriptionCircleRect(static_cast<float>(BORDER_INDENT),
                                            lv_InscriptionCircleY,
                                            lv_InscriptionCircleSize, 
                                            lv_InscriptionCircleSize);

    // Calculate the bounds of the pie chart.
    float lv_PieChartX = (m_ChartRect.right + m_ChartRect.left -
                         lv_PieChartSize) / 2;
    float lv_PieChartY = _Position + static_cast<float>(m_ChartRect.top) +
                         ELEMENT_INDENT * 2;
    Gdiplus::RectF lv_PieChartRect(lv_PieChartX, lv_PieChartY,
                                   lv_PieChartSize, lv_PieChartSize);
    
    // Prepare graphic components.
    Gdiplus::Graphics lv_Graphics(m_PanelDC);
    lv_Graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    Gdiplus::SolidBrush lv_SolidBrush(Gdiplus::Color::White);

    // Prepare components for creating inscriptions.
    Gdiplus::FontFamily lv_FontFamily(TEXT("Arial"));
    Gdiplus::Font lv_FontSmall(&lv_FontFamily, 12.0f, Gdiplus::FontStyleBold,
                               Gdiplus::UnitPixel);
    Gdiplus::Font lv_FontBig(&lv_FontFamily, 14.0f, Gdiplus::FontStyleBold,
                             Gdiplus::UnitPixel);   
    Gdiplus::StringFormat lv_StringFormat(Gdiplus::StringFormat::
                                          GenericTypographic());
    lv_StringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    int lv_Index = 0; // Number of iterations 
    float lv_StartAngle = 270.0f;
    for (const auto &it : lv_Channels) {
        // Retrieve the size of the color ratio string for the correct
        // positioning of the inscription.
        double lv_ColorRatio = round((static_cast<double>(it.first) * 1000)) /
                               10;
        auto lv_ColorRatioText = std::to_wstring(lv_ColorRatio).substr(0, 4) +
                                 TEXT("%");
        Gdiplus::PointF lv_PointF(0.0f, 0.0f);
        Gdiplus::RectF  lv_ColorRatioTextBounds;
        lv_Graphics.MeasureString(lv_ColorRatioText.c_str(), -1, &lv_FontBig,
                                  lv_PointF, &lv_ColorRatioTextBounds);

        // Draw an inscription circle.
        lv_InscriptionCircleRect.X = BORDER_INDENT + ((m_ChartRect.right +
                                     m_ChartRect.left - 
                                     lv_InscriptionCircleSize -
                                     lv_ColorRatioTextBounds.Width) / 2  -
                                     BORDER_INDENT) * lv_Index;
        lv_SolidBrush.SetColor(it.second.first);
        lv_Graphics.FillEllipse(&lv_SolidBrush, lv_InscriptionCircleRect);
        ++lv_Index;

        // Draw the inscription of the channel name inside the circle.
        lv_SolidBrush.SetColor(Gdiplus::Color::White);
        lv_StringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
        lv_Graphics.DrawString(it.second.second.c_str(), -1, &lv_FontSmall,
                               lv_InscriptionCircleRect, &lv_StringFormat,
                               &lv_SolidBrush);

        // Draw the inscription of the color ratio near the circle.
        float lv_TextX = lv_InscriptionCircleRect.X +
                         lv_InscriptionCircleRect.Width + ELEMENT_INDENT;
        float lv_TextY = lv_InscriptionCircleRect.Y +
                         lv_InscriptionCircleRect.Height / 2;
        Gdiplus::PointF lv_TextPosition(lv_TextX, lv_TextY);
        lv_StringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        lv_Graphics.DrawString(lv_ColorRatioText.c_str(), -1, &lv_FontBig,
                               lv_TextPosition, &lv_StringFormat,
                               &lv_SolidBrush);

        // Draw a slice of a pie chart.
        float lv_SweepAngle = round(it.first * 360);
        lv_SolidBrush.SetColor(it.second.first);
        lv_Graphics.FillPie(&lv_SolidBrush, lv_PieChartRect, lv_StartAngle,
                            lv_SweepAngle);       
        lv_StartAngle += lv_SweepAngle;
    }

    lv_Channels.clear();

    // Draw a circle to turn a pie chart into a donut .
    lv_SolidBrush.SetColor(Gdiplus::Color(255, 34, 36, 55));
    lv_PieChartRect.Inflate(-lv_PieChartSize / 4, -lv_PieChartSize / 4);
    lv_Graphics.FillEllipse(&lv_SolidBrush, lv_PieChartRect);
}

void ColorData::DrawTextOut(const int _FontHeight, const int _FontWeight,
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

void ColorData::CleanPanel()
{
    BITMAP lv_Bitmap;
    ::GetObject(m_PanelBasicBitmap, sizeof(BITMAP), &lv_Bitmap);
    ::BitBlt(m_PanelDC, 0, 0, lv_Bitmap.bmWidth, lv_Bitmap.bmHeight,
             m_PanelBasicDC, 0, 0, SRCCOPY);
}

POINT ColorData::GetPositionPanel() const
{
    return m_PanelPosition;
}