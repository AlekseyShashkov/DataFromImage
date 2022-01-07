#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <memory>

using BitmapPtr = std::shared_ptr<Gdiplus::Bitmap>;

enum class Controllers : unsigned {
    ID_TEXTBOX_IMAGESUBTITLE = WM_USER + 1U,
    ID_BUTTON_LOADFILE = WM_USER + 2U,
    ID_BUTTON_SAVEFILE = WM_USER + 3U,
    ID_BUTTON_RECOLOR = WM_USER + 4U,
    ID_BUTTON_UPDATE = WM_USER + 5U,
    ID_BUTTON_ERASE = WM_USER + 6U,   
    ID_PROGRESSBAR = WM_USER + 7U,
    ID_SCROLLBAR = WM_USER + 8U
};

class ElementGUI
{
public:
    ElementGUI() noexcept;
    virtual ~ElementGUI() noexcept;

    HDC GetHandlePanel() const noexcept;

// Overrides.
public:
    virtual void ProcessingData(BitmapPtr _image) = 0;
    virtual POINT GetPositionPanel() const noexcept = 0;

protected:
    HDC m_handlePanel;
    HBITMAP m_memoryPanel;
    POINT m_positionPanel;
};