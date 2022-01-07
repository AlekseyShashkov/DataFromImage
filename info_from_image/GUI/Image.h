#pragma once

#include "ElementGUI.h"

class Image : public ElementGUI
{
public:
    Image(const HWND &_hWnd);
    ~Image() noexcept;

// Overrides.
public:
    void ProcessingData(BitmapPtr _image) override final;
    POINT GetPositionPanel() const noexcept override final;

private:
    void DrawImage(BitmapPtr _image, const float _ratio);

private:
    RECT m_imageRect;
};