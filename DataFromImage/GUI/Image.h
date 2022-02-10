#pragma once

#include <string>

#include "GUIElement.h"

class Image : public GUIElement
{
public:
    Image(const HWND &_Window);
    ~Image();

public:
    // Overrides.
    void ProcessingData(const BitmapPtr &_Image) override final;
    void CleanPanel() override final;
    POINT GetPositionPanel() const override final;

private:
    void DrawImage(const BitmapPtr &_Image, const float _Ratio);
    void DrawTextOut(const int _FontHeight, const int _FontWeight,
                     const COLORREF _TextColor, const wchar_t *_Text,
                     const int _TextX, const int _TextY);

private:
    RECT m_ImageRect;
};