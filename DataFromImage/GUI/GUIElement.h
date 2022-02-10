#pragma once

#include <memory>

#include <windows.h>
#include <gdiplus.h>

using BitmapPtr = std::unique_ptr<Gdiplus::Bitmap>;

class GUIElement
{
public:
    GUIElement() = default;
    virtual ~GUIElement();

    HDC GetHandlePanel() const;

public:
    // Overrides.
    virtual void ProcessingData(const BitmapPtr &_BitmapPtr) = 0;
    virtual void CleanPanel() = 0;
    virtual POINT GetPositionPanel() const = 0;

protected:
    static constexpr int ELEMENT_INDENT = 5;
    static constexpr int BORDER_INDENT  = 15;

    HDC m_PanelDC;
    HDC m_PanelBasicDC;
    HBITMAP m_PanelBitmap;
    HBITMAP m_PanelBasicBitmap;
    HBITMAP m_PanelBitmapDefault;
    HBITMAP m_PanelBasicBitmapDefault;
    POINT m_PanelPosition;
};