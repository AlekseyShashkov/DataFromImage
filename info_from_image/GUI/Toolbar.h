#pragma once

#include "ElementGUI.h"

class Toolbar : public ElementGUI
{
public:
    Toolbar(const HWND &_hWnd);
    ~Toolbar() noexcept;

// Overrides.
public:
    void ProcessingData(BitmapPtr _image) override final;
    POINT GetPositionPanel() const noexcept override final;

private:
    HWND m_loadFileButton;
    HBITMAP m_logoLoadFileButton;
};