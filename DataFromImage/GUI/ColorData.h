#pragma once

#include <map>
#include <string>
#include <functional>

#include "GUIElement.h"

class ColorData : public GUIElement
{
public:
    ColorData(const HWND &_Window);
    ~ColorData();

public:
    // Overrides.
    void ProcessingData(const BitmapPtr &_Image) override final;
    void CleanPanel() override final;
    POINT GetPositionPanel() const override final;

private:
    void DrawHistogram(const int *_Channel, const int _AverageValue,
                       const COLORREF _Color, const int _Intensity,
                       const int _Position);
    void DrawPieChart(const int _AverageValueR, const int _AverageValueG,
                      const int _AverageValueB, const int _Position);
    void DrawTextOut(const int _FontHeight, const int _FontWeight,
                     const COLORREF _TextColor, const wchar_t *_Text,
                     const int _TextX, const int _TextY);

private:
    RECT m_ChartRect;
};