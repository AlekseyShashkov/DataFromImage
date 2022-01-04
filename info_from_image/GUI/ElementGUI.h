#pragma once

#include <windows.h>

enum class Controllers : unsigned {
    ID_BUTTON_LOADFILE = WM_USER + 1U,
    ID_BUTTON_SAVEFILE = WM_USER + 2U,
    ID_BUTTON_RECOLOR = WM_USER + 3U,
    ID_BUTTON_UPDATE = WM_USER + 4U,
    ID_BUTTON_ERASE = WM_USER + 5U,
    ID_PROGRESSBAR = WM_USER + 6U,
    ID_SCROLLBAR = WM_USER + 7U
};

class ElementGUI
{
public:
    ElementGUI() noexcept;
    virtual ~ElementGUI() noexcept;

    HDC GetHandlePanel() const noexcept;

    virtual POINT GetPositionPanel() const noexcept = 0;
    virtual void CreatePanel(const HWND &_hWnd) = 0;

protected:
    HDC m_handlePanel;
    HBITMAP m_memoryPanel;
    POINT m_positionPanel;
};